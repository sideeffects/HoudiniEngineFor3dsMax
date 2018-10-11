#pragma once

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qboxlayout.h>
#endif

#include "HEMAX_ShelfGroup.h"
#include "../HEMAX_Shelf.h"

#include <string>
#include <vector>
#include <unordered_map>

const int HEMAX_ShelfTab_CreateHdaButtonMinWidth = 50;

const int HEMAX_ShelfTab_NameDialog_Width = 600;
const int HEMAX_ShelfTab_NameDialog_Height = 300;

const int HEMAX_ShelfTab_ComboBox_MinWidth = 300;

class HEMAX_ShelfTab : public QWidget
{
    Q_OBJECT

public:
    HEMAX_ShelfTab();
    HEMAX_ShelfTab(HEMAX_Shelf* const ToolShelf);
    ~HEMAX_ShelfTab();

    void SetShelf(HEMAX_Shelf* const ToolShelf);

    void Update();

private:
    HEMAX_Shelf* Shelf;

    QGridLayout* MainLayout;
    QPushButton* ConfigurationButton;
    QPushButton* AddShelfDirButton;
    QPushButton* RemoveShelfDirButton;
    QPushButton* ReloadShelfDirButton;

    QWidget* ShelfSelectionWidget;
    QHBoxLayout* ShelfSelectionWidgetLayout;
    QLabel* ShelfLabel;
    QComboBox* ShelfComboBox;

    QWidget* ShelfWidgetContainer;
    QGridLayout* ShelfWidgetLayout;
    std::unordered_map<std::string, HEMAX_ShelfGroup*> ShelfWidgets;

    QPushButton* CreateObjectButton;
    QPushButton* CreateModifiersButton;

    std::string ActiveShelf;

    void DeleteShelfWidgets();

    void HideConfiguration();
    void OpenConfiguration();

private slots:
    void Slot_ConfigurationButton_Clicked();
    void Slot_AddShelfDirButton_Clicked();
    void Slot_RemoveShelfDirButton_Clicked();
    void Slot_ReloadShelfDirButton_Clicked();

    void Slot_CreateObjectButton_Clicked();
    void Slot_CreateModifiersButton_Clicked();

    void Slot_ShelfComboBox_Activated(int Index);

signals:
    void Signal_ShelfUpdated();

    void Signal_ShelfTool_CreateObject(std::string);
    void Signal_ShelfTool_CreateModifiers(std::string);
};