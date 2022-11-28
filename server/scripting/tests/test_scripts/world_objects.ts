class CustomRoom extends Room
{
    private _count: number = 0;
    public get Count(): number
    {
        return this._count;
    }

    public Increment(): void
    {
        this._count++;
    }
}

class RoomProcessor
{
    public Process(room: CustomRoom): CustomRoom
    {
        room.Increment();
        room.Increment();
        room.Increment();

        return room;
    }
}