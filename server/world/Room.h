#pragma once
#include <list>

#include "world/MUDObject.h"

class Player;
class Transition;

class Room : public MUDObject
{
    public:
        void sendToAll(std::string message);
        void add(Player* player);
        void remove(Player* player);
        void addLink(Transition* transition);
        Room(UUID id);
    private:
         std::list<Player*> players;
         std::list<Transition*> transitions;
};