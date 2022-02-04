#include "HEMAX_Part.h"

#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"

HEMAX_Part::HEMAX_Part()
{
    Valid = false;
    PartMesh = nullptr;
    MeshPlugin = nullptr;
    PartType = HEMAX_PARTTYPE_INVALID;

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
    if (PartMesh)
    {
	delete PartMesh;
	PartMesh = nullptr;
    }

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
    PartMesh = nullptr;
    MeshPlugin = nullptr;
    PartType = HEMAX_PARTTYPE_INVALID;

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
    PartMesh = nullptr;
    MeshPlugin = nullptr;
    PartType = HEMAX_PARTTYPE_INVALID;

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

    if (PartMesh)
    {
	delete PartMesh;
	PartMesh = nullptr;
    }

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    Valid = SM.Session->GetPartInfo(Node, PartNum, &Info);

    if (Info.type == HEMAX_PARTTYPE_INSTANCER)
    {
	PackedPrimInfo.InstanceCount = Info.instanceCount;
	PackedPrimInfo.InstancedPartCount = Info.instancedPartCount;

	PackedPrimInfo.InstancedPartIds.resize(PackedPrimInfo.InstancedPartCount);
	SM.Session->GetInstancedPartIds(Node, PartNum, &PackedPrimInfo.InstancedPartIds.front(), 0, PackedPrimInfo.InstancedPartCount);

	PackedPrimInfo.InstancedTransforms = GetInstancedPartTransforms();
    }

    if (Info.type == HEMAX_PARTTYPE_CURVE)
    {
	SM.Session->GetCurveInfo(Node, PartNum, &CurveInfo);
	CurvePlugin.SetPart(Node, PartNum, CurveInfo);
    }
}

void
HEMAX_Part::SetMeshPlugin(HEMAX_GeometryPlugin* GeomPlugin)
{
    MeshPlugin = GeomPlugin;
}

void
HEMAX_Part::SetCurvePlugin(LinearShape* Shape)
{
    CurvePlugin.SetLinearShape(Shape);
}

void
HEMAX_Part::SetCurvePlugin(INode* NurbsObject)
{
    CurvePlugin.SetNurbsCurve(NurbsObject);
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
    if (Info.type == HEMAX_PARTTYPE_MESH)
    {
	if (MeshPlugin)
	{
	    return BuildGeometryPlugin();
	}
    }
    else if (Info.type == HEMAX_PARTTYPE_CURVE)
    {
	return BuildCurvePlugin();
    }

    return false;
}

INode*
HEMAX_Part::GetINodeOf3dsmaxObject()
{
    if (MeshPlugin && Info.type == HEMAX_PARTTYPE_MESH)
    {
	return MeshPlugin->MaxNode;
    }
    else if (Info.type == HEMAX_PARTTYPE_CURVE)
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

    if (!Info.type == HEMAX_PARTTYPE_MESH)
    {
	std::string Msg = ToString() + ": cannot build mesh for a part that is not a mesh type";
	HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
	return nullptr;
    }

    if (PartMesh)
    {
	delete PartMesh;
	PartMesh = nullptr;
    }

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_AttributeInfo PointAttributeInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_POSITION_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT,
		    &PointAttributeInfo);

    PartMesh = new HEMAX_Mesh(Info.faceCount,
		    Info.vertexCount, PointAttributeInfo.count);

    SM.Session->GetAttributeFloatData(NodeId, Info.id,
                    HEMAX_POSITION_ATTRIBUTE, &PointAttributeInfo, -1,
                    PartMesh->GetPointListArray(), 0, PointAttributeInfo.count);

    // NORMALS //

    HAPI_AttributeInfo PointNormalAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_NORMAL_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT,
		    &PointNormalAttrInfo);

    HAPI_AttributeInfo VertexNormalAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_NORMAL_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX,
		    &VertexNormalAttrInfo);

    if (PointNormalAttrInfo.exists)
    {
	PartMesh->SetNormalsExist(true);
	PartMesh->AllocatePointNormalArray();
        SM.Session->GetAttributeFloatData(NodeId, Info.id,
                        HEMAX_NORMAL_ATTRIBUTE, &PointNormalAttrInfo, -1,
                        PartMesh->GetPointNormalsListArray(), 0,
                        PointNormalAttrInfo.count);
    }
    else if (VertexNormalAttrInfo.exists)
    {
	PartMesh->SetNormalsExist(true);
	PartMesh->AllocateVertexNormalArray();
	SM.Session->GetAttributeFloatData(NodeId, Info.id,
			HEMAX_NORMAL_ATTRIBUTE, &VertexNormalAttrInfo, -1,
			PartMesh->GetVertexNormalsListArray(), 0,
			VertexNormalAttrInfo.count);
    }
    else
    {
	PartMesh->SetNormalsExist(false);
    }

    /////////////

    // COLORS //

    HAPI_AttributeInfo PointColorAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_COLOR_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT,
		    &PointColorAttrInfo);

    HAPI_AttributeInfo VertexColorAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_COLOR_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX,
		    &VertexColorAttrInfo);

    if (PointColorAttrInfo.exists)
    {
	PartMesh->AllocatePointCdArray();
	SM.Session->GetAttributeFloatData(NodeId, Info.id,
			HEMAX_COLOR_ATTRIBUTE, &PointColorAttrInfo, -1,
			PartMesh->GetPointCdArray(), 0,
			PointColorAttrInfo.count);
    }
    else if (VertexColorAttrInfo.exists)
    {
	PartMesh->AllocateVertexCdArray();
	SM.Session->GetAttributeFloatData(NodeId, Info.id,
			HEMAX_COLOR_ATTRIBUTE, &VertexColorAttrInfo, -1,
			PartMesh->GetVertexCdArray(), 0,
			VertexColorAttrInfo.count);
    }

    ////////////

    // ALPHA //

    HAPI_AttributeInfo PointAlphaAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
	    HEMAX_ALPHA_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT,
	    &PointAlphaAttrInfo);

    HAPI_AttributeInfo VertexAlphaAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_ALPHA_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX,
		    &VertexAlphaAttrInfo);

    if (PointAlphaAttrInfo.exists)
    {
	PartMesh->AllocateAlphaArray(HEMAX_ATTRIBUTEOWNER_POINT);
	SM.Session->GetAttributeFloatData(NodeId, Info.id,
			HEMAX_ALPHA_ATTRIBUTE, &PointAlphaAttrInfo, -1,
			PartMesh->GetAlphaArray(), 0, PointAlphaAttrInfo.count);
    }
    else if (VertexAlphaAttrInfo.exists)
    {
	PartMesh->AllocateAlphaArray(HEMAX_ATTRIBUTEOWNER_VERTEX);
	SM.Session->GetAttributeFloatData(NodeId, Info.id,
			HEMAX_ALPHA_ATTRIBUTE, &VertexAlphaAttrInfo, -1,
			PartMesh->GetAlphaArray(), 0,
			VertexAlphaAttrInfo.count);
    }

    ///////////

    // ILLUMINATION //

    HAPI_AttributeInfo PointIllumeAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_ILLUMINATION_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT,
		    &PointIllumeAttrInfo);

    HAPI_AttributeInfo VertexIllumeAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_ILLUMINATION_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX,
		    &VertexIllumeAttrInfo);

    if (PointIllumeAttrInfo.exists)
    {
	PartMesh->AllocateIlluminationArray(HEMAX_ATTRIBUTEOWNER_POINT);
	SM.Session->GetAttributeFloatData(NodeId, Info.id,
			HEMAX_ILLUMINATION_ATTRIBUTE, &PointIllumeAttrInfo, -1,
			PartMesh->GetIlluminationArray(), 0,
			PointIllumeAttrInfo.count);
    }
    else if (VertexIllumeAttrInfo.exists)
    {
	PartMesh->AllocateIlluminationArray(HEMAX_ATTRIBUTEOWNER_VERTEX);
	SM.Session->GetAttributeFloatData(NodeId, Info.id,
			HEMAX_ILLUMINATION_ATTRIBUTE, &VertexIllumeAttrInfo, -1,
			PartMesh->GetIlluminationArray(), 0,
			VertexIllumeAttrInfo.count);
    }

    /////////////////

    // SMOOTHING GROUPS //

    HAPI_AttributeInfo SGAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_SMOOTHING_GROUP_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_PRIM,
		    &SGAttrInfo);

    if (SGAttrInfo.exists)
    {
	PartMesh->AllocateSmoothingGroupsArray();
	SM.Session->GetAttributeIntData(NodeId, Info.id,
			HEMAX_SMOOTHING_GROUP_ATTRIBUTE, &SGAttrInfo, -1,
			PartMesh->GetSmoothingGroupArray(), 0,
			PartMesh->GetFaceCount());
    }

    //////////////////////

    // MATERIAL IDS //

    HAPI_AttributeInfo MatIDAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_MATERIAL_ID_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_PRIM,
		    &MatIDAttrInfo);

    if (MatIDAttrInfo.exists)
    {
	PartMesh->AllocateMaterialIDArray();
	SM.Session->GetAttributeIntData(NodeId, Info.id,
			HEMAX_MATERIAL_ID_ATTRIBUTE, &MatIDAttrInfo, -1,
			PartMesh->GetMaterialIDArray(), 0,
			PartMesh->GetFaceCount());
    }

    // Selections //
    
    if (HasGroup(HEMAX_SELECTION_FACE, HAPI_GROUPTYPE_PRIM))
    {
        PartMesh->AllocateFaceSelectionsArray();
        SM.Session->GetGroupMembership(NodeId, Info.id, HAPI_GROUPTYPE_PRIM,
            HEMAX_SELECTION_FACE, nullptr, PartMesh->GetFaceSelectionsArray(),
            0, PartMesh->GetFaceCount());
    }

    if (HasGroup(HEMAX_SELECTION_VERTEX, HAPI_GROUPTYPE_POINT))
    {
        PartMesh->AllocateVertexSelectionsArray();
        SM.Session->GetGroupMembership(NodeId, Info.id, HAPI_GROUPTYPE_POINT,
            HEMAX_SELECTION_VERTEX, nullptr,
            PartMesh->GetVertexSelectionsArray(), 0, PartMesh->GetPointCount());
    }

    if (HasGroup(HEMAX_SELECTION_EDGE, HAPI_GROUPTYPE_EDGE))
    {
        int EdgeCount = 0;
        SM.Session->GetEdgeCountOfEdgeGroup(NodeId, Info.id,
            HEMAX_SELECTION_EDGE, &EdgeCount);
        PartMesh->AllocateEdgeSelectionsArray(EdgeCount);
        SM.Session->GetGroupMembership(NodeId, Info.id, HAPI_GROUPTYPE_EDGE,
            HEMAX_SELECTION_EDGE, nullptr, PartMesh->GetEdgeSelectionsArray(),
            0, EdgeCount*2);
    }

    /////////////////////

    // SHOP MATERIAL PATH //

    HAPI_AttributeInfo MatPathAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, Info.id,
		    HEMAX_MATERIAL_PATH_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_DETAIL,
		    &MatPathAttrInfo);

    if (MatPathAttrInfo.exists)
    {
	std::vector<HAPI_StringHandle> StringHandles(1);
	SM.Session->GetAttributeStringData(NodeId, Info.id,
			HEMAX_MATERIAL_PATH_ATTRIBUTE, &MatPathAttrInfo,
			StringHandles.data(), 0, 1);
	PartMesh->MaterialPath = SM.Session->GetHAPIString(StringHandles[0]);
    }
    else
    {
	PartMesh->MaterialPath = "";
    }

    ////////////////////////

    // DETAIL META-DATA ////

    int DetailAttribCount = Info.attributeCounts[HEMAX_ATTRIBUTEOWNER_DETAIL];

    if (DetailAttribCount > 0)
    {
	std::string MetadataStr(HEMAX_METADATA);

	HAPI_StringHandle* SH_Names = new HAPI_StringHandle[DetailAttribCount];
	SM.Session->GetAttributeNames(NodeId, Info.id,
			HEMAX_ATTRIBUTEOWNER_DETAIL, SH_Names,
			DetailAttribCount);
	for (int n = 0; n < DetailAttribCount; n++)
	{
	    std::string AttribName = SM.Session->GetHAPIString(SH_Names[n]);
	    if (AttribName.compare(0, MetadataStr.length(), MetadataStr) == 0)
	    {
		HAPI_AttributeInfo AttribInfo;
		if (SM.Session->GetAttributeInfo(NodeId, Info.id,
				    AttribName.c_str(),
				    HEMAX_ATTRIBUTEOWNER_DETAIL, &AttribInfo))
		{
		    switch (AttribInfo.storage)
		    {
			case HAPI_STORAGETYPE_FLOAT:
			case HAPI_STORAGETYPE_FLOAT64:
			    {
				PartMesh->AddMetadata(AttribName,
					    HEMAX_MESH_FLOAT_METADATA,
					    AttribInfo.count,
					    AttribInfo.tupleSize,
					    HEMAX_ATTRIBUTEOWNER_DETAIL);
				HEMAX_MeshList<float>& Metadata = PartMesh->GetFloatMetadata(AttribName);
				SM.Session->GetAttributeFloatData(NodeId,
						Info.id, AttribName.c_str(),
						&AttribInfo, -1,
						Metadata.Data(), 0, 1);
			    } break;
			case HAPI_STORAGETYPE_INT:
			case HAPI_STORAGETYPE_INT64:
			    {
				PartMesh->AddMetadata(AttribName,
					    HEMAX_MESH_INT_METADATA,
					    AttribInfo.count,
					    AttribInfo.tupleSize,
					    HEMAX_ATTRIBUTEOWNER_DETAIL);
				HEMAX_MeshList<int>& Metadata = PartMesh->GetIntMetadata(AttribName);
				SM.Session->GetAttributeIntData(NodeId,
						Info.id, AttribName.c_str(),
						&AttribInfo, -1,
						Metadata.Data(), 0, 1);
			    } break;
			case HAPI_STORAGETYPE_STRING:
			    {
				PartMesh->AddMetadata(AttribName,
					    HEMAX_MESH_STRING_METADATA,
					    AttribInfo.count,
					    AttribInfo.tupleSize,
					    HEMAX_ATTRIBUTEOWNER_DETAIL);
				HEMAX_MeshList<std::string>& Metadata = PartMesh->GetStringMetadata(AttribName);
				std::vector<HAPI_StringHandle> StringHandles(1);
				if (SM.Session->GetAttributeStringData(NodeId,
						    Info.id, AttribName.c_str(),
						    &AttribInfo,
						    StringHandles.data(), 0, 1))
				{
				    Metadata.Data()[0] = SM.Session->GetHAPIString(StringHandles[0]);
				}
			    } break;
			default:
			    {

			    } break;
		    }
		}
	    }
	}

	delete SH_Names;
    }

    ////////////////////////

    if (PartMesh->GetFaceCount() > 0)
    {
	SM.Session->GetFaceCounts(NodeId, Info.id,
			PartMesh->GetFaceVertexCountsArray(), 0,
			Info.faceCount);
	SM.Session->GetVertexList(NodeId, Info.id,
			PartMesh->GetVertexListArray(), 0, Info.vertexCount);

	PartMesh->AllocateMaterialIdsArray();
	SM.Session->GetMaterialNodeIdsOnFaces(NodeId, Info.id,
		&PartMesh->AreMaterialIdsSame, PartMesh->GetMaterialIdsArray(),
		0, Info.faceCount);

	// Main UV Layer

	HAPI_AttributeInfo UVPointAttributeInfo;
	SM.Session->GetAttributeInfo(NodeId, Info.id,
			HEMAX_UV_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT,
			&UVPointAttributeInfo);

	HAPI_AttributeInfo UVVertexAttributeInfo;
	SM.Session->GetAttributeInfo(NodeId, Info.id,
			HEMAX_UV_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX,
			&UVVertexAttributeInfo);

	if (UVPointAttributeInfo.exists)
	{
	    PartMesh->AllocatePointUVArray(UVPointAttributeInfo.tupleSize);
	    SM.Session->GetAttributeFloatData(NodeId, Info.id,
			    HEMAX_UV_ATTRIBUTE, &UVPointAttributeInfo, -1,
			    PartMesh->GetPointUVArray(), 0, Info.pointCount);
	}

	if (UVVertexAttributeInfo.exists)
	{
	    PartMesh->AllocateVertexUVArray(UVVertexAttributeInfo.tupleSize);
	    SM.Session->GetAttributeFloatData(NodeId, Info.id,
			    HEMAX_UV_ATTRIBUTE, &UVVertexAttributeInfo, -1,
			    PartMesh->GetVertexUVArray(), 0, Info.vertexCount);
	}

	// Secondary UV Layers

	for (int UVLayer = 2; UVLayer < MAX_MESHMAPS; UVLayer++)
	{
	    std::string UVName = HEMAX_UV_ATTRIBUTE + std::to_string(UVLayer);

	    HAPI_AttributeInfo UVPointInfo;
	    SM.Session->GetAttributeInfo(NodeId, Info.id, UVName.c_str(),
			    HEMAX_ATTRIBUTEOWNER_POINT, &UVPointInfo);
	    HAPI_AttributeInfo UVVertexInfo;
	    SM.Session->GetAttributeInfo(NodeId, Info.id, UVName.c_str(),
			    HEMAX_ATTRIBUTEOWNER_VERTEX, &UVVertexInfo);

	    if (UVPointInfo.exists)
	    {
		PartMesh->CreateSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_POINT,
				UVLayer, Info.pointCount * 3);
		SM.Session->GetAttributeFloatData(NodeId, Info.id,
				UVName.c_str(), &UVPointInfo, -1,
				PartMesh->GetSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_POINT, UVLayer).data(),
				0, Info.pointCount);
	    }

	    if (UVVertexInfo.exists)
	    {
		PartMesh->CreateSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_VERTEX,
				UVLayer, Info.vertexCount * 3);
		SM.Session->GetAttributeFloatData(NodeId, Info.id,
				UVName.c_str(), &UVVertexInfo, -1,
				PartMesh->GetSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_VERTEX, UVLayer).data(),
				0, Info.vertexCount);
	    }
	}
    }

    return PartMesh;
}

HEMAX_Mesh*
HEMAX_Part::GetMesh()
{
    return PartMesh; 
}

void
HEMAX_Part::ClearAnyGeometryRefs()
{
    if (PartMesh)
    {
	delete PartMesh;
	PartMesh = nullptr;
    }

    MeshPlugin = nullptr;
}

std::string
HEMAX_Part::ToString()
{
    std::string RetString;

    if (Valid)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
	std::string PartName = SM.Session->GetHAPIString(Info.nameSH);    
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
    PartType = (HEMAX_PartType)Info.type;
}

HEMAX_PartType
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

	bool Success = SM.Session->GetInstancerPartTransforms(
			    NodeId, Info.id,
			    HAPI_RSTORDER_DEFAULT,
			    &HAPITransforms.front(),
			    0, PackedPrimInfo.InstanceCount
			);

	if (Success)
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
HEMAX_Part::HasGroup(const std::string& GroupName,
    HAPI_GroupType GroupType) const
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HAPI_GeoInfo GeometryInfo;
    SM.Session->GetGeometryInfo(NodeId, &GeometryInfo);

    std::vector<HAPI_StringHandle> GroupNameHandles;

    if (GroupType == HAPI_GROUPTYPE_PRIM)
    {
        GroupNameHandles.resize(GeometryInfo.primitiveGroupCount);
    }
    else if (GroupType == HAPI_GROUPTYPE_POINT)
    {
        GroupNameHandles.resize(GeometryInfo.pointGroupCount);
    }
    else if (GroupType == HAPI_GROUPTYPE_EDGE)
    {
        GroupNameHandles.resize(GeometryInfo.edgeGroupCount);
    }

    SM.Session->GetGroupNames(NodeId, GroupType, GroupNameHandles.data(),
        (int)GroupNameHandles.size());

    for (int i = 0; i < GroupNameHandles.size(); i++)
    {
        if (GroupName == SM.Session->GetHAPIString(GroupNameHandles[i]))
            return true;
    }

    return false;
}

bool
HEMAX_Part::BuildGeometryPlugin()
{
    BuildMesh();   
    MeshPlugin->Mesh = PartMesh;
    MeshPlugin->BuildMesh(GetCOREInterface()->GetTime());
    MeshPlugin->Mesh->ApplyDetailMetadata(MeshPlugin->MaxNode);
    
    TimeValue CurTime = GetCOREInterface()->GetTime();
    MeshPlugin->ForceNotify(Interval(CurTime, CurTime));

    return true;
}

bool
HEMAX_Part::BuildCurvePlugin()
{
    return CurvePlugin.BuildCurve();
}
