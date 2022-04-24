#pragma once

class UUID
{
    public:
        static UUID create();
        bool operator==(const UUID& other) const;
        bool operator!=(const UUID& other) const;
        void operator=(const UUID& other);
        UUID(const UUID& other);
    private:
        UUID();
        unsigned char data[16];
};