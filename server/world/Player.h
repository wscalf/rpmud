#pragma once

#include "world/MUDObject.h"
#include <memory>
#include <optional>
#include <functional>

class Room;
class ClientAdapter;
class CommandSystem;

class Player : public MUDObject, public std::enable_shared_from_this<Player>
{
    public:
        explicit Player(UUID id, std::string name, std::unique_ptr<ClientAdapter> adapter, CommandSystem& commands);
        void send(std::string text);
        UUID getSessionId();
        Room* getRoom();
        void setRoom(Room* room);
        ~Player();
    private:
        CommandSystem& _commandSystem;
        std::unique_ptr<ClientAdapter> _adapter;
        Room* _room;
        void onDisconnect();
        void onCommand(std::string command);
};