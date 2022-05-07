#include <functional>

#include "world/Player.h"
#include "world/Room.h"
#include "network/ClientAdapter.h"
#include "scripting/CommandSystem.h"
#include "util/Log.h"

Player::Player(UUID id, std::string name, unique_ptr<ClientAdapter> adapter, CommandSystem& commands)
    : MUDObject(id), _commandSystem {commands}, _adapter {std::move(adapter)}
{   
    _adapter->setCommandHandler(std::bind(&Player::onCommand, this, std::placeholders::_1));
    _adapter->setDisconnectHandler([this]([[maybe_unused]]ClientAdapter* c) {this->onDisconnect();});

    setName(name);

    Log::info("Player " + getName() + " connected", getSessionId());
}

void Player::send(std::string text)
{
    _adapter->sendOutput(text);
}

Room* Player::getRoom()
{
    return _room;
}

void Player::setRoom(Room* room)
{
    _room = room;
}

UUID Player::getSessionId()
{
    return _adapter->getSessionId();
}

void Player::onDisconnect()
{
    Log::info("Player " + getName() + " disconnected", getSessionId());

    if (_room)
        _room->remove(shared_from_this());
}

void Player::onCommand(std::string command)
{
    _commandSystem.execute(shared_from_this(), command);
}

Player::~Player()
{
    Log::info("Player " + getName() + " is out of the game", getSessionId());
}