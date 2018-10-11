#include "HEMAX_AssetSelection.h"

#include "moc_HEMAX_AssetSelection.cpp"

HEMAX_AssetSelection::HEMAX_AssetSelection(std::vector<std::string> AssetNames)
    : HEMAX_AssetSelection(AssetNames, "Asset Selection", "Assets found in HDA:")
{
}

HEMAX_AssetSelection::HEMAX_AssetSelection(std::vector<std::string> AssetNames, std::string DialogTitle, std::string DialogSubtitle)
{
    this->resize(HEMAX_AssetSelection_DefaultWidth, HEMAX_AssetSelection_DefaultHeight);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowTitle(DialogTitle.c_str());

    MainLayout = new QVBoxLayout;

    AssetSelectionBox = new QGroupBox(DialogSubtitle.c_str());
    AssetSelectionLayout = new QVBoxLayout;
    AssetSelectionList = new QListWidget;
    AssetSelectionButton = new QPushButton("Select");

    MainLayout->setAlignment(Qt::AlignTop);
    AssetSelectionBox->setAlignment(Qt::AlignTop);

    AssetSelectionLayout->addWidget(AssetSelectionList);
    AssetSelectionLayout->addWidget(AssetSelectionButton);

    AssetSelectionBox->setLayout(AssetSelectionLayout);

    MainLayout->addWidget(AssetSelectionBox);

    this->setLayout(MainLayout);

    for (int i = 0; i < AssetNames.size(); ++i)
    {
        AssetSelectionList->addItem(AssetNames[i].c_str());
    }

    AssetSelectionList->sortItems();

    QObject::connect(AssetSelectionButton, SIGNAL(clicked()), this, SLOT(SlotSelectionButtonClicked()));
    QObject::connect(AssetSelectionList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SlotListSelection(QListWidgetItem*)));

    CurrentSelection = "";
}

HEMAX_AssetSelection::~HEMAX_AssetSelection()
{
    delete AssetSelectionButton;
    delete AssetSelectionList;
    delete AssetSelectionLayout;
    delete AssetSelectionBox;

    delete MainLayout;
}

void
HEMAX_AssetSelection::SlotSelectionButtonClicked()
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
HEMAX_AssetSelection::SlotListSelection(QListWidgetItem* Selection)
{
    CurrentSelection = Selection->text();
}

std::string
HEMAX_AssetSelection::GetSelectedAssetName()
{
    return CurrentSelection.toStdString();
}
