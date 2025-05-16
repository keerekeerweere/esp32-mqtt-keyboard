#include <Arduino.h>


#include "azerty_map.h"

#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"

USBHIDKeyboard Keyboard;
USBHIDConsumerControl ConsumerControl;

void keyboard_setup() {

  // initialize control over the keyboard:
  USB.begin();
  Keyboard.begin();
}

void sendAzertyChar(char c) {
  AzertyMapping m = getAzertyMapping(c);
  if (m.keycode == 0) return;

  if (m.modifier & MOD_SHIFT) Keyboard.press(KEY_LEFT_SHIFT);
  if (m.modifier & MOD_ALTGR) {
    Keyboard.press(KEY_RIGHT_ALT);
    Keyboard.press(KEY_LEFT_CTRL);  // AltGr often treated as Ctrl+Alt
  }

  Keyboard.pressRaw(m.keycode);
  delay(5);
  Keyboard.releaseRaw(m.keycode);

  if (m.modifier & MOD_SHIFT) Keyboard.release(KEY_LEFT_SHIFT);
  if (m.modifier & MOD_ALTGR) {
    Keyboard.release(KEY_RIGHT_ALT);
    Keyboard.release(KEY_LEFT_CTRL);
  }
}

void keyboard_write(uint8_t c) {
  sendAzertyChar((char)c);
}

void keyboard_home() {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_ESC);
  Keyboard.releaseAll();
}

#include "azerty_map.h"

void keyboard_sendString(const String &s) {
  for (size_t i = 0; i < s.length(); ++i) {
    AzertyMapping m = getAzertyMapping(s[i]);
    if (m.keycode == 0) continue; // skip unmapped characters

    // Apply modifiers
    if (m.modifier & MOD_SHIFT) Keyboard.press(KEY_LEFT_SHIFT);
    if (m.modifier & MOD_ALTGR) {
      Keyboard.press(KEY_RIGHT_ALT);
      Keyboard.press(KEY_LEFT_CTRL); // AltGr = RightAlt + Ctrl
    }

    // Send key
    Keyboard.pressRaw(m.keycode);
    delay(5);
    Keyboard.releaseRaw(m.keycode);

    // Release modifiers
    if (m.modifier & MOD_SHIFT) Keyboard.release(KEY_LEFT_SHIFT);
    if (m.modifier & MOD_ALTGR) {
      Keyboard.release(KEY_RIGHT_ALT);
      Keyboard.release(KEY_LEFT_CTRL);
    }

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

