function command(keyword: string)
{
    return (constructor: any) =>
    {
        let typeName = constructor.name;
        internal.register_command(typeName, keyword);
    }
}