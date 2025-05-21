#pragma once


#include <stdint.h>
#include "keyboard_map.h"

// Struct to represent a key event
struct KeyEvent {
    uint8_t keycode;
    uint8_t modifier;
};

struct KeyMapEntry {
    uint32_t characterCode;  // MUST be 32-bit to handle all Unicode
    KeyEvent event;
};

KeyEvent getMappedKeyEvent(uint32_t characterCode);
