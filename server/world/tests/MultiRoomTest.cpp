#include "gtest/gtest.h"

#include "doubles/FakeClientAdapter.h"
#include "world/Player.h"
#include "world/Room.h"
#include "world/DirectTransition.h"

#include "scripting/CommandSystem.h"
#include "scripting/builtin/SayCommand.h"

#include <memory>

class MultiRoomTest : public testing::Test
{
    protected:
        CommandSystem* comsys;
        FakeClientAdapter* playerAConnection;
        FakeClientAdapter* playerBConnection;
        std::shared_ptr<Player> playerA;
        std::shared_ptr<Player> playerB;
        Room* roomA;
        Room* roomB;

    void SetUp() override
    {
        comsys = new CommandSystem();
        comsys->add(std::unique_ptr<Command>(new SayCommand()));
        
        playerAConnection = new FakeClientAdapter();
        playerA = std::make_shared<Player>(UUID::create(), "A", std::unique_ptr<ClientAdapter>(playerAConnection), *comsys);

        playerBConnection = new FakeClientAdapter();
        playerB = std::make_shared<Player>(UUID::create(), "B", std::unique_ptr<ClientAdapter>(playerBConnection), *comsys);

        roomA = new Room(UUID::create());
        roomB = new Room(UUID::create());

        roomA->addLink(std::unique_ptr<Transition>(new DirectTransition(UUID::create(), *roomA, *roomB, "Cross", "go")));
        roomB->addLink(std::unique_ptr<Transition>(new DirectTransition(UUID::create(), *roomB, *roomA, "Cross", "go")));
    }

    void TearDown() override
    {
        delete comsys;
        delete roomA;
        delete roomB;
    }
};

TEST_F(MultiRoomTest, say_does_not_cross_rooms)
{
    roomA->add(playerA);
    roomB->add(playerB);

    playerAConnection->sendInput("say Hello");

    EXPECT_FALSE(playerBConnection->hasReceivedOutputContaining("Hello"));
}

TEST_F(MultiRoomTest, say_does_not_cross_rooms_transitions)
{
    roomA->add(playerA);
    roomA->add(playerB);

    playerAConnection->sendInput("say initial");
    EXPECT_TRUE(playerBConnection->hasReceivedOutputContaining("initial"));

    playerAConnection->sendInput("go");
    playerAConnection->sendInput("say other");
    EXPECT_FALSE(playerBConnection->hasReceivedOutputContaining("other"));

    playerAConnection->sendInput("go");
    playerAConnection->sendInput("say back");
    EXPECT_TRUE(playerBConnection->hasReceivedOutputContaining("back"));
}