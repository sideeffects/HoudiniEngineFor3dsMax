#include "HEMAX_UI.h"

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qmessagebox.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qmessagebox.h>
#endif

#include "../HEMAX_HAPISession.h"
#include "../HEMAX_SessionManager.h"
#include "../HEMAX_Undo.h"
#include "../HEMAX_Logger.h"

#include "moc_HEMAX_UI.cpp"

HEMAX_UI::HEMAX_UI(QMainWindow* MainWindow)
    : QDockWidget()
{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setEnabled(true);

    this->setWindowTitle("Houdini Engine");

    ScrollArea = new QScrollArea;
    TabContainer = new QTabWidget;

    ShelfToolsWidget = new HEMAX_ShelfTab;
    SessionWidget = new HEMAX_SessionWidget;
    HDAWidget = new HEMAX_HDAWidget();
    MHAWidget = new HEMAX_MaxHoudiniAssetWidget;
    OptionsWidget = new HEMAX_OptionsWidget;
    DebugWidget = new HEMAX_DebugWidget;

    TabContainer->addTab(HDAWidget, "Load Assets");
    TabContainer->addTab(MHAWidget, "Parameters");
    TabContainer->addTab(ShelfToolsWidget, "Shelf");
    TabContainer->addTab(SessionWidget, "Session Configuration");
    TabContainer->addTab(OptionsWidget, "Options");
    TabContainer->addTab(DebugWidget, "Debug");

    ScrollArea->setWidget(TabContainer);
    ScrollArea->setWidgetResizable(true);

    this->setWidget(ScrollArea);
    this->setProperty("NoMaxAccelerators", true);

    if (MainWindow)
    {
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, (QDockWidget*)this);
    }
    else
    {
        this->setWindowFlags(Qt::WindowStaysOnTopHint);
        this->resize(HEMAX_2017_WINDOW_START_WIDTH, HEMAX_2017_WINDOW_START_HEIGHT);
    }

    QObject::connect(HDAWidget->AssetLoadButton, SIGNAL(clicked()), this, SLOT(LoadAssetTriggered()));
    QObject::connect(HDAWidget->LoadSelectedAssetButton, SIGNAL(clicked()), this, SLOT(CreateLoadedAssetTriggered()));
    QObject::connect(HDAWidget->CreateModifiersButton, SIGNAL(clicked()), this, SLOT(CreateModifierHdasTriggered()));

    QObject::connect(MHAWidget, SIGNAL(Signal_NodeRequiresRecook(HEMAX_Node*, bool)), this, SLOT(Slot_HandleRecookRequested(HEMAX_Node*)));
    QObject::connect(MHAWidget, SIGNAL(Signal_ReloadAssetDefinition(HEMAX_Node*)), this, SLOT(Slot_ReloadAssetDefinition(HEMAX_Node*)));
    QObject::connect(MHAWidget, SIGNAL(Signal_BakeHda(HEMAX_3dsmaxHda*)), this, SLOT(Slot_BakeHda(HEMAX_3dsmaxHda*)));
    QObject::connect(MHAWidget, SIGNAL(Signal_CloneHda(HEMAX_3dsmaxHda*)), this, SLOT(Slot_CloneHda(HEMAX_3dsmaxHda*)));
    QObject::connect(MHAWidget, SIGNAL(Signal_CopyHdaToNode(HEMAX_3dsmaxHda*, INode*)), this, SLOT(Slot_CopyHdaToNode(HEMAX_3dsmaxHda*, INode*)));
    QObject::connect(MHAWidget, SIGNAL(Signal_UpdateParameterIntValues(HEMAX_Node*, HEMAX_Parameter, std::vector<int>, bool)), this, SLOT(Slot_HandleUpdateParameterIntValues(HEMAX_Node*, HEMAX_Parameter, std::vector<int>, bool)));
    QObject::connect(MHAWidget, SIGNAL(Signal_UpdateParameterFloatValues(HEMAX_Node*, HEMAX_Parameter, std::vector<float>, bool)), this, SLOT(Slot_HandleUpdateParameterFloatValues(HEMAX_Node*, HEMAX_Parameter, std::vector<float>, bool)));
    QObject::connect(MHAWidget, SIGNAL(Signal_UpdateParameterStringValues(HEMAX_Node*, HEMAX_Parameter, std::vector<std::string>)), this, SLOT(Slot_HandleUpdateParameterStringValues(HEMAX_Node*, HEMAX_Parameter, std::vector<std::string>)));
    QObject::connect(MHAWidget, SIGNAL(Signal_UpdateMultiParameterList(HEMAX_Node*, HEMAX_Parameter, HEMAX_MultiParameterChangeInfo)), this, SLOT(Slot_HandleUpdateMultiParameterList(HEMAX_Node*, HEMAX_Parameter, HEMAX_MultiParameterChangeInfo)));

    QObject::connect(SessionWidget, SIGNAL(Signal_SessionStopped()), this, SLOT(HandleSessionStopped()));
    QObject::connect(SessionWidget, SIGNAL(Signal_SessionStarted()), this, SLOT(HandleSessionStarted()));

    QObject::connect(MHAWidget, SIGNAL(Signal_InputSelection(HEMAX_Node*, HEMAX_Parameter, bool)), this, SLOT(HandleInputSelection(HEMAX_Node*, HEMAX_Parameter, bool)));
    QObject::connect(MHAWidget, SIGNAL(Signal_SubnetworkInputSelection(HEMAX_Node*, int, bool)), this, SLOT(HandleSubnetworkInputSelection(HEMAX_Node*, int, bool)));

    QObject::connect(OptionsWidget, SIGNAL(Signal_SelectHDARootOption(int)), this, SLOT(Slot_Options_AutoSelectRoot(int)));
    QObject::connect(OptionsWidget, SIGNAL(Signal_SelectAutoStartWindowOption(int)), this, SLOT(Slot_Options_AutoStartWindow(int)));
    QObject::connect(OptionsWidget, SIGNAL(Signal_AutoLoadHDADir_EditingFinished(std::string)), this, SLOT(Slot_Options_AutoLoadHDADirectory(std::string)));
    QObject::connect(OptionsWidget, SIGNAL(Signal_HdaRepoDir_EditingFinished(std::string)), this, SLOT(Slot_Options_HdaRepoDir_EditingFinished(std::string)));

    QObject::connect(DebugWidget, SIGNAL(SignalTempDirChanged(std::string)), this, SLOT(Slot_DebugTempDirChanged(std::string)));

    QObject::connect(SessionWidget, SIGNAL(Signal_SessionConfigPathChanged(const char*, std::string)), this, SLOT(Slot_SessionConfigPathChanged(const char*, std::string)));
    QObject::connect(HDAWidget, SIGNAL(SignalRemoveAssetClicked(QString)), this, SLOT(Slot_RemoveAsset(QString)));

    QObject::connect(ShelfToolsWidget, SIGNAL(Signal_ShelfUpdated()), this, SLOT(Slot_ShelfUpdated()));
    QObject::connect(ShelfToolsWidget, SIGNAL(Signal_ShelfTool_CreateObject(std::string)), this, SLOT(Slot_ShelfTool_CreateObject(std::string)));
    QObject::connect(ShelfToolsWidget, SIGNAL(Signal_ShelfTool_CreateModifiers(std::string)), this, SLOT(Slot_ShelfTool_CreateModifiers(std::string)));
}

HEMAX_UI::~HEMAX_UI()
{
    delete DebugWidget;
    delete OptionsWidget;
    delete MHAWidget;
    delete HDAWidget;
    delete SessionWidget;
    delete ShelfToolsWidget;

    delete TabContainer;
    delete ScrollArea;
}

void
HEMAX_UI::ChangeHdaSelection(HEMAX_3dsmaxHda* Hda, bool ForceUnlock)
{
    MHAWidget->SetSelection(Hda, ForceUnlock);

    if (Hda)
    {
        TabContainer->setCurrentWidget(MHAWidget);
    }
}

HEMAX_3dsmaxHda*
HEMAX_UI::GetCurrentHdaSelection()
{
    return MHAWidget->GetCurrentHdaSelection();
}

void
HEMAX_UI::ShowHEMAXWindow()
{
    this->show();
}

void
HEMAX_UI::UnshowHEMAXWindow()
{
    this->hide();
}

void
HEMAX_UI::LoadAssetTriggered()
{
    std::string LoadPath = HDAWidget->GetCurrentAssetLoadPath();
    ActivePlugin->LoadNewAsset(LoadPath);
}

std::string
HEMAX_UI::GetCurrentAssetSelection()
{
    return HDAWidget->GetSelectedAssetPath();
}

std::string
HEMAX_UI::GetAssetLoadPath()
{
    return HDAWidget->GetCurrentAssetLoadPath();
}

void
HEMAX_UI::Update()
{
    ShelfToolsWidget->Update();
    HDAWidget->UpdateLoadedAssetList(&ActivePlugin->GetPluginStore()->GetListOfLoadedAssets());
}

void
HEMAX_UI::CreateLoadedAssetTriggered()
{
    std::string AssetPath = HDAWidget->GetSelectedAssetPath();
    ActivePlugin->CreateGeometryHDA(AssetPath);
}

void
HEMAX_UI::CreateModifierHdasTriggered()
{
    std::string AssetPath = HDAWidget->GetSelectedAssetPath();
    ActivePlugin->CreateModifierHDAs(AssetPath);
}

void
HEMAX_UI::TriggerOutOfProcessSession()
{
    SessionWidget->SessionsOutOfProcessRadioButton->click();
    SessionWidget->SessionsStartButton->click();
}

void
HEMAX_UI::AttachPlugin(HEMAX_Plugin* Plugin)
{
    ActivePlugin = Plugin;
    ShelfToolsWidget->SetShelf(ActivePlugin->GetToolShelf());
    DebugWidget->SetUserPrefs(ActivePlugin->GetUserPrefs());
    SetPluginOptions();
}

void
HEMAX_UI::SetPluginOptions()
{
    OptionsWidget->SetAutoSelectOption(ActivePlugin->GetAutoSelectRootMHANode());
    OptionsWidget->SetAutoStartWindowOption(ActivePlugin->GetAutoStartWindowOption());
    OptionsWidget->SetAutoLoadHDADirectory(ActivePlugin->GetAutoLoadHDADirectory());
    OptionsWidget->SetHdaRepoDirectory(ActivePlugin->GetHdaRepoDirectory());

    std::string TempDirectory;
    ActivePlugin->GetUserPrefs()->GetStringSetting(HEMAX_SETTING_DEBUG_TEMP_DIR, TempDirectory);
    DebugWidget->SetTempDirectory(TempDirectory);

    std::string SessionPath;

    ActivePlugin->GetUserPrefs()->GetStringSetting(HEMAX_SETTING_SESSION_ENV_FILES, SessionPath);
    SessionWidget->PushHoudiniEnvFilePath(SessionPath);

    ActivePlugin->GetUserPrefs()->GetStringSetting(HEMAX_SETTING_SESSION_OTL_SEARCH, SessionPath);
    SessionWidget->PushOtlSearchPath(SessionPath);

    ActivePlugin->GetUserPrefs()->GetStringSetting(HEMAX_SETTING_SESSION_DSO_SEARCH, SessionPath);
    SessionWidget->PushDsoSearchPath(SessionPath);

    ActivePlugin->GetUserPrefs()->GetStringSetting(HEMAX_SETTING_SESSION_IMAGE_DSO_SEARCH, SessionPath);
    SessionWidget->PushImageDsoSearchPath(SessionPath);

    ActivePlugin->GetUserPrefs()->GetStringSetting(HEMAX_SETTING_SESSION_AUDIO_DSO_SEARCH, SessionPath);
    SessionWidget->PushAudioDsoSearchPath(SessionPath);
}

void
HEMAX_UI::UpdateLoadedAssetLibrariesList(std::vector<std::string>* LoadedAssetLibraries)
{
    HDAWidget->UpdateLoadedAssetList(LoadedAssetLibraries);
}

void
HEMAX_UI::HandleSessionStopped()
{
    ActivePlugin->CurrentSessionStopped();
}

void
HEMAX_UI::HandleSessionStarted()
{
    ActivePlugin->NewSessionStarted();
}

void
HEMAX_UI::Slot_HandleRecookRequested(HEMAX_Node* Node)
{
    ActivePlugin->HandleRecookRequest(Node);
}

void
HEMAX_UI::Slot_ReloadAssetDefinition(HEMAX_Node *Node)
{
    QMessageBox ConfirmationDialog;
    ConfirmationDialog.setWindowFlags(Qt::WindowStaysOnTopHint);
    ConfirmationDialog.setWindowTitle("Reload Asset Definition");
    ConfirmationDialog.setText("This will delete and recreate all HDAs in your scene that are using this asset definition.");
    ConfirmationDialog.setInformativeText("Are you sure that you want to continue?");
    ConfirmationDialog.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int Result = ConfirmationDialog.exec();

    if (Result == QMessageBox::Ok)
    {
        if (MHAWidget->IsSelectionLocked())
        {
            MHAWidget->HandleLockSelectionButtonChanged(false);
        }
        ActivePlugin->ReloadAssetDefinition(Node);
    }
}

void
HEMAX_UI::Slot_BakeHda(HEMAX_3dsmaxHda* MaxHda)
{
    if (MaxHda)
    {
        BakeHda((*MaxHda));
    }
}

void
HEMAX_UI::Slot_CloneHda(HEMAX_3dsmaxHda* MaxHda)
{
    if (MaxHda)
    {
        ActivePlugin->CloneHda(MaxHda);
    }
}

void
HEMAX_UI::Slot_CopyHdaToNode(HEMAX_3dsmaxHda* MaxHda, INode* Node)
{
    if (MaxHda && Node)
    {
        ActivePlugin->CopyHdaToNode(MaxHda, Node);
    }
}

void
HEMAX_UI::HandleInputSelection(HEMAX_Node* Node, HEMAX_Parameter Parameter, bool ClearSelection)
{
    if (ClearSelection)
    {
        ActivePlugin->HandleParameterInputCleared(Node, Parameter);
    }
    else
    {
        ActivePlugin->HandleParameterInputSelection(Node, Parameter);
    }

    MHAWidget->RefreshParameterUI();
}

void
HEMAX_UI::HandleSubnetworkInputSelection(HEMAX_Node* Node, int Subnetwork, bool ClearSelection)
{
    if (ClearSelection)
    {
        ActivePlugin->HandleSubnetworkInputCleared(Node, Subnetwork);
    }
    else
    {
        ActivePlugin->HandleSubnetworkInputSelection(Node, Subnetwork);
    }

    MHAWidget->RefreshParameterUI();
}

void
HEMAX_UI::Slot_HandleUpdateParameterIntValues(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<int> IntValues, bool DoNotRefreshUI)
{
    ActivePlugin->HandleParameterIntValuesUpdate(Node, Parameter, IntValues);

    if (Node->AutoRecookOnParameterUpdate && !DoNotRefreshUI)
    {
        MHAWidget->RefreshParameterUI();
        TabContainer->setCurrentWidget(MHAWidget);
    }
}

void
HEMAX_UI::Slot_HandleUpdateParameterFloatValues(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<float> FloatValues, bool DoNotRefreshUI)
{
    ActivePlugin->HandleParameterFloatValuesUpdate(Node, Parameter, FloatValues);

    if (Node->AutoRecookOnParameterUpdate && !DoNotRefreshUI)
    {
        MHAWidget->RefreshParameterUI();
        TabContainer->setCurrentWidget(MHAWidget);
    }
}

void
HEMAX_UI::Slot_HandleUpdateParameterStringValues(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<std::string> StringValues)
{
    ActivePlugin->HandleParameterStringValuesUpdate(Node, Parameter, StringValues);

    if (Node->AutoRecookOnParameterUpdate)
    {
        MHAWidget->RefreshParameterUI();
        TabContainer->setCurrentWidget(MHAWidget);
    }
}

void
HEMAX_UI::Slot_HandleUpdateMultiParameterList(HEMAX_Node* Node, HEMAX_Parameter Parameter, HEMAX_MultiParameterChangeInfo ChangeInfo)
{
    ActivePlugin->HandleMultiParameterUpdate(Node, Parameter, ChangeInfo);
    MHAWidget->RefreshParameterUI();
    TabContainer->setCurrentWidget(MHAWidget);
}

void
HEMAX_UI::Slot_Options_AutoSelectRoot(int Checked)
{
    ActivePlugin->SetAutoSelectRootMHANode(Checked);
}

void
HEMAX_UI::Slot_Options_AutoStartWindow(int Checked)
{
    ActivePlugin->SetAutoStartWindowOption(Checked);
}

void
HEMAX_UI::Slot_Options_AutoLoadHDADirectory(std::string Directory)
{
    ActivePlugin->SetAutoLoadHDADirectory(Directory);
}

void
HEMAX_UI::Slot_Options_HdaRepoDir_EditingFinished(std::string Directory)
{
    ActivePlugin->SetHdaRepoDirectory(Directory);
}

void
HEMAX_UI::Slot_DebugTempDirChanged(std::string Directory)
{
    ActivePlugin->GetUserPrefs()->SetStringSetting(HEMAX_SETTING_DEBUG_TEMP_DIR, Directory);
}

void
HEMAX_UI::Slot_SessionConfigPathChanged(const char* PathChanged, std::string Path)
{
    ActivePlugin->GetUserPrefs()->SetStringSetting(PathChanged, Path);
}

void
HEMAX_UI::HandleParameterUndoRedoEvent()
{
    if (MHAWidget)
    {
        MHAWidget->RefreshParameterUI(false);
    }
}

void
HEMAX_UI::Slot_RemoveAsset(QString AssetPath)
{
    if (!ActivePlugin->RemoveAsset(AssetPath.toStdString()))
    {
        HEMAX_Logger::Instance().ShowDialog("Cannot remove asset", "Could not remove asset because it is in use by HDAs in the scene.", HEMAX_LOG_LEVEL_INFO);
    }
}

void
HEMAX_UI::Slot_ShelfUpdated()
{
    UpdateLoadedAssetLibrariesList(&ActivePlugin->GetPluginStore()->GetListOfLoadedAssets());
}

void
HEMAX_UI::Slot_ShelfTool_CreateObject(std::string AssetPath)
{
    ActivePlugin->CreateGeometryHDA(AssetPath);
}

void
HEMAX_UI::Slot_ShelfTool_CreateModifiers(std::string AssetPath)
{
    ActivePlugin->CreateModifierHDAs(AssetPath);
}
