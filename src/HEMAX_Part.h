#pragma once

#include "HEMAX_Types.h"
#include "HEMAX_Utilities.h"
#include <vector>

struct HEMAX_PackedPrimitiveInstanceInfo
{
    int InstanceCount;
    int InstancedPartCount;
    std::vector<HEMAX_PartId> InstancedPartIds;
    std::vector<HEMAX_MaxTransform> InstancedTransforms;
};

struct HEMAX_Part
{
    HEMAX_PartInfo Info;
    HEMAX_PackedPrimitiveInstanceInfo PackedPrimInfo;
};

void
LoadPartInfo(HEMAX_Part& Part, HEMAX_NodeId Node, int PartNum);