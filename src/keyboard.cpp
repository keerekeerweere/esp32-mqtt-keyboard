#include <Arduino.h>

#include "keyboard.h"
#include "mqtt.h"
#include "config.h"


#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"



#include <map>

USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;

// Mapping JavaScript key strings to USB HID codes
std::map<std::string, uint8_t> map_jskey_hid = {
    { MQTT_FUNCTION_ENTER,       KEY_RETURN },
    { MQTT_FUNCTION_ESCAPE,      KEY_ESC },
    { MQTT_FUNCTION_BACKSPACE,   KEY_BACKSPACE },
    { MQTT_FUNCTION_TAB,         KEY_TAB },
    { MQTT_FUNCTION_SHIFT,       KEY_LEFT_SHIFT },
    { MQTT_FUNCTION_CONTROL,     KEY_LEFT_CTRL },
    { MQTT_FUNCTION_ALT,         KEY_LEFT_ALT },
//    { MQTT_FUNCTION_META,        KEY_META },
    { MQTT_FUNCTION_CAPSLOCK,    KEY_CAPS_LOCK },
//    { MQTT_FUNCTION_CONTEXTMENU, KEY_CONTEXT_MENU },

    { MQTT_FUNCTION_ARROWUP,     KEY_UP_ARROW },
    { MQTT_FUNCTION_ARROWDOWN,   KEY_DOWN_ARROW },
    { MQTT_FUNCTION_ARROWLEFT,   KEY_LEFT_ARROW },
    { MQTT_FUNCTION_ARROWRIGHT,  KEY_RIGHT_ARROW },

    { MQTT_FUNCTION_INSERT,      KEY_INSERT },
    { MQTT_FUNCTION_DELETE,      KEY_DELETE },
    { MQTT_FUNCTION_HOME,        KEY_HOME },
    { MQTT_FUNCTION_END,         KEY_END },
    { MQTT_FUNCTION_PAGEUP,      KEY_PAGE_UP },
    { MQTT_FUNCTION_PAGEDOWN,    KEY_PAGE_DOWN },

    { MQTT_FUNCTION_F1,          KEY_F1 },
    { MQTT_FUNCTION_F2,          KEY_F2 },
    { MQTT_FUNCTION_F3,          KEY_F3 },
    { MQTT_FUNCTION_F4,          KEY_F4 },
    { MQTT_FUNCTION_F5,          KEY_F5 },
    { MQTT_FUNCTION_F6,          KEY_F6 },
    { MQTT_FUNCTION_F7,          KEY_F7 },
    { MQTT_FUNCTION_F8,          KEY_F8 },
    { MQTT_FUNCTION_F9,          KEY_F9 },
    { MQTT_FUNCTION_F10,         KEY_F10 },
    { MQTT_FUNCTION_F11,         KEY_F11 },
    { MQTT_FUNCTION_F12,         KEY_F12 }
};


void keyboard_setup() {
  // initialize control over the keyboard:
  USB.begin();
  Keyboard.begin();
}

void sendMappedChar(uint32_t c, uint8_t modifier) {
  KeyEvent e = getMappedKeyEvent(c);
  if (e.keycode == 0) {
    logger.log(LOGGING_ID, ERROR, "sendMappedChar - no key found for UTF-8: U+%04lX", c);  
    return;
  }
  uint8_t key = e.keycode;
  uint8_t mod = (e.modifier != MOD_NONE)  ? e.modifier : modifier;
  logger.log(LOGGING_ID, DEBUG, "sendMappedChar - found keycode %02X and mod  %02X", key, mod);  

  keyboard_write(key, mod);
}


void keyboard_write(uint8_t key, uint8_t modifier) {
  logger.log(LOGGING_ID, DEBUG, "keyboard_write : %u", key);

  if (modifier != MOD_NONE) {
        //decode possibly multiple modifiers
      if (modifier & MOD_LEFT_CTRL) Keyboard.press(KEY_LEFT_CTRL);
      if (modifier & MOD_LEFT_SHIFT) Keyboard.press(KEY_LEFT_SHIFT);
      if (modifier & MOD_LEFT_ALT) Keyboard.press(KEY_LEFT_ALT);
      if (modifier & MOD_LEFT_CTRL) Keyboard.press(KEY_LEFT_CTRL);
     
      Keyboard.pressRaw(key);
      delay(5);
      Keyboard.releaseRaw(key);


      if (modifier & MOD_LEFT_CTRL) Keyboard.release(KEY_LEFT_CTRL);
      if (modifier & MOD_LEFT_SHIFT) Keyboard.release(KEY_LEFT_SHIFT);
      if (modifier & MOD_LEFT_ALT) Keyboard.release(KEY_LEFT_ALT);
      if (modifier & MOD_LEFT_CTRL) Keyboard.release(KEY_LEFT_CTRL);
    } else {

      Keyboard.pressRaw(key);
      delay(5);
      Keyboard.releaseRaw(key);
  }
}

void keyboard_function(const std::string &s, uint8_t modifier)  {
  //ignore empty items
  if (s.length()==0) return;
  uint8_t key = 0;

  auto it = map_jskey_hid.find(s);

  if (it != map_jskey_hid.end()) {
      key = it->second;      
  }

  if (key ==0) return;
  //call write directly, no layout specific mapping required (I believe)
  if (modifier != MOD_NONE) {
        //decode possibly multiple modifiers
      if (modifier & MOD_LEFT_CTRL) Keyboard.press(KEY_LEFT_CTRL);
      if (modifier & MOD_LEFT_SHIFT) Keyboard.press(KEY_LEFT_SHIFT);
      if (modifier & MOD_LEFT_ALT) Keyboard.press(KEY_LEFT_ALT);
      if (modifier & MOD_LEFT_CTRL) Keyboard.press(KEY_LEFT_CTRL);
     
      Keyboard.write(key);


      if (modifier & MOD_LEFT_CTRL) Keyboard.release(KEY_LEFT_CTRL);
      if (modifier & MOD_LEFT_SHIFT) Keyboard.release(KEY_LEFT_SHIFT);
      if (modifier & MOD_LEFT_ALT) Keyboard.release(KEY_LEFT_ALT);
      if (modifier & MOD_LEFT_CTRL) Keyboard.release(KEY_LEFT_CTRL);
    } else {

      Keyboard.write(key);
  }
}


void keyboard_home() {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
}


void keyboard_sendString(const std::string &s) {
  for (size_t i = 0; i < s.length(); ++i) {
    KeyEvent e = getMappedKeyEvent(s[i]);
    if (e.keycode == 0) continue; // skip unmapped characters

    keyboard_write(e.keycode, e.modifier);

    delay(5); // slight delay between characters
  }
}


void consumerControl_write(uint16_t value) {
  ConsumerControl.press(value);
  ConsumerControl.release();
}

void consumerControl_longpress(uint16_t value) {
  ConsumerControl.press(value);
  delay(1000);
  ConsumerControl.release();
}

