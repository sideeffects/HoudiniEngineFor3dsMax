#include "HEMAX_Platform.h"

#include "windows.h"

void*
HEMAX_Platform::GetDllExport(void* LibraryHandle, const char* ExportName)
{
    return GetProcAddress((HMODULE)LibraryHandle, ExportName);
}
