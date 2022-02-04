#pragma once

#include "HEMAX_Hda.h"
#include "HEMAX_CustomAttributeContainer.h"
#include "HEMAX_Modifier.h"
#include "HEMAX_InputInstance.h"
#include "HEMAX_Types.h"
#include <icustattribcontainer.h>
#include <unordered_map>

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

class HEMAX_3dsmaxHda
{
    public:
	HEMAX_3dsmaxHda() = default;
	~HEMAX_3dsmaxHda() = default;

	void Cook3dsmaxHda();
	
	virtual void Update3dsmaxHda() = 0;

	void SetParameterInput(HAPI_ParmId ParamId, HEMAX_InputInstance* Input);
	HEMAX_InputInstance* FindParameterInput(HAPI_ParmId Paramid);

	std::vector<HEMAX_Parameter> GetAllParametersWithInputs();
	std::vector<HEMAX_InputInstance*> GetAllParameter3dsmaxInputs();

	void UpdateParameterInputNode(HAPI_ParmId ParamId);
	void ClearParameterInputNode(HAPI_ParmId ParamId);

	void SetSubnetworkInput(int Subnetwork, HEMAX_InputInstance* Input);
	HEMAX_InputInstance* FindSubnetworkInput(int Subnetwork);
	void UpdateSubnetworkInput(int Subnetwork);
	void ClearSubnetworkInput(int Subnetwork);
	void UpdateMultiParameterList(HEMAX_Parameter Parameter);

	std::string GetCustAttribStringValue(std::string CustAttribName, bool& Success);
	void InitializeParameterCustomAttributes();
	void UpdateAllCustomAttributes();
	void ClearParameterCustomAttributes();

	void UpdateIntCustomAttribute(HEMAX_Parameter& Parameter, std::vector<int>& IntValues);

	void UpdateFloatCustomAttribute(HEMAX_Parameter& Parameter, std::vector<float>& FloatValues);

	void UpdateStringCustomAttribute(HEMAX_Parameter& Parameter, std::vector<std::string>& StringValues);

	void UpdateToggleCustomAttribute(HEMAX_Parameter& Parameter, std::vector<int>& ToggleValues);

	void UpdateInputNodeCustomAttribute(HEMAX_Parameter& Parameter, HEMAX_Input* InputNode);

	void UpdateSubnetworkCustomAttribute(int Subnetwork, HEMAX_Input* InputNode);

	std::vector<HEMAX_SubnetworkInputMapping> ReloadSubnetworkInputsFromCustomAttributes();

	std::vector<HEMAX_ParameterInputMapping> ReloadParametersFromCustomAttributes();

	std::string GetHardcodedHdaAssetPath();

	void SetHardcodedHdaAssetPath(std::string AssetPath);

	void CopyAllParameterValues(HEMAX_3dsmaxHda& Source);

	virtual void SetCustomAttributeContainer(ICustAttribContainer* Container);

	HEMAX_HdaType Type;
	HEMAX_Hda Hda;

	std::vector<HEMAX_InputInstance*> SubnetworkNodeInputs;
	std::unordered_map<HAPI_ParmId, HEMAX_InputInstance*> InputNodeMap;

    protected:

	void InitializeSubnetworks();

	void RemakeIntParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

	void RemakeStringParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

	void RemakeFloatParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

	void RemakeToggleParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

	HEMAX_ParameterInputMapping RemakeInputParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

	void RemakeMultiParameter(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap);

	ICustAttribContainer* GetCustAttribContainer();

	std::unordered_map<std::string, HEMAX_ParameterAttrib*>* GetCustAttribMap();

	ICustAttribContainer* CustomAttributes;
	std::unordered_map<std::string, HEMAX_ParameterAttrib*> CustomAttributeMap;

};
