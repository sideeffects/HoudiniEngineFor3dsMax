#pragma once

#include "../HEMAX_Events.h"
#include "../HEMAX_Plugin.h"
#include "HEMAX_AssetWidget.h"
#include "HEMAX_OutputLogWidget.h"
#include "HEMAX_ShelfTab.h"

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
#include <Qt/QmaxMainWindow.h>
#include <Qt/QmaxDockWidget.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qtabwidget.h>
#endif

#ifdef HEMAX_VERSION_2017
#pragma warning(push, 0)
#include <QtGui/qscrollarea.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qmainwindow.h>
#pragma warning(pop)
#endif

#define HEMAX_2017_WINDOW_START_HEIGHT 600
#define HEMAX_2017_WINDOW_START_WIDTH 600

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <maxapi.h>
#pragma warning(pop)

class HEMAX_3dsmaxHda;
class HEMAX_HDAWidget;
class HEMAX_Plugin;

class HEMAX_UI : public QDockWidget, public HEMAX_EventHandler
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

        void OnSessionChangedCallback();

    private:

	QScrollArea* ScrollArea = nullptr;
	QTabWidget* TabContainer = nullptr;

	HEMAX_ShelfTab* ShelfToolsWidget = nullptr;
	HEMAX_AssetWidget* AssetWidget = nullptr;
        HEMAX_HDAWidget* HdaWidget = nullptr;
        HEMAX_OutputLogWidget* OutputLogTab = nullptr;

	HEMAX_Plugin* ActivePlugin = nullptr;
};
