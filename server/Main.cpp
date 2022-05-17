#include <iostream>
#include "version.h"

#include "network/TelnetProtocol.h"
#include "network/ClientAdapter.h"
#include "world/World.h"
#include "world/Room.h"
#include "world/DirectTransition.h"
#include "world/LoginProcess.h"

#include "scripting/CommandSystem.h"
#include "scripting/builtin/SayCommand.h"
#include "scripting/builtin/LookCommand.h"

#include "util/UUID.h"

#include "util/Log.h"

using namespace std;

int main() {
    Log::init(LogLevel::INFO);
    Log::info("Starting up...");
    
    World* world = new World();
    world->load("sample");
    Log::info("World loaded.");
    Room& startingZone = world->getStartingRoom();
    
    CommandSystem *commandSystem = new CommandSystem();
    commandSystem->add(std::unique_ptr<Command>(new SayCommand()));
    commandSystem->add(std::unique_ptr<Command>(new LookCommand()));

    LoginProcess* login = new LoginProcess(startingZone, *commandSystem);
    std::function<void(ClientAdapter*)> handler = std::bind(&LoginProcess::begin, login, std::placeholders::_1);

    TelnetProtocol *proto = new TelnetProtocol(4000);
    proto->setConnectionHandler(handler);
    proto->Start();

    Log::info("Shutting down"); //Might need some way to hold it open until all logs are written. Does this get shown?
    delete proto;
}

