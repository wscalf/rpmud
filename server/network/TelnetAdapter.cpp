#include "TelnetAdapter.h"
#include <event2/event.h>
#include <event2/buffer.h>
#include <memory>

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

//TODO: add means to detect client disconnects and to forcefully disconnect clients
//Also need a means of cleaning up objects after disconnection, related to above

TelnetAdapter::TelnetAdapter(bufferevent *bev)
{
    this->bev = bev;
    peer = telnet_init(telnet_opts, telnet_callback, 0, this);
    bufferevent_setcb(bev, on_buffer_recv, NULL, NULL, this);
    bufferevent_enable(bev, EV_READ);
}

void TelnetAdapter::sendOutput(string text)
{
    bufferevent_write(bev, text.data(), text.size());
    bufferevent_write(bev, "\n", 1); //Is this slow?
}

void TelnetAdapter::on_buffer_recv(bufferevent *bev, void *data)
{
    TelnetAdapter* obj = (TelnetAdapter*)data;

    evbuffer *input_buf = bufferevent_get_input(bev);

    size_t length = evbuffer_get_length(input_buf);
    unique_ptr<char[]> dest_buf = make_unique<char[]>(length);
    evbuffer_remove(input_buf, dest_buf.get(), length);

    telnet_recv(obj->peer, dest_buf.get(), length);
}

void TelnetAdapter::telnet_callback(telnet_t *peer, telnet_event_t *event, void* data)
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
            bufferevent_write(obj->bev, event->data.buffer, event->data.size);
            break;
    }
}