#pragma once

#include <string>
#include <functional>

class Room;
class ClientAdapter;

class LoginWorker
{
    public:
        LoginWorker(ClientAdapter* adapter);
        void setCompleteHandler(std::function<void(LoginWorker*)> handler);

        ClientAdapter* getClientAdapter();
        bool isSuccessful();
        Player* createPlayer();
    private:
        std::string name;
        ClientAdapter* adapter;
        void processCommand(std::string command);
        std::function<void(LoginWorker*)> completeHandler;
};

class LoginProcess
{
    public:
        void begin(ClientAdapter* adapter);
        LoginProcess(Room* startRoom);
    private:
        void processLogin(LoginWorker* worker);
        Room* startRoom;
};