#include "HoudiniEngineFor3DSMax.h"

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
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <maxapi.h>
#include <maxscript/maxscript.h>
#pragma warning(pop)

#if defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <CUI/ICuiMenuManager.h>
#include <CUI/ICuiMenu.h>
#pragma warning(pop)
#endif

#if defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
MaxSDK::MaxGuid HoudiniEngineMenuGuid("47dec1b8-78b6-44e3-ad16-f690946856d2");
MaxSDK::MaxGuid HoudiniEngineOpenActionGuid("01a24d08-9f84-48ed-8b58-476d88f6b470");
MaxSDK::MaxGuid HoudiniEngineHideActionGuid("f1e8ae17-aca1-4fc6-84fe-82ea0a964582");
MaxSDK::MaxGuid HoudiniEngineCreateSessionActionGuid("224bced3-74e9-4947-9563-eb8cf16c97b2");
MaxSDK::MaxGuid HoudiniEngineConnectSessionActionGuid("78594593-ea24-46b0-b5ad-d94586b26775");
MaxSDK::MaxGuid HoudiniEngineStopSessionActionGuid("3703d5dc-8abb-4e9b-ad2c-b7174a2d1d64");
MaxSDK::MaxGuid HoudiniEngineRestartSessionActionGuid("f45c52b5-27ed-4c4c-bd97-52afbe1d303c");
MaxSDK::MaxGuid HoudiniEngineOpenSessionSyncActionGuid("1d9a595c-807d-4736-8feb-3f7253ba96d8");
MaxSDK::MaxGuid HoudiniEngineCloseSessionSyncActionGuid("4cf22fd8-b04e-43f4-b66b-d638dc97412b");

MaxSDK::MaxGuid HoudiniEngineOptionsActionGuid("204aca3d-98e7-4505-a43a-0b64c2ecb760");
MaxSDK::MaxGuid HoudiniEngineVersionActionGuid("5076703e-a2d5-4103-9c4b-b6e008e0a396");

MaxSDK::MaxGuid HoudiniEngineSeparator1Guid("029dc07e-f811-4989-9692-7408e0ef6353");
MaxSDK::MaxGuid HoudiniEngineSeparator2Guid("9f32ff4c-a8fe-435c-a36e-78eaafcba655");
MaxSDK::MaxGuid HoudiniEngineSeparator3Guid("fece40fa-60d3-4edc-949e-3b7c6f3f0db3");
#endif

#define OPEN_HEMAX_ACTION               1
#define HIDE_HEMAX_ACTION               2
#define CREATE_SESSION_HEMAX_ACTION     3
#define CONNECT_SESSION_HEMAX_ACTION    4
#define STOP_SESSION_HEMAX_ACTION       5
#define RESTART_SESSION_HEMAX_ACTION    6
#define OPEN_SESSION_SYNC_HEMAX_ACTION  7
#define CLOSE_SESSION_SYNC_HEMAX_ACTION 8
#define OPTIONS_HEMAX_ACTION            9
#define VERSION_HEMAX_ACTION            10

const wchar_t* const LIB_HAPIL_NAME = L"libHAPIL.dll";
const wchar_t* const LIB_HAPIL_SUBDIRECTORY = L"bin";
const wchar_t* const HOUDINI_TOOLS_SUBDIRECTORY = L"engine\\tools";
const wchar_t* const HOUDINI_HARS_LOCATION_ENV_VAR = L"HOUDINI_HARS_LOCATION";
const wchar_t* const HOUDINI_HFS_ENV_VAR = L"HFS";

MSTR theHoudiniEngineActionTableName = GetString(IDS_CATEGORY);

static ActionDescription MenuActions[] = {
    OPEN_HEMAX_ACTION,
    IDS_HEMAX_MENU_ACTION,
    IDS_HEMAX_MENU_ACTION_DESC,
    IDS_CATEGORY,

    HIDE_HEMAX_ACTION,
    IDS_HEMAX_MENU_HIDE,
    IDS_HEMAX_MENU_HIDE_DESC,
    IDS_CATEGORY,

    CREATE_SESSION_HEMAX_ACTION,
    IDS_HEMAX_MENU_CREATE_SESSION,
    IDS_HEMAX_MENU_CREATE_SESSION_DESC,
    IDS_CATEGORY,

    CONNECT_SESSION_HEMAX_ACTION,
    IDS_HEMAX_MENU_CONNECT_SESSION,
    IDS_HEMAX_MENU_CONNECT_SESSION_DESC,
    IDS_CATEGORY,

    STOP_SESSION_HEMAX_ACTION,
    IDS_HEMAX_MENU_STOP_SESSION,
    IDS_HEMAX_MENU_STOP_SESSION_DESC,
    IDS_CATEGORY,

    RESTART_SESSION_HEMAX_ACTION,
    IDS_HEMAX_MENU_RESTART_SESSION,
    IDS_HEMAX_MENU_RESTART_SESSION_DESC,
    IDS_CATEGORY,

    OPEN_SESSION_SYNC_HEMAX_ACTION,
    IDS_HEMAX_MENU_OPEN_SESSION_SYNC,
    IDS_HEMAX_MENU_OPEN_SESSION_SYNC_DESC,
    IDS_CATEGORY,

    CLOSE_SESSION_SYNC_HEMAX_ACTION,
    IDS_HEMAX_MENU_CLOSE_SESSION_SYNC,
    IDS_HEMAX_MENU_CLOSE_SESSION_SYNC_DESC,
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
                  theHoudiniEngineActionTableName,
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

HEMAXLauncher::~HEMAXLauncher()
{
    if (OptionsDialog)
        delete OptionsDialog;

    if (VersionDialog)
        delete VersionDialog;
}

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

    HEMAX_SessionManager::GetSessionManager().SetHFSPath(HFSDirectory);
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

        ThePlugin->Init(std::string(HAPIToolsDirectory.begin(),
            HAPIToolsDirectory.end()));

        OptionsDialog = new HEMAX_OptionsDialog(ThePlugin);
        OptionsDialog->hide();

	VersionDialog = new HEMAX_VersionDialog();
	VersionDialog->hide();
    }

#if !defined(HEMAX_VERSION_2025) && \
    !defined(HEMAX_VERSION_2026)
    RegisterNotification(&HEMAXLauncher::OnCUIMenusLoaded, this, NOTIFY_CUI_MENUS_POST_LOAD);
    RegisterNotification(&HEMAXLauncher::OnCUIMenusPreSaved, this, NOTIFY_CUI_MENUS_PRE_SAVE);
    RegisterNotification(&HEMAXLauncher::OnCUIMenusPostSaved, this, NOTIFY_CUI_MENUS_POST_SAVE);
    InstallMenu(GetCOREInterface()->GetMenuManager());
#endif

    return GUPRESULT_KEEP;
}

void
HEMAXLauncher::Stop()
{
#if !defined(HEMAX_VERSION_2025) && \
    !defined(HEMAX_VERSION_2026)
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

#if defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
void
HEMAXLauncher::OnCUIRegisterMenus(void* Param, NotifyInfo* Info)
{
    if (!Param)
        return;

    HEMAXLauncher* Launcher = static_cast<HEMAXLauncher*>(Param);
    Launcher->InstallMenu(static_cast<MaxSDK::CUI::ICuiMenuManager*>(Info->callParam));
}
#endif

#if !defined(HEMAX_VERSION_2025) && \
    !defined(HEMAX_VERSION_2026)
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

#if defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
void
HEMAXLauncher::InstallMenu(MaxSDK::CUI::ICuiMenuManager* MenuManager)
{
    for (int i = 0; i < 100; ++i)
    {
        MaxSDK::MaxGuid tempguid = MaxSDK::MaxGuid::CreateMaxGuid();
        MSTR tempguid_str = tempguid.ToString();
        DebugPrint(tempguid_str);
    }

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

    GetCOREInterface()->GetActionManager()->ActivateActionTable(this, HEMAX_Actions_Id);

    PluginMenu->CreateAction(HoudiniEngineOpenActionGuid, HEMAX_Actions_Id,
        OPEN_HEMAX_ACTION, OPEN_HEMAX_MENU_STRING);
    PluginMenu->CreateAction(HoudiniEngineHideActionGuid, HEMAX_Actions_Id,
        HIDE_HEMAX_ACTION, HIDE_HEMAX_MENU_STRING);
    PluginMenu->CreateSeparator(HoudiniEngineSeparator1Guid);
    PluginMenu->CreateAction(HoudiniEngineCreateSessionActionGuid,
        HEMAX_Actions_Id, CREATE_SESSION_HEMAX_ACTION,
        CREATE_SESSION_HEMAX_MENU_STRING);
    PluginMenu->CreateAction(HoudiniEngineConnectSessionActionGuid,
        HEMAX_Actions_Id, CONNECT_SESSION_HEMAX_ACTION,
        CONNECT_SESSION_HEMAX_MENU_STRING);
    PluginMenu->CreateAction(HoudiniEngineStopSessionActionGuid,
        HEMAX_Actions_Id, STOP_SESSION_HEMAX_ACTION,
        STOP_SESSION_HEMAX_MENU_STRING);
    PluginMenu->CreateAction(HoudiniEngineRestartSessionActionGuid,
        HEMAX_Actions_Id, RESTART_SESSION_HEMAX_ACTION,
        RESTART_SESSION_HEMAX_MENU_STRING);
    PluginMenu->CreateAction(HoudiniEngineOpenSessionSyncActionGuid,
        HEMAX_Actions_Id, OPEN_SESSION_SYNC_HEMAX_ACTION,
        OPEN_SESSION_SYNC_HEMAX_MENU_STRING);
    PluginMenu->CreateAction(HoudiniEngineCloseSessionSyncActionGuid,
        HEMAX_Actions_Id, CLOSE_SESSION_SYNC_HEMAX_ACTION,
        CLOSE_SESSION_SYNC_HEMAX_MENU_STRING);
    PluginMenu->CreateSeparator(HoudiniEngineSeparator2Guid);
    PluginMenu->CreateAction(HoudiniEngineOptionsActionGuid, HEMAX_Actions_Id,
        OPTIONS_HEMAX_ACTION, OPTIONS_HEMAX_MENU_STRING);
    PluginMenu->CreateSeparator(HoudiniEngineSeparator3Guid);
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

    IMenuItem* CreateSessionSub = GetIMenuItem();
    ActionItem* CreateSessionAction =
        HEMAXActionTable->GetAction(CREATE_SESSION_HEMAX_ACTION);
    CreateSessionSub->SetActionItem(CreateSessionAction);
    CreateSessionSub->SetUseCustomTitle(true);
    CreateSessionSub->SetTitle(CREATE_SESSION_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(CreateSessionSub);

    IMenuItem* ConnectSessionSub = GetIMenuItem();
    ActionItem* ConnectSessionAction =
        HEMAXActionTable->GetAction(CONNECT_SESSION_HEMAX_ACTION);
    ConnectSessionSub->SetActionItem(ConnectSessionAction);
    ConnectSessionSub->SetUseCustomTitle(true);
    ConnectSessionSub->SetTitle(CONNECT_SESSION_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(ConnectSessionSub);

    IMenuItem* StopSessionSub = GetIMenuItem();
    ActionItem* StopSessionAction =
        HEMAXActionTable->GetAction(STOP_SESSION_HEMAX_ACTION);
    StopSessionSub->SetActionItem(StopSessionAction);
    StopSessionSub->SetUseCustomTitle(true);
    StopSessionSub->SetTitle(STOP_SESSION_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(StopSessionSub);

    IMenuItem* RestartSessionSub = GetIMenuItem();
    ActionItem* RestartSessionAction =
        HEMAXActionTable->GetAction(RESTART_SESSION_HEMAX_ACTION);
    RestartSessionSub->SetActionItem(RestartSessionAction);
    RestartSessionSub->SetUseCustomTitle(true);
    RestartSessionSub->SetTitle(RESTART_SESSION_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(RestartSessionSub);

    IMenuItem* OpenSessionSyncSub = GetIMenuItem();
    ActionItem* OpenSessionSyncAction =
        HEMAXActionTable->GetAction(OPEN_SESSION_SYNC_HEMAX_ACTION);
    OpenSessionSyncSub->SetActionItem(OpenSessionSyncAction);
    OpenSessionSyncSub->SetUseCustomTitle(true);
    OpenSessionSyncSub->SetTitle(OPEN_SESSION_SYNC_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(OpenSessionSyncSub);

    IMenuItem* CloseSessionSyncSub = GetIMenuItem();
    ActionItem* CloseSessionSyncAction =
        HEMAXActionTable->GetAction(CLOSE_SESSION_SYNC_HEMAX_ACTION);
    CloseSessionSyncSub->SetActionItem(CloseSessionSyncAction);
    CloseSessionSyncSub->SetUseCustomTitle(true);
    CloseSessionSyncSub->SetTitle(CLOSE_SESSION_SYNC_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(CloseSessionSyncSub);

    IMenuItem* Separator2 = GetIMenuItem();
    Separator2->ActAsSeparator();
    HEMAXMenu->AddItem(Separator2);

    IMenuItem* OptionsSub = GetIMenuItem();
    ActionItem* OptionsAction = HEMAXActionTable->GetAction(
                                            OPTIONS_HEMAX_ACTION);
    OptionsSub->SetActionItem(OptionsAction);
    OptionsSub->SetUseCustomTitle(true);
    OptionsSub->SetTitle(OPTIONS_HEMAX_MENU_STRING);
    HEMAXMenu->AddItem(OptionsSub);

    IMenuItem* Separator3 = GetIMenuItem();
    Separator3->ActAsSeparator();
    HEMAXMenu->AddItem(Separator3);

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
#if defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
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
        case CREATE_SESSION_HEMAX_ACTION:
        {
            HEMAX_SessionManager::GetSessionManager().CreateSession();
            return true;
        }
        case CONNECT_SESSION_HEMAX_ACTION:
        {
            HEMAX_SessionManager::GetSessionManager().ConnectSession();
            return true;
        }
        case STOP_SESSION_HEMAX_ACTION:
        {
            HEMAX_SessionManager::GetSessionManager().StopSession();
            return true;
        }
        case RESTART_SESSION_HEMAX_ACTION:
        {
            HEMAX_SessionManager::GetSessionManager().RestartSession();
            return true;
        }
        case OPEN_SESSION_SYNC_HEMAX_ACTION:
        {
            HEMAX_SessionManager::GetSessionManager().OpenSessionSync();
            return true;
        }
        case CLOSE_SESSION_SYNC_HEMAX_ACTION:
        {
            HEMAX_SessionManager::GetSessionManager().CloseSessionSync();
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
