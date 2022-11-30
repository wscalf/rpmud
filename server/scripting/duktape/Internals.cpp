#include "scripting/duktape/DukScriptSystem.h"
#include "scripting/duktape/DukScriptObject.h"
#include "scripting/duktape/Internals.h"
#include "scripting/ScriptCommand.h"
#include "scripting/CommandSystem.h"

#include "world/MUDObject.h"
#include "world/Player.h"
#include "world/Room.h"

#include "util/Log.h"

#define ASSERT_NOT_NULL(obj) if (obj == nullptr) {return DUK_ERR_REFERENCE_ERROR;}

void* unpack_duk_pointer(MUDObject* obj)
{
    auto script = (DukScriptObject*)obj->getScript();
    
    return script->get_duk_reference();
}

int log_debug(duk_context *ctx)
{
    std::string message = duk_get_string(ctx, 0);
    Log::debug(message);

    return 0;
}

int log_info(duk_context *ctx)
{
    std::string message = duk_get_string(ctx, 0);
    Log::info(message);

    return 0;
}

int log_warn(duk_context *ctx)
{
    std::string message = duk_get_string(ctx, 0);
    Log::warn(message);

    return 0;
}

int log_error(duk_context *ctx)
{
    std::string message = duk_get_string(ctx, 0);
    Log::error(message);

    return 0;
}

int log_fatal(duk_context *ctx)
{
    std::string message = duk_get_string(ctx, 0);
    Log::fatal(message);

    return 0;
}

int object_get_id(duk_context *ctx)
{
    auto object = (MUDObject*)duk_get_pointer(ctx, 0);
    ASSERT_NOT_NULL(object);
    auto id = object->getId();

    duk_push_string(ctx, id.toStr().c_str()); //Note: this might deallocate when leaving scope.

    return 1;
}

int object_get_name(duk_context *ctx)
{
    auto object = (MUDObject*)duk_get_pointer(ctx, 0);
    ASSERT_NOT_NULL(object);
    auto name = object->getName();

    duk_push_string(ctx, name.c_str()); //Note: this might deallocate when leaving scope.

    return 1;
}

int object_set_name(duk_context *ctx)
{
    auto object = (MUDObject*)duk_get_pointer(ctx, 0);
    ASSERT_NOT_NULL(object);
    auto name = duk_get_string(ctx, 1);

    object->setName(name);

    return 0;
}

int object_describe(duk_context *ctx)
{
    auto object = (MUDObject*)duk_get_pointer(ctx, 0);
    ASSERT_NOT_NULL(object);

    auto desc = object->describe();

    duk_push_string(ctx, desc.c_str());
    return 1;
}

int player_send(duk_context *ctx)
{
    auto player = (Player*)duk_get_pointer(ctx, 0);
    ASSERT_NOT_NULL(player);
    auto text = duk_get_string(ctx, 1);

    player->send(text);

    return 0;
}

int player_get_room(duk_context *ctx)
{
    auto player = (Player*)duk_get_pointer(ctx, 0);
    ASSERT_NOT_NULL(player);
    auto room = player->getRoom();

    duk_push_heapptr(ctx, unpack_duk_pointer(room));

    return 1;
}

int room_send_to_all(duk_context *ctx)
{
    auto room = (Room*)duk_get_pointer(ctx, 0);
    ASSERT_NOT_NULL(room);
    auto text = duk_get_string(ctx, 1);

    room->sendToAll(text);

    return 0;
}

int room_get_players(duk_context *ctx)
{
    auto room = (Room*)duk_get_pointer(ctx, 0);
    ASSERT_NOT_NULL(room);

    auto idx = duk_push_array(ctx);

    auto players = room->getPlayers();
    int i = 0;
    for (auto player = players.begin(); player != players.end(); player++)
    {
        auto script = (DukScriptObject*)(*player)->getScript();
        duk_push_heapptr(ctx, script->get_duk_reference());
        duk_put_prop_index(ctx, idx, i);

        i++;
    }

    return 1;
}

int room_find_player(duk_context *ctx)
{
    auto room = (Room*)duk_get_pointer(ctx, 0);
    ASSERT_NOT_NULL(room);
    auto name = duk_get_string(ctx, 1);

    auto player = room->findPlayer(name);

    if (player)
        duk_push_heapptr(ctx, unpack_duk_pointer(player.get()));
    else
        duk_push_null(ctx);

    return 1;
}

int register_command(duk_context *ctx)
{
    std::string typeName = duk_get_string(ctx, 0);
    std::string keyword = duk_get_string(ctx, 1);

    auto cmd = new ScriptCommand(typeName, *g_scriptSystem);
    cmd->setKeyword(keyword);

    int length = duk_get_length(ctx, 2);
    for (auto i = 0; i < length; i++)
    {
        duk_get_prop_index(ctx, 2, i); //Push Parameter object to the stack

        duk_get_prop_literal(ctx, -1, "Name"); //Push name to the stack
        auto parameterName = duk_get_string(ctx, -1);
        duk_pop(ctx); //Pop name

        duk_get_prop_literal(ctx, -1, "Type");
        auto type = duk_get_int(ctx, -1);
        duk_pop(ctx); //Pop type

        duk_pop(ctx); //Pop Parameter object

        switch (type)
        {
            case 1: //ShortText
                cmd->addParameter(SegmentFactory::makeShortText(parameterName));
                break;
            case 2: //FreeText
                cmd->addParameter(SegmentFactory::makeFreeText(parameterName));
                break;
        }
    }

    g_commandSystem->add(std::unique_ptr<Command>(cmd));

    return 0;
}
