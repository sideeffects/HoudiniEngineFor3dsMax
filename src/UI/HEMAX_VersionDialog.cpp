#include "HEMAX_VersionDialog.h"

#include <string>

#define HEMAX_VERSION_DIALOG_MIN_WIDTH 350

HEMAX_VersionDialog::HEMAX_VersionDialog()
{
    HoudiniVersionLabel = "Houdini Version: " + std::to_string(HEMAX_HAPI_VERSION_HOUDINI_MAJOR) + "." + std::to_string(HEMAX_HAPI_VERSION_HOUDINI_MINOR) + "." + std::to_string(HEMAX_HAPI_VERSION_HOUDINI_BUILD);
    HoudiniEngineVersionLabel = "Houdini Engine: " + std::to_string(HEMAX_HAPI_VERSION_HOUDINI_ENGINE_MAJOR) + "." + std::to_string(HEMAX_HAPI_VERSION_HOUDINI_ENGINE_MINOR) + " (API: " + std::to_string(HEMAX_HAPI_VERSION_HOUDINI_ENGINE_API) + ")";

    this->setWindowTitle("Version Information");

    Layout = new QVBoxLayout;
    HoudiniInfo = new QLabel(HoudiniVersionLabel.c_str());
    HoudiniEngineInfo = new QLabel(HoudiniEngineVersionLabel.c_str());

    Layout->setAlignment(Qt::AlignTop);

    Layout->addWidget(HoudiniInfo);
    Layout->addWidget(HoudiniEngineInfo);

    this->setLayout(Layout);
    this->setMinimumWidth(HEMAX_VERSION_DIALOG_MIN_WIDTH);
}

HEMAX_VersionDialog::~HEMAX_VersionDialog()
{
    delete HoudiniEngineInfo;
    delete HoudiniInfo;
    delete Layout;
}