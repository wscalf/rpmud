#include "scripting/duktape/DukScriptSystem.h"
#include "scripting/duktape/Internals.h"
#include "scripting/ScriptCommand.h"
#include "scripting/CommandSystem.h"
#include "util/Log.h"

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

int register_command(duk_context *ctx)
{
    std::string typeName = duk_get_string(ctx, 0);
    std::string keyword = duk_get_string(ctx, 1);

    auto cmd = new ScriptCommand(typeName, *g_scriptSystem);
    cmd->setKeyword(keyword);
    g_commandSystem->add(std::unique_ptr<Command>(cmd));

    return 0;
}