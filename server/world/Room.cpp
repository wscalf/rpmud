#include "world/Room.h"
#include "world/Player.h"
#include "world/Transition.h"

Room::Room(UUID id)
    : MUDObject(id)
{

}

void Room::add(std::shared_ptr<Player> player)
{
    sendToAll(player->getName() + " joined");
    players.push_back(player);
    player->setRoom(this);
}

void Room::remove(std::shared_ptr<Player> player)
{
    player->setRoom(nullptr);
    players.remove(player);
    sendToAll(player->getName() + " left");
}

void Room::addLink(std::unique_ptr<Transition> transition)
{
    transitions.push_back(std::move(transition));
}

void Room::sendToAll(std::string message)
{
    for (auto i = players.begin(); i != players.end(); ++i)
    {
        auto player = *i;
        player->send(message);
    }
}

std::shared_ptr<Player> Room::findPlayer(std::string_view name)
{
    for (auto i = players.begin(); i != players.end(); ++i)
    {
        if ((*i)->getName() == name)
            return *i;
    }

    return nullptr;
}

MUDObject* Room::findObject(std::string_view name)
{
    for (auto i = transitions.begin(); i != transitions.end(); ++i)
    {
        if ((*i)->getName() == name)
            return (*i).get();
    }

    return nullptr;
}

bool Room::containsPlayer(UUID id)
{
    for (auto i = players.begin(); i != players.end(); ++i)
    {
        auto player = *i;
        if (player->getId() == id)
            return true;
    }
    return false;
}