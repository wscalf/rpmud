#include "scripting/Command.h"
#include "scripting/builtin/LookCommand.h"
#include "world/Player.h"
#include "world/Room.h"

std::string LookCommand::getName() const
{
    return "look";
}

void LookCommand::innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters)
{
    MUDObject* target;
    std::shared_ptr<Player> otherPlayer;

    if (parameters.count("name"))
    {
        auto name = parameters.at("name");

        otherPlayer = name.resolvePlayer();
        if (otherPlayer)
            target = otherPlayer.get();
        else
            target = name.resolveRoomObject();        
    }
    else
    {
        target = player->getRoom();
    }

    player->send(target->getName()); //Should be some variant of 'describe'
}

void LookCommand::setUpParameterPattern()
{
    parameterPattern.push_back(std::unique_ptr<Segment>(SegmentFactory::makeLiteral("at")));
    parameterPattern.push_back(std::unique_ptr<Segment>(SegmentFactory::makeShortText("name")));
}