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
    type = Empty;
}

Variant::Variant(const Variant& other)
{
    operator=(other);
}

void Variant::operator=(const Variant& other)
{
    type = other.type;
    
    switch (type)
    {
        case Empty:
            break;
        case Object:
            object = other.object;
            break;
        case Text:
            text = other.text;
            break;
        case Number:
            number = other.number;
            break;
        case Boolean:
            flag = other.flag;
            break;
    }
}

void Variant::operator=(MUDObject* obj)
{
    type = Object;
    object = obj;
}

void Variant::operator=(std::string txt)
{
    type = Text;
    text = txt;
}

void Variant::operator=(float num)
{
    type = Number;
    number = num;
}

void Variant::operator=(bool flg)
{
    type = Boolean;
    flag = flg;
}

Variant::~Variant()
{

}