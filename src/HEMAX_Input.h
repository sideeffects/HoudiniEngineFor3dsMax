#pragma once

#include "HEMAX_Node.h"

#include <string>

class HEMAX_Input
{
    public:
	HAPI_NodeId GetInputNodeId();
	HAPI_NodeId GetOwnerNodeId();
	std::string GetInputNodeName();
	ULONG GetMaxNodeHandle();

	virtual void RebuildAfterChange() = 0;

	void PushMaxTransform(HEMAX_MaxTransform Transform);
	void PushMaxTransform();

	void CreateInputNode(std::string Name);

        void MarshalNodeNameDetailAttribute();

	void AddNewPart(HAPI_PartType PartType, int FaceCount, int VertexCount,
                int PointCount);

	HAPI_AttributeInfo AddNewPointAttribute(int Count, int TupleSize,
                std::string AttributeName,
                HAPI_StorageType StorageType = HAPI_STORAGETYPE_FLOAT);

	void SendPointAttributeData(HAPI_AttributeInfo AttributeInfo,
                float* Points, int* Vertices, int* FaceCounts, int FaceCount,
                int VertexCount, int PointCount, std::string AttributeName);

        HAPI_AttributeInfo AddNewPointIntAttribute(int Count, int TupleSize,
                std::string AttributeName);

	HAPI_AttributeInfo AddNewVertexAttribute(int Count, int TupleSize,
                std::string AttributeName);

	HAPI_AttributeInfo AddNewDetailFloatAttribute(int Count, int TupleSize,
                std::string AttributeName);

	HAPI_AttributeInfo AddNewPrimitiveIntAttribute(int Count,
                int TupleSize, std::string AttributeName);

	HAPI_AttributeInfo AddNewPrimitiveStringAttribute(int Count,
                int TupleSize, std::string AttributeName);

        HAPI_AttributeInfo AddNewDetailStringAttribute(int Count,
                int TupleSize, std::string AttributeName);

        HAPI_AttributeInfo AddNewDetailIntAttribute(int Count, int TupleSize,
            std::string AttributeName);

	void SendFloatAttributeData(std::string AttributeName,
                HAPI_AttributeInfo& AttributeInfo, float* Data, int Length);

	void SendIntAttributeData(std::string AttributeName,
                HAPI_AttributeInfo& AttributeInfo, int* Data, int Length);

	void SendStringAttributeData(std::string AttributeName,
                HAPI_AttributeInfo& AttributeInfo, const char** Data,
                int Length);

	void FinalizeInputGeometry();

	virtual ~HEMAX_Input();

    protected:
	HEMAX_Input(HEMAX_InputType Type, int Id, ULONG MaxNode);
	HEMAX_Input(HEMAX_Node* EditableNode, ULONG MaxNode);
	HEMAX_Input(ULONG MaxNode);

        void AddNodeTransformAttributes(INode* MaxNode);

	HEMAX_Node* Node;
	ULONG MaxNodeHandle;

	int OwnerNodeId;

	HEMAX_InputType InputNodeType;

        bool GroupMembership;
        std::string GroupHeadName;
};

HEMAX_Input* CreateOperatorNodeFromMaxNode(ULONG MaxInput,
        HEMAX_Parameter* Parameter);
HEMAX_Input* CreateSubnetworkInputFromMaxNode(ULONG MaxInput,
        HEMAX_Node* Node);
