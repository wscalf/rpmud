class Player extends MUDObject
{
    public get Room(): Room
    {
        return internal.player_get_room(this.native_ptr);
    }

    public Send(text: string): void
    {
        internal.player_send(this.native_ptr, text);
    }
}