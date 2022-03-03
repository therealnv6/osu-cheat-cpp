#pragma once

#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "global.h"

char* scanIntern(char* pattern, char* mask, char* begin, unsigned int size);
char* scanExtern(char* pattern, char* mask, HANDLE process, char* begin = 0x00000000, char* end = (char*) 0xFFFFFFFF);

u8* ScanMemory(HANDLE processHandle, u8* pattern, u32 pattenSize);

HANDLE GetProcessHandle(const wchar_t* processName);