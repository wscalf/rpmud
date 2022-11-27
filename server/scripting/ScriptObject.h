#pragma once
#include <string>

class MUDObject;
struct Variant;

class ScriptObject
{
    public:
        virtual void set(std::string field, Variant value) = 0;
        virtual Variant get(std::string field) = 0;
        
        virtual Variant call(std::string method, std::initializer_list<Variant> args = {}) = 0;

        virtual ~ScriptObject() = 0;
};