#pragma once

#include "HEMAX_Hda.h"
#include "HEMAX_CustomAttributeContainer.h"
#include "HEMAX_Modifier.h"
#include "HEMAX_InputInstance.h"
#include "HEMAX_Types.h"

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <icustattribcontainer.h>
#include <maxstring.h>
#include <strbasic.h>
#pragma warning(pop)

#include <unordered_map>
#include <vector>

struct HEMAX_SubnetworkInputMapping
{
    int Subnetwork;
    std::vector<INode*> Nodes;
};

struct HEMAX_ParameterInputMapping
{
    std::string ParameterName = "";
    std::vector<INode*> Nodes;
};

class HEMAX_3dsmaxHda
{
    public:
	HEMAX_3dsmaxHda() = default;
	virtual ~HEMAX_3dsmaxHda() = default;

	void Cook3dsmaxHda();
	
	virtual void Update3dsmaxHda() = 0;

	void SetParameterInput(HAPI_ParmId ParamId, HEMAX_InputInstance* Input);
        HEMAX_InputInstance* InitParameterInput(HEMAX_Parameter& Parm);
	HEMAX_InputInstance* FindParameterInput(const HEMAX_Parameter& Parm);
        void ClearParameterInput(HEMAX_Parameter* Parameter);

	std::vector<HEMAX_Parameter> GetAllParametersWithInputs();
	std::vector<HEMAX_InputInstance*> GetAllParameter3dsmaxInputs();

	void UpdateParameterInputNode(HEMAX_Parameter& Parm);

        void ResetParameters();

        bool HasSubnetworkInput(int Subnetwork);
        HEMAX_InputInstance* GetSubnetworkInput(int Subnetwork);
	void UpdateSubnetworkInput(int Subnetwork);
	void ClearSubnetworkInput(int Subnetwork);

        void RemoveAllUsages(HEMAX_3dsMaxInput* Input);

        std::unordered_map<HAPI_ParmId, HEMAX_InputInstance>& GetInputNodeMap()
            { return InputNodeMap; }

        void RefreshInputConnections();

	void UpdateMultiParameterList(HEMAX_Parameter Parameter);

	std::string GetCustAttribStringValue(const TSTR& CustAttribName,
                bool& Success);

	void InitializeParameterCustomAttributes();
	void UpdateAllCustomAttributes();
	void ClearParameterCustomAttributes();

        virtual M_STD_STRING Get3dsMaxContainerName() = 0;

protected:

        virtual HEMAX_NodeListParameterAttrib* CreateNodeListCustAttrib(
                int Subnetwork) = 0;

public:

	void UpdateIntCustomAttribute(
                HEMAX_Parameter& Parameter, std::vector<int>& IntValues);
	void UpdateFloatCustomAttribute(
                HEMAX_Parameter& Parameter, std::vector<float>& FloatValues);
	void UpdateStringCustomAttribute(
                HEMAX_Parameter& Parameter,
                std::vector<std::string>& StringValues);
	void UpdateToggleCustomAttribute(
                HEMAX_Parameter& Parameter, std::vector<int>& ToggleValues);
	void UpdateInputNodeCustomAttribute(
                HEMAX_Parameter& Parameter,
                const INodeTab& InputNodes);
        void UpdateSubnetworkCustomAttribute(
                int Subnetwork,
                const INodeTab& InputNodes);

	std::vector<HEMAX_SubnetworkInputMapping>
            ReloadSubnetworkInputsFromCustomAttributes();
	std::vector<HEMAX_ParameterInputMapping> 
            ReloadParametersFromCustomAttributes();

	std::string GetHardcodedHdaAssetPath();

	void SetHardcodedHdaAssetPath(std::string AssetPath);

	void CopyAllParameterValues(HEMAX_3dsmaxHda& Source);

	virtual void SetCustomAttributeContainer(ICustAttribContainer* Container);

	HEMAX_HdaType Type;
	HEMAX_Hda Hda;

    protected:

        std::unordered_map<int, HEMAX_InputInstance> SubnetworkNodeInputs;
	std::unordered_map<HAPI_ParmId, HEMAX_InputInstance> InputNodeMap;

	void RemakeIntParameterFromCustAttrib(HEMAX_Parameter Parameter);
	void RemakeStringParameterFromCustAttrib(HEMAX_Parameter Parameter);
	void RemakeFloatParameterFromCustAttrib(HEMAX_Parameter Parameter);
	void RemakeToggleParameterFromCustAttrib(HEMAX_Parameter Parameter);
	HEMAX_ParameterInputMapping RemakeInputParameterFromCustAttrib(
                HEMAX_Parameter Parameter);
	void RemakeMultiParameter(HEMAX_Parameter Parameter);

	ICustAttribContainer* GetCustAttribContainer()
                              { return CustomAttributes; }

#ifdef UNICODE
	std::unordered_map<std::wstring, HEMAX_ParameterAttrib*>*
                GetCustAttribMap() { return &CustomAttributeMap; }
#else
	std::unordered_map<std::string, HEMAX_ParameterAttrib*>*
                GetCustAttribMap() { return &CustomAttributeMap; }
#endif

	ICustAttribContainer* CustomAttributes;

#ifdef UNICODE
        std::unordered_map<std::wstring, HEMAX_ParameterAttrib*> CustomAttributeMap;
#else
        std::unorderd_map<std::string, HEMAX_ParameterAttrib*> CustomAttributeMap;
#endif

};
