#include "HEMAX_SessionWidget.h"
#include "../HEMAX_HAPISession.h"
#include "../HEMAX_SessionManager.h"
#include "../HEMAX_UserPrefs.h"

#include "moc_HEMAX_SessionWidget.cpp"

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qfiledialog.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qfiledialog.h>
#endif

HEMAX_SessionWidget::HEMAX_SessionWidget()
    : QWidget()
{
    MainLayout = new QVBoxLayout;

    SessionsBox = new QWidget(this);
    SessionsBoxLayout = new QVBoxLayout;

    SessionsControlLayout = new QHBoxLayout;
    SessionsTypeLayout = new QVBoxLayout;
    SessionsPipeLayout = new QHBoxLayout;
    SessionsSocketLayout = new QHBoxLayout;

    SessionsStartButton = new QPushButton("Start Session");
    SessionsStopButton = new QPushButton("Stop Session");

    SessionsTypeBox = new QGroupBox("Session Type");
    AutoStartOptionsLayout = new QHBoxLayout;
    ManualStartOptionsLayout = new QHBoxLayout;
    SessionsAutoStartLabel = new QLabel("Auto-Start Session");
    SessionsManualStartLabel = new QLabel("Manual Session");
    SessionsOutOfProcessRadioButton = new QRadioButton("Out of Process");
    SessionsInProcessRadioButton = new QRadioButton("In-Process");
    SessionsPipeRadioButton = new QRadioButton("Thift Pipe");
    SessionsSocketRadioButton = new QRadioButton("Thrift Socket");

    SessionsHostNameLabel = new QLabel("Host Name:");
    SessionsSocketHostName = new QLineEdit;
    SessionsSocketHostName->setText(HEMAX_SESSION_HOST_NAME_DEFAULT);
    SessionsPortNumberLabel = new QLabel("Port Number:");
    SessionsSocketPortNumber = new QLineEdit;
    SessionsSocketPortNumber->setText(HEMAX_SESSION_PORT_NUM_DEFAULT);

    SessionsPipeNameLabel = new QLabel("Pipe Name:");
    SessionsPipeName = new QLineEdit;
    SessionsPipeName->setText(HEMAX_SESSION_PIPE_NAME_DEFAULT);

    SessionsConfigurationBox = new QGroupBox("Session Configuration");
    SessionsConfigurationBoxLayout = new QGridLayout;

    SessionsHoudiniEnvFilesLabel = new QLabel("Houdini Environment Files:");
    SessionsHoudiniEnvFiles = new QLineEdit;
    SessionsHoudiniEnvFilesBrowse = new QPushButton("...");
    SessionsOtlSearchPathLabel = new QLabel("otl Search Path:");
    SessionsOtlSearchPath = new QLineEdit;
    SessionsOtlSearchPathBrowse = new QPushButton("...");
    SessionsDsoSearchPathLabel = new QLabel("dso Search Path: ");
    SessionsDsoSearchPath = new QLineEdit;
    SessionsDsoSearchPathBrowse = new QPushButton("...");
    SessionsImageDsoSearchPathLabel = new QLabel("Image dso Search Path:");
    SessionsImageDsoSearchPath = new QLineEdit;
    SessionsImageDsoSearchPathBrowse = new QPushButton("...");
    SessionsAudioDsoSearchPathLabel = new QLabel("Audio dso Search Path:");
    SessionsAudioDsoSearchPath = new QLineEdit;
    SessionsAudioDsoSearchPathBrowse = new QPushButton("...");

    MainLayout->setAlignment(Qt::AlignTop);
    SessionsBoxLayout->setAlignment(Qt::AlignTop);
    SessionsControlLayout->setAlignment(Qt::AlignTop);
    SessionsTypeLayout->setAlignment(Qt::AlignTop);
    SessionsPipeLayout->setAlignment(Qt::AlignTop);
    SessionsSocketLayout->setAlignment(Qt::AlignTop);
    SessionsConfigurationBox->setAlignment(Qt::AlignTop);

    SessionsControlLayout->addWidget(SessionsStartButton);
    SessionsControlLayout->addWidget(SessionsStopButton);

    SessionsBoxLayout->addLayout(SessionsControlLayout);

    SessionsOutOfProcessRadioButton->setChecked(true);

    AutoStartOptionsLayout->setAlignment(Qt::AlignLeft);
    ManualStartOptionsLayout->setAlignment(Qt::AlignLeft);

    AutoStartOptionsLayout->addWidget(SessionsOutOfProcessRadioButton);
    //AutoStartOptionsLayout->addWidget(SessionsInProcessRadioButton);

    ManualStartOptionsLayout->addWidget(SessionsSocketRadioButton);
    ManualStartOptionsLayout->addWidget(SessionsPipeRadioButton);
    
    SessionsTypeLayout->addWidget(SessionsAutoStartLabel);
    SessionsTypeLayout->addLayout(AutoStartOptionsLayout);
    SessionsTypeLayout->addWidget(SessionsManualStartLabel);
    SessionsTypeLayout->addLayout(ManualStartOptionsLayout);

    SessionsTypeLayout->setAlignment(Qt::AlignLeft);

    SessionsTypeBox->setLayout(SessionsTypeLayout);

    SessionsBoxLayout->addWidget(SessionsTypeBox);

    SessionsSocketLayout->addWidget(SessionsHostNameLabel);
    SessionsSocketLayout->addWidget(SessionsSocketHostName);
    SessionsSocketLayout->addWidget(SessionsPortNumberLabel);
    SessionsSocketLayout->addWidget(SessionsSocketPortNumber);

    SessionsBoxLayout->addLayout(SessionsSocketLayout);

    SessionsPipeLayout->addWidget(SessionsPipeNameLabel);
    SessionsPipeLayout->addWidget(SessionsPipeName);

    SessionsBoxLayout->addLayout(SessionsPipeLayout);

    SessionsBox->setLayout(SessionsBoxLayout);

    SessionsConfigurationBoxLayout->addWidget(SessionsHoudiniEnvFilesLabel, 0, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsHoudiniEnvFiles, 1, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsHoudiniEnvFilesBrowse, 1, 1);
    SessionsConfigurationBoxLayout->addWidget(SessionsOtlSearchPathLabel, 2, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsOtlSearchPath, 3, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsOtlSearchPathBrowse, 3, 1);
    SessionsConfigurationBoxLayout->addWidget(SessionsDsoSearchPathLabel, 4, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsDsoSearchPath, 5, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsDsoSearchPathBrowse, 5, 1);
    SessionsConfigurationBoxLayout->addWidget(SessionsImageDsoSearchPathLabel, 6, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsImageDsoSearchPath, 7, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsImageDsoSearchPathBrowse, 7, 1);
    SessionsConfigurationBoxLayout->addWidget(SessionsAudioDsoSearchPathLabel, 8, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsAudioDsoSearchPath, 9, 0);
    SessionsConfigurationBoxLayout->addWidget(SessionsAudioDsoSearchPathBrowse, 9, 1);

    SessionsConfigurationBox->setLayout(SessionsConfigurationBoxLayout);

    MainLayout->addWidget(SessionsBox);
    MainLayout->addWidget(SessionsConfigurationBox);

    this->setLayout(MainLayout);

    SlotSessionsSocketToggle(false);
    SlotSessionsPipeToggle(false);

    QObject::connect(SessionsStartButton, SIGNAL(clicked()), this, SLOT(SlotSessionsStartButtonClicked()));
    QObject::connect(SessionsStopButton, SIGNAL(clicked()), this, SLOT(SlotSessionsStopButtonClicked()));

    QObject::connect(SessionsSocketRadioButton, SIGNAL(toggled(bool)), this, SLOT(SlotSessionsSocketToggle(bool)));
    QObject::connect(SessionsPipeRadioButton, SIGNAL(toggled(bool)), this, SLOT(SlotSessionsPipeToggle(bool)));

    QObject::connect(SessionsHoudiniEnvFilesBrowse, SIGNAL(clicked()), this, SLOT(SlotSessionsHoudiniEnvFileBrowse()));
    QObject::connect(SessionsOtlSearchPathBrowse, SIGNAL(clicked()), this, SLOT(SlotSessionsOtlSearchPathBrowse()));
    QObject::connect(SessionsDsoSearchPathBrowse, SIGNAL(clicked()), this, SLOT(SlotSessionsDsoSearchPathBrowse()));
    QObject::connect(SessionsImageDsoSearchPathBrowse, SIGNAL(clicked()), this, SLOT(SlotSessionsImageDsoSearchPathBrowse()));
    QObject::connect(SessionsAudioDsoSearchPathBrowse, SIGNAL(clicked()), this, SLOT(SlotSessionsAudioDsoSearchPathBrowse()));

    QObject::connect(SessionsHoudiniEnvFiles, SIGNAL(editingFinished()), this, SLOT(SlotSessionsHoudiniEnvFileEdited()));
    QObject::connect(SessionsOtlSearchPath, SIGNAL(editingFinished()), this, SLOT(SlotSessionsOtlSearchPathEdited()));
    QObject::connect(SessionsDsoSearchPath, SIGNAL(editingFinished()), this, SLOT(SlotSessionsDsoSearchPathEdited()));
    QObject::connect(SessionsImageDsoSearchPath, SIGNAL(editingFinished()), this, SLOT(SlotSessionsImageSearchPathEdited()));
    QObject::connect(SessionsAudioDsoSearchPath, SIGNAL(editingFinished()), this, SLOT(SlotSessionsAudioSearchPathEdited()));
}

HEMAX_SessionWidget::~HEMAX_SessionWidget()
{
    delete SessionsAudioDsoSearchPathBrowse;
    delete SessionsAudioDsoSearchPath;
    delete SessionsAudioDsoSearchPathLabel;
    delete SessionsImageDsoSearchPathBrowse;
    delete SessionsImageDsoSearchPath;
    delete SessionsImageDsoSearchPathLabel;
    delete SessionsDsoSearchPathBrowse;
    delete SessionsDsoSearchPath;
    delete SessionsDsoSearchPathLabel;
    delete SessionsOtlSearchPathBrowse;
    delete SessionsOtlSearchPath;
    delete SessionsOtlSearchPathLabel;
    delete SessionsHoudiniEnvFilesBrowse;
    delete SessionsHoudiniEnvFiles;
    delete SessionsHoudiniEnvFilesLabel;

    delete SessionsConfigurationBoxLayout;
    delete SessionsConfigurationBox;

    delete SessionsPipeName;
    delete SessionsPipeNameLabel;

    delete SessionsSocketPortNumber;
    delete SessionsPortNumberLabel;
    delete SessionsSocketHostName;
    delete SessionsHostNameLabel;

    delete SessionsAutoStartLabel;
    delete SessionsManualStartLabel;

    delete SessionsPipeRadioButton;
    delete SessionsSocketRadioButton;
    delete SessionsInProcessRadioButton;
    delete SessionsOutOfProcessRadioButton;

    delete SessionsStopButton;
    delete SessionsStartButton;

    delete SessionsSocketLayout;
    delete SessionsPipeLayout;
    delete SessionsTypeLayout;
    delete SessionsControlLayout;

    delete SessionsTypeBox;

    delete SessionsBoxLayout;
    delete SessionsBox;
    
    delete MainLayout;
}

void
HEMAX_SessionWidget::PushHoudiniEnvFilePath(std::string Path)
{
    SessionsHoudiniEnvFiles->setText(Path.c_str());
}

void
HEMAX_SessionWidget::PushOtlSearchPath(std::string Path)
{
    SessionsOtlSearchPath->setText(Path.c_str());
}

void
HEMAX_SessionWidget::PushDsoSearchPath(std::string Path)
{
    SessionsDsoSearchPath->setText(Path.c_str());
}

void
HEMAX_SessionWidget::PushImageDsoSearchPath(std::string Path)
{
    SessionsImageDsoSearchPath->setText(Path.c_str());
}

void
HEMAX_SessionWidget::PushAudioDsoSearchPath(std::string Path)
{
    SessionsAudioDsoSearchPath->setText(Path.c_str());
}

void
HEMAX_SessionWidget::SlotSessionsStartButtonClicked()
{
    HEMAX_SessionType SelectedSessionType;

    if (SessionsInProcessRadioButton->isChecked()) SelectedSessionType = HEMAX_IN_PROCESS;
    else if (SessionsPipeRadioButton->isChecked()) SelectedSessionType = HEMAX_THRIFT_PIPE;
    else if (SessionsOutOfProcessRadioButton->isChecked()) SelectedSessionType = HEMAX_THRIFT_PIPE;
    else SelectedSessionType = HEMAX_THRIFT_SOCKET;

    HEMAX_SessionManager& Manager = HEMAX_SessionManager::GetSessionManager();

    if (Manager.GetSessionType() != SelectedSessionType)
    {
        Manager.SetSessionType(SelectedSessionType);
    }

    switch (SelectedSessionType)
    {
    case (HEMAX_THRIFT_PIPE):
    {
        if (SessionsPipeRadioButton->isChecked())
        {
            Manager.SetThriftNamedPipeSessionName(SessionsPipeName->text().toStdString());
        }
        else
        {
            Manager.StartThriftNamedPipeThinClient();
        }
        break;
    }
    case (HEMAX_THRIFT_SOCKET):
    {
        Manager.SetThriftSocketHostName(SessionsSocketHostName->text().toStdString());
        Manager.SetThriftSocketPortNumber(SessionsSocketPortNumber->text().toInt());
        break;
    }
    default:
    {
        break;
    }
    }

    const char* HoudiniEnvArg = SessionsHoudiniEnvFiles->text().toStdString().empty() ? nullptr : SessionsHoudiniEnvFiles->text().toStdString().c_str();
    const char* OtlPathArg = SessionsOtlSearchPath->text().toStdString().empty() ? nullptr : SessionsOtlSearchPath->text().toStdString().c_str();
    const char* DsoPathArg = SessionsDsoSearchPath->text().toStdString().empty() ? nullptr : SessionsDsoSearchPath->text().toStdString().c_str();
    const char* ImageDsoPathArg = SessionsImageDsoSearchPath->text().toStdString().empty() ? nullptr : SessionsImageDsoSearchPath->text().toStdString().c_str();
    const char* AudioDsoSearchArg = SessionsAudioDsoSearchPath->text().toStdString().empty() ? nullptr : SessionsAudioDsoSearchPath->text().toStdString().c_str();

    if (Manager.StartSession(HoudiniEnvArg, OtlPathArg, DsoPathArg, ImageDsoPathArg, AudioDsoSearchArg))
    {
        SessionsStartButton->setDisabled(true);
        SessionsStopButton->setDisabled(false);
        SessionsTypeBox->setDisabled(true);
        SessionsConfigurationBox->setDisabled(true);
        
        SessionsSocketHostName->setDisabled(true);
        SessionsHostNameLabel->setDisabled(true);
        SessionsSocketPortNumber->setDisabled(true);
        SessionsPortNumberLabel->setDisabled(true);

        SessionsPipeName->setDisabled(true);
        SessionsPipeNameLabel->setDisabled(true);

        emit Signal_SessionStarted();
    }
}

void
HEMAX_SessionWidget::SlotSessionsStopButtonClicked()
{
    SessionsStartButton->setDisabled(false);
    SessionsStopButton->setDisabled(true);
    SessionsTypeBox->setDisabled(false);
    SessionsConfigurationBox->setDisabled(false);
    
    if (SessionsSocketRadioButton->isChecked())
    {
        SessionsSocketHostName->setDisabled(false);
        SessionsHostNameLabel->setDisabled(false);
        SessionsSocketPortNumber->setDisabled(false);
        SessionsPortNumberLabel->setDisabled(false);
    }

    if (SessionsPipeRadioButton->isChecked())
    {
        SessionsPipeName->setDisabled(false);
        SessionsPipeNameLabel->setDisabled(false);
    }

    emit Signal_SessionStopped();
}

void
HEMAX_SessionWidget::SlotSessionsSocketToggle(bool checked)
{
    if (checked)
    {
        SessionsSocketHostName->setDisabled(false);
        SessionsHostNameLabel->setDisabled(false);
        SessionsSocketPortNumber->setDisabled(false);
        SessionsPortNumberLabel->setDisabled(false);
    }
    else
    {
        SessionsSocketHostName->setDisabled(true);
        SessionsHostNameLabel->setDisabled(true);
        SessionsSocketPortNumber->setDisabled(true);
        SessionsPortNumberLabel->setDisabled(true);
    }
}

void
HEMAX_SessionWidget::SlotSessionsPipeToggle(bool checked)
{
    if (checked)
    {
        SessionsPipeName->setDisabled(false);
        SessionsPipeNameLabel->setDisabled(false);
    }
    else
    {
        SessionsPipeName->setDisabled(true);
        SessionsPipeNameLabel->setDisabled(true);
    }
}

void
HEMAX_SessionWidget::SlotSessionsHoudiniEnvFileBrowse()
{
    SessionsHoudiniEnvFiles->setText(FileBrowseDialog(SessionsHoudiniEnvFiles->text()));
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_ENV_FILES, SessionsHoudiniEnvFiles->text().toStdString());
}

void
HEMAX_SessionWidget::SlotSessionsOtlSearchPathBrowse()
{
    SessionsOtlSearchPath->setText(DirectoryBrowseDialog(SessionsOtlSearchPath->text()));
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_OTL_SEARCH, SessionsOtlSearchPath->text().toStdString());
}

void
HEMAX_SessionWidget::SlotSessionsDsoSearchPathBrowse()
{
    SessionsDsoSearchPath->setText(DirectoryBrowseDialog(SessionsDsoSearchPath->text()));
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_DSO_SEARCH, SessionsDsoSearchPath->text().toStdString());
}

void
HEMAX_SessionWidget::SlotSessionsImageDsoSearchPathBrowse()
{
    SessionsImageDsoSearchPath->setText(DirectoryBrowseDialog(SessionsImageDsoSearchPath->text()));
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_IMAGE_DSO_SEARCH, SessionsImageDsoSearchPath->text().toStdString());
}

void
HEMAX_SessionWidget::SlotSessionsAudioDsoSearchPathBrowse()
{
    SessionsAudioDsoSearchPath->setText(DirectoryBrowseDialog(SessionsAudioDsoSearchPath->text()));
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_AUDIO_DSO_SEARCH, SessionsAudioDsoSearchPath->text().toStdString());
}

void
HEMAX_SessionWidget::SlotSessionsHoudiniEnvFileEdited()
{
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_ENV_FILES, SessionsHoudiniEnvFiles->text().toStdString());
}

void
HEMAX_SessionWidget::SlotSessionsOtlSearchPathEdited()
{
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_OTL_SEARCH, SessionsOtlSearchPath->text().toStdString());
}

void
HEMAX_SessionWidget::SlotSessionsDsoSearchPathEdited()
{
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_DSO_SEARCH, SessionsDsoSearchPath->text().toStdString());
}

void
HEMAX_SessionWidget::SlotSessionsImageSearchPathEdited()
{
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_IMAGE_DSO_SEARCH, SessionsImageDsoSearchPath->text().toStdString());
}

void
HEMAX_SessionWidget::SlotSessionsAudioSearchPathEdited()
{
    emit Signal_SessionConfigPathChanged(HEMAX_SETTING_SESSION_AUDIO_DSO_SEARCH, SessionsAudioDsoSearchPath->text().toStdString());
}

QString
HEMAX_SessionWidget::DirectoryBrowseDialog(QString CurrentValue)
{
    QFileDialog DirectoryDialog;
    DirectoryDialog.setFileMode(QFileDialog::Directory);
    DirectoryDialog.setOption(QFileDialog::ShowDirsOnly);

    if (DirectoryDialog.exec())
    {
        return DirectoryDialog.selectedFiles()[0];
    }
    else
    {
        return CurrentValue;
    }
}

QString
HEMAX_SessionWidget::FileBrowseDialog(QString CurrentValue)
{
    QFileDialog FileDialog;
    FileDialog.setFileMode(QFileDialog::ExistingFile);
    FileDialog.setNameFilter("env (*.env);; All files (*.*)");
    
    if (FileDialog.exec())
    {
        return FileDialog.selectedFiles()[0];
    }
    else
    {
        return CurrentValue;
    }
}