#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <regex>

class Player;
class Room;
class MUDObject;

struct Segment
{
    bool isRequired;
    bool isParameter;
    std::string name;
    std::string errorMsg;
    std::regex regex;
};

class SegmentFactory
{
    public:
        static Segment makeLiteral(std::string text, std::string error = nullptr);
        static Segment makeShortText(std::string name, std::string error = nullptr);
        static Segment makeFreeText(std::string name, std::string error = nullptr);
};

class BoundParameter
{
    public:
        MUDObject* resolveRoomObject();
        std::shared_ptr<Player> resolvePlayer();
        std::string_view getText();

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
        void execute(std::shared_ptr<Player> player, std::string command) const;
        virtual void innerExecute(std::shared_ptr<Player> player, std::map<std::string, BoundParameter> parameters) const = 0;
    protected:
        void setName(std::string name);
        virtual void setUpParameterPattern() = 0;
    private:
        std::map<std::string, BoundParameter> parseCommand(std::string command, Room* room) const;
        std::vector<Segment> parameterPattern;
        std::string _name;
};