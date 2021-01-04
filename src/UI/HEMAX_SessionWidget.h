#pragma once

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021)
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

#include "../HEMAX_UserPrefs.h"

class HEMAX_Plugin;

class HEMAX_SessionWidget : public QWidget
{
    friend class HEMAX_UI;

    public:
	HEMAX_SessionWidget(HEMAX_Plugin* ActivePlugin);
	~HEMAX_SessionWidget();

	void Update();

    private:

        HEMAX_Plugin* Plugin;

        void InitFieldsFromPrefs();

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
        void SlotSessionsAutoToggle(bool Checked);
	void SlotSessionsSocketToggle(bool Checked);
	void SlotSessionsPipeToggle(bool Checked);

        void SlotSessionsSocketHostName();
        void SlotSessionsSocketPortNumber();
        void SlotSessionsPipeName();

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
};
