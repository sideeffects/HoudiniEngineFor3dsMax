#pragma once

#include "../HEMAX_UserPrefs.h"

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qcheckbox.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qlabel.h>
#include <QtGui/qgridlayout.h>
#include <QtGui/qcheckbox.h>
#endif

#include <string>

#define HEMAX_DEBUG_DEFAULT_HIP_NAME "debug_hip_file.hip"

class HEMAX_DebugWidget : public QWidget
{

    Q_OBJECT
    
public:
    HEMAX_DebugWidget();
    ~HEMAX_DebugWidget();

    void SetTempDirectory(std::string Directory);

    void SetUserPrefs(HEMAX_UserPrefs* UserPreferences);

private:
    QVBoxLayout* Layout;

    QGroupBox* HoudiniDebugConfigBox;
    QGridLayout* HoudiniDebugConfigBoxLayout;
    QLabel* TempFilesLabel;
    QLineEdit* TempFilesDirectoryField;
    QPushButton* TempFilesDirectoryBrowseButton;

    QGroupBox* HoudiniDebuggingBox;
    QVBoxLayout* HoudiniDebuggingLayout;
    QPushButton* SaveHoudiniSceneButton;
    QPushButton* OpenInHoudiniButton;

    QGroupBox* LoggingBox;
    QGridLayout* LoggingLayout;
    QLabel* LoggingLevels;
    QCheckBox* ErrorLevelCheckbox;
    QCheckBox* WarnLevelCheckbox;
    QCheckBox* InfoLevelCheckbox;

    HEMAX_UserPrefs* UserPrefs;

private slots:

    void SlotTempDirBrowseButtonClicked();
    void SlotSaveHoudiniSceneButtonClicked();
    void SlotOpenInHoudiniButtonClicked();

    void SlotErrorLevelToggleStateChanged(int State);
    void SlotWarnLevelToggleStateChanged(int State);
    void SlotInfoLevelToggleStateChanged(int State);

signals:

    void SignalTempDirChanged(std::string Directory);
};
