#pragma once

#include <string>
#include <vector>

#include "HEMAX_Types.h"

struct HEMAX_ParmChoice
{
    std::string ChoiceLabel;
    std::string ChoiceValue;
};

struct HEMAX_MultiParameterChangeInfo
{
    bool Added;
    int Count;
    int Position;
};

class HEMAX_Parameter
{
    public:
	HEMAX_Parameter();
	HEMAX_Parameter(const HEMAX_NodeId& NodeId, const HEMAX_ParameterInfo& ParameterInfo);
	HEMAX_Parameter& operator=(const HEMAX_Parameter& Other);
	HEMAX_Parameter(const HEMAX_Parameter& Other);

	std::string& GetName();
	std::string& GetLabel();
	std::string& GetHelp();

	std::vector<int> GetIntVals() const;
	std::vector<float> GetFloatVals() const;
	std::vector<std::string> GetStringVals() const;

	std::vector<HEMAX_ParmChoice> GetIntParameterChoiceLists();
	std::vector<HEMAX_ParmChoice> GetStringParameterChoiceLists();

	HEMAX_NodeId GetInputNodeId();
	std::string GetInputNodeName();

	void UpdateIntVals(std::vector<int>& Vals);
	void UpdateFloatVals(std::vector<float>& Vals);
	void UpdateStringVals(std::vector<std::string>& Vals);

	void UpdateInputNode(HEMAX_NodeId InputNode);

	bool IsChoiceList();
	bool IsRootLevel();
	bool HasUIConstraints();
	bool IsMultiParameter();

	int GetInstancePosition();
	int GetInstanceStartOffset();
	void InsertInstance(int Position);
	void RemoveInstance(int Position);	

	void CopyValuesFrom(const HEMAX_Parameter& Other);

	HEMAX_NodeId Node;
	HEMAX_ParameterInfo Info;
	HEMAX_ParameterType Type;


    private:
	std::string Name;
	std::string Label;
	std::string Help;
};
