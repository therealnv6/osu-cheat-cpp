#include "scanner.hpp"
#include "utils.h"

// found this on https://stackoverflow.com/questions/37277883/c-signature-scanner-in-console

u32 FindPattern(char* module, char* patern, char* mask)
{
    return 0;
}

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

char* scanExtern(char* pattern, char* mask, HANDLE hProc, char* begin, char* end)
{
    char* currentChunk = begin;
    char* match = nullptr;
    SIZE_T bytesRead;

    Utils::Hexdump(pattern, 0x07);
    Utils::Hexdump(mask, 0x07);

    while (currentChunk < end)
    {
        MEMORY_BASIC_INFORMATION mbi;

        //return nullptr if VirtualQuery fails
        if (!VirtualQueryEx(hProc, currentChunk, &mbi, sizeof(mbi)))
        {
            uint32_t error = GetLastError();
            std::cout << "Error: " << error << std::endl;

            return nullptr;
        }

        std::cout << "Region size: " << mbi.RegionSize << std::endl;
        
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

u8* ScanMemory(HANDLE processHandle, u8* pattern, u32 patternSize)
{
    MEMORY_BASIC_INFORMATION mbi;

    u8* address = 0;
    
    while (VirtualQueryEx(processHandle, address, &mbi, sizeof(mbi)))
    {
        if (mbi.State == MEM_COMMIT && mbi.State != PAGE_NOACCESS)
        {
            u8* buffer = (u8*)malloc(mbi.RegionSize);
            if (buffer == NULL)
                return NULL;

            size_t bytesRead = 0;

            if (ReadProcessMemory(processHandle, (LPCVOID)address, buffer, mbi.RegionSize, &bytesRead))
            {
                u32 scanSize = (bytesRead > 0 ? bytesRead : mbi.RegionSize);

                for (u32 i = 0; i < scanSize; i++)
                {
                    if (!memcmp(buffer + i, pattern, patternSize))
                    {
                        
                        std::cout << "Found match!" << std::endl;
                        Utils::Hexdump(buffer + i, patternSize);
                        free(buffer);
                        return (address + i);
                    }
                }

            }

            free(buffer);
        }

        address += mbi.RegionSize;
    }
    
    return NULL;
}

HANDLE GetProcessHandle(const wchar_t* processName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (snapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 entry;

        entry.dwSize = sizeof PROCESSENTRY32;

        if (Process32First(snapshot, &entry))
        {
            while (Process32Next(snapshot, &entry))
            {
                if (_wcsicmp(entry.szExeFile, processName) == 0)
                {
                    std::wcout << "Found: " << entry.szExeFile << std::endl;
                    
                    CloseHandle(snapshot);
                    return OpenProcess(PROCESS_ALL_ACCESS, NULL, entry.th32ProcessID);
                }
                else
                {
                    std::wcout << "Skip: " << entry.szExeFile << std::endl;
                }
            }
        }
    }
}