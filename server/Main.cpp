#include <iostream>
#include "version.h"

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libtelnet.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <memory>

#include "network/TelnetProtocol.h"

using namespace std;

static const telnet_telopt_t my_telopts[] = {
    { TELNET_TELOPT_ECHO,      TELNET_WILL, TELNET_DONT },
    { TELNET_TELOPT_TTYPE,     TELNET_WILL, TELNET_DONT },
    { TELNET_TELOPT_COMPRESS2, TELNET_WONT, TELNET_DO   },
    { TELNET_TELOPT_ZMP,       TELNET_WONT, TELNET_DO   },
    { TELNET_TELOPT_MSSP,      TELNET_WONT, TELNET_DO   },
    { TELNET_TELOPT_BINARY,    TELNET_WILL, TELNET_DO   },
    { TELNET_TELOPT_NAWS,      TELNET_WILL, TELNET_DONT },
    { -1, 0, 0 }
};

static const char* greeting = "Welcome, client!\n";

void on_connect(evconnlistener* listener, evutil_socket_t descriptor, sockaddr* addr, int socklen, void* data);
void on_buffer_recv(bufferevent *bev, void *data);
void telnet_callback(telnet_t *peer, telnet_event_t *event, void* data);

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

    cout << "Got past start?" << endl;

    sockaddr_in addr;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4000);

    socklen_t size = sizeof(addr);

    event_base *eb = event_base_new();

    evconnlistener *listener = evconnlistener_new_bind(eb, 
        on_connect, 
        eb, 
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 
        5, 
        (sockaddr*)&addr, 
        sizeof(addr));

    event_base_dispatch(eb);
    evconnlistener_free(listener);
    event_base_free(eb);

    cout << "Connection received!" << endl;

    //cin.ignore();
}



//Thought on this design: the order is mixed: libevent calls telnet on receive and telnet calls libevent on send
//Is there a way to stack them so one always calls the other?
//Also, how do we tease this apart into a ClientListener/ClientAdapter pattern? Or should it look different?
//It's definitely an outer-ring concept, but it might be enough that it calls the model without necessarily being injected into it
//Except the model does have to call it sometimes: like when sending

void on_connect(evconnlistener* listener, evutil_socket_t descriptor, sockaddr* addr, int socklen, void* data)
{
    event_base *eb = (event_base*)data;
    bufferevent *bev = bufferevent_socket_new(eb, descriptor, BEV_OPT_CLOSE_ON_FREE);
    telnet_t *peer = telnet_init(my_telopts, telnet_callback, 0, bev);

    bufferevent_setcb(bev, on_buffer_recv, NULL, NULL, peer);
    bufferevent_enable(bev, EV_READ);

    telnet_send(peer, greeting, strlen(greeting));
}

void on_buffer_recv(bufferevent *bev, void *data)
{
    telnet_t *peer = (telnet_t*)data;
    evbuffer *input_buf = bufferevent_get_input(bev);
    size_t length = evbuffer_get_length(input_buf);
    unique_ptr<char[]> dest_buf = make_unique<char[]>(length);
    evbuffer_remove(input_buf, dest_buf.get(), length);
    telnet_recv(peer, "\255\241\r\nTest", 6);
    telnet_recv(peer, dest_buf.get(), length);
}

void telnet_callback(telnet_t *peer, telnet_event_t *event, void* data)
{
    bufferevent *bev = (bufferevent*)data;
    string input;

    switch (event->type)
    {
        case TELNET_EV_DATA: //Note: for large messages over 4096 bytes, this will be called multiple times. This is rare but should be expected in a roleplaying environment. Actually: all libtelnet seems to do is options negotiation. telnet_recv pretty much passes straight into here.
            input = string(event->data.buffer, event->data.size);
            if (input.find("quit") == 0)
                bufferevent_free(bev);
            
            printf("Received: %s\n", input.data());
            break;
        case TELNET_EV_SEND:
            bufferevent_write(bev, event->data.buffer, event->data.size);
            break;
    }
}