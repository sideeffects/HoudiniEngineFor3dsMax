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

class HEMAX_HDASelectionDialog : public QDialog
{
    Q_OBJECT

    public:
	HEMAX_HDASelectionDialog(std::vector<std::string> AssetPaths);
	~HEMAX_HDASelectionDialog();

	std::string GetSelectedAssetPath();

    private:

	QVBoxLayout* MainLayout;

	QGroupBox* HDASelectionBox;
	QVBoxLayout* HDASelectionLayout;
	QListWidget* HDASelectionList;
	QPushButton* HDASelectionButton;

	QString CurrentSelection;

    private slots:

	void SlotSelectionButtonClicked();
	void SlotListSelection(QListWidgetItem*);

};
