#pragma once

#include "HEMAX_Input.h"

#include "linshape.h"

class HEMAX_Input_Spline : public HEMAX_Input
{
    public:
	HEMAX_Input_Spline(ULONG MaxNode);
	HEMAX_Input_Spline(HEMAX_InputType Type, int Id, ULONG MaxNode);
	HEMAX_Input_Spline(HEMAX_InputType Type, LinearShape* TheShape, ULONG MaxNode);
	HEMAX_Input_Spline(HEMAX_Node* EditableNode, ULONG MaxNode);
	~HEMAX_Input_Spline();

	void RebuildAfterChange() override;

    private:
	void BuildInputNode();

	void BuildLinearCurveForInputNode(HEMAX_Node* Node, LinearShape* TheShape, std::string InputNodeName);
	void BuildLinearCurveForEditableNode(HEMAX_Node* Node, LinearShape* TheShape, std::string InputNodeName);
};
