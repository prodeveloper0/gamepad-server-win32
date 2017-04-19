#pragma once
#include <string>
#include <map>

#include "keypad.h"

/**
* VirtualKey reference
* https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
*/

extern int _interval;
extern unsigned short _port;
extern BYTE _vktable[MAX_KP];

int readfile(const char* filename);