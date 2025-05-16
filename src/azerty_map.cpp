#include "azerty_map.h"

// Keycode mappings for Belgian AZERTY layout
AzertyMapping getAzertyMapping(char c) {
    switch (c) {
        // Top number row (AZERTY symbols)
        case '&': return {0x1E, MOD_NONE}; // 1
        case 'é': return {0x1F, MOD_NONE}; // 2
        case '"': return {0x20, MOD_NONE}; // 3
        case '\'': return {0x21, MOD_NONE}; // 4
        case '(': return {0x22, MOD_NONE}; // 5
        case '§': return {0x23, MOD_NONE}; // 6
        case 'è': return {0x24, MOD_NONE}; // 7
        case '!': return {0x25, MOD_NONE}; // 8
        case 'ç': return {0x26, MOD_NONE}; // 9
        case 'à': return {0x27, MOD_NONE}; // 0
        case ')': return {0x2D, MOD_NONE}; // ° on AZERTY
        case '-': return {0x2E, MOD_NONE};

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
        case 'm': return {0x10, MOD_NONE};
        case 'ù': return {0x33, MOD_NONE};
        case 'µ': return {0x34, MOD_NONE};

        // Third row
        case 'w': return {0x1D, MOD_NONE};
        case 'x': return {0x1B, MOD_NONE};
        case 'c': return {0x06, MOD_NONE};
        case 'v': return {0x19, MOD_NONE};
        case 'b': return {0x05, MOD_NONE};
        case 'n': return {0x11, MOD_NONE};
        case ',': return {0x36, MOD_NONE};
        case ';': return {0x37, MOD_NONE};
        case ':': return {0x38, MOD_NONE};
        case '=': return {0x2E, MOD_NONE};

        // Numbers and symbols
        case '1': return {0x1E, MOD_NONE}; // '&'
        case '2': return {0x1F, MOD_NONE}; // 'é'
        case '3': return {0x20, MOD_NONE}; // '"'
        case '4': return {0x21, MOD_NONE}; // '\''
        case '5': return {0x22, MOD_NONE}; // '('
        case '6': return {0x23, MOD_NONE}; // '§'
        case '7': return {0x24, MOD_NONE}; // 'è'
        case '8': return {0x25, MOD_NONE}; // '!'
        case '9': return {0x26, MOD_NONE}; // 'ç'
        case '0': return {0x27, MOD_NONE}; // 'à'
        case '°': return {0x2D, MOD_NONE}; // ')'

        // Letters
        case 'A': return {0x14, MOD_SHIFT};
        case 'Z': return {0x1A, MOD_SHIFT};
        case 'E': return {0x08, MOD_SHIFT};
        case 'R': return {0x15, MOD_SHIFT};
        case 'T': return {0x17, MOD_SHIFT};
        case 'Y': return {0x1C, MOD_SHIFT};
        case 'U': return {0x18, MOD_SHIFT};
        case 'I': return {0x0C, MOD_SHIFT};
        case 'O': return {0x12, MOD_SHIFT};
        case 'P': return {0x13, MOD_SHIFT};
        case 'Q': return {0x04, MOD_SHIFT};
        case 'S': return {0x16, MOD_SHIFT};
        case 'D': return {0x07, MOD_SHIFT};
        case 'F': return {0x09, MOD_SHIFT};
        case 'G': return {0x0A, MOD_SHIFT};
        case 'H': return {0x0B, MOD_SHIFT};
        case 'J': return {0x0D, MOD_SHIFT};
        case 'K': return {0x0E, MOD_SHIFT};
        case 'L': return {0x0F, MOD_SHIFT};
        case 'M': return {0x10, MOD_SHIFT};
        case 'W': return {0x1D, MOD_SHIFT};
        case 'X': return {0x1B, MOD_SHIFT};
        case 'C': return {0x06, MOD_SHIFT};
        case 'V': return {0x19, MOD_SHIFT};
        case 'B': return {0x05, MOD_SHIFT};
        case 'N': return {0x11, MOD_SHIFT};

        // Special characters
        case '?': return {0x38, MOD_NONE};
        case '.': return {0x37, MOD_NONE};
        case '/': return {0x36, MOD_NONE};
        case '+': return {0x2E, MOD_SHIFT};
        case '_': return {0x2D, MOD_SHIFT};

        // Space and control characters
        case ' ': return {0x2C, MOD_NONE};
        case '\n': return {0x28, MOD_NONE}; // Enter
        case '\b': return {0x2A, MOD_NONE}; // Backspace

        default: return {0x00, MOD_NONE}; // Unmapped
    }
}
