#pragma once

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#include <QtGui/qlayout.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#endif

#define HEMAX_SESSION_PIPE_NAME_DEFAULT "hapi"
#define HEMAX_SESSION_HOST_NAME_DEFAULT "localhost"
#define HEMAX_SESSION_PORT_NUM_DEFAULT  "9090"

class HEMAX_SessionWidget : public QWidget
{
    friend class HEMAX_UI;

public:
    HEMAX_SessionWidget();
    ~HEMAX_SessionWidget();

    void PushHoudiniEnvFilePath(std::string Path);
    void PushOtlSearchPath(std::string Path);
    void PushDsoSearchPath(std::string Path);
    void PushImageDsoSearchPath(std::string Path);
    void PushAudioDsoSearchPath(std::string Path);

private:

    Q_OBJECT

    QVBoxLayout* MainLayout;

    QWidget* SessionsBox;
    QVBoxLayout* SessionsBoxLayout;

    QHBoxLayout* SessionsControlLayout;
    QVBoxLayout* SessionsTypeLayout;
    QHBoxLayout* SessionsPipeLayout;
    QHBoxLayout* SessionsSocketLayout;

    QPushButton* SessionsStartButton;
    QPushButton* SessionsStopButton;

    QGroupBox* SessionsTypeBox;
    QLabel* SessionsAutoStartLabel;
    QLabel* SessionsManualStartLabel;
    QHBoxLayout* AutoStartOptionsLayout;
    QHBoxLayout* ManualStartOptionsLayout;
    QRadioButton* SessionsOutOfProcessRadioButton;
    QRadioButton* SessionsInProcessRadioButton;
    QRadioButton* SessionsSocketRadioButton;
    QRadioButton* SessionsPipeRadioButton;

    QLabel* SessionsHostNameLabel;
    QLineEdit* SessionsSocketHostName;
    QLabel* SessionsPortNumberLabel;
    QLineEdit* SessionsSocketPortNumber;

    QLabel* SessionsPipeNameLabel;
    QLineEdit* SessionsPipeName;

    QGroupBox* SessionsConfigurationBox;
    QGridLayout* SessionsConfigurationBoxLayout;
    
    QLabel* SessionsHoudiniEnvFilesLabel;
    QLineEdit* SessionsHoudiniEnvFiles;
    QPushButton* SessionsHoudiniEnvFilesBrowse;
    QLabel* SessionsOtlSearchPathLabel;
    QLineEdit* SessionsOtlSearchPath;
    QPushButton* SessionsOtlSearchPathBrowse;
    QLabel* SessionsDsoSearchPathLabel;
    QLineEdit* SessionsDsoSearchPath;
    QPushButton* SessionsDsoSearchPathBrowse;
    QLabel* SessionsImageDsoSearchPathLabel;
    QLineEdit* SessionsImageDsoSearchPath;
    QPushButton* SessionsImageDsoSearchPathBrowse;
    QLabel* SessionsAudioDsoSearchPathLabel;
    QLineEdit* SessionsAudioDsoSearchPath;
    QPushButton* SessionsAudioDsoSearchPathBrowse;

    QString DirectoryBrowseDialog(QString CurrentValue);
    QString FileBrowseDialog(QString CurrentValue);

 private slots:

    void SlotSessionsStartButtonClicked();
    void SlotSessionsStopButtonClicked();
    void SlotSessionsSocketToggle(bool checked);
    void SlotSessionsPipeToggle(bool checked);

    void SlotSessionsHoudiniEnvFileBrowse();
    void SlotSessionsOtlSearchPathBrowse();
    void SlotSessionsDsoSearchPathBrowse();
    void SlotSessionsImageDsoSearchPathBrowse();
    void SlotSessionsAudioDsoSearchPathBrowse();

    void SlotSessionsHoudiniEnvFileEdited();
    void SlotSessionsOtlSearchPathEdited();
    void SlotSessionsDsoSearchPathEdited();
    void SlotSessionsImageSearchPathEdited();
    void SlotSessionsAudioSearchPathEdited();

signals:

    void Signal_SessionStopped();
    void Signal_SessionStarted();

    void Signal_SessionConfigPathChanged(const char*, std::string);

};
