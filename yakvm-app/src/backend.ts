
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';
import type { IClientOptions, MqttClient } from 'mqtt';
import { connect as mqttConnect } from 'mqtt';
import express from 'express';
import cors from 'cors';
import dotenv from 'dotenv';
import protobufjs from 'protobufjs';

// Import type for typing
import { ProtoKeyEvent } from './generated/keyevent.js';

dotenv.config();

const app = express();
app.use(cors());
app.use(express.json());


const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const logFile = path.join(__dirname, 'yakvm-app.log');

function log(message: string): void {
    const timestamp = new Date().toISOString();
    const line = `[${timestamp}] ${message}\n`;
    fs.appendFile(logFile, line, (err: NodeJS.ErrnoException | null) => {
        if (err) console.error('Failed to write log:', err);
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

const {
  MQTT_PROTOCOL,
  MQTT_HOST,
  MQTT_PORT,
  MQTT_PATH,
  MQTT_USERNAME,
  MQTT_PASSWORD
} = process.env;

const mqttProtocol = (MQTT_PROTOCOL ?? 'mqtt') as 'mqtt' | 'mqtts' | 'ws' | 'wss';

const connectOptions: IClientOptions = {
  protocol: mqttProtocol,
  host: MQTT_HOST,
  port: Number(MQTT_PORT),
  username: MQTT_USERNAME,
  password: MQTT_PASSWORD,
  path: MQTT_PATH || undefined
};

const client: MqttClient = mqttConnect(connectOptions);

client.on('connect', () => {
    const msg = '✅ MQTT connected';
    console.log(msg);
    log(msg);
});

client.on('error', (err: Error) => {
    const msg = `❌ MQTT error: ${err.message}`;
    console.error(msg);
    log(msg);
});

app.post('/keypress', (req: express.Request, res: express.Response) => {
    const { key, location, ctrlKey, shiftKey, altKey, metaKey } = req.body;
    if (!key || typeof location !== 'number') {
        return res.status(400).json({ error: 'Missing or invalid key or location' });
    }

    let mod = 0;

    if (ctrlKey) mod |= location === 2 ? MOD_RIGHT_CTRL : MOD_LEFT_CTRL;
    if (shiftKey) mod |= location === 2 ? MOD_RIGHT_SHIFT : MOD_LEFT_SHIFT;
    if (altKey) mod |= location === 2 ? MOD_RIGHT_ALT : MOD_LEFT_ALT;
    if (metaKey) mod |= location === 2 ? MOD_RIGHT_GUI : MOD_LEFT_GUI;

    //const { ProtoKeyEvent } = require('./keyevent');
    const payload: ProtoKeyEvent = {
        modifier: 0,
        functionkey: '',
        sendstring: ''
    };

    // default topic for writing characters and modified characters
    let topic: string = 'esphid1/cmnd/WRITE';

    //handle functions, ENTER, BACKSPACE, ESC, ENTER, F-keys ...
    if (key.length>=1) {
        payload.functionkey = key;
    } 
    if (key.length>1) {
        topic = 'esphid1/cmnd/FUNCTION';
    } 

    const buffer = ProtoKeyEvent.encode(payload).finish(); // this is a Buffer
    console.log("Sending bytes:", buffer);

    client.publish(topic, Buffer.from(buffer), {}, (err) => {
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
