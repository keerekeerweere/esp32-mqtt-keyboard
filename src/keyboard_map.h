
#pragma once

#include <stdint.h>

enum KeyboardMods : uint8_t {
    MOD_NONE            = 0x00,
    MOD_LEFT_CTRL       = 1,
    MOD_LEFT_SHIFT      = (1 <<  1),
    MOD_LEFT_ALT        = (1 << 2),
    MOD_LEFT_GUI        = (1 << 3),
    MOD_RIGHT_CTRL      = (1 << 4),
    MOD_RIGHT_SHIFT     = (1 << 5),
    MOD_RIGHT_ALT       = (1 << 6),
    MOD_RIGHT_GUI       = (1 << 7)
};
