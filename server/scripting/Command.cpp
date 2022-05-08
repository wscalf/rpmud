#include "scripting/Command.h"

#include <stdexcept>

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
    setUpParameterPattern();
}

void Command::execute(std::shared_ptr<Player> player, std::string command) const
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

std::map<std::string, BoundParameter> Command::parseCommand(std::string command, Room* room) const
{
    std::smatch match;
    std::map<std::string, BoundParameter> params;
    std::string_view full(command);

    for (auto i = parameterPattern.begin(); i != parameterPattern.end(); ++i)
    {
        if (std::regex_search(command, match, i->regex))
        {
            if (i->isParameter)
            {
                auto text = full.substr(match.position(), match.length());
                params.insert_or_assign(i->name, BoundParameter(text, room));
            }
        }
        else
        {
            if (i->isRequired)
                throw std::invalid_argument(i->errorMsg);
        }
    }

    return params;
}

BoundParameter::BoundParameter(std::string_view text, Room* room)
    : _text {text}, _room {room}
{

}

std::string_view BoundParameter::getText()
{
    return _text;
}

MUDObject* BoundParameter::resolveRoomObject()
{
    return nullptr; //Need search
}

std::shared_ptr<Player> BoundParameter::resolvePlayer()
{
    return nullptr; //Need search
}

Segment SegmentFactory::makeLiteral(std::string text, std::string error)
{
    //text = std::regex_
    return Segment{!error.empty(), false, nullptr, nullptr, std::regex{""}};
}

Segment SegmentFactory::makeShortText(std::string name, std::string error = nullptr)
{

}

Segment SegmentFactory::makeFreeText(std::string name, std::string error)
{

}
