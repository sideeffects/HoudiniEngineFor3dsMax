#include "HEMAX_DebugWidget.h"
#include "../HEMAX_SessionManager.h"
#include "../HoudiniEngineFor3DSMax.h"
#include <windows.h>
#include "../HEMAX_Logger.h"

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qfiledialog.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qfiledialog.h>
#endif

#include "moc_HEMAX_DebugWidget.cpp"

HEMAX_DebugWidget::HEMAX_DebugWidget()
{
    UserPrefs = nullptr;

    Layout = new QVBoxLayout;
    Layout->setAlignment(Qt::AlignTop);

    HoudiniDebugConfigBox = new QGroupBox("Configuration");
    HoudiniDebugConfigBoxLayout = new QGridLayout;
    HoudiniDebugConfigBoxLayout->setAlignment(Qt::AlignTop);

    TempFilesLabel = new QLabel("Temp Files Folder:");
    TempFilesDirectoryField = new QLineEdit;
    TempFilesDirectoryField->setEnabled(false);
    TempFilesDirectoryBrowseButton = new QPushButton("...");

    HoudiniDebugConfigBoxLayout->addWidget(TempFilesLabel, 0, 0);
    HoudiniDebugConfigBoxLayout->addWidget(TempFilesDirectoryField, 0, 1);
    HoudiniDebugConfigBoxLayout->addWidget(TempFilesDirectoryBrowseButton, 0, 2);
    HoudiniDebugConfigBox->setLayout(HoudiniDebugConfigBoxLayout);

    HoudiniDebuggingBox = new QGroupBox("Houdini");
    HoudiniDebuggingLayout = new QVBoxLayout;
    HoudiniDebuggingLayout->setAlignment(Qt::AlignTop);
    SaveHoudiniSceneButton = new QPushButton("Save Houdini Scene (.hip)");
    OpenInHoudiniButton = new QPushButton("Open Scene in Houdini");

    LoggingBox = new QGroupBox("Logging");
    LoggingLayout = new QGridLayout;
    LoggingLevels = new QLabel("Enable logging for levels:");
    ErrorLevelCheckbox = new QCheckBox("Error");
    WarnLevelCheckbox = new QCheckBox("Warning");
    InfoLevelCheckbox = new QCheckBox("Info");

    LoggingLayout->addWidget(LoggingLevels, 0, 0);
    LoggingLayout->addWidget(ErrorLevelCheckbox, 1, 0);
    LoggingLayout->addWidget(WarnLevelCheckbox, 2, 0);
    LoggingLayout->addWidget(InfoLevelCheckbox, 3, 0);

    LoggingBox->setLayout(LoggingLayout);
    LoggingLayout->setAlignment(Qt::AlignTop);
    LoggingBox->setAlignment(Qt::AlignTop);

    HoudiniDebuggingLayout->addWidget(SaveHoudiniSceneButton);
    HoudiniDebuggingLayout->addWidget(OpenInHoudiniButton);

    HoudiniDebuggingBox->setLayout(HoudiniDebuggingLayout);

    Layout->addWidget(HoudiniDebugConfigBox);
    Layout->addWidget(HoudiniDebuggingBox);
    Layout->addWidget(LoggingBox);

    this->setLayout(Layout);

    QObject::connect(TempFilesDirectoryBrowseButton, SIGNAL(clicked()), this, SLOT(SlotTempDirBrowseButtonClicked()));
    QObject::connect(SaveHoudiniSceneButton, SIGNAL(clicked()), this, SLOT(SlotSaveHoudiniSceneButtonClicked()));
    QObject::connect(OpenInHoudiniButton, SIGNAL(clicked()), this, SLOT(SlotOpenInHoudiniButtonClicked()));

    QObject::connect(ErrorLevelCheckbox, SIGNAL(stateChanged(int)), this, SLOT(SlotErrorLevelToggleStateChanged(int)));
    QObject::connect(WarnLevelCheckbox, SIGNAL(stateChanged(int)), this, SLOT(SlotWarnLevelToggleStateChanged(int)));
    QObject::connect(InfoLevelCheckbox, SIGNAL(stateChanged(int)), this, SLOT(SlotInfoLevelToggleStateChanged(int)));
}

HEMAX_DebugWidget::~HEMAX_DebugWidget()
{
    delete InfoLevelCheckbox;
    delete WarnLevelCheckbox;
    delete ErrorLevelCheckbox;

    delete LoggingLevels;
    delete LoggingLayout;
    delete LoggingBox;

    delete OpenInHoudiniButton;
    delete SaveHoudiniSceneButton;
    delete HoudiniDebuggingLayout;
    delete HoudiniDebuggingBox;

    delete TempFilesDirectoryBrowseButton;
    delete TempFilesDirectoryField;
    delete TempFilesLabel;

    delete Layout;
}

void
HEMAX_DebugWidget::SetTempDirectory(std::string Directory)
{
    TempFilesDirectoryField->setText(Directory.c_str());
}

void
HEMAX_DebugWidget::SetUserPrefs(HEMAX_UserPrefs* UserPreferences)
{
    UserPrefs = UserPreferences;

    bool Enabled;

    if (UserPrefs->GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_ERRORS, Enabled))
    {
        ErrorLevelCheckbox->setChecked(Enabled);
    }

    if (UserPrefs->GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_WARNINGS, Enabled))
    {
        WarnLevelCheckbox->setChecked(Enabled);
    }

    if (UserPrefs->GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_INFO, Enabled))
    {
        InfoLevelCheckbox->setChecked(Enabled);
    }
}

void
HEMAX_DebugWidget::SlotTempDirBrowseButtonClicked()
{
    TempFilesDirectoryField->setText(QFileDialog::getExistingDirectory());
    emit SignalTempDirChanged(TempFilesDirectoryField->text().toStdString());
}

void
HEMAX_DebugWidget::SlotSaveHoudiniSceneButtonClicked()
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    if (SessionManager.IsSessionActive())
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
                SessionManager.Session->SaveHIPFile(SelectedPath.c_str(), false);
            }
        }
    }
}

void
HEMAX_DebugWidget::SlotOpenInHoudiniButtonClicked()
{
    if (HEMAX_SessionManager::GetSessionManager().IsSessionActive() && !TempFilesDirectoryField->text().isEmpty())
    {
        HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

        std::string TempHIPPath = TempFilesDirectoryField->text().toStdString() + "\\" + HEMAX_DEBUG_DEFAULT_HIP_NAME;
        SessionManager.Session->SaveHIPFile(TempHIPPath.c_str(), false);

        STARTUPINFOA StartupInfo;
        PROCESS_INFORMATION ProcessInfo;

        ZeroMemory(&StartupInfo, sizeof(StartupInfo));
        ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
        StartupInfo.cb = sizeof(StartupInfo);

        std::string ExecutablePath = HEMAXLauncher::GetLibHAPILDirectory() + "\\houdinifx.exe";

        std::string CommandLine = '"' + ExecutablePath + "\"  \"" + TempHIPPath + '"';

        char* CmdLineStr = new char[CommandLine.length() + 1];
        strcpy_s(CmdLineStr, CommandLine.length() + 1, CommandLine.c_str());

        if (!CreateProcessA(ExecutablePath.c_str(), CmdLineStr, nullptr, nullptr, false, NORMAL_PRIORITY_CLASS, nullptr, nullptr, &StartupInfo, &ProcessInfo))
        {
            DWORD ErrorCode = GetLastError();

            CHAR ErrorTextBuffer[512];
            DWORD NumberOfChars;

            NumberOfChars = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, ErrorCode, 0, ErrorTextBuffer, 512, nullptr);

            if (NumberOfChars)
            {
                std::string Msg = std::string("HEMAX_DebugWidget::could not launch Houdini executable due to error: ") + ErrorTextBuffer;
                HEMAX_Logger::Instance().AddEntry(Msg.c_str(), HEMAX_LOG_LEVEL_ERROR);
            }
            else
            {
                std::string Msg = "HEMAX_DebugWidget::could not launch Houdini executable. Could not retrieve Windows error message";
                HEMAX_Logger::Instance().AddEntry(Msg.c_str(), HEMAX_LOG_LEVEL_ERROR);
            }
        }

        delete CmdLineStr;
    }
}

void
HEMAX_DebugWidget::SlotErrorLevelToggleStateChanged(int State)
{
    if (UserPrefs)
    {
        UserPrefs->SetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_ERRORS, State);
        HEMAX_Logger::Instance().ConfigurePrintLevels(HEMAX_LOG_LEVEL_ERROR, State);
    }
}

void
HEMAX_DebugWidget::SlotWarnLevelToggleStateChanged(int State)
{
    if (UserPrefs)
    {
        UserPrefs->SetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_WARNINGS, State);
        HEMAX_Logger::Instance().ConfigurePrintLevels(HEMAX_LOG_LEVEL_WARN, State);
    }
}

void
HEMAX_DebugWidget::SlotInfoLevelToggleStateChanged(int State)
{
    if (UserPrefs)
    {
        UserPrefs->SetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_INFO, State);
        HEMAX_Logger::Instance().ConfigurePrintLevels(HEMAX_LOG_LEVEL_INFO, State);
    }
}
