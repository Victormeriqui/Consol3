#ifndef WINDOWSTRUCTSFORLINUX_HPP
#define WINDOWSTRUCTSFORLINUX_HPP

#include <cstdint>

typedef struct _CHAR_INFO
{
    union
    {
        char UnicodeChar;
        char AsciiChar;
    } Char;
    uint32_t Attributes;
} CHAR_INFO, *PCHAR_INFO;

using WCHAR = char;

typedef struct tagRECT {
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

#endif