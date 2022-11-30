#include "world/Room.h"

#include <sstream>

#include "world/Player.h"
#include "world/Transition.h"

Room::Room(UUID id, const std::string roomId)
    : MUDObject(id), _roomId {roomId}
{

}

void Room::add(std::shared_ptr<Player> player)
{
    sendToAll(player->getName() + " joined");
    _players.push_back(player);
    player->setRoom(this);

    player->send(describe());
}

void Room::remove(std::shared_ptr<Player> player)
{
    player->setRoom(nullptr);
    _players.remove(player);
    sendToAll(player->getName() + " left");
}

void Room::addLink(std::unique_ptr<Transition> transition)
{
    _transitions.push_back(std::move(transition));
}

void Room::sendToAll(std::string message)
{
    for (auto i = _players.begin(); i != _players.end(); ++i)
    {
        auto player = *i;
        player->send(message);
    }
}

Command* Room::findLocalCommand(std::string_view name)
{
    //NOTE: if this room is destroyed while a command is in progress, we'll have a dangling pointer
    for (auto i = _transitions.begin(); i != _transitions.end(); ++i)
    {
        if ((*i)->getKeyword() == name)
            return (*i).get();
    }

    return nullptr;
}

std::list<std::shared_ptr<Player>> Room::getPlayers()
{
    return _players;
}

std::shared_ptr<Player> Room::findPlayer(std::string_view name)
{
    for (auto i = _players.begin(); i != _players.end(); ++i)
    {
        if ((*i)->getName() == name)
            return *i;
    }

    return nullptr;
}

MUDObject* Room::findObject(std::string_view name)
{
    for (auto i = _transitions.begin(); i != _transitions.end(); ++i)
    {
        if ((*i)->getName() == name || (*i)->getKeyword() == name)
            return (*i).get();
    }

    return nullptr;
}

std::string Room::getRoomId()
{
    return _roomId;
}

std::string Room::describe()
{
    std::stringstream ret;

    ret << getName() << std::endl;
    ret << "----------------" << std::endl;
    ret << getDescription() << std::endl;
    ret << "----------------" << std::endl;
    
    ret << "Players:" << std::endl;
    for (auto i = _players.begin(); i != _players.end(); ++i)
        ret << (*i)->getName() << std::endl;

    ret << "----------------" << std::endl;

    ret << "Exits:" << std::endl;
    for (auto i = _transitions.begin(); i != _transitions.end(); ++i)
        ret << (*i)->getName() << " [" << (*i)->getKeyword() << "]" << std::endl;
    ret << "----------------" << std::endl;

    return ret.str();
}