#pragma once
#include <list>

#include <memory>
#include <functional>
#include "world/MUDObject.h"
#include "world/Transition.h"

class Player;
class Transition;

class Room : public MUDObject
{
    public:
        void sendToAll(std::string message);
        void add(std::shared_ptr<Player> player);
        void remove(std::shared_ptr<Player> player);
        void addLink(std::unique_ptr<Transition> transition);
        std::shared_ptr<Player> findPlayer(std::string_view name);
        Command* findLocalCommand(std::string_view keyword);
        MUDObject* findObject(std::string_view name);
        std::string describe() override;
        std::string getRoomId();
        Room(UUID id, const std::string roomId);
    private:
        std::string _roomId;
        std::list<std::shared_ptr<Player>> players;
        std::list<std::unique_ptr<Transition>> transitions;
};