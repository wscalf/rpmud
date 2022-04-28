#include "network/ClientAdapter.h"

void ClientAdapter::setCommandHandler(std::function<void(std::string)> handler)
{
    commandHandler = handler;
}

void ClientAdapter::setDisconnectHandler(std::function<void(ClientAdapter*)> handler)
{
    disconnectHandler = handler;
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