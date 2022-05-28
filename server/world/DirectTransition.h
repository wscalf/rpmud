#pragma once

#include <set>

#include "world/Transition.h"

class Player;
class Room;

class DirectTransition : public Transition
{
    public:
        void innerActivate(std::shared_ptr<Player> player) override;
        std::string describe() override;
        bool hasTag(std::string tag);
        void addTag(std::string tag);
        DirectTransition(UUID id, Room& from, std::string name, std::string command);
        void setDestination(Room* to);
    private:
        Room* _to;
        std::set<std::string> _tags;
};