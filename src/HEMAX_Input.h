#pragma once

#include "HEMAX_Node.h"

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

    virtual ~HEMAX_Input();

protected:
    HEMAX_Input(HEMAX_InputType Type, int Id, ULONG MaxNode);
    HEMAX_Input(HEMAX_Node* EditableNode, ULONG MaxNode);
    HEMAX_Input(ULONG MaxNode);

    HEMAX_Node* Node;
    ULONG MaxNodeHandle;

    int OwnerNodeId;

    HEMAX_InputType InputNodeType;
};

HEMAX_Input* CreateOperatorNodeFromMaxNode(ULONG MaxInput, HEMAX_Parameter* Parameter);
HEMAX_Input* CreateSubnetworkInputFromMaxNode(ULONG MaxInput, HEMAX_Node* Node);