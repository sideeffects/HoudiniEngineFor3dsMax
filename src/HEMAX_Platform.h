#pragma once

struct HEMAX_Platform
{
    static void* GetDllExport(void* LibraryHandle, const char* ExportName);
};
