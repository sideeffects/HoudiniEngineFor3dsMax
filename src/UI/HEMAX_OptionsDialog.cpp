#include "HEMAX_OptionsDialog.h"

#include "moc_HEMAX_OptionsDialog.cpp"

#include "../HEMAX_HoudiniApi.h"
#include "../HEMAX_Logger.h"
#include "../HEMAX_SessionManager.h"
#include "../HEMAX_Plugin.h"
#include "../HEMAX_UserPrefs.h"
#include "../HoudiniEngineFor3dsMax.h"

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025)
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtabwidget.h>
#endif

#ifdef HEMAX_VERSION_2017
#pragma warning(push, 0)
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtabwidget.h>
#pragma warning(pop)
#endif

#define HEMAX_DEBUG_DEFAULT_HIP_NAME "debug_hip_file.hip"

static const std::pair<std::string, int> SharedMemoryBufferTypeMenu_Ring =
    { HEMAX_SharedMemoryBufferType_Ring, 0 };
static const std::pair<std::string, int> SharedMemoryBufferTypeMenu_Fixed =
    { HEMAX_SharedMemoryBufferType_FixedLength, 1 };

HEMAX_OptionsDialog::HEMAX_OptionsDialog(HEMAX_Plugin* ThePlugin)
    : Plugin(ThePlugin)
{
    this->setWindowTitle("Houdini Engine for 3ds Max Options");
    
    Layout = new QVBoxLayout;
    OptionsTabs = new QTabWidget;

    Layout->setContentsMargins(5, 5, 5, 5);
    Layout->addWidget(OptionsTabs);

    GeneralOptions = new QWidget;
    GeneralOptionsLayout = new QVBoxLayout;

    GeneralOptions->setLayout(GeneralOptionsLayout);

    InstallationOptions = new QGroupBox("Installation");
    InstallationOptionsLayout = new QGridLayout;
    OverrideHoudiniInstallPathLabel = new QLabel("Override Houdini Install Path:");
    OverrideHoudiniInstallPath = new QLineEdit;
    OverrideHoudiniInstallPathDirBrowse = new QPushButton("...");
    RestartApplicationInstruction = new QLabel(
        "<font color=\"red\">3ds Max must be restarted for the changes to take effect.</font>");
    RestartApplicationInstruction->setHidden(true);
    InstallationOptionsLayout->addWidget(OverrideHoudiniInstallPathLabel, 0, 0, 1, 2);
    InstallationOptionsLayout->addWidget(OverrideHoudiniInstallPath, 1, 0);
    InstallationOptionsLayout->addWidget(OverrideHoudiniInstallPathDirBrowse, 1, 1);
    InstallationOptionsLayout->addWidget(RestartApplicationInstruction, 2, 0, 1, 2);
    InstallationOptions->setLayout(InstallationOptionsLayout);

    SelectionOptions = new QGroupBox("Selection");
    SelectionOptionsLayout = new QVBoxLayout;
    AutoSelectHDARoot = new QCheckBox("Automatically select HDA root "
            "node when selecting child geometry nodes.");

    SelectionOptions->setLayout(SelectionOptionsLayout);
    SelectionOptionsLayout->addWidget(AutoSelectHDARoot);

    OnStartupOptions = new QGroupBox("On Startup");
    OnStartupOptionsLayout = new QVBoxLayout;
    AutoOpenWindow = new QCheckBox("Open the plugin pane when 3ds Max starts");

    OnStartupOptions->setLayout(OnStartupOptionsLayout);
    OnStartupOptionsLayout->addWidget(AutoOpenWindow);

    AssetOptions = new QGroupBox("Assets");
    AssetOptionsLayout = new QGridLayout;

    HdaLoadDirLabel = new QLabel("HDA Load Directory:");
    HdaLoadDir = new QLineEdit;
    HdaLoadDirBrowse = new QPushButton("...");

    HdaSearchPathLabel = new QLabel("HDA Search Path:");
    HdaSearchPath = new QLineEdit;
    HdaSearchPathBrowse = new QPushButton("...");
    
    AssetOptions->setLayout(AssetOptionsLayout);
    AssetOptionsLayout->addWidget(HdaLoadDirLabel, 0, 0, 1, 2);
    AssetOptionsLayout->addWidget(HdaLoadDir, 1, 0);
    AssetOptionsLayout->addWidget(HdaLoadDirBrowse, 1, 1);
    AssetOptionsLayout->addWidget(HdaSearchPathLabel, 2, 0, 1, 2);
    AssetOptionsLayout->addWidget(HdaSearchPath, 3, 0);
    AssetOptionsLayout->addWidget(HdaSearchPathBrowse, 3, 1);
    
    GeneralOptionsLayout->addWidget(InstallationOptions);
    GeneralOptionsLayout->addWidget(SelectionOptions);
    GeneralOptionsLayout->addWidget(OnStartupOptions);
    GeneralOptionsLayout->addWidget(AssetOptions);

    SessionOptions = new QWidget;
    SessionOptionsLayout = new QGridLayout;
    SessionOptions->setLayout(SessionOptionsLayout);

    SessionAutoStart = new QCheckBox("Automatically Start Session on Plugin Start");

    SessionTypeLabel = new QLabel("Session Type");
    SessionTypeChoice = new QComboBox;
    SessionTypeChoice->addItem("Socket");
    SessionTypeChoice->addItem("Named Pipe");
    SessionTypeChoice->addItem("Shared Memory");

    SessionTimeoutLabel = new QLabel("Session Connection Timeout (s)");
    SessionTimeout = new QLineEdit;
    SessionTimeout->setValidator(&MyIntValidator);

    SessionHostnameLabel = new QLabel("Socket Hostname");
    SessionHostname = new QLineEdit();
    SessionPortLabel = new QLabel("Socket Port");
    SessionPort = new QLineEdit();
    SessionPipeNameLabel = new QLabel("Pipe Name");
    SessionPipeName = new QLineEdit();
    SessionSharedMemoryNameLabel = new QLabel("Shared Memory Name");
    SessionSharedMemoryName = new QLineEdit();
    SessionSharedMemoryBufferSizeLabel = new QLabel("Shared Memory Buffer Size (MB)");
    SessionSharedMemoryBufferSize = new QLineEdit();
    SessionSharedMemoryBufferTypeLabel = new QLabel("Shared Memory Buffer Type");
    SessionSharedMemoryBufferType = new QComboBox;
    SessionSharedMemoryBufferType->addItem("Ring");
    SessionSharedMemoryBufferType->addItem("Fixed Length");

    SessionHoudiniEnvFilesLabel = new QLabel("Houdini Environment Files");
    SessionHoudiniEnvFiles = new QLineEdit;
    SessionOtlSearchPathLabel = new QLabel("OTL Search Path");
    SessionOtlSearchPath = new QLineEdit;
    SessionDsoSearchPathLabel = new QLabel("DSO Search Path");
    SessionDsoSearchPath = new QLineEdit;
    SessionImageDsoSearchPathLabel = new QLabel("Image DSO Search Path");
    SessionImageDsoSearchPath = new QLineEdit;
    SessionAudioDsoSearchPathLabel = new QLabel("Audio DSO Search Path");
    SessionAudioDsoSearchPath = new QLineEdit;

    int row = 0;
    SessionOptionsLayout->addWidget(SessionAutoStart, row, 0, 1, 2);
    ++row;
    SessionOptionsLayout->addWidget(SessionTypeLabel, row, 0, Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionTypeChoice, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionTimeoutLabel, row, 0, Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionTimeout, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionHostnameLabel, row, 0, Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionHostname, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionPortLabel, row, 0, Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionPort, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionPipeNameLabel, row, 0, Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionPipeName, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionSharedMemoryNameLabel, row, 0,
        Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionSharedMemoryName, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionSharedMemoryBufferSizeLabel, row, 0,
        Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionSharedMemoryBufferSize, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionSharedMemoryBufferTypeLabel, row, 0,
        Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionSharedMemoryBufferType, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionHoudiniEnvFilesLabel, row, 0,
        Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionHoudiniEnvFiles, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionOtlSearchPathLabel, row, 0,
        Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionOtlSearchPath, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionDsoSearchPathLabel, row, 0,
        Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionDsoSearchPath, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionImageDsoSearchPathLabel, row, 0,
        Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionImageDsoSearchPath, row, 1);
    ++row;
    SessionOptionsLayout->addWidget(SessionAudioDsoSearchPathLabel, row, 0,
        Qt::AlignRight);
    SessionOptionsLayout->addWidget(SessionAudioDsoSearchPath, row, 1);
    ++row;

    DefaultOptions = new QWidget;
    DefaultOptionsLayout = new QVBoxLayout;
    DefaultOptions->setLayout(DefaultOptionsLayout);

    NodeOptionDefaults = new QGroupBox("Node Option Defaults");
    NodeOptionDefaultsLayout = new QVBoxLayout;
    NodeOptionDefaults->setLayout(NodeOptionDefaultsLayout);

    NodeOptionDefaultsAutoRecook = new QCheckBox(NodeOptionAutoRecookLabel);
    NodeOptionDefaultsSliderCook = new QCheckBox(NodeOptionSliderCookLabel);

    NodeOptionDefaultsLayout->addWidget(NodeOptionDefaultsAutoRecook);
    NodeOptionDefaultsLayout->addWidget(NodeOptionDefaultsSliderCook);

    DefaultOptionsLayout->addWidget(NodeOptionDefaults);

    GeometryHdaOptions = new QWidget;
    GeometryHdaOptionsLayout = new QVBoxLayout;
    GeometryHdaOptions->setLayout(GeometryHdaOptionsLayout);

    GeoHdaBakingOptions = new QGroupBox("Baking");
    GeoHdaBakingOptionsLayout = new QVBoxLayout;
    GeoHdaBakingOptions->setLayout(GeoHdaBakingOptionsLayout);

    BakeDummyObject = new QCheckBox("Bake Dummy Objects");
    GeoHdaBakingOptionsLayout->addWidget(BakeDummyObject);

    BakeCreateLayer = new QCheckBox("Create Layer for Baked Object");
    GeoHdaBakingOptionsLayout->addWidget(BakeCreateLayer);

    NodeOptions = new QGroupBox("Nodes");
    NodeOptionsLayout = new QVBoxLayout;
    NodeOptions->setLayout(NodeOptionsLayout);

    UseUniqueNames = new QCheckBox("Use Unique Names for Geometry Nodes");
    UseOriginalInstanceName = new QCheckBox("Use Original Node Name for Instances");
    NodeOptionsLayout->addWidget(UseUniqueNames);
    NodeOptionsLayout->addWidget(UseOriginalInstanceName);

    GeometryHdaOptionsLayout->addWidget(GeoHdaBakingOptions);
    GeometryHdaOptionsLayout->addWidget(NodeOptions);

    DebugOptions = new QWidget;
    DebugOptionsLayout = new QVBoxLayout;
    DebugOptions->setLayout(DebugOptionsLayout);

    LocationOptions = new QGroupBox("Locations");
    LocationOptionsLayout = new QGridLayout;
    LocationOptions->setLayout(LocationOptionsLayout);

    TempFilesFolderLabel = new QLabel("Temp Files Folder:");
    TempFilesFolder = new QLineEdit;
    TempFilesFolderBrowse = new QPushButton("...");

    LocationOptionsLayout->addWidget(TempFilesFolderLabel, 0, 0);
    LocationOptionsLayout->addWidget(TempFilesFolder, 0, 1);
    LocationOptionsLayout->addWidget(TempFilesFolderBrowse, 0, 2);

    HoudiniOptions = new QGroupBox("Houdini");
    HoudiniOptionsLayout = new QVBoxLayout;
    HoudiniOptions->setLayout(HoudiniOptionsLayout);

    SaveHipButton = new QPushButton("Save Houdini Scene (.hip)");
    OpenHoudiniButton = new QPushButton("Open Scene in Houdini");

    HoudiniOptionsLayout->addWidget(SaveHipButton);
    HoudiniOptionsLayout->addWidget(OpenHoudiniButton);

    LoggingOptions = new QGroupBox("Logging");
    LoggingOptionsLayout = new QVBoxLayout;
    LoggingOptions->setLayout(LoggingOptionsLayout);

    LoggingLevelsLabel = new QLabel("Enable logging for:");
    ErrorLogging = new QCheckBox("Errors");
    WarningLogging = new QCheckBox("Warning");
    InfoLogging = new QCheckBox("Info");

    LoggingOptionsLayout->addWidget(LoggingLevelsLabel);
    LoggingOptionsLayout->addWidget(ErrorLogging);
    LoggingOptionsLayout->addWidget(WarningLogging);
    LoggingOptionsLayout->addWidget(InfoLogging);

    DebugOptionsLayout->addWidget(LocationOptions);
    DebugOptionsLayout->addWidget(HoudiniOptions);
    DebugOptionsLayout->addWidget(LoggingOptions);

    OptionsTabs->addTab(GeneralOptions, "General");
    OptionsTabs->addTab(SessionOptions, "Sessions");
    OptionsTabs->addTab(DefaultOptions, "Defaults");
    OptionsTabs->addTab(GeometryHdaOptions, "Geometry HDA");
    OptionsTabs->addTab(DebugOptions, "Debug");

    this->setLayout(Layout);
    this->setMinimumWidth(350);

    InitializeOptions();

    QObject::connect(OverrideHoudiniInstallPath,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotOverrideHoudiniInstallPath()));

    QObject::connect(OverrideHoudiniInstallPathDirBrowse,
                     SIGNAL(clicked()),
                     this,
                     SLOT(SlotOverrideHoudiniInstallPathDirBrowse()));

    QObject::connect(AutoSelectHDARoot,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotAutoSelectHDARoot(int)));

    QObject::connect(AutoOpenWindow,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotAutoOpenWindow(int)));

    QObject::connect(HdaLoadDir,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotHdaLoadDir()));

    QObject::connect(HdaLoadDirBrowse,
                     SIGNAL(clicked()),
                     this,
                     SLOT(SlotHdaLoadDirBrowse()));

    QObject::connect(HdaSearchPath,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotHdaSearchPath()));

    QObject::connect(HdaSearchPathBrowse,
                     SIGNAL(clicked()),
                     this,
                     SLOT(SlotHdaSearchPathBrowse()));

    QObject::connect(SessionAutoStart,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotSessionAutoStart(int)));

    QObject::connect(SessionTypeChoice,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(SlotSessionDefaultStartType(int)));

    QObject::connect(SessionTimeout,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionTimeout()));

    QObject::connect(SessionHostname,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionHostname()));

    QObject::connect(SessionPort,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionPort()));

    QObject::connect(SessionPipeName,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionPipeName()));

    QObject::connect(SessionSharedMemoryName,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionSharedMemoryName()));

    QObject::connect(SessionSharedMemoryBufferSize,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionSharedMemoryBufferSize()));

    QObject::connect(SessionSharedMemoryBufferType,
                     SIGNAL(currentIndexChanged(int)),
                     this,
                     SLOT(SlotSessionSharedMemoryBufferType(int)));

    QObject::connect(SessionHoudiniEnvFiles,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionHoudiniEnvFiles()));

    QObject::connect(SessionOtlSearchPath,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionOtlSearchPath()));

    QObject::connect(SessionDsoSearchPath,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionDsoSearchPath()));

    QObject::connect(SessionImageDsoSearchPath,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionImageDsoSearchPath()));

    QObject::connect(SessionAudioDsoSearchPath,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotSessionAudioDsoSearchPath()));

    QObject::connect(NodeOptionDefaultsAutoRecook,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotNodeOptionDefaultsAutoRecook(int)));

    QObject::connect(NodeOptionDefaultsSliderCook,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotNodeOptionDefaultsSliderCook(int)));

    QObject::connect(BakeDummyObject,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotBakeDummyObject(int)));

    QObject::connect(BakeCreateLayer,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotBakeCreateLayer(int)));

    QObject::connect(UseUniqueNames,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotUseUniqueNames(int)));

    QObject::connect(UseOriginalInstanceName,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotUseOriginalInstanceName(int)));

    QObject::connect(TempFilesFolder,
                     SIGNAL(editingFinished()),
                     this,
                     SLOT(SlotTempFilesFolder()));

    QObject::connect(TempFilesFolderBrowse,
                     SIGNAL(clicked()),
                     this,
                     SLOT(SlotTempFilesFolderBrowse()));

    QObject::connect(SaveHipButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(SlotSaveHipButton()));

    QObject::connect(OpenHoudiniButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(SlotOpenHoudiniButton()));

    QObject::connect(ErrorLogging,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotErrorLogging(int)));

    QObject::connect(WarningLogging,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotWarningLogging(int)));

    QObject::connect(InfoLogging,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotInfoLogging(int)));

}

void
HEMAX_OptionsDialog::InitializeOptions()
{
    bool Checked = false;
    std::string SVal;
    int IVal;

    HEMAX_UserPrefs& Prefs = HEMAX_UserPrefs::Get();

    if (Prefs.GetStringSetting(HEMAX_SETTING_OVERRIDE_HFS, SVal))
        OverrideHoudiniInstallPath->setText(SVal.c_str());

    if (Prefs.GetBoolSetting(HEMAX_SETTING_GRAB_ROOT, Checked))
        AutoSelectHDARoot->setChecked(Checked);

    if (Prefs.GetBoolSetting(HEMAX_SETTING_AUTO_START_WINDOW, Checked))
        AutoOpenWindow->setChecked(Checked);

    if (Prefs.GetStringSetting(HEMAX_SETTING_HDA_LOAD_PATH, SVal))
        HdaLoadDir->setText(SVal.c_str());

    if (Prefs.GetStringSetting(HEMAX_SETTING_HDA_REPO_PATH, SVal))
        HdaSearchPath->setText(SVal.c_str());

    // Sessions 

    if (Prefs.GetBoolSetting(HEMAX_SETTING_AUTO_START_SESSION, Checked))
        SessionAutoStart->setChecked(Checked);

    if (Prefs.GetIntSetting(HEMAX_SETTING_SESSION_TYPE, IVal))
    {
        if (IVal == static_cast<int>(HEMAX_SessionTypePref::Socket))
            SessionTypeChoice->setCurrentIndex(0);
        else if (IVal == static_cast<int>(HEMAX_SessionTypePref::NamedPipe))
            SessionTypeChoice->setCurrentIndex(1);
        else if (IVal == static_cast<int>(HEMAX_SessionTypePref::SharedMemory))
            SessionTypeChoice->setCurrentIndex(2);
        else
        {
            SessionTypeChoice->setCurrentIndex(1);
            Prefs.SetIntSetting(
                HEMAX_SETTING_SESSION_TYPE,
                static_cast<int>(HEMAX_SessionTypePref::NamedPipe));
        }
    }

    if (Prefs.GetIntSetting(HEMAX_SETTING_SESSION_CONNECTION_TIMEOUT, IVal))
        SessionTimeout->setText(QString::number(IVal));

    if (Prefs.GetStringSetting(HEMAX_SETTING_SESSION_HOST_NAME, SVal))
        SessionHostname->setText(SVal.c_str());

    if (Prefs.GetIntSetting(HEMAX_SETTING_SESSION_PORT, IVal))
    {
        std::string PortString = std::to_string(IVal);
        SessionPort->setText(PortString.c_str());
    }

    if (Prefs.GetStringSetting(HEMAX_SETTING_SESSION_PIPE_NAME, SVal))
        SessionPipeName->setText(SVal.c_str());

    if (Prefs.GetStringSetting(HEMAX_SETTING_SESSION_SHARED_MEMORY_NAME, SVal))
        SessionSharedMemoryName->setText(SVal.c_str());

    if (Prefs.GetIntSetting(HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_SIZE, IVal))
    {
        std::string BufferSizeString = std::to_string(IVal);
        SessionSharedMemoryBufferSize->setText(BufferSizeString.c_str());
    }

    if (Prefs.GetStringSetting(HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_TYPE, SVal))
    {
        if (SVal == SharedMemoryBufferTypeMenu_Ring.first)
            SessionSharedMemoryBufferType->setCurrentIndex(SharedMemoryBufferTypeMenu_Ring.second);
        else if(SVal == SharedMemoryBufferTypeMenu_Fixed.first)
            SessionSharedMemoryBufferType->setCurrentIndex(SharedMemoryBufferTypeMenu_Fixed.second);
    }

    if (Prefs.GetStringSetting(HEMAX_SETTING_SESSION_ENV_FILES, SVal))
        SessionHoudiniEnvFiles->setText(SVal.c_str());

    if (Prefs.GetStringSetting(HEMAX_SETTING_SESSION_OTL_SEARCH, SVal))
        SessionOtlSearchPath->setText(SVal.c_str());

    if (Prefs.GetStringSetting(HEMAX_SETTING_SESSION_DSO_SEARCH, SVal))
        SessionDsoSearchPath->setText(SVal.c_str());

    if (Prefs.GetStringSetting(HEMAX_SETTING_SESSION_IMAGE_DSO_SEARCH, SVal))
        SessionImageDsoSearchPath->setText(SVal.c_str());

    if (Prefs.GetStringSetting(HEMAX_SETTING_SESSION_AUDIO_DSO_SEARCH, SVal))
        SessionAudioDsoSearchPath->setText(SVal.c_str());

    if (Prefs.GetBoolSetting(HEMAX_SETTING_NODE_OPTION_AUTORECOOK, Checked))
        NodeOptionDefaultsAutoRecook->setChecked(Checked);

    if (Prefs.GetBoolSetting(HEMAX_SETTING_NODE_OPTION_SLIDERCOOK, Checked))
        NodeOptionDefaultsSliderCook->setChecked(Checked);

    if (Prefs.GetBoolSetting(HEMAX_SETTING_BAKE_DUMMY_OBJECT, Checked))
        BakeDummyObject->setChecked(Checked);

    if (Prefs.GetBoolSetting(HEMAX_SETTING_BAKE_CREATE_LAYER, Checked))
        BakeCreateLayer->setChecked(Checked);

    if (Prefs.GetBoolSetting(HEMAX_SETTING_NODE_NAMES_UNIQUE, Checked))
        UseUniqueNames->setChecked(Checked);

    if (Prefs.GetBoolSetting(HEMAX_SETTING_NODE_INSTANCE_NAME_ORIGINAL, Checked))
        UseOriginalInstanceName->setChecked(Checked); 

    if (Prefs.GetStringSetting(HEMAX_SETTING_DEBUG_TEMP_DIR, SVal))
        TempFilesFolder->setText(SVal.c_str());

    if (Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_ERRORS, Checked))
        ErrorLogging->setChecked(Checked);

    if (Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_WARNINGS, Checked))
        WarningLogging->setChecked(Checked);

    if (Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_INFO, Checked))
        InfoLogging->setChecked(Checked);
}

void
HEMAX_OptionsDialog::Update()
{
    InitializeOptions();
}

void
HEMAX_OptionsDialog::SlotOverrideHoudiniInstallPath()
{
    HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_OVERRIDE_HFS,
        OverrideHoudiniInstallPath->text().toStdString());
    RestartApplicationInstruction->setHidden(false);
}

void
HEMAX_OptionsDialog::SlotOverrideHoudiniInstallPathDirBrowse()
{
    QString Dir = QFileDialog::getExistingDirectory(this, "",
        OverrideHoudiniInstallPath->text());

    if (!Dir.isEmpty())
    {
        HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_OVERRIDE_HFS,
            Dir.toStdString());
        OverrideHoudiniInstallPath->setText(Dir);
        RestartApplicationInstruction->setHidden(false);
    }
}

void
HEMAX_OptionsDialog::SlotAutoSelectHDARoot(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_GRAB_ROOT, State);
}

void
HEMAX_OptionsDialog::SlotAutoOpenWindow(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_AUTO_START_WINDOW,
        State);
}

void
HEMAX_OptionsDialog::SlotHdaLoadDir()
{
    HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_HDA_LOAD_PATH,
        HdaLoadDir->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotHdaLoadDirBrowse()
{
    QString Dir = QFileDialog::getExistingDirectory(this,
                            "", HdaLoadDir->text());
    if (!Dir.isEmpty())
    {
        HdaLoadDir->setText(Dir);
        Plugin->UpdateHdaLoadDirectory(Dir.toStdString());
    }
}

void
HEMAX_OptionsDialog::SlotHdaSearchPath()
{
    HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_HDA_REPO_PATH,
        HdaSearchPath->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotHdaSearchPathBrowse()
{
    QString Dir = QFileDialog::getExistingDirectory(this,
                            "", HdaSearchPath->text());
    if (!Dir.isEmpty())
    {
        HdaSearchPath->setText(Dir);
        HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_HDA_REPO_PATH,
            Dir.toStdString());
    }
}

void
HEMAX_OptionsDialog::SlotSessionAutoStart(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(
        HEMAX_SETTING_AUTO_START_SESSION, State);
}

void
HEMAX_OptionsDialog::SlotSessionDefaultStartType(int CurrentIndex)
{
    if (CurrentIndex == 0)
        HEMAX_UserPrefs::Get().SetIntSetting(HEMAX_SETTING_SESSION_TYPE,
            static_cast<int>(HEMAX_SessionTypePref::Socket));
    else if (CurrentIndex == 1)
        HEMAX_UserPrefs::Get().SetIntSetting(HEMAX_SETTING_SESSION_TYPE,
            static_cast<int>(HEMAX_SessionTypePref::NamedPipe));
    else if (CurrentIndex == 2)
        HEMAX_UserPrefs::Get().SetIntSetting(HEMAX_SETTING_SESSION_TYPE,
            static_cast<int>(HEMAX_SessionTypePref::SharedMemory));
}

void
HEMAX_OptionsDialog::SlotSessionTimeout()
{
    HEMAX_UserPrefs::Get().SetIntSetting(HEMAX_SETTING_SESSION_CONNECTION_TIMEOUT,
        SessionTimeout->text().toInt());
}

void
HEMAX_OptionsDialog::SlotSessionHostname()
{
    HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_SESSION_HOST_NAME,
        SessionHostname->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotSessionPort()
{
    try
    {
        HEMAX_UserPrefs::Get().SetIntSetting(HEMAX_SETTING_SESSION_PORT,
            std::stoi(SessionPort->text().toStdString()));
    }
    catch (const std::exception&)
    {
        QMessageBox ErrorPrompt(QMessageBox::Icon::Critical, "Invalid Input",
            "Invalid port number was provided. Please enter a valid port "
            "number.");
        ErrorPrompt.exec();
        SessionPort->setText("");
    }
}

void
HEMAX_OptionsDialog::SlotSessionPipeName()
{
    HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_SESSION_PIPE_NAME,
        SessionPipeName->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotSessionSharedMemoryName()
{
    HEMAX_UserPrefs::Get().SetStringSetting(
        HEMAX_SETTING_SESSION_SHARED_MEMORY_NAME,
        SessionSharedMemoryName->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotSessionSharedMemoryBufferSize()
{
    try
    {
        HEMAX_UserPrefs::Get().SetIntSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_SIZE,
            std::stoi(SessionSharedMemoryBufferSize->text().toStdString()));
    }
    catch (const std::exception&)
    {
        QMessageBox ErrorPrompt(QMessageBox::Icon::Critical, "Invalid Input",
            "Invalid shared memory buffer size was provided. Please enter a "
            "valid number.");
        ErrorPrompt.exec();
        SessionSharedMemoryBufferSize->setText("");
    }
}

void
HEMAX_OptionsDialog::SlotSessionSharedMemoryBufferType(int CurrentIndex)
{
    if (CurrentIndex == SharedMemoryBufferTypeMenu_Ring.second)
        HEMAX_UserPrefs::Get().SetStringSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_TYPE,
            SharedMemoryBufferTypeMenu_Ring.first.c_str());
    else if (CurrentIndex == SharedMemoryBufferTypeMenu_Fixed.second)
        HEMAX_UserPrefs::Get().SetStringSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_TYPE,
            SharedMemoryBufferTypeMenu_Fixed.first.c_str());
}

void
HEMAX_OptionsDialog::SlotSessionHoudiniEnvFiles()
{
    HEMAX_UserPrefs::Get().SetStringSetting(
        HEMAX_SETTING_SESSION_ENV_FILES,
        SessionHoudiniEnvFiles->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotSessionOtlSearchPath()
{
    HEMAX_UserPrefs::Get().SetStringSetting(
        HEMAX_SETTING_SESSION_OTL_SEARCH,
        SessionOtlSearchPath->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotSessionDsoSearchPath()
{
    HEMAX_UserPrefs::Get().SetStringSetting(
        HEMAX_SETTING_SESSION_DSO_SEARCH,
        SessionDsoSearchPath->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotSessionImageDsoSearchPath()
{
    HEMAX_UserPrefs::Get().SetStringSetting(
        HEMAX_SETTING_SESSION_IMAGE_DSO_SEARCH,
        SessionImageDsoSearchPath->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotSessionAudioDsoSearchPath()
{
    HEMAX_UserPrefs::Get().SetStringSetting(
        HEMAX_SETTING_SESSION_AUDIO_DSO_SEARCH,
        SessionAudioDsoSearchPath->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotNodeOptionDefaultsAutoRecook(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_NODE_OPTION_AUTORECOOK,
        State);
}

void
HEMAX_OptionsDialog::SlotNodeOptionDefaultsSliderCook(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_NODE_OPTION_SLIDERCOOK,
        State);
}

void
HEMAX_OptionsDialog::SlotBakeDummyObject(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_BAKE_DUMMY_OBJECT,
        State);
}

void
HEMAX_OptionsDialog::SlotBakeCreateLayer(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_BAKE_CREATE_LAYER,
        State);
}

void
HEMAX_OptionsDialog::SlotUseUniqueNames(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_NODE_NAMES_UNIQUE,
        State);
}

void
HEMAX_OptionsDialog::SlotUseOriginalInstanceName(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(
        HEMAX_SETTING_NODE_INSTANCE_NAME_ORIGINAL, State);
}

void
HEMAX_OptionsDialog::SlotTempFilesFolder()
{
    HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_DEBUG_TEMP_DIR,
        TempFilesFolder->text().toStdString());
}

void
HEMAX_OptionsDialog::SlotTempFilesFolderBrowse()
{
    QString Dir = QFileDialog::getExistingDirectory(this,
                            "", TempFilesFolder->text());
    if (!Dir.isEmpty())
    {
        TempFilesFolder->setText(Dir);
        HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_DEBUG_TEMP_DIR,
            Dir.toStdString());
    }
}

void
HEMAX_OptionsDialog::SlotSaveHipButton()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.IsSessionValidAndInitialized())
    {
	QFileDialog SaveHIPDialog;

	SaveHIPDialog.setFileMode(QFileDialog::AnyFile);
	SaveHIPDialog.setNameFilter(tr("Houdini Scene File (*.hip)"));
	SaveHIPDialog.setDefaultSuffix("hip");
	SaveHIPDialog.setAcceptMode(QFileDialog::AcceptSave);

	if (SaveHIPDialog.exec())
	{
	    QStringList Selected = SaveHIPDialog.selectedFiles();

	    QFileInfo PathInfo(Selected[0]);

	    if (PathInfo.completeSuffix() == "hip")
	    {
		std::string SelectedPath = Selected[0].toStdString();
                HEMAX_HoudiniApi::SaveHIPFile(&SM.Session, SelectedPath.c_str(),
                    false);
	    }
	}
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(
            "Cannot save Houdini .hip file (no Houdini session is active)",
            HEMAX_LOG_LEVEL_ERROR);
    }
}

void
HEMAX_OptionsDialog::SlotOpenHoudiniButton()
{
    if (!HEMAX_SessionManager::GetSessionManager().IsSessionValidAndInitialized())
    {
        HEMAX_Logger::Instance().AddEntry(
            "Cannot open Houdini (no Houdini session is active).",
            HEMAX_LOG_LEVEL_ERROR);
        return;
    }

    if (TempFilesFolder->text().isEmpty())
    {
        HEMAX_Logger::Instance().AddEntry(
            "Cannot open Houdini (a Temp Files Folder has not been specified",
            HEMAX_LOG_LEVEL_ERROR);
        return;
    }

    HEMAX_SessionManager& SessionManager =
                        HEMAX_SessionManager::GetSessionManager();

    std::string TempHIPPath = TempFilesFolder->text().toStdString()
        + "\\" + HEMAX_DEBUG_DEFAULT_HIP_NAME;
    HEMAX_HoudiniApi::SaveHIPFile(&SessionManager.Session, TempHIPPath.c_str(),
        false);

    STARTUPINFOA StartupInfo;
    PROCESS_INFORMATION ProcessInfo;

    ZeroMemory(&StartupInfo, sizeof(StartupInfo));
    ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
    StartupInfo.cb = sizeof(StartupInfo);

    std::wstring HAPILDir = HEMAXLauncher::GetInstance()->GetLibHAPILDirectory();
    std::string ExecutablePath = std::string(HAPILDir.begin(), HAPILDir.end())
                                 + "\\houdinifx.exe";

    std::string CommandLine = '"' + ExecutablePath + "\"  \""
                              + TempHIPPath + '"';

    char* CmdLineStr = new char[CommandLine.length() + 1];
    strcpy_s(CmdLineStr, CommandLine.length() + 1, CommandLine.c_str());

    if (!CreateProcessA(ExecutablePath.c_str(),
                        CmdLineStr,
                        nullptr,
                        nullptr,
                        false,
                        NORMAL_PRIORITY_CLASS,
                        nullptr,
                        nullptr,
                        &StartupInfo,
                        &ProcessInfo))
    {
        DWORD ErrorCode = GetLastError();

        CHAR ErrorTextBuffer[512];
        DWORD NumberOfChars;

        NumberOfChars = FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                ErrorCode,
                0,
                ErrorTextBuffer,
                512,
                nullptr);

        if (NumberOfChars)
        {
            std::string Msg = std::string("Could not launch Houdini "
                    "executable due to error: ") + ErrorTextBuffer;
            HEMAX_Logger::Instance().AddEntry(Msg.c_str(),
                                              HEMAX_LOG_LEVEL_ERROR);
        }
        else
        {
            std::string Msg = "Could not launch Houdini executable due to "
                "unknown error (could not retrieve Windows error message";
            HEMAX_Logger::Instance().AddEntry(Msg.c_str(),
                                              HEMAX_LOG_LEVEL_ERROR);
        }
    }

    delete CmdLineStr;
}

void
HEMAX_OptionsDialog::SlotErrorLogging(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_ERRORS,
        State);
    HEMAX_Logger::Instance().ConfigurePrintLevels(HEMAX_LOG_LEVEL_ERROR,
                                                  State);
}

void
HEMAX_OptionsDialog::SlotWarningLogging(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_WARNINGS,
        State);
    HEMAX_Logger::Instance().ConfigurePrintLevels(HEMAX_LOG_LEVEL_WARN,
                                                  State);
}

void
HEMAX_OptionsDialog::SlotInfoLogging(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_INFO,
        State);
    HEMAX_Logger::Instance().ConfigurePrintLevels(HEMAX_LOG_LEVEL_INFO,
                                                  State);
}
