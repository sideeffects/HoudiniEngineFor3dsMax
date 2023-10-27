#pragma once

#include "HEMAX_Types.h"

#include "linshape.h"
#include "surf_api.h"

class HEMAX_CurvePlugin
{
    public:
	HEMAX_CurvePlugin();
	~HEMAX_CurvePlugin() = default;

	void SetPart(HAPI_NodeId _NodeId, HAPI_PartId _PartId, HAPI_CurveInfo _CurveInfo);

        void SetCurveNode(INode* Node);
	INode* GetINode();

	bool BuildCurve();

    private:

	bool BuildLinearShape();
	bool BuildNURBSObject();

	HAPI_NodeId NodeId;
	HAPI_PartId PartId;
	HAPI_CurveInfo CurveInfo;

	INode* CurveNode;

	LinearShape* Shape;
        NURBSSet CurveSet;
};
