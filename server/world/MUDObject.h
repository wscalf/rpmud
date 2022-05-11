#pragma once
#include <string>
#include <util/UUID.h>

class MUDObject
{
    public:
        UUID getId();

        std::string getName();
        void setName(std::string name);
        
        void setDescription(std::string description);
        std::string getDescription();
        virtual std::string describe();

        MUDObject(UUID id);
        virtual ~MUDObject();

        bool operator==(const MUDObject& other);
    private:
        UUID _id;
        std::string _name;
        std::string _description;
};