#pragma once

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qdialog.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qlabel.h>
#endif

class HEMAX_VersionDialog : public QDialog
{
public:
    HEMAX_VersionDialog();
    ~HEMAX_VersionDialog();

private:
    QVBoxLayout* Layout;
    QLabel* HoudiniInfo;
    QLabel* HoudiniEngineInfo;

    std::string HoudiniVersionLabel;
    std::string HoudiniEngineVersionLabel;
};