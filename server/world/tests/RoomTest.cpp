#include "gtest/gtest.h"

#include "doubles/FakeClientAdapter.h"
#include "world/Player.h"
#include "world/Room.h"
#include "scripting/CommandSystem.h"
#include "util/UUID.h"

#include <memory>

class RoomTest : public ::testing::Test
{
    protected:
        CommandSystem* comsys;
        FakeClientAdapter* playerAConnection;
        FakeClientAdapter* playerBConnection;
        Player* playerA;
        Player* playerB;
        Room* room;

    void SetUp() override
    {
        comsys = new CommandSystem();
        
        playerAConnection = new FakeClientAdapter();
        playerA = new Player(UUID::create(), "A", std::unique_ptr<ClientAdapter>(playerAConnection), *comsys);

        playerBConnection = new FakeClientAdapter();
        playerB = new Player(UUID::create(), "B", std::unique_ptr<ClientAdapter>(playerBConnection), *comsys);

        room = new Room(UUID::create());
    }

    void TearDown() override
    {
        delete room;
        delete playerA;
        delete playerB;
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

    EXPECT_FALSE(room->containsPlayer(playerA->getId()));
}

TEST_F(RoomTest, other_players_are_notified_of_dcs)
{
    room->add(playerA);
    room->add(playerB);

    playerAConnection->disconnect();

    EXPECT_TRUE(playerBConnection->hasReceivedOutputContaining("left"));
}