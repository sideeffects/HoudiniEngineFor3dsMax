#include "HoudiniEngineFor3DSMax.h"

#include "HEMAX_Events.h"
#include "HEMAX_Logger.h"
#include "HEMAX_MaxScriptInterface.h"
#include "HEMAX_Path.h"
#include "HEMAX_Plugin.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Utilities.h"

#include "UI/HEMAX_OptionsDialog.h"
#include "UI/HEMAX_UI.h"
#include "UI/HEMAX_VersionDialog.h"

#include <fstream>
#include <sstream>

#include <maxapi.h>
#include <maxscript/maxscript.h>

#define OPEN_HEMAX_ACTION    1
#define HIDE_HEMAX_ACTION    2
#define OPTIONS_HEMAX_ACTION 3
#define VERSION_HEMAX_ACTION 4

std::string HEMAXLauncher::LibHAPILDirectory = "";
std::string HEMAXLauncher::HAPIToolsDirectory = "";

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
    HoudiniMajorVersion = HEMAX_HAPI_VERSION_HOUDINI_MAJOR;
    HoudiniMinorVersion = HEMAX_HAPI_VERSION_HOUDINI_MINOR;
    HoudiniBuildVersion = HEMAX_HAPI_VERSION_HOUDINI_BUILD;
    HoudiniPatchNumber = HEMAX_HAPI_VERSION_HOUDINI_PATCH;

    HMODULE HAPIL = FindHoudiniEngineLibs();

    if (HAPIL)
    {
        // Set HOUDINI_HARS_LOCATION   
        std::string HARSProgram = std::string(LibHAPILDirectory) +
                                  "\\" +
                                  "HARS.exe"; 

        if (GetFileAttributesA(HARSProgram.c_str()) != INVALID_FILE_ATTRIBUTES)
        {
            int CallResult = SetEnvironmentVariableA("HOUDINI_HARS_LOCATION",
                                                     HARSProgram.c_str());

            if (!CallResult)
            {
                mprintf(L"Failed to set the HOUDINI_HARS_LOCATION environment "
                        "variable\n");
                mflush();
                mprintf(L"Windows Error Message: %d\n", GetLastError());
                mflush();
            }
        }

	Interface* TheInterface = GetCOREInterface();

	ThePlugin = new HEMAX_Plugin(TheInterface, HAPIL);

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022)
	PluginUserInterface = new HEMAX_UI(TheInterface->GetQmaxMainWindow(),
                                           ThePlugin);
#endif
#ifdef HEMAX_VERSION_2017
	PluginUserInterface = new HEMAX_UI(nullptr, ThePlugin);
#endif

        HEMAX_MaxScriptInterface::PluginInstance = ThePlugin;
        HEMAX_MaxScriptInterface::PluginUserInterface = PluginUserInterface;

        PluginEvents = new HEMAX_Events(PluginUserInterface);
        ThePlugin->SetEventHub(PluginEvents);
        HEMAX_SessionManager::GetSessionManager().SetEventHub(PluginEvents);

        ThePlugin->Init(HAPIToolsDirectory);

        OptionsDialog = new HEMAX_OptionsDialog(ThePlugin);
        OptionsDialog->hide();

	VersionDialog = new HEMAX_VersionDialog();
	VersionDialog->hide();
    }
}

HEMAXLauncher::~HEMAXLauncher() {}

HMODULE
HEMAXLauncher::LoadLibHAPIL()
{
    SetDllDirectoryA(LibHAPILDirectory.c_str());
    HMODULE libHAPIL = LoadLibraryA("libHAPIL.dll");
    SetDllDirectoryA("");

    if (libHAPIL)
    {
        // Since we found libHAPIL
	// Update %PATH% for this process
	std::stringstream PathStream;
	PathStream << LibHAPILDirectory;
	PathStream << ";";

	char SystemPath[4096];
	int Count = GetEnvironmentVariableA("PATH", SystemPath, 4096);

	if (Count <= 4096)
	{
	    PathStream << SystemPath;
	}
	else
	{
	    char* SystemPathLong = new char[Count];
	    GetEnvironmentVariableA("PATH", SystemPathLong, Count);
	    PathStream << SystemPathLong;
	    delete[] SystemPathLong;
	}

	PathStream << "\0";
	std::string PluginSystemPath = PathStream.str();

	int CallResult = SetEnvironmentVariableA("PATH",
                                                 PluginSystemPath.c_str());
        
        if (!CallResult)
        {
            mprintf(L"Failed to prepend the $HFS/bin directory to the %%PATH%% "
                    "variable.\n");
            mflush();
            mprintf(L"Windows Error Message: %d\n", GetLastError());
            mflush();
        }

	FoundHAPIDLL = true;

	return libHAPIL;
    }
    else
    {
	FoundHAPIDLL = false;
	return nullptr;
    }
}

HMODULE
HEMAXLauncher::FindHoudiniEngineLibs()
{
    // This is the process of tracking down HFS

    // Start with the REGISTRY

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
	    SetHoudiniSubDirectories(std::wstring(StringValue));
	    HMODULE libHAPILModule = LoadLibHAPIL();
	    if (libHAPILModule)
	    {
		return libHAPILModule;
	    }
	}
    }

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
	    SetHoudiniSubDirectories(std::wstring(StringValue));
	    HMODULE libHAPILModule = LoadLibHAPIL();
	    if (libHAPILModule)
	    {
		return libHAPILModule;
	    }
	}
    }

    // Worst case, try looking where Houdini usually is
    HAPIToolsDirectory = "C:\\Program Files\\Side Effects Software\\" +
                         HoudiniVersionString + "\\" +
                         HEMAX_HOUDINI_TOOLS_SUBDIRECTORY;

    LibHAPILDirectory = "C:\\Program Files\\Side Effects Software\\" +
                        HoudiniVersionString + "\\" +
                        HEMAX_HOUDINI_LIBHAPIL_SUBDIRECTORY;

    HEMAX_Path::HEMAX_PathPrefix_HFS_Resolved = 
        "C:\\Program Files\\Side Effects Software\\" + HoudiniVersionString;

    HMODULE libHAPILModule = LoadLibHAPIL();

    if (libHAPILModule)
    {
	return libHAPILModule;
    }

    // Ultra worst case, they can specify a houdini_path.txt

    std::ifstream HoudiniBinaryPathFile("houdini_path.txt");

    if (HoudiniBinaryPathFile.good())
    {
	char HoudiniPath[1024];
	HoudiniBinaryPathFile.getline(HoudiniPath, 1024);

	LibHAPILDirectory = HoudiniPath;
	LibHAPILDirectory += "\\";
	LibHAPILDirectory += HEMAX_HOUDINI_LIBHAPIL_SUBDIRECTORY;
	HEMAX_Path::HEMAX_PathPrefix_HFS_Resolved = HoudiniPath;

	HAPIToolsDirectory = HoudiniPath;
	HAPIToolsDirectory += "\\";
	HAPIToolsDirectory += HEMAX_HOUDINI_TOOLS_SUBDIRECTORY;

	libHAPILModule = LoadLibHAPIL();

	if (libHAPILModule)
	{
	    return libHAPILModule;
	}
    }

    HoudiniBinaryPathFile.close();

    // If we are here, it means that we couldn't find the .dlls

    HEMAX_Logger::Instance().ShowDialog("Houdini Engine Not Found",
	    "Houdini Engine could not be found. "
            "Please refer to the documentation for help.",
	    HEMAX_LOG_LEVEL_ERROR);

    return nullptr;
}

void
HEMAXLauncher::SetHoudiniSubDirectories(std::wstring HoudiniDir)
{
    std::wstring HAPIToolsDir = HoudiniDir + L"\\" + 
        _T(HEMAX_HOUDINI_TOOLS_SUBDIRECTORY);
    HAPIToolsDirectory = std::string(HAPIToolsDir.begin(), HAPIToolsDir.end());

    std::wstring LibHAPILPath = HoudiniDir + L"\\" +
        _T(HEMAX_HOUDINI_LIBHAPIL_SUBDIRECTORY);
    LibHAPILDirectory = std::string(LibHAPILPath.begin(), LibHAPILPath.end());

    std::wstring WStrValue = HoudiniDir;
    HEMAX_Path::HEMAX_PathPrefix_HFS_Resolved = std::string(WStrValue.begin(),
        WStrValue.end());
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
    RegisterNotification(&OnCUIMenusLoaded, this, NOTIFY_CUI_MENUS_POST_LOAD);
    RegisterNotification(&OnCUIMenusPreSaved, this, NOTIFY_CUI_MENUS_PRE_SAVE);
    RegisterNotification(&OnCUIMenusPostSaved, this, NOTIFY_CUI_MENUS_POST_SAVE);
    InstallMenu();
    return GUPRESULT_KEEP;
}

void
HEMAXLauncher::Stop()
{
    UnRegisterNotification(&OnCUIMenusLoaded, this, NOTIFY_CUI_MENUS_POST_LOAD);
    UnRegisterNotification(&OnCUIMenusPreSaved, this, NOTIFY_CUI_MENUS_PRE_SAVE);
    UnRegisterNotification(&OnCUIMenusPostSaved, this, NOTIFY_CUI_MENUS_POST_SAVE);
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

std::string
HEMAXLauncher::GetLibHAPILDirectory()
{
    return LibHAPILDirectory;
}

void
HEMAXLauncher::UpdateOptionsDialog()
{
    OptionsDialog->Update();
}

void
HEMAXLauncher::OnCUIMenusLoaded(void* param, NotifyInfo* Info)
{
    auto Launcher = static_cast<HEMAXLauncher*>(param);

    if (Launcher && Launcher->WasHAPIDLLFound())
    {
	Launcher->InstallMenu();
    }
}

void
HEMAXLauncher::OnCUIMenusPostSaved(void* param, NotifyInfo* Info)
{
    auto Launcher = static_cast<HEMAXLauncher*>(param);

    if (Launcher && Launcher->WasHAPIDLLFound())
    {
	Launcher->InstallMenu();
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

void
HEMAXLauncher::InstallMenu()
{
    if (WasHAPIDLLFound())
    {
	Interface* ip = GetCOREInterface();
	IMenuManager* MenuManager = ip->GetMenuManager();
	IMenu* Menu = MenuManager->FindMenu(GetString(IDS_CLASS_NAME));

	if (Menu == nullptr)
	{
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
    }
}

void
HEMAXLauncher::RemoveMenu()
{
    if (WasHAPIDLLFound())
    {
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
	GetCOREInterface()->GetActionManager()->RegisterActionContext(HEMAX_Context_Id, TSTR(GetString(IDS_HEMAX_AT_CONTEXT)));
    }
    return HEMAX_ActionTable;
}
