#include "HEMAX_UI.h"

#include "../HEMAX_3dsmaxHda.h"
#include "../HEMAX_GeometryHda.h"
#include "../HEMAX_SessionManager.h"
#include "../HEMAX_Logger.h"
#include "../HEMAX_UserPrefs.h"

#include "moc_HEMAX_UI.cpp"

std::string HEMAX_CurrentAssetSelection = "";

HEMAX_UI::HEMAX_UI(QMainWindow* MainWindow, HEMAX_Plugin* Plugin)
    : QDockWidget()
    , ActivePlugin(Plugin)
{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setEnabled(true);

    this->setWindowTitle("Houdini Engine");

    ScrollArea = new QScrollArea;
    TabContainer = new QTabWidget;

    ShelfToolsWidget = new HEMAX_ShelfTab(ActivePlugin, false);
    HDAWidget = new HEMAX_HDAWidget(ActivePlugin);
    MHAWidget = new HEMAX_MaxHoudiniAssetWidget(ActivePlugin);
    OutputLogTab = new HEMAX_OutputLogWidget;

    TabContainer->addTab(HDAWidget, "Load Assets");
    TabContainer->addTab(MHAWidget, "Parameters");
    TabContainer->addTab(ShelfToolsWidget, "Shelf");
    TabContainer->addTab(OutputLogTab, "Output Log");

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
	this->resize(HEMAX_2017_WINDOW_START_WIDTH,
                     HEMAX_2017_WINDOW_START_HEIGHT);
    }

    QObject::connect(MHAWidget,
                     SIGNAL(Signal_UpdateParameterIntValues(HEMAX_Node*,
                                                            HEMAX_Parameter,
                                                            std::vector<int>,
                                                            bool)),
                     this,
                     SLOT(Slot_HandleUpdateParameterIntValues(HEMAX_Node*,
                                                              HEMAX_Parameter,
                                                              std::vector<int>,
                                                              bool)));

    QObject::connect(
            MHAWidget,
            SIGNAL(Signal_UpdateParameterFloatValues(HEMAX_Node*,
                                                     HEMAX_Parameter,
                                                     std::vector<float>,
                                                     bool)),
            this,
            SLOT(Slot_HandleUpdateParameterFloatValues(HEMAX_Node*,
                                                       HEMAX_Parameter,
                                                       std::vector<float>,
                                                       bool)));

    QObject::connect(
        MHAWidget,
        SIGNAL(Signal_UpdateParameterStringValues(HEMAX_Node*,
                                                  HEMAX_Parameter,
                                                  std::vector<std::string>)),
        this,
        SLOT(Slot_HandleUpdateParameterStringValues(HEMAX_Node*,
                                                    HEMAX_Parameter,
                                                    std::vector<std::string>)));

    QObject::connect(
        MHAWidget,
        SIGNAL(Signal_UpdateMultiParameterList(HEMAX_Node*,
                                               HEMAX_Parameter,
                                               HEMAX_MultiParameterChangeInfo)),
        this,
        SLOT(Slot_HandleUpdateMultiParameterList(HEMAX_Node*,
                                                 HEMAX_Parameter,
                                                 HEMAX_MultiParameterChangeInfo)));

    QObject::connect(
        MHAWidget,
        SIGNAL(Signal_InputSelection(HEMAX_Node*, HEMAX_Parameter, bool)),
        this,
        SLOT(HandleInputSelection(HEMAX_Node*, HEMAX_Parameter, bool)));

    QObject::connect(
        MHAWidget,
        SIGNAL(Signal_SubnetworkInputSelection(HEMAX_Node*, int, bool)),
        this,
        SLOT(HandleSubnetworkInputSelection(HEMAX_Node*, int, bool)));

    ShelfToolsWidget->SetShelf(ActivePlugin->GetToolShelf());

    bool AutoStartWindow;
    HEMAX_UserPrefs::Get().GetBoolSetting(HEMAX_SETTING_AUTO_START_WINDOW,
        AutoStartWindow);

    if (AutoStartWindow)
    {
        ShowHEMAXWindow();
    }
    else
    {
        UnshowHEMAXWindow();
    }

    HEMAX_SessionManager::GetSessionManager().
        RegisterOnSessionChangedCallback(
            std::bind(&HEMAX_UI::OnSessionChangedCallback, this));
}

HEMAX_UI::~HEMAX_UI()
{
    if (MHAWidget)
        delete MHAWidget;
    if (HDAWidget)
        delete HDAWidget;
    if (ShelfToolsWidget)
        delete ShelfToolsWidget;
    if (OutputLogTab)
        delete OutputLogTab;

    if (TabContainer)
        delete TabContainer;
    if (ScrollArea)
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
HEMAX_UI::SetSelectionLocked(bool Locked)
{
    MHAWidget->HandleLockSelectionButtonChanged(Locked);
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
HEMAX_UI::Update()
{
    ShelfToolsWidget->Update();
    std::vector<std::string> LoadedAssetList =
        ActivePlugin->GetPluginStore()->GetListOfLoadedAssets();
    HDAWidget->UpdateLoadedAssetList(&LoadedAssetList);
    MHAWidget->RefreshParameterUI(false);

    if (HEMAX_SessionManager::GetSessionManager().IsSessionValidAndInitialized())
    {
        ShelfToolsWidget->EnableShelf();
    }
    else
    {
        ShelfToolsWidget->DisableShelf();
    }
}


void
HEMAX_UI::OnSessionChangedCallback()
{
    Update();
}

void
HEMAX_UI::UpdateLoadedAssetLibrariesList()
{
    HEMAX_Store* Store = ActivePlugin->GetPluginStore();
    std::vector<std::string> Paths = Store->GetListOfLoadedAssets();
    HDAWidget->UpdateLoadedAssetList(&Paths);
}

void
HEMAX_UI::HandleInputSelection(HEMAX_Node* Node,
                               HEMAX_Parameter Parameter,
                               bool ClearSelection)
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
HEMAX_UI::HandleSubnetworkInputSelection(HEMAX_Node* Node,
                                         int Subnetwork,
                                         bool ClearSelection)
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
HEMAX_UI::Slot_HandleUpdateParameterIntValues(HEMAX_Node* Node,
                                              HEMAX_Parameter Parameter,
                                              std::vector<int> IntValues,
                                              bool DoNotRefreshUI)
{
    ActivePlugin->HandleParameterIntValuesUpdate(Node, Parameter, IntValues);

    if (Node->AutoRecookOnParameterUpdate && !DoNotRefreshUI)
    {
	MHAWidget->RefreshParameterUI();
	TabContainer->setCurrentWidget(MHAWidget);
    }
}

void
HEMAX_UI::Slot_HandleUpdateParameterFloatValues(HEMAX_Node* Node,
                                                HEMAX_Parameter Parameter,
                                                std::vector<float> FloatValues,
                                                bool DoNotRefreshUI)
{
    ActivePlugin->HandleParameterFloatValuesUpdate(Node,
                                                   Parameter,
                                                   FloatValues);

    if (Node->AutoRecookOnParameterUpdate && !DoNotRefreshUI)
    {
	MHAWidget->RefreshParameterUI();
	TabContainer->setCurrentWidget(MHAWidget);
    }
}

void
HEMAX_UI::Slot_HandleUpdateParameterStringValues(
                                        HEMAX_Node* Node,
                                        HEMAX_Parameter Parameter,
                                        std::vector<std::string> StringValues)
{
    ActivePlugin->HandleParameterStringValuesUpdate(Node,
                                                    Parameter,
                                                    StringValues);

    if (Node->AutoRecookOnParameterUpdate)
    {
	MHAWidget->RefreshParameterUI();
	TabContainer->setCurrentWidget(MHAWidget);
    }
}

void
HEMAX_UI::Slot_HandleUpdateMultiParameterList(
                                    HEMAX_Node* Node,
                                    HEMAX_Parameter Parameter,
                                    HEMAX_MultiParameterChangeInfo ChangeInfo)
{
    ActivePlugin->HandleMultiParameterUpdate(Node, Parameter, ChangeInfo);
    MHAWidget->RefreshParameterUI();
    TabContainer->setCurrentWidget(MHAWidget);
}

void
HEMAX_UI::HandleHdaPreDeleteEvent(HEMAX_3dsmaxHda* Hda)
{
    if (GetCurrentHdaSelection() == Hda)
    {
        ChangeHdaSelection(nullptr, true);
    }
}
