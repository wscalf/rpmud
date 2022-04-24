#include <uuid/uuid.h>

#include "util/UUID.h"

UUID UUID::create()
{
    UUID val;
    uuid_generate(&val.data[0]);
    return val;
}

UUID::UUID()
{

}

bool UUID::operator==(const UUID& other) const
{
    for (auto i = 0; i < 16; i++)
    {
        if (this->data[i] != other.data[i])
            return false;
    }

    return true;
}

bool UUID::operator!=(const UUID& other) const
{
    return !(*this == other);
}

void UUID::operator=(const UUID& other)
{
    for (auto i = 0; i < 16; i++)
        this->data[i] = other.data[i];
}

UUID::UUID(const UUID& other)
{
    *this = other;
}