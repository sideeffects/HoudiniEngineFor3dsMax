#pragma once

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlistwidget.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qwidget.h>
#include <QtGui/qgroupbox.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qlistwidget.h>
#endif

#include <string>

#define HEMAX_HDAWIDGET_MAX_LIST_HEIGHT 350
#define HEMAX_HDAWIDGET_CREATE_BUTTONS_MIN_HEIGHT 50

class HEMAX_HDAWidget : public QWidget
{
    Q_OBJECT

    friend class HEMAX_UI;

public:
    HEMAX_HDAWidget();
    HEMAX_HDAWidget(std::string AssetsBoxTitle);
    ~HEMAX_HDAWidget();

    std::string GetCurrentAssetLoadPath();
    std::string GetSelectedAssetPath();

    void UpdateLoadedAssetList(std::vector<std::string>* Paths);

    void SetAssetLoadWidgetEnabled(bool Enabled);

private:

    QVBoxLayout* MainLayout;

    QWidget* MainBox;
    QVBoxLayout* MainBoxLayout;

    QGroupBox* AssetLoadOptionsBox;
    QGridLayout* AssetLoadOptionsBoxLayout;
    QLabel* AssetLoadOptionsPathLabel;
    QLineEdit* AssetLoadOptionsPath;
    QPushButton* AssetLoadOptionsPathBrowse;
    QPushButton* AssetLoadButton;

    QGroupBox* LoadedAssetsBox;
    QVBoxLayout* LoadedAssetsBoxLayout;
    QListWidget* LoadedAssetsList;
    QPushButton* LoadSelectedAssetButton;
    QPushButton* CreateModifiersButton;

    QString SelectedAssetPath;
    QString CurrentContextMenuSelection;

private slots:

    void SlotAssetLoadOptionsPathBrowse();
    void SlotLoadedAssetItemClicked(QListWidgetItem* Item);

    void SlotShowAssetContextMenu(QPoint Position);

    void SlotRemoveAssetClicked();
	void SlotCopyAssetPathClicked();

signals:
    void SignalRemoveAssetClicked(QString);

};
