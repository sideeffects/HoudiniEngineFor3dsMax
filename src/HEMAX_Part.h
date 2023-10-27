#pragma once

#include "HEMAX_CurvePlugin.h"
#include "HEMAX_GeometryPlugin.h"
#include "HEMAX_Mesh.h"
#include "HEMAX_Types.h"
#include "HEMAX_Utilities.h"

#pragma warning(push, 0)
#include <inode.h>
#include <ref.h>
#pragma pop

#include <vector>
#include <string>

struct HEMAX_PackedPrimitiveInstanceInfo
{
    int InstanceCount;
    int InstancedPartCount;
    std::vector<HAPI_PartId> InstancedPartIds;
    std::vector<HEMAX_MaxTransform> InstancedTransforms;
};

class HEMAX_Part
{
    public:
	HEMAX_Part();
	~HEMAX_Part();

	HEMAX_Part(const HEMAX_Part& Other);
	HEMAX_Part& operator=(const HEMAX_Part& Other);
	
	void Update(HAPI_NodeId Node, int PartNum);

	void SetMeshPlugin(HEMAX_GeometryPlugin* GeomPlugin);
	void SetCurvePlugin(INode* CurveNode);

	HEMAX_GeometryPlugin* GetMeshPlugin();
	HEMAX_CurvePlugin* GetCurvePlugin();

	bool Build3dsmaxObject();

	INode* GetINodeOf3dsmaxObject();

	HEMAX_Mesh* BuildMesh();
	HEMAX_Mesh* GetMesh();

	void ClearAnyGeometryRefs();

	std::string ToString();

	void UpdatePartType();
	HAPI_PartType GetPartType();

	bool Valid;
	HAPI_NodeId NodeId;

	HAPI_PartInfo Info;

	bool IsInstancer;
	HEMAX_PackedPrimitiveInstanceInfo PackedPrimInfo;

	HAPI_CurveInfo CurveInfo;

    private:
	std::vector<HEMAX_MaxTransform> GetInstancedPartTransforms();

        bool HasGroup(const std::string& GroupName, HAPI_GroupType GroupType) const;

	bool BuildGeometryPlugin();
	bool BuildCurvePlugin();

	HEMAX_Mesh* PartMesh;

	HEMAX_GeometryPlugin* MeshPlugin;
	HEMAX_CurvePlugin CurvePlugin;

	HAPI_PartType PartType;
};
