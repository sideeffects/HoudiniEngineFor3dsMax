#pragma once

#include "resource.h"

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
#include <utilapi.h>
#include <actiontable.h>
#if !defined(HEMAX_VERSION_2025) && \
    !defined(HEMAX_VERSION_2026)
#include <imenuman.h>
#endif
#include <guplib.h>
#include <notify.h>
#pragma warning(pop)

#include <string>

#define HEMAXLauncher_CLASS_ID Class_ID(0x93a752c7, 0x774a3d7f)

#define HEMAX_HOUDINI_REGISTRY_INSTALL_PATH_NAME "InstallPath"

const TCHAR* const OPEN_HEMAX_MENU_STRING =                 _T("Open");
const TCHAR* const HIDE_HEMAX_MENU_STRING =                 _T("Hide");
const TCHAR* const CREATE_SESSION_HEMAX_MENU_STRING =       _T("Create Session");
const TCHAR* const CONNECT_SESSION_HEMAX_MENU_STRING =      _T("Connect Session");
const TCHAR* const STOP_SESSION_HEMAX_MENU_STRING =         _T("Stop Session");
const TCHAR* const RESTART_SESSION_HEMAX_MENU_STRING =      _T("Restart Session");
const TCHAR* const OPEN_SESSION_SYNC_HEMAX_MENU_STRING =    _T("Open Houdini SessionSync");
const TCHAR* const CLOSE_SESSION_SYNC_HEMAX_MENU_STRING =   _T("Close Houdini SessionSync");
const TCHAR* const OPTIONS_HEMAX_MENU_STRING =              _T("Options");
const TCHAR* const VERSION_HEMAX_MENU_STRING =              _T("Version Information");

const ActionTableId HEMAX_Actions_Id = 0x33fa7596;
const ActionContextId HEMAX_Context_Id = 0x626b4e5c;

extern TCHAR *GetString( int id );
extern HINSTANCE hInstance;

#if defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
namespace MaxSDK::CUI
{
class ICuiMenuManager;
}
#else
class IMenuManager;
#endif

class HEMAX_OptionsDialog;
class HEMAX_Plugin;
class HEMAX_UI;
class HEMAX_VersionDialog;

class HEMAXActionTable : public ActionTable
{
    public:
	HEMAXActionTable();
	~HEMAXActionTable();
};

class HEMAXLauncher : public GUP, ActionCallback
{
    public:
	HEMAXLauncher();
	virtual ~HEMAXLauncher();

	virtual DWORD     Start();
	virtual void      Stop();
	virtual DWORD_PTR Control(DWORD parameter);
	virtual void      DeleteThis();

#if defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
        void InstallMenu(MaxSDK::CUI::ICuiMenuManager* MenuManager);
#else
	void InstallMenu(IMenuManager* MenuManager);
#endif
	void RemoveMenu();

	int kContextIdHEMAXMenu;

	BOOL ExecuteAction(int id) override;

	static HEMAXLauncher* GetInstance() {
	    static HEMAXLauncher theHEMAXLauncher;
	    return &theHEMAXLauncher;
	}

	bool WasHAPIDLLFound();
	std::wstring GetLibHAPILDirectory();

        void UpdateOptionsDialog();

#if defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
        static void OnCUIRegisterMenus(void* Param, NotifyInfo* Info);
#else
        static void OnCUIMenusLoaded(void* param, NotifyInfo* info);
        static void OnCUIMenusPreSaved(void* param, NotifyInfo* info);
        static void OnCUIMenusPostSaved(void* param, NotifyInfo* info);
#endif

    private:

	HEMAX_Plugin* ThePlugin = nullptr;
	HEMAX_UI* PluginUserInterface = nullptr;
        HEMAX_OptionsDialog* OptionsDialog = nullptr;
	HEMAX_VersionDialog* VersionDialog = nullptr;

	HMODULE LoadLibHAPIL(const std::wstring& HFS);

	HMODULE FindHoudiniEngineLibs();
        void SetHoudiniDirectories(const std::wstring& HFS);

	bool FoundHAPIDLL;

        std::wstring HFSDirectory;
	std::wstring LibHAPILDirectory;
	std::wstring HAPIToolsDirectory;

	int HoudiniMajorVersion;
	int HoudiniMinorVersion;
	int HoudiniBuildVersion;
	int HoudiniPatchNumber;
};

class HEMAXLauncherClassDesc : public ClassDesc2
{
    public:
	virtual int IsPublic() { return TRUE; }
	virtual void* Create(BOOL) { return HEMAXLauncher::GetInstance(); }
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
        virtual const TCHAR* NonLocalizedClassName() override
            { return GetString(IDS_CLASS_NAME); }
#endif
	virtual const TCHAR* ClassName() override
            { return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() { return GUP_CLASS_ID; }
	virtual Class_ID ClassID() { return HEMAXLauncher_CLASS_ID; }
	virtual const TCHAR* Category() { return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() { return _T("HEMAXLauncher"); }
	virtual HINSTANCE HInstance() { return hInstance; }

	int NumActionTables() override;
	ActionTable* GetActionTable(int) override;

    private:
	ActionTable* HEMAX_ActionTable = nullptr;
};
