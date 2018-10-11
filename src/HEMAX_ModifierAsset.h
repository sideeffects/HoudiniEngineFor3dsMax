#pragma once

#include "HEMAX_Modifier.h"

class HEMAX_Modifier;

struct HEMAX_ModifierBundle
{
    HEMAX_Modifier* Modifier;
    //HEMAX_MaxHoudiniAsset* AssetContainer;
};

class HEMAX_ModifierAsset
{
public:
    HEMAX_ModifierAsset(INode* Node);
    ~HEMAX_ModifierAsset();

    void AppendNewModifierToStack(HEMAX_Modifier* TheModifier, std::string AssetPath, int AssetLibIndex, ULONG MaxNode, bool GenerateCustomAttributes);
    void RemoveModifierFromStack(HEMAX_NodeId NodeId);
    void DeleteAndRemoveModifierFromStack(HEMAX_NodeId NodeId);
    void DeleteAndRemoveModifierFromStack(HEMAX_Modifier* TheModifier);
    void RemoveAllModifiers();

    void LoadSubnetworkInputsFromCustomAttributes(HEMAX_NodeId NodeId);
    void LoadModifierParametersFromCustomAttributes(HEMAX_NodeId NodeId);

    HEMAX_Modifier* SearchModifierStackByNode(HEMAX_Node* TheNode);
    HEMAX_Modifier* SearchModifierStackByNodeId(HEMAX_NodeId TheNode);
    HEMAX_Modifier* GetModifierByIndex(int Index);

    size_t GetNumberOfHDAModifiers();

    void UpdateNodeParameterIntValues(HEMAX_NodeId NodeId, int ParameterId, std::vector<int> IntValues);
    void UpdateNodeParameterFloatValues(HEMAX_NodeId NodeId, int ParameterId, std::vector<float> FloatValues);
    void UpdateNodeParameterStringValues(HEMAX_NodeId NodeId, int ParameterId, std::vector<std::string> StringValues);
    void UpdateNodeParameterToggleValues(HEMAX_NodeId NodeId, int ParameterId, std::vector<int> ToggleValues);
    void UpdateInputNode(HEMAX_NodeId NodeId, int ParameterId, HEMAX_Input* InputNode);
    void UpdateSubnetworkInput(HEMAX_NodeId NodeId, int Subnetwork, HEMAX_Input* InputNode);
    void UpdateMultiParameterList(HEMAX_NodeId NodeId, int ParameterId);

    void CookModifier(HEMAX_NodeId NodeId);

    INode* GetMaxNode();

    //HEMAX_MaxHoudiniAsset* GetMaxHoudiniAssetForModifier(HEMAX_NodeId NodeId);

private:
    INode* TheMaxNode;

    std::list<HEMAX_ModifierBundle> NodeModifierStack;

    HEMAX_ModifierBundle SearchForModifierBundle(HEMAX_NodeId NodeId);
};
