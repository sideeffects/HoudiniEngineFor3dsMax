#pragma once

#include "HEMAX_Hda.h"
#include "HEMAX_GeometryHda.h"
#include "HEMAX_ModifierHda.h"
#include "HEMAX_CustomAttributeContainer.h"
#include "HEMAX_InputInstance.h"
#include <icustattribcontainer.h>
#include <unordered_map>

enum HEMAX_HdaType
{
    HEMAX_GEOMETRY_HDA,
    HEMAX_MODIFIER_HDA
};

struct HEMAX_SubnetworkInputMapping
{
    int Subnetwork;
    INode* Node;
};

struct HEMAX_ParameterInputMapping
{
    std::string ParameterName;
    INode* Node;
};

struct HEMAX_3dsmaxHda
{
    HEMAX_HdaType Type;
    HEMAX_Hda Hda;

    HEMAX_GeometryHda GeometryHda;
    HEMAX_ModifierHda ModifierHda;

    std::vector<HEMAX_InputInstance*> SubnetworkNodeInputs;
    std::unordered_map<HEMAX_ParameterId, HEMAX_InputInstance*> InputNodeMap;
};

void
CreateGeometry3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, HEMAX_Asset& Asset, int AssetIndex);

void
CreateGeometry3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, INode* ContainerNode, HEMAX_Asset& Asset, int AssetIndex);

bool
CreateModifier3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, HEMAX_Asset& Asset, int AssetIndex, HEMAX_Modifier* Modifier, INode* MaxNode);

void
RecreateGeometry3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, HEMAX_Asset& Asset, int AssetIndex, INode* ContainerNode);

void
RecreateModifier3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, HEMAX_Asset& Asset, int AssetIndex, HEMAX_Modifier* Modifier, INode* ContainerNode);

static void
InitializeSubnetworks(HEMAX_3dsmaxHda& MaxHda);

void
Cook3dsmaxHda(HEMAX_3dsmaxHda& MaxHda);

void
Update3dsmaxHda(HEMAX_3dsmaxHda& MaxHda);

void
SetParameterInput(HEMAX_3dsmaxHda& MaxHda, HEMAX_ParameterId ParamId, HEMAX_InputInstance* Input);

HEMAX_InputInstance*
FindParameterInput(HEMAX_3dsmaxHda& MaxHda, HEMAX_ParameterId ParamId);

std::vector<HEMAX_Parameter>
GetAllParametersWithInputs(HEMAX_3dsmaxHda& MaxHda);

std::vector<HEMAX_InputInstance*>
GetAllParameter3dsmaxInputs(HEMAX_3dsmaxHda& MaxHda);

void
UpdateParameterInputNode(HEMAX_3dsmaxHda& MaxHda, HEMAX_ParameterId ParamId);

void
ClearParameterInputNode(HEMAX_3dsmaxHda& MaxHda, HEMAX_ParameterId ParamId);

void
SetSubnetworkInput(HEMAX_3dsmaxHda& MaxHda, int Subnetwork, HEMAX_InputInstance* Input);

HEMAX_InputInstance*
FindSubnetworkInput(HEMAX_3dsmaxHda& MaxHda, int Subnetwork);

void
UpdateSubnetworkInput(HEMAX_3dsmaxHda& MaxHda, int Subnetwork);

void
ClearSubnetworkInput(HEMAX_3dsmaxHda& MaxHda, int Subnetwork);

void
UpdateMultiParameterList(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter Parameter);

static ICustAttribContainer*
GetCustAttribContainer(HEMAX_3dsmaxHda& MaxHda);

static std::unordered_map<std::string, HEMAX_ParameterAttrib*>*
GetCustAttribMap(HEMAX_3dsmaxHda& MaxHda);

std::string
GetCustAttribStringValue(HEMAX_3dsmaxHda& MaxHda, std::string CustAttribName, bool& Success);

void
InitializeParameterCustomAttributes(HEMAX_3dsmaxHda& MaxHda);

void
UpdateAllCustomAttributes(HEMAX_3dsmaxHda& MaxHda);

void
ClearParameterCustomAttributes(HEMAX_3dsmaxHda& MaxHda);

void
UpdateIntCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, std::vector<int>& IntValues);

void
UpdateFloatCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, std::vector<float>& FloatValues);

void
UpdateStringCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, std::vector<std::string>& StringValues);

void
UpdateToggleCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, std::vector<int>& ToggleValues);

void
UpdateInputNodeCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, HEMAX_Input* InputNode);

void
UpdateSubnetworkCustomAttribute(HEMAX_3dsmaxHda& MaxHda, int Subnetwork, HEMAX_Input* InputNode);

std::vector<HEMAX_SubnetworkInputMapping>
ReloadSubnetworkInputsFromCustomAttributes(HEMAX_3dsmaxHda& MaxHda);

std::vector<HEMAX_ParameterInputMapping>
ReloadParametersFromCustomAttributes(HEMAX_3dsmaxHda& MaxHda);

void
RemakeIntParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

void
RemakeStringParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

void
RemakeFloatParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

void
RemakeToggleParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

HEMAX_ParameterInputMapping
RemakeInputParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

void
RemakeMultiParameter(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

std::string
GetHardcodedHdaAssetPath(HEMAX_3dsmaxHda& MaxHda);

void
SetHardcodedHdaAssetPath(HEMAX_3dsmaxHda& MaxHda, std::string AssetPath);

void
BakeHda(HEMAX_3dsmaxHda& MaxHda);

void
CopyAllParameterValues(HEMAX_3dsmaxHda& Dest, HEMAX_3dsmaxHda& Source);