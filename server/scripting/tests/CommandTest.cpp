#include "gtest/gtest.h"
#include "scripting/Command.h"
#include "world/Player.h"
#include "world/Room.h"
#include "util/UUID.h"
#include "scripting/CommandSystem.h"
#include "doubles/FakeClientAdapter.h"

class TestCommand : public Command
{
    public:
        bool wasExecuted()
        {
            return _executed;
        }
        void reset()
        {
            _executed = false;
        }
    protected:
        virtual void innerExecute(std::shared_ptr<Player>, const std::map<std::string, BoundParameter>&) override
        {
            _executed = true;
        }
    private:
        bool _executed = false;
};


class VerbOnlyCommand : public TestCommand
{
    public:
        std::string getName() const override
        {
            return "verb";
        }
    protected:
        void innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters) override
        {
            TestCommand::innerExecute(player, parameters);
        }
        void setUpParameterPattern() override
        {

        }
};

TEST(command, verb_only)
{
    CommandSystem cmds;
    auto player = std::make_shared<Player>(UUID::create(), std::string("name"), std::unique_ptr<ClientAdapter>(new FakeClientAdapter()), cmds);
    
    auto cmd = VerbOnlyCommand();
    cmd.init();
    cmd.execute(player, "verb");

    EXPECT_TRUE(cmd.wasExecuted());
}

class OptionalMiddleCommand : public TestCommand
{
    public:
        std::string getName() const override
        {
            return "look";
        }
        void expect(std::string value)
        {
            _expected = value;
        }
    protected:
        void innerExecute(std::shared_ptr<Player> player, const std::map<std::string, BoundParameter>& parameters) override
        {
            EXPECT_EQ(_expected, parameters.at("name").getText());
            TestCommand::innerExecute(player, parameters);
        }
        void setUpParameterPattern()
        {
            parameterPattern.push_back(std::unique_ptr<Segment>(SegmentFactory::makeLiteral("at")));
            parameterPattern.push_back(std::unique_ptr<Segment>(SegmentFactory::makeShortText("name", "Look at what?")));
        }
    private:
        std::string _expected;
};

TEST(command, optional_middle)
{
    CommandSystem cmds;
    auto player = std::make_shared<Player>(UUID::create(), std::string("name"), std::unique_ptr<ClientAdapter>(new FakeClientAdapter()), cmds);
    
    auto cmd = new OptionalMiddleCommand();
    cmds.add(std::unique_ptr<Command>(cmd));

    cmd->expect("thing");
    
    cmds.execute(player, "look thing");
    EXPECT_TRUE(cmd->wasExecuted());
    cmd->reset();

    cmds.execute(player, "look at thing");
    EXPECT_TRUE(cmd->wasExecuted());
}

TEST(command, quoted_value)
{
    CommandSystem cmds;
    auto player = std::make_shared<Player>(UUID::create(), std::string("name"), std::unique_ptr<ClientAdapter>(new FakeClientAdapter()), cmds);
    
    auto cmd = new OptionalMiddleCommand();
    cmds.add(std::unique_ptr<Command>(cmd));

    cmd->expect("the thing");

    cmds.execute(player, "look at \"the thing\"");
    EXPECT_TRUE(cmd->wasExecuted());
}

TEST(command, invalid_syntax)
{
    CommandSystem cmds;
    auto player = std::make_shared<Player>(UUID::create(), std::string("name"), std::unique_ptr<ClientAdapter>(new FakeClientAdapter()), cmds);
    
    auto cmd = new OptionalMiddleCommand();
    cmds.add(std::unique_ptr<Command>(cmd));
    
    cmds.execute(player, "look");
    EXPECT_FALSE(cmd->wasExecuted());
}