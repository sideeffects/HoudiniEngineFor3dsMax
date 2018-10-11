#pragma once

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qbuttongroup.h>
#include <QtWidgets/qtoolbutton.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#include <QtGui/qgridlayout.h>
#include <QtGui/qbuttongroup.h>
#include <QtGui/qtoolbutton.h>
#endif

#include <vector>
#include <string>

const int HEMAX_ShelfGroup_ShelfToolButton_Width = 100;
const int HEMAX_ShelfGroup_ShelfToolButton_Height = 60;
const int HEMAX_ShelfGroup_ShelfButtonIcon_Dimension = 60;
const int HEMAX_ShelfGroup_ShelfToolButtonStripWidth = 5;

const char* const HEMAX_ShelfGroup_HelpProp = "HEMAX_HelpUrl";

class HEMAX_ShelfGroup : public QWidget
{
    Q_OBJECT

public:
    HEMAX_ShelfGroup();
    ~HEMAX_ShelfGroup();

    void AddShelfTool(std::string Name, std::string IconPath, std::string ToolTip, std::string HelpUrl);

    int GetSelectedButtonId();

private:
    QGridLayout * Layout;
    QButtonGroup* ButtonGroup;
    QWidget* ButtonWidget;
    QGridLayout* ButtonLayout;
    std::vector<QToolButton*> Buttons;

    std::string ContextMenuUrl;

private slots:
    void SlotShowAssetContextMenu(QPoint);
    void SlotOpenHelpUrl();
};