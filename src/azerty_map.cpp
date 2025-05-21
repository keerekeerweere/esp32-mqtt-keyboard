#include "azerty_map.h"
#include "keyboard_map.h"

const KeyMapEntry azertyKeyMap[] = {
    // Top number row (AZERTY symbols)
    {'&', {0x1E, MOD_NONE}},
    {0x00E9, {0x1F, MOD_NONE}}, // é
    {'"', {0x20, MOD_NONE}},
    {'\'', {0x21, MOD_NONE}},
    {'(', {0x22, MOD_NONE}},
    {0x00A7, {0x23, MOD_NONE}}, // §
    {0x00E8, {0x24, MOD_NONE}}, // è
    {'!', {0x25, MOD_NONE}},
    {0x00E7, {0x26, MOD_NONE}}, // ç
    {0x00E0, {0x27, MOD_NONE}}, // à
    {')', {0x2D, MOD_NONE}},
    {'-', {0x2E, MOD_NONE}},

    // AltGr symbols
    {'|', {0x1E, MOD_RIGHT_ALT}},
    {'@', {0x1F, MOD_RIGHT_ALT}},
    {'#', {0x20, MOD_RIGHT_ALT}},

    // Whitespace and control
    {' ', {0x2C, MOD_NONE}},
    {'\n', {0x28, MOD_NONE}},
    {'\b', {0x2A, MOD_NONE}},

    // First row
    {'a', {0x14, MOD_NONE}},
    {'z', {0x1A, MOD_NONE}},
    {'e', {0x08, MOD_NONE}},
    {'r', {0x15, MOD_NONE}},
    {'t', {0x17, MOD_NONE}},
    {'y', {0x1C, MOD_NONE}},
    {'u', {0x18, MOD_NONE}},
    {'i', {0x0C, MOD_NONE}},
    {'o', {0x12, MOD_NONE}},
    {'p', {0x13, MOD_NONE}},
    {'^', {0x2F, MOD_NONE}},
    {'$', {0x30, MOD_NONE}},

    // Second row
    {'q', {0x04, MOD_NONE}},
    {'s', {0x16, MOD_NONE}},
    {'d', {0x07, MOD_NONE}},
    {'f', {0x09, MOD_NONE}},
    {'g', {0x0A, MOD_NONE}},
    {'h', {0x0B, MOD_NONE}},
    {'j', {0x0D, MOD_NONE}},
    {'k', {0x0E, MOD_NONE}},
    {'l', {0x0F, MOD_NONE}},
    {'m', {0x33, MOD_NONE}}, // matches AZERTY layout
    {'ù', {0x34, MOD_NONE}},
    {'µ', {0x32, MOD_NONE}},

    // Third row
    {'w', {0x1D, MOD_NONE}},
    {'x', {0x1B, MOD_NONE}},
    {'c', {0x06, MOD_NONE}},
    {'v', {0x19, MOD_NONE}},
    {'b', {0x05, MOD_NONE}},
    {'n', {0x11, MOD_NONE}},
    {',', {0x37, MOD_NONE}},
    {';', {0x36, MOD_NONE}},
    {':', {0x38, MOD_NONE}},
    {'=', {0x2E, MOD_NONE}},

};

const long azertyKeyMapSize = sizeof(azertyKeyMap) / sizeof(azertyKeyMap[0]);

KeyEvent getMappedKeyEvent(uint32_t characterCode)
{
    for (long i = 0; i < azertyKeyMapSize; ++i)
    {
        if (azertyKeyMap[i].characterCode == characterCode)
        {
            return azertyKeyMap[i].event;
        }
    }
    return {0x00, MOD_NONE}; // Default: no match
}

/*

// Inspired by NicoHood's positional mapping for Belgian AZERTY
KeyboardMapping getKeyboardMapping(uint32_t c) {
    switch (c) {
        // Top number row (AZERTY symbols)
        case '&': return {0x1E, MOD_NONE}; // same as '1'
        case 'é': return {0x1F, MOD_NONE}; // same as '2'
        case '"': return {0x20, MOD_NONE}; // same as '3'
        case '\'': return {0x21, MOD_NONE}; // same as '4'
        case '(': return {0x22, MOD_NONE}; // same as '5'
        case '§': return {0x23, MOD_NONE}; // same as '6'
        case 'è': return {0x24, MOD_NONE}; // same as '7'
        case '!': return {0x25, MOD_NONE}; // same as '8'
        case 'ç': return {0x26, MOD_NONE}; // same as '9'
        case 'à': return {0x27, MOD_NONE}; // same as '0'
        case ')': return {0x2D, MOD_NONE};
        case '-': return {0x2E, MOD_NONE};

        case '|': return {0x1E, MOD_RIGHT_ALT};
        case '@': return {0x1F, MOD_RIGHT_ALT};
        case '#': return {0x20, MOD_RIGHT_ALT};

        // First row
        case 'a': return {0x14, MOD_NONE};
        case 'z': return {0x1A, MOD_NONE};
        case 'e': return {0x08, MOD_NONE};
        case 'r': return {0x15, MOD_NONE};
        case 't': return {0x17, MOD_NONE};
        case 'y': return {0x1C, MOD_NONE};
        case 'u': return {0x18, MOD_NONE};
        case 'i': return {0x0C, MOD_NONE};
        case 'o': return {0x12, MOD_NONE};
        case 'p': return {0x13, MOD_NONE};
        case '^': return {0x2F, MOD_NONE};
        case '$': return {0x30, MOD_NONE};

        // Second row
        case 'q': return {0x04, MOD_NONE};
        case 's': return {0x16, MOD_NONE};
        case 'd': return {0x07, MOD_NONE};
        case 'f': return {0x09, MOD_NONE};
        case 'g': return {0x0A, MOD_NONE};
        case 'h': return {0x0B, MOD_NONE};
        case 'j': return {0x0D, MOD_NONE};
        case 'k': return {0x0E, MOD_NONE};
        case 'l': return {0x0F, MOD_NONE};
        case 'm': return {0x33, MOD_NONE}; // matches AZERTY layout
        case 'ù': return {0x34, MOD_NONE};
        case 'µ': return {0x32, MOD_NONE};

        // Third row
        case 'w': return {0x1D, MOD_NONE};
        case 'x': return {0x1B, MOD_NONE};
        case 'c': return {0x06, MOD_NONE};
        case 'v': return {0x19, MOD_NONE};
        case 'b': return {0x05, MOD_NONE};
        case 'n': return {0x11, MOD_NONE};
        case ',': return {0x37, MOD_NONE};
        case ';': return {0x36, MOD_NONE};
        case ':': return {0x38, MOD_NONE};
        case '=': return {0x2E, MOD_NONE};

        // Aliases using physical positions (like NicoHood)
        case '1': return {0x1E, MOD_NONE};
        case '2': return {0x1F, MOD_NONE};
        case '3': return {0x20, MOD_NONE};
        case '4': return {0x21, MOD_NONE};
        case '5': return {0x22, MOD_NONE};
        case '6': return {0x23, MOD_NONE};
        case '7': return {0x24, MOD_NONE};
        case '8': return {0x25, MOD_NONE};
        case '9': return {0x26, MOD_NONE};
        case '0': return {0x27, MOD_NONE};
        case '°': return {0x2D, MOD_NONE};

        // Uppercase letters
        case 'A': return {0x14, MOD_LEFT_SHIFT};
        case 'Z': return {0x1A, MOD_LEFT_SHIFT};
        case 'E': return {0x08, MOD_LEFT_SHIFT};
        case 'R': return {0x15, MOD_LEFT_SHIFT};
        case 'T': return {0x17, MOD_LEFT_SHIFT};
        case 'Y': return {0x1C, MOD_LEFT_SHIFT};
        case 'U': return {0x18, MOD_LEFT_SHIFT};
        case 'I': return {0x0C, MOD_LEFT_SHIFT};
        case 'O': return {0x12, MOD_LEFT_SHIFT};
        case 'P': return {0x13, MOD_LEFT_SHIFT};
        case 'Q': return {0x04, MOD_LEFT_SHIFT};
        case 'S': return {0x16, MOD_LEFT_SHIFT};
        case 'D': return {0x07, MOD_LEFT_SHIFT};
        case 'F': return {0x09, MOD_LEFT_SHIFT};
        case 'G': return {0x0A, MOD_LEFT_SHIFT};
        case 'H': return {0x0B, MOD_LEFT_SHIFT};
        case 'J': return {0x0D, MOD_LEFT_SHIFT};
        case 'K': return {0x0E, MOD_LEFT_SHIFT};
        case 'L': return {0x0F, MOD_LEFT_SHIFT};
        case 'M': return {0x33, MOD_LEFT_SHIFT};
        case 'W': return {0x1D, MOD_LEFT_SHIFT};
        case 'X': return {0x1B, MOD_LEFT_SHIFT};
        case 'C': return {0x06, MOD_LEFT_SHIFT};
        case 'V': return {0x19, MOD_LEFT_SHIFT};
        case 'B': return {0x05, MOD_LEFT_SHIFT};
        case 'N': return {0x11, MOD_LEFT_SHIFT};

        // Special characters
        case '?': return {0x38, MOD_NONE};
        case '.': return {0x37, MOD_NONE};
        case '/': return {0x36, MOD_NONE};
        case '+': return {0x2E, MOD_LEFT_SHIFT};
        case '_': return {0x2D, MOD_LEFT_SHIFT};

        // Whitespace and control
        case ' ': return {0x2C, MOD_NONE};
        case '\n': return {0x28, MOD_NONE};
        case '\b': return {0x2A, MOD_NONE};

        default: return {0x00, MOD_NONE};
    }
}
*/
