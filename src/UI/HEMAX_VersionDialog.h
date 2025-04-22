#pragma once

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026)
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#endif

#ifdef HEMAX_VERSION_2017
#pragma warning(push, 0)
#include <QtGui/qdialog.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qlabel.h>
#pragma warning(pop)
#endif

class HEMAX_VersionDialog : public QDialog
{
    public:
	HEMAX_VersionDialog();
	~HEMAX_VersionDialog();

    private:
	QVBoxLayout* Layout = nullptr;
	QLabel* HoudiniInfo = nullptr;
	QLabel* HoudiniEngineInfo = nullptr;

	std::string HoudiniVersionLabel;
	std::string HoudiniEngineVersionLabel;
};
