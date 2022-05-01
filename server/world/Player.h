#pragma once

#include "world/MUDObject.h"
#include <memory>
#include <optional>
#include <functional>

class Room;
class ClientAdapter;
class CommandSystem;

class Player : public MUDObject
{
    public:
        explicit Player(UUID id, std::string name, std::unique_ptr<ClientAdapter> adapter, CommandSystem& commands);
        void send(std::string text);
        UUID getSessionId();
        Room& getRoom();
        void setRoom(Room& room);
        void clearRoom();
        ~Player();
    private:
        CommandSystem& _commandSystem;
        std::unique_ptr<ClientAdapter> _adapter;
        std::optional<std::reference_wrapper<Room>> _room;
        void onDisconnect();
};