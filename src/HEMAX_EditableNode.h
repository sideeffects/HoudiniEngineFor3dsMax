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
    HEMAX_GeometryInfo GeoInfo;
    std::vector<HEMAX_Part> Parts;
};

struct HEMAX_EditableCurve
{
    INode* Node;
    HEMAX_CurveInfo CurveInfo;
    std::vector<float> Knots;
    bool Dirty;

    HEMAX_NodeId PushNodeId;
    HEMAX_PartId PushPartId;
};

INode*
MarshallDataInto3dsMaxLinearCurve(HEMAX_EditableCurve& EditableCurve, HEMAX_EditableNode& EditableNode, int PartNum);

INode*
MarshallDataInto3dsMaxNURBSCVCurve(HEMAX_EditableCurve& EditableCurve, HEMAX_EditableNode& EditableNode, int PartNum);
