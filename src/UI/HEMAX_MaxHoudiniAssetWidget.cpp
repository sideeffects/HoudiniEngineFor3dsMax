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
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025)
#include <QtWidgets/qmessagebox.h>
#endif

#ifdef HEMAX_VERSION_2017
#include <QtGui/qmessagebox.h>
#endif

#include <sstream>

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

    CreateAdvancedOptionsUI();

    HdaActionButtonsLayout->setAlignment(Qt::AlignTop);
    HdaActionButtons->setLayout(HdaActionButtonsLayout);
    MainBoxLayout->insertWidget(1, HdaActionButtons);

    HdaActionButtonsLayout->addWidget(RecookHdaButton, 0, 0);
    HdaActionButtonsLayout->addWidget(ReloadHdaButton, 0, 1);
    HdaActionButtonsLayout->addWidget(BakeHdaButton, 1, 0);
    HdaActionButtonsLayout->addWidget(CloneHdaButton, 1, 1);

    MainBoxLayout->addWidget(AdvancedOptionsBox);

    RecookHdaButton->setVisible(false);
    ReloadHdaButton->setVisible(false);
    BakeHdaButton->setVisible(false);
    CloneHdaButton->setVisible(false);
    CopyToNodeButton->setVisible(false);
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

    QObject::connect(AdvancedOptionsEnabledCheckbox,
                     SIGNAL(stateChanged(int)),
                     this,
                     SLOT(Slot_AdvancedOptionsCheckbox(int)));

    QObject::connect(AdvancedOptions_HDAPathSave,
                     SIGNAL(clicked()),
                     this,
                     SLOT(Slot_AdvancedOptionsHdaPathSave()));
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
HEMAX_MaxHoudiniAssetWidget::UpdateWidget()
{
    HideHdaActionButtonsUI();

    if (!Selection)
    {
	SelectHDA(nullptr);
    }
    else if (Selection->Type == HEMAX_GEOMETRY_HDA)
    {
	UpdateHdaActionButtonsUI(Selection->Type);
	SelectHDA(Selection);
	
	HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(Selection);

	AdvancedOptionsBox->setVisible(true);
	PushSubnetworkInputNames();
	PushParameterInputNames();
    }
    else if (Selection->Type == HEMAX_MODIFIER_HDA)
    {
	SelectHDA(Selection);
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
    if (!Selection)
        return;

    for (int i = 0; i < SubnetworkInputs.size(); i++)
    {
        if (!SubnetworkInputs[i])
            continue;

        if (!Selection->SubnetworkNodeInputs[i])
            continue;

        std::wstringstream sstream;

        for (auto&& Input : Selection->SubnetworkNodeInputs[i]->MaxInputs)
        {
            INode* Node = GetCOREInterface()->GetINodeByHandle(
                    Input->Get3dsMaxNodeHandle());

            if (sstream.str().size() > 0)
                sstream << " ";

            sstream << Node->GetName();
        }

        std::wstring WideName = sstream.str();
        std::string InputNodesField = std::string(WideName.begin(), WideName.end());

        HEMAX_ParameterWidget_Node* NodePWidget =
            dynamic_cast<HEMAX_ParameterWidget_Node*>(SubnetworkInputs[i]);
        NodePWidget->SetInputName(InputNodesField);
    }
}

void
HEMAX_MaxHoudiniAssetWidget::PushParameterInputNames()
{
    if (!Selection)
        return;

    for (int i = 0; i < OpParmWidgets.size(); ++i)
    {
        if (!OpParmWidgets[i])
            continue;

        HEMAX_ParameterWidget_Parameter* PWidget =
            dynamic_cast<HEMAX_ParameterWidget_Parameter*>(OpParmWidgets[i]);
        HAPI_ParmId ParmId = PWidget->GetParameterId();

        auto Search = Selection->InputNodeMap.find(ParmId);

        if (Search == Selection->InputNodeMap.end())
            continue;

        std::wstringstream sstream;

        for (auto&& Input : Search->second->MaxInputs)
        {
            INode* Node = GetCOREInterface()->GetINodeByHandle(
                Input->Get3dsMaxNodeHandle());

            if (sstream.str().size() > 0)
                sstream << " ";

            sstream << Node->GetName();
        }

        std::wstring WideName = sstream.str();
        std::string InputNodesFieldValue =
            std::string(WideName.begin(), WideName.end());

        HEMAX_ParameterWidget_Node* NodePWidget =
            dynamic_cast<HEMAX_ParameterWidget_Node*>(PWidget);
        NodePWidget->SetInputName(InputNodesFieldValue);
    }
}

/*
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
*/

HEMAX_3dsmaxHda*
HEMAX_MaxHoudiniAssetWidget::GetCurrentHdaSelection()
{
    return Selection;
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
