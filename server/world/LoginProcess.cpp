#include <functional>
#include <iostream>

#include "util/UUID.h"
#include "network/ClientAdapter.h"
#include "world/Room.h"
#include "world/LoginProcess.h"
#include "world/Player.h"
#include "scripting/CommandSystem.h"

static const char* greeting = "Welcome, client!\n";

void LoginProcess::begin(ClientAdapter* adapter)
{

    auto worker = new LoginWorker(std::unique_ptr<ClientAdapter>(adapter), _commandSystem);
    worker->setCompleteHandler(std::bind(&LoginProcess::processLogin, this, std::placeholders::_1));
}

LoginProcess::LoginProcess(Room& startRoom, CommandSystem& commandSystem)
    : _startRoom {startRoom}, _commandSystem {commandSystem}
{

}

void LoginProcess::processLogin(LoginWorker* worker)
{
    if (worker->isSuccessful())
    {
        auto player = std::shared_ptr<Player>(worker->createPlayer()); //TODO: track this object in some sort of manager
        _startRoom.add(player);
    }

    delete worker; //Consider object pool
}


LoginWorker::LoginWorker(std::unique_ptr<ClientAdapter> adapter, CommandSystem& commandSystem)
    : _adapter {std::move(adapter)}, _commandSystem {commandSystem}
{
    _adapter->setCommandHandler(std::bind(&LoginWorker::processCommand, this, std::placeholders::_1));
    _adapter->setDisconnectHandler(std::bind(&LoginWorker::processDisconnect, this));
    _adapter->sendOutput(greeting);
}

void LoginWorker::setCompleteHandler(std::function<void(LoginWorker*)> handler)
{
    this->completeHandler = handler;
}

void LoginWorker::processCommand(std::string command)
{
    if (command == "quit")
    {
        completeHandler(this);
        return;
    }

    name = command;
    completeHandler(this);
}

bool LoginWorker::isSuccessful()
{
    return !name.empty();
}

Player* LoginWorker::createPlayer()
{
    auto player = new Player(UUID::create(), name, std::move(this->_adapter), _commandSystem); //Need access to commandsystem
    player->setName(name);
    return player;
}

void LoginWorker::processDisconnect()
{
    completeHandler(this);
}
