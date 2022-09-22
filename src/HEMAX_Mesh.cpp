#include "HEMAX_Mesh.h"

#include "HEMAX_Utilities.h"
#include "MeshNormalSpec.h"
#include "MNNormalSpec.h"

#include <unordered_map>
#include <deque>

const int HEMAX_Mesh_PointTupleSize = 3;
const int HEMAX_Mesh_VectorTupleSize = 3;
const int HEMAX_Mesh_SingularTuple = 1;

template<typename T>
HEMAX_MeshList<T>::HEMAX_MeshList()
{
    List = nullptr;
    Owner = HAPI_ATTROWNER_INVALID;
    TupleSize = 0;
    IsMerged = false;
}

template<typename T>
HEMAX_MeshList<T>::~HEMAX_MeshList()
{
    if (List)
    {
	delete [] List;
    }
}

template<typename T>
void
HEMAX_MeshList<T>::Init(unsigned int _Size, unsigned int _TupleSize, HAPI_AttributeOwner _Owner)
{
    if (!List)
    {
	List = new T[_Size * _TupleSize];
	Size = _Size;
	TupleSize = _TupleSize;
	Owner = _Owner;
    }
}

template<typename T>
T*
HEMAX_MeshList<T>::Data()
{
    return List;
}

template<typename T>
std::vector<T>
HEMAX_MeshList<T>::Value(int Index)
{
    std::vector<T> Val;
    for (unsigned int i = 0; i < TupleSize; i++)
    {
	Val.push_back(List[(Index * TupleSize) + i]);
    }

    return Val;
}

template<typename T>
unsigned int
HEMAX_MeshList<T>::DataSize()
{
    return Size;
}

template<typename T>
unsigned int
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

    struct vec3Hash {
	size_t operator()(const std::tuple<T, T, T> &Tuple) const {
	    size_t hash1 = std::hash<T>()(std::get<0>(Tuple));
	    size_t hash2 = std::hash<T>()(std::get<1>(Tuple));
	    size_t hash3 = std::hash<T>()(std::get<2>(Tuple));
	    return ((hash1 << 1) ^ (hash2 << 2) ^ (hash3 << 3));
	}
    };

    std::unordered_map<std::tuple<T, T, T>, int, vec3Hash> SetIndexMap;

    for (unsigned int i = 0; i < Size; i++)
    {
	unsigned int SetIndex = -1;

	std::tuple<T, T, T> vecTuple = std::make_tuple(List[i*3],
						       List[i*3+1],
						       List[i*3+2]);

	auto Search = SetIndexMap.find(vecTuple);

	if (Search != SetIndexMap.end())
	{
	    SetIndex = Search->second;
	}
	
	if (SetIndex == -1)
	{
	    SetIndex = (unsigned int)TupleSet.size()/TupleSize;
	    SetIndexMap.insert({vecTuple, SetIndex});

	    for (unsigned int t = 0; t < TupleSize; t++)
	    {
		TupleSet.push_back(List[(i*TupleSize)+t]);
	    }	
	    
	}

	IndexMapping.push_back(SetIndex);
    }	

    IsMerged = true;
}

template<typename T>
unsigned int
HEMAX_MeshList<T>::MergedDataSize()
{
    return (unsigned int)TupleSet.size()/TupleSize;
}

template<typename T>
std::vector<T>&
HEMAX_MeshList<T>::MergedValues()
{
    return TupleSet;
}

template<typename T>
unsigned int
HEMAX_MeshList<T>::GetMergedIndex(int Index)
{
    return IndexMapping[Index];
}

HEMAX_Mesh::HEMAX_Mesh()
    : FaceCount( 0 )
    , VertexCount( 0 )
    , PointCount( 0 )
    , NormalType(HEMAX_NO_NORMAL)
    , NormalsExist( false )
    , UVTupleSize( 0 )
    , HasUVs( false )
    , UVType(HEMAX_NO_UV)
    , ColorAttrExists(false)
    , ColorAttrOwner(HAPI_ATTROWNER_INVALID)
    , AlphaAttrExists(false)
    , IlluminationAttrExists(false)
    , SmoothingGroupsExist(false)
    , MaterialIDsExist(false)
    , FaceSelectionsExist(false)
    , VertexSelectionsExist(false)
    , EdgeSelectionsExist(false)
    , SecondaryUVCount(0)
    , AreMaterialIdsSame(false)
{
}

HEMAX_Mesh::HEMAX_Mesh(int FCount, int VCount, int PCount)
    : FaceCount(FCount)
    , VertexCount(VCount)
    , PointCount(PCount)
    , NormalType(HEMAX_NO_NORMAL)
    , NormalsExist( false )
    , UVTupleSize ( 0 )
    , HasUVs( false )
    , UVType(HEMAX_NO_UV)
    , ColorAttrExists(false)
    , ColorAttrOwner(HAPI_ATTROWNER_INVALID)
    , AlphaAttrExists(false)
    , IlluminationAttrExists(false)
    , SmoothingGroupsExist(false)
    , MaterialIDsExist(false)
    , FaceSelectionsExist(false)
    , VertexSelectionsExist(false)
    , EdgeSelectionsExist(false)
    , SecondaryUVCount(0)
    , AreMaterialIdsSame(false)
{
    FaceVertexCounts.Init(FaceCount, HEMAX_Mesh_SingularTuple, HAPI_ATTROWNER_PRIM);
    VertexList.Init(VertexCount, HEMAX_Mesh_SingularTuple, HAPI_ATTROWNER_VERTEX);
    PointList.Init(PointCount, HEMAX_Mesh_PointTupleSize, HAPI_ATTROWNER_POINT);
}

void
HEMAX_Mesh::AllocatePointNormalArray()
{
    NormalType = HEMAX_POINT_NORMAL;
    Normals.Init(PointCount, HEMAX_Mesh_VectorTupleSize, HAPI_ATTROWNER_POINT);
}

void
HEMAX_Mesh::AllocateVertexNormalArray()
{
    NormalType = HEMAX_VERTEX_NORMAL;
    Normals.Init(VertexCount, HEMAX_Mesh_VectorTupleSize, HAPI_ATTROWNER_VERTEX);
}

void
HEMAX_Mesh::AllocateMaterialIdsArray()
{
    FaceMaterialIds.Init(FaceCount, HEMAX_Mesh_SingularTuple, HAPI_ATTROWNER_PRIM);
}

void
HEMAX_Mesh::AllocatePointUVArray(int TupleSize)
{
    UVList.Init(PointCount, TupleSize, HAPI_ATTROWNER_POINT);
    UVTupleSize = TupleSize;
    HasUVs = true;
    UVType = HEMAX_POINT_UV;
}

void
HEMAX_Mesh::AllocateVertexUVArray(int TupleSize)
{
    UVList.Init(VertexCount, TupleSize, HAPI_ATTROWNER_VERTEX);
    UVTupleSize = TupleSize;
    HasUVs = true;
    UVType = HEMAX_VERTEX_UV;
}

void
HEMAX_Mesh::AllocatePointCdArray()
{
    CdList.Init(PointCount, HEMAX_Mesh_VectorTupleSize, HAPI_ATTROWNER_POINT);
    ColorAttrExists = true;
    ColorAttrOwner = HAPI_ATTROWNER_POINT;
}

void
HEMAX_Mesh::AllocateVertexCdArray()
{
    CdList.Init(VertexCount, HEMAX_Mesh_VectorTupleSize, HAPI_ATTROWNER_VERTEX);
    ColorAttrExists = true;
    ColorAttrOwner = HAPI_ATTROWNER_VERTEX;
}

void
HEMAX_Mesh::AllocateAlphaArray(HAPI_AttributeOwner Owner)
{
    if (Owner == HAPI_ATTROWNER_POINT)
    {
	AlphaList.Init(PointCount, HEMAX_Mesh_SingularTuple, Owner);
	AlphaAttrExists = true;
	AlphaAttrOwner = Owner;
    }
    else if (Owner == HAPI_ATTROWNER_VERTEX)
    {
	AlphaList.Init(VertexCount, HEMAX_Mesh_SingularTuple, Owner);
	AlphaAttrExists = true;
	AlphaAttrOwner = Owner;
    }
}

void
HEMAX_Mesh::AllocateIlluminationArray(HAPI_AttributeOwner Owner)
{
    if (Owner == HAPI_ATTROWNER_POINT)
    {
	IlluminationList.Init(PointCount, HEMAX_Mesh_VectorTupleSize, Owner);
	IlluminationAttrExists = true;
	IlluminationAttrOwner = Owner;
    }
    else if (Owner == HAPI_ATTROWNER_VERTEX)
    {
	IlluminationList.Init(VertexCount, HEMAX_Mesh_VectorTupleSize, Owner);
	IlluminationAttrExists = true;
	IlluminationAttrOwner = Owner;
    }
}

void
HEMAX_Mesh::AllocateSmoothingGroupsArray()
{
    SmoothingGroupList.Init(FaceCount, HEMAX_Mesh_SingularTuple, HAPI_ATTROWNER_PRIM);
    SmoothingGroupsExist = true;
}

void
HEMAX_Mesh::AllocateMaterialIDArray()
{
    MaterialIDList.Init(FaceCount, HEMAX_Mesh_SingularTuple, HAPI_ATTROWNER_PRIM);
    MaterialIDsExist = true;
}

void
HEMAX_Mesh::AllocateFaceSelectionsArray()
{
    FaceSelectionsList.Init(FaceCount, HEMAX_Mesh_SingularTuple,
        HAPI_ATTROWNER_PRIM);
    FaceSelectionsExist = true;
}

void
HEMAX_Mesh::AllocateVertexSelectionsArray()
{
    VertexSelectionsList.Init(PointCount, HEMAX_Mesh_SingularTuple,
        HAPI_ATTROWNER_POINT);
    VertexSelectionsExist = true;
}

void
HEMAX_Mesh::AllocateEdgeSelectionsArray(int EdgeCount)
{
    EdgeSelectionsList.Init(EdgeCount*2, HEMAX_Mesh_SingularTuple,
        HAPI_ATTROWNER_POINT);
    EdgeSelectionsExist = true;
}

HEMAX_Mesh::~HEMAX_Mesh()
{
}

int
HEMAX_Mesh::GetFaceCount()
{
    return FaceCount;
}

int
HEMAX_Mesh::GetVertexCount()
{
    return VertexCount;
}

int
HEMAX_Mesh::GetPointCount()
{
    return PointCount;
}

int
HEMAX_Mesh::GetPointUVCount()
{
    return PointCount;
}

int
HEMAX_Mesh::GetVertexUVCount()
{
    return VertexCount;
}

int
HEMAX_Mesh::GetUVTupleSize()
{
    return UVTupleSize;
}

int
HEMAX_Mesh::GetNumMaterials()
{
    std::unordered_map<HAPI_NodeId, bool> SeenMaterials;
    int NumMaterials = 0;

    for (int f = 0; f < FaceCount; f++)
    {
	HAPI_NodeId MatId = FaceMaterialIds.Data()[f];
	auto Search = SeenMaterials.find(MatId);
	if (Search == SeenMaterials.end())
	{
	    NumMaterials++;
	    SeenMaterials.insert({ MatId, true });
	}
    }

    return NumMaterials;
}

int*
HEMAX_Mesh::GetFaceVertexCountsArray()
{
    return FaceVertexCounts.Data();
}

int*
HEMAX_Mesh::GetVertexListArray()
{
    return VertexList.Data();
}

float*
HEMAX_Mesh::GetPointListArray()
{
    return PointList.Data();
}

float*
HEMAX_Mesh::GetPointNormalsListArray()
{
    return Normals.Data();
}

float*
HEMAX_Mesh::GetVertexNormalsListArray()
{
    return Normals.Data();
}

HAPI_NodeId*
HEMAX_Mesh::GetMaterialIdsArray()
{
    return FaceMaterialIds.Data();
}

float*
HEMAX_Mesh::GetPointUVArray()
{
    return UVList.Data();
}

float*
HEMAX_Mesh::GetVertexUVArray()
{
    return UVList.Data();
}

float*
HEMAX_Mesh::GetSecondaryPointUVArray(int Layer)
{
    auto&& Search = SecondaryPointUVs.find(Layer);
    if (Search != SecondaryPointUVs.end())
        return Search->second.Data();

    return nullptr;
}

float*
HEMAX_Mesh::GetSecondaryVertexUVArray(int Layer)
{
    auto&& Search = SecondaryVertexUVs.find(Layer);
    if (Search != SecondaryVertexUVs.end())
        return Search->second.Data();

    return nullptr;
}

float*
HEMAX_Mesh::GetPointCdArray()
{
    return CdList.Data();
}

float*
HEMAX_Mesh::GetVertexCdArray()
{
    return CdList.Data();
}

float*
HEMAX_Mesh::GetAlphaArray()
{
    return AlphaList.Data();
}

float*
HEMAX_Mesh::GetIlluminationArray()
{
    return IlluminationList.Data();
}

int*
HEMAX_Mesh::GetSmoothingGroupArray()
{
    return SmoothingGroupList.Data();
}

int*
HEMAX_Mesh::GetMaterialIDArray()
{
    return MaterialIDList.Data();
}

int*
HEMAX_Mesh::GetFaceSelectionsArray()
{
    return FaceSelectionsList.Data();
}

int*
HEMAX_Mesh::GetVertexSelectionsArray()
{
    return VertexSelectionsList.Data();
}

int*
HEMAX_Mesh::GetEdgeSelectionsArray()
{
    return EdgeSelectionsList.Data();
}

int
HEMAX_Mesh::GetFaceVertexCount(int Index)
{
    std::vector<int> Count = FaceVertexCounts.Value(Index);
    if (Count.size() > 0)
    {
	return Count[0];
    }
    else
    {
	return 0;
    }
}

void
HEMAX_Mesh::GetPointAtIndex( int Index, float* Point )
{
    std::vector<float> Points = PointList.Value(Index);
    for ( int i = 0; i < Points.size(); ++i )
    {
	Point[ i ] = Points[i];
    }
}

void
HEMAX_Mesh::GetPointNormalAtIndex(int Index, float* Normal)
{
    std::vector<float> NormalVals = Normals.Value(Index);
    for (int i = 0; i < NormalVals.size(); i++)
    {
	Normal[i] = NormalVals[i];
    }
}

void 
HEMAX_Mesh::GetVertexNormalAtIndex(int Index, float* Normal)
{
    std::vector<float> NormalVals = Normals.Value(Index);
    for (int i = 0; i < NormalVals.size(); i++)
    {
	Normal[i] = NormalVals[i];
    }
}

int
HEMAX_Mesh::GetVertex( int Index )
{
    std::vector<int> Vertex = VertexList.Value(Index);
    if (Vertex.size() > 0)
    {
	return Vertex[0];
    }
    else
    {
	return 0;
    }
}

void
HEMAX_Mesh::GetPointUVAtIndex(int Index, float* UVVals)
{
    std::vector<float> UV = UVList.Value(Index);
    for (int i = 0; i < UV.size(); ++i)
    {
	UVVals[i] = UV[i];
    }
}

void
HEMAX_Mesh::GetPointCdAtIndex(int Index, float* CdVals)
{
    std::vector<float> Cd = CdList.Value(Index);
    for (int i = 0; i < Cd.size(); i++)
    {
	CdVals[i] = Cd[i];
    }
}

void
HEMAX_Mesh::GetVertexCdAtIndex(int Index, float* CdVals)
{
    std::vector<float> Cd = CdList.Value(Index);
    for (int i = 0; i < Cd.size(); i++)
    {
	CdVals[i] = Cd[i];
    }
}

float
HEMAX_Mesh::GetAlphaAtIndex(int Index)
{
    std::vector<float> Alpha = AlphaList.Value(Index);
    if (Alpha.size() > 0)
    {
	return Alpha[0];
    }
    else
    {
	return 0;
    }
}

void
HEMAX_Mesh::GetIlluminationAtIndex(int Index, float* IlluminationVals)
{
    std::vector<float> Illumination = IlluminationList.Value(Index);
    for (int i = 0; i < Illumination.size(); i++)
    {
	IlluminationVals[i] = Illumination[i];
    }
}

void
HEMAX_Mesh::SetNormalsExist(bool Exist)
{
    NormalsExist = Exist;
}

bool
HEMAX_Mesh::DoNormalsExist()
{
    return NormalsExist;
}

HEMAX_NormalType
HEMAX_Mesh::GetNormalType()
{
    return NormalType;
}

HEMAX_UVType
HEMAX_Mesh::GetUVType()
{
    return UVType;
}

bool
HEMAX_Mesh::DoUVsExist()
{
    return HasUVs;
}

bool
HEMAX_Mesh::DoesCdAttrExist()
{
    return ColorAttrExists;
}

HAPI_AttributeOwner
HEMAX_Mesh::GetCdAttrOwner()
{
    return ColorAttrOwner;
}

bool
HEMAX_Mesh::DoesAlphaAttrExist()
{
    return AlphaAttrExists;
}

HAPI_AttributeOwner
HEMAX_Mesh::GetAlphaAttrOwner()
{
    return AlphaAttrOwner;
}

bool
HEMAX_Mesh::DoesIlluminationAttrExist()
{
    return IlluminationAttrExists;
}

HAPI_AttributeOwner
HEMAX_Mesh::GetIlluminationOwner()
{
    return IlluminationAttrOwner;
}

bool
HEMAX_Mesh::DoesSmoothingGroupAttrExist()
{
    return SmoothingGroupsExist;
}

bool
HEMAX_Mesh::DoesMaterialIDAttrExist()
{
    return MaterialIDsExist;
}

bool
HEMAX_Mesh::HasFaceSelections() const
{
    return FaceSelectionsExist;
}

bool
HEMAX_Mesh::HasVertexSelections() const
{
    return VertexSelectionsExist;
}

bool
HEMAX_Mesh::HasEdgeSelections() const
{
    return EdgeSelectionsExist;
}

int
HEMAX_Mesh::GetPostTriangulationFaceCount()
{
    int TotalFaceCount = 0;

    for (int i = 0; i < GetFaceCount(); i++)
    {
	TotalFaceCount += (GetFaceVertexCount(i) - 2);
    }

    return TotalFaceCount;
}

void
HEMAX_Mesh::CreateSecondaryUVLayer(int Layer, const HAPI_AttributeInfo& Attr)
{
    if (Attr.owner == HAPI_ATTROWNER_POINT)
    {
        auto&& Item = SecondaryPointUVs.emplace(Layer, HEMAX_MeshList<float>()); 
        Item.first->second.Init(Attr.count, Attr.tupleSize, Attr.owner);
        ++SecondaryUVCount;
    }
    else if (Attr.owner == HAPI_ATTROWNER_VERTEX)
    {
        auto&& Item = SecondaryVertexUVs.emplace(Layer, HEMAX_MeshList<float>());
        Item.first->second.Init(Attr.count, Attr.tupleSize, Attr.owner);
        ++SecondaryUVCount;
    } 
}

HEMAX_MeshList<float>*
HEMAX_Mesh::GetSecondaryUVLayer(HAPI_AttributeOwner Owner, int Layer)
{
    if (Owner == HAPI_ATTROWNER_POINT)
    {
        auto&& Search = SecondaryPointUVs.find(Layer);
        if (Search != SecondaryPointUVs.end())
            return &Search->second;
    }
    else if (Owner == HAPI_ATTROWNER_VERTEX)
    {
        auto&& Search = SecondaryVertexUVs.find(Layer);
        if (Search != SecondaryVertexUVs.end())
            return &Search->second;
    }

    return nullptr;
}

bool
HEMAX_Mesh::DoesSecondaryUVLayerExist(HAPI_AttributeOwner Owner, int Layer)
{
    if (Owner == HAPI_ATTROWNER_POINT)
    {
	auto Search = SecondaryPointUVs.find(Layer);

	if (Search != SecondaryPointUVs.end())
	{
	    return true;
	}
	else
	{
	    return false;
	}
    }
    else if (Owner == HAPI_ATTROWNER_VERTEX)
    {
	auto Search = SecondaryVertexUVs.find(Layer);

	if (Search != SecondaryVertexUVs.end())
	{
	    return true;
	}
	else
	{
	    return false;
	}
    }

    return false;
}

void
HEMAX_Mesh::AddMetadata(std::string Name, HEMAX_Mesh_MetadataType Type, unsigned int Size, unsigned int TupleSize, HAPI_AttributeOwner Owner)
{
    switch (Type)
    {
	case HEMAX_MESH_INT_METADATA:
	{
	    IntMetadata[Name] = HEMAX_MeshList<int>();
	    IntMetadata[Name].Init(Size, TupleSize, Owner);
	} break;
	case HEMAX_MESH_FLOAT_METADATA:
	{
	    FloatMetadata[Name] = HEMAX_MeshList<float>();
	    FloatMetadata[Name].Init(Size, TupleSize, Owner);
	} break;
	case HEMAX_MESH_STRING_METADATA:
	{
	    StringMetadata[Name] = HEMAX_MeshList<std::string>();
	    StringMetadata[Name].Init(Size, TupleSize, Owner);
	} break;
	default:
	{

	} break;
    }
}

HEMAX_MeshList<int>&
HEMAX_Mesh::GetIntMetadata(std::string Name)
{
    return IntMetadata.find(Name)->second;
}

HEMAX_MeshList<float>&
HEMAX_Mesh::GetFloatMetadata(std::string Name)
{
    return FloatMetadata.find(Name)->second;
}

HEMAX_MeshList<std::string>&
HEMAX_Mesh::GetStringMetadata(std::string Name)
{
    return StringMetadata.find(Name)->second;
}

void
HEMAX_Mesh::ApplyDetailMetadata(INode* Node)
{
    for (auto It = IntMetadata.begin(); It != IntMetadata.end(); It++)
    {
	HEMAX_MeshList<int> &MeshList = It->second;
	if (MeshList.DataOwner() == HAPI_ATTROWNER_DETAIL)
	{
	    if (MeshList.DataSize() == 1 && MeshList.DataTupleSize() == 1)
	    {
		std::wstring WideName(It->first.begin(), It->first.end());
		Node->SetUserPropInt(WideName.c_str(), MeshList.Data()[0]);
	    }
	}
    }

    for (auto It = FloatMetadata.begin(); It != FloatMetadata.end(); It++)
    {
	HEMAX_MeshList<float> &MeshList = It->second;
	if (MeshList.DataOwner() == HAPI_ATTROWNER_DETAIL)
	{
	    if (MeshList.DataSize() == 1 && MeshList.DataTupleSize() == 1)
	    {
		std::wstring WideName(It->first.begin(), It->first.end());
		Node->SetUserPropFloat(WideName.c_str(), MeshList.Data()[0]);
	    }
	}
    }

    for (auto It = StringMetadata.begin(); It != StringMetadata.end(); It++)
    {
	HEMAX_MeshList<std::string> &MeshList = It->second;
	if (MeshList.DataOwner() == HAPI_ATTROWNER_DETAIL)
	{
	    if (MeshList.DataSize() == 1 && MeshList.DataTupleSize() == 1)
	    {
		std::wstring WideName(It->first.begin(), It->first.end());
		std::wstring WideVal(MeshList.Data()[0].begin(), MeshList.Data()[0].end());
		Node->SetUserPropString(WideName.c_str(), WideVal.c_str());
	    }
	}
    }
}

void
HEMAX_Mesh::MarshallDataInto3dsMaxMNMesh(MNMesh& MaxMesh)
{
    MaxMesh.ClearAndFree();

    float ScaleConversion = HEMAX_Utilities::GetHoudiniToMaxScale();
    float Point[3];

    MaxMesh.setNumVerts(GetPointCount());
    MaxMesh.setNumFaces(GetFaceCount());

    for (int p = 0; p < PointCount; p++)
    {
	GetPointAtIndex(p, Point);
        MaxMesh.v[p].p = Point3(
                            Point[0] * ScaleConversion,
                            -Point[2] * ScaleConversion,
                            Point[1] * ScaleConversion);
    }

    int CurrentIndex = 0;

    BitArray FaceSelections;
    if (HasFaceSelections())
    {
        FaceSelections.SetSize(GetFaceCount());
    }

    BitArray VertexSelections;
    if (HasVertexSelections())
    {
        VertexSelections.SetSize(GetPointCount());
    }
    
    for (int f = 0; f < GetFaceCount(); f++)
    {
	MaxMesh.F(f)->SetDeg(GetFaceVertexCount(f));
	for (int v = GetFaceVertexCount(f) - 1; v >= 0; v--)
	{
	    MaxMesh.F(f)->vtx[v] = GetVertex(CurrentIndex++);

	    if (DoesSmoothingGroupAttrExist())
	    {
		MaxMesh.F(f)->smGroup = GetSmoothingGroupArray()[f];
	    }
	    else
	    {
		MaxMesh.F(f)->smGroup = 1;
	    }

	    if (DoesMaterialIDAttrExist())
	    {
		MaxMesh.F(f)->material = GetMaterialIDArray()[f];
	    }

            if (HasFaceSelections())
            {
                FaceSelections.Set(f, GetFaceSelectionsArray()[f]);
            }
	}

	if (GetFaceVertexCount(f) > 4)
	{
	    MaxMesh.RetriangulateFace(f);
	}
    }

    MaxMesh.buildNormals();
    float NormalVals[3];
    MNNormalSpec* SpecNormals = nullptr;

    if (DoNormalsExist() && GetNormalType() == HEMAX_VERTEX_NORMAL)
    {
	MaxMesh.SpecifyNormals();

	SpecNormals = MaxMesh.GetSpecifiedNormals();
	SpecNormals->BuildNormals();
	SpecNormals->ComputeNormals();

	if (SpecNormals)
	{
	    SpecNormals->SetNumFaces(GetFaceCount());

	    CurrentIndex = 0;

	    for (int f = 0; f < GetFaceCount(); f++)
	    {
		MNNormalFace& NormFace = SpecNormals->Face(f);
		for (int v = GetFaceVertexCount(f) - 1; v >= 0; v--)
		{
		    GetVertexNormalAtIndex(CurrentIndex, NormalVals);
                    Point3 NormalVec = Point3(
                                            NormalVals[0],
                                            -NormalVals[2],
                                            NormalVals[1]);
		    SpecNormals->SetNormal(f, v, NormalVec);
		    CurrentIndex++;
		}
	    }

	    SpecNormals->SetAllExplicit();
	    SpecNormals->NShrink();
	}
    }

    // MNMesh requires we outright declare the number of maps
    int TotalMapCount = 0;

    if (DoUVsExist())
    {
	// Cd = 0, uv = 1, hence the += 2.
	TotalMapCount += 2;
    }

    TotalMapCount += SecondaryUVCount;

    MaxMesh.SetMapNum(TotalMapCount);

    MNMap* UVMap;
    MNMap* CdMap;
    MNMap* AlphaMap;
    MNMap* IlluminationMap;

    if (DoUVsExist())
    {
	UVMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_UVW);

	if (!UVMap)
	{
	    MaxMesh.InitMap(HEMAX_MAPPING_CHANNEL_UVW);
	    UVMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_UVW);
	}

	if (UVMap)
	{
	    UVMap->ClearAllFlags();

	    if (GetUVType() == HEMAX_POINT_UV)
	    {
		UVMap->setNumFaces(GetFaceCount());
		UVMap->setNumVerts(GetPointCount());
	    }
	    else if (GetUVType() == HEMAX_VERTEX_UV)
	    {
		UVList.MergeEqualTuples();

		UVMap->setNumFaces(GetFaceCount());
		UVMap->setNumVerts(GetVertexCount());

		std::vector<float> UVSet = UVList.MergedValues();

		for (unsigned int z = 0; z < UVList.MergedDataSize(); z++)
		{
		    UVMap->v[z].x = UVSet[z * UVList.DataTupleSize()];
		    UVMap->v[z].y = UVSet[z * UVList.DataTupleSize()+1];
		    UVMap->v[z].z = UVSet[z * UVList.DataTupleSize()+2];
		}
	    }
	}
    }
    if (DoesCdAttrExist())
    {
	CdMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_COLOR);

	if (!CdMap)
	{
	    MaxMesh.InitMap(HEMAX_MAPPING_CHANNEL_COLOR);
	    CdMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_COLOR);
	}

	if (CdMap)
	{
	    CdMap->ClearAllFlags();

	    if (GetCdAttrOwner() == HAPI_ATTROWNER_POINT)
	    {
		CdMap->setNumFaces(GetFaceCount());
		CdMap->setNumVerts(GetPointCount());
	    }
	    else if (GetCdAttrOwner() == HAPI_ATTROWNER_VERTEX)
	    {
		CdMap->setNumFaces(GetFaceCount());
		CdMap->setNumVerts(GetVertexCount());
	    }
	}
    }

    if (DoesAlphaAttrExist())
    {
	AlphaMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_ALPHA);

	if (!AlphaMap)
	{
	    MaxMesh.InitMap(HEMAX_MAPPING_CHANNEL_ALPHA);
	    AlphaMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_ALPHA);
	}

	if (AlphaMap)
	{
	    AlphaMap->ClearAllFlags();

	    if (GetAlphaAttrOwner() == HAPI_ATTROWNER_POINT)
	    {
		AlphaMap->setNumFaces(GetFaceCount());
		AlphaMap->setNumVerts(GetPointCount());
	    }
	    else if (GetAlphaAttrOwner() == HAPI_ATTROWNER_VERTEX)
	    {
		AlphaMap->setNumFaces(GetFaceCount());
		AlphaMap->setNumVerts(GetVertexCount());
	    }
	}
    }

    if (DoesIlluminationAttrExist())
    {
	IlluminationMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_ILLUMINATION);

	if (!IlluminationMap)
	{
	    MaxMesh.InitMap(HEMAX_MAPPING_CHANNEL_ILLUMINATION);
	    IlluminationMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_ILLUMINATION);
	}

	if (IlluminationMap)
	{
	    IlluminationMap->ClearAllFlags();

	    if (GetIlluminationOwner() == HAPI_ATTROWNER_POINT)
	    {
		IlluminationMap->setNumFaces(GetFaceCount());
		IlluminationMap->setNumVerts(GetPointCount());
	    }
	    else if (GetIlluminationOwner() == HAPI_ATTROWNER_VERTEX)
	    {
		IlluminationMap->setNumFaces(GetFaceCount());
		IlluminationMap->setNumVerts(GetVertexCount());
	    }
	}
    }

    float UVVals[3];
    UVVals[0] = 0;
    UVVals[1] = 0;
    UVVals[2] = 0;

    float CdVals[3];
    float AlphaVal;
    float IlluminationVals[3];

    for (int p = 0; p < GetPointCount(); p++)
    {
	if (UVMap && DoUVsExist() && GetUVType() == HEMAX_POINT_UV)
	{
	    GetPointUVAtIndex(p, UVVals);
	    UVMap->v[p].x = UVVals[0];
	    UVMap->v[p].y = UVVals[1];
	    UVMap->v[p].z = UVVals[2];
	}
	if (CdMap &&
            DoesCdAttrExist() &&
            GetCdAttrOwner() == HAPI_ATTROWNER_POINT)
	{
	    GetPointCdAtIndex(p, CdVals);
	    CdMap->v[p].x = CdVals[0];
	    CdMap->v[p].y = CdVals[1];
	    CdMap->v[p].z = CdVals[2];
	}
	if (AlphaMap &&
            DoesAlphaAttrExist() &&
            GetAlphaAttrOwner() == HAPI_ATTROWNER_POINT)
	{
	    AlphaVal = GetAlphaAtIndex(p);
	    AlphaMap->v[p].x = AlphaVal;
	    AlphaMap->v[p].y = AlphaVal;
	    AlphaMap->v[p].z = AlphaVal;
	}
	if (IlluminationMap &&
            DoesIlluminationAttrExist() &&
            GetIlluminationOwner() == HAPI_ATTROWNER_POINT)
	{
	    GetIlluminationAtIndex(p, IlluminationVals);
	    IlluminationMap->v[p].x = IlluminationVals[0];
	    IlluminationMap->v[p].y = IlluminationVals[1];
	    IlluminationMap->v[p].z = IlluminationVals[2];
	}

        if (HasVertexSelections())
        {
            VertexSelections.Set(p, GetVertexSelectionsArray()[p]);
        }
    }

    int VertexIndex = 0;
    
    for (int f = 0; f < GetFaceCount(); f++)
    {
	if (UVMap && DoUVsExist())
	{
	    UVMap->F(f)->SetSize(GetFaceVertexCount(f));
	}
	if (CdMap && DoesCdAttrExist())
	{
	    CdMap->F(f)->SetSize(GetFaceVertexCount(f));
	}
	if (AlphaMap && DoesAlphaAttrExist())
	{
	    AlphaMap->F(f)->SetSize(GetFaceVertexCount(f));
	}
	if (IlluminationMap && DoesIlluminationAttrExist())
	{
	    IlluminationMap->F(f)->SetSize(GetFaceVertexCount(f));
	}

	for (int v = GetFaceVertexCount(f) - 1; v >= 0; v--)
	{
	    if (UVMap && DoUVsExist())
	    {
		if (GetUVType() == HEMAX_POINT_UV)
		{
		    UVMap->F(f)->tv[v] = MaxMesh.F(f)->vtx[v];
		}
		else if (GetUVType() == HEMAX_VERTEX_UV)
		{
		    UVMap->F(f)->tv[v] = UVList.GetMergedIndex(VertexIndex);
		}
	    }
	    if (CdMap && DoesCdAttrExist())
	    {
		if (GetCdAttrOwner() == HAPI_ATTROWNER_POINT)
		{
		    CdMap->F(f)->tv[v] = MaxMesh.F(f)->vtx[v];
		}
		else if (GetCdAttrOwner() == HAPI_ATTROWNER_VERTEX)
		{
		    GetVertexCdAtIndex(VertexIndex, CdVals);
		    CdMap->v[VertexIndex].x = CdVals[0];
		    CdMap->v[VertexIndex].y = CdVals[1];
		    CdMap->v[VertexIndex].z = CdVals[2];

		    CdMap->F(f)->tv[v] = VertexIndex;
		}
	    }
	    if (AlphaMap && DoesAlphaAttrExist())
	    {
		if (GetAlphaAttrOwner() == HAPI_ATTROWNER_POINT)
		{
		    AlphaMap->F(f)->tv[v] = MaxMesh.F(f)->vtx[v];
		}
		else if (GetAlphaAttrOwner() == HAPI_ATTROWNER_VERTEX)
		{
		    AlphaVal = GetAlphaAtIndex(VertexIndex);
		    AlphaMap->v[VertexIndex].x = AlphaVal;
		    AlphaMap->v[VertexIndex].y = AlphaVal;
		    AlphaMap->v[VertexIndex].z = AlphaVal;

		    AlphaMap->F(f)->tv[v] = VertexIndex;
		}
	    }
	    if (IlluminationMap && DoesIlluminationAttrExist())
	    {
		if (GetIlluminationOwner() == HAPI_ATTROWNER_POINT)
		{
		    IlluminationMap->F(f)->tv[v] = MaxMesh.F(f)->vtx[v];
		}
		else if (GetIlluminationOwner() == HAPI_ATTROWNER_VERTEX)
		{
		    GetIlluminationAtIndex(VertexIndex, IlluminationVals);
		    IlluminationMap->v[VertexIndex].x = IlluminationVals[0];
		    IlluminationMap->v[VertexIndex].y = IlluminationVals[1];
		    IlluminationMap->v[VertexIndex].z = IlluminationVals[2];

		    IlluminationMap->F(f)->tv[v] = VertexIndex;
		}
	    }
	    VertexIndex++;
	}
    }

    for (int Layer = 2; Layer < MAX_MESHMAPS; Layer++)
    {
	if (DoesSecondaryUVLayerExist(HAPI_ATTROWNER_POINT, Layer))
	{
	    MNMap* UVMap = MaxMesh.M(Layer);

	    if (!UVMap)
	    {
		MaxMesh.InitMap(Layer);
		UVMap = MaxMesh.M(Layer);
	    }

	    if (UVMap)
	    {
                HEMAX_MeshList<float>* UVValues = GetSecondaryUVLayer(
                    HAPI_ATTROWNER_POINT, Layer);

                if (!UVValues)
                    continue;

                UVMap->ClearAllFlags();
		UVMap->setNumFaces(GetFaceCount());
		UVMap->setNumVerts(GetPointCount());

		for (int p = 0; p < GetPointCount(); p++)
		{
                    std::vector<float> UV = UVValues->Value(p);
                    UVMap->v[p].x = UV[0];
                    UVMap->v[p].y = UV[1];
                    UVMap->v[p].z = UV[2];
		}

                int VIndex = 0;

                for (int f = 0; f < GetFaceCount(); f++)
                {
                    UVMap->F(f)->SetSize(GetFaceVertexCount(f));

                    for (int v = GetFaceVertexCount(f) - 1; v >= 0; v--)
                    {
                        UVMap->F(f)->tv[v] = MaxMesh.F(f)->vtx[v];
                    }
                }
	    }
	}
	else if (DoesSecondaryUVLayerExist(HAPI_ATTROWNER_VERTEX, Layer))
	{
	    MNMap* UVMap = MaxMesh.M(Layer);

	    if (!UVMap)
	    {
		MaxMesh.InitMap(Layer);
		UVMap = MaxMesh.M(Layer);
	    }

	    if (UVMap)
	    {
                HEMAX_MeshList<float>* UVValues = GetSecondaryUVLayer(
                        HAPI_ATTROWNER_VERTEX, Layer);

                if (!UVValues)
                    continue;

                UVValues->MergeEqualTuples();

		UVMap->ClearAllFlags();
                UVMap->setNumFaces(GetFaceCount());
                UVMap->setNumVerts(GetVertexCount());

                std::vector<float> UVSet = UVValues->MergedValues();

                for (unsigned int z = 0; z < UVValues->MergedDataSize(); z++)
                {
                    UVMap->v[z].x = UVSet[z * UVValues->DataTupleSize()];
                    UVMap->v[z].y = UVSet[z * UVValues->DataTupleSize()+1];
                    UVMap->v[z].z = UVSet[z * UVValues->DataTupleSize()+2];
                }

                int VIndex = 0;

                for (int f = 0; f < GetFaceCount(); f++)
                {
                    UVMap->F(f)->SetSize(GetFaceVertexCount(f));

                    for (int v = GetFaceVertexCount(f) - 1; v >= 0; v--)
                    {
                        UVMap->F(f)->tv[v] = UVValues->GetMergedIndex(VIndex);
                        ++VIndex;
                    }
                }
	    }
	}
    }

    MaxMesh.InvalidateGeomCache();
    MaxMesh.InvalidateTopoCache();

    MaxMesh.FillInMesh();

    if (HasEdgeSelections() && MaxMesh.ENum() > 0)
    {
        BitArray EdgeSelections;
        EdgeSelections.SetSize(MaxMesh.ENum());
        EdgeSelections.ClearAll();

        for (unsigned int e = 0; e < EdgeSelectionsList.DataSize() - 1; e += 2)
        {
            int EdgeToSelect = MaxMesh.FindEdgeFromVertToVert(
                EdgeSelectionsList.Data()[e], EdgeSelectionsList.Data()[e+1]);
            if (EdgeToSelect > -1)
            {
                EdgeSelections.Set(EdgeToSelect, 1);
            }
        } 

        MaxMesh.EdgeSelect(EdgeSelections);
    }

    // Set selections
    MaxMesh.FaceSelect(FaceSelections);
    MaxMesh.VertexSelect(VertexSelections);
}
