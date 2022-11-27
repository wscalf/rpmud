class Properties
{
    public SimpleString: string;
    public SimpleStringWithDefault: string = "default";
    public get ReadOnlyString(): string
    {
        return "readonly";
    }
    private backingString: string;
    public get GetterSetterString(): string
    {
        return this.backingString;
    }
    public set GetterSetterString(value: string)
    {
        this.backingString = value;
    }

    public SimpleNumber: number;
    public SimpleNumberWithDefault: number = 42;
    private count: number = 0;
    public get ReadOnlyNumber(): number
    {
        return this.count;
    }
    private backingNumber: number;
    public get GetterSetterNumber(): number
    {
        return this.backingNumber;
    }
    public set GetterSetterNumber(value: number)
    {
        this.backingNumber = value;
    }

    public SimpleBool: boolean;
    public SimpleBoolWithDefault: boolean = true;
    public get ReadOnlyBool(): boolean
    {
        return true;
    }
    private backingBool: boolean;
    public get GetterSetterBool(): boolean
    {
        return this.backingBool;
    }
    public set GetterSetterBool(value: boolean)
    {
        this.backingBool = value;
    }

    public Increment(): void
    {
        this.count++;
    }
    public Sum(a: number, b: number): number
    {
        return a + b;
    }
}