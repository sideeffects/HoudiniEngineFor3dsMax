#include "HEMAX_ModifierAsset.h"
#include "HEMAX_Logger.h"

HEMAX_ModifierAsset::HEMAX_ModifierAsset(INode* Node)
    : TheMaxNode(Node)
{}
/*
HEMAX_ModifierAsset::~HEMAX_ModifierAsset()
{
    for (auto ModBundle = NodeModifierStack.begin(); ModBundle != NodeModifierStack.end(); ++ModBundle)
    {
        if ((*ModBundle).AssetContainer)
        {
            (*ModBundle).Modifier->ClearHAPIReferences();
            delete (*ModBundle).AssetContainer;
        }
    }
    NodeModifierStack.clear();
}

void
HEMAX_ModifierAsset::RemoveAllModifiers()
{
    for (auto ModBundle = NodeModifierStack.begin(); ModBundle != NodeModifierStack.end(); ++ModBundle)
    {
        if ((*ModBundle).Modifier)
        {
            (*ModBundle).Modifier->DeleteThis();
        }
    }
}

void
HEMAX_ModifierAsset::AppendNewModifierToStack(HEMAX_Modifier* TheModifier, std::string AssetPath, int AssetLibIndex, ULONG MaxNode, bool GenerateCustomAttributes)
{
    HEMAX_MaxHoudiniAsset* ModifierContainer = new HEMAX_MaxHoudiniAsset(AssetPath, AssetLibIndex, TheModifier->GetNode(), MaxNode);

    HEMAX_ModifierBundle TheBundle;
    TheBundle.Modifier = TheModifier;
    TheBundle.AssetContainer = ModifierContainer;

    if (GenerateCustomAttributes)
    {
        TheModifier->AllocCustAttribContainer();

        if (TheModifier->GetCustAttribContainer())
        {
            ModifierContainer->SetCustomAttributeContainer(TheModifier->GetCustAttribContainer());
            ModifierContainer->GenerateBoilerplateModifierCustomAttributes(AssetPath, AssetLibIndex);
            ModifierContainer->InitializeParameterCustomAttributes();
        }
    }
    else
    {
        if (TheModifier->GetCustAttribContainer())
        {
            ModifierContainer->SetCustomAttributeContainer(TheModifier->GetCustAttribContainer());
            ModifierContainer->ReinstateCustomAttributes(HEMAX_HOUDINI_MODIFIER_MAX_INDEX);
        }
    }

    NodeModifierStack.push_back(TheBundle);
}

HEMAX_Modifier*
HEMAX_ModifierAsset::SearchModifierStackByNode(HEMAX_Node* TheNode)
{
    for (auto Search = NodeModifierStack.begin(); Search != NodeModifierStack.end(); ++Search)
    {
        if ((*Search).Modifier->GetNode()->Info.id == TheNode->Info.id)
        {
            return (*Search).Modifier;
        }
    }

    return nullptr;
}

HEMAX_Modifier*
HEMAX_ModifierAsset::SearchModifierStackByNodeId(HEMAX_NodeId TheNode)
{
    for (auto Search = NodeModifierStack.begin(); Search != NodeModifierStack.end(); ++Search)
    {
        if ((*Search).Modifier->GetNode()->Info.id == TheNode)
        {
            return (*Search).Modifier;
        }
    }

    return nullptr;
}

HEMAX_ModifierBundle
HEMAX_ModifierAsset::SearchForModifierBundle(HEMAX_NodeId NodeId)
{
    for (auto Search = NodeModifierStack.begin(); Search != NodeModifierStack.end(); ++Search)
    {
        if ((*Search).AssetContainer->GetNode()->Info.id == NodeId)
        {
            return (*Search);
        }
    }

    HEMAX_ModifierBundle NullBundle;
    NullBundle.AssetContainer = nullptr;
    NullBundle.Modifier = nullptr;

    return NullBundle;
}

void
HEMAX_ModifierAsset::UpdateNodeParameterIntValues(HEMAX_NodeId NodeId, int ParameterId, std::vector<int> IntValues)
{
    HEMAX_ModifierBundle ModBundle = SearchForModifierBundle(NodeId);

    if (ModBundle.AssetContainer)
    {
        ModBundle.AssetContainer->UpdateNodeParameterIntValues(ParameterId, IntValues);
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_ModifierAsset: problem when updating parameter with id " + std::to_string(ParameterId) +
            "-> could not find the node that it belongs to with node id " + std::to_string(NodeId) + "in its asset container"), HEMAX_LOG_LEVEL_ERROR);
    }
}

void
HEMAX_ModifierAsset::UpdateNodeParameterFloatValues(HEMAX_NodeId NodeId, int ParameterId, std::vector<float> FloatValues)
{
    HEMAX_ModifierBundle ModBundle = SearchForModifierBundle(NodeId);

    if (ModBundle.AssetContainer)
    {
        ModBundle.AssetContainer->UpdateNodeParameterFloatValues(ParameterId, FloatValues);
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_ModifierAsset: problem when updating parameter with id " + std::to_string(ParameterId) +
            "-> could not find the node that it belongs to with node id " + std::to_string(NodeId) + "in its asset container"), HEMAX_LOG_LEVEL_ERROR);
    }
}

void
HEMAX_ModifierAsset::UpdateNodeParameterStringValues(HEMAX_NodeId NodeId, int ParameterId, std::vector<std::string> StringValues)
{
    HEMAX_ModifierBundle ModBundle = SearchForModifierBundle(NodeId);

    if (ModBundle.AssetContainer)
    {
        ModBundle.AssetContainer->UpdateNodeParameterStringValues(ParameterId, StringValues);
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_ModifierAsset: problem when updating parameter with id " + std::to_string(ParameterId) +
            "-> could not find the node that it belongs to with node id " + std::to_string(NodeId) + "in its asset container"), HEMAX_LOG_LEVEL_ERROR);
    }
}

void
HEMAX_ModifierAsset::UpdateNodeParameterToggleValues(HEMAX_NodeId NodeId, int ParameterId, std::vector<int> ToggleValues)
{
    HEMAX_ModifierBundle ModBundle = SearchForModifierBundle(NodeId);

    if (ModBundle.AssetContainer)
    {
        ModBundle.AssetContainer->UpdateNodeParameterToggleValues(ParameterId, ToggleValues);
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_ModifierAsset: problem when updating parameter with id " + std::to_string(ParameterId) +
            "-> could not find the node that it belongs to with node id " + std::to_string(NodeId) + "in its asset container"), HEMAX_LOG_LEVEL_ERROR);
    }
}

void
HEMAX_ModifierAsset::UpdateInputNode(HEMAX_NodeId NodeId, int ParameterId, HEMAX_Input* InputNode)
{
    HEMAX_ModifierBundle ModBundle = SearchForModifierBundle(NodeId);

    if (ModBundle.AssetContainer)
    {
        ModBundle.AssetContainer->UpdateInputNode(ParameterId);
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_ModifierAsset: problem when updating parameter with id " + std::to_string(ParameterId) +
            "-> could not find the node that it belongs to with node id " + std::to_string(NodeId) + "in its asset container"), HEMAX_LOG_LEVEL_ERROR);
    }
}

void
HEMAX_ModifierAsset::UpdateSubnetworkInput(HEMAX_NodeId NodeId, int Subnetwork, HEMAX_Input* InputNode)
{
    HEMAX_ModifierBundle ModBundle = SearchForModifierBundle(NodeId);

    if (ModBundle.AssetContainer)
    {
        ModBundle.AssetContainer->UpdateSubnetworkInput(Subnetwork);
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_ModifierAsset: problem when updating subnetwork  " + std::to_string(Subnetwork) +
            "-> could not find the node that it belongs to with node id " + std::to_string(NodeId) + "in its asset container"), HEMAX_LOG_LEVEL_ERROR);
    }
}

void
HEMAX_ModifierAsset::UpdateMultiParameterList(HEMAX_NodeId NodeId, int ParameterId)
{
    HEMAX_ModifierBundle ModBundle = SearchForModifierBundle(NodeId);

    if (ModBundle.AssetContainer)
    {
        ModBundle.AssetContainer->UpdateMultiParameterList(ParameterId);
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_ModifierAsset: problem when updating multi-parameter with id " + std::to_string(ParameterId) +
            "-> could not find the node that it belongs to with node id " + std::to_string(NodeId) + "in its asset container"), HEMAX_LOG_LEVEL_ERROR);
    }
}

void
HEMAX_ModifierAsset::CookModifier(HEMAX_NodeId NodeId)
{
    HEMAX_ModifierBundle ModBundle = SearchForModifierBundle(NodeId);

    if (ModBundle.AssetContainer)
    {
        ModBundle.AssetContainer->CookHDA();
        ModBundle.Modifier->CookNode();
        ModBundle.Modifier->ForceNotify(FOREVER);
    }
}

void
HEMAX_ModifierAsset::RemoveModifierFromStack(HEMAX_NodeId NodeId)
{
    for (auto Search = NodeModifierStack.begin(); Search != NodeModifierStack.end(); ++Search)
    {
        if (((*Search).Modifier) && (*Search).Modifier->GetNode()->Info.id == NodeId)
        {
            (*Search).Modifier->CleanupHAPIComponents();
            NodeModifierStack.erase(Search);
            break;
        }
    }
}

void
HEMAX_ModifierAsset::DeleteAndRemoveModifierFromStack(HEMAX_NodeId NodeId)
{
    for (auto Search = NodeModifierStack.begin(); Search != NodeModifierStack.end(); ++Search)
    {
        if ((*Search).Modifier->GetNode()->Info.id == NodeId)
        {
            (*Search).AssetContainer->EmptyOutCustomAttributeContainer();
            (*Search).Modifier->DeleteCustAttribContainer();
            delete (*Search).AssetContainer;
            (*Search).Modifier->DeleteThis();

            NodeModifierStack.erase(Search);

            break;
        }
    }
}

void
HEMAX_ModifierAsset::DeleteAndRemoveModifierFromStack(HEMAX_Modifier* TheModifier)
{
    for (auto Search = NodeModifierStack.begin(); Search != NodeModifierStack.end(); ++Search)
    {
        if ((*Search).Modifier == TheModifier)
        {
            (*Search).AssetContainer->EmptyOutCustomAttributeContainer();
            (*Search).Modifier->DeleteCustAttribContainer();
            delete (*Search).AssetContainer;
            (*Search).Modifier->DeleteThis();

            NodeModifierStack.erase(Search);
            break;
        }
    }
}

size_t
HEMAX_ModifierAsset::GetNumberOfHDAModifiers()
{
    return NodeModifierStack.size();
}

void
HEMAX_ModifierAsset::LoadSubnetworkInputsFromCustomAttributes(HEMAX_NodeId NodeId)
{
    HEMAX_ModifierBundle Bundle = SearchForModifierBundle(NodeId);

    if (Bundle.AssetContainer)
    {
        Bundle.AssetContainer->ReloadSubnetworkInputsFromCustomAttributes();
    }
}

void
HEMAX_ModifierAsset::LoadModifierParametersFromCustomAttributes(HEMAX_NodeId NodeId)
{
    HEMAX_ModifierBundle Bundle = SearchForModifierBundle(NodeId);

    if (Bundle.AssetContainer)
    {
        Bundle.AssetContainer->ReloadParametersFromCustomAttributes();
    }
}

INode*
HEMAX_ModifierAsset::GetMaxNode()
{
    return TheMaxNode;
}

HEMAX_Modifier*
HEMAX_ModifierAsset::GetModifierByIndex(int Index)
{
    if (Index >= NodeModifierStack.size())
    {
        return nullptr;
    }
    else
    {
        auto Iter = NodeModifierStack.begin();
        std::advance(Iter, Index);
        return ((*Iter).Modifier);
    }
}

HEMAX_MaxHoudiniAsset*
HEMAX_ModifierAsset::GetMaxHoudiniAssetForModifier(HEMAX_NodeId NodeId)
{
    HEMAX_ModifierBundle ModBundle = SearchForModifierBundle(NodeId);

    if (ModBundle.AssetContainer)
    {
        return ModBundle.AssetContainer;
    }
    else
    {
        return nullptr;
    }
}
*/