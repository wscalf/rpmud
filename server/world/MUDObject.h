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

        bool operator==(const MUDObject& other);
    private:
        UUID id;
        std::string name;
};