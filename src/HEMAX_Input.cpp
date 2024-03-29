#include "HEMAX_Input.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Utilities.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"

#pragma warning(push, 0)
#include <surf_api.h>
#include <triobj.h>

#if defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024)
#include <geom/VertexNormal.h>
#else
#include <VertexNormal.h>
#endif

#include <MeshNormalSpec.h>
#include <MNNormalSpec.h>
#pragma warning(pop)

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

void
HEMAX_Input::AddNodeTransformAttributes(INode* MaxNode)
{
    HEMAX_MaxTransform NodeTM = HEMAX_Utilities::BuildMaxTransformFromINode(MaxNode);
    HAPI_Transform HAPITM = HEMAX_Utilities::MaxTransformToHAPITransform(NodeTM);
    HAPI_TransformEuler EulerTM = HEMAX_Utilities::MaxTransformToHAPITransformEuler(NodeTM);
    Matrix3 RawNodeTM = HEMAX_Utilities::GetINodeTransformationMatrix(MaxNode);

    HAPI_AttributeInfo TranslateAttrInfo = AddNewDetailFloatAttribute(1, 3, HEMAX_TRANSLATE_ATTR);
    HAPI_AttributeInfo RotateAttrInfo = AddNewDetailFloatAttribute(1, 3, HEMAX_ROTATE_ATTR);
    HAPI_AttributeInfo ScaleAttrInfo = AddNewDetailFloatAttribute(1, 3, HEMAX_SCALE_ATTR);
    HAPI_AttributeInfo QuatAttrInfo = AddNewDetailFloatAttribute(1, 4, HEMAX_QUATERNION_ATTR);
    HAPI_AttributeInfo WorldSpaceAttrInfo = AddNewDetailFloatAttribute(1, 12, HEMAX_MAX_RAW_TM_WORLD);
    HAPI_AttributeInfo LocalSpaceAttrInfo = AddNewDetailFloatAttribute(1, 12, HEMAX_MAX_RAW_TM_LOCAL);

    SendFloatAttributeData(HEMAX_TRANSLATE_ATTR, TranslateAttrInfo, EulerTM.position, 1);
    SendFloatAttributeData(HEMAX_ROTATE_ATTR, RotateAttrInfo, EulerTM.rotationEuler, 1);
    SendFloatAttributeData(HEMAX_SCALE_ATTR, ScaleAttrInfo, EulerTM.scale, 1);
    SendFloatAttributeData(HEMAX_QUATERNION_ATTR, QuatAttrInfo, HAPITM.rotationQuaternion, 1);

    std::vector<float> WorldSpaceTM;
    HEMAX_Utilities::Matrix3ToFlatArray(RawNodeTM, WorldSpaceTM);

    SendFloatAttributeData(HEMAX_MAX_RAW_TM_WORLD, WorldSpaceAttrInfo, &WorldSpaceTM.front(), 1);

    Matrix3 NodeLocalTM = HEMAX_Utilities::GetINodeLocalTransformationMatrix(MaxNode);
    std::vector<float> LocalSpaceTM;
    HEMAX_Utilities::Matrix3ToFlatArray(NodeLocalTM, LocalSpaceTM);

    SendFloatAttributeData(HEMAX_MAX_RAW_TM_LOCAL, LocalSpaceAttrInfo, &LocalSpaceTM.front(), 1);
}

HEMAX_Input::~HEMAX_Input()
{
    if (Node)
    {
	delete Node;
    }
}

HAPI_NodeId
HEMAX_Input::GetOwnerNodeId()
{
    return OwnerNodeId;
}

HAPI_NodeId
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
	return HEMAX_Utilities::WideStringToStringUnsafe(Node->GetName());
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
    HEMAX_HoudiniApi::CreateInputNode(SM.Session, &Node->Info.id, Name.c_str());
    Node->Type = HAPI_NODETYPE_SOP;
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
                GroupMembership = true;
                GroupHeadName = HEMAX_Utilities::WideStringToStringUnsafe(
                                                        ParentNode->GetName()); 
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

    std::string NodeName = HEMAX_Utilities::WideStringToStringUnsafe(
                                                    SourceNode->GetName());
    const char *NodeNameC = NodeName.c_str();

    HAPI_AttributeInfo NodeNameAttr = AddNewDetailStringAttribute(
                                        1, 1, HEMAX_MAX_NODE_NAME);

    SendStringAttributeData(HEMAX_MAX_NODE_NAME, NodeNameAttr, &NodeNameC, 1);
}

void
HEMAX_Input::AddNewPart(HAPI_PartType PartType, int FaceCount, int VertexCount,
                        int PointCount)
{
    HAPI_PartInfo NewNodePart;

    NewNodePart.id = 0;
    NewNodePart.nameSH = 0;
    NewNodePart.type = HAPI_PartType(PartType);

    NewNodePart.faceCount = FaceCount;
    NewNodePart.vertexCount = VertexCount;
    NewNodePart.pointCount = PointCount;

    NewNodePart.isInstanced = false;
    NewNodePart.instancedPartCount = 0;
    NewNodePart.instanceCount = 0;

    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::SetPartInfo(SessionManager.Session, Node->Info.id,
        0, &NewNodePart);

    if (GroupMembership)
    {
        HEMAX_HoudiniApi::AddGroup(SessionManager.Session, Node->Info.id,
            NewNodePart.id, HAPI_GROUPTYPE_POINT, GroupHeadName.c_str());

        std::vector<int> Membership(PointCount, 1);

        HEMAX_HoudiniApi::SetGroupMembership(SessionManager.Session,
            Node->Info.id, NewNodePart.id, HAPI_GROUPTYPE_POINT,
            GroupHeadName.c_str(), Membership.data(), 0, PointCount);
    }
}

HAPI_AttributeInfo
HEMAX_Input::AddNewPointAttribute(int Count, int TupleSize,
                                  std::string AttributeName,
                                  HAPI_StorageType StorageType)
{
    HAPI_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = StorageType;
    NewNodeAttribute.owner = HAPI_ATTROWNER_POINT;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::AddAttribute(SessionManager.Session, Node->Info.id, 0,
            AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

void
HEMAX_Input::SendPointAttributeData(HAPI_AttributeInfo AttributeInfo,
                float* Points, int* Vertices, int* FaceCounts,
		int FaceCount, int VertexCount, int PointCount,
                std::string AttributeName)
{
    HEMAX_SessionManager& SM =
        HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::SetAttributeFloatData(SM.Session, Node->Info.id, 0,
            AttributeName.c_str(), &AttributeInfo, Points, 0, PointCount);
    HEMAX_HoudiniApi::SetVertexList(SM.Session, Node->Info.id, 0, Vertices, 0,
            VertexCount);
    HEMAX_HoudiniApi::SetFaceCounts(SM.Session, Node->Info.id, 0, FaceCounts, 0,
            FaceCount);
}

HAPI_AttributeInfo
HEMAX_Input::AddNewPointIntAttribute(int Count, int TupleSize,
    std::string AttributeName)
{
    HAPI_AttributeInfo NewAttr;
    NewAttr.count = Count;
    NewAttr.tupleSize = TupleSize;
    NewAttr.exists = true;
    NewAttr.storage = HAPI_STORAGETYPE_INT;
    NewAttr.owner = HAPI_ATTROWNER_POINT;
    NewAttr.originalOwner = HAPI_ATTROWNER_POINT;
    NewAttr.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::AddAttribute(SM.Session, Node->Info.id, 0,
        AttributeName.c_str(), &NewAttr);

    return NewAttr;
}

HAPI_AttributeInfo
HEMAX_Input::AddNewVertexAttribute(int Count, int TupleSize,
                std::string AttributeName)
{
    HAPI_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_FLOAT;
    NewNodeAttribute.owner = HAPI_ATTROWNER_VERTEX;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::AddAttribute(SessionManager.Session, Node->Info.id, 0,
            AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

HAPI_AttributeInfo
HEMAX_Input::AddNewDetailFloatAttribute(int Count, int TupleSize,
                std::string AttributeName)
{
    HAPI_AttributeInfo Attr;
    Attr.count = Count;
    Attr.tupleSize = TupleSize;
    Attr.exists = true;
    Attr.storage = HAPI_STORAGETYPE_FLOAT;
    Attr.owner = HAPI_ATTROWNER_DETAIL;
    Attr.originalOwner = HAPI_ATTROWNER_INVALID;
    Attr.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::AddAttribute(SM.Session, Node->Info.id, 0,
        AttributeName.c_str(), &Attr);

    return Attr;
}

HAPI_AttributeInfo
HEMAX_Input::AddNewPrimitiveIntAttribute(int Count, int TupleSize,
                std::string AttributeName)
{
    HAPI_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_INT;
    NewNodeAttribute.owner = HAPI_ATTROWNER_PRIM;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::AddAttribute(SessionManager.Session, Node->Info.id, 0,
            AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

HAPI_AttributeInfo
HEMAX_Input::AddNewPrimitiveStringAttribute(int Count, int TupleSize,
                std::string AttributeName)
{
    HAPI_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_STRING;
    NewNodeAttribute.owner = HAPI_ATTROWNER_PRIM;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::AddAttribute(SessionManager.Session, Node->Info.id, 0,
            AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

HAPI_AttributeInfo
HEMAX_Input::AddNewDetailStringAttribute(int Count,
                                         int TupleSize,
                                         std::string AttributeName)
{
    HAPI_AttributeInfo Attr;
    Attr.count = Count;
    Attr.tupleSize = TupleSize;
    Attr.exists = true;
    Attr.storage = HAPI_STORAGETYPE_STRING;
    Attr.owner = HAPI_ATTROWNER_DETAIL;
    Attr.originalOwner = HAPI_ATTROWNER_INVALID;
    Attr.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& sm = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::AddAttribute(sm.Session, Node->Info.id, 0,
        AttributeName.c_str(), &Attr);

    return Attr;
}

HAPI_AttributeInfo
HEMAX_Input::AddNewDetailIntAttribute(int Count, int TupleSize,
    std::string AttributeName)
{
    HAPI_AttributeInfo Attr;
    Attr.count = Count;
    Attr.tupleSize = TupleSize;
    Attr.exists = true;
    Attr.storage = HAPI_STORAGETYPE_INT;
    Attr.owner = HAPI_ATTROWNER_DETAIL;
    Attr.originalOwner = HAPI_ATTROWNER_DETAIL;
    Attr.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& sm = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::AddAttribute(sm.Session, Node->Info.id, 0,
        AttributeName.c_str(), &Attr);

    return Attr;
}

void
HEMAX_Input::SendFloatAttributeData(std::string AttributeName,
                HAPI_AttributeInfo& AttributeInfo, float* Data, int Length)
{
    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::SetAttributeFloatData(SessionManager.Session,
            Node->Info.id, 0, AttributeName.c_str(), &AttributeInfo, Data,
            0, Length);
}

void
HEMAX_Input::SendIntAttributeData(std::string AttributeName,
                HAPI_AttributeInfo& AttributeInfo, int* Data, int Length)
{
    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::SetAttributeIntData(SessionManager.Session, Node->Info.id,
            0, AttributeName.c_str(), &AttributeInfo, Data, 0, Length);
}

void
HEMAX_Input::SendStringAttributeData(std::string AttributeName,
                HAPI_AttributeInfo& AttributeInfo, const char** Data,
                int Length)
{
    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::SetAttributeStringData(SessionManager.Session,
            Node->Info.id, 0, AttributeName.c_str(), &AttributeInfo, Data, 0,
            Length);
}

void
HEMAX_Input::FinalizeInputGeometry()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::CommitGeo(SM.Session, Node->Info.id);
}

HEMAX_Input*
CreateOperatorNodeFromMaxNode(ULONG MaxInput, HEMAX_Parameter* Parameter)
{
    HAPI_NodeType InputNodeType = HAPI_NodeType(Parameter->Type);
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput);
    HEMAX_Input* GeneratedInput;

    if (MaxInputNode)
    {
	bool InputNodeCreated = false;

	switch (InputNodeType)
	{
	    case HAPI_NODETYPE_OBJ:
	    {
		GeneratedInput = new HEMAX_Input_Transform(
                        HEMAX_INPUT_PARAMETER, Parameter->Info.id, MaxInput);
		InputNodeCreated = true;
		break;
	    }
	    case HAPI_NODETYPE_ANY:
	    case HAPI_NODETYPE_SOP:
	    {
		INode* TheMaxNode =
                    GetCOREInterface()->GetINodeByHandle(MaxInput);

		if (TheMaxNode)
		{
		    ObjectState MaxObjectState = TheMaxNode->EvalWorldState(0);
		    Object* MaxObject = MaxObjectState.obj;

		    if (MaxObject->CanConvertToType(Class_ID(LINEARSHAPE_CLASS_ID, 0)))
		    {
			GeneratedInput = new HEMAX_Input_Spline(
                                            HEMAX_INPUT_PARAMETER,
                                            Parameter->Info.id, MaxInput);
		    }
		    else if (MaxObject->CanConvertToType(EDITABLE_CVCURVE_CLASS_ID))
		    {
			GeneratedInput = new HEMAX_Input_NURBS(
                                            HEMAX_INPUT_PARAMETER,
                                            Parameter->Info.id, MaxInput);
		    }
		    else
		    {
			GeneratedInput = new HEMAX_Input_Geometry(
                                            HEMAX_INPUT_PARAMETER,
                                            Parameter->Info.id, MaxInput);
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
    HAPI_NodeType InputNodeType = Node->Type;
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput);
    HEMAX_Input* GeneratedInput = nullptr;

    if (MaxInputNode)
    {
	switch (InputNodeType)
	{
	    case HAPI_NODETYPE_OBJ:
	    {
		GeneratedInput = new HEMAX_Input_Transform(
                        HEMAX_INPUT_SUBNETWORK, Node->Info.id, MaxInput);
		break;
	    }
	    case HAPI_NODETYPE_SOP:
	    case HAPI_NODETYPE_ANY:
	    {
		INode* TheMaxNode =
                    GetCOREInterface()->GetINodeByHandle(MaxInput);

		if (TheMaxNode)
		{
		    ObjectState MaxObjectState = TheMaxNode->EvalWorldState(0);
		    Object* MaxObject = MaxObjectState.obj;

		    if (MaxObject->CanConvertToType(Class_ID(LINEARSHAPE_CLASS_ID, 0)))
		    {
			GeneratedInput = new HEMAX_Input_Spline(
                            HEMAX_INPUT_SUBNETWORK, Node->Info.id, MaxInput);
		    }
		    else if (MaxObject->CanConvertToType(EDITABLE_CVCURVE_CLASS_ID))
		    {
			GeneratedInput = new HEMAX_Input_NURBS(
                            HEMAX_INPUT_SUBNETWORK, Node->Info.id, MaxInput);
		    }
		    else
		    {
			GeneratedInput = new HEMAX_Input_Geometry(
                            HEMAX_INPUT_SUBNETWORK, Node->Info.id, MaxInput);
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
