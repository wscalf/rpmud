#include "TelnetProtocol.h"

#include <event2/bufferevent.h>
#include <event2/event_struct.h>
#include <signal.h>

#include "network/TelnetAdapter.h"
#include "util/Log.h"

void TelnetProtocol::Start()
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    evbase.reset(event_base_new()); //This might need to be promoted to a higher thing. It may be shared across protocols and applies to signals too
    if (!evbase)
    {
        Log::fatal("Failed to initialize libevent");
        return;
    }

    listener.reset(evconnlistener_new_bind(evbase.get(),
        on_connect,
        this,
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
        5,
        (sockaddr*)&addr,
        sizeof(addr)));
    if (!listener)
    {
        Log::errorNo("Failed to listen on port " + std::to_string(port));
        return;
    }

    auto signal_event = evsignal_new(evbase.get(), SIGINT, on_signal, this);
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

    event_base_dispatch(evbase.get());
}

void TelnetProtocol::Stop()
{
    event_base_loopexit(evbase.get(), NULL);
}

TelnetProtocol::~TelnetProtocol()
{

}

void TelnetProtocol::on_connect(evconnlistener*, evutil_socket_t descriptor, sockaddr*, int, void* data)
{
    TelnetProtocol *tp = (TelnetProtocol*)data;
    bufferevent *bev = bufferevent_socket_new(tp->evbase.get(), descriptor, BEV_OPT_CLOSE_ON_FREE);
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