declare class Log {
    static Debug(message: string): void;
    static Info(message: string): void;
    static Warn(message: string): void;
    static Error(message: string): void;
    static Fatal(message: string): void;
}
declare class MUDObject {
    protected native_ptr: Object;
    get Id(): string;
    get Name(): string;
    set Name(value: string);
}
declare class Player extends MUDObject {
}
declare class Room extends MUDObject {
}
declare function command(keyword: string): (constructor: any) => void;
