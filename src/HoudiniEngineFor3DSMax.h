#pragma once

#include "resource.h"
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
#include <utilapi.h>
#include "actiontable.h"
#include "imenuman.h"
#include "guplib.h"
#include "notify.h"

#include <string>

#define HEMAXLauncher_CLASS_ID Class_ID(0x93a752c7, 0x774a3d7f)

#define HEMAX_HOUDINI_REGISTRY_INSTALL_PATH_NAME "InstallPath"
#define HEMAX_HOUDINI_LIBHAPIL_SUBDIRECTORY      "bin"
#define HEMAX_HOUDINI_TOOLS_SUBDIRECTORY         "engine\\tools"

const TCHAR* const OPEN_HEMAX_MENU_STRING =    _T("Open");
const TCHAR* const HIDE_HEMAX_MENU_STRING =    _T("Hide");
const TCHAR* const OPTIONS_HEMAX_MENU_STRING = _T("Options");
const TCHAR* const VERSION_HEMAX_MENU_STRING = _T("Version Information");

const ActionTableId HEMAX_Actions_Id = 0x33fa7596;
const ActionContextId HEMAX_Context_Id = 0x626b4e5c;

extern TCHAR *GetString( int id );
extern HINSTANCE hInstance;

class HEMAX_Events;
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

	void InstallMenu();
	void RemoveMenu();

	int kContextIdHEMAXMenu;

	BOOL ExecuteAction(int id) override;

	static void OnCUIMenusLoaded(void* param, NotifyInfo* info);
	static void OnCUIMenusPreSaved(void* param, NotifyInfo* info);
	static void OnCUIMenusPostSaved(void* param, NotifyInfo* info);

	static HEMAXLauncher* GetInstance() {
	    static HEMAXLauncher theHEMAXLauncher;
	    return &theHEMAXLauncher;
	}

	bool WasHAPIDLLFound();
	static std::string GetLibHAPILDirectory();

        void UpdateOptionsDialog();

    private:

	HEMAX_Plugin* ThePlugin;
	HEMAX_UI* PluginUserInterface;
        HEMAX_OptionsDialog* OptionsDialog;
	HEMAX_VersionDialog* VersionDialog;
        HEMAX_Events* PluginEvents;

	HMODULE LoadLibHAPIL();

	HMODULE FindHoudiniEngineLibs();
	void SetHoudiniSubDirectories(std::wstring HoudiniDir);

	bool FoundHAPIDLL;
	static std::string LibHAPILDirectory;
	static std::string HAPIToolsDirectory;

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
#ifdef HEMAX_VERSION_2022
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
