#include "world/DirectTransition.h"

#include "world/Room.h"

DirectTransition::DirectTransition(UUID id, Room& from, Room& to, std::string name, std::string command)
    : Transition(id, from, name, command), _to (to)
{

}

std::string DirectTransition::describe()
{
    return _to.describe();
}

void DirectTransition::innerActivate(std::shared_ptr<Player> player)
{
    _from.remove(player);
    _to.add(player);
}