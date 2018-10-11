#include "HEMAX_Logger.h"

#include <maxapi.h>
#include <maxscript/maxscript.h>

HEMAX_Logger&
HEMAX_Logger::Instance()
{
    static HEMAX_Logger TheInstance;
    return TheInstance;
}

HEMAX_Logger::~HEMAX_Logger() {}

HEMAX_Logger::HEMAX_Logger() {}

void
HEMAX_Logger::AddEntry(std::string Log, HEMAX_LogLevel LogLevel)
{
    GetCOREInterface()->Log()->LogEntry(LogLevel, NO_DIALOG, nullptr, ConvertToWideChar(Log));

    if (LogLevel == HEMAX_LOG_LEVEL_WARN || HEMAX_LOG_LEVEL_ERROR)
    {
        mprintf(ConvertToWideChar(Log + "\n"));
        mflush();
    }
}

void
HEMAX_Logger::AddEntry(const char* Log, HEMAX_LogLevel LogLevel)
{
    GetCOREInterface()->Log()->LogEntry(LogLevel, NO_DIALOG, nullptr, ConvertToWideChar(Log));

    if (LogLevel == HEMAX_LOG_LEVEL_WARN || HEMAX_LOG_LEVEL_ERROR)
    {
        std::string MaxscriptListenerString = std::string(Log) + "\n";
        mprintf(ConvertToWideChar(MaxscriptListenerString));
        mflush();
    }
}

void
HEMAX_Logger::ShowDialog(std::string Title, std::string Message, HEMAX_LogLevel LogLevel)
{
    GetCOREInterface()->Log()->LogEntry(LogLevel, DISPLAY_DIALOG, ConvertToWideChar(Title), ConvertToWideChar(Message));
}

void
HEMAX_Logger::ShowDialog(const char* Title, const char* Message, HEMAX_LogLevel LogLevel)
{
    GetCOREInterface()->Log()->LogEntry(LogLevel, DISPLAY_DIALOG, ConvertToWideChar(Title), ConvertToWideChar(Message));
}

const wchar_t*
HEMAX_Logger::ConvertToWideChar(std::string Msg)
{
    std::wstring WideString(Msg.begin(), Msg.end());
    return WideString.c_str();
}

const wchar_t*
HEMAX_Logger::ConvertToWideChar(const char* Msg)
{
    std::string AString(Msg);
    return ConvertToWideChar(AString);
}
