#include <string>

struct ParseResult
{
    bool matched;
    size_t startIndex;
    std::string_view match;

    static const ParseResult Error;
};

class Segment
{
    public:
        bool isRequired() const;
        bool isParameter() const;
        std::string name() const;
        std::string errorMsg() const;
        virtual ParseResult parseNext(std::string_view remaining) const = 0;
        Segment(std::string name, std::string errMsg);    
        protected:
            size_t findNextWhitespace(std::string_view remaining) const;
            size_t findNextNonwhitespace(std::string_view remaining) const;
    private:
        std::string _name;
        std::string _errMsg;
};

class SegmentFactory
{
    public:
        static Segment* makeLiteral(std::string text, std::string error = "");
        static Segment* makeShortText(std::string name, std::string error = "");
        static Segment* makeFreeText(std::string name, std::string error = "");
};