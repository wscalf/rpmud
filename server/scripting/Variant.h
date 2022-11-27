#pragma once
#include <variant>
#include <string>

class MUDObject;

struct Variant
{
    public:
        enum Type {Object, Text, Number, Boolean, Empty};

        Type getType() const;
        MUDObject* object() const;
        std::string text() const;
        float number() const;
        bool flag() const;

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
        void operator=(const char* txt);
        void operator=(float num);
        void operator=(bool flg);
        ~Variant();
    private:
        std::variant<MUDObject*, std::string, float, bool> _value;
        bool _empty = true;

};