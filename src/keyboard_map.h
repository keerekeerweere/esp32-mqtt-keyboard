
#pragma once

#include <stdint.h>

// Keyboard Modifiers
enum KeyboardMods : uint16_t {
    MOD_NONE            = 0,
    MOD_LEFT_CTRL       = (1 <<  8),
    MOD_LEFT_SHIFT      = (1 <<  9),
    MOD_LEFT_ALT        = (1 << 10),
    MOD_LEFT_GUI        = (1 << 11),
    MOD_RIGHT_CTRL      = (1 << 12),
    MOD_RIGHT_SHIFT     = (1 << 13),
    MOD_RIGHT_ALT       = (1 << 14),
    MOD_RIGHT_GUI       = (uint16_t)(1 << 15),
};
