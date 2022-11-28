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
var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
var CustomRoom = (function (_super) {
    __extends(CustomRoom, _super);
    function CustomRoom() {
        var _this = _super !== null && _super.apply(this, arguments) || this;
        _this._count = 0;
        return _this;
    }
    Object.defineProperty(CustomRoom.prototype, "Count", {
        get: function () {
            return this._count;
        },
        enumerable: true,
        configurable: true
    });
    CustomRoom.prototype.Increment = function () {
        this._count++;
    };
    return CustomRoom;
}(Room));
var RoomProcessor = (function () {
    function RoomProcessor() {
    }
    RoomProcessor.prototype.Process = function (room) {
        room.Increment();
        room.Increment();
        room.Increment();
        return room;
    };
    return RoomProcessor;
}());
