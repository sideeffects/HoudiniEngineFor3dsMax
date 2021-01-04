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

class HEMAX_Object
{
    public:
	HEMAX_Object();

	void Init(HEMAX_NodeId NodeId);
	void Update();

	HEMAX_DisplayGeoNode* FindSopNodeFromName(const std::string& Name);

	HEMAX_NodeId Node;
	HEMAX_ObjectInfo Info;
	HEMAX_HAPITransform Transform;
	std::string Name;

	std::unordered_map<HEMAX_NodeId, HEMAX_DisplayGeoNode> SopNodes;
	std::unordered_map<HEMAX_NodeId, HEMAX_InstancingInformation> Instances;

    private:
	void Create(HEMAX_NodeId NodeId);
};


static bool
GetInstancingInfo(HEMAX_ObjectInfo& InstancerObjInfo, HEMAX_InstancingInformation& Info);

static std::vector<HEMAX_MaxTransform>
GetInstanceTransforms(HEMAX_NodeId InstancerNode);
