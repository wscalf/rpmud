#include <iostream>
#include "version.h"

#include "network/TelnetProtocol.h"
#include "network/ClientAdapter.h"
#include "world/Room.h"
#include "world/LoginProcess.h"
#include "util/UUID.h"

using namespace std;

Room* createWorld()
{
    Room *start = new Room(UUID::create());
    return start;
}

int main(int argc, char** argv) {
    Room *startingZone = createWorld();
    LoginProcess* login = new LoginProcess(startingZone);
    std::function<void(ClientAdapter*)> handler = std::bind(&LoginProcess::begin, login, std::placeholders::_1);

    TelnetProtocol *proto = new TelnetProtocol(4000);
    proto->setConnectionHandler(handler);
    proto->Start();

    cout << "Protocol yielded, shutting down";

    delete proto;
}

