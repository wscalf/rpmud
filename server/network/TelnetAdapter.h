#pragma once
#include <functional>
#include <libtelnet.h>
#include <memory>
#include <event2/bufferevent.h>

#include "network/ClientAdapter.h"

class TelnetAdapter : public ClientAdapter
{
    public:
        void sendOutput(std::string text) override;
        explicit TelnetAdapter(bufferevent *bev);
    private:
        std::unique_ptr<bufferevent, void(*)(bufferevent*)> bev;
        std::unique_ptr<telnet_t, void(*)(telnet_t*)> peer;

        static void on_buffer_recv(bufferevent *bev, void *data);
        static void on_buffer_event(bufferevent *bev, short event, void *data);
        static void telnet_callback(telnet_t *peer, telnet_event_t *event, void* data);
};