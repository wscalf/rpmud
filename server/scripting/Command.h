#pragma once

#include "scripting/Segment.h"

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <regex>

class Player;
class Room;
class MUDObject;
class Segment;

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
        virtual std::string getKeyword() const = 0;
        void init();
        void execute(std::shared_ptr<Player> player, std::string_view command);
    protected:
        virtual void innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters) = 0;
        virtual void setUpParameterPattern() = 0;
        std::vector<std::unique_ptr<Segment>> parameterPattern;
    private:
        std::map<std::string, BoundParameter> parseCommand(std::string_view command, Room* room) const;
        std::string _name;
};