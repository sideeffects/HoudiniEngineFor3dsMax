#include "HEMAX_MaxHoudiniAssetWidget.h"

#include "../HEMAX_3dsmaxHda.h"
#include "../HEMAX_GeometryHda.h"
#include "../HEMAX_Plugin.h"
#include "../HEMAX_UserPrefs.h"

#include "HEMAX_AssetSelection.h"

#include "moc_HEMAX_MaxHoudiniAssetWidget.cpp"

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024)
#include <QtWidgets/qmessagebox.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qmessagebox.h>
#endif

HEMAX_MaxHoudiniAssetWidget::HEMAX_MaxHoudiniAssetWidget(
                                HEMAX_Plugin* ActivePlugin)
    : Plugin(ActivePlugin)
    , Selection(nullptr)
{
    HdaActionButtons = new QWidget;
    HdaActionButtonsLayout = new QGridLayout;

    RecookHdaButton = new QPushButton("Recook");
    ReloadHdaButton = new QPushButton("Reload Asset Definition");
    BakeHdaButton = new QPushButton("Bake");
    CloneHdaButton = new QPushButton("Clone");
    CopyToNodeButton = new QPushButton("Copy To Different Object");

    CreateMHAOptionsUI();
    CreateAdvancedOptionsUI();

    HdaActionButtonsLayout->setAlignment(Qt::AlignTop);
    HdaActionButtons->setLayout(HdaActionButtonsLayout);
    MainBoxLayout->insertWidget(1, HdaActionButtons);

    HdaActionButtonsLayout->addWidget(RecookHdaButton, 0, 0);
    HdaActionButtonsLayout->addWidget(ReloadHdaButton, 0, 1);
    HdaActionButtonsLayout->addWidget(BakeHdaButton, 1, 0);
    HdaActionButtonsLayout->addWidget(CloneHdaButton, 1, 1);

    MainBoxLayout->insertWidget(2, MHAOptionsBox);
    MainBoxLayout->addWidget(AdvancedOptionsBox);

    RecookHdaButton->setVisible(false);
    ReloadHdaButton->setVisible(false);
    BakeHdaButton->setVisible(false);
    CloneHdaButton->setVisible(false);
    CopyToNodeButton->setVisible(false);
    MHAOptionsBox->setVisible(false);
    AdvancedOptionsBox->setVisible(false);

    QObject::connect(this, SIGNAL(Signal_CookNode(HEMAX_Node*)),
                     this, SLOT(Slot_RecookHdaButton()));

    QObject::connect(RecookHdaButton, SIGNAL(clicked()),
                     this, SLOT(Slot_RecookHdaButton()));

    QObject::connect(ReloadHdaButton, SIGNAL(clicked()),
                     this, SLOT(Slot_ReloadHdaButton()));

    QObject::connect(BakeHdaButton, SIGNAL(clicked()),
                     this, SLOT(Slot_BakeHdaButton()));

    QObject::connect(CloneHdaButton, SIGNAL(clicked()),
                     this, SLOT(Slot_CloneHdaButton()));

    QObject::connect(CopyToNodeButton, SIGNAL(clicked()),
                     this, SLOT(Slot_CopyToNodeButton()));

    QObject::connect(MHAOptions_PushTransformToHAPI,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(Slot_MHAOptions_PushTransformToHAPI(int)));

    QObject::connect(MHAOptions_ApplyHAPITransformToNode,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(Slot_MHAOptions_ApplyHAPITransformToNode(int)));

    QObject::connect(AdvancedOptionsEnabledCheckbox,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(Slot_AdvancedOptionsCheckbox(int)));

    QObject::connect(AdvancedOptions_HDAPathSave,
                     SIGNAL(clicked()),
                     this,
                     SLOT(Slot_AdvancedOptionsHdaPathSave()));
}

HEMAX_MaxHoudiniAssetWidget::~HEMAX_MaxHoudiniAssetWidget()
{
    delete AdvancedOptions_HDAPathSave;
    delete AdvancedOptions_HDAPath;
    delete AdvancedOptions_HDAPathLabel;
    delete AdvancedOptions_WidgetLayout;
    delete AdvancedOptions_Widget;

    delete AdvancedOptionsEnabledCheckbox;
    delete AdvancedOptionsLayout;
    delete AdvancedOptionsBox;

    delete MHAOptions_ApplyHAPITransformToNode;
    delete MHAOptions_PushTransformToHAPI;

    delete MHAOptionsBoxLayout;
    delete MHAOptionsBox;

    delete CopyToNodeButton;
    delete CloneHdaButton;
    delete BakeHdaButton;
    delete ReloadHdaButton;
    delete RecookHdaButton;

    delete HdaActionButtonsLayout;
    delete HdaActionButtons;
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_LockSelectionButton_Clicked()
{
    HandleLockSelectionButtonChanged(!SelectionLocked);
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_RecookHdaButton()
{
    if (Selection)
    {
        Plugin->HandleRecookRequest(&Selection->Hda.MainNode);
    }
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_ReloadHdaButton()
{
    if (Selection)
    {
        QMessageBox ConfirmationDialog;
        ConfirmationDialog.setWindowFlags(Qt::WindowStaysOnTopHint);
        ConfirmationDialog.setWindowTitle("Reload Asset Definition");
        ConfirmationDialog.setText("This will delete and recreate all HDAs "
                "in your scene that are using this asset definition.");
        ConfirmationDialog.setInformativeText(
                "Are you sure that you want to continue?");
        ConfirmationDialog.setStandardButtons(
                (QMessageBox::Ok | QMessageBox::Cancel));
        int Result = ConfirmationDialog.exec();

        if (Result == QMessageBox::Ok)
        {
            HEMAX_Node* MainNode = &(Selection->Hda.MainNode);

            if (IsSelectionLocked())
            {
                HandleLockSelectionButtonChanged(false);
            }

            Plugin->ReloadAssetDefinition(MainNode);
        }
    }
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_BakeHdaButton()
{
    if (Selection && Selection->Type == HEMAX_GEOMETRY_HDA)
    {
        HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(Selection);

        bool BakeDummyObj;
        HEMAX_UserPrefs::Get().GetBoolSetting(HEMAX_SETTING_BAKE_DUMMY_OBJECT,
            BakeDummyObj);
        GeometryHda->BakeGeometryHda(BakeDummyObj);
    }
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_CloneHdaButton()
{
    if (Selection)
    {
        Plugin->CloneHda(Selection);
    }
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_CopyToNodeButton()
{
    if (Selection)
    {
	std::vector<std::wstring> NodeNames;

	HEMAX_Utilities::GetListOfAllSceneNodes(NodeNames);

	std::vector<std::string> NodeNamesA;
	for (auto It = NodeNames.begin(); It != NodeNames.end(); It++)
	{
	    NodeNamesA.push_back(std::string(It->begin(), It->end()));
	}

	HEMAX_AssetSelection NodeSelectDialog(NodeNamesA, "Node Selection", "Nodes in scene:");

	if (NodeSelectDialog.exec())
	{
	    std::string Selected = NodeSelectDialog.GetSelectedAssetName();

	    if (!Selected.empty())
	    {
		std::wstring Selected_W = std::wstring(Selected.begin(), Selected.end());
		INode* SelectedNode = GetCOREInterface()->GetINodeByName(Selected_W.c_str());
		if (SelectedNode)
		{
		    Plugin->CopyHdaToNode(Selection, SelectedNode);
		}
	    }
	}
    }
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_MHAOptions_PushTransformToHAPI(int State)
{
    if (Selection && Selection->Type == HEMAX_GEOMETRY_HDA)
    {
	HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(Selection);
	GeometryHda->SetPushTransformsOption(State);
    }
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_MHAOptions_ApplyHAPITransformToNode(int State)
{
    if (Selection && Selection->Type == HEMAX_GEOMETRY_HDA)
    {
	HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(Selection);
	GeometryHda->SetApplyHAPITransformOption(State);
    }
}

void
HEMAX_MaxHoudiniAssetWidget::SetSelection(HEMAX_3dsmaxHda* Hda, bool ForceUnlock)
{
    if (ForceUnlock && SelectionLocked)
    {
	Slot_LockSelectionButton_Clicked();
    }

    if (!SelectionLocked)
    {
	Selection = Hda;
	UpdateWidget();
    }
}

void
HEMAX_MaxHoudiniAssetWidget::UpdateWidget()
{
    HideHdaActionButtonsUI();

    if (!Selection)
    {
	SelectHDA(false);
    }
    else if (Selection->Type == HEMAX_GEOMETRY_HDA)
    {
	UpdateHdaActionButtonsUI(Selection->Type);
	SelectHDA(&Selection->Hda.MainNode);
	// Hide for now
	//MHAOptionsBox->setVisible(true);
	
	HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(Selection);

	MHAOptions_PushTransformToHAPI->setChecked(GeometryHda->IsPushTransformsOptionEnabled());
	MHAOptions_ApplyHAPITransformToNode->setChecked(GeometryHda->ShouldApplyHAPITransform());
	AdvancedOptionsBox->setVisible(true);
	PushSubnetworkInputNames();
	PushParameterInputNames();
    }
    else if (Selection->Type == HEMAX_MODIFIER_HDA)
    {
	SelectHDA(&Selection->Hda.MainNode);
	UpdateHdaActionButtonsUI(Selection->Type);
	AdvancedOptionsBox->setVisible(true);

	if (Selection->Hda.MainNode.Info.inputCount > 0)
	{
	    DisableSubnetworkInputUI(0);
	}
	PushSubnetworkInputNames();
	PushParameterInputNames();
    }

    UpdateAdvancedOptionsUI();
}

void
HEMAX_MaxHoudiniAssetWidget::PushSubnetworkInputNames()
{
    if (Selection)
    {
	for (int i = 0; i < SubnetworkInputs.size(); i++)
	{
	    if (SubnetworkInputs[i])
	    {
		if (Selection->SubnetworkNodeInputs[i])
		{
		    INode* InputNode = GetCOREInterface()->GetINodeByHandle(Selection->SubnetworkNodeInputs[i]->MaxInput->Get3dsMaxNodeHandle());
		    std::wstring WideName = InputNode->GetName();
		    std::string InputNodeName = std::string(WideName.begin(), WideName.end());

		    HEMAX_ParameterWidget_Node* NodePWidget = dynamic_cast<HEMAX_ParameterWidget_Node*>(SubnetworkInputs[i]);
		    NodePWidget->SetInputName(InputNodeName);
		}
	    }
	}
    }
}

void
HEMAX_MaxHoudiniAssetWidget::PushParameterInputNames()
{
    if (Selection)
    {
	for (int i = 0; i < OpParmWidgets.size(); i++)
	{
	    if (OpParmWidgets[i])
	    {
		HEMAX_ParameterWidget_Parameter* PWidget = dynamic_cast<HEMAX_ParameterWidget_Parameter*>(OpParmWidgets[i]);
		HAPI_ParmId ParmId = PWidget->GetParameterId();

		auto Search = Selection->InputNodeMap.find(ParmId);

		if (Search != Selection->InputNodeMap.end())
		{
		    INode* InputNode = GetCOREInterface()->GetINodeByHandle(Search->second->MaxInput->Get3dsMaxNodeHandle());
		    std::wstring WideName = InputNode->GetName();
		    std::string InputNodeName = std::string(WideName.begin(), WideName.end());

		    HEMAX_ParameterWidget_Node* NodePWidget = dynamic_cast<HEMAX_ParameterWidget_Node*>(PWidget);
		    NodePWidget->SetInputName(InputNodeName);
		}
	    }
	}
    }
}

void
HEMAX_MaxHoudiniAssetWidget::RefreshParameterUI(bool DeleteLater)
{
    RefreshUI(DeleteLater);

    if (Selection && Selection->Type == HEMAX_MODIFIER_HDA)
    {
	if (Selection->Hda.MainNode.Info.inputCount > 0)
	{
	    DisableSubnetworkInputUI(0);
	}
    }
    PushSubnetworkInputNames();
    PushParameterInputNames();
}

HEMAX_3dsmaxHda*
HEMAX_MaxHoudiniAssetWidget::GetCurrentHdaSelection()
{
    return Selection;
}

bool
HEMAX_MaxHoudiniAssetWidget::IsSelectionLocked()
{
    return SelectionLocked;
}

void
HEMAX_MaxHoudiniAssetWidget::HandleLockSelectionButtonChanged(int Locked)
{
    // This means it's about to be unlocked
    if (!Locked)
    {
	Selection = nullptr;
	HideHdaActionButtonsUI();
    }

    HEMAX_ParameterWidget::SetSelectionLocked(Locked);
}

void
HEMAX_MaxHoudiniAssetWidget::CreateMHAOptionsUI()
{
    MHAOptionsBox = new QGroupBox("Houdini Digital Asset Options");
    MHAOptionsBoxLayout = new QGridLayout;

    MHAOptions_PushTransformToHAPI = new QCheckBox("Push transform to HAPI");
    MHAOptions_ApplyHAPITransformToNode = new QCheckBox("Apply HAPI transform to 3DS Max Node");

    MHAOptionsBox->setAlignment(Qt::AlignTop);
    MHAOptionsBoxLayout->setAlignment(Qt::AlignTop);

    MHAOptionsBoxLayout->addWidget(MHAOptions_PushTransformToHAPI, 1, 0);
    MHAOptionsBoxLayout->addWidget(MHAOptions_ApplyHAPITransformToNode, 2, 0);

    MHAOptionsBox->setLayout(MHAOptionsBoxLayout);
}

void
HEMAX_MaxHoudiniAssetWidget::CreateAdvancedOptionsUI()
{
    AdvancedOptionsBox = new QGroupBox("Advanced Options");
    AdvancedOptionsLayout = new QGridLayout;
    AdvancedOptionsEnabledCheckbox = new QCheckBox("Show Advanced Options");

    AdvancedOptions_Widget = new QWidget;
    AdvancedOptions_WidgetLayout = new QGridLayout;
    AdvancedOptions_HDAPathLabel = new QLabel("HDA Path:");
    AdvancedOptions_HDAPath = new QLineEdit;
    AdvancedOptions_HDAPathSave = new QPushButton("Save");

    AdvancedOptions_WidgetLayout->setAlignment(Qt::AlignTop);
    AdvancedOptions_WidgetLayout->addWidget(AdvancedOptions_HDAPathLabel, 0, 0);
    AdvancedOptions_WidgetLayout->addWidget(AdvancedOptions_HDAPath, 0, 1);
    AdvancedOptions_WidgetLayout->addWidget(AdvancedOptions_HDAPathSave, 0, 2);
    AdvancedOptions_Widget->setLayout(AdvancedOptions_WidgetLayout);

    AdvancedOptionsLayout->addWidget(AdvancedOptionsEnabledCheckbox);
    AdvancedOptionsLayout->addWidget(AdvancedOptions_Widget);

    AdvancedOptionsBox->setLayout(AdvancedOptionsLayout);
    AdvancedOptionsLayout->setAlignment(Qt::AlignTop);

    AdvancedOptions_Widget->setHidden(true);
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_AdvancedOptionsCheckbox(int State)
{
    if (State)
    {
	AdvancedOptions_Widget->setHidden(false);
    }
    else
    {
	AdvancedOptions_Widget->setHidden(true);
    }
}

void
HEMAX_MaxHoudiniAssetWidget::Slot_AdvancedOptionsHdaPathSave()
{
    if (Selection)
    {
	Selection->SetHardcodedHdaAssetPath(AdvancedOptions_HDAPath->text().toStdString());
    }
}

void
HEMAX_MaxHoudiniAssetWidget::UpdateAdvancedOptionsUI()
{
    if (Selection)
    {
	AdvancedOptions_HDAPath->setText(Selection->GetHardcodedHdaAssetPath().c_str());
    }
    else
    {
	AdvancedOptions_HDAPath->setText("");
    }
}

void
HEMAX_MaxHoudiniAssetWidget::HideHdaActionButtonsUI()
{
    RecookHdaButton->setVisible(false);
    ReloadHdaButton->setVisible(false);
    BakeHdaButton->setVisible(false);
    CloneHdaButton->setVisible(false);
    CopyToNodeButton->setVisible(false);
    MHAOptionsBox->setVisible(false);
    AdvancedOptionsBox->setVisible(false);
}

void
HEMAX_MaxHoudiniAssetWidget::UpdateHdaActionButtonsUI(HEMAX_HdaType HdaType)
{
    if (HdaType == HEMAX_GEOMETRY_HDA)
    {
	RecookHdaButton->setVisible(true);
	ReloadHdaButton->setVisible(true);
	BakeHdaButton->setVisible(true);
	CloneHdaButton->setVisible(true);
	CopyToNodeButton->setVisible(false);

	HdaActionButtonsLayout->removeWidget(CopyToNodeButton);
	HdaActionButtonsLayout->removeWidget(BakeHdaButton);
	HdaActionButtonsLayout->addWidget(BakeHdaButton, 1, 0);
    }
    else if (HdaType == HEMAX_MODIFIER_HDA)
    {
	RecookHdaButton->setVisible(true);
	ReloadHdaButton->setVisible(true);
	BakeHdaButton->setVisible(false);
	CloneHdaButton->setVisible(true);
	CopyToNodeButton->setVisible(true);

	HdaActionButtonsLayout->removeWidget(CopyToNodeButton);
	HdaActionButtonsLayout->removeWidget(BakeHdaButton);
	HdaActionButtonsLayout->addWidget(CopyToNodeButton, 1, 0);
    }
}
