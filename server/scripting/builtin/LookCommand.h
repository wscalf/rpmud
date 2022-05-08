#include "scripting/Command.h"

class LookCommand : public Command
{
    protected:
        void innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters) override;
        void setUpParameterPattern() override;
};
