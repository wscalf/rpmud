#include "doubles/FakeClientAdapter.h"
#include "util/Log.h"

FakeClientAdapter::FakeClientAdapter()
{
    reportConnected();
}

void FakeClientAdapter::sendOutputInternal(std::string text)
{
    outputs.push_back(text);
}

void FakeClientAdapter::sendInput(std::string text)
{
    commandHandler(text);
}

void FakeClientAdapter::disconnect()
{
    reportDisconnected();
}

bool FakeClientAdapter::hasReceivedOutput(std::function<bool(std::string)> predicate)
{
    for (auto it = outputs.begin(); it != outputs.end(); ++it)
    {
        if (predicate(*it))
            return true;
    }

    return false;
}

bool FakeClientAdapter::hasReceivedOutputContaining(std::string sample)
{
    return hasReceivedOutput([sample] (std::string s) {return s.find(sample) != std::string::npos;});
}

FakeClientAdapter::~FakeClientAdapter()
{

}