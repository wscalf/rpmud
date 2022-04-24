#pragma once

#include "world/MUDObject.h"

class Room;
class ClientAdapter;
class CommandSystem;

class Player : public MUDObject
{
    public:
        explicit Player(UUID id, ClientAdapter* adapter, CommandSystem* commands);
        void send(std::string text);
        Room* getRoom();
        void setRoom(Room* room);
        ~Player();
    private:
        CommandSystem* commandSystem;
        ClientAdapter* adapter;
        Room* room;
};