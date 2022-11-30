function command(keyword: string)
{
    return (constructor: any) =>
    {
        let typeName = constructor.name;

        if (paramsByCommand[typeName])
            internal.register_command(typeName, keyword, paramsByCommand[typeName]);
        else
            internal.register_command(typeName, keyword, []);
    }
}

function parameter(type: ParameterType)
{
    return (proto: any, member: string) =>
    {
        let param = new Parameter();
        param.Name = member;
        param.Type = type;

        let clazz: string = proto.constructor.name;

        if (paramsByCommand[clazz])
        {
            paramsByCommand[clazz].push(param);
            delete paramsByCommand[clazz];
        }
        else
        {
            paramsByCommand[clazz] = new Array<Parameter>(param);
        }
    }
}

let paramsByCommand: any = [];
