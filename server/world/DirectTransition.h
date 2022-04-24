#pragma once

#include "world/Transition.h"

class Player;
class Room;

class DirectTransition : public Transition
{
    public:
        void activate(Player* player);
        DirectTransition(Room* from, Room* to);
    private:
        Room* to;
};