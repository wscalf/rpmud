#pragma once
#include <string>
#include <functional>

class ClientAdapter
{
    public:
        void setCommandHandler(std::function<void(std::string)> handler)
        {
            commandHandler = handler;
        }
        virtual void sendOutput(std::string text) = 0;
    protected:
        std::function<void(std::string)> commandHandler;
};