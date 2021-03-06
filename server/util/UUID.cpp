#include <uuid/uuid.h>
#include <memory>

#include "util/UUID.h"

UUID UUID::create()
{
    UUID val;
    uuid_generate(&val.data[0]);
    return val;
}

const UUID UUID::empty = UUID::createEmpty();

UUID::UUID()
{

}

std::string UUID::toStr() const
{
    char buffer[36];
    uuid_unparse_lower(data, &buffer[0]);
    return std::string(&buffer[0], 36);
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

UUID UUID::createEmpty()
{
    UUID val;
    for (auto i = 0; i < 16; i++)
        val.data[i] = 0;

    return val;
}