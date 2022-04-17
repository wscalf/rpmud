#include "ClientAdapter.h"
#include "ClientProtocol.h"

#include <event2/event.h>
#include <event2/listener.h>

class TelnetProtocol : public ClientProtocol
{
    public:
        void Start();
        void Stop();

        TelnetProtocol(int port)
        {
            this->port = port;
        }
        ~TelnetProtocol();

    private:
        int port;
        event_base *evbase;
        evconnlistener *listener;
        static void on_connect(evconnlistener* listener, evutil_socket_t descriptor, sockaddr* addr, int socklen, void* data);
};