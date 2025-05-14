#include "HEMAX_Part.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"

HEMAX_Part::HEMAX_Part()
{
    Valid = false;
    MeshPlugin = nullptr;
    PartType = HAPI_PARTTYPE_INVALID;

    Info.id = 0;
    Info.nameSH = 0;
    Info.type = HAPI_PARTTYPE_INVALID;

    Info.faceCount = 0;
    Info.vertexCount = 0;
    Info.pointCount = 0;

    Info.attributeCounts[HAPI_ATTROWNER_POINT] = 0;
    Info.attributeCounts[HAPI_ATTROWNER_PRIM] = 0;
    Info.attributeCounts[HAPI_ATTROWNER_VERTEX] = 0;
    Info.attributeCounts[HAPI_ATTROWNER_DETAIL] = 0;

    Info.isInstanced = false;
    Info.instancedPartCount = 0;
    Info.instanceCount = 0;
    Info.hasChanged = true;
}

HEMAX_Part::~HEMAX_Part()
{
    INode* PartNode = GetINodeOf3dsmaxObject();
    if (PartNode)
    {
	Interface* MaxInterface = GetCOREInterface();
	if (MaxInterface)
	{
	    MaxInterface->DeleteNode(PartNode);
	}
    }
}

HEMAX_Part::HEMAX_Part(const HEMAX_Part& Other)
{
    Valid = false;
    MeshPlugin = nullptr;
    PartType = HAPI_PARTTYPE_INVALID;

    if (Other.Valid)
    {
	NodeId = Other.NodeId;
	Update(NodeId, Other.Info.id);
    }
}

HEMAX_Part&
HEMAX_Part::operator=(const HEMAX_Part& Other)
{
    Valid = false;
    MeshPlugin = nullptr;
    PartType = HAPI_PARTTYPE_INVALID;

    if (Other.Valid)
    {
	NodeId = Other.NodeId;
	Update(NodeId, Other.Info.id);
    }

    return *this;
}

void
HEMAX_Part::Update(HAPI_NodeId Node, int PartNum)
{
    NodeId = Node;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_Result Result = HEMAX_HoudiniApi::GetPartInfo(&SM.Session, Node,
            PartNum, &Info);
    Valid = (Result == HAPI_RESULT_SUCCESS);

    if (Info.type == HAPI_PARTTYPE_INSTANCER)
    {
	PackedPrimInfo.InstanceCount = Info.instanceCount;
	PackedPrimInfo.InstancedPartCount = Info.instancedPartCount;

	PackedPrimInfo.InstancedPartIds.resize(PackedPrimInfo.InstancedPartCount);
        HEMAX_HoudiniApi::GetInstancedPartIds(&SM.Session, Node, PartNum,
            &PackedPrimInfo.InstancedPartIds.front(), 0,
            PackedPrimInfo.InstancedPartCount);

	PackedPrimInfo.InstancedTransforms = GetInstancedPartTransforms();
    }

    if (Info.type == HAPI_PARTTYPE_CURVE)
    {
        HEMAX_HoudiniApi::GetCurveInfo(&SM.Session, Node, PartNum, &CurveInfo);
	CurvePlugin.SetPart(Node, PartNum, CurveInfo);
    }
}

void
HEMAX_Part::SetMeshPlugin(HEMAX_GeometryPlugin* GeomPlugin)
{
    MeshPlugin = GeomPlugin;
}

void
HEMAX_Part::SetCurvePlugin(INode* CurveNode)
{
    CurvePlugin.SetCurveNode(CurveNode);
}

HEMAX_GeometryPlugin*
HEMAX_Part::GetMeshPlugin()
{
    return MeshPlugin;
}

HEMAX_CurvePlugin*
HEMAX_Part::GetCurvePlugin()
{
    return &CurvePlugin;
}

bool
HEMAX_Part::Build3dsmaxObject()
{
    if (Info.type == HAPI_PARTTYPE_MESH)
    {
	if (MeshPlugin)
	{
	    return BuildGeometryPlugin();
	}
    }
    else if (Info.type == HAPI_PARTTYPE_CURVE)
    {
	return BuildCurvePlugin();
    }

    return false;
}

INode*
HEMAX_Part::GetINodeOf3dsmaxObject()
{
    if (MeshPlugin && Info.type == HAPI_PARTTYPE_MESH)
    {
	return MeshPlugin->MaxNode;
    }
    else if (Info.type == HAPI_PARTTYPE_CURVE)
    {
	return CurvePlugin.GetINode();
    }

    return nullptr;
}

HEMAX_Mesh*
HEMAX_Part::BuildMesh()
{
    if (!Valid)
    {
	std::string Msg = std::string(__func__) + ": cannot build mesh for invalid part";
	HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
	return nullptr;
    }

    if (!Info.type == HAPI_PARTTYPE_MESH)
    {
	std::string Msg = ToString() + ": cannot build mesh for a part that is not a mesh type";
	HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
	return nullptr;
    }

    PartMesh.Clear();
    PartMesh.InitFromPart(NodeId, Info);

    return &PartMesh;
}

HEMAX_Mesh*
HEMAX_Part::GetMesh()
{
    return &PartMesh; 
}

void
HEMAX_Part::ClearAnyGeometryRefs()
{
    PartMesh.Clear();
    MeshPlugin = nullptr;
}

std::string
HEMAX_Part::ToString()
{
    std::string RetString;

    if (Valid)
    {
	std::string PartName = HEMAX_Utilities::GetHAPIString(Info.nameSH);    
	RetString = "Part: [Name:" + PartName + "] [NodeId:" + std::to_string(NodeId) + "] [PartId:" + std::to_string(Info.id) + "]";
    }
    else
    {
	RetString = "Part: Invalid";
    }

    return RetString;
}

void
HEMAX_Part::UpdatePartType()
{
    PartType = (HAPI_PartType)Info.type;
}

HAPI_PartType
HEMAX_Part::GetPartType()
{
    return PartType;
}

std::vector<HEMAX_MaxTransform>
HEMAX_Part::GetInstancedPartTransforms()
{
    std::vector<HEMAX_MaxTransform> MaxTransforms;

    if (!Valid)
    {
	std::string Msg = std::string(__func__) + ": called with invalid part";
	HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
    }

    if (PackedPrimInfo.InstanceCount > 0)
    {
	std::vector<HAPI_Transform> HAPITransforms(PackedPrimInfo.InstanceCount);

	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

	HAPI_Result Result = HEMAX_HoudiniApi::GetInstancerPartTransforms(
                                    &SM.Session,
			            NodeId, Info.id,
			            HAPI_RSTORDER_DEFAULT,
			            &HAPITransforms.front(),
			            0, PackedPrimInfo.InstanceCount);

	if (Result == HAPI_RESULT_SUCCESS)
	{
	    for (int t = 0; t < PackedPrimInfo.InstanceCount; t++)
	    {
		MaxTransforms.push_back(HEMAX_Utilities::HAPITransformToMaxTransform(HAPITransforms[t]));
	    }
	}
    }
    else
    {
	std::string Msg = ToString() + " -> is not a instancer, or has no instanced parts.";
    }

    return MaxTransforms;
}

bool
HEMAX_Part::BuildGeometryPlugin()
{
    BuildMesh();   
    MeshPlugin->Mesh = &PartMesh;
    MeshPlugin->BuildMesh(GetCOREInterface()->GetTime());
    MeshPlugin->Mesh->ApplyMetadataToINode(*(MeshPlugin->MaxNode));
    
    TimeValue CurTime = GetCOREInterface()->GetTime();
    Interval Now(CurTime, CurTime);
    MeshPlugin->ForceNotify(Now);

    return true;
}

bool
HEMAX_Part::BuildCurvePlugin()
{
    return CurvePlugin.BuildCurve();
}
