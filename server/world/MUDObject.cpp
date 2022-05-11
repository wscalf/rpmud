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

MUDObject::MUDObject(UUID id)
    : _id {id}
{

}

bool MUDObject::operator==(const MUDObject& other)
{
    return _id == other._id;
}

MUDObject::~MUDObject()
{
    
}