#pragma once
#include <string>
#include <memory>

class Player;
class CommandSystem
{
    public:
        void execute(std::shared_ptr<Player> player, std::string command);
};