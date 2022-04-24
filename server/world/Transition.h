#pragma once

#include "world/MUDObject.h"

class Player;
class Room;

class Transition : public MUDObject
{
    public:
        virtual void activate(Player* player) = 0;
        Transition(UUID id, Room* from)
            : MUDObject(id), from(from)
        {

        }
    protected:
        Room* from;
        std::string command;
};