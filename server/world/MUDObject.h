#pragma once
#include <string>
#include <memory>
#include <util/UUID.h>
#include "scripting/ScriptObject.h"

class MUDObject
{
    public:
        UUID getId();

        std::string getName();
        void setName(std::string name);
        
        void setDescription(std::string description);
        std::string getDescription();
        virtual std::string describe();

        bool hasScript();
        void attachScript(ScriptObject *script);
        ScriptObject* getScript();

        MUDObject(UUID id);
        virtual ~MUDObject();

        bool operator==(const MUDObject& other);
    protected:
        std::unique_ptr<ScriptObject> script;
    private:
        UUID _id;
        std::string _name;
        std::string _description;
};