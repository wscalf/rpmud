#include "gtest/gtest.h"

#include <memory.h>

#include "scripting/CommandSystem.h"
#include "scripting/builtin/LookCommand.h"
#include "doubles/FakeClientAdapter.h"
#include "world/Room.h"
#include "world/Player.h"
#include "world/World.h"

class LoaderTest : public testing::Test
{
    protected:
    CommandSystem* comsys;
    FakeClientAdapter* client;
    std::shared_ptr<Player> player;
    World* world;

    void SetUp() override
    {
        comsys = new CommandSystem();
        comsys->add(std::unique_ptr<Command>(new LookCommand()));

        client = new FakeClientAdapter();
        player = std::make_shared<Player>(UUID::create(), "player", std::unique_ptr<ClientAdapter>(client), *comsys);
        world = new World();
    }

    void TearDown() override
    {
        delete world;
        delete comsys;
    }
};

TEST_F(LoaderTest, load_and_explore)
{
    world->load("sample");

    Room& start = world->getStartingRoom();
    start.add(player);

    EXPECT_TRUE(client->hasReceivedOutputContaining(start.getName()));
    EXPECT_TRUE(client->hasReceivedOutputContaining(start.getDescription()));

    client->sendInput("look o");

    EXPECT_TRUE(client->hasReceivedOutputContaining("to the overflow room")); //Sample from the transition
    EXPECT_FALSE(client->hasReceivedOutputContaining("where you can steal away from the crowd")); //Sample from the description of the other room

    client->sendInput("o");

    EXPECT_TRUE(client->hasReceivedOutputContaining("where you can steal away from the crowd")); //Sample from the description of the other room

    client->clearOutput();
    client->sendInput("look back");

    EXPECT_TRUE(client->hasReceivedOutputContaining("Peeking beyond it reveals..")); //Sample from the description of the transition
    EXPECT_TRUE(client->hasReceivedOutputContaining(start.getDescription())); //Sample from the description of the start room
}