#pragma once

#include "scripting/ScriptObject.h"
#include "util/UUID.h"
#include "string"
#include "duk/duktape.h"

class MUDObject;

class DukScriptObject : public ScriptObject
{
    public:
        DukScriptObject(duk_context *ctx, std::string type);
        
        void* get_reference();

        void set(std::string field, Variant value) override;
        Variant get(std::string field) override;

        virtual Variant call(std::string method, std::initializer_list<Variant> args) override;
        ~DukScriptObject();
    private:
        void push_this();
        void pop();
        duk_context *ctx;
        UUID id;
        std::string _type;
        void *duk_obj;
};