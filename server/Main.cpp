#include <iostream>
#include "version.h"

#include "network/TelnetProtocol.h"
#include "network/ClientAdapter.h"
#include "world/World.h"
#include "world/Room.h"
#include "world/DirectTransition.h"
#include "world/LoginProcess.h"

#include "scripting/duktape/DukScriptSystem.h"
#include "scripting/CommandSystem.h"
#include "scripting/builtin/SayCommand.h"
#include "scripting/builtin/LookCommand.h"

#include "util/UUID.h"

#include "util/Log.h"

using namespace std;

int main() {
    Log::init(LogLevel::INFO);
    Log::info("Starting up...");
    
    try 
    {
        ScriptSystem* scripting = new DukScriptSystem();
        scripting->initialize();
        Log::info("Script system initialized.");
        World* world = new World();
        world->load("sample/exhibition");
        Log::info("World loaded.");
    
        Room& startingZone = world->getStartingRoom();
        
        CommandSystem *commandSystem = new CommandSystem();
        commandSystem->add(std::unique_ptr<Command>(new SayCommand()));
        commandSystem->add(std::unique_ptr<Command>(new LookCommand()));

        LoginProcess* login = new LoginProcess(startingZone, *commandSystem);
        std::function<void(ClientAdapter*)> handler = std::bind(&LoginProcess::begin, login, std::placeholders::_1);

        auto proto = std::unique_ptr<ClientProtocol>(new TelnetProtocol(4000));
        proto->setConnectionHandler(handler);
        proto->Start();
    }
    catch (std::exception& e)
    {
        Log::fatal("Unhandled exception: " + std::string(e.what()));
    }

    Log::info("Shutting down");
}

