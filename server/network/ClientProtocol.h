#include <functional>
#include "network/ClientAdapter.h"

class ClientProtocol
{
    public:
        virtual void setConnectionHandler(std::function<void(ClientAdapter*)> cb)
        {
            connectionCallback = cb;
        }

        virtual void Start() = 0; //This will probably block at first, and I think that's okay, but it probably shouldn't with multiple protocols
        virtual void Stop() = 0;
        virtual ~ClientProtocol()
        {
            
        }
    protected:
        std::function<void(ClientAdapter*)> connectionCallback;
};