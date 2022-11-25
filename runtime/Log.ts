class Log
{
    public static Debug(message: string): void
    {
        internal.log_debug(message);
    }

    public static Info(message: string): void
    {
        internal.log_info(message);
    }

    public static Warn(message: string): void
    {
        internal.log_warn(message);
    }

    public static Error(message: string): void
    {
        internal.log_error(message);
    }

    public static Fatal(message: string): void
    {
        internal.log_fatal(message);
    }
}