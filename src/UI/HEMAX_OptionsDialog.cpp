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
    defined(HEMAX_VERSION_2024)
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtabwidget.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qcheckbox.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtabwidget.h>
#endif

#define HEMAX_DEBUG_DEFAULT_HIP_NAME "debug_hip_file.hip"

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
    AutoStartSession = new QCheckBox("Automatically start a session when "
            "3ds Max starts");
    AutoOpenWindow = new QCheckBox("Open the plugin pane when 3ds Max starts");

    OnStartupOptions->setLayout(OnStartupOptionsLayout);
    OnStartupOptionsLayout->addWidget(AutoStartSession);
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

    QObject::connect(AutoStartSession,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(SlotAutoStartSession(int)));

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
    HEMAX_UserPrefs& Prefs = HEMAX_UserPrefs::Get();

    std::string SVal;

    if (Prefs.GetStringSetting(HEMAX_SETTING_OVERRIDE_HFS, SVal))
    {
        OverrideHoudiniInstallPath->setText(SVal.c_str());
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_GRAB_ROOT, Checked))
    {
        AutoSelectHDARoot->setChecked(Checked);
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_AUTO_START_SESSION, Checked))
    {
        AutoStartSession->setChecked(Checked);
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_AUTO_START_WINDOW, Checked))
    {
        AutoOpenWindow->setChecked(Checked);
    }

    if (Prefs.GetStringSetting(HEMAX_SETTING_HDA_LOAD_PATH, SVal))
    {
        HdaLoadDir->setText(SVal.c_str());
    }

    if (Prefs.GetStringSetting(HEMAX_SETTING_HDA_REPO_PATH, SVal))
    {
        HdaSearchPath->setText(SVal.c_str());
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_NODE_OPTION_AUTORECOOK, Checked))
    {
        NodeOptionDefaultsAutoRecook->setChecked(Checked);
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_NODE_OPTION_SLIDERCOOK, Checked))
    {
        NodeOptionDefaultsSliderCook->setChecked(Checked);
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_BAKE_DUMMY_OBJECT, Checked))
    {
        BakeDummyObject->setChecked(Checked);
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_BAKE_CREATE_LAYER, Checked))
    {
        BakeCreateLayer->setChecked(Checked);
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_NODE_NAMES_UNIQUE, Checked))
    {
        UseUniqueNames->setChecked(Checked);
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_NODE_INSTANCE_NAME_ORIGINAL, Checked))
    {
        UseOriginalInstanceName->setChecked(Checked); 
    }

    if (Prefs.GetStringSetting(HEMAX_SETTING_DEBUG_TEMP_DIR, SVal))
    {
        TempFilesFolder->setText(SVal.c_str());
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_ERRORS, Checked))
    {
        ErrorLogging->setChecked(Checked);
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_WARNINGS, Checked))
    {
        WarningLogging->setChecked(Checked);
    }

    if (Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_INFO, Checked))
    {
        InfoLogging->setChecked(Checked);
    }
}

HEMAX_OptionsDialog::~HEMAX_OptionsDialog()
{

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
HEMAX_OptionsDialog::SlotAutoStartSession(int State)
{
    HEMAX_UserPrefs::Get().SetBoolSetting(HEMAX_SETTING_AUTO_START_SESSION,
        State);
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
        Plugin->UpdateHdaSearchPathDirectory(Dir.toStdString());
    }
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

    if (SM.IsSessionActive())
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
                HEMAX_HoudiniApi::SaveHIPFile(SM.Session, SelectedPath.c_str(),
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
    if (!HEMAX_SessionManager::GetSessionManager().IsSessionActive())
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
    HEMAX_HoudiniApi::SaveHIPFile(SessionManager.Session, TempHIPPath.c_str(),
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
