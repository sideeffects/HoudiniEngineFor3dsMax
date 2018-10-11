#include "HEMAX_ModifierHda.h"

#include "HEMAX_SessionManager.h"
#include "HEMAX_Modifier.h"

bool
CreateNewModifierHda(HEMAX_ModifierHda& ModifierHda, HEMAX_Hda& Hda, HEMAX_Modifier* Modifier, INode* MaxNode)
{
    if (Hda.SopLevelHda)
    {
        HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

        ModifierHda.ContainerNode = MaxNode;

        InitModifierPluginCustAttribContainer(ModifierHda, Modifier);
        GenerateBoilerplateModifierPluginCustomAttributes(ModifierHda, Modifier, Hda);

        ModifierHda.DisplayGeometry = Modifier;

        Modifier->OutputNode = Hda.MainNode;
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
RecreateExistingModifierHda(HEMAX_ModifierHda& ModifierHda, HEMAX_Hda& Hda, HEMAX_Modifier* Modifier, INode* MaxNode)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    
    ModifierHda.ContainerNode = MaxNode;
    ModifierHda.DisplayGeometry = Modifier;
    ModifierHda.CustomAttributes = Modifier->GetCustAttribContainer();
    ModifierHda.MaxStampIndex = HEMAX_HOUDINI_MODIFIER_MAX_INDEX;

    for (int i = HEMAX_HOUDINI_MODIFIER_MAX_INDEX; i < ModifierHda.CustomAttributes->GetNumCustAttribs(); i++)
    {
        HEMAX_ParameterAttrib* CustAttrib = static_cast<HEMAX_ParameterAttrib*>(ModifierHda.CustomAttributes->GetCustAttrib(i));
        ModifierHda.CustomAttributeMap.insert({ CustAttrib->GetParameterName(), CustAttrib });
    }
    
    Modifier->OutputNode = Hda.MainNode;
    Modifier->SetAssetName(Hda.Name);
    Modifier->MaxNode = MaxNode;
    Modifier->MarkCreated();
}

void
UpdateModifierHda(HEMAX_ModifierHda& ModifierHda, HEMAX_Hda& Hda)
{
    if (ModifierHda.DisplayGeometry)
    {
        ModifierHda.DisplayGeometry->OutputNode = Hda.MainNode;
        ModifierHda.DisplayGeometry->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
    }
}

void
DeleteMainInput(HEMAX_ModifierHda& ModifierHda)
{
    if (ModifierHda.DisplayGeometry)
    {
        ModifierHda.DisplayGeometry->DeleteInputNode();
    }
}

static void
InitModifierPluginCustAttribContainer(HEMAX_ModifierHda& ModifierHda, HEMAX_Modifier* ModifierPlugin)
{
    ModifierHda.CustomAttributes = ModifierPlugin->GetCustAttribContainer();

    if (!ModifierHda.CustomAttributes)
    {
        ModifierPlugin->AllocCustAttribContainer();
        ModifierHda.CustomAttributes = ModifierPlugin->GetCustAttribContainer();
    }
}

static void
GenerateBoilerplateModifierPluginCustomAttributes(HEMAX_ModifierHda& ModifierHda, HEMAX_Modifier* ModifierPlugin, HEMAX_Hda& Hda)
{
    if (ModifierHda.CustomAttributes)
    {
        // Generate HEMAX boilerplate custom attributes. These are necessary for reloading the scene
        HEMAX_StringParameterAttrib* ModifierAssetStamp = new HEMAX_StringParameterAttrib;
        ModifierAssetStamp->SetParameterName(std::string(HEMAX_HOUDINI_MODIFIER_STAMP_NAME));
        ModifierAssetStamp->PBlock->SetValue(0, 0, L"TRUE");
        ModifierHda.CustomAttributes->InsertCustAttrib(HEMAX_HOUDINI_MODIFIER_STAMP_INDEX, ModifierAssetStamp);

        HEMAX_StringParameterAttrib* ModifierAssetPath = new HEMAX_StringParameterAttrib;
        ModifierAssetPath->SetParameterName(std::string(HEMAX_HOUDINI_MODIFIER_ASSET_PATH_NAME));
        std::wstring WideAssetPath(Hda.HdaAsset.Path.begin(), Hda.HdaAsset.Path.end());
        ModifierAssetPath->PBlock->SetValue(0, 0, WideAssetPath.c_str());
        ModifierHda.CustomAttributes->InsertCustAttrib(HEMAX_HOUDINI_MODIFIER_ASSET_PATH_INDEX, ModifierAssetPath);

        HEMAX_IntegerParameterAttrib* ModifierAssetLibIndex = new HEMAX_IntegerParameterAttrib;
        ModifierAssetLibIndex->SetParameterName(std::string(HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_NAME));
        ModifierAssetLibIndex->PBlock->SetValue(0, 0, Hda.HdaAssetIndex);
        ModifierHda.CustomAttributes->InsertCustAttrib(HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_INDEX, ModifierAssetLibIndex);

        // Make custom attributes for the secondary sub-network inputs. First subnetwork input is reserved for the modifier stack input.
        for (int s = 1; s < Hda.MainNode.Info.inputCount; s++)
        {
            HEMAX_NodeParameterAttrib* InputCustAttrib = new HEMAX_NodeParameterAttrib;
            InputCustAttrib->SetParameterName("subnetwork_" + std::to_string(s));
            // TODO what is this doing?
            //InputCustAttrib->CreateMaxHoudiniAssetLink(this, HEMAX_INPUT_SUBNETWORK, s);
            ModifierHda.CustomAttributes->AppendCustAttrib(InputCustAttrib);
            ModifierHda.CustomAttributeMap.insert({ "subnetwork_" + std::to_string(s), InputCustAttrib });
        }

        ModifierHda.MaxStampIndex = HEMAX_HOUDINI_MODIFIER_MAX_INDEX;
    }
}