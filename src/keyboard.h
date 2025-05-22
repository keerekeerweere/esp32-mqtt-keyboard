
#pragma once

#include "azerty_map.h"
#include "keyboard_map.h"

#include <Elog.h>



#define MQTTCMND_WILDCARD            UNIQUE_DEVICE_NAME "/cmnd/#"

#define MQTTCMND_UP                  UNIQUE_DEVICE_NAME "/cmnd/UP"
#define MQTTCMND_DOWN                UNIQUE_DEVICE_NAME "/cmnd/DOWN"
#define MQTTCMND_RIGHT               UNIQUE_DEVICE_NAME "/cmnd/RIGHT"
#define MQTTCMND_LEFT                UNIQUE_DEVICE_NAME "/cmnd/LEFT"
#define MQTTCMND_SELECT              UNIQUE_DEVICE_NAME "/cmnd/SELECT"
#define MQTTCMND_ENTER               UNIQUE_DEVICE_NAME "/cmnd/ENTER"
#define MQTTCMND_WRITE               UNIQUE_DEVICE_NAME "/cmnd/WRITE"
#define MQTTCMND_BEANWRITE           UNIQUE_DEVICE_NAME "/cmnd/BEANWRITE"
#define MQTTCMND_SENDSTRING          UNIQUE_DEVICE_NAME "/cmnd/SENDSTRING"
#define MQTTCMND_BACKSPACE           UNIQUE_DEVICE_NAME "/cmnd/BACKSPACE"
#define MQTTCMND_BACK                UNIQUE_DEVICE_NAME "/cmnd/BACKS"
#define MQTTCMND_DEL                 UNIQUE_DEVICE_NAME "/cmnd/DEL"
#define MQTTCMND_FUNCTION            UNIQUE_DEVICE_NAME "/cmnd/FUNCTION"
#define MQTTCMND_ESC                 UNIQUE_DEVICE_NAME "/cmnd/ESC"
#define MQTTCMND_HOME                UNIQUE_DEVICE_NAME "/cmnd/HOME"
#define MQTTCMND_MENU                UNIQUE_DEVICE_NAME "/cmnd/MENU"
#define MQTTCMND_SCAN_PREVIOUS_TRACK UNIQUE_DEVICE_NAME "/cmnd/SCAN_PREVIOUS_TRACK"
#define MQTTCMND_REWIND_LONG         UNIQUE_DEVICE_NAME "/cmnd/REWIND_LONG"
#define MQTTCMND_REWIND              UNIQUE_DEVICE_NAME "/cmnd/REWIND"
#define MQTTCMND_PLAYPAUSE           UNIQUE_DEVICE_NAME "/cmnd/PLAYPAUSE"
#define MQTTCMND_FASTFORWARD         UNIQUE_DEVICE_NAME "/cmnd/FASTFORWARD"
#define MQTTCMND_FASTFORWARD_LONG    UNIQUE_DEVICE_NAME "/cmnd/FASTFORWARD_LONG"
#define MQTTCMND_SCAN_NEXT_TRACK     UNIQUE_DEVICE_NAME "/cmnd/SCAN_NEXT_TRACK"
#define MQTTCMND_MUTE                UNIQUE_DEVICE_NAME "/cmnd/MUTE"
#define MQTTCMND_VOLUME_INCREMENT    UNIQUE_DEVICE_NAME "/cmnd/VOLUME_INCREMENT"
#define MQTTCMND_VOLUME_DECREMENT    UNIQUE_DEVICE_NAME "/cmnd/VOLUME_DECREMENT"


#define MQTT_FUNCTION_ENTER "Enter"
#define MQTT_FUNCTION_ESCAPE "Escape"
#define MQTT_FUNCTION_BACKSPACE "Backspace"
#define MQTT_FUNCTION_TAB "Tab"
#define MQTT_FUNCTION_SPACE " "
#define MQTT_FUNCTION_SHIFT "Shift"
#define MQTT_FUNCTION_CONTROL "Control"
#define MQTT_FUNCTION_ALT "Alt"
#define MQTT_FUNCTION_META "Meta"
#define MQTT_FUNCTION_CAPSLOCK "CapsLock"
#define MQTT_FUNCTION_CONTEXTMENU "ContextMenu"

#define MQTT_FUNCTION_ARROWUP "ArrowUp"
#define MQTT_FUNCTION_ARROWDOWN "ArrowDown"
#define MQTT_FUNCTION_ARROWLEFT "ArrowLeft"
#define MQTT_FUNCTION_ARROWRIGHT "ArrowRight"

#define MQTT_FUNCTION_INSERT "Insert"
#define MQTT_FUNCTION_DELETE "Delete"
#define MQTT_FUNCTION_HOME "Home"
#define MQTT_FUNCTION_END "End"
#define MQTT_FUNCTION_PAGEUP "PageUp"
#define MQTT_FUNCTION_PAGEDOWN "PageDown"

#define MQTT_FUNCTION_F1 "F1"
#define MQTT_FUNCTION_F2 "F2"
#define MQTT_FUNCTION_F3 "F3"
#define MQTT_FUNCTION_F4 "F4"
#define MQTT_FUNCTION_F5 "F5"
#define MQTT_FUNCTION_F6 "F6"
#define MQTT_FUNCTION_F7 "F7"
#define MQTT_FUNCTION_F8 "F8"
#define MQTT_FUNCTION_F9 "F9"
#define MQTT_FUNCTION_F10 "F10"
#define MQTT_FUNCTION_F11 "F11"
#define MQTT_FUNCTION_F12 "F12"




void keyboard_setup(void);
void keyboard_write(uint8_t c, uint16_t modifier);
void keyboard_home();
void keyboard_sendString(const std::string &s);
void keyboard_function(const std::string &s, const uint16_t modifier);
void consumerControl_write(uint16_t value);
void consumerControl_longpress(uint16_t value);
void sendMappedChar(uint32_t c, uint16_t modifier);
