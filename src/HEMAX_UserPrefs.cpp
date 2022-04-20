#include "HEMAX_UserPrefs.h"

#include "HEMAX_Logger.h"
#include "HEMAX_Utilities.h"
#include <Windows.h>
#include <json.hpp>

#define HEMAX_SESSION_PIPE_NAME_DEFAULT "hapi"
#define HEMAX_SESSION_HOST_NAME_DEFAULT "localhost"
#define HEMAX_SESSION_PORT_NUM_DEFAULT  "9090"

HEMAX_UserPrefs&
HEMAX_UserPrefs::Get()
{
    static HEMAX_UserPrefs UserPrefs;
    return UserPrefs;
}

HEMAX_UserPrefs::HEMAX_UserPrefs()
{
    AddUserSetting(HEMAX_SETTING_SESSION_TYPE,
        HEMAX_SETTING_TYPE_INT,
        std::to_string(static_cast<int>(HEMAX_SessionTypePref::AutoStart)));
    AddUserSetting(HEMAX_SETTING_SESSION_HOST_NAME,
                   HEMAX_SETTING_TYPE_STRING,
                   HEMAX_SESSION_HOST_NAME_DEFAULT);
    AddUserSetting(HEMAX_SETTING_SESSION_PORT,
                   HEMAX_SETTING_TYPE_INT,
                   HEMAX_SESSION_PORT_NUM_DEFAULT);
    AddUserSetting(HEMAX_SETTING_SESSION_PIPE_NAME,
                   HEMAX_SETTING_TYPE_STRING,
                   HEMAX_SESSION_PIPE_NAME_DEFAULT);
    AddUserSetting(HEMAX_SETTING_SESSION_ENV_FILES,
                   HEMAX_SETTING_TYPE_STRING,
                   "");
    AddUserSetting(HEMAX_SETTING_SESSION_OTL_SEARCH,
                   HEMAX_SETTING_TYPE_STRING,
                   "");
    AddUserSetting(HEMAX_SETTING_SESSION_DSO_SEARCH,
                   HEMAX_SETTING_TYPE_STRING,
                   "");
    AddUserSetting(HEMAX_SETTING_SESSION_IMAGE_DSO_SEARCH,
                   HEMAX_SETTING_TYPE_STRING,
                   "");
    AddUserSetting(HEMAX_SETTING_SESSION_AUDIO_DSO_SEARCH,
                   HEMAX_SETTING_TYPE_STRING,
                   "");
    AddUserSetting(HEMAX_SETTING_OVERRIDE_HFS,
                   HEMAX_SETTING_TYPE_STRING,
                   "");
    AddUserSetting(HEMAX_SETTING_GRAB_ROOT,
                   HEMAX_SETTING_TYPE_BOOL,
                   "1");
    AddUserSetting(HEMAX_SETTING_AUTO_START_SESSION,
                   HEMAX_SETTING_TYPE_BOOL,
                   "0");
    AddUserSetting(HEMAX_SETTING_AUTO_START_WINDOW,
                   HEMAX_SETTING_TYPE_BOOL,
                   "1");
    AddUserSetting(HEMAX_SETTING_HDA_LOAD_PATH,
                   HEMAX_SETTING_TYPE_STRING,
                   "");
    AddUserSetting(HEMAX_SETTING_NODE_OPTION_AUTORECOOK,
                   HEMAX_SETTING_TYPE_BOOL,
                   "1");
    AddUserSetting(HEMAX_SETTING_NODE_OPTION_SLIDERCOOK,
                   HEMAX_SETTING_TYPE_BOOL,
                   "0");
    AddUserSetting(HEMAX_SETTING_HDA_REPO_PATH,
                   HEMAX_SETTING_TYPE_STRING,
                   "");
    AddUserSetting(HEMAX_SETTING_BAKE_DUMMY_OBJECT,
                   HEMAX_SETTING_TYPE_BOOL,
                   "1");
    AddUserSetting(HEMAX_SETTING_BAKE_CREATE_LAYER,
                   HEMAX_SETTING_TYPE_BOOL,
                   "1");
    AddUserSetting(HEMAX_SETTING_NODE_NAMES_UNIQUE,
                   HEMAX_SETTING_TYPE_BOOL,
                   "0");
    AddUserSetting(HEMAX_SETTING_NODE_INSTANCE_NAME_ORIGINAL,
                   HEMAX_SETTING_TYPE_BOOL,
                   "1");
    AddUserSetting(HEMAX_SETTING_DEBUG_TEMP_DIR,
                   HEMAX_SETTING_TYPE_STRING,
                   "");
    AddUserSetting(HEMAX_SETTING_DEBUG_PRINT_ERRORS,
                   HEMAX_SETTING_TYPE_BOOL,
                   "1");
    AddUserSetting(HEMAX_SETTING_DEBUG_PRINT_WARNINGS,
                   HEMAX_SETTING_TYPE_BOOL,
                   "1");
    AddUserSetting(HEMAX_SETTING_DEBUG_PRINT_INFO,
                   HEMAX_SETTING_TYPE_BOOL,
                   "0");

    std::wstring UserPrefsFile = GetConfigFilePath();

    if (UserPrefsFile != L"")
    {
	FoundPrefFile = true;
	FullCfgFilePath = UserPrefsFile;

	LPTSTR Output = new wchar_t[2048];

	for (auto Iter = UserSettings.begin();
             Iter != UserSettings.end();
             Iter++)
	{
	    std::wstring Key_Wide(Iter->second.SettingKey.begin(),
                                  Iter->second.SettingKey.end());
	    std::wstring Value_Wide(Iter->second.SettingValue.begin(),
                                    Iter->second.SettingValue.end());
	    GetPrivateProfileString(_T(HEMAX_SETTINGS_FILE_APPNAME),
                                    Key_Wide.c_str(),
                                    Value_Wide.c_str(),
                                    Output,
                                    2048,
                                    UserPrefsFile.c_str());

	    std::string RealValue
		    = HEMAX_Utilities::WideStringToStringUnsafe(Output);
	    Iter->second.SettingValue = RealValue;
	}

	delete[] Output;
    }
    else
    {
	FoundPrefFile = false;
    }
}

HEMAX_UserPrefs::~HEMAX_UserPrefs()
{
    WriteAllSettings();
}

bool
HEMAX_UserPrefs::GetStringSetting(std::string Key, std::string& Out)
{
    auto Search = UserSettings.find({ Key });

    if (Search != UserSettings.end())
    {
	if (Search->second.Type != HEMAX_SETTING_TYPE_STRING)
	{
	    return false;
	}

	Out = Search->second.SettingValue;
    }
    else
    {
	return false;
    }

    return true;
}

bool
HEMAX_UserPrefs::GetBoolSetting(std::string Key, bool& Out)
{
    auto Search = UserSettings.find({ Key });

    if (Search != UserSettings.end())
    {
	if (Search->second.Type != HEMAX_SETTING_TYPE_BOOL)
	{
	    return false;
	}

	Out = (std::stoi(Search->second.SettingValue) != 0);
    }
    else
    {
	return false;
    }

    return true;
}

bool
HEMAX_UserPrefs::GetIntSetting(std::string Key, int& Out)
{
    auto Search = UserSettings.find({Key});
    if (Search != UserSettings.end())
    {
        if (Search->second.Type != HEMAX_SETTING_TYPE_INT)
        {
            return false;
        }

        Out = std::stoi(Search->second.SettingValue);
    }
    else
    {
        return false;
    }

    return true;
}

bool
HEMAX_UserPrefs::SetStringSetting(std::string Key, std::string In)
{
    auto Search = UserSettings.find({ Key });

    if (Search != UserSettings.end())
    {
	if (Search->second.Type != HEMAX_SETTING_TYPE_STRING)
	{
	    return false;
	}

	Search->second.SettingValue = In;
    }
    else
    {
	return false;
    }

    return true;
}

bool
HEMAX_UserPrefs::SetBoolSetting(std::string Key, bool In)
{
        auto Search = UserSettings.find({ Key });

    if (Search != UserSettings.end())
    {
	if (Search->second.Type != HEMAX_SETTING_TYPE_BOOL)
	{
	    return false;
	}

	std::string Value;

	if (In)
	{
	    Value = "1";
	}
	else
	{
	    Value = "0";
	}

	Search->second.SettingValue = Value;
    }
    else
    {
	return false;
    }

    return true;
}

bool
HEMAX_UserPrefs::SetIntSetting(std::string Key, int In)
{
    auto Search = UserSettings.find({Key});
    if (Search != UserSettings.end())
    {
        if (Search->second.Type != HEMAX_SETTING_TYPE_INT)
        {
            return false;
        }

        std::string Value = std::to_string(In);
        Search->second.SettingValue = Value;
    }
    else
    {
        return false;
    }

    return true;
}

HEMAX_UserPrefs_Setting_Type
HEMAX_UserPrefs::GetSettingType(const std::string& Key) const
{
    auto PrefPair = UserSettings.find(Key);

    if (PrefPair != UserSettings.cend())
    {
        return PrefPair->second.Type;
    }
    else
    {
        return HEMAX_SETTING_TYPE_INVALID;
    }
}

std::string
HEMAX_UserPrefs::GetPluginConfigFolder()
{
    return HEMAX_Utilities::WideStringToStringUnsafe(
    		GetCOREInterface()->GetDir(HEMAX_SETTINGS_FILE_DIRECTORY));
}

std::wstring
HEMAX_UserPrefs::GetConfigFilePath()
{
    std::wstring CfgDir(GetCOREInterface()->GetDir(
                                            HEMAX_SETTINGS_FILE_DIRECTORY));

    std::wstring FullPluginCfgPath = L"";

    DWORD Result = GetFileAttributes(CfgDir.c_str());

    if (Result == INVALID_FILE_ATTRIBUTES)
    {
	HEMAX_Logger::Instance().AddEntry("HEMAX_UserPrefs::the folder used "
                "for user settings does not exist or there is a problem",
                HEMAX_LOG_LEVEL_WARN);
    }
    else if (Result & FILE_ATTRIBUTE_DIRECTORY)
    {
	FullPluginCfgPath = CfgDir + L"\\" + _T(HEMAX_SETTINGS_FILE);
    }

    return FullPluginCfgPath;
}

void
HEMAX_UserPrefs::AddUserSetting(std::string Name,
                                HEMAX_UserPrefs_Setting_Type SettingType,
                                std::string Value)
{
    HEMAX_UserPrefs_Setting Setting;
    Setting.SettingKey = Name;
    Setting.Type = SettingType;
    Setting.SettingValue = Value;

    UserSettings.insert({ Name, Setting });
}

void
HEMAX_UserPrefs::WriteAllSettings()
{
    if (FoundPrefFile)
    {
	for (auto Iter = UserSettings.begin();
             Iter != UserSettings.end();
             Iter++)
	{
	    std::wstring Key_Wide(Iter->second.SettingKey.begin(),
                                  Iter->second.SettingKey.end());
	    std::wstring Value_Wide(Iter->second.SettingValue.begin(),
                                    Iter->second.SettingValue.end());
	    WritePrivateProfileString(_T(HEMAX_SETTINGS_FILE_APPNAME),
                                      Key_Wide.c_str(),
                                      Value_Wide.c_str(),
                                      FullCfgFilePath.c_str());
	}
    }
}
