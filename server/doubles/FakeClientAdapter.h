#pragma once

#include "network/ClientAdapter.h"
#include <list>
#include <string>
#include <functional>

class FakeClientAdapter : public ClientAdapter
{
    public:
        void sendOutputInternal(std::string text) override;
        void sendInput(std::string text);
        void disconnect();
        bool hasReceivedOutput(std::function<bool(std::string)> predicate);
        bool hasReceivedOutputContaining(std::string sample);
        void clearOutput();
        FakeClientAdapter();
        ~FakeClientAdapter() override;
    private:
        std::list<std::string> outputs;
};