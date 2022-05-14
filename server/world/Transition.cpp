#include "world/Transition.h"
#include "world/Player.h"
#include "world/Room.h"

Transition::Transition(UUID id, Room& from, std::string name, std::string command)
    : MUDObject(id), _from(from), _command(command)
{
    setName(name);
}

std::string Transition::getKeyword() const
{
    return _command;
}

void Transition::activate(std::shared_ptr<Player> player)
{
    innerActivate(player);
}

void Transition::innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>&)
{
    activate(player);
}

void Transition::setUpParameterPattern()
{

}

