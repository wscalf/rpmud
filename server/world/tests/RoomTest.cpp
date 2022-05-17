#include "gtest/gtest.h"

#include "doubles/FakeClientAdapter.h"
#include "world/Player.h"
#include "world/Room.h"
#include "scripting/CommandSystem.h"
#include "util/UUID.h"
#include "scripting/builtin/SayCommand.h"

#include <memory>

class RoomTest : public ::testing::Test
{
    protected:
        CommandSystem* comsys;
        FakeClientAdapter* playerAConnection;
        FakeClientAdapter* playerBConnection;
        std::shared_ptr<Player> playerA;
        std::shared_ptr<Player> playerB;
        Room* room;

    void SetUp() override
    {
        comsys = new CommandSystem();
        comsys->add(std::unique_ptr<Command>(new SayCommand()));
        
        playerAConnection = new FakeClientAdapter();
        playerA = std::make_shared<Player>(UUID::create(), "A", std::unique_ptr<ClientAdapter>(playerAConnection), *comsys);

        playerBConnection = new FakeClientAdapter();
        playerB = std::make_shared<Player>(UUID::create(), "B", std::unique_ptr<ClientAdapter>(playerBConnection), *comsys);

        room = new Room(UUID::create(), "room");
    }

    void TearDown() override
    {
        delete room;
        delete comsys;
    }

};

TEST_F(RoomTest, joining_players_are_announced)
{
    room->add(playerA);
    room->add(playerB);

    EXPECT_TRUE(playerAConnection->hasReceivedOutputContaining(playerB->getName()));
}

TEST_F(RoomTest, room_speech_is_delivered_to_all_players)
{
    room->add(playerA);
    room->add(playerB);

    playerAConnection->sendInput("say Test input");

    EXPECT_TRUE(playerAConnection->hasReceivedOutputContaining("Test input"));
    EXPECT_TRUE(playerBConnection->hasReceivedOutputContaining("Test input"));
}

TEST_F(RoomTest, disconnected_players_are_removed)
{
    room->add(playerA);
    room->add(playerB);

    playerAConnection->disconnect();

    EXPECT_FALSE(room->findPlayer(playerA->getName()));
}

TEST_F(RoomTest, other_players_are_notified_of_dcs)
{
    room->add(playerA);
    room->add(playerB);

    playerAConnection->disconnect();

    EXPECT_TRUE(playerBConnection->hasReceivedOutputContaining("left"));
}