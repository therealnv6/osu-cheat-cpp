#include "scanner.hpp"

// found this on https://stackoverflow.com/questions/37277883/c-signature-scanner-in-console

char* scanIntern(char* pattern, char* mask, char* begin, unsigned int size)
{
    unsigned int patternLength = strlen(mask);

    for (unsigned int i = 0; i < size - patternLength; i++)
    {
        bool found = true;
        for (unsigned int j = 0; j < patternLength; j++)
        {
            if (mask[j] != '?' && pattern[j] != *(begin + i + j))
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return (begin + i);
        }
    }
    return nullptr;
}

char* scanExtern(char* pattern, char* mask, HANDLE* hProc, char* begin, char* end)
{
    char* currentChunk = begin;
    char* match = nullptr;
    SIZE_T bytesRead;

    while (currentChunk < end)
    {
        MEMORY_BASIC_INFORMATION mbi;

        //return nullptr if VirtualQuery fails
        if (!VirtualQueryEx(hProc, currentChunk, &mbi, sizeof(mbi)))
        {
            return nullptr;
        }

        char* buffer = new char[mbi.RegionSize];

        if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)
        {
            DWORD oldprotect;
            if (VirtualProtectEx(hProc, mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldprotect))
            {
                ReadProcessMemory(hProc, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead);
                VirtualProtectEx(hProc, mbi.BaseAddress, mbi.RegionSize, oldprotect, &oldprotect);

                char* internalAddress = scanIntern(pattern, mask, buffer, bytesRead);

                if (internalAddress != nullptr)
                {
                    //calculate from internal to external
                    uintptr_t offsetFromBuffer = internalAddress - buffer;
                    match = currentChunk + offsetFromBuffer;
                    delete[] buffer;
                    break;
                }
            }
        }

        currentChunk = currentChunk + mbi.RegionSize;
        delete[] buffer;
    }
    return match;
}

HANDLE getProcess(const wchar_t* processName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (snapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 entry = {};
        entry.dwSize = sizeof PROCESSENTRY32;

        if (Process32First(snapshot, &entry))
        {
            do
            {
                if (_wcsicmp(entry.szExeFile, processName) == 0)
                {
                    return OpenProcess(PROCESS_ALL_ACCESS, NULL, entry.th32ProcessID);
                }
            } while(Process32Next(snapshot, &entry));
        }
    }
}