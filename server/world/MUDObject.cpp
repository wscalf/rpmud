#include "world/MUDObject.h"

UUID MUDObject::getId()
{
    return this->id;
}

std::string MUDObject::getName()
{
    return this->name;
}

void MUDObject::setName(std::string name)
{
    this->name = name;
}

MUDObject::MUDObject(UUID id)
    : id {id}
{

}

bool MUDObject::operator==(const MUDObject& other)
{
    return id == other.id;
}