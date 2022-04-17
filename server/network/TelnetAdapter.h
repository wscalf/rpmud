#pragma once
#include <functional>
#include <libtelnet.h>
#include <event2/bufferevent.h>

#include "ClientAdapter.h"

class TelnetAdapter : public ClientAdapter
{
    public:
        void SendOutput(std::string text);
        TelnetAdapter(bufferevent *bev);
    private:
        telnet_t *peer;
        bufferevent *bev;

        static void on_buffer_recv(bufferevent *bev, void *data);
        static void telnet_callback(telnet_t *peer, telnet_event_t *event, void* data);
};