#pragma once
#include "scripting/ScriptSystem.h"
#include "duk/duktape.h"

class DukScriptSystem : public ScriptSystem
{
    public:
        DukScriptSystem();
        void initialize(CommandSystem *commandSystem) override;
        virtual void load_module(std::string filePath) override;
        ScriptObject* create_object(std::string type) override;
        ~DukScriptSystem();
    private:
        void create_internal_namespace();
        duk_context *ctx;
};