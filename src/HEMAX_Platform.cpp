#include "HEMAX_Platform.h"

void*
HEMAX_Platform::GetDllExport(void* LibraryHandle, const char* ExportName)
{
    return GetProcAddress((HMODULE)LibraryHandle, ExportName);
}

bool
HEMAX_Platform::IsProcessRunning(HANDLE ProcHandle)
{
    if (!ProcHandle)
        return false;

    DWORD ExitCode;
    GetExitCodeProcess(ProcHandle, &ExitCode);

    return ExitCode == STILL_ACTIVE;
}
