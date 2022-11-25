#pragma once
#include <string>

class ScriptObject;
class CommandSystem;

class ScriptSystem
{
    public:
        virtual void initialize(CommandSystem *commandSystem) = 0;
        virtual void load_module(std::string filePath) = 0;
        virtual ScriptObject* create_object(std::string type) = 0;
        virtual ~ScriptSystem() = 0;
};