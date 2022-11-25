export as namespace internal;

export function log_debug(message: string): void;
export function log_info(message: string): void;
export function log_warn(message: string): void;
export function log_error(message: string): void;
export function log_fatal(message: string): void;

export function register_command(typeName: string, keyword: string): void;