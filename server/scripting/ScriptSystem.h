#pragma once
#include <string>

class ScriptObject;

class ScriptSystem
{
    public:
        virtual void initialize() = 0;
        virtual void load_module(std::string filePath) = 0;
        virtual ScriptObject* create_object(std::string type) = 0;
        virtual ~ScriptSystem() = 0;
};