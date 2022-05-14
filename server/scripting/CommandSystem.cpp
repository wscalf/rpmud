#include "scripting/CommandSystem.h"

#include "util/Log.h"
#include "scripting/Command.h"
#include "world/Player.h"
#include "world/Room.h"

std::string_view drainKeyword(std::string_view& buffer);

void CommandSystem::execute(std::shared_ptr<Player> player, std::string command)
{
    std::string_view buffer(command);
    auto keyword = std::string(drainKeyword(buffer));

    auto room = player->getRoom();
    if (room)
    {
        auto command = room->findLocalCommand(keyword);
        if (command)
        {
            command->execute(player, buffer);
            return;
        }
    }

    if (commands.count(keyword))
    {
        commands.at(keyword)->execute(player, buffer);
    }
    else
    {
        std::string error = "Unrecognized command: " + keyword;
        Log::debug(error, player->getSessionId());
        player->send(error);
    }
}

void CommandSystem::add(std::unique_ptr<Command> command)
{
    command->init();
    commands.insert_or_assign(command->getKeyword(), std::move(command));
}

std::string_view drainKeyword(std::string_view& buffer)
{
    auto firstSpace = buffer.find(' ');
    if (firstSpace != std::string::npos)
    {
        auto ret = buffer.substr(0, firstSpace);
        buffer = buffer.substr(firstSpace);
        return ret;
    }
    else
    {
        auto ret = buffer;
        buffer = "";
        return ret;
    }
}