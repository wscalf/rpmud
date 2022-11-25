#include "duk/duktape.h"
#include <string>
#include "scripting/duktape/DukScriptObject.h"
#include "world/MUDObject.h"
#include "scripting/Variant.h"

void* extract_duk_ptr(MUDObject* object);
Variant get_from_context(duk_context *ctx, duk_idx_t idx);

DukScriptObject::DukScriptObject(duk_context *ctx, std::string type)
    : ctx {ctx}, id {UUID::create()}
{
    //Get constructor from global context
    duk_push_global_stash(this->ctx);
    if (!duk_get_global_string(this->ctx, type.c_str()))
        throw std::domain_error("Unrecognized type: " + type);
    
    //Call constructor to create object
    duk_new(this->ctx, 0);

    //Get heapptr and start sponsorship
    this->duk_obj = duk_get_heapptr(this->ctx, 1);
    duk_put_prop_string(this->ctx, 0, this->id.toStr().c_str());
    
    pop(); //Pop global stash
}

void* DukScriptObject::get_reference()
{
    return this->duk_obj;
}

void DukScriptObject::set(std::string field, MUDObject *object)
{

    auto ptr = extract_duk_ptr(object);

    push_this();
    duk_push_string(this->ctx, field.c_str());
    
    duk_push_heapptr(this->ctx, ptr);
    
    duk_put_prop(this->ctx, 0);

    pop(); //Pop this
}

void DukScriptObject::set(std::string field, std::string text) 
{
    push_this();
    duk_push_string(this->ctx, field.c_str());
    
    duk_push_string(this->ctx, text.c_str());

    duk_put_prop(this->ctx, 0);

    pop(); //Pop this
}

void DukScriptObject::set(std::string field, float number)         
{
    push_this();
    duk_push_string(this->ctx, field.c_str());
    
    duk_push_number(this->ctx, number);

    duk_put_prop(this->ctx, 0);

    pop(); //Pop this
}

MUDObject* DukScriptObject::get_object([[maybe_unused]] std::string field) 
{
    return nullptr; //Requires the item in JS memory to have a pointer back to its object
}

std::string DukScriptObject::get_text(std::string field) 
{
    push_this();
    duk_push_string(this->ctx, field.c_str());
    duk_get_prop(this->ctx, 0);

    std::string ret = duk_get_string(this->ctx, 1);

    pop(); //Pop string
    pop(); //Pop this

    return ret;
}

float DukScriptObject::get_number(std::string field)
{
    push_this();
    duk_push_string(this->ctx, field.c_str());
    duk_get_prop(this->ctx, 0);

    float ret = duk_get_number(this->ctx, 1);

    pop(); //Pop number
    pop(); //Pop this

    return ret;
}

Variant DukScriptObject::call(std::string method, std::initializer_list<Variant> args)
{
    push_this();

    duk_push_string(ctx, method.c_str());

    int num_args = 0;

    for (auto& param : args)
    {
        switch (param.type)
        {
            case param.Object:
                duk_push_heapptr(this->ctx, extract_duk_ptr(param.object));
                break;
            case param.Empty:
                duk_push_null(this->ctx);
                break;
            case param.Text:
                duk_push_string(this->ctx, param.text.c_str());
                break;
            case param.Number:
                duk_push_number(this->ctx, param.number);
                break;
            case param.Boolean:
                duk_push_boolean(this->ctx, param.flag);
                break;
        }

        num_args++;
    }

    duk_call_prop(this->ctx, 0, num_args);

    auto ret = get_from_context(this->ctx, 1);

    pop(); //Pop return value
    pop(); //Pop this

    return ret;
}

void DukScriptObject::push_this()
{
    duk_push_heapptr(this->ctx, duk_obj);
}

void DukScriptObject::pop()
{
    duk_pop(this->ctx);
}

DukScriptObject::~DukScriptObject()
{
    duk_push_global_stash(this->ctx);
    duk_del_prop_string(this->ctx, 0, this->id.toStr().c_str());
    pop(); //Pop global stash
}

Variant get_from_context(duk_context *ctx, duk_idx_t idx)
{
    if (duk_is_string(ctx, idx))
        return duk_get_string(ctx, idx);
    else if (duk_is_number(ctx, idx))
        return (float)duk_get_number(ctx, idx);
    else if (duk_is_boolean(ctx, idx))
        return (bool)duk_get_boolean(ctx, idx);
    else if (duk_is_object(ctx, idx))
        throw std::domain_error("Not implemented"); //Will need to access the native_ptr field to unwind back to an object
    else if (duk_is_null(ctx, idx) || duk_is_undefined(ctx, idx))
        return Variant();
    else
        return Variant();
}

void* extract_duk_ptr(MUDObject* object)
{
    if (!object->hasScript())
        throw std::domain_error("No script attached to object.");
    
    auto script = (DukScriptObject*)object->getScript();

    return script->get_reference();
}