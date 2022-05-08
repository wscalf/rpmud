#include <sstream>

#include "scripting/builtin/SayCommand.h"
#include "world/Player.h"
#include "world/Room.h"

void SayCommand::innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters)
{
    auto text = parameters.at("text")
                        .getText();
    auto room = player->getRoom();

    std::stringstream output;

    output << player->getName();
    output << " says, \"";
    output << text;
    output << "\"";

    room->sendToAll(output.str());
}

void SayCommand::setUpParameterPattern()
{
    setName("say");

    parameterPattern.push_back(std::unique_ptr<Segment>(SegmentFactory::makeFreeText("text", "Say what?")));
}