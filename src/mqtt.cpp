#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include "config.h"
#include "wifiCommunication.h"
#include "mqtt.h"
#include "keyboard.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"

#include "keyevent.pb.h"

#include <pb_decode.h>
#include <pb_encode.h>

#include <Elog.h>
#include <ElogMacros.h>  // for logging macros like info(), error(), etc.


// https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
// https://github.com/knolleary/pubsubclient
// https://gist.github.com/igrr/7f7e7973366fc01d6393

unsigned long reconnectInterval = 15000;
// in order to do reconnect immediately ...
unsigned long lastReconnectAttempt = millis() - reconnectInterval - 1;
unsigned long timerStartForHAdiscovery = 1;

bool checkMQTTconnection();
bool publishMQTTMessage(const char* topic, const char* payload, boolean retained, uint8_t len);
void onMqttMessage(char* topic, uint8_t* payload, unsigned int len);

WiFiClient wifiClient;   
PubSubClient mqttClient(MQTT_SERVER , MQTT_SERVER_PORT, onMqttMessage, wifiClient);

void mqtt_setup() {
  // Set bigger buffer size if needed
  mqttClient.setBufferSize(1540);
  //mqttClient.setCallback(onMqttMessage);
  //mqttClient.onPublish(onMqttPublish);
  //mqttClient.onConnect(onMqttConnect);
  //mqttClient.setCredentials(MQTT_USER, MQTT_PASS);
  //mqttClient.setAutoReconnect(true);
}


void onMqttConnect(bool sessionPresent) {
  logger.log(LOGGING_ID, INFO, "Connected to MQTT.");
  mqttClient.subscribe(MQTTCMND_WILDCARD, 0);
}

void mqtt_loop(){
  // Handle MQTT client loop
  
  if (!mqttClient.connected()) {
    unsigned long currentMillis = millis();
    if ((currentMillis - lastReconnectAttempt) > reconnectInterval) {
      lastReconnectAttempt = currentMillis;
      // Attempt to reconnect
      checkMQTTconnection();
    }
  }  

  if (mqttClient.connected()) {
    mqttClient.loop();
  }
}

bool checkMQTTconnection() {
  if (wifiIsDisabled) return false;

  if (WiFi.isConnected()) {
    if (mqttClient.connected()) {
      return true;
    } else {
      // try to connect to mqtt server
      if (mqttClient.connect(MQTT_CLIENTNAME, MQTT_USER, MQTT_PASS)) {
        logger.log(LOGGING_ID, INFO, "Successfully connected to MQTT broker.");
        // subscribes to all relevant messages with given topic. 
        // wildcard subscribes for everything on the 3rd topic part: "/cmnd/#" e.g. esphid1/cmnd/ESC
        // Callback function will be called 1. in client.loop() 2. when sending a message
        mqttClient.subscribe(MQTTCMND_WILDCARD);
      } else {
        logger.log(LOGGING_ID, ERROR, "MQTT connection failed (but WiFi is available). Will try later ...");
      }
      return mqttClient.connected();
    }
  } else {
    Serial.printf("  No connection to MQTT server, because WiFi ist not connected.\r\n");
    return false;
  }  
}


bool publishMQTTMessage(const char *topic, const char* payload, boolean retained, uint8_t len){
  if (wifiIsDisabled) return false;
  if (checkMQTTconnection()) {
    // Serial.printf("Sending mqtt payload to topic \"%s\": %s\r\n", topic, payload);      
    if (mqttClient.publish(topic, (uint8_t*)payload, len, 0)) {
      logger.log(LOGGING_ID, DEBUG, "Published mqtt %s ", topic);
      return true;
    }
    else {
      logger.log(LOGGING_ID, ERROR, "Publish mqtt %s failed ", topic);
    }
  } else {
    logger.log(LOGGING_ID, ERROR, "Cannot Publish mqtt %s  ", topic);
  }
  return false;
}



bool mqtt_publish_tele() {
  bool error = false;
  // maximum message length 128 Byte
  String payload = "";

  // WiFi
  payload = "";
  payload += "{\"wifiRSSI\":";
  payload += WiFi.RSSI();
  payload += ",\"wifiChan\":";
  payload += WiFi.channel();
  payload += ",\"wifiSSID\":";
  payload += WiFi.SSID();
  payload += ",\"wifiBSSID\":";
  payload += WiFi.BSSIDstr();
  payload += ",\"IP\":";
  payload += WiFi.localIP().toString();
  payload += "}";
  error = error || !publishMQTTMessage(MQTTTELESTATE1, payload.c_str(), 0, payload.length());
  logger.log(LOGGING_ID, INFO, payload.c_str());

  // ESP32 stats
  payload = "";
  payload += "{\"up\":";
  payload += String(millis());
  payload += ",\"heapSize\":";
  payload += String(ESP.getHeapSize());
  payload += ",\"heapFree\":";
  payload += String(ESP.getFreeHeap());
  payload += ",\"heapMin\":";
  payload += String(ESP.getMinFreeHeap());
  payload += ",\"heapMax\":";
  payload += String(ESP.getMaxAllocHeap());
  payload += "}";
  logger.log(LOGGING_ID, INFO, payload.c_str());

  return !error;
}


uint32_t decodeUtf8(const uint8_t * data, size_t length, size_t startIndex ) {
    if (length <= startIndex) return 0;

    uint8_t first = data[startIndex];
    if (first < 0x80) {
        // 1-byte (ASCII)
        return first;
    } else if ((first & 0xE0) == 0xC0 && startIndex + 1 < length) {
        // 2-byte sequence
        return ((first & 0x1F) << 6) |
               (data[startIndex + 1] & 0x3F);
    } else if ((first & 0xF0) == 0xE0 && startIndex + 2 < length) {
        // 3-byte sequence
        return ((first & 0x0F) << 12) |
               ((data[startIndex + 1] & 0x3F) << 6) |
               (data[startIndex + 2] & 0x3F);
    } else if ((first & 0xF8) == 0xF0 && startIndex + 3 < length) {
        // 4-byte sequence
        return ((first & 0x07) << 18) |
               ((data[startIndex + 1] & 0x3F) << 12) |
               ((data[startIndex + 2] & 0x3F) << 6) |
               (data[startIndex + 3] & 0x3F);
    }
    // Invalid or incomplete UTF-8
//    mqttlog("decodeUtf8","Invalid or incomplete UTF-8");

    return 0;
}

void dumpPayloadBytes(const char* label, const uint8_t* payload, size_t length) {
    char buf[128];
    char* ptr = buf;

    for (size_t i = 0; i < length && i < sizeof(buf)/3 - 1; ++i) {
        ptr += snprintf(ptr, buf + sizeof(buf) - ptr, "%02X ", payload[i]);
    }
    logger.log(LOGGING_ID, DEBUG, "Payload bytes (%d): %s", (int)length, buf);
}

void onMqttMessage(char* topic, uint8_t* payload, size_t len) {
  logger.log(LOGGING_ID, DEBUG, "Topic: %s, Length: %i", topic, len);  
  dumpPayloadBytes("payload", (const uint8_t*)payload, len);

  if (strcmp(MQTTCMND_BEANWRITE, topic) == 0) {
    ProtoKeyEvent keyEvent = ProtoKeyEvent_init_zero;
    keyEvent.modifier = 0;
    snprintf(keyEvent.functionkey, sizeof(keyEvent.functionkey), "t");
    snprintf(keyEvent.sendstring, sizeof(keyEvent.sendstring), "");
   
    uint8_t buffer[156];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (!pb_encode(&stream, ProtoKeyEvent_fields, &keyEvent)) {
      logger.log(LOGGING_ID, ERROR, "encode fail Error encoding: %s", PB_GET_ERROR(&stream));  
        return;
    }

    mqttClient.publish(MQTTCMND_WRITE, buffer, stream.bytes_written);    
  } else if (strcmp(MQTTCMND_WRITE, topic) == 0) {
      ProtoKeyEvent keyEvent = ProtoKeyEvent_init_zero;
      pb_istream_t stream = pb_istream_from_buffer(payload, len);

      if (!pb_decode(&stream, ProtoKeyEvent_fields, &keyEvent)) {
          logger.log(LOGGING_ID, ERROR, "Failed to decode topic %s - %s ", topic, PB_GET_ERROR(&stream));  
          return;
      }

      char c = keyEvent.functionkey[0];
      if (c != 0x00 ) {
        //add the azerty mapping too !
        sendMappedChar(c, keyEvent.modifier);
    } else {
      mqttlog("callback-error","no typed character ");
    }
  } else if (strcmp(MQTTCMND_FUNCTION, topic) == 0) {
      ProtoKeyEvent keyEvent = ProtoKeyEvent_init_zero;
      pb_istream_t stream = pb_istream_from_buffer(payload, len);

      if (!pb_decode(&stream, ProtoKeyEvent_fields, &keyEvent)) {
          logger.log(LOGGING_ID, ERROR, "Failed to decode topic %s - %s ", topic, PB_GET_ERROR(&stream));  
          return;
      }

    std::string functionStr(keyEvent.functionkey);
    keyboard_function(functionStr, keyEvent.modifier);

  }
  /*
  ProtoKeyEvent keyEvent = ProtoKeyEvent_init_zero;
  pb_istream_t stream = pb_istream_from_buffer((const uint8_t*)payload, len);

  if (!pb_decode(&stream, ProtoKeyEvent_fields, &keyEvent)) {
    mqttlog("decode-fail", "Error decoding: %s", PB_GET_ERROR(&stream));
    return;
  }

  mqttlog("keyevent", "key=%s, mod=%u, send=%s", keyEvent.functionkey,
          keyEvent.modifier, keyEvent.sendstring);
*/
}

/*
void callback(char* topic, byte* payload, unsigned int length) {

  mqttlog("callback-1", "topic: %s, length: %u", topic, length);
  dumpPayloadBytes("callback-raw", payload, length);  // Log raw bytes first

  ProtoKeyEvent keyEvent = ProtoKeyEvent_init_zero;
  pb_istream_t stream = pb_istream_from_buffer(payload, length);

  if (!pb_decode(&stream, ProtoKeyEvent_fields, &keyEvent)) {
        mqttlog("callback-error", "Failed to decode topic %s", topic);
        mqttlog("callback-error-stream", PB_GET_ERROR(&stream));      
      return;
  }

  mqttlog("callback-ok", "key=%s, mod=%u %s", keyEvent.functionkey, keyEvent.modifier, keyEvent.sendstring);


  bool doLog = false;

  if (strcmp(MQTTCMND_WRITE, topic) == 0) {
    char c = keyEvent.functionkey[0];
    if (c != 0x00 ) {
      //add the azerty mapping too !
      sendMappedChar(c, keyEvent.modifier);
    } else {
      mqttlog("callback-error","no typed character ");
    }
  } else if (strcmp(MQTTCMND_FUNCTION, topic) == 0) {
    if (doLog) {Serial.printf("FUNCTION received\r\n");}
    std::string functionStr(keyEvent.functionkey);
    keyboard_function(functionStr, keyEvent.modifier);

  } else if (strcmp(MQTTCMND_MENU, topic) == 0) {
    if (doLog) {Serial.printf("MENU received\r\n");}
    keyboard_write(0xED, MOD_NONE); // 0xDA + 13 = 0xED

  } else if (strcmp(MQTTCMND_SENDSTRING, topic) == 0) {
    if (doLog) {Serial.printf("SENDSTRING received\r\n");}
    std::string sendStr(keyEvent.sendstring);
    keyboard_sendString(sendStr);

  } else if (strcmp(MQTTCMND_SCAN_PREVIOUS_TRACK, topic) == 0) {
    if (doLog) {Serial.printf("SCAN_PREVIOUS_TRACK received\r\n");}
    consumerControl_write(CONSUMER_CONTROL_SCAN_PREVIOUS);

  } else if (strcmp(MQTTCMND_REWIND_LONG, topic) == 0) {
    if (doLog) {Serial.printf("REWIND_LONG received\r\n");}
    consumerControl_longpress(0x00B4);

  } else if (strcmp(MQTTCMND_REWIND, topic) == 0) {
    if (doLog) {Serial.printf("REWIND received\r\n");}
    consumerControl_write(0x00B4);

  } else if (strcmp(MQTTCMND_PLAYPAUSE, topic) == 0) {
    if (doLog) {Serial.printf("PLAYPAUSE received\r\n");}
    consumerControl_write(CONSUMER_CONTROL_PLAY_PAUSE);

  } else if (strcmp(MQTTCMND_FASTFORWARD, topic) == 0) {
    if (doLog) {Serial.printf("FASTFORWARD received\r\n");}
    consumerControl_write(0x00B3);
    
  } else if (strcmp(MQTTCMND_FASTFORWARD_LONG, topic) == 0) {
    if (doLog) {Serial.printf("FASTFORWARD_LONG received\r\n");}
    consumerControl_longpress(0x00B3);
    
  } else if (strcmp(MQTTCMND_SCAN_NEXT_TRACK, topic) == 0) {
    if (doLog) {Serial.printf("SCAN_NEXT_TRACK received\r\n");}
    consumerControl_write(CONSUMER_CONTROL_SCAN_NEXT);

  } else if (strcmp(MQTTCMND_MUTE, topic) == 0) {
    if (doLog) {Serial.printf("MUTE received\r\n");}
    consumerControl_write(CONSUMER_CONTROL_MUTE);

  } else if (strcmp(MQTTCMND_VOLUME_INCREMENT, topic) == 0) {
    if (doLog) {Serial.printf("VOLUME_INCREMENT received\r\n");}
    consumerControl_write(CONSUMER_CONTROL_VOLUME_INCREMENT);

  } else if (strcmp(MQTTCMND_VOLUME_DECREMENT, topic) == 0) {
    if (doLog) {Serial.printf("VOLUME_DECREMENT received\r\n");}
    consumerControl_write(CONSUMER_CONTROL_VOLUME_DECREMENT);

  } else if (strcmp(MQTTCMND_RESTART_ESP32, topic) == 0) {
    if (doLog) {Serial.printf("RESTART_ESP32 received\r\n");}
    ESP.restart();

  } else {
    //errorLog("topic not parsed %s ", topic);
  }




}

*/