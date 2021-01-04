#pragma once

#include "HEMAX_Node.h"

#include <string>

class HEMAX_Input
{
    public:
	HEMAX_NodeId GetInputNodeId();
	HEMAX_NodeId GetOwnerNodeId();
	std::string GetInputNodeName();
	ULONG GetMaxNodeHandle();

	virtual void RebuildAfterChange() = 0;

	void PushMaxTransform(HEMAX_MaxTransform Transform);
	void PushMaxTransform();

	void CreateInputNode(std::string Name);

        void MarshalNodeNameDetailAttribute();

	void AddNewPart(HEMAX_PartType PartType, int FaceCount, int VertexCount, int PointCount);

	HEMAX_AttributeInfo AddNewPointAttribute(int Count, int TupleSize, std::string AttributeName);

	void SendPointAttributeData(HEMAX_AttributeInfo AttributeInfo, float* Points, int* Vertices,
		int* FaceCounts, int FaceCount, int VertexCount, int PointCount, std::string AttributeName);

	HEMAX_AttributeInfo AddNewVertexAttribute(int Count, int TupleSize, std::string AttributeName);

	HEMAX_AttributeInfo AddNewDetailFloatAttribute(int Count, int TupleSize, std::string AttributeName);

	HEMAX_AttributeInfo AddNewPrimitiveIntAttribute(int Count, int TupleSize, std::string AttributeName);

	HEMAX_AttributeInfo AddNewPrimitiveStringAttribute(int Count, int TupleSize, std::string AttributeName);

        HEMAX_AttributeInfo AddNewDetailStringAttribute(int Count, int TupleSize, std::string AttributeName);

	void SendFloatAttributeData(std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo,
		float* Data, int Length);

	void SendIntAttributeData(std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo,
		int* Data, int Length);

	void SendStringAttributeData(std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo,
		const char** Data, int Length);

	void FinalizeInputGeometry();

	virtual ~HEMAX_Input();

    protected:
	HEMAX_Input(HEMAX_InputType Type, int Id, ULONG MaxNode);
	HEMAX_Input(HEMAX_Node* EditableNode, ULONG MaxNode);
	HEMAX_Input(ULONG MaxNode);

	HEMAX_Node* Node;
	ULONG MaxNodeHandle;

	int OwnerNodeId;

	HEMAX_InputType InputNodeType;

        bool GroupMembership;
        std::string GroupHeadName;
};

HEMAX_Input* CreateOperatorNodeFromMaxNode(ULONG MaxInput, HEMAX_Parameter* Parameter);
HEMAX_Input* CreateSubnetworkInputFromMaxNode(ULONG MaxInput, HEMAX_Node* Node);
