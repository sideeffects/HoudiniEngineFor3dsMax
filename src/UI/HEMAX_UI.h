#pragma once

#include "../HEMAX_Plugin.h"
#include "HEMAX_SessionWidget.h"
#include "HEMAX_HDAWidget.h"
#include "HEMAX_MaxHoudiniAssetWidget.h"
#include "HEMAX_ShelfTab.h"

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
#include <Qt/QmaxMainWindow.h>
#include <Qt/QmaxDockWidget.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qtabwidget.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qscrollarea.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qmainwindow.h>
#endif

#define HEMAX_2017_WINDOW_START_HEIGHT 600
#define HEMAX_2017_WINDOW_START_WIDTH 600

#pragma warning(push, 0)
#include <maxapi.h>
#pragma warning(pop)

class HEMAX_Plugin;
class HEMAX_3dsmaxHda;

class HEMAX_UI : public QDockWidget
{

    Q_OBJECT

    public:
	HEMAX_UI(QMainWindow* MainWindow, HEMAX_Plugin* Plugin);
	~HEMAX_UI();

	void ChangeHdaSelection(HEMAX_3dsmaxHda* Hda, bool ForceUnlock = false);
	HEMAX_3dsmaxHda* GetCurrentHdaSelection();
        void SetSelectionLocked(bool Locked);
	void ShowHEMAXWindow();
	void UnshowHEMAXWindow();
	void UpdateLoadedAssetLibrariesList();
        void HandleHdaPreDeleteEvent(HEMAX_3dsmaxHda* Hda);
	void Update();

    private:

	QScrollArea* ScrollArea;
	QTabWidget* TabContainer;

	HEMAX_ShelfTab* ShelfToolsWidget;
	HEMAX_SessionWidget* SessionWidget;
	HEMAX_HDAWidget* HDAWidget;
	HEMAX_MaxHoudiniAssetWidget* MHAWidget;

	HEMAX_Plugin* ActivePlugin;

    private slots:

	void HandleInputSelection(HEMAX_Node* Node,
                                  HEMAX_Parameter Parameter,
                                  bool ClearSelection);

	void HandleSubnetworkInputSelection(HEMAX_Node*, int, bool);

	void Slot_HandleUpdateParameterIntValues(HEMAX_Node* Node,
                                                 HEMAX_Parameter Parameter,
                                                 std::vector<int> IntValues,
                                                 bool DoNotRefreshUI);

	void Slot_HandleUpdateParameterFloatValues(
                                HEMAX_Node* Node,
                                HEMAX_Parameter Parameter,
                                std::vector<float> FloatValues,
                                bool DoNotRefreshUI);

	void Slot_HandleUpdateParameterStringValues(
                                HEMAX_Node* Node,
                                HEMAX_Parameter Parameter,
                                std::vector<std::string> StringValues);

	void Slot_HandleUpdateMultiParameterList(
                            HEMAX_Node* Node,
                            HEMAX_Parameter Parameter,
                            HEMAX_MultiParameterChangeInfo ChangeInfo);
};
