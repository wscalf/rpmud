#pragma once
#include <string>

class MUDObject;
struct Variant;

class ScriptObject
{
    public:
        virtual void set(std::string field, MUDObject *object) = 0;
        virtual void set(std::string field, std::string text) = 0;
        virtual void set(std::string field, float number) = 0;

        virtual MUDObject* get_object(std::string field) = 0;
        virtual std::string get_text(std::string field) = 0;
        virtual float get_number(std::string field) = 0;

        
        virtual Variant call(std::string method, std::initializer_list<Variant> args = {}) = 0;

        virtual ~ScriptObject() = 0;
};