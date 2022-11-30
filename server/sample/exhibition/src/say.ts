@command("say")
class SayCommand
{
    @parameter(ParameterType.FreeText)
    public Text: string;

    public Execute(player: Player): void
    {
        let room = player.Room;
        
        room.SendToAllExcept(player, `${player.Name} says, "${this.Text}"`);
        player.Send(`You say, "${this.Text}"`);
    }
}