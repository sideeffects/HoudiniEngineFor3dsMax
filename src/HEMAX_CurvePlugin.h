#pragma once

#include "HEMAX_Types.h"

#include "linshape.h"
#include "surf_api.h"

class HEMAX_CurvePlugin
{
    public:
	HEMAX_CurvePlugin();
	~HEMAX_CurvePlugin() = default;

	void SetPart(HEMAX_NodeId _NodeId, HEMAX_PartId _PartId, HEMAX_CurveInfo _CurveInfo);

	void SetLinearShape(LinearShape* _Shape);
	void SetNurbsCurve(INode* Node);

	INode* GetINode();

	bool BuildCurve();

    private:

	bool BuildLinearShape();
	bool BuildNURBSObject();

	HEMAX_NodeId NodeId;
	HEMAX_PartId PartId;
	HEMAX_CurveInfo CurveInfo;

	LinearShape* Shape;

	INode* NurbsCurveNode;
	NURBSSet CurveSet;
};
