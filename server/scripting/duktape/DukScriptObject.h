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

        void set(std::string field, MUDObject *object) override;
        void set(std::string field, std::string text) override;
        void set(std::string field, float number) override;        
        MUDObject* get_object(std::string field) override;
        std::string get_text(std::string field) override;
        float get_number(std::string field) override;

        virtual Variant call(std::string method, std::initializer_list<Variant> args) override;
        ~DukScriptObject();
    private:
        void push_this();
        void pop();
        duk_context *ctx;
        UUID id;
        void *duk_obj;
};