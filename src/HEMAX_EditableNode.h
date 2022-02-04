#pragma once

#include "HEMAX_Types.h"
#include "HEMAX_Node.h"
#include "HEMAX_Part.h"
#include <inode.h>
#include <vector>

const char* const EditableNodeCurveClosedParm = "close";

struct HEMAX_EditableNode
{
    HEMAX_Node Node;
    HAPI_GeoInfo GeoInfo;
    std::vector<HEMAX_Part> Parts;
};

struct HEMAX_EditableCurve
{
    INode* Node;
    HAPI_CurveInfo CurveInfo;
    std::vector<float> Knots;
    bool Dirty;

    HAPI_NodeId PushNodeId;
    HAPI_PartId PushPartId;
};

INode*
MarshallDataInto3dsMaxLinearCurve(HEMAX_EditableCurve& EditableCurve, HEMAX_EditableNode& EditableNode, int PartNum);

INode*
MarshallDataInto3dsMaxNURBSCVCurve(HEMAX_EditableCurve& EditableCurve, HEMAX_EditableNode& EditableNode, int PartNum);
