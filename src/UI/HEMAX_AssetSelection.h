#pragma once

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024)
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qdialog.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qlistwidget.h>
#include <QtGui/qpushbutton.h>
#endif

const int HEMAX_AssetSelection_DefaultWidth = 600;
const int HEMAX_AssetSelection_DefaultHeight = 400;

class HEMAX_AssetSelection : public QDialog
{
    Q_OBJECT

    public:
	HEMAX_AssetSelection(std::vector<std::string> AssetNames);
	HEMAX_AssetSelection(std::vector<std::string> AssetNames, std::string DialogTitle, std::string DialogSubtitle);
	~HEMAX_AssetSelection();

	std::string GetSelectedAssetName();

    private:

	QVBoxLayout* MainLayout;

	QGroupBox* AssetSelectionBox;
	QVBoxLayout* AssetSelectionLayout;
	QListWidget* AssetSelectionList;
	QPushButton* AssetSelectionButton;

	QString CurrentSelection;

    private slots:

	void SlotSelectionButtonClicked();
	void SlotListSelection(QListWidgetItem*);
};
