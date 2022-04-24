#pragma once
#include <string>
class Player;
class CommandSystem
{
    public:
        void execute(Player* player, std::string command);
};