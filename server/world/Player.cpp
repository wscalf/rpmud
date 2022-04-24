#include <functional>

#include "world/Player.h"
#include "network/ClientAdapter.h"
#include "scripting/CommandSystem.h"

Player::Player(UUID id, ClientAdapter* adapter, CommandSystem* commands)
    : MUDObject(id), commandSystem {commands}, adapter {adapter}
{   
    adapter->setCommandHandler(std::bind(&CommandSystem::execute, commandSystem, this, std::placeholders::_1));
}

void Player::send(std::string text)
{
    adapter->sendOutput(text);
}

Room* Player::getRoom()
{
    return this->room;
}

void Player::setRoom(Room* room)
{
    this->room = room;
}