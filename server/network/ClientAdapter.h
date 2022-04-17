#pragma once
#include <string>
#include <functional>

class ClientAdapter
{
    public:
        void SetCommandHandler(std::function<void(std::string)> handler)
        {
            commandHandler = handler;
        }
        virtual void SendOutput(std::string text) = 0;
    protected:
        std::function<void(std::string)> commandHandler;
};