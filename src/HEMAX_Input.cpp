#include "HEMAX_Input.h"
#include "HEMAX_Utilities.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"

#include "surf_api.h"
#include "triobj.h"
#include "VertexNormal.h"
#include "MeshNormalSpec.h"
#include "MNNormalSpec.h"
#include "HEMAX_Input_Transform.h"
#include "HEMAX_Input_Geometry.h"
#include "HEMAX_Input_NURBS.h"
#include "HEMAX_Input_Spline.h"

HEMAX_Input::HEMAX_Input(HEMAX_InputType Type, int Id, ULONG MaxNode)
    : MaxNodeHandle(MaxNode),
      InputNodeType(Type),
      GroupMembership(false),
      GroupHeadName("")
{
    Node = new HEMAX_Node;

    if (Type == HEMAX_INPUT_PARAMETER)
    {
	OwnerNodeId = -1;
    }
    else if (Type == HEMAX_INPUT_SUBNETWORK)
    {
	OwnerNodeId = Id;
    }
}

HEMAX_Input::HEMAX_Input(HEMAX_Node* EditableNode, ULONG MaxNode)
    : MaxNodeHandle(MaxNode),
      InputNodeType(HEMAX_INPUT_EDITABLENODE),
      Node(EditableNode),
      GroupMembership(false),
      GroupHeadName("")
{
    OwnerNodeId = EditableNode->Info.id;
}

HEMAX_Input::HEMAX_Input(ULONG MaxNode)
    : MaxNodeHandle(MaxNode),
      GroupMembership(false),
      GroupHeadName("")
{
    Node = new HEMAX_Node;
}

HEMAX_Input::~HEMAX_Input()
{
    if (Node)
    {
	delete Node;
    }
}

HEMAX_NodeId
HEMAX_Input::GetOwnerNodeId()
{
    return OwnerNodeId;
}

HEMAX_NodeId
HEMAX_Input::GetInputNodeId()
{
    return Node->Info.id;
}

ULONG
HEMAX_Input::GetMaxNodeHandle()
{
    return MaxNodeHandle;
}

std::string
HEMAX_Input::GetInputNodeName()
{
    INode* Node = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    if (Node)
    {
	std::wstring WName(Node->GetName());
	std::string RetName(WName.begin(), WName.end());

	return RetName;
    }

    return "";
}

void
HEMAX_Input::PushMaxTransform(HEMAX_MaxTransform Transform)
{
    Node->SetParentTransform(Transform);
}

void
HEMAX_Input::PushMaxTransform()
{
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    if (MaxInputNode)
    {
	Node->SetParentTransform(HEMAX_Utilities::BuildMaxTransformFromINode(MaxInputNode));
    }
}

void
HEMAX_Input::CreateInputNode(std::string Name)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    SM.Session->CreateInputNode(&Node->Info.id, Name.c_str());
    Node->Type = HEMAX_NODE_SOP;
    Node->Cook();

    INode* MaxNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);
    if (MaxNode)
    {
        if (MaxNode->IsGroupMember())
        {
            INode* ParentNode = MaxNode->GetParentNode();

            while (ParentNode && !ParentNode->IsGroupHead())
            {
                ParentNode = ParentNode->GetParentNode();
            }

            if (ParentNode && ParentNode->IsGroupHead())
            {
                std::wstring WName(ParentNode->GetName()); 
                std::string GroupName(WName.begin(), WName.end());
                
                GroupMembership = true;
                GroupHeadName = GroupName;
            }
        }
    }
}

void
HEMAX_Input::MarshalNodeNameDetailAttribute()
{
    INode* SourceNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    if (!SourceNode)
        return;

    std::wstring WNodeName(SourceNode->GetName());
    std::string NodeName(WNodeName.begin(), WNodeName.end());
    const char *NodeNameC = NodeName.c_str();

    HEMAX_AttributeInfo NodeNameAttr = AddNewDetailStringAttribute(
                                        1, 1, HEMAX_MAX_NODE_NAME);

    SendStringAttributeData(HEMAX_MAX_NODE_NAME, NodeNameAttr, &NodeNameC, 1);
}

void
HEMAX_Input::AddNewPart(HEMAX_PartType PartType, int FaceCount, int VertexCount, int PointCount)
{
    HEMAX_PartInfo NewNodePart;

    NewNodePart.id = 0;
    NewNodePart.nameSH = 0;
    NewNodePart.type = HAPI_PartType(PartType);

    NewNodePart.faceCount = FaceCount;
    NewNodePart.vertexCount = VertexCount;
    NewNodePart.pointCount = PointCount;

    NewNodePart.isInstanced = false;
    NewNodePart.instancedPartCount = 0;
    NewNodePart.instanceCount = 0;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetPartInfo(Node->Info.id, 0, &NewNodePart);

    if (GroupMembership)
    {
        SessionManager.Session->AddGroup(Node->Info.id, NewNodePart.id,
                                         HAPI_GROUPTYPE_POINT,
                                         GroupHeadName.c_str());

        std::vector<int> Membership(PointCount, 1);

        SessionManager.Session->SetGroupMembership(Node->Info.id,
                                    NewNodePart.id, HAPI_GROUPTYPE_POINT,
                                    GroupHeadName.c_str(), Membership.data(),
                                    0, PointCount);
    }
}

HEMAX_AttributeInfo
HEMAX_Input::AddNewPointAttribute(int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_FLOAT;
    NewNodeAttribute.owner = HAPI_ATTROWNER_POINT;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();
    SessionManager.Session->AddAttribute(Node->Info.id, 0, AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

void
HEMAX_Input::SendPointAttributeData(HEMAX_AttributeInfo AttributeInfo, float* Points, int* Vertices, int* FaceCounts,
		int FaceCount, int VertexCount, int PointCount, std::string AttributeName)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetAttributeFloatData(Node->Info.id, 0, AttributeName.c_str(), &AttributeInfo, Points, 0, PointCount);
    SessionManager.Session->SetVertexList(Node->Info.id, 0, Vertices, 0, VertexCount);
    SessionManager.Session->SetFaceCounts(Node->Info.id, 0, FaceCounts, 0, FaceCount);
}

HEMAX_AttributeInfo
HEMAX_Input::AddNewVertexAttribute(int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_FLOAT;
    NewNodeAttribute.owner = HAPI_ATTROWNER_VERTEX;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->AddAttribute(Node->Info.id, 0, AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

HEMAX_AttributeInfo
HEMAX_Input::AddNewDetailFloatAttribute(int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo Attr;
    Attr.count = Count;
    Attr.tupleSize = TupleSize;
    Attr.exists = true;
    Attr.storage = HAPI_STORAGETYPE_FLOAT;
    Attr.owner = HAPI_ATTROWNER_DETAIL;
    Attr.originalOwner = HAPI_ATTROWNER_INVALID;
    Attr.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    SM.Session->AddAttribute(Node->Info.id, 0, AttributeName.c_str(), &Attr);

    return Attr;
}

HEMAX_AttributeInfo
HEMAX_Input::AddNewPrimitiveIntAttribute(int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_INT;
    NewNodeAttribute.owner = HAPI_ATTROWNER_PRIM;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();
    SessionManager.Session->AddAttribute(Node->Info.id, 0, AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

HEMAX_AttributeInfo
HEMAX_Input::AddNewPrimitiveStringAttribute(int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_STRING;
    NewNodeAttribute.owner = HAPI_ATTROWNER_PRIM;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();
    SessionManager.Session->AddAttribute(Node->Info.id, 0, AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

HEMAX_AttributeInfo
HEMAX_Input::AddNewDetailStringAttribute(int Count,
                                         int TupleSize,
                                         std::string AttributeName)
{
    HEMAX_AttributeInfo Attr;
    Attr.count = Count;
    Attr.tupleSize = TupleSize;
    Attr.exists = true;
    Attr.storage = HAPI_STORAGETYPE_STRING;
    Attr.owner = HAPI_ATTROWNER_DETAIL;
    Attr.originalOwner = HAPI_ATTROWNER_INVALID;
    Attr.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& sm = HEMAX_SessionManager::GetSessionManager();
    sm.Session->AddAttribute(Node->Info.id, 0, AttributeName.c_str(), &Attr);

    return Attr;
}

void
HEMAX_Input::SendFloatAttributeData(std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo, float* Data, int Length)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetAttributeFloatData(Node->Info.id, 0, AttributeName.c_str(), &AttributeInfo, Data, 0, Length);
}

void
HEMAX_Input::SendIntAttributeData(std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo, int* Data, int Length)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetAttributeIntData(Node->Info.id, 0, AttributeName.c_str(), &AttributeInfo, Data, 0, Length);
}

void
HEMAX_Input::SendStringAttributeData(std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo, const char** Data, int Length)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetAttributeStringData(Node->Info.id, 0, AttributeName.c_str(), &AttributeInfo, Data, 0, Length);
}

void
HEMAX_Input::FinalizeInputGeometry()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    SM.Session->CommitGeometry(Node->Info.id);
}

HEMAX_Input*
CreateOperatorNodeFromMaxNode(ULONG MaxInput, HEMAX_Parameter* Parameter)
{
    HEMAX_NodeType InputNodeType = HEMAX_NodeType(Parameter->Type);
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput);
    HEMAX_Input* GeneratedInput;

    if (MaxInputNode)
    {
	bool InputNodeCreated = false;

	switch (InputNodeType)
	{
	    case HEMAX_NODE_OBJ:
	    {
		GeneratedInput = new HEMAX_Input_Transform(HEMAX_INPUT_PARAMETER, Parameter->Info.id, MaxInput);
		InputNodeCreated = true;
		break;
	    }
	    case HEMAX_NODE_ANY:
	    case HEMAX_NODE_SOP:
	    {
		INode* TheMaxNode = GetCOREInterface()->GetINodeByHandle(MaxInput);

		if (TheMaxNode)
		{
		    ObjectState MaxObjectState = TheMaxNode->EvalWorldState(0);
		    Object* MaxObject = MaxObjectState.obj;

		    if (MaxObject->CanConvertToType(Class_ID(LINEARSHAPE_CLASS_ID, 0)))
		    {
			GeneratedInput = new HEMAX_Input_Spline(HEMAX_INPUT_PARAMETER, Parameter->Info.id, MaxInput);
		    }
		    else if (MaxObject->CanConvertToType(EDITABLE_CVCURVE_CLASS_ID))
		    {
			GeneratedInput = new HEMAX_Input_NURBS(HEMAX_INPUT_PARAMETER, Parameter->Info.id, MaxInput);
		    }
		    else
		    {
			GeneratedInput = new HEMAX_Input_Geometry(HEMAX_INPUT_PARAMETER, Parameter->Info.id, MaxInput);
		    }
		    InputNodeCreated = true;
		}
		break;
	    }
	    default:
	    {
		break;
	    }
	}

	if (InputNodeCreated)
	{
	    return GeneratedInput;
	}
    }

    return nullptr;
}

HEMAX_Input*
CreateSubnetworkInputFromMaxNode(ULONG MaxInput, HEMAX_Node* Node)
{
    HEMAX_NodeType InputNodeType = Node->Type;
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput);
    HEMAX_Input* GeneratedInput = nullptr;

    if (MaxInputNode)
    {
	switch (InputNodeType)
	{
	    case HEMAX_NODE_OBJ:
	    {
		GeneratedInput = new HEMAX_Input_Transform(HEMAX_INPUT_SUBNETWORK, Node->Info.id, MaxInput);
		break;
	    }
	    case HEMAX_NODE_SOP:
	    case HEMAX_NODE_ANY:
	    {
		INode* TheMaxNode = GetCOREInterface()->GetINodeByHandle(MaxInput);

		if (TheMaxNode)
		{
		    ObjectState MaxObjectState = TheMaxNode->EvalWorldState(0);
		    Object* MaxObject = MaxObjectState.obj;

		    if (MaxObject->CanConvertToType(Class_ID(LINEARSHAPE_CLASS_ID, 0)))
		    {
			GeneratedInput = new HEMAX_Input_Spline(HEMAX_INPUT_SUBNETWORK, Node->Info.id, MaxInput);
		    }
		    else if (MaxObject->CanConvertToType(EDITABLE_CVCURVE_CLASS_ID))
		    {
			GeneratedInput = new HEMAX_Input_NURBS(HEMAX_INPUT_SUBNETWORK, Node->Info.id, MaxInput);
		    }
		    else
		    {
			GeneratedInput = new HEMAX_Input_Geometry(HEMAX_INPUT_SUBNETWORK, Node->Info.id, MaxInput);
		    }
		}
		break;
	    }
	    default:
	    {
		break;
	    }
	}
    }

    return GeneratedInput;
}
