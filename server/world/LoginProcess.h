#pragma once

#include <string>
#include <functional>
#include <memory>

class Room;
class ClientAdapter;
class CommandSystem;

class LoginWorker
{
    public:
        LoginWorker(std::unique_ptr<ClientAdapter> adapter, CommandSystem& commandSystem);
        void setCompleteHandler(std::function<void(LoginWorker*)> handler);
        bool isSuccessful();
        Player* createPlayer();
    private:
        std::string name;
        std::unique_ptr<ClientAdapter> adapter;
        CommandSystem& _commandSystem;
        void processCommand(std::string command);
        void processDisconnect();
        std::function<void(LoginWorker*)> completeHandler;
};

class LoginProcess
{
    public:
        void begin(ClientAdapter* adapter);
        LoginProcess(Room& startRoom, CommandSystem& commandSystem);
    private:
        void processLogin(LoginWorker* worker);
        Room& _startRoom;
        CommandSystem& _commandSystem;
};