#pragma once

#include <string>

class MUDObject;

struct Variant
{
    enum{Empty, Object, Text, Number, Boolean} type;
    union
    {
        MUDObject* object;
        std::string text;
        float number;
        bool flag;
    };

    Variant(MUDObject* obj);
    Variant(const char* txt);
    Variant(std::string txt);
    Variant(float num);
    Variant(bool flg);

    Variant();
    Variant(const Variant& other);
    void operator=(const Variant& other);
    void operator=(MUDObject* obj);
    void operator=(std::string txt);
    void operator=(float num);
    void operator=(bool flg);
    ~Variant();
};