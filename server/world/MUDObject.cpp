#include "world/MUDObject.h"

#include <sstream>

UUID MUDObject::getId()
{
    return _id;
}

std::string MUDObject::getName()
{
    return _name;
}

void MUDObject::setName(std::string name)
{
    _name = name;
}

void MUDObject::setDescription(std::string description)
{
    _description = description;
}

std::string MUDObject::getDescription()
{
    return _description;
}

std::string MUDObject::describe()
{
    std::stringstream ret;

    ret << getName() << std::endl;
    ret << "-------------------" << std::endl;
    ret << _description << std::endl;

    return ret.str();
}

bool MUDObject::hasScript()
{
    return this->script != nullptr;
}

void MUDObject::attachScript(ScriptObject *script)
{
    if (this->hasScript())
        throw std::domain_error("A script is already present on object: " + _name);

    this->script = std::unique_ptr<ScriptObject>(script);
}

ScriptObject* MUDObject::getScript()
{
    if (!this->hasScript())
        throw std::domain_error("No script is present on object: " + _name);

    return this->script.get();
}

MUDObject::MUDObject(UUID id)
    : script {nullptr}, _id {id}
{

}

bool MUDObject::operator==(const MUDObject& other)
{
    return _id == other._id;
}

MUDObject::~MUDObject()
{
    
}