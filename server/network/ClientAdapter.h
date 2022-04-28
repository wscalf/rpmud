#pragma once
#include <string>
#include <util/UUID.h>
#include <functional>

class ClientAdapter
{
    public:
        void setCommandHandler(std::function<void(std::string)> handler);
        void setDisconnectHandler(std::function<void(ClientAdapter*)> handler);
        UUID getSessionId();

        virtual void sendOutput(std::string text) = 0;

        ClientAdapter();
        virtual ~ClientAdapter();
    protected:
        std::function<void(std::string)> commandHandler;
        std::function<void(ClientAdapter*)> disconnectHandler;
    private:
        UUID sessionId;
};