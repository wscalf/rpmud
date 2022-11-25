#include "stdexcept"
#include <fstream>
#include <sstream>
#include "scripting/duktape/DukScriptSystem.h"
#include "scripting/duktape/DukScriptObject.h"
#include "scripting/duktape/Internals.h"
#include "duk/duktape.h"

std::string load_script(std::string filename);

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

void DukScriptSystem::load_module(std::string filePath)
{
    auto source = load_script(filePath);

    duk_push_string(this->ctx, source.c_str());
    duk_push_string(this->ctx, filePath.c_str());
    duk_compile(this->ctx, 0); //Replaces source and filename with a function
    duk_call(this->ctx, 0); //Runs the code to populate global namespace
    duk_pop(this->ctx); //Pop function
}

ScriptObject* DukScriptSystem::create_object(std::string type)
{
    return new DukScriptObject(this->ctx, type);
}

void DukScriptSystem::create_internal_namespace()
{
    duk_push_object(this->ctx);
    duk_put_function_list(this->ctx, 0, internals);
    duk_put_global_string(this->ctx, "internal");
}

DukScriptSystem::~DukScriptSystem()
{
    //If debugger is active, disconnect before shutting down
    duk_destroy_heap(this->ctx);
}

std::string load_script(std::string filename)
{
    std::ifstream file(filename);
    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}