var Log = (function () {
    function Log() {
    }
    Log.Debug = function (message) {
        internal.log_debug(message);
    };
    Log.Info = function (message) {
        internal.log_info(message);
    };
    Log.Warn = function (message) {
        internal.log_warn(message);
    };
    Log.Error = function (message) {
        internal.log_error(message);
    };
    Log.Fatal = function (message) {
        internal.log_fatal(message);
    };
    return Log;
}());
var MUDObject = (function () {
    function MUDObject() {
    }
    return MUDObject;
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
var Player = (function (_super) {
    __extends(Player, _super);
    function Player() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    return Player;
}(MUDObject));
var Room = (function (_super) {
    __extends(Room, _super);
    function Room() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    return Room;
}(MUDObject));
function command(keyword) {
    return function (constructor) {
        var typeName = constructor.name;
        internal.register_command(typeName, keyword);
    };
}
