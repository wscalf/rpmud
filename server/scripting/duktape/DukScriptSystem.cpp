#include "scripting/duktape/DukScriptSystem.h"
#include "scripting/duktape/DukScriptObject.h"
#include "scripting/duktape/Internals.h"
#include "duk/duktape.h"
#include "stdexcept"

DukScriptSystem::DukScriptSystem()
{
    
}

void DukScriptSystem::initialize()
{
    this->ctx = duk_create_heap_default();
    if (!this->ctx)
        throw std::domain_error("Failed to initialize duk heap.");

    //Set up C function bindings (internal)
    create_internal_namespace();
    //Load core module
    load_module("core.js");
    //Load mud.js (separate call?)
}

void DukScriptSystem::load_module([[maybe_unused]] std::string filePath)
{
    //Todo: read JS file, compile, and run
}

ScriptObject* DukScriptSystem::create_object(std::string type)
{
    return new DukScriptObject(this->ctx, type);
}

void DukScriptSystem::create_internal_namespace()
{
    duk_push_object(this->ctx);
    duk_put_function_list(this->ctx, 1, internals);
    duk_put_global_string(this->ctx, "internal");
}

DukScriptSystem::~DukScriptSystem()
{
    //If debugger is active, disconnect before shutting down
    duk_destroy_heap(this->ctx);
}