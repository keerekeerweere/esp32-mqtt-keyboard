#pragma once
#include <stdint.h>

#define MOD_NONE   0x00
#define MOD_SHIFT  0x02
#define MOD_ALTGR  0x40
#define MOD_CTRL   0x01

struct AzertyMapping {
    uint8_t keycode;
    uint8_t modifier;
};

AzertyMapping getAzertyMapping(char c);
