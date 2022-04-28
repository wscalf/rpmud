#include <functional>
#include <iostream>

#include "util/UUID.h"
#include "network/ClientAdapter.h"
#include "world/Room.h"
#include "world/LoginProcess.h"
#include "world/Player.h"

static const char* greeting = "Welcome, client!\n";

void LoginProcess::begin(ClientAdapter* adapter)
{
    auto worker = new LoginWorker(adapter);
    worker->setCompleteHandler(std::bind(&LoginProcess::processLogin, this, std::placeholders::_1));
}

LoginProcess::LoginProcess(Room* startRoom)
    : startRoom {startRoom}
{

}

void LoginProcess::processLogin(LoginWorker* worker)
{
    if (worker->isSuccessful())
    {
        auto player = worker->createPlayer();
        startRoom->add(player);
    }
    else
    {
        delete worker->getClientAdapter();
    }

    delete worker; //Consider object pool
}




LoginWorker::LoginWorker(ClientAdapter* adapter)
    : adapter {adapter}
{
    adapter->setCommandHandler(std::bind(&LoginWorker::processCommand, this, std::placeholders::_1));
    adapter->sendOutput(greeting);
}

void LoginWorker::setCompleteHandler(std::function<void(LoginWorker*)> handler)
{
    this->completeHandler = handler;
}

ClientAdapter* LoginWorker::getClientAdapter()
{
    return this->adapter;
}

void LoginWorker::processCommand(std::string command)
{
    name = command;
    completeHandler(this);
}

bool LoginWorker::isSuccessful()
{
    return true;
}

Player* LoginWorker::createPlayer()
{
    auto player = new Player(UUID::create(), name, this->adapter, nullptr); //Need access to commandsystem
    player->setName(name);
    return player;
}

