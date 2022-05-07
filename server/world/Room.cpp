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
        auto player = (*i).get();
        player->send(message);
    }
}

bool Room::containsPlayer(UUID id)
{
    for (auto i = players.begin(); i != players.end(); ++i)
    {
        auto player = (*i).get();
        if (player->getId() == id)
            return true;
    }
    return false;
}