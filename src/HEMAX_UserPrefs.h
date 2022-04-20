#pragma once

#include "maxapi.h"
#include <string>
#include <unordered_map>

#define HEMAX_SETTINGS_FILE_DIRECTORY APP_PLUGCFG_DIR
#define HEMAX_SETTINGS_FILE           "HoudiniEngine.ini"
#define HEMAX_SETTINGS_FILE_APPNAME   "Houdini Engine for 3DS Max"

#define HEMAX_SESSION_TYPE_PREF_AUTO            "AutoStart"
#define HEMAX_SESSION_TYPE_PREF_SOCKET          "Socket"
#define HEMAX_SESSION_TYPE_PREF_NAMED_PIPE      "NamedPipe"

#define HEMAX_SETTING_SESSION_TYPE              "Session\\Type"
#define HEMAX_SETTING_SESSION_HOST_NAME         "Session\\Hostname"
#define HEMAX_SETTING_SESSION_PORT              "Session\\Port"
#define HEMAX_SETTING_SESSION_PIPE_NAME         "Session\\PipeName"
#define HEMAX_SETTING_SESSION_ENV_FILES         "Session\\HoudiniEnvironmentFiles"
#define HEMAX_SETTING_SESSION_OTL_SEARCH        "Session\\OtlSearchPath"
#define HEMAX_SETTING_SESSION_DSO_SEARCH        "Session\\DsoSearchPath"
#define HEMAX_SETTING_SESSION_IMAGE_DSO_SEARCH  "Session\\ImageDsoSearchPath"
#define HEMAX_SETTING_SESSION_AUDIO_DSO_SEARCH  "Session\\AudioDsoSearchPath"

#define HEMAX_SETTING_OVERRIDE_HFS "Installation\\OverrideHFS"
#define HEMAX_SETTING_GRAB_ROOT       "Selection\\GrabHDARootNode"
#define HEMAX_SETTING_AUTO_START_SESSION "OnStart\\AutoStartSession"
#define HEMAX_SETTING_AUTO_START_WINDOW "OnStart\\AutoStartWindow"
#define HEMAX_SETTING_HDA_LOAD_PATH   "OnStart\\LoadHDADirectory"
#define HEMAX_SETTING_HDA_REPO_PATH   "Assets\\HdaRepository"

#define HEMAX_SETTING_NODE_OPTION_AUTORECOOK    "Nodes\\AutoRecook"
#define HEMAX_SETTING_NODE_OPTION_SLIDERCOOK    "Nodes\\SliderCook"

#define HEMAX_SETTING_BAKE_DUMMY_OBJECT "Bake\\DummyObject"
#define HEMAX_SETTING_BAKE_CREATE_LAYER "Bake\\CreateLayer"
#define HEMAX_SETTING_NODE_NAMES_UNIQUE "Naming\\UseUnique"
#define HEMAX_SETTING_NODE_INSTANCE_NAME_ORIGINAL "Naming\\UseOriginalInstanceName"

#define HEMAX_SETTING_DEBUG_TEMP_DIR  "Debug\\TemporaryDirectory"
#define HEMAX_SETTING_DEBUG_PRINT_ERRORS "Debug\\PrintErrorLevelLogs"
#define HEMAX_SETTING_DEBUG_PRINT_WARNINGS "Debug\\PrintWarningLevelLogs"
#define HEMAX_SETTING_DEBUG_PRINT_INFO "Debug\\PrintInfoLevelLogs"

constexpr const char* const NodeOptionAutoRecookLabel
    = "Enable automatic recooking";
constexpr const char* const NodeOptionSliderCookLabel
    = "Cook while dragging parameter slider";

enum class HEMAX_SessionTypePref : int
{
    AutoStart = 0,
    Socket = 1,
    NamedPipe = 2
};

typedef enum
{
    HEMAX_SETTING_TYPE_INVALID,
    HEMAX_SETTING_TYPE_STRING,
    HEMAX_SETTING_TYPE_BOOL,
    HEMAX_SETTING_TYPE_INT
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
        static HEMAX_UserPrefs& Get();

	~HEMAX_UserPrefs();

	bool GetStringSetting(std::string Key, std::string& Out);
	bool GetBoolSetting(std::string Key, bool& Out);
        bool GetIntSetting(std::string Key, int& Out);

	bool SetStringSetting(std::string Key, std::string In);
	bool SetBoolSetting(std::string Key, bool In);
        bool SetIntSetting(std::string Key, int In);

        HEMAX_UserPrefs_Setting_Type GetSettingType(const std::string& Key) const;

	std::string GetPluginConfigFolder();

    private:
	HEMAX_UserPrefs();

	std::wstring GetConfigFilePath();
	void AddUserSetting(std::string Name,
                            HEMAX_UserPrefs_Setting_Type SettingType,
                            std::string Value);

	void WriteAllSettings();

	std::unordered_map<std::string, HEMAX_UserPrefs_Setting> UserSettings;

	bool FoundPrefFile;
	std::wstring FullCfgFilePath;
};
