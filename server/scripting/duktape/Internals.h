#include "duk/duktape.h"

int log_debug(duk_context *ctx);
int log_info(duk_context *ctx);
int log_warn(duk_context *ctx);
int log_error(duk_context *ctx);
int log_fatal(duk_context *ctx);

const duk_function_list_entry internals[] = {
    {"log_debug", log_debug, 1},
    {"log_info", log_info, 1},
    {"log_warn", log_warn, 1},
    {"log_error", log_error, 1},
    {"log_fatal", log_fatal, 1},
    {NULL, NULL, 0}
};