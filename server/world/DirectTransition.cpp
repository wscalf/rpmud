#include "world/DirectTransition.h"

#include "world/Room.h"
#include "world/Constants.h"

DirectTransition::DirectTransition(UUID id, Room& from, std::string name, std::string command)
    : Transition(id, from, name, command)
{

}

bool DirectTransition::hasTag(std::string tag)
{
    return _tags.count(tag);
}

void DirectTransition::addTag(std::string tag)
{
    _tags.insert(tag);
}

std::string DirectTransition::describe()
{
    std::stringstream ret;
    ret << MUDObject::describe();

    if (_to && hasTag(Tags::Transparent))
        ret << _to->describe();

    return ret.str();
}

void DirectTransition::innerActivate(std::shared_ptr<Player> player)
{
    _from.remove(player);
    _to->add(player);
}

void DirectTransition::setDestination(Room* to)
{
    _to = to;
}