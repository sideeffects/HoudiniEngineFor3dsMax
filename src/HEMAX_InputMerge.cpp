#include "HEMAX_InputMerge.h"

HEMAX_InputMerge::HEMAX_InputMerge(HEMAX_MaxTransform Transform)
{
    MergeCount = 0;
    CreateNode(Node, HEMAX_INPUT_MERGE_ASSET_NAME, true);
    PushMaxTransform(Transform);
}

HEMAX_InputMerge::~HEMAX_InputMerge()
{
    HEMAX_Node ParentNode(Node.Info.parentId, HEMAX_NODE_NOTDEFINED);
    DeleteNode(ParentNode);
}

void
HEMAX_InputMerge::PushMaxTransform(HEMAX_MaxTransform NewTransform)
{
    SetParentTransform(Node, NewTransform);
}

void
HEMAX_InputMerge::RemoveAllMergedInputs()
{
    DisconnectInputNode(Node, MergeCount);
    MergeCount--;
}

void
HEMAX_InputMerge::MergeInput(HEMAX_Input& InputNode)
{
    ConnectInputNode(Node, InputNode.GetInputNodeId(), MergeCount);
    HEMAX_NodeId ConnectedNode = QueryNodeInput(Node, MergeCount);
    if (ConnectedNode != -1)
    {
        HEMAX_Node ObjMergeNode(ConnectedNode, HEMAX_NODE_NOTDEFINED);
        Cook(ObjMergeNode);
        HEMAX_Parameter XformParm = GetParameter(ObjMergeNode, HEMAX_INPUT_MERGE_XFORM_PARAM_NAME);

        if (XformParm.Type != HEMAX_PARAM_INVALID)
        {
            std::vector<int> IntValues = { 0 };
            UpdateParameterIntValues(XformParm, IntValues);
        }
    }

    MergeCount++;
}

HEMAX_Node&
HEMAX_InputMerge::GetMergedInputs()
{
    return Node;
}