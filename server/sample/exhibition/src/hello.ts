@command("hello")
class HelloCommand
{
    public Execute(): void
    {
        Log.Info("Hello, world!");
    }
}