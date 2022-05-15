#include "scripting/Command.h"

class SayCommand : public Command
{
    public:
        std::string getKeyword() const override;
    protected:
        void innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters) override;
        void setUpParameterPattern() override;
};