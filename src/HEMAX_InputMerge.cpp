#include "HEMAX_InputMerge.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_SessionManager.h"

HEMAX_InputMerge::HEMAX_InputMerge()
{
    Node.Init(HEMAX_INPUT_MERGE_ASSET_NAME);
    Node.Cook();
}

HEMAX_InputMerge::~HEMAX_InputMerge()
{
    HEMAX_Node ParentNode(Node.Info.parentId, HAPI_NODETYPE_NONE);
    ParentNode.Delete();
}

void
HEMAX_InputMerge::MergeInput(HEMAX_Input& InputNode)
{
    Node.ConnectInputNode(InputNode.GetInputNodeId(), MergeCount);
    HAPI_NodeId ConnectedNode = Node.QueryNodeInput(MergeCount);
    if (ConnectedNode != -1)
    {
	HEMAX_Node ObjMergeNode(ConnectedNode, HAPI_NODETYPE_SOP);
	ObjMergeNode.Cook();
	HEMAX_Parameter* XformParm = ObjMergeNode.GetParameter(
            HEMAX_INPUT_MERGE_XFORM_PARAM_NAME);

	if (XformParm)
	{
            // Parameter Transform=Into This Object
	    std::vector<int> IntValues = {1};
	    XformParm->UpdateIntVals(IntValues);
	}
    }

    MergeCount++;
}

void
HEMAX_InputMerge::MergeInputRelativeTo(
        HEMAX_Input& InputNode,
        const HEMAX_MaxTransform& MaxTransform)
{
    HAPI_NodeId XformNodeId = -1;
    HEMAX_HoudiniApi::CreateNode(
        HEMAX_SessionManager::GetSessionManager().Session, Node.Info.parentId,
        "xform", nullptr, false, &XformNodeId);
    HAPI_TransformEuler EulerXform =
       HEMAX_Utilities::MaxTransformToHAPITransformEuler(MaxTransform,
            HAPI_SRT); 

    HAPI_Session* Session =
        HEMAX_SessionManager::GetSessionManager().Session;

    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "tx", 0, EulerXform.position[0]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "ty", 1, EulerXform.position[1]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "tz", 2, EulerXform.position[2]);

    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "rx", 0, EulerXform.rotationEuler[0]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "ry", 1, EulerXform.rotationEuler[1]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "rz", 2, EulerXform.rotationEuler[2]);

    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "sx", 0, EulerXform.scale[0]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "sy", 1, EulerXform.scale[1]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "sz", 2, EulerXform.scale[2]);

    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "shear1", 0, EulerXform.shear[0]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "shear2", 1, EulerXform.shear[1]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "shear3", 2, EulerXform.shear[2]);

    HEMAX_HoudiniApi::ConnectNodeInput(
        HEMAX_SessionManager::GetSessionManager().Session, Node.Info.id,
        MergeCount, XformNodeId, 0);
    HEMAX_HoudiniApi::ConnectNodeInput(
        HEMAX_SessionManager::GetSessionManager().Session, XformNodeId,
        0, InputNode.GetInputNodeId(), 0);

    HAPI_NodeId ConnectedNode = -1;
    HEMAX_HoudiniApi::QueryNodeInput(
        HEMAX_SessionManager::GetSessionManager().Session, XformNodeId,
        0, &ConnectedNode);

    HEMAX_Node ObjMergeNode(ConnectedNode, HAPI_NODETYPE_SOP);
    ObjMergeNode.Cook();
    HEMAX_Parameter* XformParm = ObjMergeNode.GetParameter(
        HEMAX_INPUT_MERGE_XFORM_PARAM_NAME);

    if (XformParm)
    {
        std::vector<int> IntValues = {0};
        XformParm->UpdateIntVals(IntValues);
    }

    MergeCount++;
}

void
HEMAX_InputMerge::RemoveAllMergedInputs()
{
    for (int i = MergeCount - 1; i >= 0; --i)
    {
        Node.DisconnectInputNode(i);
    }

    MergeCount = 0;
}

HEMAX_Node&
HEMAX_InputMerge::GetMergedInputs()
{
    return Node;
}
