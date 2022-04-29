#pragma once

#include "network/ClientAdapter.h"
#include "network/ClientProtocol.h"

#include <event2/event.h>
#include <event2/listener.h>
#include <memory>

#include <iostream>

class TelnetProtocol : public ClientProtocol
{
    public:
        void Start();
        void Stop();

        TelnetProtocol(int port)
            : port {port}, evbase {NULL, event_base_free}, listener {NULL, evconnlistener_free}
        {

        }
        ~TelnetProtocol();

    private:
        int port;
        std::unique_ptr<event_base, decltype(&event_base_free)> evbase;
        std::unique_ptr<evconnlistener, decltype(&evconnlistener_free)> listener;
        static void on_connect(evconnlistener* listener, evutil_socket_t descriptor, sockaddr* addr, int socklen, void* data);
        static void on_signal(evutil_socket_t signal, short events, void* data);
};