#pragma once

#include "world/MUDObject.h"
#include "scripting/Command.h"

class Player;
class Room;

class Transition : public MUDObject, public Command
{
    public:
        void activate(std::shared_ptr<Player> player);
        virtual void innerActivate(std::shared_ptr<Player> player) = 0;
        std::string getKeyword() const override;
        Transition(UUID id, Room& from, std::string name, std::string command);
    protected:
        void innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters) override;
        void setUpParameterPattern() override;
        Room& _from;
    private:
        std::string _command;
};