#include "TelnetProtocol.h"
#include <event2/bufferevent.h>
#include "TelnetAdapter.h"

//Calling start/stop/start/stop probably won't work like this
void TelnetProtocol::Start()
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port); //Need error checking

    evbase = event_base_new(); //This might need to be promoted to a higher thing. It may be shared across protocols and applies to signals too
    listener = evconnlistener_new_bind(evbase,
        on_connect,
        this,
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
        5,
        (sockaddr*)&addr,
        sizeof(addr));

    event_base_dispatch(evbase);
}

void TelnetProtocol::Stop()
{
    timeval delay {2, 0};
    event_base_loopexit(evbase, &delay);

    evconnlistener_free(listener);
    event_base_free(evbase);
}

TelnetProtocol::~TelnetProtocol()
{
    evconnlistener_free(listener);
    event_base_free(evbase);
}

void TelnetProtocol::on_connect(evconnlistener* listener, evutil_socket_t descriptor, sockaddr* addr, int socklen, void* data)
{
    TelnetProtocol *tp = (TelnetProtocol*)data;
    bufferevent *bev = bufferevent_socket_new(tp->evbase, descriptor, BEV_OPT_CLOSE_ON_FREE);
    
    TelnetAdapter *ta = new TelnetAdapter(bev);

    tp->connectionCallback(ta);
}