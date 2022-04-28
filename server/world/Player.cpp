#include <functional>

#include "world/Player.h"
#include "world/Room.h"
#include "network/ClientAdapter.h"
#include "scripting/CommandSystem.h"
#include "util/Log.h"

Player::Player(UUID id, std::string name, ClientAdapter* adapter, CommandSystem* commands)
    : MUDObject(id), commandSystem {commands}, adapter {adapter}
{   
    adapter->setCommandHandler(std::bind(&CommandSystem::execute, commandSystem, this, std::placeholders::_1));
    adapter->setDisconnectHandler([this]([[maybe_unused]]ClientAdapter* c) {this->onDisconnect();});

    setName(name);

    Log::info("Player " + getName() + " connected", getSessionId());
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

UUID Player::getSessionId()
{
    return adapter->getSessionId();
}

void Player::onDisconnect()
{
    if (room)
        room->remove(this);

    Log::info("Player " + getName() + " disconnected", getSessionId());
}

Player::~Player()
{
    delete adapter; //Shouldn't be necessary with smart pointers
}