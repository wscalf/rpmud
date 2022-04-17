#include <iostream>
#include "version.h"

#include "network/TelnetProtocol.h"
#include "network/ClientAdapter.h"

using namespace std;

static const char* greeting = "Welcome, client!\n";

void on_command(string command)
{
    cout << "Received: " << command << endl;
}

void on_new_client(ClientAdapter *client)
{
    client->SetCommandHandler(on_command);
    client->SendOutput(greeting);
}

int main(int argc, char** argv) {
    TelnetProtocol *proto = new TelnetProtocol(4000);
    proto->SetConnectionHandler(on_new_client);
    proto->Start();

    cout << "Protocol yielded, shutting down";
}