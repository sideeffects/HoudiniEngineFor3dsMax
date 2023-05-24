#include "HEMAX_ShelfGroup.h"

#include "moc_HEMAX_ShelfGroup.cpp"

#include <QtCore/qvariant.h>
#include <QtGui/qdesktopservices.h>
#include <QtCore/qurl.h>

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024)
#include <QtWidgets/qmenu.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui\qmenu.h>
#endif

HEMAX_ShelfGroup::HEMAX_ShelfGroup()
{
    Layout = new QGridLayout;
    ButtonGroup = new QButtonGroup;
    ButtonWidget = new QWidget;
    ButtonLayout = new QGridLayout;
    ButtonWidget->setLayout(ButtonLayout);
    Layout->addWidget(ButtonWidget);
    this->setLayout(Layout);

    ButtonWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ButtonWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SlotShowAssetContextMenu(QPoint)));
}

HEMAX_ShelfGroup::~HEMAX_ShelfGroup()
{
    for (int i = 0; i < Buttons.size(); i++)
    {
	delete Buttons[i];
    }

    delete ButtonLayout;
    delete ButtonWidget;
    delete ButtonGroup;
    delete Layout;
}

void
HEMAX_ShelfGroup::AddShelfTool(std::string Name, std::string IconPath, std::string ToolTip, std::string HelpUrl)
{
    QToolButton* ShelfButton = new QToolButton;
    ShelfButton->setMinimumWidth(HEMAX_ShelfGroup_ShelfToolButton_Width);
    ShelfButton->setMinimumHeight(HEMAX_ShelfGroup_ShelfToolButton_Height);
    ShelfButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ShelfButton->setCheckable(true);
    ShelfButton->setIcon(QIcon(IconPath.c_str()));
    ShelfButton->setIconSize(QSize(HEMAX_ShelfGroup_ShelfButtonIcon_Dimension, HEMAX_ShelfGroup_ShelfButtonIcon_Dimension));
    ShelfButton->setText(Name.c_str());
    ShelfButton->setProperty(HEMAX_ShelfGroup_HelpProp, HelpUrl.c_str());

    if (!ToolTip.empty())
    {
	ShelfButton->setToolTip(ToolTip.c_str());
    }

    int NumButtons = ButtonLayout->count();

    ButtonGroup->addButton(ShelfButton, NumButtons);
    int Row = NumButtons / HEMAX_ShelfGroup_ShelfToolButtonStripWidth;
    int Col = (NumButtons + HEMAX_ShelfGroup_ShelfToolButtonStripWidth) % HEMAX_ShelfGroup_ShelfToolButtonStripWidth;

    ButtonLayout->addWidget(ShelfButton, Row, Col);
}

int
HEMAX_ShelfGroup::GetSelectedButtonId()
{
    return ButtonGroup->checkedId();
}

void
HEMAX_ShelfGroup::SlotShowAssetContextMenu(QPoint Position)
{
    QPoint MenuSpawn = ButtonWidget->mapToGlobal(Position);
    QWidget* Item = ButtonWidget->childAt(Position);

    if (Item)
    {
	std::string Url = Item->property(HEMAX_ShelfGroup_HelpProp).toString().toStdString();

	QMenu Menu;

	if (Url.empty())
	{
	    Menu.addAction("No Help URL");
	}
	else
	{
	    Menu.addAction("Open Help URL", this, SLOT(SlotOpenHelpUrl()));
	    ContextMenuUrl = Url;
	}

	Menu.exec(MenuSpawn);
    }   
}

void
HEMAX_ShelfGroup::SlotOpenHelpUrl()
{
    QDesktopServices::openUrl(QString(ContextMenuUrl.c_str()));
}
