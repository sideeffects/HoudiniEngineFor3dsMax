#pragma once

#include "HEMAX_HAPISession.h"
#include "HEMAX_Parameter.h"
#include "HEMAX_Utilities.h"
#include "HEMAX_Types.h"

#include <string>
#include <unordered_map>

struct HEMAX_Node
{
    HEMAX_Node();
    HEMAX_Node(HEMAX_NodeId NodeId, HEMAX_NodeType NodeType);

    HEMAX_NodeInfo Info;
    HEMAX_NodeType Type;

    std::string AssetName;

    bool RecookRequired;

    bool AutoRecookOnParameterUpdate;
    bool RealtimeRecookEnabled;
    bool AutoRecookOnInputChange;
};

void
Cook(HEMAX_Node& Node);

void
CreateNode(HEMAX_Node& Node, std::string AssetName, bool CookOnCreation);

void
CreateInputNode(HEMAX_Node& Node, std::string Name);

void
DeleteNode(HEMAX_Node& Node);

std::string
GetNodeName(HEMAX_Node& Node);

std::vector<HEMAX_Parameter>
GetAllParameters(HEMAX_Node& Node);

HEMAX_Parameter
GetParameter(HEMAX_Node& Node, HEMAX_ParameterId ParameterId);

HEMAX_Parameter
GetParameter(HEMAX_Node& Node, std::string ParameterName);

std::vector<HEMAX_Parameter>
GetAllMultiParameterInstances(HEMAX_Node& Node, int MultiParameterListId);

void
SetObjectTransform(HEMAX_Node& Node, HEMAX_MaxTransform& NewTransform);

void
SetParentTransform(HEMAX_Node& Node, HEMAX_MaxTransform& NewTransform);

std::vector<HEMAX_NodeId>
GetConnectedInputNodeIds(HEMAX_Node& Node);

std::vector<std::string>
GetConnectedNodeNames(HEMAX_Node& Node);

void
ConnectInputNode(HEMAX_Node& Node, HEMAX_NodeId NodeToConnect, int InputIndex);

void
DisconnectInputNode(HEMAX_Node& Node, int InputIndex);

HEMAX_NodeId
QueryNodeInput(HEMAX_Node& Node, int InputIndex);

void
AddNewPart(HEMAX_Node& Node, HEMAX_PartType PartType, int FaceCount, int VertexCount, int PointCount);

HEMAX_AttributeInfo
AddNewPointAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName);

void
SendPointAttributeData(HEMAX_Node& Node, HEMAX_AttributeInfo AttributeInfo, float* Points, int* Vertices, int* FaceCounts,
                        int FaceCount, int VertexCount, int PointCount, std::string AttributeName);

HEMAX_AttributeInfo
AddNewVertexAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName);

HEMAX_AttributeInfo
AddNewDetailFloatAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName);

HEMAX_AttributeInfo
AddNewPrimitiveIntAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName);

HEMAX_AttributeInfo
AddNewPrimitiveStringAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName);

void
SendFloatAttributeData(HEMAX_Node& Node, std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo, float* Data, int Length);

void
SendIntAttributeData(HEMAX_Node& Node, std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo, int* Data, int Length);

void
SendStringAttributeData(HEMAX_Node& Node, std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo, const char** Data, int Length);

void
FinalizeInputGeometry(HEMAX_Node& Node);

std::vector<HEMAX_Node>
GetAllChildNodes(HEMAX_Node& Node);

std::vector<HEMAX_Node>
GetImmediateChildNodes(HEMAX_Node& Node);
