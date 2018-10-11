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

struct HEMAX_Parameter
{
    HEMAX_Parameter();
    HEMAX_Parameter(HEMAX_NodeId Owner, HEMAX_ParameterInfo ParameterInfo);

    HEMAX_NodeId OwnerId;
    HEMAX_ParameterInfo Info;
    HEMAX_ParameterType Type;
};

std::vector<int>
GetParameterIntValues(HEMAX_Parameter& Parameter);

std::vector<float>
GetParameterFloatValues(HEMAX_Parameter& Parameter);

std::vector<std::string>
GetParameterStringValues(HEMAX_Parameter& Parameter, bool Evaluate);

std::vector<HEMAX_ParmChoice>
GetIntParameterChoiceLists(HEMAX_Parameter& Parameter);

std::vector<HEMAX_ParmChoice>
GetStringParameterChoiceLists(HEMAX_Parameter& Parameter);

void
UpdateParameterIntValues(HEMAX_Parameter& Parameter, std::vector<int> Values);

void
UpdateParameterFloatValues(HEMAX_Parameter& Parameter, std::vector<float> Values);

void
UpdateParameterStringValues(HEMAX_Parameter& Parameter, std::vector<std::string> Values);

void
UpdateParameterInputNode(HEMAX_Parameter& Parameter, HEMAX_NodeId InputNode);

std::string
GetParameterName(HEMAX_Parameter& Parameter);

std::string
GetParameterLabel(HEMAX_Parameter& Parameter);

HEMAX_NodeId
GetParameterInputNodeId(HEMAX_Parameter& Parameter);

std::string
GetParameterInputNodeName(HEMAX_Parameter& Parameter);

std::string
GetHelpString(HEMAX_Parameter& Parameter);

bool
IsParameterChoiceList(HEMAX_Parameter& Parameter);

bool
IsRootLevelParameter(HEMAX_Parameter& Parameter);

bool
HasUIConstraints(HEMAX_Parameter& Parameter);

bool
IsMultiParameter(HEMAX_Parameter& Parameter);

int
GetMultiParameterInstancePosition(HEMAX_Parameter& Parameter);

int
GetMultiParameterStartOffset(HEMAX_Parameter& Parameter);

void 
InsertMultiParameterInstance(HEMAX_Parameter& Parameter, int Position);

void
RemoveMultiParameterInstance(HEMAX_Parameter& Parameter, int Position);

// Returns whether a recook of the node is required
// This occurs when a multiparm list parameter value is updated
// The node must be cooked so that it reflects the new number of multi-parm instances
bool
CopyParameter(HEMAX_Parameter& Source, HEMAX_Parameter& Dest);
