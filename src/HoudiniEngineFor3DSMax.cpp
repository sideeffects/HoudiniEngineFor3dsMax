#include "HoudiniEngineFor3DSMax.h"

#include "HEMAX_Events.h"
#include "HEMAX_Logger.h"
#include "HEMAX_MaxScriptInterface.h"
#include "HEMAX_Path.h"
#include "HEMAX_Plugin.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_UserPrefs.h"
#include "HEMAX_Utilities.h"

#include "UI/HEMAX_OptionsDialog.h"
#include "UI/HEMAX_UI.h"
#include "UI/HEMAX_VersionDialog.h"

#ifdef USE_HAPI_VERSION_HEADER
#include <HAPI_Version.h>
#endif

#include <fstream>
#include <sstream>

#pragma warning(push, 0)
#include <maxapi.h>
#include <maxscript/maxscript.h>
#pragma warning(pop)

#ifdef HEMAX_VERSION_2025
#include <CUI/ICuiMenuManager.h>
#include <CUI/ICuiMenu.h>
#endif

#ifdef HEMAX_VERSION_2025
MaxSDK::MaxGuid HoudiniEngineMenuGuid("47dec1b8-78b6-44e3-ad16-f690946856d2");
MaxSDK::MaxGuid HoudiniEngineOpenActionGuid("01a24d08-9f84-48ed-8b58-476d88f6b470");
MaxSDK::MaxGuid HoudiniEngineHideActionGuid("f1e8ae17-aca1-4fc6-84fe-82ea0a964582");
MaxSDK::MaxGuid HoudiniEngineOptionsActionGuid("204aca3d-98e7-4505-a43a-0b64c2ecb760");
MaxSDK::MaxGuid HoudiniEngineVersionActionGuid("5076703e-a2d5-4103-9c4b-b6e008e0a396");

MaxSDK::MaxGuid HoudiniEngineSeparator1Guid("029dc07e-f811-4989-9692-7408e0ef6353");
MaxSDK::MaxGuid HoudiniEngineSeparator2Guid("9f32ff4c-a8fe-435c-a36e-78eaafcba655");
#endif

#define OPEN_HEMAX_ACTION    1
#define HIDE_HEMAX_ACTION    2
#define OPTIONS_HEMAX_ACTION 3
#define VERSION_HEMAX_ACTION 4

const wchar_t* const LIB_HAPIL_NAME = L"libHAPIL.dll";
const wchar_t* const LIB_HAPIL_SUBDIRECTORY = L"bin";
const wchar_t* const HOUDINI_TOOLS_SUBDIRECTORY = L"engine\\tools";
const wchar_t* const HOUDINI_HARS_LOCATION_ENV_VAR = L"HOUDINI_HARS_LOCATION";
const wchar_t* const HOUDINI_HFS_ENV_VAR = L"HFS";

static ActionDescription MenuActions[] = {
    OPEN_HEMAX_ACTION,
    IDS_HEMAX_MENU_ACTION,
    IDS_HEMAX_MENU_ACTION_DESC,
    IDS_CATEGORY,

    HIDE_HEMAX_ACTION,
    IDS_HEMAX_MENU_HIDE,
    IDS_HEMAX_MENU_HIDE_DESC,
    IDS_CATEGORY,

    OPTIONS_HEMAX_ACTION,
    IDS_HEMAX_MENU_OPTIONS,
    IDS_HEMAX_MENU_OPTIONS_DESC,
    IDS_CATEGORY,

    VERSION_HEMAX_ACTION,
    IDS_HEMAX_MENU_VERSION,
    IDS_HEMAX_MENU_VERSION_DESC,
    IDS_CATEGORY
};

HEMAXActionTable::HEMAXActionTable()
    : ActionTable(HEMAX_Actions_Id,
                  HEMAX_Context_Id,
                  TSTR(GetString(IDS_CATEGORY)),
                  nullptr,
                  sizeof(MenuActions) / sizeof(MenuActions[0]),
      MenuActions,
      hInstance)
{

}

HEMAXActionTable::~HEMAXActionTable() {}

ClassDesc2*
GetHEMAXLauncherDesc() { 
    static HEMAXLauncherClassDesc HEMAXLauncherDesc;
    return &HEMAXLauncherDesc; 
}

HEMAXLauncher::HEMAXLauncher()
    : FoundHAPIDLL(false)
{
}

HEMAXLauncher::~HEMAXLauncher() {}

HMODULE
HEMAXLauncher::LoadLibHAPIL(const std::wstring& HFS)
{
    std::wstring HFSBinDir = HFS + L"\\" + LIB_HAPIL_SUBDIRECTORY;
    SetDllDirectory(HFSBinDir.c_str());

    HMODULE libHAPIL = LoadLibrary(LIB_HAPIL_NAME);
    FoundHAPIDLL = (libHAPIL != 0);

    if (libHAPIL)
    {
        // Prepend HFS to PATH so we can invoke HARS later
        std::wstringstream PathStream;
        PathStream << HFSBinDir << L";";

        wchar_t SystemPath[4096];
        int Count = GetEnvironmentVariable(L"PATH", SystemPath, 4096);

        if (Count <= 4096)
        {
            PathStream << SystemPath;
        }
        else
        {
            wchar_t* SystemPathLong = new wchar_t[Count];
            GetEnvironmentVariable(L"PATH", SystemPathLong, Count);
            PathStream << SystemPathLong;
            delete [] SystemPathLong;
        }

        PathStream << "\0";

        std::wstring PathVar = PathStream.str();
        int CallResult = SetEnvironmentVariable(L"PATH", PathVar.c_str());

        if (!CallResult)
        {
            HEMAX_Logger::Instance().AddEntry(
                "Failed to prepend the $HFS/bin directory to the %%PATH%% "
                "variable.",
                HEMAX_LOG_LEVEL_ERROR);
        }
    }

    SetDllDirectoryA(nullptr);
    return libHAPIL;
}

HMODULE
HEMAXLauncher::FindHoudiniEngineLibs()
{
    // This is the process of tracking down HFS
    
    // Option 1: Check for HFS Override
    std::string HFSOverride;
    if (HEMAX_UserPrefs::Get().GetStringSetting(HEMAX_SETTING_OVERRIDE_HFS, HFSOverride))
    {
        std::wstring HFSOverrideWide(HFSOverride.begin(), HFSOverride.end());
        HMODULE libHAPILModule = LoadLibHAPIL(HFSOverrideWide);
        if (libHAPILModule)
        {
            SetHoudiniDirectories(HFSOverrideWide);
            return libHAPILModule;
        }     
    }

    // Option 2: Find HFS in the Registry

    std::string HoudiniVersionString =
        HEMAX_Utilities::CreateHoudiniVersionString(HoudiniMajorVersion,
                                                    HoudiniMinorVersion,
                                                    HoudiniBuildVersion,
                                                    HoudiniPatchNumber);

    HKEY Key;
    std::string HoudiniRegPath = HEMAX_Utilities::GetHoudiniRegistryPath(
                                                        HoudiniVersionString);

    LONG Result = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                                HoudiniRegPath.c_str(),
                                0,
                                KEY_READ,
                                &Key);

    if (Result == ERROR_SUCCESS)
    {
	WCHAR StringValue[2048];
	DWORD BufferSize = sizeof(StringValue);
	Result = RegQueryValueEx(Key,
                                 _T(HEMAX_HOUDINI_REGISTRY_INSTALL_PATH_NAME),
                                 nullptr,
                                 nullptr,
                                 (LPBYTE)(StringValue),
                                 &BufferSize);

	if (Result == ERROR_SUCCESS)
	{
	    HMODULE libHAPILModule = LoadLibHAPIL(StringValue);
	    if (libHAPILModule)
	    {
                SetHoudiniDirectories(StringValue);
		return libHAPILModule;
	    }
	}
    }

    // Option 3: Find HFS in the Steam Registry location

    std::string HoudiniSteamRegPath =
        HEMAX_Utilities::GetHoudiniSteamRegistryPath(HoudiniVersionString);

    Result = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                           HoudiniSteamRegPath.c_str(),
                           0,
                           KEY_READ,
                           &Key);

    if (Result == ERROR_SUCCESS)
    {
	WCHAR StringValue[2048];
	DWORD BufferSize = sizeof(StringValue);
	Result = RegQueryValueEx(Key,
                                 _T(HEMAX_HOUDINI_REGISTRY_INSTALL_PATH_NAME),
                                 nullptr,
                                 nullptr,
                                 (LPBYTE)(StringValue),
                                 &BufferSize);

	if (Result == ERROR_SUCCESS)
	{
	    HMODULE libHAPILModule = LoadLibHAPIL(StringValue);
	    if (libHAPILModule)
	    {
                SetHoudiniDirectories(StringValue);
		return libHAPILModule;
	    }
	}
    }

    // Option 4: Use the path stored in the HFS environment variable if it exists

    wchar_t HFSEnvVal[4096];
    int Count = GetEnvironmentVariable(HOUDINI_HFS_ENV_VAR, HFSEnvVal, 4096);

    if (Count > 0 && Count <= 4096)
    {
        std::wstring HFSEnvValStr(HFSEnvVal);
        HMODULE libHAPILModule = LoadLibHAPIL(HFSEnvValStr);
        if (libHAPILModule)
        {
            SetHoudiniDirectories(HFSEnvValStr);
            return libHAPILModule; 
        } 
    }
    else if (Count > 4096)
    {
        wchar_t* HFSEnvValLong = new wchar_t[Count];
        GetEnvironmentVariable(HOUDINI_HFS_ENV_VAR, HFSEnvValLong, Count);
        std::wstring HFSEnvValStr(HFSEnvValLong);
        delete [] HFSEnvValLong;
        HMODULE libHAPILModule = LoadLibHAPIL(HFSEnvValStr);
        if (libHAPILModule)
        {
            SetHoudiniDirectories(HFSEnvValStr);
            return libHAPILModule;
        }
    }

    // Option 5: As a worst case, try looking where Houdini usually is

    std::wstring DefaultHoudiniLocation =
        L"C:\\Program Files\\Side Effects Software\\" +
        std::wstring(HoudiniVersionString.begin(), HoudiniVersionString.end());
    HMODULE libHAPILModule = LoadLibHAPIL(DefaultHoudiniLocation);
    if (libHAPILModule)
    {
        SetHoudiniDirectories(DefaultHoudiniLocation);
        return libHAPILModule;
    }

    // If we are here, it means that we couldn't find the .dlls

    HEMAX_Logger::Instance().ShowDialog("Houdini Engine Not Found",
	    "Houdini Engine could not be found. "
            "Please refer to the documentation for help.",
	    HEMAX_LOG_LEVEL_ERROR);

    return nullptr;
}

void
HEMAXLauncher::SetHoudiniDirectories(const std::wstring& HFS)
{
    HFSDirectory = HFS;
    HAPIToolsDirectory = HFS + L"\\" + HOUDINI_TOOLS_SUBDIRECTORY;
    LibHAPILDirectory = HFS + L"\\" + LIB_HAPIL_SUBDIRECTORY;

    std::string HFS_String(HFS.begin(), HFS.end());
    HEMAX_Path::HEMAX_PathPrefix_HFS_Resolved = HFS_String;
}

void
HEMAXLauncher::DeleteThis()
{
    if (PluginUserInterface)
    {
#ifdef HEMAX_VERSION_2017
	delete PluginUserInterface;
#endif
	PluginUserInterface = nullptr;
    }
    if (ThePlugin)
    {
	delete ThePlugin;
	ThePlugin = nullptr;
    }
}


DWORD
HEMAXLauncher::Start()
{
    HoudiniMajorVersion = HAPI_VERSION_HOUDINI_MAJOR;
    HoudiniMinorVersion = HAPI_VERSION_HOUDINI_MINOR;
    HoudiniBuildVersion = HAPI_VERSION_HOUDINI_BUILD;
    HoudiniPatchNumber = HAPI_VERSION_HOUDINI_PATCH;

    HMODULE HAPIL = FindHoudiniEngineLibs();

    if (HAPIL)
    {
        // Set HOUDINI_HARS_LOCATION   
        std::wstring HARSProgram = std::wstring(LibHAPILDirectory) +
                                  L"\\" +
                                  L"HARS.exe"; 

        if (GetFileAttributes(HARSProgram.c_str()) != INVALID_FILE_ATTRIBUTES)
        {
            int CallResult = SetEnvironmentVariable(
                HOUDINI_HARS_LOCATION_ENV_VAR, HARSProgram.c_str());

            if (!CallResult)
            {
                std::wstring LogMsg = std::wstring(L"Failed to set the ") +
                    HOUDINI_HARS_LOCATION_ENV_VAR + L" environment variable.";
                HEMAX_Logger::Instance().AddEntry(LogMsg, HEMAX_LOG_LEVEL_ERROR);
                LogMsg = L"Windows Error Message: " +
                    std::to_wstring(GetLastError());
                HEMAX_Logger::Instance().AddEntry(LogMsg, HEMAX_LOG_LEVEL_ERROR);
            }
        }

	Interface* TheInterface = GetCOREInterface();

	ThePlugin = new HEMAX_Plugin(TheInterface, HAPIL);

#ifdef HEMAX_VERSION_2017
        PluginUserInterface = new HEMAX_UI(nullptr, ThePlugin);
#else
        PluginUserInterface = new HEMAX_UI(TheInterface->GetQmaxMainWindow(),
                                           ThePlugin);
#endif

        HEMAX_MaxScriptInterface::PluginInstance = ThePlugin;
        HEMAX_MaxScriptInterface::PluginUserInterface = PluginUserInterface;

        PluginEvents = new HEMAX_Events(PluginUserInterface);
        ThePlugin->SetEventHub(PluginEvents);
        HEMAX_SessionManager::GetSessionManager().SetEventHub(PluginEvents);

        ThePlugin->Init(std::string(HAPIToolsDirectory.begin(),
            HAPIToolsDirectory.end()));

        OptionsDialog = new HEMAX_OptionsDialog(ThePlugin);
        OptionsDialog->hide();

	VersionDialog = new HEMAX_VersionDialog();
	VersionDialog->hide();
    }

#ifndef HEMAX_VERSION_2025
    RegisterNotification(&HEMAXLauncher::OnCUIMenusLoaded, this, NOTIFY_CUI_MENUS_POST_LOAD);
    RegisterNotification(&HEMAXLauncher::OnCUIMenusPreSaved, this, NOTIFY_CUI_MENUS_PRE_SAVE);
    RegisterNotification(&HEMAXLauncher::OnCUIMenusPostSaved, this, NOTIFY_CUI_MENUS_POST_SAVE);
    InstallMenu(nullptr);
#endif

    return GUPRESULT_KEEP;
}

void
HEMAXLauncher::Stop()
{
#ifndef HEMAX_VERSION_2025
    UnRegisterNotification(&HEMAXLauncher::OnCUIMenusLoaded, this, NOTIFY_CUI_MENUS_POST_LOAD);
    UnRegisterNotification(&HEMAXLauncher::OnCUIMenusPreSaved, this, NOTIFY_CUI_MENUS_PRE_SAVE);
    UnRegisterNotification(&HEMAXLauncher::OnCUIMenusPostSaved, this, NOTIFY_CUI_MENUS_POST_SAVE);
#endif
    RemoveMenu();
}

DWORD_PTR
HEMAXLauncher::Control(DWORD)
{
    return 0;
}

bool
HEMAXLauncher::WasHAPIDLLFound()
{
    return FoundHAPIDLL;
}

std::wstring
HEMAXLauncher::GetLibHAPILDirectory()
{
    return LibHAPILDirectory;
}

void
HEMAXLauncher::UpdateOptionsDialog()
{
    OptionsDialog->Update();
}

#ifdef HEMAX_VERSION_2025
void
HEMAXLauncher::OnCUIRegisterMenus(void* Param, NotifyInfo* Info)
{
    if (!Param)
        return;

    HEMAXLauncher* Launcher = static_cast<HEMAXLauncher*>(Param);
    Launcher->InstallMenu(static_cast<MaxSDK::CUI::ICuiMenuManager*>(Info->callParam));
}
#endif

#ifndef HEMAX_VERSION_2025
void
HEMAXLauncher::OnCUIMenusLoaded(void* param, NotifyInfo* Info)
{
    auto Launcher = static_cast<HEMAXLauncher*>(param);

    if (Launcher && Launcher->WasHAPIDLLFound())
    {
	Launcher->InstallMenu(GetCOREInterface()->GetMenuManager());
    }
}

void
HEMAXLauncher::OnCUIMenusPostSaved(void* param, NotifyInfo* Info)
{
    auto Launcher = static_cast<HEMAXLauncher*>(param);

    if (Launcher && Launcher->WasHAPIDLLFound())
    {
	Launcher->InstallMenu(GetCOREInterface()->GetMenuManager());
    }
}

void
HEMAXLauncher::OnCUIMenusPreSaved(void* param, NotifyInfo* Info)
{
    auto Launcher = static_cast<HEMAXLauncher*>(param);

    if (Launcher && Launcher->WasHAPIDLLFound())
    {
	Launcher->RemoveMenu();
    }
}
#endif

#ifdef HEMAX_VERSION_2025
void
HEMAXLauncher::InstallMenu(MaxSDK::CUI::ICuiMenuManager* MenuManager)
{
    if (!MenuManager)
        return;

    // If the Houdini Engine menu already exists, there's nothing to be done.
    if (MenuManager->GetMenuById(HoudiniEngineMenuGuid))
        return;

    MaxSDK::CUI::ICuiMenu* MainMenu = MenuManager->GetMainMenuBar();

    MaxSDK::CUI::ICuiMenu* PluginMenu = MainMenu->CreateSubMenu(
        HoudiniEngineMenuGuid, GetString(IDS_CLASS_NAME));

    if (!PluginMenu)
        return;

    ActionTable* HoudiniEngineActionTable = GetHEMAXLauncherDesc()->GetActionTable(0);
    GetCOREInterface()->GetActionManager()->ActivateActionTable(this, HEMAX_Actions_Id);

    PluginMenu->CreateAction(HoudiniEngineOpenActionGuid, HEMAX_Actions_Id,
        OPEN_HEMAX_ACTION, OPEN_HEMAX_MENU_STRING);
    PluginMenu->CreateAction(HoudiniEngineHideActionGuid, HEMAX_Actions_Id,
        HIDE_HEMAX_ACTION, HIDE_HEMAX_MENU_STRING);
    PluginMenu->CreateSeparator(HoudiniEngineSeparator1Guid);
    PluginMenu->CreateAction(HoudiniEngineOptionsActionGuid, HEMAX_Actions_Id,
        OPTIONS_HEMAX_ACTION, OPTIONS_HEMAX_MENU_STRING);
    PluginMenu->CreateSeparator(HoudiniEngineSeparator2Guid);
    PluginMenu->CreateAction(HoudiniEngineVersionActionGuid, HEMAX_Actions_Id,
        VERSION_HEMAX_ACTION, VERSION_HEMAX_MENU_STRING);

}
#else
void
HEMAXLauncher::InstallMenu(IMenuManager* MenuManager)
{
    if (!MenuManager)
        return;

    if (!WasHAPIDLLFound())
        return;

    Interface* ip = GetCOREInterface();
    IMenu* Menu = MenuManager->FindMenu(GetString(IDS_CLASS_NAME));

    // If the  Houdini Engine menu already exists, there's nothing to be done.
    if (Menu)
        return;

    IMenuBarContext* MenuContext =
            (IMenuBarContext*)MenuManager->GetContext(kMainMenuBar);
    IMenu* MainMenu = MenuContext->GetMenu();

    MenuManager->RegisterMenuBarContext(kContextIdHEMAXMenu,
                                        GetString(IDS_CLASS_NAME));

    IMenu* HEMAXMenu = GetIMenu();
    HEMAXMenu->SetTitle(GetString(IDS_CLASS_NAME));
    MenuManager->RegisterMenu(HEMAXMenu, 0);
    IMenuBarContext* Context =
        (IMenuBarContext*)MenuManager->GetContext(kContextIdHEMAXMenu);
    Context->SetMenu(HEMAXMenu);

    IActionManager* ActionManager = ip->GetActionManager();
    if (ActionManager)
    {
        ActionManager->ActivateActionTable(this, HEMAX_Actions_Id);
    }

    ActionTable* HEMAXActionTable =
        GetHEMAXLauncherDesc()->GetActionTable(0);

    TSTR ATName = HEMAXActionTable->GetName();

    IMenuItem* ItemSub = GetIMenuItem();
    ActionItem* Action = HEMAXActionTable->GetAction(OPEN_HEMAX_ACTION);
    ItemSub->SetActionItem(Action);
    ItemSub->SetUseCustomTitle(true);
    ItemSub->SetTitle(OPEN_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(ItemSub);

    IMenuItem* HideSub = GetIMenuItem();
    ActionItem* HideAction = HEMAXActionTable->GetAction(
                                            HIDE_HEMAX_ACTION);
    HideSub->SetActionItem(HideAction);
    HideSub->SetUseCustomTitle(true);
    HideSub->SetTitle(HIDE_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(HideSub);

    IMenuItem* Separator1 = GetIMenuItem();
    Separator1->ActAsSeparator();
    HEMAXMenu->AddItem(Separator1);

    IMenuItem* OptionsSub = GetIMenuItem();
    ActionItem* OptionsAction = HEMAXActionTable->GetAction(
                                            OPTIONS_HEMAX_ACTION);
    OptionsSub->SetActionItem(OptionsAction);
    OptionsSub->SetUseCustomTitle(true);
    OptionsSub->SetTitle(OPTIONS_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(OptionsSub);

    IMenuItem* Separator2 = GetIMenuItem();
    Separator2->ActAsSeparator();
    HEMAXMenu->AddItem(Separator2);

    IMenuItem* VersionSub = GetIMenuItem();
    ActionItem* VersionAction = HEMAXActionTable->GetAction(
                                                VERSION_HEMAX_ACTION);
    VersionSub->SetActionItem(VersionAction);
    VersionSub->SetUseCustomTitle(true);
    VersionSub->SetTitle(VERSION_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(VersionSub);

    IMenuItem* MainItem = GetIMenuItem();
    MainItem->SetSubMenu(HEMAXMenu);

    MainMenu->AddItem(MainItem, -1);
    MenuManager->UpdateMenuBar();
}
#endif

void
HEMAXLauncher::RemoveMenu()
{
    if (WasHAPIDLLFound())
    {
#ifdef HEMAX_VERSION_2025
#else
	Interface* ip = GetCOREInterface();
	IMenuManager* MenuManager = ip->GetMenuManager();
	IMenu* Menu = MenuManager->FindMenu(GetString(IDS_CLASS_NAME));

	if (Menu)
	{
	    while (Menu->NumItems() > 0)
	    {
		Menu->RemoveItem(0);
	    }

	    IMenuBarContext* Context = (IMenuBarContext*)MenuManager->GetContext(kContextIdHEMAXMenu);
	    Context->SetMenu(nullptr);
	    MenuManager->UnRegisterMenu(Menu);

	    MenuManager->UpdateMenuBar();
	}
#endif
    }
}

BOOL
HEMAXLauncher::ExecuteAction(int ID)
{
    switch (ID)
    {
	case OPEN_HEMAX_ACTION:
	{
	    PluginUserInterface->show();
	    return true;
	}
	case HIDE_HEMAX_ACTION:
	{
	    PluginUserInterface->hide();
	    return true;
	}
        case OPTIONS_HEMAX_ACTION:
        {
            OptionsDialog->show();
            return true;
        }
	case VERSION_HEMAX_ACTION:
	{
	    VersionDialog->show();
            return true;
	}
    }

    return false;
}

int
HEMAXLauncherClassDesc::NumActionTables()
{
    return 1;
}

ActionTable*
HEMAXLauncherClassDesc::GetActionTable(int)
{
    if (HEMAX_ActionTable == nullptr)
    {
	HEMAX_ActionTable = new HEMAXActionTable();
	GetCOREInterface()->GetActionManager()->RegisterActionContext(HEMAX_Context_Id, GetString(IDS_HEMAX_AT_CONTEXT));
    }
    return HEMAX_ActionTable;
}
