class MUDObject
{
    protected native_ptr: Object

    public get Id(): string
    {
        return internal.object_get_id(this.native_ptr);
    }

    public get Name(): string
    {
        return internal.object_get_name(this.native_ptr);
    }
    public set Name(value: string)
    {
        internal.object_set_name(this.native_ptr, value);
    }
}