#include "scripting/Variant.h"

Variant::Variant(MUDObject* obj)
{
    operator=(obj);
}

Variant::Variant(const char* txt)
{
    operator=(txt);
}

Variant::Variant(std::string txt)
{
    operator=(txt);
}

Variant::Variant(float num)
{
    operator=(num);
}

Variant::Variant(bool flg)
{
    operator=(flg);
}

Variant::Variant()
{

}

Variant::Variant(const Variant& other)
{
    operator=(other);
}

void Variant::operator=(const Variant& other)
{
    if (!(_empty = other._empty))
        _value = other._value;
}

void Variant::operator=(MUDObject* obj)
{
    _empty = false;
    _value = obj;
}

void Variant::operator=(std::string txt)
{
    _empty = false;
    _value = txt;
}

void Variant::operator=(const char* txt)
{
    operator=(std::string(txt));
}

void Variant::operator=(float num)
{
    _empty = false;
    _value = num;
}

void Variant::operator=(bool flg)
{
    _empty = false;
    _value = flg;
}

Variant::Type Variant::getType() const
{
    if (_empty)
        return Empty;
    
    return (Variant::Type)_value.index();
}

MUDObject* Variant::object() const
{
    return std::get<MUDObject*>(_value);
}

std::string Variant::text() const
{
    return std::get<std::string>(_value);
}

float Variant::number() const
{
    return std::get<float>(_value);
}

bool Variant::flag() const
{
    return std::get<bool>(_value);
}

Variant::~Variant()
{

}