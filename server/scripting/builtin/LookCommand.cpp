#include "scripting/Command.h"
#include "scripting/builtin/LookCommand.h"
#include "world/Player.h"
#include "world/Room.h"

std::string LookCommand::getKeyword() const
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

    if (target)
    {
        player->send(target->describe());
        if (otherPlayer)
            otherPlayer->send(player->getName() + " looked at you.");
    }
    else
        player->send("I don't see that here");
}

void LookCommand::setUpParameterPattern()
{
    parameterPattern.push_back(std::unique_ptr<Segment>(SegmentFactory::makeLiteral("at")));
    parameterPattern.push_back(std::unique_ptr<Segment>(SegmentFactory::makeShortText("name")));
}