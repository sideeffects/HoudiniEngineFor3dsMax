#include "HEMAX_Logger.h"

#include "HEMAX_UserPrefs.h"
#include "HEMAX_Utilities.h"

#pragma warning(push, 0)
#include <maxapi.h>
#include <maxscript/maxscript.h>
#pragma warning(pop)

HEMAX_Logger&
HEMAX_Logger::Instance()
{
    static HEMAX_Logger TheInstance;
    return TheInstance;
}

HEMAX_Logger::~HEMAX_Logger() {}

HEMAX_Logger::HEMAX_Logger()
{
    bool Setting;
    HEMAX_UserPrefs& Prefs = HEMAX_UserPrefs::Get();

    Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_ERRORS, Setting);
    ConfigurePrintLevels(HEMAX_LOG_LEVEL_ERROR, Setting);

    Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_WARNINGS, Setting);
    ConfigurePrintLevels(HEMAX_LOG_LEVEL_WARN, Setting);

    Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_INFO, Setting);
    ConfigurePrintLevels(HEMAX_LOG_LEVEL_INFO, Setting);
}

void
HEMAX_Logger::AddEntry(const std::string& Log, HEMAX_LogLevel LogLevel)
{
    StoreEntry(Log, LogLevel);

    GetCOREInterface()->Log()->LogEntry(LogLevel, NO_DIALOG, nullptr,
        HEMAX_Utilities::GetWideString(Log).c_str());

    if (ShouldPrint(LogLevel))
    {
	mprintf(HEMAX_Utilities::GetWideString(Log + "\n").c_str());
	mflush();
    }
}

void
HEMAX_Logger::AddEntry(const char* Log, HEMAX_LogLevel LogLevel)
{
    StoreEntry(Log, LogLevel);

    GetCOREInterface()->Log()->LogEntry(LogLevel, NO_DIALOG, nullptr,
        HEMAX_Utilities::GetWideString(Log).c_str());

    if (ShouldPrint(LogLevel))
    {
	std::string MaxscriptListenerString = std::string(Log) + "\n";
	mprintf(HEMAX_Utilities::GetWideString(MaxscriptListenerString).c_str());
	mflush();
    }
}

void
HEMAX_Logger::AddEntry(const std::wstring& Log, HEMAX_LogLevel LogLevel)
{
    StoreEntry(HEMAX_Utilities::GetUtf8String(Log), LogLevel);

    GetCOREInterface()->Log()->LogEntry(LogLevel, NO_DIALOG, nullptr,
        Log.c_str());

    if (ShouldPrint(LogLevel))
    {
        std::wstring MaxScriptListenerString = Log + L"\n";
        mprintf(MaxScriptListenerString.c_str());
        mflush();
    }
}

void
HEMAX_Logger::ShowDialog(std::string Title, std::string Message, HEMAX_LogLevel LogLevel)
{
    StoreEntry(Message, LogLevel);

    GetCOREInterface()->Log()->LogEntry(LogLevel, DISPLAY_DIALOG,
        HEMAX_Utilities::GetWideString(Title).c_str(),
        HEMAX_Utilities::GetWideString(Message).c_str());
}

void
HEMAX_Logger::ShowDialog(const char* Title, const char* Message, HEMAX_LogLevel LogLevel)
{
    StoreEntry(Message, LogLevel);

    GetCOREInterface()->Log()->LogEntry(LogLevel, DISPLAY_DIALOG,
        HEMAX_Utilities::GetWideString(Title).c_str(),
        HEMAX_Utilities::GetWideString(Message).c_str());
}

void
HEMAX_Logger::ConfigurePrintLevels(HEMAX_LogLevel LogLevel, bool Print)
{
    switch (LogLevel)
    {
	case HEMAX_LOG_LEVEL_ERROR:
	    PrintErrorLogs = Print;
	    break;
	case HEMAX_LOG_LEVEL_WARN:
	    PrintWarnLogs = Print;
	    break;
	case HEMAX_LOG_LEVEL_INFO:
	    PrintInfoLogs = Print;
	    break;
	default:
	    break;
    }
}

void
HEMAX_Logger::RegisterOnEntryAddedCallback(
    const std::function<void(const HEMAX_LogEntry&)>& CB)
{
    EntryAddedCallbacks.push_back(CB);
}

void
HEMAX_Logger::StoreEntry(const std::string& Log, HEMAX_LogLevel LogLevel)
{
    Entries.emplace_back(LogLevel, Log);
    InvokeOnEntryAddedCallbacks(static_cast<int>(Entries.size() - 1));
}

bool
HEMAX_Logger::ShouldPrint(HEMAX_LogLevel Level)
{
    if (Level == HEMAX_LOG_LEVEL_ERROR && PrintErrorLogs)
    {
	return true;
    }
    else if (Level == HEMAX_LOG_LEVEL_WARN && PrintWarnLogs)
    {
	return true;
    }
    else if (Level == HEMAX_LOG_LEVEL_INFO && PrintInfoLogs)
    {
	return true;
    }
    else
    {
	return false;
    }
}

void
HEMAX_Logger::InvokeOnEntryAddedCallbacks(int EntryIndex)
{
    for (auto&& Callback : EntryAddedCallbacks)
    {
        Callback(Entries[EntryIndex]);
    }
}
