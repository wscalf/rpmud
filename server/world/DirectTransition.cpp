#include "world/DirectTransition.h"

#include "world/Room.h"

void DirectTransition::activate(Player* player)
{
    from->remove(player);
    to->add(player);
}