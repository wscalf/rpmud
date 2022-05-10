#pragma once
#include <string>
#include <memory>
#include <map>

#include <scripting/Command.h>

class Player;

class CommandSystem
{
    public:
        void add(std::unique_ptr<Command> command);
        void execute(std::shared_ptr<Player> player, std::string command);
    private:
        std::map<std::string, std::unique_ptr<Command>> commands;
};