#pragma once

#include <windows.h>

struct HEMAX_Platform
{
    static void*    GetDllExport(void* LibraryHandle, const char* ExportName);
    static bool     IsProcessRunning(HANDLE ProcHandle);
};
