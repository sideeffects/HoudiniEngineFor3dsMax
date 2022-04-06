#include "HEMAX_VersionDialog.h"

#ifdef USE_HAPI_VERSION_HEADER
#include <HAPI_Version.h>
#endif

#include <sstream>
#include <string>

#define HEMAX_VERSION_DIALOG_MIN_WIDTH 350

HEMAX_VersionDialog::HEMAX_VersionDialog()
{
    std::stringstream Label;
    Label << "Houdini Version: " << HAPI_VERSION_HOUDINI_MAJOR << "."
        << HAPI_VERSION_HOUDINI_MINOR << "." << HAPI_VERSION_HOUDINI_BUILD;
    HoudiniVersionLabel = Label.str();

    Label.str("");
    Label << "Houdini Engine: " << HAPI_VERSION_HOUDINI_ENGINE_MAJOR << "."
        << HAPI_VERSION_HOUDINI_ENGINE_MINOR << " (API: "
        << HAPI_VERSION_HOUDINI_ENGINE_API << ")";
    HoudiniEngineVersionLabel = Label.str();

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
