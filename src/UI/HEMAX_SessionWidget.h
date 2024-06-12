#pragma once

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025)
#include <QtWidgets/qwidget.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#endif

class HEMAX_Plugin;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QGroupBox;
class QLabel;
class QRadioButton;
class QGridLayout;
class QComboBox;
class QLineEdit;
class QIntValidator;

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
        QHBoxLayout* SessionsSharedMemoryLayout;

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
        QRadioButton* SessionsSharedMemoryRadioButton;

	QLabel* SessionsHostNameLabel;
	QLineEdit* SessionsSocketHostName;
	QLabel* SessionsPortNumberLabel;
	QLineEdit* SessionsSocketPortNumber;

	QLabel* SessionsPipeNameLabel;
	QLineEdit* SessionsPipeName;

        QLabel* SessionsSharedMemoryLabel;
        QLineEdit* SessionsSharedMemoryName;
        QLabel* SessionsSharedMemoryBufferSizeLabel;
        QIntValidator* SessionsSharedMemoryBufferSizeValidator;
        QLineEdit* SessionsSharedMemoryBufferSize;
        QLabel* SessionsSharedMemoryBufferTypeLabel;
        QComboBox* SessionsSharedMemoryBufferType;

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
        void SlotSessionsSharedMemoryToggle(bool Checked);

        void SlotSessionsSocketHostName();
        void SlotSessionsSocketPortNumber();
        void SlotSessionsPipeName();
        void SlotSessionsSharedMemoryName();
        void SlotSessionsSharedMemoryBufferSize();
        void SlotSessionsSharedMemoryBufferType(int);

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
