#pragma once

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
#include <QtGui/qvalidator.h>
#include <QtWidgets/qdialog.h>
#endif

#ifdef HEMAX_VERSION_2017
#pragma warning(push, 0)
#include <QtGui/qdialog.h>
#include <QtGui/qvalidator.h>
#pragma warning(pop)
#endif

class HEMAX_Plugin;

class QCheckBox;
class QComboBox;
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
        HEMAX_OptionsDialog(HEMAX_Plugin* ThePlugin);
        ~HEMAX_OptionsDialog() = default;

        void Update();

    private:

        void InitializeOptions();

        // Validators

        QIntValidator MyIntValidator;

        QVBoxLayout* Layout;
        QTabWidget* OptionsTabs;

        // General Settings

        QWidget* GeneralOptions;
        QVBoxLayout* GeneralOptionsLayout;

        QGroupBox* InstallationOptions;
        QGridLayout* InstallationOptionsLayout;
        QLabel* OverrideHoudiniInstallPathLabel;
        QLineEdit* OverrideHoudiniInstallPath;
        QPushButton* OverrideHoudiniInstallPathDirBrowse;
        QLabel* RestartApplicationInstruction;

        QGroupBox* SelectionOptions;
        QVBoxLayout* SelectionOptionsLayout;
        QCheckBox* AutoSelectHDARoot;

        QGroupBox* OnStartupOptions;
        QVBoxLayout* OnStartupOptionsLayout;
        QCheckBox* AutoOpenWindow;

        QGroupBox* AssetOptions;
        QGridLayout* AssetOptionsLayout;

        QLabel* HdaLoadDirLabel;
        QLineEdit* HdaLoadDir;
        QPushButton* HdaLoadDirBrowse;

        QLabel* HdaSearchPathLabel;
        QLineEdit* HdaSearchPath;
        QPushButton* HdaSearchPathBrowse;

        // Session Configuration
        
        QWidget*        SessionOptions                      = nullptr;
        QGridLayout*    SessionOptionsLayout                = nullptr;

        QCheckBox*      SessionAutoStart                    = nullptr;
        QLabel*         SessionTypeLabel                    = nullptr;
        QComboBox*      SessionTypeChoice                   = nullptr;

        QLabel*         SessionTimeoutLabel                 = nullptr;
        QLineEdit*      SessionTimeout                      = nullptr;

        QLabel*         SessionHostnameLabel                = nullptr;
        QLineEdit*      SessionHostname                     = nullptr;
        QLabel*         SessionPortLabel                    = nullptr;
        QLineEdit*      SessionPort                         = nullptr;
        QLabel*         SessionPipeNameLabel                = nullptr;
        QLineEdit*      SessionPipeName                     = nullptr;
        QLabel*         SessionSharedMemoryNameLabel        = nullptr;
        QLineEdit*      SessionSharedMemoryName             = nullptr;
        QLabel*         SessionSharedMemoryBufferSizeLabel  = nullptr;
        QLineEdit*      SessionSharedMemoryBufferSize       = nullptr;
        QLabel*         SessionSharedMemoryBufferTypeLabel  = nullptr;
        QComboBox*      SessionSharedMemoryBufferType       = nullptr;

        QLabel*         SessionHoudiniEnvFilesLabel         = nullptr;
        QLineEdit*      SessionHoudiniEnvFiles              = nullptr;
        QLabel*         SessionOtlSearchPathLabel           = nullptr;
        QLineEdit*      SessionOtlSearchPath                = nullptr;
        QLabel*         SessionDsoSearchPathLabel           = nullptr;
        QLineEdit*      SessionDsoSearchPath                = nullptr;
        QLabel*         SessionImageDsoSearchPathLabel      = nullptr;
        QLineEdit*      SessionImageDsoSearchPath           = nullptr;
        QLabel*         SessionAudioDsoSearchPathLabel      = nullptr;
        QLineEdit*      SessionAudioDsoSearchPath           = nullptr;

        // Defaults

        QWidget* DefaultOptions;
        QVBoxLayout* DefaultOptionsLayout;

        QGroupBox* NodeOptionDefaults;
        QVBoxLayout* NodeOptionDefaultsLayout;
        QCheckBox* NodeOptionDefaultsAutoRecook;
        QCheckBox* NodeOptionDefaultsSliderCook;

        // Geometry HDA Settings

        QWidget* GeometryHdaOptions;
        QVBoxLayout* GeometryHdaOptionsLayout;

        QGroupBox* GeoHdaBakingOptions;
        QVBoxLayout* GeoHdaBakingOptionsLayout;
        QCheckBox* BakeDummyObject;
        QCheckBox* BakeCreateLayer;

        QGroupBox* NodeOptions;
        QVBoxLayout* NodeOptionsLayout;
        QCheckBox* UseUniqueNames;
        QCheckBox* UseOriginalInstanceName;

        // Debug Options

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

        HEMAX_Plugin* Plugin;

    private slots:

        void SlotOverrideHoudiniInstallPath();
        void SlotOverrideHoudiniInstallPathDirBrowse();
        void SlotAutoSelectHDARoot(int State);
        void SlotAutoOpenWindow(int State);
        void SlotHdaLoadDir();
        void SlotHdaLoadDirBrowse();
        void SlotHdaSearchPath();
        void SlotHdaSearchPathBrowse();

        void SlotSessionAutoStart(int State);
        void SlotSessionDefaultStartType(int CurrentIndex);
        void SlotSessionTimeout();
        void SlotSessionHostname();
        void SlotSessionPort();
        void SlotSessionPipeName();
        void SlotSessionSharedMemoryName();
        void SlotSessionSharedMemoryBufferSize();
        void SlotSessionSharedMemoryBufferType(int CurrentIndex);
        void SlotSessionHoudiniEnvFiles();
        void SlotSessionOtlSearchPath();
        void SlotSessionDsoSearchPath();
        void SlotSessionImageDsoSearchPath();
        void SlotSessionAudioDsoSearchPath();

        void SlotNodeOptionDefaultsAutoRecook(int State);
        void SlotNodeOptionDefaultsSliderCook(int State);
        void SlotBakeDummyObject(int State);
        void SlotBakeCreateLayer(int State);
        void SlotUseUniqueNames(int State);
        void SlotUseOriginalInstanceName(int State);
        void SlotTempFilesFolder();
        void SlotTempFilesFolderBrowse();
        void SlotSaveHipButton();
        void SlotOpenHoudiniButton();
        void SlotErrorLogging(int State);
        void SlotWarningLogging(int State);
        void SlotInfoLogging(int State);
};
