#include "HEMAX_InputMerge.h"

HEMAX_InputMerge::HEMAX_InputMerge(HEMAX_MaxTransform Transform)
{
    MergeCount = 0;
    Node.Init(HEMAX_INPUT_MERGE_ASSET_NAME);
    Node.Cook();
    PushMaxTransform(Transform);
}

HEMAX_InputMerge::~HEMAX_InputMerge()
{
    HEMAX_Node ParentNode(Node.Info.parentId, HAPI_NODETYPE_NONE);
    ParentNode.Delete();
}

void
HEMAX_InputMerge::PushMaxTransform(HEMAX_MaxTransform NewTransform)
{
    Node.SetParentTransform(NewTransform);
}

void
HEMAX_InputMerge::RemoveAllMergedInputs()
{
    Node.DisconnectInputNode(MergeCount);
    MergeCount--;
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
	HEMAX_Parameter* XformParm = ObjMergeNode.GetParameter(HEMAX_INPUT_MERGE_XFORM_PARAM_NAME);

	if (XformParm)
	{
	    std::vector<int> IntValues = { 0 };
	    XformParm->UpdateIntVals(IntValues);
	}
    }

    MergeCount++;
}

HEMAX_Node&
HEMAX_InputMerge::GetMergedInputs()
{
    return Node;
}
