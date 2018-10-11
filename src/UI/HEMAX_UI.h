#pragma once

#include "../HEMAX_Plugin.h"
#include "HEMAX_SessionWidget.h"
#include "HEMAX_HDAWidget.h"
#include "HEMAX_MaxHoudiniAssetWidget.h"
#include "HEMAX_OptionsWidget.h"
#include "HEMAX_DebugWidget.h"
#include "HEMAX_ShelfTab.h"

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <Qt/QmaxMainWindow.h>
#include <Qt/QmaxDockWidget.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qtabwidget.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qscrollarea.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qmainwindow.h>
#endif

#define HEMAX_2017_WINDOW_START_HEIGHT 600
#define HEMAX_2017_WINDOW_START_WIDTH 600

#include <maxapi.h>

class HEMAX_Plugin;

class HEMAX_UI : public QDockWidget
{

    Q_OBJECT

public:
    HEMAX_UI(QMainWindow* MainWindow);
    ~HEMAX_UI();

    void ChangeHdaSelection(HEMAX_3dsmaxHda* Hda, bool ForceUnlock = false);
    HEMAX_3dsmaxHda* GetCurrentHdaSelection();

    void ShowHEMAXWindow();
    void UnshowHEMAXWindow();

    void TriggerOutOfProcessSession();

    void AttachPlugin(HEMAX_Plugin* Plugin);

    void UpdateLoadedAssetLibrariesList(std::vector<std::string>* LoadedAssetLibraries);
    void HandleParameterUndoRedoEvent();

    std::string GetCurrentAssetSelection();
    std::string GetAssetLoadPath();

    void Update();

private:

    QScrollArea* ScrollArea;
    QTabWidget* TabContainer;

    HEMAX_ShelfTab* ShelfToolsWidget;
    HEMAX_SessionWidget* SessionWidget;
    HEMAX_HDAWidget* HDAWidget;
    HEMAX_MaxHoudiniAssetWidget* MHAWidget;
    HEMAX_OptionsWidget* OptionsWidget;
    HEMAX_DebugWidget* DebugWidget;
    
    HEMAX_Plugin* ActivePlugin;

    void SetPluginOptions();

private slots:

    void LoadAssetTriggered();
    void CreateLoadedAssetTriggered();
    void CreateModifierHdasTriggered();
    void HandleSessionStopped();
    void HandleSessionStarted();

    void Slot_HandleRecookRequested(HEMAX_Node* Node);
    void Slot_ReloadAssetDefinition(HEMAX_Node* Node);
    void Slot_BakeHda(HEMAX_3dsmaxHda* MaxHda);
    void Slot_CloneHda(HEMAX_3dsmaxHda* MaxHda);
    void Slot_CopyHdaToNode(HEMAX_3dsmaxHda* MaxHda, INode* Node);
    void HandleInputSelection(HEMAX_Node* Node, HEMAX_Parameter Parameter, bool ClearSelection);
    void HandleSubnetworkInputSelection(HEMAX_Node*, int, bool);
    void Slot_HandleUpdateParameterIntValues(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<int> IntValues, bool DoNotRefreshUI);
    void Slot_HandleUpdateParameterFloatValues(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<float> FloatValues, bool DoNotRefreshUI);
    void Slot_HandleUpdateParameterStringValues(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<std::string> StringValues);
    void Slot_HandleUpdateMultiParameterList(HEMAX_Node* Node, HEMAX_Parameter Parameter, HEMAX_MultiParameterChangeInfo ChangeInfo);

    void Slot_Options_AutoSelectRoot(int Checked);
    void Slot_Options_AutoStartWindow(int Checked);
    void Slot_Options_AutoLoadHDADirectory(std::string Directory);
    void Slot_Options_HdaRepoDir_EditingFinished(std::string Directory);
    void Slot_DebugTempDirChanged(std::string Directory);

    void Slot_SessionConfigPathChanged(const char* PathChanged, std::string Path);

    void Slot_RemoveAsset(QString AssetPath);

    void Slot_ShelfUpdated();
    void Slot_ShelfTool_CreateObject(std::string AssetPath);
    void Slot_ShelfTool_CreateModifiers(std::string AssetPath);
};
