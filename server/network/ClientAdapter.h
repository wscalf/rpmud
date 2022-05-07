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

        void sendOutput(std::string text);
        virtual void sendOutputInternal(std::string text) = 0;

        ClientAdapter();
        virtual ~ClientAdapter();
    protected:
        void reportDisconnected();
        void reportConnected();
        std::function<void(std::string)> commandHandler;
    private:
        bool alive = false;
        std::function<void(ClientAdapter*)> disconnectHandler;
        UUID sessionId;
};