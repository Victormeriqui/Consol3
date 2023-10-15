#ifndef WINDOWSTRUCTSFORLINUX_HPP
#define WINDOWSTRUCTSFORLINUX_HPP

#include <cstdint>

typedef struct _CHAR_INFO {
    union {
        char UnicodeChar;
        char AsciiChar;
    } Char;
    uint32_t Attributes;
} CHAR_INFO, *PCHAR_INFO;

using WCHAR = char;

#endif