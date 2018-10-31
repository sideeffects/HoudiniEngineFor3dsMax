#include "HEMAX_ShelfTab.h"

#include "moc_HEMAX_ShelfTab.cpp"

#include "HEMAX_AssetSelection.h"
#include "../HEMAX_ShelfTool.h"
#include "../HEMAX_ShelfDirectory.h"

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qinputdialog.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qfiledialog.h>
#include <QtGui/qinputdialog.h>
#endif

HEMAX_ShelfTab::HEMAX_ShelfTab(bool Active)
{
    Shelf = nullptr;

    MainLayout = new QGridLayout;
    ConfigurationButton = new QPushButton("Open Shelf Configuration");
    AddShelfDirButton = new QPushButton("Add Shelf Directory");
    RemoveShelfDirButton = new QPushButton("Remove Shelf Directory");
    ReloadShelfDirButton = new QPushButton("Reload Current Shelf");

    ShelfSelectionWidget = new QWidget;
    ShelfSelectionWidgetLayout = new QHBoxLayout;
    ShelfLabel = new QLabel("Active Shelf:");
    ShelfComboBox = new QComboBox;
    ShelfComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ShelfComboBox->setMinimumWidth(HEMAX_ShelfTab_ComboBox_MinWidth);

    ShelfSelectionWidgetLayout->addWidget(ShelfLabel);
    ShelfSelectionWidgetLayout->addWidget(ShelfComboBox);
    ShelfSelectionWidget->setLayout(ShelfSelectionWidgetLayout);

    ShelfWidgetContainer = new QWidget;
    ShelfWidgetLayout = new QGridLayout;

    CreateObjectButton = new QPushButton("Create Object");
    CreateObjectButton->setMinimumHeight(HEMAX_ShelfTab_CreateHdaButtonMinWidth);
    CreateModifiersButton = new QPushButton("Create Modifier HDA On Selected Objects");
    CreateModifiersButton->setMinimumHeight(HEMAX_ShelfTab_CreateHdaButtonMinWidth);

    WarningWidget = new QWidget;
    WarningLayout = new QVBoxLayout;
    WarningLayout->setAlignment(Qt::AlignCenter);
    NoActiveSessionWarning = new QLabel("<b>No active Houdini session</b>");
    WarningLayout->addWidget(NoActiveSessionWarning);
    WarningWidget->setLayout(WarningLayout);

    MainLayout->setAlignment(Qt::AlignTop);
    MainLayout->addWidget(WarningWidget, 0, 0, 1, 3, Qt::AlignCenter);
    MainLayout->addWidget(ConfigurationButton, 1, 2);
    MainLayout->addWidget(AddShelfDirButton, 2, 0);
    MainLayout->addWidget(RemoveShelfDirButton, 2, 1);
    MainLayout->addWidget(ReloadShelfDirButton, 2, 2);
    MainLayout->addWidget(ShelfSelectionWidget, 3, 0, 1, 3, Qt::AlignLeft);
    MainLayout->addWidget(ShelfWidgetContainer, 4, 0, 1, 3, Qt::AlignCenter);
    MainLayout->addWidget(CreateObjectButton, 5, 0, 1, 3);
    MainLayout->addWidget(CreateModifiersButton, 6, 0, 1, 3);

    ShelfWidgetContainer->setLayout(ShelfWidgetLayout);

    this->setLayout(MainLayout);

    ShelfSelectionWidget->setVisible(false);

    HideConfiguration();

    QObject::connect(ConfigurationButton, SIGNAL(clicked()), this, SLOT(Slot_ConfigurationButton_Clicked()));
    QObject::connect(AddShelfDirButton, SIGNAL(clicked()), this, SLOT(Slot_AddShelfDirButton_Clicked()));
    QObject::connect(RemoveShelfDirButton, SIGNAL(clicked()), this, SLOT(Slot_RemoveShelfDirButton_Clicked()));
    QObject::connect(ReloadShelfDirButton, SIGNAL(clicked()), this, SLOT(Slot_ReloadShelfDirButton_Clicked()));

    QObject::connect(CreateObjectButton, SIGNAL(clicked()), this, SLOT(Slot_CreateObjectButton_Clicked()));
    QObject::connect(CreateModifiersButton, SIGNAL(clicked()), this, SLOT(Slot_CreateModifiersButton_Clicked()));

    QObject::connect(ShelfComboBox, SIGNAL(activated(int)), this, SLOT(Slot_ShelfComboBox_Activated(int)));

    if (Active)
    {
        EnableShelf();
    }
    else
    {
        DisableShelf();
    }
}

HEMAX_ShelfTab::HEMAX_ShelfTab(HEMAX_Shelf* const ToolShelf, bool Active)
    : HEMAX_ShelfTab(Active)
{
    Shelf = ToolShelf;
}

HEMAX_ShelfTab::~HEMAX_ShelfTab()
{
    DeleteShelfWidgets();

    delete NoActiveSessionWarning;
    delete WarningLayout;
    delete WarningWidget;

    delete ShelfWidgetLayout;
    delete ShelfWidgetContainer;

    delete ShelfComboBox;
    delete ShelfLabel;
    delete ShelfSelectionWidgetLayout;
    delete ShelfSelectionWidget;

    delete ReloadShelfDirButton;
    delete RemoveShelfDirButton;
    delete AddShelfDirButton;
    delete ConfigurationButton;
    delete MainLayout;
}

void
HEMAX_ShelfTab::SetShelf(HEMAX_Shelf* const ToolShelf)
{
    Shelf = ToolShelf;
}

void
HEMAX_ShelfTab::Update()
{
    ShelfSelectionWidget->setVisible(false);
    ShelfComboBox->clear();
    DeleteShelfWidgets();

    std::vector<std::string> ShelfDirs = Shelf->GetShelfDirectories();

    if (ShelfDirs.size() >= 1)
    {
        ShelfSelectionWidget->setVisible(true);
    }

    for (int i = 0; i < ShelfDirs.size(); i++)
    {
        HEMAX_ShelfDirectory ShelfDir = Shelf->GetShelfDirectory(ShelfDirs[i]);
        ShelfComboBox->addItem(ShelfDir.GetName().c_str());
        ShelfComboBox->setItemData(i, ShelfDir.GetDirectory().c_str());

        if (ActiveShelf.empty())
        {
            ActiveShelf = ShelfDirs[i];
        }

        if (ShelfDirs[i] == ActiveShelf)
        {
            ShelfComboBox->setCurrentIndex(i);

            HEMAX_ShelfGroup* ShelfGroup = new HEMAX_ShelfGroup;
            std::vector<HEMAX_ShelfTool>* Tools = Shelf->GetShelf(ShelfDirs[i]);
            for (int t = 0; t < Tools->size(); t++)
            {
                ShelfGroup->AddShelfTool((*Tools)[t].Name, (*Tools)[t].IconPath, (*Tools)[t].Description, (*Tools)[t].HelpUrl);
            }
            ShelfWidgets.insert({ ShelfDirs[i], ShelfGroup });  
            ShelfWidgetLayout->addWidget(ShelfGroup, ShelfWidgetLayout->rowCount(), Qt::AlignCenter);
        }
    }
}

void
HEMAX_ShelfTab::DisableShelf()
{
    WarningWidget->setHidden(false);
    ShelfWidgetContainer->setDisabled(true);
    CreateObjectButton->setDisabled(true);
    CreateModifiersButton->setDisabled(true);
}

void
HEMAX_ShelfTab::EnableShelf()
{
    WarningWidget->setHidden(true);
    ShelfWidgetContainer->setEnabled(true);
    CreateObjectButton->setEnabled(true);
    CreateModifiersButton->setEnabled(true);
}

void
HEMAX_ShelfTab::DeleteShelfWidgets()
{
    for (auto It = ShelfWidgets.begin(); It != ShelfWidgets.end(); It++)
    {
        delete It->second;
    }

    ShelfWidgets.clear();
}

void
HEMAX_ShelfTab::HideConfiguration()
{
    ConfigurationButton->setText("Open Shelf Configuration");
    AddShelfDirButton->setVisible(false);
    RemoveShelfDirButton->setVisible(false);
    ReloadShelfDirButton->setVisible(false);
}

void
HEMAX_ShelfTab::OpenConfiguration()
{
    ConfigurationButton->setText("Close Shelf Configuration");
    AddShelfDirButton->setVisible(true);
    RemoveShelfDirButton->setVisible(true);
    ReloadShelfDirButton->setVisible(true);
}

void
HEMAX_ShelfTab::Slot_ConfigurationButton_Clicked()
{
    if (ConfigurationButton->text() == "Open Shelf Configuration")
    {
        OpenConfiguration();
    }
    else
    {
        HideConfiguration();
    }
}

void
HEMAX_ShelfTab::Slot_AddShelfDirButton_Clicked()
{
    if (Shelf)
    {
        QFileDialog DirDialog;
        DirDialog.setFileMode(QFileDialog::Directory);
        DirDialog.setOption(QFileDialog::ShowDirsOnly);
        DirDialog.setWindowFlags(Qt::WindowStaysOnTopHint);

        if (DirDialog.exec())
        {
            QInputDialog NameDialog;
            NameDialog.setWindowTitle("Add New Shelf Directory");
            NameDialog.setLabelText("Enter a name for the shelf:");
            NameDialog.resize(QSize(HEMAX_ShelfTab_NameDialog_Width, HEMAX_ShelfTab_NameDialog_Height));
            NameDialog.setWindowFlags(Qt::WindowStaysOnTopHint);

            if (NameDialog.exec())
            {
                if (!NameDialog.textValue().isEmpty())
                {
                    QString Folder = DirDialog.selectedFiles()[0];
                    Shelf->AddShelfDirectory(Folder.toStdString(), NameDialog.textValue().toStdString());

                    ActiveShelf = Folder.toStdString();

                    Update();

                    emit Signal_ShelfUpdated();
                }
            }
        }
    }
}

void
HEMAX_ShelfTab::Slot_RemoveShelfDirButton_Clicked()
{
    if (Shelf)
    {
        HEMAX_AssetSelection ShelfSelection(Shelf->GetShelfDirectories(), "Remove Shelf", "Currently active shelves:");

        if (ShelfSelection.exec())
        {
            std::string SelectedShelf = ShelfSelection.GetSelectedAssetName();

            if (SelectedShelf == ActiveShelf)
            {
                ActiveShelf = "";
            }

            Shelf->RemoveShelfDirectory(SelectedShelf);

            Update();
        }
    }
}

void
HEMAX_ShelfTab::Slot_ReloadShelfDirButton_Clicked()
{
    if (Shelf)
    {
        std::string ReloadShelf = ActiveShelf;
        std::string ShelfName = Shelf->GetShelfDirectory(ReloadShelf).GetName();

        Shelf->RemoveShelfDirectory(ReloadShelf);
        Shelf->AddShelfDirectory(ReloadShelf, ShelfName);

        Update();
    }
}

void
HEMAX_ShelfTab::Slot_CreateObjectButton_Clicked()
{
    auto Search = ShelfWidgets.find(ActiveShelf);
    if (Search != ShelfWidgets.end())
    {
        int SelectedButton = Search->second->GetSelectedButtonId();
        if (SelectedButton > -1)
        {
            std::vector<HEMAX_ShelfTool>* ShelfTools = Shelf->GetShelf(ActiveShelf);
            if (ShelfTools)
            {
                emit Signal_ShelfTool_CreateObject((*ShelfTools)[SelectedButton].AssetPath);
            }
        }
    }
}

void
HEMAX_ShelfTab::Slot_CreateModifiersButton_Clicked()
{
    auto Search = ShelfWidgets.find(ActiveShelf);
    if (Search != ShelfWidgets.end())
    {
        int SelectedButton = Search->second->GetSelectedButtonId();
        if (SelectedButton > -1)
        {
            std::vector<HEMAX_ShelfTool>* ShelfTools = Shelf->GetShelf(ActiveShelf);
            if (ShelfTools)
            {
                emit Signal_ShelfTool_CreateModifiers((*ShelfTools)[SelectedButton].AssetPath);
            }
        }
    }
}

void
HEMAX_ShelfTab::Slot_ShelfComboBox_Activated(int Index)
{
    ActiveShelf = ShelfComboBox->itemData(Index).toString().toStdString();
    Update();
}