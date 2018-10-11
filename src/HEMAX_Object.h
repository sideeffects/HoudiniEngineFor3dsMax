#pragma once

#include "HEMAX_Types.h"
#include "HEMAX_DisplayGeoNode.h"
#include "HEMAX_Utilities.h"
#include <vector>
#include <string>
#include <unordered_map>

struct HEMAX_InstancingInformation
{
    bool InstanceAttributeExists;
    std::vector<HEMAX_NodeId> InstanceNodeIds;
    HEMAX_NodeId InstanceeId;
    std::vector<HEMAX_MaxTransform> InstanceTransforms;
};

struct HEMAX_Object
{
    HEMAX_ObjectInfo Info;
    HEMAX_HAPITransform Transform;
    std::string NodeName;

    std::unordered_map<HEMAX_NodeId, HEMAX_DisplayGeoNode> SopNodes;
    std::unordered_map<HEMAX_NodeId, HEMAX_InstancingInformation> Instances;
};

// Returns true if an object node was successfully parsed
// returns false otherwise
bool
GetObjectInfo(HEMAX_NodeId NodeId, HEMAX_Object& Object);

static bool
GetInstancingInfo(HEMAX_ObjectInfo& InstancerObjInfo, HEMAX_InstancingInformation& Info);

void
UpdateObject(HEMAX_Object& Object);

static void
UpdateInstancingInformation(HEMAX_ObjectInfo& InstancerObjInfo, HEMAX_InstancingInformation& Info);

HEMAX_DisplayGeoNode
FindSopNodeFromName(HEMAX_Object& Object, std::string Name);

void
ClearObject(HEMAX_Object& Object);