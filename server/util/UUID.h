#pragma once

#include <string>

class UUID //Need some kind of tostring on this for logging purposes
{
    public:
        static UUID create();
        static const UUID empty;
        bool operator==(const UUID& other) const;
        bool operator!=(const UUID& other) const;
        void operator=(const UUID& other);
        std::string toStr() const;
        UUID(const UUID& other);
    private:
        UUID();
        static UUID createEmpty();
        unsigned char data[16];
};