#include "scripting/Command.h"

#include <stdexcept>
#include <cctype> //for character functions

#include "util/Log.h"
#include "world/Player.h"
#include "world/Room.h"

std::string Command::getName() const
{
    return _name;
}

void Command::setName(std::string name)
{
    _name = name;
}

void Command::init()
{
    parameterPattern.push_back(std::unique_ptr<Segment>(SegmentFactory::makeShortText("cmd")));

    setUpParameterPattern();
}

void Command::execute(std::shared_ptr<Player> player, std::string command)
{
    try
    {
        auto params = parseCommand(command, player->getRoom());
        innerExecute(player, params);
    }
    catch(const std::invalid_argument& e)
    {
        Log::error(e.what(), player->getSessionId());
    }  
}

std::map<std::string, BoundParameter> Command::parseCommand(const std::string& command, Room* room) const
{
    std::smatch match;
    std::map<std::string, BoundParameter> params;
    std::string_view buffer(command);

    for (auto i = parameterPattern.begin(); i != parameterPattern.end(); ++i)
    {
        auto next = (*i)->parseNext(buffer);
        if (next.matched)
        {
            buffer = buffer.substr(next.startIndex + next.match.size());
            params.insert_or_assign((*i)->name(), BoundParameter {next.match, room});
        }
        else
        {
            if ((*i)->isRequired())
            {
                throw std::invalid_argument((*i)->errorMsg());
            }
        }
    }

    return params;
}

BoundParameter::BoundParameter(std::string_view text, Room* room)
    : _text {text}, _room {room}
{

}

std::string_view BoundParameter::getText() const
{
    return _text;
}

MUDObject* BoundParameter::resolveRoomObject() const
{
    return _room->findObject(_text);
}

std::shared_ptr<Player> BoundParameter::resolvePlayer() const
{
    return _room->findPlayer(_text);
}


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
        size_t findNextQuote(string_view buffer) const
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
