#include "HEMAX_OptionsWidget.h"

#include "moc_HEMAX_OptionsWidget.cpp"

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qfiledialog.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qfiledialog.h>
#endif

HEMAX_OptionsWidget::HEMAX_OptionsWidget()
{
    Layout = new QVBoxLayout;
    Layout->setAlignment(Qt::AlignTop);

    SelectionOptionsBox = new QGroupBox("Selection");
    SelectionOptionsBoxLayout = new QVBoxLayout;
    SelectionOptionsBoxLayout->setAlignment(Qt::AlignTop);
    AutoSelectHDARoot = new QCheckBox("Automatically select HDA root node when selecting child geometry nodes");

    SelectionOptionsBoxLayout->addWidget(AutoSelectHDARoot);
    SelectionOptionsBox->setLayout(SelectionOptionsBoxLayout);

    StartupOptionsBox = new QGroupBox("Startup");
    StartupOptionsBoxLayout = new QGridLayout;
    StartupOptionsBoxLayout->setAlignment(Qt::AlignTop);
    AutoStartWindowToggle = new QCheckBox("Open the plugin pane when 3ds Max starts");
    AutoLoadHDADirLabel = new QLabel("HDA Load Directory:");
    AutoLoadHDADir = new QLineEdit;
    AutoLoadHDADir->setEnabled(false);
    AutoLoadHDADirBrowse = new QPushButton("...");

    StartupOptionsBoxLayout->addWidget(AutoStartWindowToggle, 0, 0);
    StartupOptionsBoxLayout->addWidget(AutoLoadHDADirLabel, 1, 0);
    StartupOptionsBoxLayout->addWidget(AutoLoadHDADir, 1, 1);
    StartupOptionsBoxLayout->addWidget(AutoLoadHDADirBrowse, 1, 2);
    StartupOptionsBox->setLayout(StartupOptionsBoxLayout);

    HdaOptionsBox = new QGroupBox("Assets");
    HdaOptionsBoxLayout = new QGridLayout;
    HdaOptionsBoxLayout->setAlignment(Qt::AlignTop);
    HdaRepoLabel = new QLabel("HDA Search Path:");
    HdaRepoDir = new QLineEdit;
    HdaRepoDir->setEnabled(false);
    HdaRepoDirBrowse = new QPushButton("...");

    HdaOptionsBoxLayout->addWidget(HdaRepoLabel, 0, 0);
    HdaOptionsBoxLayout->addWidget(HdaRepoDir, 0, 1);
    HdaOptionsBoxLayout->addWidget(HdaRepoDirBrowse, 0, 2);
    HdaOptionsBox->setLayout(HdaOptionsBoxLayout);

    Layout->addWidget(SelectionOptionsBox);
    Layout->addWidget(StartupOptionsBox);
    Layout->addWidget(HdaOptionsBox);

    this->setLayout(Layout);

    QObject::connect(AutoSelectHDARoot, SIGNAL(stateChanged(int)), this, SLOT(Slot_SelectHDARoot(int)));
    QObject::connect(AutoStartWindowToggle, SIGNAL(stateChanged(int)), this, SLOT(Slot_AutoStartWindow(int)));
    QObject::connect(AutoLoadHDADir, SIGNAL(editingFinished()), this, SLOT(Slot_AutoLoadHDADir()));
    QObject::connect(AutoLoadHDADirBrowse, SIGNAL(clicked()), this, SLOT(Slot_AutoLoadHDADirBrowse()));
    QObject::connect(HdaRepoDirBrowse, SIGNAL(clicked()), this, SLOT(Slot_HdaRepoDirBrowse()));
}

HEMAX_OptionsWidget::~HEMAX_OptionsWidget()
{
    delete AutoLoadHDADirBrowse;
    delete AutoLoadHDADir;
    delete AutoLoadHDADirLabel;
    delete AutoStartWindowToggle;
    delete StartupOptionsBoxLayout;
    delete StartupOptionsBox;

    delete AutoSelectHDARoot;
    delete SelectionOptionsBoxLayout;
    delete SelectionOptionsBox;

    delete Layout;
}

void
HEMAX_OptionsWidget::Slot_SelectHDARoot(int Checked)
{
    emit Signal_SelectHDARootOption(Checked);
}

void
HEMAX_OptionsWidget::Slot_AutoStartWindow(int Checked)
{
    emit Signal_SelectAutoStartWindowOption(Checked);
}

void
HEMAX_OptionsWidget::Slot_AutoLoadHDADir()
{
    emit Signal_AutoLoadHDADir_EditingFinished(AutoLoadHDADir->text().toStdString());
}

void
HEMAX_OptionsWidget::SetAutoSelectOption(bool Enabled)
{
    AutoSelectHDARoot->setChecked(Enabled);
}

void
HEMAX_OptionsWidget::SetAutoStartWindowOption(bool Enabled)
{
    AutoStartWindowToggle->setChecked(Enabled);
}

void
HEMAX_OptionsWidget::SetAutoLoadHDADirectory(std::string Dir)
{
    AutoLoadHDADir->setText(Dir.c_str());
}

void
HEMAX_OptionsWidget::SetHdaRepoDirectory(std::string Dir)
{
    HdaRepoDir->setText(Dir.c_str());
}

void
HEMAX_OptionsWidget::Slot_AutoLoadHDADirBrowse()
{
    QString SelectedDir = QFileDialog::getExistingDirectory();
    if (!SelectedDir.isEmpty())
    {
        AutoLoadHDADir->setText(SelectedDir);
        emit Signal_AutoLoadHDADir_EditingFinished(SelectedDir.toStdString());
    }
}

void
HEMAX_OptionsWidget::Slot_HdaRepoDirBrowse()
{
    QString SelectedDir = QFileDialog::getExistingDirectory();
    if (!SelectedDir.isEmpty())
    {
        HdaRepoDir->setText(SelectedDir);
        emit Signal_HdaRepoDir_EditingFinished(SelectedDir.toStdString());
    }
}
