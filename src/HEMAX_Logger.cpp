#include "HEMAX_Logger.h"

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

HEMAX_Logger::HEMAX_Logger() {}

void
HEMAX_Logger::AddEntry(const std::string& Log, HEMAX_LogLevel LogLevel)
{
    GetCOREInterface()->Log()->LogEntry(LogLevel, NO_DIALOG, nullptr,
        ConvertToWideString(Log).c_str());

    if (ShouldPrint(LogLevel))
    {
	mprintf(ConvertToWideString(Log + "\n").c_str());
	mflush();
    }
}

void
HEMAX_Logger::AddEntry(const char* Log, HEMAX_LogLevel LogLevel)
{
    GetCOREInterface()->Log()->LogEntry(LogLevel, NO_DIALOG, nullptr,
        ConvertToWideString(Log).c_str());

    if (ShouldPrint(LogLevel))
    {
	std::string MaxscriptListenerString = std::string(Log) + "\n";
	mprintf(ConvertToWideString(MaxscriptListenerString).c_str());
	mflush();
    }
}

void
HEMAX_Logger::AddEntry(const std::wstring& Log, HEMAX_LogLevel LogLevel)
{
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
    GetCOREInterface()->Log()->LogEntry(LogLevel, DISPLAY_DIALOG,
        ConvertToWideString(Title).c_str(),
        ConvertToWideString(Message).c_str());
}

void
HEMAX_Logger::ShowDialog(const char* Title, const char* Message, HEMAX_LogLevel LogLevel)
{
    GetCOREInterface()->Log()->LogEntry(LogLevel, DISPLAY_DIALOG,
        ConvertToWideString(Title).c_str(),
        ConvertToWideString(Message).c_str());
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

std::wstring
HEMAX_Logger::ConvertToWideString(const std::string& Msg)
{
    std::wstring WideString(Msg.begin(), Msg.end());
    return WideString;
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
