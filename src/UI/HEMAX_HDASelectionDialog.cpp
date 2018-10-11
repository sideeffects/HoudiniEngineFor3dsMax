#include "HEMAX_HDASelectionDialog.h"

#include <QtCore/qfileinfo.h>

#include "moc_HEMAX_HDASelectionDialog.cpp"

#define HEMAX_MODIFIER_SELECTION_DIALOG_WIDTH  500
#define HEMAX_MODIFIER_SELECTION_DIALOG_HEIGHT 300

HEMAX_HDASelectionDialog::HEMAX_HDASelectionDialog(std::vector<std::string> AssetPaths)
{
    this->setWindowTitle("Select a Houdini Digital Asset");
    this->setMinimumWidth(HEMAX_MODIFIER_SELECTION_DIALOG_WIDTH);
    this->setMinimumHeight(HEMAX_MODIFIER_SELECTION_DIALOG_HEIGHT);

    MainLayout = new QVBoxLayout;

    HDASelectionBox = new QGroupBox("Loaded assets:");
    HDASelectionLayout = new QVBoxLayout;
    HDASelectionList = new QListWidget;
    HDASelectionButton = new QPushButton("Select");
    
    MainLayout->setAlignment(Qt::AlignTop);
    HDASelectionBox->setAlignment(Qt::AlignTop);

    HDASelectionLayout->addWidget(HDASelectionList);
    HDASelectionLayout->addWidget(HDASelectionButton);

    HDASelectionBox->setLayout(HDASelectionLayout);

    MainLayout->addWidget(HDASelectionBox);

    this->setLayout(MainLayout);

    for (int i = 0; i < AssetPaths.size(); ++i)
    {
        QFileInfo HDAFile(AssetPaths[i].c_str());

        HDASelectionList->addItem(HDAFile.fileName());
        HDASelectionList->item(i)->setToolTip(AssetPaths[i].c_str());
    }

    HDASelectionList->sortItems();

    QObject::connect(HDASelectionButton, SIGNAL(clicked()), this, SLOT(SlotSelectionButtonClicked()));
    QObject::connect(HDASelectionList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SlotListSelection(QListWidgetItem*)));

    CurrentSelection = "";
}

HEMAX_HDASelectionDialog::~HEMAX_HDASelectionDialog()
{
    delete HDASelectionButton;
    delete HDASelectionList;
    delete HDASelectionLayout;
    delete HDASelectionBox;

    delete MainLayout;
}

void
HEMAX_HDASelectionDialog::SlotSelectionButtonClicked()
{
    if (CurrentSelection == "")
    {
        this->done(0);
    }
    else
    {
        this->done(1);
    }
}

void
HEMAX_HDASelectionDialog::SlotListSelection(QListWidgetItem* Selection)
{
    CurrentSelection = Selection->toolTip();
}

std::string
HEMAX_HDASelectionDialog::GetSelectedAssetPath()
{
    return CurrentSelection.toStdString();
}
