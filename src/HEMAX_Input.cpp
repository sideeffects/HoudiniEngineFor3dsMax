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

HEMAX_Input::HEMAX_Input(HEMAX_InputType Type, int Id, ULONG MaxNode) :
    MaxNodeHandle(MaxNode),
    InputNodeType(Type)
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
    : MaxNodeHandle(MaxNode), InputNodeType(HEMAX_INPUT_EDITABLENODE), Node(EditableNode)
{
    OwnerNodeId = EditableNode->Info.id;
}

HEMAX_Input::HEMAX_Input(ULONG MaxNode)
    : MaxNodeHandle(MaxNode)
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
    SetParentTransform(*Node, Transform);
}

void
HEMAX_Input::PushMaxTransform()
{
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    if (MaxInputNode)
    {
        SetParentTransform(*Node, HEMAX_Utilities::BuildMaxTransformFromINode(MaxInputNode));
    }
}