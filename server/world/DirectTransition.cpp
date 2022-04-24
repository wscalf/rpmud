#include "world/DirectTransition.h"

void DirectTransition::activate(Player& player)
{
    from.remove(player);
    to.add(player);
}