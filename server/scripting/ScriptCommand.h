#pragma once
#include "scripting/Command.h"

class ScriptSystem;

class ScriptCommand : public Command
{
    public:
        std::string getKeyword() const override;
        void setKeyword(std::string keyword);
        void addParameter(Segment *segment);
        ScriptCommand(std::string type, ScriptSystem& scriptSystem);
    protected:
        void innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters) override;
        void setUpParameterPattern() override;
    private:
        std::string _keyword;
        std::string _type;
        ScriptSystem& _scriptSystem;
};