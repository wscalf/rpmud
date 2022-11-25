#include "duk/duktape.h"

class DukScriptSystem;
class CommandSystem;

extern DukScriptSystem* g_scriptSystem;
extern CommandSystem* g_commandSystem;

int log_debug(duk_context *ctx);
int log_info(duk_context *ctx);
int log_warn(duk_context *ctx);
int log_error(duk_context *ctx);
int log_fatal(duk_context *ctx);
int register_command(duk_context *ctx);

const duk_function_list_entry internals[] = {
    {"log_debug", log_debug, 1},
    {"log_info", log_info, 1},
    {"log_warn", log_warn, 1},
    {"log_error", log_error, 1},
    {"log_fatal", log_fatal, 1},
    {"register_command", register_command, 2},
    {NULL, NULL, 0}
};