#pragma once

#include "HEMAX_Input.h"

#pragma warning(push, 0)
#include <triobj.h>
#include <polyobj.h>
#pragma warning(pop)

class HEMAX_Input_Geometry : public HEMAX_Input
{
    public:
	HEMAX_Input_Geometry(ULONG MaxNode);
	HEMAX_Input_Geometry(HEMAX_InputType Type, int Id, ULONG MaxNode);
	HEMAX_Input_Geometry(HEMAX_InputType Type, PolyObject* MaxPolyObject, ULONG MaxNode);
	~HEMAX_Input_Geometry();

	void RebuildAfterChange() override;

    private:
	void BuildInputNode();

	void BuildPolyGeometryForInputNode(HEMAX_Node* Node, MNMesh& MaxMesh, std::string InputNodeName, HEMAX_MaxTransform NodeTransform, INode* MaxNode);
};
