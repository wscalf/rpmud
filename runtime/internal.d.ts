export as namespace internal;

export function log_debug(message: string): void;
export function log_info(message: string): void;
export function log_warn(message: string): void;
export function log_error(message: string): void;
export function log_fatal(message: string): void;

export function register_command(typeName: string, keyword: string, parameters: Array<Parameter>): void;

export function object_get_id(native_ptr: any): string;
export function object_get_name(native_ptr: any): string;
export function object_set_name(native_ptr: any, name: string): void;
export function object_describe(native_ptr: any): string;

export function player_send(native_ptr: any, text: string): void;
export function player_get_room(native_ptr: any): Room;

export function room_send_to_all(native_ptr: any, text: string): void;
export function room_get_players(native_ptr: any): Player[];
export function room_find_player(native_ptr: any, name: string): Player;
