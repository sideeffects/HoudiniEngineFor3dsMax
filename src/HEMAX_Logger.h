#pragma once

#pragma warning(push, 0)
#include <log.h>
#pragma warning(pop)

#include <functional>
#include <string>
#include <vector>

typedef enum
{
    HEMAX_LOG_LEVEL_ERROR = SYSLOG_ERROR,
    HEMAX_LOG_LEVEL_WARN  = SYSLOG_WARN,
    HEMAX_LOG_LEVEL_INFO  = SYSLOG_INFO,
    HEMAX_LOG_LEVEL_DEBUG = SYSLOG_DEBUG
} HEMAX_LogLevel;

struct HEMAX_LogEntry
{
    HEMAX_LogEntry(HEMAX_LogLevel TheLevel, const std::string& TheText)
        : Level(TheLevel)
        , Text(TheText)
    {

    }

    HEMAX_LogEntry(HEMAX_LogEntry&& Other)
        : Level(Other.Level)
        , Text(std::move(Other.Text))
    {

    }
    
    HEMAX_LogEntry& operator=(HEMAX_LogEntry& Other)
    {
        Level = Other.Level;
        Text = Other.Text;
    }

    HEMAX_LogLevel  Level;
    std::string     Text;
};

class HEMAX_Logger
{
    public:
	static HEMAX_Logger& Instance();

	void AddEntry(const std::string& Log, HEMAX_LogLevel LogLevel);
	void AddEntry(const char* Log, HEMAX_LogLevel LogLevel);

        void AddEntry(const std::wstring& Log, HEMAX_LogLevel LogLevel);

	void ShowDialog(std::string Title, std::string Message, HEMAX_LogLevel LogLevel);
	void ShowDialog(const char* Title, const char* Message, HEMAX_LogLevel LogLevel);

        const std::vector<HEMAX_LogEntry>& GetEntries() { return Entries; }

	void ConfigurePrintLevels(HEMAX_LogLevel LogLevel, bool Print);

        void RegisterOnEntryAddedCallback(
            const std::function<void(const HEMAX_LogEntry&)>& CB);

	~HEMAX_Logger();
    private:
	HEMAX_Logger();

        void StoreEntry(const std::string& Log, HEMAX_LogLevel LogLevel);

	bool ShouldPrint(HEMAX_LogLevel Level);

        void InvokeOnEntryAddedCallbacks(int EntryIndex);

	bool PrintErrorLogs;
	bool PrintWarnLogs;
	bool PrintInfoLogs;

        std::vector<HEMAX_LogEntry> Entries;

        std::vector<std::function<void(const HEMAX_LogEntry&)> > EntryAddedCallbacks;
};
