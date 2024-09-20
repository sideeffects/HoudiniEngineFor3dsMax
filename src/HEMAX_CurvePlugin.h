#pragma once

#include "HEMAX_Types.h"

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <linshape.h>
#include <surf_api.h>
#pragma warning(pop)

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
