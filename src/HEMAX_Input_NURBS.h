#pragma once

#include "HEMAX_Input.h"

#include "surf_api.h"

class HEMAX_Input_NURBS : public HEMAX_Input
{
public:
    HEMAX_Input_NURBS(ULONG MaxNode);
    HEMAX_Input_NURBS(HEMAX_InputType Type, int Id, ULONG MaxNode);
    HEMAX_Input_NURBS(HEMAX_Node* EditableNode, ULONG MaxNode);
    HEMAX_Input_NURBS(HEMAX_InputType Type, Object* MaxObject);
    ~HEMAX_Input_NURBS();

    void RebuildAfterChange() override;

    void BuildCurveForInputNode(HEMAX_Node* Node, NURBSObject* CurveObj, std::string InputNodeName);
    void BuildCurveForEditableNode(HEMAX_Node* Node, NURBSObject* CurveObj, std::string InputNodeName);

    std::vector<float>& GetKnotsArray();

private:
    void BuildInputNode();

    void BuildPointCurveForInputNode(HEMAX_Node* Node, std::vector<NURBSPoint*>& NURBSPoints, std::string InputNodeName);

    std::vector<float> Knots;
};