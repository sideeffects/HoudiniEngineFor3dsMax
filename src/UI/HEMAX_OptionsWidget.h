#pragma once

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qlabel.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#endif

class HEMAX_OptionsWidget : public QWidget
{
    Q_OBJECT

public:
    HEMAX_OptionsWidget();
    ~HEMAX_OptionsWidget();

    void SetAutoSelectOption(bool Enabled);
    void SetAutoStartSessionOption(bool Enabled);
    void SetAutoStartWindowOption(bool Enabled);
    void SetAutoLoadHDADirectory(std::string Dir);
    void SetHdaRepoDirectory(std::string Dir);
    
private:
    QVBoxLayout* Layout;

    QGroupBox* SelectionOptionsBox;
    QVBoxLayout* SelectionOptionsBoxLayout;
    QCheckBox* AutoSelectHDARoot;

    QGroupBox* StartupOptionsBox;
    QGridLayout* StartupOptionsBoxLayout;
    QCheckBox* AutoStartSessionToggle;
    QCheckBox* AutoStartWindowToggle;
    QLabel* AutoLoadHDADirLabel;
    QLineEdit* AutoLoadHDADir;
    QPushButton* AutoLoadHDADirBrowse;

    QGroupBox* HdaOptionsBox;
    QGridLayout* HdaOptionsBoxLayout;
    QLabel* HdaRepoLabel;
    QLineEdit* HdaRepoDir;
    QPushButton* HdaRepoDirBrowse;

private slots:

    void Slot_SelectHDARoot(int Checked);

    void Slot_AutoStartSession(int Checked);
    void Slot_AutoStartWindow(int Checked);
    void Slot_AutoLoadHDADir();
    void Slot_AutoLoadHDADirBrowse();

    void Slot_HdaRepoDirBrowse();

signals:

    void Signal_SelectHDARootOption(int Checked);
    void Signal_SelectAutoStartSessionOption(int Checked);
    void Signal_SelectAutoStartWindowOption(int Checked);
    void Signal_AutoLoadHDADir_EditingFinished(std::string AutoLoadDir);
    void Signal_HdaRepoDir_EditingFinished(std::string HdaRepoDir);
};
