#include <iostream>
#include "version.h"

#include "network/TelnetProtocol.h"
#include "network/ClientAdapter.h"
#include "world/Room.h"
#include "world/DirectTransition.h"
#include "world/LoginProcess.h"

#include "scripting/CommandSystem.h"
#include "scripting/builtin/SayCommand.h"
#include "scripting/builtin/LookCommand.h"

#include "util/UUID.h"

#include "util/Log.h"

using namespace std;

Room* createWorld()
{
    Room *start = new Room(UUID::create());
    Room *overflow = new Room(UUID::create());
    
    start->addLink(std::unique_ptr<Transition>(new DirectTransition(UUID::create(), *start, *overflow, "Overflow", "o")));
    overflow->addLink(std::unique_ptr<Transition>(new DirectTransition(UUID::create(), *overflow, *start, "Starting Room", "back")));

    return start;
}

int main() {
    Log::init(LogLevel::INFO);
    Log::info("Starting up...");
    Room *startingZone = createWorld();
    
    CommandSystem *commandSystem = new CommandSystem();
    commandSystem->add(std::unique_ptr<Command>(new SayCommand()));
    commandSystem->add(std::unique_ptr<Command>(new LookCommand()));

    LoginProcess* login = new LoginProcess(*startingZone, *commandSystem);
    std::function<void(ClientAdapter*)> handler = std::bind(&LoginProcess::begin, login, std::placeholders::_1);

    TelnetProtocol *proto = new TelnetProtocol(4000);
    proto->setConnectionHandler(handler);
    proto->Start();

    Log::info("Shutting down"); //Might need some way to hold it open until all logs are written. Does this get shown?
    delete proto;
}

