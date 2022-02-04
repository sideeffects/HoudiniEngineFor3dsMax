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
    void RemoveModifierFromStack(HAPI_NodeId NodeId);
    void DeleteAndRemoveModifierFromStack(HAPI_NodeId NodeId);
    void DeleteAndRemoveModifierFromStack(HEMAX_Modifier* TheModifier);
    void RemoveAllModifiers();

    void LoadSubnetworkInputsFromCustomAttributes(HAPI_NodeId NodeId);
    void LoadModifierParametersFromCustomAttributes(HAPI_NodeId NodeId);

    HEMAX_Modifier* SearchModifierStackByNode(HEMAX_Node* TheNode);
    HEMAX_Modifier* SearchModifierStackByNodeId(HAPI_NodeId TheNode);
    HEMAX_Modifier* GetModifierByIndex(int Index);

    size_t GetNumberOfHDAModifiers();

    void UpdateNodeParameterIntValues(HAPI_NodeId NodeId, int ParameterId, std::vector<int> IntValues);
    void UpdateNodeParameterFloatValues(HAPI_NodeId NodeId, int ParameterId, std::vector<float> FloatValues);
    void UpdateNodeParameterStringValues(HAPI_NodeId NodeId, int ParameterId, std::vector<std::string> StringValues);
    void UpdateNodeParameterToggleValues(HAPI_NodeId NodeId, int ParameterId, std::vector<int> ToggleValues);
    void UpdateInputNode(HAPI_NodeId NodeId, int ParameterId, HEMAX_Input* InputNode);
    void UpdateSubnetworkInput(HAPI_NodeId NodeId, int Subnetwork, HEMAX_Input* InputNode);
    void UpdateMultiParameterList(HAPI_NodeId NodeId, int ParameterId);

    void CookModifier(HAPI_NodeId NodeId);

    INode* GetMaxNode();

    //HEMAX_MaxHoudiniAsset* GetMaxHoudiniAssetForModifier(HAPI_NodeId NodeId);

private:
    INode* TheMaxNode;

    std::list<HEMAX_ModifierBundle> NodeModifierStack;

    HEMAX_ModifierBundle SearchForModifierBundle(HAPI_NodeId NodeId);
};
