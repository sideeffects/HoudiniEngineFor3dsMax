#pragma once

#include <log.h>

#include <string>
#include <vector>

typedef enum
{
    HEMAX_LOG_LEVEL_ERROR = SYSLOG_ERROR,
    HEMAX_LOG_LEVEL_WARN  = SYSLOG_WARN,
    HEMAX_LOG_LEVEL_INFO  = SYSLOG_INFO,
    HEMAX_LOG_LEVEL_DEBUG = SYSLOG_DEBUG
} HEMAX_LogLevel;

class HEMAX_Logger
{
    public:
	static HEMAX_Logger& Instance();

	void AddEntry(const std::string& Log, HEMAX_LogLevel LogLevel);
	void AddEntry(const char* Log, HEMAX_LogLevel LogLevel);

        void AddEntry(const std::wstring& Log, HEMAX_LogLevel LogLevel);

	void ShowDialog(std::string Title, std::string Message, HEMAX_LogLevel LogLevel);
	void ShowDialog(const char* Title, const char* Message, HEMAX_LogLevel LogLevel);

	void ConfigurePrintLevels(HEMAX_LogLevel LogLevel, bool Print);

	~HEMAX_Logger();
    private:
	HEMAX_Logger();

        std::wstring ConvertToWideString(const std::string& Msg);

	bool ShouldPrint(HEMAX_LogLevel Level);

	bool PrintErrorLogs;
	bool PrintWarnLogs;
	bool PrintInfoLogs;
};
