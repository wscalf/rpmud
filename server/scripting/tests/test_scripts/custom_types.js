var Properties = (function () {
    function Properties() {
        this.SimpleStringWithDefault = "default";
        this.SimpleNumberWithDefault = 42;
        this.count = 0;
        this.SimpleBoolWithDefault = true;
    }
    Object.defineProperty(Properties.prototype, "ReadOnlyString", {
        get: function () {
            return "readonly";
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Properties.prototype, "GetterSetterString", {
        get: function () {
            return this.backingString;
        },
        set: function (value) {
            this.backingString = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Properties.prototype, "ReadOnlyNumber", {
        get: function () {
            return this.count;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Properties.prototype, "GetterSetterNumber", {
        get: function () {
            return this.backingNumber;
        },
        set: function (value) {
            this.backingNumber = value;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Properties.prototype, "ReadOnlyBool", {
        get: function () {
            return true;
        },
        enumerable: true,
        configurable: true
    });
    Object.defineProperty(Properties.prototype, "GetterSetterBool", {
        get: function () {
            return this.backingBool;
        },
        set: function (value) {
            this.backingBool = value;
        },
        enumerable: true,
        configurable: true
    });
    Properties.prototype.Increment = function () {
        this.count++;
    };
    Properties.prototype.Sum = function (a, b) {
        return a + b;
    };
    return Properties;
}());
