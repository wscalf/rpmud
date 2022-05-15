#pragma once

#include "world/Transition.h"

class Player;
class Room;

class DirectTransition : public Transition
{
    public:
        void innerActivate(std::shared_ptr<Player> player) override;
        std::string describe() override;
        DirectTransition(UUID id, Room& from, Room& to, std::string name, std::string command);
    private:
        Room& _to;
};