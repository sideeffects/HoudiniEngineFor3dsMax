#pragma once

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021)
#include <QtWidgets/qdialog.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qdialog.h>
#endif

class HEMAX_Plugin;
class HEMAX_UserPrefs;

class QCheckBox;
class QVBoxLayout;
class QGridLayout;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTabWidget;

class HEMAX_OptionsDialog : public QDialog
{
    Q_OBJECT

    public:
        HEMAX_OptionsDialog(HEMAX_UserPrefs* UserPrefs,
                            HEMAX_Plugin* ThePlugin);
        ~HEMAX_OptionsDialog();

    private:

        void InitializeOptions();

        QVBoxLayout* Layout;
        QTabWidget* OptionsTabs;

        QWidget* GeneralOptions;
        QVBoxLayout* GeneralOptionsLayout;

        QGroupBox* SelectionOptions;
        QVBoxLayout* SelectionOptionsLayout;
        QCheckBox* AutoSelectHDARoot;

        QGroupBox* OnStartupOptions;
        QVBoxLayout* OnStartupOptionsLayout;
        QCheckBox* AutoStartSession;
        QCheckBox* AutoOpenWindow;

        QGroupBox* AssetOptions;
        QGridLayout* AssetOptionsLayout;

        QLabel* HdaLoadDirLabel;
        QLineEdit* HdaLoadDir;
        QPushButton* HdaLoadDirBrowse;

        QLabel* HdaSearchPathLabel;
        QLineEdit* HdaSearchPath;
        QPushButton* HdaSearchPathBrowse;

        QWidget* GeometryHdaOptions;
        QVBoxLayout* GeometryHdaOptionsLayout;

        QGroupBox* GeoHdaBakingOptions;
        QVBoxLayout* GeoHdaBakingOptionsLayout;
        QCheckBox* BakeDummyObject;

        QGroupBox* NodeNamingOptions;
        QVBoxLayout* NodeNamingOptionsLayout;
        QCheckBox* UseUniqueNames;

        QWidget* DebugOptions;
        QVBoxLayout* DebugOptionsLayout;

        QGroupBox* LocationOptions;
        QGridLayout* LocationOptionsLayout;
        
        QLabel* TempFilesFolderLabel;
        QLineEdit* TempFilesFolder;
        QPushButton* TempFilesFolderBrowse;

        QGroupBox* HoudiniOptions;
        QVBoxLayout* HoudiniOptionsLayout;
    
        QPushButton* SaveHipButton;
        QPushButton* OpenHoudiniButton; 

        QGroupBox* LoggingOptions;
        QVBoxLayout* LoggingOptionsLayout;

        QLabel* LoggingLevelsLabel;
        QCheckBox* ErrorLogging;
        QCheckBox* WarningLogging;
        QCheckBox* InfoLogging;

        HEMAX_UserPrefs* Prefs;
        HEMAX_Plugin* Plugin;

    private slots:

        void SlotAutoSelectHDARoot(int State);
        void SlotAutoStartSession(int State);
        void SlotAutoOpenWindow(int State);
        void SlotHdaLoadDir();
        void SlotHdaLoadDirBrowse();
        void SlotHdaSearchPath();
        void SlotHdaSearchPathBrowse();
        void SlotBakeDummyObject(int State);
        void SlotUseUniqueNames(int State);
        void SlotTempFilesFolder();
        void SlotTempFilesFolderBrowse();
        void SlotSaveHipButton();
        void SlotOpenHoudiniButton();
        void SlotErrorLogging(int State);
        void SlotWarningLogging(int State);
        void SlotInfoLogging(int State);
};
