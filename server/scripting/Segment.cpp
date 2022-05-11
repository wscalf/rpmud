#include "scripting/Segment.h"

const ParseResult ParseResult::Error {false, std::string::npos, ""};

Segment::Segment(std::string name, std::string errMsg)
    : _name {name}, _errMsg {errMsg}
{

}

bool Segment::isParameter() const
{
    return !_name.empty();
}

bool Segment::isRequired() const
{
    return !_errMsg.empty();
}

std::string Segment::name() const
{
    return _name;
}

std::string Segment::errorMsg() const
{
    return _errMsg;
}

size_t Segment::findNextNonwhitespace(std::string_view buffer) const
{
    for (size_t i = 0; i < buffer.length(); i++)
    {
        if (!isspace(buffer.at(i)))
            return i;
    }

    return std::string::npos;
}

size_t Segment::findNextWhitespace(std::string_view buffer) const
{
    for (size_t i = 0; i < buffer.length(); i++)
    {
        if (isspace(buffer.at(i)))
            return i;
    }

    return std::string::npos;
}

class LiteralSegment : public Segment
{
    public:
        ParseResult parseNext(std::string_view buffer) const override
        {
            auto start = findNextNonwhitespace(buffer);

            if (start == std::string::npos)
                return ParseResult::Error;
            if (start + _literal.size() >= buffer.size())
                return ParseResult::Error; //The text to find won't fit in our bounds

            for (size_t literalIndex = 0; literalIndex < _literal.size(); literalIndex++)
            {
                size_t bufferIndex = start + literalIndex;

                if (_literal.at(literalIndex) != buffer.at(bufferIndex))
                    return ParseResult::Error; //Mismatched values
            }

            return ParseResult {true, start, buffer.substr(start, _literal.size())};
        }
        LiteralSegment(std::string literal, std::string error)
            : Segment("", error), _literal {literal}
        {

        }
    private:
        std::string _literal;
};

class ShortTextSegment : public Segment
{
    public:
        ParseResult parseNext(std::string_view buffer) const override
        {
            auto start = findNextNonwhitespace(buffer);
            if (start == std::string::npos)
                return ParseResult::Error;

            if (buffer.at(start) == QUOTE)
            {
                start += 1;
                auto length = findNextQuote(buffer.substr(start));
                if (length != std::string::npos)
                    return ParseResult {true, start, buffer.substr(start, length)};
                else
                    return ParseResult::Error;
            }
            else
            {
                auto length = findNextWhitespace(buffer.substr(start));
                if (length != std::string::npos)
                    return ParseResult {true, start, buffer.substr(start, length)};
                else
                    return ParseResult {true, start, buffer.substr(start)};
            }
        }

        ShortTextSegment(std::string name, std::string errMsg)
            : Segment(name, errMsg)
        {

        }
    private:
        size_t findNextQuote(std::string_view buffer) const
        {
            for (size_t i = 0; i < buffer.length(); i++)
            {
                if (buffer.at(i) == QUOTE)
                    return i;
            }

            return std::string::npos;
        }
        const char QUOTE = '"';
};

class FreeTextSegment : public Segment
{
    public:
        ParseResult parseNext(std::string_view buffer) const override
        {
            auto start = findNextNonwhitespace(buffer);
            if (start != std::string::npos && start < buffer.length())
            {
                return ParseResult {true, start, buffer.substr(start)};
            }
            else
            {
                return ParseResult::Error;
            }
        }
        FreeTextSegment(std::string name, std::string errMsg)
            : Segment(name, errMsg)
        {

        }
};

Segment* SegmentFactory::makeLiteral(std::string text, std::string error)
{
    return new LiteralSegment(text, error);
}

Segment* SegmentFactory::makeShortText(std::string name, std::string error)
{
    return new ShortTextSegment(name, error);
}

Segment* SegmentFactory::makeFreeText(std::string name, std::string error)
{
    return new FreeTextSegment(name, error);
}
