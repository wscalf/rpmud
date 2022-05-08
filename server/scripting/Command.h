#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <regex>

class Player;
class Room;
class MUDObject;

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

class BoundParameter
{
    public:
        MUDObject* resolveRoomObject() const;
        std::shared_ptr<Player> resolvePlayer() const;
        std::string_view getText() const;

        BoundParameter(std::string_view text, Room* room);
    private:
        std::string_view _text;
        Room* _room;
};

class Command
{
    public:
        std::string getName() const;
        void init();
        void execute(std::shared_ptr<Player> player, std::string command);
    protected:
        virtual void innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters) = 0;
        void setName(std::string name);
        virtual void setUpParameterPattern() = 0;
        std::vector<std::unique_ptr<Segment>> parameterPattern;
    private:
        std::map<std::string, BoundParameter> parseCommand(const std::string& command, Room* room) const;
        std::string _name;
};