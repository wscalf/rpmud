class Room extends MUDObject
{
    public get Players(): Player[]
    {
        return internal.room_get_players(this.native_ptr);
    }

    public SendToAll(text: string): void
    {
        internal.room_send_to_all(this.native_ptr, text);
    }

    public SendToAllExcept(notit: Player, text: string)
    {
        this.SendWhere(text, p => p.Id != notit.Id);
    }

    public FindPlayer(name: string): Player
    {
        return internal.room_find_player(this.native_ptr, name);
    }

    private SendWhere(text: string, predicate: (p: Player) => boolean)
    {
        for (var player of this.Players)
            if (predicate(player))
                player.Send(text);
    }
}