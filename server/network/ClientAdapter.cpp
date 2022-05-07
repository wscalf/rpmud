#include "network/ClientAdapter.h"
#include "util/Log.h"

void ClientAdapter::setCommandHandler(std::function<void(std::string)> handler)
{
    commandHandler = handler;
}

void ClientAdapter::setDisconnectHandler(std::function<void(ClientAdapter*)> handler)
{
    disconnectHandler = handler;
}

void ClientAdapter::sendOutput(std::string output)
{
    if (alive)
        sendOutputInternal(output);
    else
        Log::warn("Output has been discarded due to connection liveliness.", getSessionId());
}

void ClientAdapter::reportConnected()
{
    alive = true;
}

void ClientAdapter::reportDisconnected()
{
    alive = false;
    disconnectHandler(this);
}

UUID ClientAdapter::getSessionId()
{
    return sessionId;
}

ClientAdapter::ClientAdapter()
    : sessionId {UUID::create()}
{

}

ClientAdapter::~ClientAdapter()
{

}