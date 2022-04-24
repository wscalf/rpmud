#pragma once
#include <string>
#include <util/UUID.h>

class MUDObject
{
    public:
        UUID getId();

        std::string getName();
        void setName(std::string name);

        MUDObject(UUID id);
    private:
        UUID id;
        std::string name;
};