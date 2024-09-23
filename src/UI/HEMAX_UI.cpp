#include "HEMAX_UI.h"

#include "../HEMAX_3dsmaxHda.h"
#include "../HEMAX_GeometryHda.h"
#include "../HEMAX_SessionManager.h"
#include "../HEMAX_Logger.h"
#include "../HEMAX_UserPrefs.h"

#include "HEMAX_HDAWidget.h"

#include "moc_HEMAX_UI.cpp"

std::string HEMAX_CurrentAssetSelection = "";

HEMAX_UI::HEMAX_UI(QMainWindow* MainWindow, HEMAX_Plugin* Plugin)
    : HEMAX_EventHandler(HEMAX_SessionManager::GetSessionManager().GetEvents())
    , QDockWidget()
    , ActivePlugin(Plugin)
{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setEnabled(true);

    this->setWindowTitle("Houdini Engine");

    ScrollArea = new QScrollArea;
    TabContainer = new QTabWidget;

    ShelfToolsWidget = new HEMAX_ShelfTab(ActivePlugin, false);
    AssetWidget = new HEMAX_AssetWidget(ActivePlugin);
    HdaWidget = new HEMAX_HDAWidget(ActivePlugin);
    OutputLogTab = new HEMAX_OutputLogWidget;

    TabContainer->addTab(AssetWidget, "Load Assets");
    TabContainer->addTab(HdaWidget, "Houdini Digital Asset");
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

    ShelfToolsWidget->SetShelf(
        &HEMAX_SessionManager::GetSessionManager().GetStore().GetShelf());

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

    RegisterCallback(HEMAX_EventType::SessionChanged,
                     [this](HEMAX_EventData* Data) {
        this->Update();
    });
    RegisterCallback(HEMAX_EventType::ShelfUpdated,
                     [this](HEMAX_EventData* Data) {
        this->Update();
    });
    RegisterCallback(HEMAX_EventType::AssetLoaded,
                     [this](HEMAX_EventData* Data) {
        this->UpdateLoadedAssetLibrariesList();
    });
    RegisterCallback(HEMAX_EventType::AssetRemoved,
                     [this](HEMAX_EventData* Data) {
        this->UpdateLoadedAssetLibrariesList();
    });
    RegisterCallback(HEMAX_EventType::InputChanged,
                     [this](HEMAX_EventData* Data) {
        this->Update();
    });
    RegisterCallback(HEMAX_EventType::ParameterChanged,
                     [this](HEMAX_EventData* Data) {
        this->Update();
    });
    RegisterCallback(HEMAX_EventType::NodeSettingsChanged,
                     [this](HEMAX_EventData* Data) {
        this->Update();
    });
    RegisterCallback(HEMAX_EventType::SelectionSetChanged,
                     [this](HEMAX_EventData* Data) {
        HEMAX_EventData_SelectionSetChanged* EventData =
            dynamic_cast<HEMAX_EventData_SelectionSetChanged*>(Data);

        if (!EventData)
            return;

        ChangeHdaSelection(EventData->Hda, EventData->ForceUnlock);
    });
    RegisterCallback(HEMAX_EventType::HdaPreDeleteNotification,
                     [this](HEMAX_EventData* Data) {
        HEMAX_EventData_HdaPreDeleteNotification* EventData =
            dynamic_cast<HEMAX_EventData_HdaPreDeleteNotification*>(Data);

        if (!EventData)
            return;

        HandleHdaPreDeleteEvent(EventData->Hda);
    });
    RegisterCallback(HEMAX_EventType::PreSessionStopped,
                    [this](HEMAX_EventData* Data) {
        ChangeHdaSelection(nullptr, true);
    });
}

HEMAX_UI::~HEMAX_UI()
{
    if (AssetWidget)
        delete AssetWidget;
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
    HdaWidget->SetSelection(Hda, ForceUnlock);

    if (Hda)
    {
	TabContainer->setCurrentWidget(HdaWidget);
    }
}

HEMAX_3dsmaxHda*
HEMAX_UI::GetCurrentHdaSelection()
{
    return HdaWidget->GetSelection();
}

void
HEMAX_UI::SetSelectionLocked(bool Locked)
{
    HdaWidget->SetLocked(Locked);
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
        HEMAX_SessionManager::GetSessionManager().GetStore().GetListOfLoadedAssets();
    AssetWidget->UpdateLoadedAssetList(&LoadedAssetList);
    HdaWidget->Update();

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
    HEMAX_Store& Store =
        HEMAX_SessionManager::GetSessionManager().GetStore();
    std::vector<std::string> Paths = Store.GetListOfLoadedAssets();
    AssetWidget->UpdateLoadedAssetList(&Paths);
}

void
HEMAX_UI::HandleHdaPreDeleteEvent(HEMAX_3dsmaxHda* Hda)
{
    if (GetCurrentHdaSelection() == Hda)
        ChangeHdaSelection(nullptr, true);
}
