#include "HEMAX_Part.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"

HEMAX_Part::HEMAX_Part()
{
    Valid = false;
    PartMesh = nullptr;
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
    PartMesh = nullptr;
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

    if (PartMesh)
    {
	delete PartMesh;
	PartMesh = nullptr;
    }

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    Valid = HEMAX_HoudiniApi::GetPartInfo(SM.Session, Node, PartNum, &Info);

    if (Info.type == HAPI_PARTTYPE_INSTANCER)
    {
	PackedPrimInfo.InstanceCount = Info.instanceCount;
	PackedPrimInfo.InstancedPartCount = Info.instancedPartCount;

	PackedPrimInfo.InstancedPartIds.resize(PackedPrimInfo.InstancedPartCount);
        HEMAX_HoudiniApi::GetInstancedPartIds(SM.Session, Node, PartNum,
            &PackedPrimInfo.InstancedPartIds.front(), 0,
            PackedPrimInfo.InstancedPartCount);

	PackedPrimInfo.InstancedTransforms = GetInstancedPartTransforms();
    }

    if (Info.type == HAPI_PARTTYPE_CURVE)
    {
        HEMAX_HoudiniApi::GetCurveInfo(SM.Session, Node, PartNum, &CurveInfo);
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

    if (PartMesh)
    {
	delete PartMesh;
	PartMesh = nullptr;
    }

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_AttributeInfo PointAttributeInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_POSITION_ATTRIBUTE, HAPI_ATTROWNER_POINT,
		    &PointAttributeInfo);

    PartMesh = new HEMAX_Mesh(Info.faceCount,
		    Info.vertexCount, PointAttributeInfo.count);

    HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
                    HEMAX_POSITION_ATTRIBUTE, &PointAttributeInfo, -1,
                    PartMesh->GetPointListArray(), 0, PointAttributeInfo.count);

    // NORMALS //

    HAPI_AttributeInfo PointNormalAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_NORMAL_ATTRIBUTE, HAPI_ATTROWNER_POINT,
		    &PointNormalAttrInfo);

    HAPI_AttributeInfo VertexNormalAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_NORMAL_ATTRIBUTE, HAPI_ATTROWNER_VERTEX,
		    &VertexNormalAttrInfo);

    if (PointNormalAttrInfo.exists)
    {
	PartMesh->SetNormalsExist(true);
	PartMesh->AllocatePointNormalArray();
        HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
                        HEMAX_NORMAL_ATTRIBUTE, &PointNormalAttrInfo, -1,
                        PartMesh->GetPointNormalsListArray(), 0,
                        PointNormalAttrInfo.count);
    }
    else if (VertexNormalAttrInfo.exists)
    {
	PartMesh->SetNormalsExist(true);
	PartMesh->AllocateVertexNormalArray();
        HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
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
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_COLOR_ATTRIBUTE, HAPI_ATTROWNER_POINT,
		    &PointColorAttrInfo);

    HAPI_AttributeInfo VertexColorAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_COLOR_ATTRIBUTE, HAPI_ATTROWNER_VERTEX,
		    &VertexColorAttrInfo);

    if (PointColorAttrInfo.exists)
    {
	PartMesh->AllocatePointCdArray(PointColorAttrInfo.tupleSize);
        HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
			HEMAX_COLOR_ATTRIBUTE, &PointColorAttrInfo, -1,
			PartMesh->GetPointCdArray(), 0,
			PointColorAttrInfo.count);
    }
    else if (VertexColorAttrInfo.exists)
    {
	PartMesh->AllocateVertexCdArray(VertexColorAttrInfo.tupleSize);
        HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
			HEMAX_COLOR_ATTRIBUTE, &VertexColorAttrInfo, -1,
			PartMesh->GetVertexCdArray(), 0,
			VertexColorAttrInfo.count);
    }

    ////////////

    // ALPHA //

    HAPI_AttributeInfo PointAlphaAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
	    HEMAX_ALPHA_ATTRIBUTE, HAPI_ATTROWNER_POINT,
	    &PointAlphaAttrInfo);

    HAPI_AttributeInfo VertexAlphaAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_ALPHA_ATTRIBUTE, HAPI_ATTROWNER_VERTEX,
		    &VertexAlphaAttrInfo);

    if (PointAlphaAttrInfo.exists)
    {
	PartMesh->AllocateAlphaArray(HAPI_ATTROWNER_POINT);
        HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
			HEMAX_ALPHA_ATTRIBUTE, &PointAlphaAttrInfo, -1,
			PartMesh->GetAlphaArray(), 0, PointAlphaAttrInfo.count);
    }
    else if (VertexAlphaAttrInfo.exists)
    {
	PartMesh->AllocateAlphaArray(HAPI_ATTROWNER_VERTEX);
        HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
			HEMAX_ALPHA_ATTRIBUTE, &VertexAlphaAttrInfo, -1,
			PartMesh->GetAlphaArray(), 0,
			VertexAlphaAttrInfo.count);
    }

    ///////////

    // ILLUMINATION //

    HAPI_AttributeInfo PointIllumeAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_ILLUMINATION_ATTRIBUTE, HAPI_ATTROWNER_POINT,
		    &PointIllumeAttrInfo);

    HAPI_AttributeInfo VertexIllumeAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_ILLUMINATION_ATTRIBUTE, HAPI_ATTROWNER_VERTEX,
		    &VertexIllumeAttrInfo);

    if (PointIllumeAttrInfo.exists)
    {
	PartMesh->AllocateIlluminationArray(HAPI_ATTROWNER_POINT);
        HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
			HEMAX_ILLUMINATION_ATTRIBUTE, &PointIllumeAttrInfo, -1,
			PartMesh->GetIlluminationArray(), 0,
			PointIllumeAttrInfo.count);
    }
    else if (VertexIllumeAttrInfo.exists)
    {
	PartMesh->AllocateIlluminationArray(HAPI_ATTROWNER_VERTEX);
        HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
			HEMAX_ILLUMINATION_ATTRIBUTE, &VertexIllumeAttrInfo, -1,
			PartMesh->GetIlluminationArray(), 0,
			VertexIllumeAttrInfo.count);
    }

    /////////////////

    // SMOOTHING GROUPS //

    HAPI_AttributeInfo SGAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_SMOOTHING_GROUP_ATTRIBUTE, HAPI_ATTROWNER_PRIM,
		    &SGAttrInfo);

    if (SGAttrInfo.exists)
    {
	PartMesh->AllocateSmoothingGroupsArray();
        HEMAX_HoudiniApi::GetAttributeIntData(SM.Session, NodeId, Info.id,
			HEMAX_SMOOTHING_GROUP_ATTRIBUTE, &SGAttrInfo, -1,
			PartMesh->GetSmoothingGroupArray(), 0,
			PartMesh->GetFaceCount());
    }

    //////////////////////

    // MATERIAL IDS //

    HAPI_AttributeInfo MatIDAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_MATERIAL_ID_ATTRIBUTE, HAPI_ATTROWNER_PRIM,
		    &MatIDAttrInfo);

    if (MatIDAttrInfo.exists)
    {
	PartMesh->AllocateMaterialIDArray();
        HEMAX_HoudiniApi::GetAttributeIntData(SM.Session, NodeId, Info.id,
			HEMAX_MATERIAL_ID_ATTRIBUTE, &MatIDAttrInfo, -1,
			PartMesh->GetMaterialIDArray(), 0,
			PartMesh->GetFaceCount());
    }

    // Selections //
    
    if (HasGroup(HEMAX_SELECTION_FACE, HAPI_GROUPTYPE_PRIM))
    {
        PartMesh->AllocateFaceSelectionsArray();
        HEMAX_HoudiniApi::GetGroupMembership(SM.Session, NodeId, Info.id,
            HAPI_GROUPTYPE_PRIM, HEMAX_SELECTION_FACE, nullptr,
            PartMesh->GetFaceSelectionsArray(),
            0, PartMesh->GetFaceCount());
    }

    if (HasGroup(HEMAX_SELECTION_VERTEX, HAPI_GROUPTYPE_POINT))
    {
        PartMesh->AllocateVertexSelectionsArray();
        HEMAX_HoudiniApi::GetGroupMembership(SM.Session, NodeId, Info.id,
            HAPI_GROUPTYPE_POINT, HEMAX_SELECTION_VERTEX, nullptr,
            PartMesh->GetVertexSelectionsArray(), 0, PartMesh->GetPointCount());
    }

    if (HasGroup(HEMAX_SELECTION_EDGE, HAPI_GROUPTYPE_EDGE))
    {
        int EdgeCount = 0;
        HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroup(SM.Session, NodeId, Info.id,
            HEMAX_SELECTION_EDGE, &EdgeCount);
        PartMesh->AllocateEdgeSelectionsArray(EdgeCount);
        HEMAX_HoudiniApi::GetGroupMembership(SM.Session, NodeId, Info.id,
            HAPI_GROUPTYPE_EDGE, HEMAX_SELECTION_EDGE, nullptr,
            PartMesh->GetEdgeSelectionsArray(), 0, EdgeCount*2);
    }

    /////////////////////

    // SHOP MATERIAL PATH //

    HAPI_AttributeInfo MatPathAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
		    HEMAX_MATERIAL_PATH_ATTRIBUTE, HAPI_ATTROWNER_DETAIL,
		    &MatPathAttrInfo);

    if (MatPathAttrInfo.exists)
    {
	std::vector<HAPI_StringHandle> StringHandles(1);
        HEMAX_HoudiniApi::GetAttributeStringData(SM.Session, NodeId, Info.id,
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

    int DetailAttribCount = Info.attributeCounts[HAPI_ATTROWNER_DETAIL];

    if (DetailAttribCount > 0)
    {
	std::string MetadataStr(HEMAX_METADATA);

	HAPI_StringHandle* SH_Names = new HAPI_StringHandle[DetailAttribCount];
        HEMAX_HoudiniApi::GetAttributeNames(SM.Session, NodeId, Info.id,
			HAPI_ATTROWNER_DETAIL, SH_Names,
			DetailAttribCount);
	for (int n = 0; n < DetailAttribCount; n++)
	{
	    std::string AttribName = SM.Session->GetHAPIString(SH_Names[n]);
	    if (AttribName.compare(0, MetadataStr.length(), MetadataStr) == 0)
	    {
		HAPI_AttributeInfo AttribInfo;
		if (HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId,
                                    Info.id,
				    AttribName.c_str(),
				    HAPI_ATTROWNER_DETAIL, &AttribInfo))
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
					    HAPI_ATTROWNER_DETAIL);
				HEMAX_MeshList<float>& Metadata = PartMesh->GetFloatMetadata(AttribName);
                                HEMAX_HoudiniApi::GetAttributeFloatData(
                                                SM.Session, NodeId,
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
					    HAPI_ATTROWNER_DETAIL);
				HEMAX_MeshList<int>& Metadata = PartMesh->GetIntMetadata(AttribName);
                                HEMAX_HoudiniApi::GetAttributeIntData(
                                                SM.Session, NodeId,
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
					    HAPI_ATTROWNER_DETAIL);
				HEMAX_MeshList<std::string>& Metadata = PartMesh->GetStringMetadata(AttribName);
				std::vector<HAPI_StringHandle> StringHandles(1);
				if (HEMAX_HoudiniApi::GetAttributeStringData(
                                                    SM.Session, NodeId,
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
        HEMAX_HoudiniApi::GetFaceCounts(SM.Session, NodeId, Info.id,
			PartMesh->GetFaceVertexCountsArray(), 0,
			Info.faceCount);
        HEMAX_HoudiniApi::GetVertexList(SM.Session, NodeId, Info.id,
			PartMesh->GetVertexListArray(), 0, Info.vertexCount);

	PartMesh->AllocateMaterialIdsArray();
        HEMAX_HoudiniApi::GetMaterialNodeIdsOnFaces(SM.Session, NodeId,
                Info.id, &PartMesh->AreMaterialIdsSame,
                PartMesh->GetMaterialIdsArray(), 0, Info.faceCount);

	// Main UV Layer

	HAPI_AttributeInfo UVPointAttributeInfo;
        HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
			HEMAX_UV_ATTRIBUTE, HAPI_ATTROWNER_POINT,
			&UVPointAttributeInfo);

	HAPI_AttributeInfo UVVertexAttributeInfo;
        HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
			HEMAX_UV_ATTRIBUTE, HAPI_ATTROWNER_VERTEX,
			&UVVertexAttributeInfo);

	if (UVPointAttributeInfo.exists)
	{
	    PartMesh->AllocatePointUVArray(UVPointAttributeInfo.tupleSize);
            HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
			    HEMAX_UV_ATTRIBUTE, &UVPointAttributeInfo, -1,
			    PartMesh->GetPointUVArray(), 0, Info.pointCount);
	}

	if (UVVertexAttributeInfo.exists)
	{
	    PartMesh->AllocateVertexUVArray(UVVertexAttributeInfo.tupleSize);
            HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, Info.id,
			    HEMAX_UV_ATTRIBUTE, &UVVertexAttributeInfo, -1,
			    PartMesh->GetVertexUVArray(), 0, Info.vertexCount);
	}

	// Secondary UV Layers

	for (int UVLayer = 2; UVLayer < MAX_MESHMAPS; UVLayer++)
	{
	    std::string UVName = HEMAX_UV_ATTRIBUTE + std::to_string(UVLayer);

	    HAPI_AttributeInfo UVPointInfo;
            HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
                            UVName.c_str(), HAPI_ATTROWNER_POINT, &UVPointInfo);
	    HAPI_AttributeInfo UVVertexInfo;
            HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, Info.id,
                            UVName.c_str(), HAPI_ATTROWNER_VERTEX, &UVVertexInfo);

	    if (UVPointInfo.exists)
	    {
                PartMesh->CreateSecondaryUVLayer(UVLayer, UVPointInfo);

                HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId,
                    Info.id, UVName.c_str(), &UVPointInfo, -1,
                    PartMesh->GetSecondaryPointUVArray(UVLayer),
                    0, UVPointInfo.count);
	    }

	    if (UVVertexInfo.exists)
	    {
                PartMesh->CreateSecondaryUVLayer(UVLayer, UVVertexInfo);

                HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId,
                    Info.id, UVName.c_str(), &UVVertexInfo, -1,
                    PartMesh->GetSecondaryVertexUVArray(UVLayer),
                    0, UVVertexInfo.count);
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

	bool Success = HEMAX_HoudiniApi::GetInstancerPartTransforms(
                            SM.Session,
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
    HEMAX_HoudiniApi::GetGeometryInfo(SM.Session, NodeId, &GeometryInfo);

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

    HEMAX_HoudiniApi::GetGroupNames(SM.Session, NodeId, GroupType,
        GroupNameHandles.data(), (int)GroupNameHandles.size());

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
