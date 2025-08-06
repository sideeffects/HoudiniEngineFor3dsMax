#include "HEMAX_Mesh.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Utilities.h"

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <MeshNormalSpec.h>
#include <MNNormalSpec.h>
#pragma warning(pop)

#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <deque>

const int HEMAX_MNMAP_ALPHA_ID          = -2;
const int HEMAX_MNMAP_ILLUMINATION_ID   = -1;
const int HEMAX_MNMAP_CD_ID             = 0;
const int HEMAX_MNMAP_UV_ID             = 1;

template<typename T>
void
HEMAX_MeshList<T>::Init(std::size_t _Size, std::size_t _TupleSize,
        HAPI_AttributeOwner _Owner)
{
    List.resize(_Size * _TupleSize);
    Size = _Size;
    TupleSize = _TupleSize;
    Owner = _Owner;
    Exists = true;
}

template<typename T>
T*
HEMAX_MeshList<T>::Data()
{
    return List.data();
}

template<typename T>
const T*
HEMAX_MeshList<T>::DataConst() const
{
    return List.data();
}

template<typename T>
void
HEMAX_MeshList<T>::Value(int Index, std::vector<T>& DataOut)
{
    DataOut.clear();
    for (unsigned int i = 0; i < TupleSize; i++)
    {
	DataOut.push_back(List[(Index * TupleSize) + i]);
    }
}

template<typename T>
std::size_t
HEMAX_MeshList<T>::DataSize()
{
    return Size;
}

template<typename T>
std::size_t
HEMAX_MeshList<T>::DataTupleSize()
{
    return TupleSize;
}

template<typename T>
HAPI_AttributeOwner
HEMAX_MeshList<T>::DataOwner()
{
    return Owner;
}

template<typename T>
void
HEMAX_MeshList<T>::MergeEqualTuples()
{
    if (IsMerged)
        return;

    struct vecHash {
        size_t operator()(const std::vector<T>& Tuple) const
        {
            size_t hash = (std::hash<T>()(Tuple[0]) << 1);
            for (int i = 1; i < Tuple.size(); ++i)
            {
                hash ^= (std::hash<T>()(Tuple[i]) << (i+1));
            }
            return hash;
        }
    };

    std::unordered_map<std::vector<T>, int, vecHash> SetIndexMap;

    for (unsigned int i = 0; i < Size; i++)
    {
        std::vector<T> vecTuple(&List[i*TupleSize], &List[i*TupleSize + TupleSize]);

	auto Search = SetIndexMap.find(vecTuple);

        if (Search == SetIndexMap.end())
        {
	    unsigned int SetIndex = (unsigned int)TupleSet.size()/TupleSize;
	    SetIndexMap.insert({vecTuple, SetIndex});

	    for (unsigned int t = 0; t < TupleSize; t++)
	    {
		TupleSet.push_back(List[(i*TupleSize)+t]);
            }
            
            IndexMapping.push_back(SetIndex);
        }
        else
        {
            IndexMapping.push_back(Search->second);
        }
    }	

    IsMerged = true;
}

template<typename T>
const T*
HEMAX_MeshList<T>::MergedDataConst() const
{
    return TupleSet.data();
}

template<typename T>
std::size_t
HEMAX_MeshList<T>::MergedDataSize()
{
    return (unsigned int)TupleSet.size()/TupleSize;
}

template<typename T>
std::size_t
HEMAX_MeshList<T>::GetMergedIndex(int Index)
{
    return IndexMapping[Index];
}

template<typename T>
void
HEMAX_MeshList<T>::Clear()
{
    List.clear();
    Owner = HAPI_ATTROWNER_INVALID;
    Size = 0;
    TupleSize = 0;
    TupleSet.clear();
    IndexMapping.clear();
    Exists = false;
    IsMerged = false;
}

void
HEMAX_Mesh::Clear()
{
    MyNumPoints = 0;
    MyNumVertices = 0;
    MyNumFaces = 0;

    MyMaxMapLayer = -1;

    MyPositions.Clear();
    MyFaceCounts.clear();
    MyVertices.clear();
    MyNormals.Clear();
    MyPrimaryUVs.Clear();
    MyColors.Clear();
    MyAlpha.Clear();
    MyIllumination.Clear();
    MySmoothingGroups.Clear();
    MyMaterialIds.Clear();
    MyMaterialNodeIds.Clear();

    MyFaceSelections.clear();
    MyVertexSelections.clear();
    MyEdgeSelections.clear();

    MySecondaryUVs.clear();

    MyMaterialPath = "";
    MyNumMaterials = 0;

    MyIntMetadata.clear();
    MyFloatMetadata.clear();
    MyStringMetadata.clear();
}

void
HEMAX_Mesh::InitFromPart(const HAPI_NodeId Node, const HAPI_PartInfo& PartInfo)
{
    SetPointCount(PartInfo.pointCount);
    SetVertexCount(PartInfo.vertexCount);
    SetFaceCount(PartInfo.faceCount);

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_AttributeInfo AttrInfo;

    HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_POSITION, HAPI_ATTROWNER_POINT, &AttrInfo);
    MyPositions.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_POINT);
    HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_POSITION, &AttrInfo, -1, MyPositions.Data(),
            0, MyPositions.DataSize());

    HEMAX_HoudiniApi::GetFaceCounts(&SM.Session, Node, PartInfo.id,
            MyFaceCounts.data(), 0, PartInfo.faceCount);
    HEMAX_HoudiniApi::GetVertexList(&SM.Session, Node, PartInfo.id,
            MyVertices.data(), 0, PartInfo.vertexCount);

    // Attribute: N
    if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_NORMAL, HAPI_ATTROWNER_POINT, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyNormals.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_POINT);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_NORMAL, &AttrInfo, -1, MyNormals.Data(),
            0, MyNormals.DataSize());
    }
    else if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_NORMAL, HAPI_ATTROWNER_VERTEX, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyNormals.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_VERTEX);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_NORMAL, &AttrInfo, -1, MyNormals.Data(),
            0, MyNormals.DataSize());
    }

    // Attribute: uv
    if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_UV, HAPI_ATTROWNER_POINT, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyPrimaryUVs.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_POINT);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_UV, &AttrInfo, -1, MyPrimaryUVs.Data(),
            0, MyPrimaryUVs.DataSize());

        if (MyMaxMapLayer < HEMAX_MNMAP_UV_ID)
            MyMaxMapLayer = HEMAX_MNMAP_UV_ID;
    }
    else if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_UV, HAPI_ATTROWNER_VERTEX, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyPrimaryUVs.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_VERTEX);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_UV, &AttrInfo, -1, MyPrimaryUVs.Data(),
            0, MyPrimaryUVs.DataSize());

        if (MyMaxMapLayer < HEMAX_MNMAP_UV_ID)
            MyMaxMapLayer = HEMAX_MNMAP_UV_ID;
    }

    for (std::size_t l = 2; l < MAX_MESHMAPS; ++l)
    {
        std::string AttribName = std::string(HAPI_ATTRIB_UV) + std::to_string(l);

        if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
                AttribName.c_str(), HAPI_ATTROWNER_POINT, &AttrInfo) == HAPI_RESULT_SUCCESS
                && AttrInfo.exists)
        {
            MySecondaryUVs[l].Init(AttrInfo.count, AttrInfo.tupleSize,
                    HAPI_ATTROWNER_POINT);
            HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node,
                    PartInfo.id, AttribName.c_str(), &AttrInfo, -1,
                    MySecondaryUVs[l].Data(), 0, MySecondaryUVs[l].DataSize());

            if (MyMaxMapLayer < l)
                MyMaxMapLayer = l;
        }
        else if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
                    AttribName.c_str(), HAPI_ATTROWNER_VERTEX, &AttrInfo) == HAPI_RESULT_SUCCESS
                    && AttrInfo.exists)
        {
            MySecondaryUVs[l].Init(AttrInfo.count, AttrInfo.tupleSize,
                    HAPI_ATTROWNER_VERTEX);
            HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node,
                    PartInfo.id, AttribName.c_str(), &AttrInfo, -1,
                    MySecondaryUVs[l].Data(), 0, MySecondaryUVs[l].DataSize());

            if (MyMaxMapLayer < l)
                MyMaxMapLayer = l;
        }
    }

    // Attribute: Cd
    if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_COLOR, HAPI_ATTROWNER_POINT, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyColors.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_POINT);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_COLOR, &AttrInfo, -1, MyColors.Data(),
            0, MyColors.DataSize());

        if (MyMaxMapLayer < HEMAX_MNMAP_CD_ID)
            MyMaxMapLayer = HEMAX_MNMAP_CD_ID;
    }
    else if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_COLOR, HAPI_ATTROWNER_VERTEX, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyColors.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_VERTEX);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HAPI_ATTRIB_COLOR, &AttrInfo, -1, MyColors.Data(),
            0, MyColors.DataSize());

        if (MyMaxMapLayer < HEMAX_MNMAP_CD_ID)
            MyMaxMapLayer = HEMAX_MNMAP_CD_ID;
    }

    // Attribute: Alpha
    if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_ALPHA, HAPI_ATTROWNER_POINT, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyAlpha.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_POINT);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_ALPHA, &AttrInfo, -1, MyAlpha.Data(),
            0, MyAlpha.DataSize());
    }
    else if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_ALPHA, HAPI_ATTROWNER_VERTEX, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyAlpha.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_VERTEX);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_ALPHA, &AttrInfo, -1, MyAlpha.Data(),
            0, MyAlpha.DataSize());
    }

    // Attribute: illumination
    if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_ILLUMINATION, HAPI_ATTROWNER_POINT, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyIllumination.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_POINT);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_ILLUMINATION, &AttrInfo, -1, MyIllumination.Data(),
            0, MyIllumination.DataSize());
    }
    else if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_ILLUMINATION, HAPI_ATTROWNER_VERTEX, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyIllumination.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_VERTEX);
        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_ILLUMINATION, &AttrInfo, -1, MyIllumination.Data(),
            0, MyIllumination.DataSize());
    }

    // Attribute: hemax_sg
    if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_SMOOTHING_GROUP, HAPI_ATTROWNER_PRIM, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MySmoothingGroups.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_PRIM);
        HEMAX_HoudiniApi::GetAttributeIntData(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_SMOOTHING_GROUP, &AttrInfo, -1, MySmoothingGroups.Data(),
            0, MySmoothingGroups.DataSize());
    }

    // Attribute: Material Node Ids (shop_materialpath)
    MyMaterialNodeIds.Init(PartInfo.faceCount, 1, HAPI_ATTROWNER_PRIM);
    bool SameMaterials;
    if (HEMAX_HoudiniApi::GetMaterialNodeIdsOnFaces(&SM.Session, Node,
            PartInfo.id, &SameMaterials, MyMaterialNodeIds.Data(),
            0, PartInfo.faceCount) == HAPI_RESULT_SUCCESS)
    {
        std::unordered_set<HAPI_NodeId> SeenMatNodes;
        for (std::size_t i = 0; i < MyMaterialNodeIds.DataSize(); ++i)
        {
            if (SeenMatNodes.count(MyMaterialNodeIds.Data()[i]) == 0)
            {
                SeenMatNodes.insert(MyMaterialNodeIds.Data()[i]);
                ++MyNumMaterials;
            }
        }
    }

    // Attribute: hemax_matid
    if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_MATERIAL_ID, HAPI_ATTROWNER_PRIM, &AttrInfo) == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        MyMaterialIds.Init(AttrInfo.count, AttrInfo.tupleSize, HAPI_ATTROWNER_PRIM);
        HEMAX_HoudiniApi::GetAttributeIntData(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_MATERIAL_ID, &AttrInfo, -1, MyMaterialIds.Data(),
            0, MyMaterialIds.DataSize());
    }

    // Attribute: hemax_material
    if (HEMAX_HoudiniApi::GetAttributeInfo(&SM.Session, Node, PartInfo.id,
            HEMAX_ATTRIB_MATERIAL_PATH, HAPI_ATTROWNER_DETAIL, &AttrInfo)  == HAPI_RESULT_SUCCESS
            && AttrInfo.exists)
    {
        HAPI_StringHandle MatPathSH;
        HEMAX_HoudiniApi::GetAttributeStringData(&SM.Session, Node, PartInfo.id,
                HEMAX_ATTRIB_MATERIAL_PATH, &AttrInfo, &MatPathSH, 0, 1);
        MyMaterialPath = HEMAX_Utilities::GetHAPIString(MatPathSH);
    }

    HAPI_GeoInfo GeoInfo;
    HEMAX_HoudiniApi::GetGeometryInfo(&SM.Session, Node, &GeoInfo);

    // Group: hemax_face_selection
    if (HEMAX_Utilities::GeoHasGroup(GeoInfo, HEMAX_SELECTION_FACE, HAPI_GROUPTYPE_PRIM))
    {
        int ElemCount = HEMAX_HoudiniApi::PartInfo_GetElementCountByGroupType(
                const_cast<HAPI_PartInfo*>(&PartInfo),
                HAPI_GROUPTYPE_PRIM);
        MyFaceSelections.resize(ElemCount);

        HAPI_Bool MembershipAllEqual;
        HEMAX_HoudiniApi::GetGroupMembership(&SM.Session, Node, PartInfo.id,
                HAPI_GROUPTYPE_PRIM, HEMAX_SELECTION_FACE, &MembershipAllEqual,
                MyFaceSelections.data(), 0, ElemCount);
    }
    
    // Group: hemax_vertex_selection
    if (HEMAX_Utilities::GeoHasGroup(GeoInfo, HEMAX_SELECTION_VERTEX, HAPI_GROUPTYPE_POINT))
    {
        int ElemCount = HEMAX_HoudiniApi::PartInfo_GetElementCountByGroupType(
                const_cast<HAPI_PartInfo*>(&PartInfo),
                HAPI_GROUPTYPE_POINT);
        MyVertexSelections.resize(ElemCount);

        HAPI_Bool MembershipAllEqual;
        HEMAX_HoudiniApi::GetGroupMembership(&SM.Session, Node, PartInfo.id,
                HAPI_GROUPTYPE_POINT, HEMAX_SELECTION_VERTEX, &MembershipAllEqual,
                MyVertexSelections.data(), 0, ElemCount);
    }
    
    // Group: hemax_edge_selection
    if (HEMAX_Utilities::GeoHasGroup(GeoInfo, HEMAX_SELECTION_EDGE, HAPI_GROUPTYPE_EDGE))
    {
        int ElemCount = 0;
        HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroup(&SM.Session, Node, PartInfo.id,
                HEMAX_SELECTION_EDGE, &ElemCount);

        if (ElemCount > 0)
        {
            MyEdgeSelections.resize(ElemCount*2);
            HAPI_Bool MembershipAllEqual;
            HEMAX_HoudiniApi::GetGroupMembership(&SM.Session, Node, PartInfo.id,
                    HAPI_GROUPTYPE_EDGE, HEMAX_SELECTION_EDGE,
                    &MembershipAllEqual, MyEdgeSelections.data(),
                    0, MyEdgeSelections.size());
        }
    }

    // Detail metadata attributes
    if (PartInfo.attributeCounts[HAPI_ATTROWNER_DETAIL] > 0)
    {
        std::vector<HAPI_StringHandle> Handles;
        Handles.resize(PartInfo.attributeCounts[HAPI_ATTROWNER_DETAIL]); 

        HEMAX_HoudiniApi::GetAttributeNames(&SM.Session, Node, PartInfo.id,
                HAPI_ATTROWNER_DETAIL, Handles.data(), Handles.size());

        std::string MetadataPrefix(HEMAX_ATTRIBPREFIX_METADATA);
        for (std::size_t i = 0; i < Handles.size(); ++i)
        {
            std::string AttribName = HEMAX_Utilities::GetHAPIString(Handles[i]);

            if (AttribName.compare(0, MetadataPrefix.size(), MetadataPrefix) != 0)
                continue;

            HAPI_AttributeInfo AttrInfo;
            HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeInfo(
                    &SM.Session, Node, PartInfo.id,
                    AttribName.c_str(), HAPI_ATTROWNER_DETAIL, &AttrInfo);
            
            if (Result != HAPI_RESULT_SUCCESS)
                continue;

            switch (AttrInfo.storage)
            {
                case HAPI_STORAGETYPE_FLOAT:
                    if (AddMetadata(MetadataType::FLOAT, AttribName, AttrInfo))
                    {
                        HEMAX_HoudiniApi::GetAttributeFloatData(&SM.Session,
                            Node, PartInfo.id, AttribName.c_str(), &AttrInfo,
                            -1, MyFloatMetadata[AttribName].Data(),
                            0, AttrInfo.count); 
                    }
                    break;
                case HAPI_STORAGETYPE_INT:
                    if (AddMetadata(MetadataType::INT, AttribName, AttrInfo))
                    {
                        HEMAX_HoudiniApi::GetAttributeIntData(&SM.Session,
                            Node, PartInfo.id, AttribName.c_str(), &AttrInfo,
                            -1, MyIntMetadata[AttribName].Data(),
                            0, AttrInfo.count);
                    }
                    break;
                case HAPI_STORAGETYPE_STRING:
                    if (AddMetadata(MetadataType::STRING, AttribName, AttrInfo))
                    {
                        std::vector<HAPI_StringHandle> Handles;
                        Handles.resize(MyStringMetadata[AttribName].DataSize(),
                                MyStringMetadata[AttribName].DataTupleSize());
                        HEMAX_HoudiniApi::GetAttributeStringData(&SM.Session,
                            Node, PartInfo.id, AttribName.c_str(), &AttrInfo,
                            Handles.data(), 0, AttrInfo.count);

                        for (std::size_t t = 0; t < Handles.size(); ++t)
                        {
                            HEMAX_MeshList<std::string>& StringList =
                                MyStringMetadata[AttribName];
                            StringList.Data()[t] =
                                HEMAX_Utilities::GetHAPIString(Handles[t]);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void
HEMAX_Mesh::BuildMNMesh(MNMesh& MaxMesh)
{
    MaxMesh.ClearAndFree();

    float ScaleConversion = HEMAX_Utilities::GetHoudiniToMaxScale();

    MaxMesh.setNumVerts(MyNumPoints);
    MaxMesh.setNumFaces(MyNumFaces);
    MaxMesh.SetMapNum(MyMaxMapLayer + 1);

    MNNormalSpec* NormalSpec = nullptr;
    MNMap *UvMap = nullptr,
          *CdMap = nullptr,
          *AlphaMap = nullptr,
          *IlluminationMap = nullptr;

    if (MyNormals.DataExists())
    {
        if (MyNormals.DataOwner() == HAPI_ATTROWNER_VERTEX)
        {
            MaxMesh.SpecifyNormals();
            NormalSpec = MaxMesh.GetSpecifiedNormals();

            if (NormalSpec)
            {
                NormalSpec->ClearNormals();
                NormalSpec->SetNumFaces(MyNumFaces);
            }
        }
    }

    if (MyPrimaryUVs.DataExists())
    {
        UvMap = MaxMesh.M(HEMAX_MNMAP_UV_ID);

        if (!UvMap)
        {
            MaxMesh.InitMap(HEMAX_MNMAP_UV_ID);
            UvMap = MaxMesh.M(HEMAX_MNMAP_UV_ID);
        }

        if (UvMap)
        {
            UvMap->ClearAllFlags();

            if (MyPrimaryUVs.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                UvMap->setNumFaces(MyNumFaces);
                UvMap->setNumVerts(MyNumPoints);
            }
            else if (MyPrimaryUVs.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                MyPrimaryUVs.MergeEqualTuples();
                UvMap->setNumFaces(MyNumFaces);
                UvMap->setNumVerts(MyNumVertices);
                
                std::size_t TupleSize = MyPrimaryUVs.DataTupleSize();
                for (std::size_t t = 0; t < MyPrimaryUVs.MergedDataSize(); ++t)
                {
                    UvMap->v[t].x = MyPrimaryUVs.MergedDataConst()[t*TupleSize];

                    if (TupleSize > 1)
                        UvMap->v[t].y = MyPrimaryUVs.MergedDataConst()[t*TupleSize+1];
                    else
                        UvMap->v[t].y = 0.0f;

                    if (TupleSize > 2)
                        UvMap->v[t].z = MyPrimaryUVs.MergedDataConst()[t*TupleSize+2];
                    else
                        UvMap->v[t].z = 0.0f;
                }
            }
            else
            {
                // It should be impossible to end up here.
                assert(false);
            }
        }
    }

    for (auto&& it = MySecondaryUVs.begin(); it != MySecondaryUVs.end(); ++it)
    {
        std::size_t UvLayer = it->first;

        MNMap* Map = MaxMesh.M(UvLayer);

        if (!Map)
        {
            MaxMesh.InitMap(UvLayer);
            Map = MaxMesh.M(UvLayer);
        }

        if (Map)
        {
            Map->ClearAllFlags();

            HEMAX_MeshList<float>& UvList = it->second;

            if (UvList.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                Map->setNumFaces(MyNumFaces);
                Map->setNumVerts(MyNumPoints);
            }
            else if (UvList.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                UvList.MergeEqualTuples();
                Map->setNumFaces(MyNumFaces);
                Map->setNumVerts(MyNumVertices);

                std::size_t TupleSize = UvList.DataTupleSize();
                for (std::size_t t = 0; t < UvList.MergedDataSize(); ++t)
                {
                    Map->v[t].x = UvList.MergedDataConst()[t*TupleSize];

                    if (TupleSize > 1)
                        Map->v[t].y = UvList.MergedDataConst()[t*TupleSize+1];
                    else
                        Map->v[t].y = 0.0f;

                    if (TupleSize > 2)
                        Map->v[t].z = UvList.MergedDataConst()[t*TupleSize+2];
                    else
                        Map->v[t].z = 0.0f;
                }
            }
        }
    }

    if (MyColors.DataExists())
    {
        CdMap = MaxMesh.M(HEMAX_MNMAP_CD_ID);

        if (!CdMap)
        {
            MaxMesh.InitMap(HEMAX_MNMAP_CD_ID);
            CdMap = MaxMesh.M(HEMAX_MNMAP_CD_ID);
        }

        if (CdMap)
        {
            CdMap->ClearAllFlags();

            if (MyColors.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                CdMap->setNumFaces(MyNumFaces);
                CdMap->setNumVerts(MyNumPoints);
            }
            else if (MyColors.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                CdMap->setNumFaces(MyNumFaces);
                CdMap->setNumVerts(MyNumVertices);
            }
            else
            {
                // It should be impossible to end up here
                assert(false);
            }
        }
    }

    if (MyAlpha.DataExists())
    {
        AlphaMap = MaxMesh.M(HEMAX_MNMAP_ALPHA_ID);

        if (!AlphaMap)
        {
            MaxMesh.InitMap(HEMAX_MNMAP_ALPHA_ID);
            AlphaMap = MaxMesh.M(HEMAX_MNMAP_ALPHA_ID);
        }

        if (AlphaMap)
        {
            AlphaMap->ClearAllFlags();

            if (MyAlpha.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                AlphaMap->setNumFaces(MyNumFaces);
                AlphaMap->setNumVerts(MyNumPoints);
            }
            else if (MyAlpha.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                AlphaMap->setNumFaces(MyNumFaces);
                AlphaMap->setNumVerts(MyNumVertices);
            }
            else
            {
                // It should be impossible to end up here
                assert(false);
            }
        }
    }

    if (MyIllumination.DataExists())
    {
        IlluminationMap = MaxMesh.M(HEMAX_MNMAP_ILLUMINATION_ID);

        if (!IlluminationMap)
        {
            MaxMesh.InitMap(HEMAX_MNMAP_ILLUMINATION_ID);
            IlluminationMap = MaxMesh.M(HEMAX_MNMAP_ILLUMINATION_ID);
        }

        if (IlluminationMap)
        {
            IlluminationMap->ClearAllFlags();

            if (MyIllumination.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                IlluminationMap->setNumFaces(MyNumFaces);
                IlluminationMap->setNumVerts(MyNumPoints);
            }
            else if (MyIllumination.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                IlluminationMap->setNumFaces(MyNumFaces);
                IlluminationMap->setNumVerts(MyNumVertices);
            }
            else
            {
                // It should be impossible to end up here
                assert(false);
            }
        }
    }

    BitArray VertexSelections;

    if (MyVertexSelections.size() > 0)
        VertexSelections.SetSize(MyVertexSelections.size());

    for (std::size_t p = 0; p < MyNumPoints; ++p)
    {
        float* Pos = MyPositions.Data()+(p*MyPositions.DataTupleSize());
        MaxMesh.v[p].p =
            Point3(Pos[0]*ScaleConversion,
                   -Pos[2]*ScaleConversion,
                   Pos[1]*ScaleConversion);

        if (MyVertexSelections.size() > 0)
            VertexSelections.Set(p, MyVertexSelections[p]);

        if (UvMap && MyPrimaryUVs.DataOwner() == HAPI_ATTROWNER_POINT)
        {
            std::size_t TupleSize = MyPrimaryUVs.DataTupleSize();
            UvMap->v[p].x = MyPrimaryUVs.DataConst()[p*TupleSize];

            if (TupleSize > 1)
                UvMap->v[p].y = MyPrimaryUVs.DataConst()[p*TupleSize+1];
            else
                UvMap->v[p].y = 0.0f;

            if (TupleSize > 2)
                UvMap->v[p].z = MyPrimaryUVs.DataConst()[p*TupleSize+2];
            else
                UvMap->v[p].z = 0.0f;
        }

        if (CdMap && MyColors.DataOwner() == HAPI_ATTROWNER_POINT)
        {
            CdMap->v[p].x = MyColors.Data()[p*3];
            CdMap->v[p].y = MyColors.Data()[p*3+1];
            CdMap->v[p].z = MyColors.Data()[p*3+2];
        }

        if (AlphaMap && MyAlpha.DataOwner() == HAPI_ATTROWNER_POINT)
        {
            AlphaMap->v[p].x = MyAlpha.Data()[p];
            AlphaMap->v[p].y = MyAlpha.Data()[p];
            AlphaMap->v[p].z = MyAlpha.Data()[p];
        }

        if (IlluminationMap && MyIllumination.DataOwner() == HAPI_ATTROWNER_POINT)
        {
            IlluminationMap->v[p].x = MyIllumination.Data()[p*3];
            IlluminationMap->v[p].y = MyIllumination.Data()[p*3+1];
            IlluminationMap->v[p].z = MyIllumination.Data()[p*3+2];
        }
    }

    if (MySecondaryUVs.size() > 0)
    {
        for (auto&& it = MySecondaryUVs.begin(); it != MySecondaryUVs.end(); ++it)
        {
            HEMAX_MeshList<float>& UvList = it->second;

            if (UvList.DataOwner() != HAPI_ATTROWNER_POINT)
                continue;

            MNMap* Map = MaxMesh.M(it->first);

            if (!Map)
                continue;

            for (std::size_t p = 0; p < MyNumPoints; ++p)
            {
                std::size_t TupleSize = UvList.DataTupleSize();

                Map->v[p].x = UvList.DataConst()[p*TupleSize];

                if (TupleSize > 1)
                    Map->v[p].y = UvList.DataConst()[p*TupleSize+1];
                else
                    Map->v[p].y = 0.0f;

                if (TupleSize > 2)
                    Map->v[p].z = UvList.DataConst()[p*TupleSize+2];
                else
                    Map->v[p].z = 0.0f;
            }
        }
    }

    BitArray FaceSelections;

    if (MyFaceSelections.size() > 0)
        FaceSelections.SetSize(MyFaceSelections.size());

    std::size_t VertIdx = 0;
    for (std::size_t f = 0; f < MyNumFaces; ++f)
    {
        MaxMesh.F(f)->SetDeg(MyFaceCounts[f]);

        if (MyFaceSelections.size() > 0)
            FaceSelections.Set(f, MyFaceSelections[f]);

        if (UvMap)
            UvMap->F(f)->SetSize(MyFaceCounts[f]);
        if (CdMap)
            CdMap->F(f)->SetSize(MyFaceCounts[f]);
        if (AlphaMap)
            AlphaMap->F(f)->SetSize(MyFaceCounts[f]);
        if (IlluminationMap)
            IlluminationMap->F(f)->SetSize(MyFaceCounts[f]);

        if (MySmoothingGroups.DataExists())
            MaxMesh.F(f)->smGroup = MySmoothingGroups.DataConst()[f];
        else
            MaxMesh.F(f)->smGroup = 1;

        if (MyMaterialIds.DataExists())
            MaxMesh.F(f)->material = MyMaterialIds.DataConst()[f];

        for (int v=0, done=0; done < MyFaceCounts[f]; ++done)
        {
            MaxMesh.F(f)->vtx[v] = MyVertices[VertIdx];

            if (NormalSpec && MyNormals.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                Point3 Normal = Point3(MyNormals.Data()[VertIdx*3],
                                       -MyNormals.Data()[VertIdx*3+2],
                                       MyNormals.Data()[VertIdx*3+1]);
                NormalSpec->SetNormal(f, v, Normal);
            }
            else if (NormalSpec && MyNormals.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                int Point = MyVertices[VertIdx];
                Point3 Normal = Point3(MyNormals.Data()[Point*3],
                                       -MyNormals.Data()[Point*3+2],
                                       MyNormals.Data()[Point*3+1]);
                NormalSpec->SetNormal(f, v, Normal);
            }

            if (UvMap && MyPrimaryUVs.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                UvMap->F(f)->tv[v] = MyPrimaryUVs.GetMergedIndex(VertIdx);
            }
            else if (UvMap && MyPrimaryUVs.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                UvMap->F(f)->tv[v] = MyVertices[VertIdx];
            }

            if (CdMap && MyColors.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                CdMap->v[VertIdx].x = MyColors.Data()[VertIdx*3];
                CdMap->v[VertIdx].y = MyColors.Data()[VertIdx*3+1];
                CdMap->v[VertIdx].z = MyColors.Data()[VertIdx*3+2];
                CdMap->F(f)->tv[v] = VertIdx;
            }
            else if (CdMap && MyColors.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                CdMap->F(f)->tv[v] = MyVertices[VertIdx];
            }

            if (AlphaMap && MyAlpha.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                AlphaMap->v[VertIdx].x = MyAlpha.Data()[VertIdx];
                AlphaMap->v[VertIdx].y = MyAlpha.Data()[VertIdx];
                AlphaMap->v[VertIdx].z = MyAlpha.Data()[VertIdx];
                AlphaMap->F(f)->tv[v] = VertIdx;
            }
            else if (AlphaMap && MyAlpha.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                AlphaMap->F(f)->tv[v] = MyVertices[VertIdx];
            }

            if (IlluminationMap && MyIllumination.DataOwner() == HAPI_ATTROWNER_VERTEX)
            {
                IlluminationMap->v[VertIdx].x = MyIllumination.Data()[VertIdx*3];
                IlluminationMap->v[VertIdx].y = MyIllumination.Data()[VertIdx*3+1];
                IlluminationMap->v[VertIdx].z = MyIllumination.Data()[VertIdx*3+2];
                IlluminationMap->F(f)->tv[v] = VertIdx;
            }
            else if (IlluminationMap && MyIllumination.DataOwner() == HAPI_ATTROWNER_POINT)
            {
                IlluminationMap->F(f)->tv[v] = MyVertices[VertIdx];
            }

            v = v==0 ? v+MyFaceCounts[f]-1 : v-1;
            ++VertIdx;
        }

        if (MyFaceCounts[f] > 4)
            MaxMesh.RetriangulateFace(f);
    }

    for (auto it = MySecondaryUVs.begin(); it != MySecondaryUVs.end(); ++it)
    {
        HEMAX_MeshList<float>& UvList = it->second;

        MNMap* Map = MaxMesh.M(it->first);

        if (!Map)
            continue;

        VertIdx = 0;
        for (std::size_t f = 0; f < MyNumFaces; ++f)
        {
            Map->F(f)->SetSize(MyFaceCounts[f]); 
            for (int v = 0, done = 0; done < MyFaceCounts[f]; ++done)
            {
                if (UvList.DataOwner() == HAPI_ATTROWNER_VERTEX)
                    Map->F(f)->tv[v] = UvList.GetMergedIndex(VertIdx);
                else if (UvList.DataOwner() == HAPI_ATTROWNER_POINT)
                    Map->F(f)->tv[v] = MyVertices[VertIdx];

                v = v==0 ? v+MyFaceCounts[f]-1 : v-1;
                ++VertIdx;
            }
        }
    }

    if (NormalSpec)
    {
        NormalSpec->SetAllExplicit();
        NormalSpec->BuildNormals();
        NormalSpec->NShrink();
    }

    MaxMesh.InvalidateGeomCache();
    MaxMesh.InvalidateTopoCache();
    MaxMesh.FillInMesh();

    MaxMesh.FaceSelect(FaceSelections);
    MaxMesh.VertexSelect(VertexSelections);

    if (MyEdgeSelections.size() > 0)
    {
        BitArray EdgeSelections;
        EdgeSelections.SetSize(MaxMesh.ENum());
        EdgeSelections.ClearAll();

        for (std::size_t e = 0; e < MyEdgeSelections.size() - 1; e += 2)
        {
            int EdgeNum = MaxMesh.FindEdgeFromVertToVert(
                    MyEdgeSelections[e], MyEdgeSelections[e+1]);
            if (EdgeNum > -1)
                EdgeSelections.Set(EdgeNum, 1);
        }

        MaxMesh.EdgeSelect(EdgeSelections);
    }

    MaxMesh.CollapseDeadStructs();
}

std::size_t
HEMAX_Mesh::GetFaceCount() const
{
    return MyNumFaces;
}

const std::string&
HEMAX_Mesh::GetMaterialPath() const
{
    return MyMaterialPath;
}
    
std::size_t
HEMAX_Mesh::GetNumMaterials() const
{
    return MyNumMaterials;
}

const HAPI_NodeId*
HEMAX_Mesh::GetMaterialNodeIds() const
{
    return MyMaterialNodeIds.DataConst();
}

void
HEMAX_Mesh::ApplyMetadataToINode(INode& Node) const
{
    for (auto&& It = MyIntMetadata.cbegin(); It != MyIntMetadata.cend(); It++)
    {
        const std::string& Name = It->first;
        const HEMAX_MeshList<int>& List = It->second;

        std::wstring WideName = HEMAX_Utilities::GetWideString(Name);
        Node.SetUserPropInt(WideName.c_str(), List.DataConst()[0]);
    }

    for (auto&& It = MyFloatMetadata.cbegin(); It != MyFloatMetadata.cend(); It++)
    {
        const std::string& Name = It->first;
        const HEMAX_MeshList<float>& List = It->second;

        std::wstring WideName = HEMAX_Utilities::GetWideString(Name);
        Node.SetUserPropFloat(WideName.c_str(), List.DataConst()[0]);
    }

    for (auto&& It = MyStringMetadata.cbegin(); It != MyStringMetadata.cend(); It++)
    {
        const std::string& Name = It->first;
        const HEMAX_MeshList<std::string>& List = It->second;

        std::wstring WideName = HEMAX_Utilities::GetWideString(Name);
        std::wstring WideValue = HEMAX_Utilities::GetWideString(List.DataConst()[0]);

        Node.SetUserPropString(WideName.c_str(), WideValue.c_str());
    }
}

void
HEMAX_Mesh::SetPointCount(std::size_t Count)
{
    MyNumPoints = Count;
}

void        
HEMAX_Mesh::SetVertexCount(std::size_t Count)
{
    MyNumVertices = Count;
    MyVertices.resize(MyNumVertices);
}

void
HEMAX_Mesh::SetFaceCount(std::size_t Count)
{
    MyNumFaces = Count;
    MyFaceCounts.resize(MyNumFaces);
}

bool
HEMAX_Mesh::AddMetadata(HEMAX_Mesh::MetadataType Type,
        const std::string& AttribName, const HAPI_AttributeInfo& AttribInfo)
{
    switch (Type)
    {
        case HEMAX_Mesh::MetadataType::INT:
            if (AttribInfo.count != 1 || AttribInfo.tupleSize != 1)
                return false;

            MyIntMetadata[AttribName].Init(AttribInfo.count,
                    AttribInfo.tupleSize, HAPI_ATTROWNER_DETAIL);

            return true;
        case HEMAX_Mesh::MetadataType::FLOAT:
            if (AttribInfo.count != 1 || AttribInfo.tupleSize != 1)
                return false;

            MyFloatMetadata[AttribName].Init(AttribInfo.count,
                    AttribInfo.tupleSize, HAPI_ATTROWNER_DETAIL);

            return true;
        case HEMAX_Mesh::MetadataType::STRING:
            if (AttribInfo.count != 1 || AttribInfo.tupleSize != 1)
                return false;

            MyStringMetadata[AttribName].Init(AttribInfo.count,
                    AttribInfo.tupleSize, HAPI_ATTROWNER_DETAIL);

            return true;
        default:
            return false;
    }
}
