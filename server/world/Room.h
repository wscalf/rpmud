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
        //Only intended for unit tests
        bool containsPlayer(UUID id);
        std::shared_ptr<Player> findPlayer(std::string_view name);
        MUDObject* findObject(std::string_view name);
        Room(UUID id);
    private:
         std::list<std::shared_ptr<Player>> players;
         std::list<std::unique_ptr<Transition>> transitions;
};