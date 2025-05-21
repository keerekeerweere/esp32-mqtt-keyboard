"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const fs_1 = __importDefault(require("fs"));
const path_1 = __importDefault(require("path"));
const mqtt_1 = require("mqtt");
const express_1 = __importDefault(require("express"));
const cors_1 = __importDefault(require("cors"));
const dotenv_1 = __importDefault(require("dotenv"));
dotenv_1.default.config();
const protobuf = require("protobufjs");
const { ProtoKeyEvent } = require('../src/keyevent');
const app = (0, express_1.default)();
app.use((0, cors_1.default)());
app.use(express_1.default.json());
const logFile = path_1.default.join(__dirname, 'yakvm-app.log');
function log(message) {
    const timestamp = new Date().toISOString();
    const line = `[${timestamp}] ${message}\n`;
    fs_1.default.appendFile(logFile, line, (err) => {
        if (err)
            console.error('Failed to write log:', err);
    });
}
const MOD_LEFT_CTRL = 1 << 0;
const MOD_LEFT_SHIFT = 1 << 1;
const MOD_LEFT_ALT = 1 << 2;
const MOD_LEFT_GUI = 1 << 3;
const MOD_RIGHT_CTRL = 1 << 4;
const MOD_RIGHT_SHIFT = 1 << 5;
const MOD_RIGHT_ALT = 1 << 6;
const MOD_RIGHT_GUI = 1 << 7;
const { MQTT_PROTOCOL, MQTT_HOST, MQTT_PORT, MQTT_PATH, MQTT_USERNAME, MQTT_PASSWORD } = process.env;
const mqttProtocol = (MQTT_PROTOCOL !== null && MQTT_PROTOCOL !== void 0 ? MQTT_PROTOCOL : 'mqtt');
const connectOptions = {
    protocol: mqttProtocol,
    host: MQTT_HOST,
    port: Number(MQTT_PORT),
    username: MQTT_USERNAME,
    password: MQTT_PASSWORD,
    path: MQTT_PATH || undefined
};
const client = (0, mqtt_1.connect)(connectOptions);
client.on('connect', () => {
    const msg = '✅ MQTT connected';
    console.log(msg);
    log(msg);
});
client.on('error', (err) => {
    const msg = `❌ MQTT error: ${err.message}`;
    console.error(msg);
    log(msg);
});
app.post('/keypress', (req, res) => {
    var _a;
    const { key, location, ctrlKey, shiftKey, altKey, metaKey } = req.body;
    if (!key || typeof location !== 'number') {
        return res.status(400).json({ error: 'Missing or invalid key or location' });
    }
    let mod = 0;
    if (ctrlKey)
        mod |= location === 2 ? MOD_RIGHT_CTRL : MOD_LEFT_CTRL;
    if (shiftKey)
        mod |= location === 2 ? MOD_RIGHT_SHIFT : MOD_LEFT_SHIFT;
    if (altKey)
        mod |= location === 2 ? MOD_RIGHT_ALT : MOD_LEFT_ALT;
    if (metaKey)
        mod |= location === 2 ? MOD_RIGHT_GUI : MOD_LEFT_GUI;
    const payload = {
        key: 0,
        modifier: mod,
        functionkey: "",
        sendstring: ""
    };
    // default topic for writing characters and modified characters
    let topic = 'esphid1/cmnd/WRITE';
    //handle functions, ENTER, BACKSPACE, ESC, ENTER, F-keys ...
    if (key.length == 1) {
        payload.key = (_a = key.codePointAt(0)) !== null && _a !== void 0 ? _a : 0;
    }
    else if (key.length > 1) {
        topic = 'esphid1/cmnd/FUNCTION';
        payload.functionkey = key;
    }
    const buffer = ProtoKeyEvent.encode(payload).finish();
    console.log("Sending bytes:", buffer);
    client.publish(topic, buffer, {}, (err) => {
        if (err) {
            const errMsg = `Failed to publish to ${topic}: ${err.message}`;
            console.error(errMsg);
            log(errMsg);
            return res.status(500).json({ error: 'MQTT publish failed' });
        }
        const successMsg = `Published to ${topic}: mod=${mod} key=${key}`;
        console.log(successMsg);
        log(successMsg);
        res.json({ status: 'Published', mod, key });
    });
});
const PORT = 3000;
app.listen(PORT, () => {
    const msg = `Backend listening on http://localhost:${PORT}`;
    console.log(msg);
    log(msg);
});
