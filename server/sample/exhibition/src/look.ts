@command("look")
class LookCommand
{
    @parameter(ParameterType.ShortText)
    public What: string;

    public Execute(player: Player)
    {
        let room = player.Room;

        if (!this.What)
        {
            player.Send(room.Describe());
            return;
        }

        let atPlayer = room.FindPlayer(this.What);
        if (atPlayer)
        {
            player.Send(atPlayer.Describe());
            atPlayer.Send(`${player.Name} looked at you.`);
            return;
        }

        player.Send("I don't see that here.");
    }
}