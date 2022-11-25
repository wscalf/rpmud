#include "scripting/Command.h"
#include "scripting/ScriptCommand.h"
#include "scripting/ScriptSystem.h"
#include "scripting/ScriptObject.h"
#include "scripting/Variant.h"

std::string ScriptCommand::getKeyword() const
{
    return _keyword;
}

void ScriptCommand::setKeyword(std::string keyword)
{
    _keyword = keyword;
}

void ScriptCommand::addParameter(Segment *segment)
{
    parameterPattern.push_back(std::unique_ptr<Segment>(segment));
}

ScriptCommand::ScriptCommand(std::string type, ScriptSystem& scriptSystem)
    : _type {type}, _scriptSystem {scriptSystem}
{

}

void ScriptCommand::innerExecute([[maybe_unused]] std::shared_ptr<Player> player, [[maybe_unused]] const std::map<std::string, BoundParameter>& parameters)
{
    auto cmd = std::unique_ptr<ScriptObject>(_scriptSystem.create_object(_type));

    //for (auto i = parameters.begin(); i != parameters.end(); i++)
    //    cmd->set(i->first, std::string(i->second.getText())); //Can string_view convert to cstr instead of explicitly copying?

    cmd->call("Execute");
}

void ScriptCommand::setUpParameterPattern()
{
    //No op- parameters are configured through addParameter
}