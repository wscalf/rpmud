#include "TelnetProtocol.h"

#include <event2/bufferevent.h>
#include <signal.h>

#include "network/TelnetAdapter.h"
#include "util/Log.h"

//Calling start/stop/start/stop probably won't work like this
void TelnetProtocol::Start()
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port); //Need error checking

    evbase = event_base_new(); //This might need to be promoted to a higher thing. It may be shared across protocols and applies to signals too
    if (!evbase)
    {
        Log::fatal("Failed to initialize libevent");
        return;
    }

    listener = evconnlistener_new_bind(evbase,
        on_connect,
        this,
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
        5,
        (sockaddr*)&addr,
        sizeof(addr));
    if (!listener)
    {
        Log::errorNo("Failed to listen on port " + std::to_string(port));
        return;
    }

    auto signal_event = evsignal_new(evbase, SIGINT, on_signal, this);
    if (!signal_event)
    {
        Log::errorNo("Failed to initialize signal handler.");
        return;
    }
    if (event_add(signal_event, NULL) < 0)
    {
        Log::errorNo("Failed to register signal handler.");
        return;
    }

    event_base_dispatch(evbase);
}

void TelnetProtocol::Stop()
{
    event_base_loopexit(evbase, NULL);
}

TelnetProtocol::~TelnetProtocol()
{
    if (listener)
        evconnlistener_free(listener);
    if (evbase)
        event_base_free(evbase);
}

void TelnetProtocol::on_connect([[maybe_unused]] evconnlistener* listener, evutil_socket_t descriptor, [[maybe_unused]] sockaddr* addr, [[maybe_unused]] int socklen, void* data)
{
    TelnetProtocol *tp = (TelnetProtocol*)data;
    bufferevent *bev = bufferevent_socket_new(tp->evbase, descriptor, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        Log::errorNo("Error opening connection from client.");
        return;
    }
    
    TelnetAdapter *ta = new TelnetAdapter(bev);

    tp->connectionCallback(ta);
}

void TelnetProtocol::on_signal([[maybe_unused]] evutil_socket_t signal, [[maybe_unused]] short events, void* data)
{
    TelnetProtocol *tp = (TelnetProtocol*)data;
    Log::info("Received interrupt signal.");
    tp->Stop();
}