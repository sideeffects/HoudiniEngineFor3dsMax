#include "HEMAX_ModifierHda.h"

#include "HEMAX_SessionManager.h"
#include "HEMAX_Modifier.h"

HEMAX_ModifierHda::HEMAX_ModifierHda()
{
    ContainerNode = nullptr;
    CustomAttributes = nullptr;
    DisplayGeometry = nullptr;
    Type = HEMAX_MODIFIER_HDA;
}

bool
HEMAX_ModifierHda::Create(HEMAX_Asset& Asset, int AssetIndex,
			  HEMAX_Modifier* Modifier, INode* MaxNode)
{
    Hda.Init(Asset, AssetIndex);

    if (Hda.MainNode.Info.inputCount > 0)
    {
	SubnetworkNodeInputs.resize(Hda.MainNode.Info.inputCount);
    }

    bool Success = CreateNewModifierHda(Hda, Modifier, MaxNode);

    if (Success)
    {
	InitializeParameterCustomAttributes();
    }

    return Success;
}

void
HEMAX_ModifierHda::Recreate(HEMAX_Asset& Asset, int AssetIndex,
			    HEMAX_Modifier* Modifier, INode* ContainerNode)
{
    Hda.Init(Asset, AssetIndex);
    InitializeSubnetworks();
    RecreateExistingModifierHda(Hda, Modifier, ContainerNode);
}

bool
HEMAX_ModifierHda::CreateNewModifierHda(HEMAX_Hda& Hda, HEMAX_Modifier* Modifier, INode* MaxNode)
{
    if (Hda.HdaType == SOP_LEVEL_HDA)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

	ContainerNode = MaxNode;

	InitModifierPluginCustAttribContainer(Modifier);
	GenerateBoilerplateModifierPluginCustomAttributes(Modifier, Hda);

	DisplayGeometry = Modifier;

	Modifier->SetHda(&Hda);
	Modifier->SetAssetName(Hda.Name);
	Modifier->MaxNode = MaxNode;
	Modifier->MarkCreated();

	return true;
    }
    else
    {
	return false;
    }
}

void
HEMAX_ModifierHda::RecreateExistingModifierHda(HEMAX_Hda& Hda, HEMAX_Modifier* Modifier, INode* MaxNode)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    ContainerNode = MaxNode;
    DisplayGeometry = Modifier;
    CustomAttributes = Modifier->GetCustAttribContainer();
    MaxStampIndex = HEMAX_HOUDINI_MODIFIER_MAX_INDEX;

    for (int i = HEMAX_HOUDINI_MODIFIER_MAX_INDEX; i < CustomAttributes->GetNumCustAttribs(); i++)
    {
        HEMAX_ParameterAttrib* CustAttrib =
            dynamic_cast<HEMAX_ParameterAttrib*>(CustomAttributes->GetCustAttrib(i));
        if (CustAttrib)
        {
            CustomAttributeMap.insert(
                {CustAttrib->GetParameterName(), CustAttrib});
        }
    }

    Modifier->SetHda(&Hda);
    Modifier->SetAssetName(Hda.Name);
    Modifier->MaxNode = MaxNode;
    Modifier->MarkCreated();
}

void
HEMAX_ModifierHda::UpdateModifierHda()
{
    if (DisplayGeometry)
    {
	DisplayGeometry->SetHda(&Hda);
	DisplayGeometry->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
    }
}

void
HEMAX_ModifierHda::DeleteMainInput()
{
    if (DisplayGeometry)
    {
	DisplayGeometry->DeleteInputNode();
    }
}

void
HEMAX_ModifierHda::InitModifierPluginCustAttribContainer(HEMAX_Modifier* ModifierPlugin)
{
    CustomAttributes = ModifierPlugin->GetCustAttribContainer();

    if (!CustomAttributes)
    {
	ModifierPlugin->AllocCustAttribContainer();
	CustomAttributes = ModifierPlugin->GetCustAttribContainer();
    }
}

void
HEMAX_ModifierHda::GenerateBoilerplateModifierPluginCustomAttributes(HEMAX_Modifier* ModifierPlugin, HEMAX_Hda& Hda)
{
    if (CustomAttributes)
    {
	// Generate HEMAX boilerplate custom attributes. These are necessary for reloading the scene
	HEMAX_StringParameterAttrib* ModifierAssetStamp = new HEMAX_StringParameterAttrib;
	ModifierAssetStamp->SetParameterName(std::string(HEMAX_HOUDINI_MODIFIER_STAMP_NAME));
	ModifierAssetStamp->PBlock->SetValue(0, 0, L"TRUE");
	CustomAttributes->InsertCustAttrib(HEMAX_HOUDINI_MODIFIER_STAMP_INDEX, ModifierAssetStamp);

	HEMAX_StringParameterAttrib* ModifierAssetPath = new HEMAX_StringParameterAttrib;
	ModifierAssetPath->SetParameterName(std::string(HEMAX_HOUDINI_MODIFIER_ASSET_PATH_NAME));
	std::wstring WideAssetPath(Hda.HdaAsset.Path.begin(), Hda.HdaAsset.Path.end());
	ModifierAssetPath->PBlock->SetValue(0, 0, WideAssetPath.c_str());
	CustomAttributes->InsertCustAttrib(HEMAX_HOUDINI_MODIFIER_ASSET_PATH_INDEX, ModifierAssetPath);

	HEMAX_IntegerParameterAttrib* ModifierAssetLibIndex = new HEMAX_IntegerParameterAttrib;
	ModifierAssetLibIndex->SetParameterName(std::string(HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_NAME));
	ModifierAssetLibIndex->PBlock->SetValue(0, 0, Hda.HdaAssetIndex);
	CustomAttributes->InsertCustAttrib(HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_INDEX, ModifierAssetLibIndex);

	// Make custom attributes for the secondary sub-network inputs. First subnetwork input is reserved for the modifier stack input.
	for (int s = 1; s < Hda.MainNode.Info.inputCount; s++)
	{
	    HEMAX_NodeParameterAttrib* InputCustAttrib = new HEMAX_NodeParameterAttrib;
	    InputCustAttrib->SetParameterName("subnetwork_" + std::to_string(s));
	    // TODO what is this doing?
	    //InputCustAttrib->CreateMaxHoudiniAssetLink(this, HEMAX_INPUT_SUBNETWORK, s);
	    CustomAttributes->AppendCustAttrib(InputCustAttrib);
	    CustomAttributeMap.insert({ "subnetwork_" + std::to_string(s), InputCustAttrib });
	}

	MaxStampIndex = HEMAX_HOUDINI_MODIFIER_MAX_INDEX;
    }
}

void
HEMAX_ModifierHda::Update3dsmaxHda()
{
    Hda.Update();
    UpdateModifierHda();
}
