#pragma once

#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

char* scanIntern(char* pattern, char* mask, char* begin, unsigned int size);
char* scanExtern(char* pattern, char* mask, HANDLE* process, char* begin = 0x00000000, char* end = (char*) 0xFFFFFFFF);

HANDLE getProcess(const wchar_t* processName);