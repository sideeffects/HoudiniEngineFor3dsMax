#include "HEMAX_HDAWidget.h"

#include "moc_HEMAX_HDAWidget.cpp"

#include "HEMAX_ParameterWidget.h"
#include "../HEMAX_3dsmaxHda.h"
#include "../HEMAX_Plugin.h"
#include "../HEMAX_SessionManager.h"
#include "../resource.h"

#ifdef HEMAX_VERSION_2017
#pragma warning(push, 0)
#include <QtGui/qboxlayout.h>
#include <QtGui/qimage.h>
#include <QtGui/qlabel.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qtoolbutton.h>
#pragma warning(pop)
#else
#include <QtGui/qimage.h>
#include <QtGui/qpixmap.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtoolbutton.h>
#endif

#include <sstream>
#include <string>
#include <windows.h>

HEMAX_HDAWidget::HEMAX_HDAWidget(HEMAX_Plugin* ThePlugin)
    : Plugin(ThePlugin)
{
    bool LoadEngineLogoResult = LoadHoudiniEngineLogo();

    SessionStatusWidget = new QWidget;
    SessionStatusWidgetLayout = new QHBoxLayout;
    SessionStatusLabel = new QLabel;

    SessionStatusWidget->setLayout(SessionStatusWidgetLayout);
    SessionStatusWidgetLayout->addWidget(SessionStatusLabel, 0,
        Qt::AlignCenter);

    SelectionWidget = new QWidget;
    SelectionWidgetLayout = new QHBoxLayout;
    CurrentSelectionLabel = new QLabel;
    LockSelectionButton = new QPushButton;
    SelectionWidgetLayout->addWidget(CurrentSelectionLabel, 2, Qt::AlignRight);
    SelectionWidgetLayout->addWidget(LockSelectionButton, 3);
    SelectionWidget->setLayout(SelectionWidgetLayout);

    CookControlsWidget = new QWidget;
    CookControlsWidgetLayout = new QVBoxLayout;
    CookControlsWidgetLayout->setSpacing(0);
    CookControlsWidgetLayout->setContentsMargins(0, 0, 0, 0);
    CookControlsHeader = new QToolButton;
    CookControlsHeader->setText("Cook");
    CookControlsHeader->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    CookControlsHeader->setArrowType(Qt::DownArrow);
    CookControlsCollapsibleWidget = new QWidget;
    CookControlsContentLayout = new QHBoxLayout;
    CookControlsCollapsibleWidget->setLayout(CookControlsContentLayout);
    RecookButton = new QPushButton("Recook");
    RebuildButton = new QPushButton("Rebuild");
    ResetParametersButton = new QPushButton("Reset Parameters");
    CookControlsContentLayout->addWidget(RecookButton);
    CookControlsContentLayout->addWidget(RebuildButton);
    CookControlsContentLayout->addWidget(ResetParametersButton);
    CookControlsWidgetLayout->addWidget(CookControlsHeader);
    CookControlsWidgetLayout->addWidget(CookControlsCollapsibleWidget);
    CookControlsWidget->setLayout(CookControlsWidgetLayout);

    BakeControlsWidget = new QWidget;
    BakeControlsWidgetLayout = new QVBoxLayout;
    BakeControlsWidgetLayout->setSpacing(0);
    BakeControlsWidgetLayout->setContentsMargins(0, 0, 0, 0);
    BakeControlsHeader = new QToolButton;
    BakeControlsHeader->setText("Bake");
    BakeControlsHeader->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    BakeControlsHeader->setArrowType(Qt::DownArrow);
    BakeControlsWidgetLayout->addWidget(BakeControlsHeader);
    BakeControlsCollapsibleWidget = new QWidget;
    BakeControlsContentLayout = new QHBoxLayout;
    BakeControlsCollapsibleWidget->setLayout(BakeControlsContentLayout);
    BakeButton = new QPushButton("Bake");
    BakeControlsContentLayout->addWidget(BakeButton);
    BakeControlsWidgetLayout->addWidget(BakeControlsCollapsibleWidget);
    BakeControlsWidget->setLayout(BakeControlsWidgetLayout);

    CloneControlsWidget = new QWidget;
    CloneControlsWidgetLayout = new QVBoxLayout;
    CloneControlsWidgetLayout->setSpacing(0);
    CloneControlsWidgetLayout->setContentsMargins(0, 0, 0, 0);
    CloneControlsHeader = new QToolButton;
    CloneControlsHeader->setText("Clone");
    CloneControlsHeader->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    CloneControlsHeader->setArrowType(Qt::DownArrow);
    CloneControlsWidgetLayout->addWidget(CloneControlsHeader);
    CloneControlsCollapsibleWidget = new QWidget;
    CloneControlsContentLayout = new QHBoxLayout;
    CloneControlsCollapsibleWidget->setLayout(CloneControlsContentLayout);
    CloneButton = new QPushButton("Clone");
    CloneControlsContentLayout->addWidget(CloneButton);
    CloneControlsWidgetLayout->addWidget(CloneControlsCollapsibleWidget);
    CloneControlsWidget->setLayout(CloneControlsWidgetLayout);

    ParametersWidget = new QWidget;
    ParametersWidgetLayout = new QVBoxLayout;
    ParametersHeader = new QToolButton;
    ParametersHeader->setText("Parameters");
    ParametersHeader->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ParametersHeader->setArrowType(Qt::DownArrow);
    ParametersWidgetLayout->addWidget(ParametersHeader);
    ParametersContentWidget = new HEMAX_ParameterWidget;
    ParametersWidgetLayout->addWidget(ParametersContentWidget);
    ParametersWidget->setLayout(ParametersWidgetLayout);

    HDAWidgetLayout = new QVBoxLayout;
    HDAWidgetLayout->setSpacing(0);
    HDAWidgetLayout->setContentsMargins(0, 0, 0, 0);
    HDAWidgetLayout->setAlignment(Qt::AlignTop);

    if (LoadEngineLogoResult)
    {
        HDAWidgetLayout->addWidget(HoudiniEngineBannerLabel);
        HDAWidgetLayout->addWidget(HoudiniEngineLogoSeparator);
    }
    HDAWidgetLayout->addWidget(SessionStatusWidget);
    HDAWidgetLayout->addWidget(SelectionWidget);
    HDAWidgetLayout->addWidget(CookControlsWidget);
    HDAWidgetLayout->addWidget(BakeControlsWidget);
    HDAWidgetLayout->addWidget(CloneControlsWidget);
    HDAWidgetLayout->addWidget(ParametersWidget);

    CookControlsHeader->setStyleSheet(
        "border: none; text-align: left");
    BakeControlsHeader->setStyleSheet(
        "border: none; text-align: left");
    CloneControlsHeader->setStyleSheet(
        "border: none; text-align: left");
    ParametersHeader->setStyleSheet(
        "border: none; text-align: left");

    this->setLayout(HDAWidgetLayout);

    Update();

    QObject::connect(CookControlsHeader, SIGNAL(clicked()),
        this, SLOT(CookControlsHeaderClickedSlot()));
    QObject::connect(BakeControlsHeader, SIGNAL(clicked()),
        this, SLOT(BakeControlsHeaderClickedSlot()));
    QObject::connect(CloneControlsHeader, SIGNAL(clicked()),
        this, SLOT(CloneControlsHeaderClickedSlot()));
    QObject::connect(ParametersHeader, SIGNAL(clicked()),
        this, SLOT(ParametersHeaderClickedSlot()));
    QObject::connect(RecookButton, SIGNAL(clicked()),
        this, SLOT(RecookButtonClickedSlot()));
    QObject::connect(RebuildButton, SIGNAL(clicked()),
        this, SLOT(RebuildButtonClickedSlot()));
    QObject::connect(ResetParametersButton, SIGNAL(clicked()),
        this, SLOT(ResetParametersButtonClickedSlot()));
    QObject::connect(LockSelectionButton, SIGNAL(clicked()),
        this, SLOT(LockSelectionButtonClickedSlot()));

    QObject::connect(ParametersContentWidget,
        SIGNAL(Signal_CookNode(HEMAX_Node*)),
        this, SLOT(CookNodeSlot(HEMAX_Node*)));
    QObject::connect(ParametersContentWidget,
        SIGNAL(Signal_InputSelection(HEMAX_Node*, HEMAX_Parameter, bool)),
        this, SLOT(InputSelectionSlot(HEMAX_Node*, HEMAX_Parameter, bool)));
    QObject::connect(ParametersContentWidget,
        SIGNAL(Signal_SubnetworkInputSelection(
                    HEMAX_Node*, int, bool)),
        this,
        SLOT(SubnetworkInputSelectionSlot(HEMAX_Node*, int, bool)));
    QObject::connect(ParametersContentWidget,
        SIGNAL(Signal_UpdateParameterIntValues(
                HEMAX_Node*, HEMAX_Parameter, std::vector<int>, bool)),
        this,
        SLOT(UpdateParameterIntValuesSlot(HEMAX_Node*, HEMAX_Parameter,
                std::vector<int>, bool)));
    QObject::connect(ParametersContentWidget,
        SIGNAL(Signal_UpdateParameterFloatValues(HEMAX_Node*, HEMAX_Parameter,
                std::vector<float>, bool)),
        this,
        SLOT(UpdateParameterFloatValuesSlot(HEMAX_Node*, HEMAX_Parameter,
                std::vector<float>, bool)));
    QObject::connect(ParametersContentWidget,
        SIGNAL(Signal_UpdateParameterStringValues(HEMAX_Node*, HEMAX_Parameter,
                std::vector<std::string>)),
        this,
        SLOT(UpdateParameterStringValuesSlot(HEMAX_Node*, HEMAX_Parameter,
                std::vector<std::string>)));
    QObject::connect(ParametersContentWidget,
        SIGNAL(Signal_UpdateMultiParameterList(HEMAX_Node*, HEMAX_Parameter,
                HEMAX_MultiParameterChangeInfo)),
        this,
        SLOT(UpdateMultiParameterListSlot(HEMAX_Node*, HEMAX_Parameter,
                HEMAX_MultiParameterChangeInfo)));

}

void
HEMAX_HDAWidget::Update()
{
    UpdateSessionStatusWidget();
    UpdateSelectionWidget();
    ParametersContentWidget->RefreshUI(false);
}

void
HEMAX_HDAWidget::SetSelection(HEMAX_3dsmaxHda* SelectedHda, bool ForceUnlock)
{
    if (ForceUnlock)
        Locked = false;

    if (!Locked)
    {
        Selection = SelectedHda;
        ParametersContentWidget->SelectHDA(SelectedHda);
        Update();
    }
}

void
HEMAX_HDAWidget::UpdateParameters(bool DeleteWidgetsLater)
{
    ParametersContentWidget->RefreshUI(DeleteWidgetsLater);
}

void
HEMAX_HDAWidget::UpdateSessionStatusWidget()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.IsSessionValidAndInitialized())
    {
        SessionStatusLabel->setText("Houdini Engine Session READY");
        SessionStatusLabel->setStyleSheet("color: lime");
    }
    else
    {
        SessionStatusLabel->setText("Houdini Engine Session STOPPED");
        SessionStatusLabel->setStyleSheet("color: yellow");
    }
}

void
HEMAX_HDAWidget::UpdateSelectionWidget()
{
    if (Selection)
    {
        M_STD_OSTRINGSTREAM Sstream;
        Sstream << Selection->Get3dsMaxContainerName() << _T(" (")
            << Selection->Hda.MainNode.AssetName << _T(")");
        CurrentSelectionLabel->setText(
            HEMAX_Utilities::GetQString(Sstream.str()));
    }
    else
    {
        CurrentSelectionLabel->setText("No Asset Selected");
    }

    if (Locked)
    {
        LockSelectionButton->setText("Unlock Selection");
    }
    else
    {
        LockSelectionButton->setText("Lock Selection");
    }
}

bool
HEMAX_HDAWidget::LoadHoudiniEngineLogo()
{
    HMODULE ModuleHandle = GetModuleHandleA("HoudiniEngineFor3DSMax.dlu");
    
    if (!ModuleHandle)
        return false;

    HRSRC ResourceHandle = FindResource(ModuleHandle,
        MAKEINTRESOURCE(IDI_HEMAX_HOUDINI_ENGINE_LOGO),
        MAKEINTRESOURCE(IDI_HEMAX_PNG_RESOURCE));

    if (!ResourceHandle)
        return false;

    HGLOBAL ResourceDataHandle = LoadResource(ModuleHandle, ResourceHandle);

    if (!ResourceDataHandle)
        return false;

    LPVOID PngData = LockResource(ResourceDataHandle);

    if (!PngData)
        return false;

    DWORD NumBytes = SizeofResource(ModuleHandle, ResourceHandle);

    if (NumBytes == 0)
        return false;

    HoudiniEngineBannerLabel = new QLabel;
    HoudiniEngineLogoImage = new QImage;
    bool LoadResult =
        HoudiniEngineLogoImage->loadFromData(
            (const uchar*)PngData, NumBytes, "PNG");

    if (!LoadResult)
    {
        delete HoudiniEngineLogoImage;
        HoudiniEngineLogoImage = nullptr;
        delete HoudiniEngineBannerLabel;
        HoudiniEngineBannerLabel = nullptr;
        return false;
    }

    HoudiniEngineBannerLabel->setPixmap(
        QPixmap::fromImage(*HoudiniEngineLogoImage));

    HoudiniEngineLogoSeparator = new QFrame;
    HoudiniEngineLogoSeparator->setFrameShape(QFrame::Shape::HLine);

    return true;
}

void
HEMAX_HDAWidget::CookControlsHeaderClickedSlot()
{
    static int MaxHeight = CookControlsCollapsibleWidget->maximumHeight();

    if (CookControlsCollapsibleWidget->maximumHeight() > 0)
    {
        CookControlsCollapsibleWidget->setMaximumHeight(0);
        CookControlsHeader->setArrowType(Qt::RightArrow);
    }
    else
    {
        CookControlsCollapsibleWidget->setMaximumHeight(MaxHeight);
        CookControlsHeader->setArrowType(Qt::DownArrow);
    }
}

void
HEMAX_HDAWidget::BakeControlsHeaderClickedSlot()
{
    static int MaxHeight = BakeControlsCollapsibleWidget->maximumHeight();

    if (BakeControlsCollapsibleWidget->maximumHeight() > 0)
    {
        BakeControlsCollapsibleWidget->setMaximumHeight(0);
        BakeControlsHeader->setArrowType(Qt::RightArrow);
    }
    else
    {
        BakeControlsCollapsibleWidget->setMaximumHeight(MaxHeight);
        BakeControlsHeader->setArrowType(Qt::DownArrow);
    }
}

void
HEMAX_HDAWidget::CloneControlsHeaderClickedSlot()
{
    static int MaxHeight = CloneControlsCollapsibleWidget->maximumHeight();

    if (CloneControlsCollapsibleWidget->maximumHeight() > 0)
    {
        CloneControlsCollapsibleWidget->setMaximumHeight(0);
        CloneControlsHeader->setArrowType(Qt::RightArrow);
    }
    else
    {
        CloneControlsCollapsibleWidget->setMaximumHeight(MaxHeight);
        CloneControlsHeader->setArrowType(Qt::DownArrow);
    }
}

void
HEMAX_HDAWidget::ParametersHeaderClickedSlot()
{
    static int MaxHeight = ParametersContentWidget->maximumHeight();

    if (ParametersContentWidget->maximumHeight() > 0)
    {
        ParametersContentWidget->setMaximumHeight(0);
        ParametersHeader->setArrowType(Qt::RightArrow);
    }
    else
    {
        ParametersContentWidget->setMaximumHeight(MaxHeight);
        ParametersHeader->setArrowType(Qt::DownArrow);
    }
}

void
HEMAX_HDAWidget::RecookButtonClickedSlot()
{
    if (Selection)
    {
        Plugin->HandleRecookRequest(&Selection->Hda.MainNode);
    } 
}

void
HEMAX_HDAWidget::RebuildButtonClickedSlot()
{
    if (!Selection)
        return;

    QMessageBox ConfirmationDialog;
    ConfirmationDialog.setWindowFlags(Qt::WindowStaysOnTopHint);
    ConfirmationDialog.setWindowTitle("Rebuild Asset");

    M_STD_OSTRINGSTREAM SStream;
    SStream << _T("This will delete and recreate all nodes in your scene that are ")
        _T("using the currently selected HDA (")
        << Selection->Hda.MainNode.AssetName << _T(")");
    ConfirmationDialog.setText(HEMAX_Utilities::GetQString(SStream.str()));
    ConfirmationDialog.setInformativeText(
            "Are you sure that you want to continue?");
    ConfirmationDialog.setStandardButtons(
            (QMessageBox::Ok | QMessageBox::Cancel));
    int Result = ConfirmationDialog.exec();

    if (Result == QMessageBox::Ok)
    {
        HEMAX_Node* MainNode = &(Selection->Hda.MainNode);
        Plugin->ReloadAssetDefinition(MainNode);
    }
}

void
HEMAX_HDAWidget::ResetParametersButtonClickedSlot()
{
    // TODO: ui
}

void
HEMAX_HDAWidget::LockSelectionButtonClickedSlot()
{
    if (!Locked && Selection)
        Locked = true;
    else
        Locked = false;

    Update();
}

void
HEMAX_HDAWidget::CookNodeSlot(HEMAX_Node* Node)
{

}

void
HEMAX_HDAWidget::InputSelectionSlot(HEMAX_Node* Node,
        HEMAX_Parameter Parameter, bool ClearSelection)
{
    if (ClearSelection)
        Plugin->HandleParameterInputCleared(Node, Parameter);
    else
        Plugin->HandleParameterInputSelection(Node, Parameter);

    UpdateParameters();
}

void
HEMAX_HDAWidget::SubnetworkInputSelectionSlot(HEMAX_Node* Node,
        int Subnetwork, bool ClearSelection)
{
    if (ClearSelection)
        Plugin->HandleSubnetworkInputCleared(Node, Subnetwork);
    else
        Plugin->HandleSubnetworkInputSelection(Node, Subnetwork);

    UpdateParameters();
}

void
HEMAX_HDAWidget::UpdateParameterIntValuesSlot(HEMAX_Node* Node,
        HEMAX_Parameter Parameter, std::vector<int> Values, bool SkipUpdateUI)
{
    Plugin->HandleParameterIntValuesUpdate(Node, Parameter, Values);

    if (Node->AutoRecookOnParameterUpdate && !SkipUpdateUI)
        UpdateParameters();
}

void
HEMAX_HDAWidget::UpdateParameterFloatValuesSlot(HEMAX_Node* Node,
        HEMAX_Parameter Parameter, std::vector<float> Values, bool SkipUpdateUI)
{
    Plugin->HandleParameterFloatValuesUpdate(Node, Parameter, Values);

    if (Node->AutoRecookOnParameterUpdate && !SkipUpdateUI)
        UpdateParameters();
}

void
HEMAX_HDAWidget::UpdateParameterStringValuesSlot(HEMAX_Node* Node,
        HEMAX_Parameter Parameter, std::vector<std::string> Values)
{
    Plugin->HandleParameterStringValuesUpdate(Node, Parameter, Values);

    if (Node->AutoRecookOnParameterUpdate)
        UpdateParameters();
}

void
HEMAX_HDAWidget::UpdateMultiParameterListSlot(HEMAX_Node* Node,
        HEMAX_Parameter Parameter, HEMAX_MultiParameterChangeInfo ChangeInfo)
{
    Plugin->HandleMultiParameterUpdate(Node, Parameter, ChangeInfo);
    UpdateParameters();
}
