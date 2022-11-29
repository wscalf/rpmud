#include "gtest/gtest.h"
#include "scripting/duktape/DukScriptObject.h"
#include "scripting/duktape/DukScriptSystem.h"
#include "scripting/CommandSystem.h"
#include "world/Room.h"
#include "scripting/Variant.h"

class DukScriptObjectTest : public ::testing::Test
{    
    protected:
        static DukScriptSystem* _scripting;
        static CommandSystem* _comsys;

        static void SetUpTestSuite()
        {
            _comsys = new CommandSystem();
            _scripting = new DukScriptSystem();
            _scripting->initialize(_comsys);
            _scripting->load_module("scripting/tests/test_scripts/testing.js");
        }
        static void TearDownTestSuite()
        {
            delete _scripting;
            delete _comsys;
        }

};

DukScriptSystem* DukScriptObjectTest::_scripting = nullptr;
CommandSystem* DukScriptObjectTest::_comsys = nullptr;

TEST_F(DukScriptObjectTest, property_gets_and_sets)
{
    auto object = std::unique_ptr<ScriptObject>(_scripting->create_object("Properties"));

    object->set("SimpleString", "foo");
    Variant result = object->get("SimpleString");
    EXPECT_EQ(result.getType(), Variant::Text);
    EXPECT_EQ(result.text(), "foo");

    result = object->get("SimpleStringWithDefault");
    EXPECT_EQ(result.getType(), Variant::Text);
    EXPECT_EQ(result.text(), "default");

    object->set("SimpleStringWithDefault", "changed");
    result = object->get("SimpleStringWithDefault");
    EXPECT_EQ(result.getType(), Variant::Text);
    EXPECT_EQ(result.text(), "changed");

    result = object->get("ReadOnlyString");
    EXPECT_EQ(result.getType(), Variant::Text);
    EXPECT_EQ(result.text(), "readonly");

    object->set("SimpleNumber", (float)42);
    result = object->get("SimpleNumber");
    EXPECT_EQ(result.getType(), Variant::Number);
    EXPECT_EQ(result.number(), (float)42);

    result = object->get("SimpleNumberWithDefault");
    EXPECT_EQ(result.getType(), Variant::Number);
    EXPECT_EQ(result.number(), (float)42);

    object->set("SimpleNumberWithDefault", (float)24);
    result = object->get("SimpleNumberWithDefault");
    EXPECT_EQ(result.getType(), Variant::Number);
    EXPECT_EQ(result.number(), (float)24);

    result = object->get("ReadOnlyNumber");
    EXPECT_EQ(result.getType(), Variant::Number);
    EXPECT_EQ(result.number(), (float)0);

    object->set("SimpleBool", true);
    result = object->get("SimpleBool");
    EXPECT_EQ(result.getType(), Variant::Boolean);
    EXPECT_EQ(result.flag(), true);

    result = object->get("SimpleBoolWithDefault");
    EXPECT_EQ(result.getType(), Variant::Boolean);
    EXPECT_EQ(result.flag(), true);

    object->set("SimpleBoolWithDefault", false);
    result = object->get("SimpleBoolWithDefault");
    EXPECT_EQ(result.getType(), Variant::Boolean);
    EXPECT_EQ(result.flag(), false);

    result = object->get("ReadOnlyBool");
    EXPECT_EQ(result.getType(), Variant::Boolean);
    EXPECT_EQ(result.flag(), true);

    object->set("_HiddenString", "hidden");
    result = object->get("_HiddenString");
    EXPECT_EQ(result.getType(), Variant::Text);
    EXPECT_EQ(result.text(), "hidden");

    EXPECT_THROW(object->get("UndefinedProperty"), std::domain_error);
}

TEST_F(DukScriptObjectTest, method_calls)
{
    auto object = std::unique_ptr<ScriptObject>(_scripting->create_object("Properties"));

    auto initial = object->get("ReadOnlyNumber").number();
    object->call("Increment");
    auto updated = object->get("ReadOnlyNumber").number();
    EXPECT_EQ(updated, initial + 1);

    auto result = object->call("Sum", {(float)20, (float)22});
    EXPECT_EQ(result.getType(), Variant::Number);
    EXPECT_EQ(result.number(), 42);
}

TEST_F(DukScriptObjectTest, object_interaction)
{
    auto obj = std::unique_ptr<MUDObject>(new MUDObject(UUID::create()));
    auto script = _scripting->create_object("CustomRoom");
    obj->attachScript(script);

    auto processor = _scripting->create_object("RoomProcessor");

    auto result = processor->call("Process", {obj.get()}).object();

    EXPECT_EQ(obj->getId(), result->getId());
    EXPECT_EQ(3, result->getScript()->get("Count").number());
}

TEST_F(DukScriptObjectTest, world_object_id)
{
    auto obj = std::unique_ptr<MUDObject>(new MUDObject(UUID::create()));
    auto script = _scripting->create_object("MUDObject");
    obj->attachScript(script);

    EXPECT_EQ(obj->getId().toStr(), script->get("Id").text());
}


TEST_F(DukScriptObjectTest, world_object_name)
{
    auto obj = std::unique_ptr<MUDObject>(new MUDObject(UUID::create()));
    auto script = _scripting->create_object("MUDObject");
    obj->attachScript(script);

    obj->setName("Initial");
    EXPECT_EQ("Initial", script->get("Name").text());

    script->set("Name", "Updated");
    EXPECT_EQ("Updated", obj->getName());
}