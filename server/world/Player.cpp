#include <functional>

#include "world/Player.h"
#include "world/Room.h"
#include "network/ClientAdapter.h"
#include "scripting/CommandSystem.h"
#include "util/Log.h"

Player::Player(UUID id, std::string name, unique_ptr<ClientAdapter> adapter, CommandSystem& commands)
    : MUDObject(id), _commandSystem {commands}, _adapter {std::move(adapter)}
{   
    _adapter->setCommandHandler(std::bind(&CommandSystem::execute, _commandSystem, this, std::placeholders::_1));
    _adapter->setDisconnectHandler([this]([[maybe_unused]]ClientAdapter* c) {this->onDisconnect();});

    setName(name);

    Log::info("Player " + getName() + " connected", getSessionId());
}

void Player::send(std::string text)
{
    _adapter->sendOutput(text);
}

Room& Player::getRoom()
{
    return _room.value();
}

void Player::setRoom(Room& room)
{
    _room = room;
}

void Player::clearRoom()
{
    _room.reset();
}

UUID Player::getSessionId()
{
    return _adapter->getSessionId();
}

void Player::onDisconnect()
{
    if (_room.has_value())
        _room.value().get().remove(this);

    Log::info("Player " + getName() + " disconnected", getSessionId());
}

Player::~Player()
{

}