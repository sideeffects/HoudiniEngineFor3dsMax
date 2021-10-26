#pragma once

#include "HEMAX_CurvePlugin.h"
#include "HEMAX_GeometryPlugin.h"
#include "HEMAX_Mesh.h"
#include "HEMAX_Types.h"
#include "HEMAX_Utilities.h"

#include "inode.h"
#include "ref.h"

#include <vector>
#include <string>

struct HEMAX_PackedPrimitiveInstanceInfo
{
    int InstanceCount;
    int InstancedPartCount;
    std::vector<HEMAX_PartId> InstancedPartIds;
    std::vector<HEMAX_MaxTransform> InstancedTransforms;
};

class HEMAX_Part
{
    public:
	HEMAX_Part();
	~HEMAX_Part();

	HEMAX_Part(const HEMAX_Part& Other);
	HEMAX_Part& operator=(const HEMAX_Part& Other);
	
	void Update(HEMAX_NodeId Node, int PartNum);

	void SetMeshPlugin(HEMAX_GeometryPlugin* GeomPlugin);
	void SetCurvePlugin(LinearShape* Shape);
	void SetCurvePlugin(INode* NurbsObject);

	HEMAX_GeometryPlugin* GetMeshPlugin();
	HEMAX_CurvePlugin* GetCurvePlugin();

	bool Build3dsmaxObject();

	INode* GetINodeOf3dsmaxObject();

	HEMAX_Mesh* BuildMesh();
	HEMAX_Mesh* GetMesh();

	void ClearAnyGeometryRefs();

	std::string ToString();

	void UpdatePartType();
	HEMAX_PartType GetPartType();

	bool Valid;
	HEMAX_NodeId NodeId;

	HEMAX_PartInfo Info;

	bool IsInstancer;
	HEMAX_PackedPrimitiveInstanceInfo PackedPrimInfo;

	HEMAX_CurveInfo CurveInfo;

    private:
	std::vector<HEMAX_MaxTransform> GetInstancedPartTransforms();

        bool HasGroup(const std::string& GroupName, HAPI_GroupType GroupType) const;

	bool BuildGeometryPlugin();
	bool BuildCurvePlugin();

	HEMAX_Mesh* PartMesh;

	HEMAX_GeometryPlugin* MeshPlugin;
	HEMAX_CurvePlugin CurvePlugin;

	HEMAX_PartType PartType;
};
