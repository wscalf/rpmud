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

void Command::init()
{
    setUpParameterPattern();
}

void Command::execute(std::shared_ptr<Player> player, std::string_view command)
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

std::map<std::string, BoundParameter> Command::parseCommand(std::string_view buffer, Room* room) const
{
    std::smatch match;
    std::map<std::string, BoundParameter> params;

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