#pragma once

#include "maxapi.h"
#include <string>
#include <unordered_map>

#define HEMAX_SETTINGS_FILE_DIRECTORY APP_PLUGCFG_DIR
#define HEMAX_SETTINGS_FILE           "HoudiniEngine.ini"
#define HEMAX_SETTINGS_FILE_APPNAME   "Houdini Engine for 3DS Max"

#define HEMAX_SETTING_SESSION_ENV_FILES        "Session\\HoudiniEnvironmentFiles"
#define HEMAX_SETTING_SESSION_OTL_SEARCH       "Session\\OtlSearchPath"
#define HEMAX_SETTING_SESSION_DSO_SEARCH       "Session\\DsoSearchPath"
#define HEMAX_SETTING_SESSION_IMAGE_DSO_SEARCH "Session\\ImageDsoSearchPath"
#define HEMAX_SETTING_SESSION_AUDIO_DSO_SEARCH "Session\\AudioDsoSearchPath"

#define HEMAX_SETTING_GRAB_ROOT       "Selection\\GrabHDARootNode"
#define HEMAX_SETTING_AUTO_START_SESSION "OnStart\\AutoStartSession"
#define HEMAX_SETTING_AUTO_START_WINDOW "OnStart\\AutoStartWindow"
#define HEMAX_SETTING_HDA_LOAD_PATH   "OnStart\\LoadHDADirectory"
#define HEMAX_SETTING_HDA_REPO_PATH   "Assets\\HdaRepository"

#define HEMAX_SETTING_DEBUG_TEMP_DIR  "Debug\\TemporaryDirectory"
#define HEMAX_SETTING_DEBUG_PRINT_ERRORS "Debug\\PrintErrorLevelLogs"
#define HEMAX_SETTING_DEBUG_PRINT_WARNINGS "Debug\\PrintWarningLevelLogs"
#define HEMAX_SETTING_DEBUG_PRINT_INFO "Debug\\PrintInfoLevelLogs"

typedef enum
{
    HEMAX_SETTING_TYPE_INVALID,
    HEMAX_SETTING_TYPE_STRING,
    HEMAX_SETTING_TYPE_BOOL
} HEMAX_UserPrefs_Setting_Type;

struct HEMAX_UserPrefs_Setting
{
    HEMAX_UserPrefs_Setting_Type Type;
    std::string SettingKey;
    std::string SettingValue;
};

class HEMAX_UserPrefs
{
public:
    HEMAX_UserPrefs();
    ~HEMAX_UserPrefs();

    bool GetStringSetting(std::string Key, std::string& Out);
    bool GetBoolSetting(std::string Key, bool& Out);

    bool SetStringSetting(std::string Key, std::string In);
    bool SetBoolSetting(std::string Key, bool In);

    std::string GetPluginConfigFolder();

private:
    std::wstring GetConfigFilePath();
    void AddUserSetting(std::string Name, HEMAX_UserPrefs_Setting_Type SettingType, std::string Value);

    void WriteAllSettings();

    std::unordered_map<std::string, HEMAX_UserPrefs_Setting> UserSettings;

    bool FoundPrefFile;
    std::wstring FullCfgFilePath;
};
