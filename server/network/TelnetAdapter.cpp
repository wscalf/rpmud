#include "TelnetAdapter.h"

#include <event2/event.h>
#include <event2/buffer.h>
#include <memory>

#include "util/Log.h"

using namespace std;

static const telnet_telopt_t telnet_opts[] = {
    { TELNET_TELOPT_ECHO,      TELNET_WILL, TELNET_DONT },
    { TELNET_TELOPT_TTYPE,     TELNET_WILL, TELNET_DONT },
    { TELNET_TELOPT_COMPRESS2, TELNET_WONT, TELNET_DO   },
    { TELNET_TELOPT_ZMP,       TELNET_WONT, TELNET_DO   },
    { TELNET_TELOPT_MSSP,      TELNET_WONT, TELNET_DO   },
    { TELNET_TELOPT_BINARY,    TELNET_WILL, TELNET_DO   },
    { TELNET_TELOPT_NAWS,      TELNET_WILL, TELNET_DONT },
    { -1, 0, 0 }
};

TelnetAdapter::TelnetAdapter(bufferevent *bev)
    : bev {bev, bufferevent_free}, peer {NULL, telnet_free}
{
    peer.reset(telnet_init(telnet_opts, telnet_callback, 0, this));
    bufferevent_setcb(bev, on_buffer_recv, NULL, on_buffer_event, this);
    bufferevent_enable(bev, EV_READ);

    reportConnected();
}

void TelnetAdapter::sendOutputInternal(string text)
{
    bufferevent_write(bev.get(), text.data(), text.size());
    bufferevent_write(bev.get(), "\n", 1); //Is this slow?
}

void TelnetAdapter::on_buffer_recv(bufferevent *bev, void *data)
{
    TelnetAdapter* obj = (TelnetAdapter*)data;

    evbuffer *input_buf = bufferevent_get_input(bev);

    size_t length = evbuffer_get_length(input_buf);
    unique_ptr<char[]> dest_buf = make_unique<char[]>(length);
    evbuffer_remove(input_buf, dest_buf.get(), length);

    telnet_recv(obj->peer.get(), dest_buf.get(), length);
}

void TelnetAdapter::on_buffer_event(bufferevent*, short event, void *data)
{
    TelnetAdapter *obj = (TelnetAdapter*)data;
    bool unrecoverable = false;
    if (BEV_EVENT_ERROR & event) //Some other issue?
    {
        Log::errorNo("Error communicating with client.", obj->getSessionId());
        unrecoverable = true;
    }
    
    if (BEV_EVENT_EOF & event)
    {
        unrecoverable = true;
    }

    if (unrecoverable)
    {
        obj->reportDisconnected();
    }
}


void TelnetAdapter::telnet_callback(telnet_t*, telnet_event_t *event, void* data)
{
    TelnetAdapter* obj = (TelnetAdapter*)data;
    string input;
    switch (event->type)
    {
        case TELNET_EV_DATA:
            input = string(event->data.buffer, event->data.size - 2); //Exclude trailing line break
            obj->commandHandler(input);
            break;
        case TELNET_EV_SEND:
            bufferevent_write(obj->bev.get(), event->data.buffer, event->data.size);
            break;
        case TELNET_EV_IAC:
        case TELNET_EV_WILL:
        case TELNET_EV_WONT:
        case TELNET_EV_DO:
        case TELNET_EV_DONT:
        case TELNET_EV_SUBNEGOTIATION:
        case TELNET_EV_COMPRESS:
        case TELNET_EV_ZMP:
        case TELNET_EV_TTYPE:
        case TELNET_EV_ENVIRON:
        case TELNET_EV_MSSP:
        case TELNET_EV_WARNING:
        case TELNET_EV_ERROR:
            break;
    }
}