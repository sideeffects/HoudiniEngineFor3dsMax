#pragma once

#include "HEMAX_Types.h"
#include "HEMAX_DisplayGeoNode.h"
#include "HEMAX_Utilities.h"
#include <vector>
#include <string>
#include <unordered_map>

struct HEMAX_InstancingInformation
{
    bool HasMultipleInstancees;
    std::vector<HAPI_NodeId> InstanceNodeIds;
    HAPI_NodeId InstanceeId;
    std::vector<HEMAX_MaxTransform> InstanceTransforms;
};

class HEMAX_Object
{
    public:
	HEMAX_Object();

	void Init(HAPI_NodeId NodeId);
	void Update();

	HEMAX_DisplayGeoNode* FindSopNodeFromName(const std::string& Name);

	HAPI_NodeId Node;
	HAPI_ObjectInfo Info;
	HAPI_Transform Transform;
	std::string Name;

	std::unordered_map<HAPI_NodeId, HEMAX_DisplayGeoNode> SopNodes;
	std::unordered_map<HAPI_NodeId, HEMAX_InstancingInformation> Instances;

    private:
	void Create(HAPI_NodeId NodeId);
};


static bool
GetInstancingInfo(HAPI_ObjectInfo& InstancerObjInfo, HEMAX_InstancingInformation& Info);

static std::vector<HEMAX_MaxTransform>
GetInstanceTransforms(HAPI_NodeId InstancerNode);
