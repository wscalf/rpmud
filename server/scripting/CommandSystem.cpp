#include "scripting/CommandSystem.h"

#include "world/Player.h"
#include "world/Room.h"


void CommandSystem::execute(Player* player, std::string command)
{
    std::string message = player->getName() + " says, \"" + command + "\"";
    
    player->getRoom().sendToAll(message);
}