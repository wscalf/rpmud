#include "world/Room.h"
#include "world/Player.h"
#include "world/Transition.h"

Room::Room(UUID id)
    : MUDObject(id)
{

}

void Room::add(Player* player)
{
    sendToAll(player->getName() + " joined");
    players.push_back(player);
    player->setRoom(*this);
}

void Room::remove(Player* player)
{
    player->clearRoom();
    players.remove(player);
    sendToAll(player->getName() + " left");
}

void Room::addLink(Transition* transition)
{
    transitions.push_back(transition);
}

void Room::sendToAll(std::string message)
{
    for (auto i = players.begin(); i != players.end(); ++i)
    {
        (*i)->send(message);
    }
}

bool Room::containsPlayer(UUID id)
{
    for (auto i = players.begin(); i != players.end(); ++i)
        if ((*i)->getId() == id)
            return true;
    return false;
}