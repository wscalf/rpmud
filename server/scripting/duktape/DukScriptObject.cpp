#include "duk/duktape.h"
#include <string>
#include "scripting/duktape/DukScriptObject.h"
#include "world/MUDObject.h"
#include "scripting/Variant.h"
#include <iostream>
#include <cassert>

void* extract_duk_ptr(MUDObject* object);
Variant get_from_context(duk_context *ctx, duk_idx_t idx);

#define ASSERT_EMPTY assert (duk_get_top(this->ctx) == 0) ;

std::string get_stack(duk_context *ctx)
{
    duk_push_context_dump(ctx);
    std::string ret = duk_get_string(ctx, -1);
    duk_pop(ctx); //Pop dump
    
    return ret;
}

DukScriptObject::DukScriptObject(duk_context *ctx, std::string type)
    : ctx {ctx}, id {UUID::create()}, _type {type}
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
    ASSERT_EMPTY;
}

void* DukScriptObject::get_duk_reference()
{
    return this->duk_obj;
}

void DukScriptObject::set(std::string field, Variant value)
{
    push_this();

    duk_push_string(this->ctx, field.c_str()); //From push_this to pop could be broken out into a helper function for bulk sets. The switch could be broken out for reuse
    
    switch (value.getType())
    {
        case Variant::Text:
            duk_push_string(this->ctx, value.text().c_str());
            break;
        case Variant::Number:
            duk_push_number(this->ctx, value.number());
            break;
        case Variant::Boolean:
            duk_push_boolean(this->ctx, value.flag());
            break;
        case Variant::Object:
            duk_push_heapptr(this->ctx, extract_duk_ptr(value.object()));
            break;
        case Variant::Empty:
            duk_push_null(this->ctx);
            break;
    }
    
    duk_put_prop(this->ctx, 0);

    pop(); //Pop this

    ASSERT_EMPTY;
}

Variant DukScriptObject::get(std::string field) 
{
    Variant ret;

    push_this();
    duk_get_prop_string(this->ctx, 0, field.c_str());
    if (duk_is_undefined(this->ctx, -1))
    {
        pop(); //Pop value
        pop(); //Pop this
        ASSERT_EMPTY;
        throw std::domain_error("Unrecognized property " + field + " of type: " + _type);
    }
    ret = get_from_context(this->ctx, -1);
    pop(); //Pop value
    pop(); //Pop this

    ASSERT_EMPTY;

    return ret;
}

Variant DukScriptObject::call(std::string method, std::initializer_list<Variant> args)
{
    push_this();

    duk_push_string(ctx, method.c_str());

    int num_args = 0;

    for (auto& param : args)
    {
        switch (param.getType())
        {
            case param.Object:
                duk_push_heapptr(this->ctx, extract_duk_ptr(param.object()));
                break;
            case param.Empty:
                duk_push_null(this->ctx);
                break;
            case param.Text:
                duk_push_string(this->ctx, param.text().c_str());
                break;
            case param.Number:
                duk_push_number(this->ctx, param.number());
                break;
            case param.Boolean:
                duk_push_boolean(this->ctx, param.flag());
                break;
        }

        num_args++;
    }

    duk_call_prop(this->ctx, 0, num_args);

    auto ret = get_from_context(this->ctx, 1);

    pop(); //Pop return value
    pop(); //Pop this

    ASSERT_EMPTY;

    return ret;
}

void DukScriptObject::setHandle(void* handle)
{
    push_this();
    duk_push_pointer(this->ctx, handle);
    duk_put_prop_string(this->ctx, 0, "native_ptr");
    pop();

    ASSERT_EMPTY;
}

void* DukScriptObject::getHandle()
{
    push_this();
    duk_get_prop_string(this->ctx, 0, "native_ptr");
    auto ret = duk_get_pointer(this->ctx, 1);
    pop();
    pop();

    ASSERT_EMPTY;

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
    std::cout << "Deleting object of type: " << _type << std::endl;
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
    {
        if (!duk_get_prop_string(ctx, idx, "native_ptr"))
            throw std::domain_error("No native_ptr on referenced object. Does it extend MUDObject?");

        auto ptr = (MUDObject*)duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        return Variant(ptr);
    }
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

    return script->get_duk_reference();
}

