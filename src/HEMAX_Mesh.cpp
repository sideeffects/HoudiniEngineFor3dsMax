#include "HEMAX_Mesh.h"

#include "HEMAX_Utilities.h"
#include "MeshNormalSpec.h"
#include "MNNormalSpec.h"

#include <unordered_map>
#include <deque>

HEMAX_Mesh::HEMAX_Mesh()
    : FaceCount( 0 )
    , VertexCount( 0 )
    , PointCount( 0 )
    , FaceVertexCounts( nullptr )
    , VertexList( nullptr )
    , PointList( nullptr )
    , PointNormals( nullptr )
    , VertexNormals( nullptr )
    , NormalType(HEMAX_NO_NORMAL)
    , NormalsExist( false )
    , FaceMaterialIds( nullptr )
    , UVPointArray( nullptr )
    , UVVertArray( nullptr )
    , UVTupleSize( 0 )
    , HasUVs( false )
    , UVType(HEMAX_NO_UV)
    , ColorAttrExists(false)
    , ColorAttrOwner(HEMAX_ATTRIBUTEOWNER_INVALID)
    , PointCdArray(nullptr)
    , VertexCdArray(nullptr)
    , AlphaArray(nullptr)
    , IlluminationArray(nullptr)
    , AlphaAttrExists(false)
    , IlluminationAttrExists(false)
    , SmoothingGroupArray(nullptr)
    , SmoothingGroupsExist(false)
    , MaterialIDArray(nullptr)
    , MaterialIDsExist(false)
    , SecondaryUVCount(0)
    , AreMaterialIdsSame(false)
{
}

HEMAX_Mesh::HEMAX_Mesh(int FCount, int VCount, int PCount, int PTupleSize)
    : FaceCount(FCount)
    , VertexCount(VCount)
    , PointCount(PCount)
    , PointTupleSize(PTupleSize)
    , PointNormals(nullptr)
    , VertexNormals(nullptr)
    , NormalType(HEMAX_NO_NORMAL)
    , NormalsExist( false )
    , FaceMaterialIds( nullptr )
    , UVPointArray( nullptr )
    , UVVertArray( nullptr )
    , UVTupleSize ( 0 )
    , HasUVs( false )
    , UVType(HEMAX_NO_UV)
    , ColorAttrExists(false)
    , ColorAttrOwner(HEMAX_ATTRIBUTEOWNER_INVALID)
    , PointCdArray(nullptr)
    , VertexCdArray(nullptr)
    , AlphaArray(nullptr)
    , IlluminationArray(nullptr)
    , AlphaAttrExists(false)
    , IlluminationAttrExists(false)
    , SmoothingGroupArray(nullptr)
    , SmoothingGroupsExist(false)
    , MaterialIDArray(nullptr)
    , MaterialIDsExist(false)
    , SecondaryUVCount(0)
    , AreMaterialIdsSame(false)
{
    FaceVertexCounts = new int[ FaceCount ];
    VertexList = new int[ VertexCount ];
    PointList = new float[ PointCount * PointTupleSize ];
}

void
HEMAX_Mesh::AllocatePointNormalArray()
{
    PointNormals = new float[PointCount * PointTupleSize];
    NormalType = HEMAX_POINT_NORMAL;
}

void
HEMAX_Mesh::AllocateVertexNormalArray()
{
    VertexNormals = new float[VertexCount* PointTupleSize];
    NormalType = HEMAX_VERTEX_NORMAL;
}

void
HEMAX_Mesh::AllocateMaterialIdsArray()
{
    FaceMaterialIds = new HEMAX_NodeId[FaceCount];
}

void
HEMAX_Mesh::AllocatePointUVArray(int TupleSize)
{
    UVPointArray = new float[PointCount * TupleSize];
    UVTupleSize = TupleSize;
    HasUVs = true;
    UVType = HEMAX_POINT_UV;
}

void
HEMAX_Mesh::AllocateVertexUVArray(int TupleSize)
{
    UVVertArray = new float[VertexCount * TupleSize];
    UVTupleSize = TupleSize;
    HasUVs = true;
    UVType = HEMAX_VERTEX_UV;
}

void
HEMAX_Mesh::AllocatePointCdArray()
{
    PointCdArray = new float[PointCount * 3];
    ColorAttrExists = true;
    ColorAttrOwner = HEMAX_ATTRIBUTEOWNER_POINT;
}

void
HEMAX_Mesh::AllocateVertexCdArray()
{
    VertexCdArray = new float[VertexCount * 3];
    ColorAttrExists = true;
    ColorAttrOwner = HEMAX_ATTRIBUTEOWNER_VERTEX;
}

void
HEMAX_Mesh::AllocateAlphaArray(HEMAX_AttributeOwner Owner)
{
    if (Owner == HEMAX_ATTRIBUTEOWNER_POINT)
    {
        AlphaArray = new float[PointCount];
        AlphaAttrExists = true;
        AlphaAttrOwner = Owner;
    }
    else if (Owner == HEMAX_ATTRIBUTEOWNER_VERTEX)
    {
        AlphaArray = new float[VertexCount];
        AlphaAttrExists = true;
        AlphaAttrOwner = Owner;
    }
}

void
HEMAX_Mesh::AllocateIlluminationArray(HEMAX_AttributeOwner Owner)
{
    if (Owner == HEMAX_ATTRIBUTEOWNER_POINT)
    {
        IlluminationArray = new float[PointCount * 3];
        IlluminationAttrExists = true;
        IlluminationAttrOwner = Owner;
    }
    else if (Owner == HEMAX_ATTRIBUTEOWNER_VERTEX)
    {
        IlluminationArray = new float[VertexCount * 3];
        IlluminationAttrExists = true;
        IlluminationAttrOwner = Owner;
    }
}

void
HEMAX_Mesh::AllocateSmoothingGroupsArray()
{
    SmoothingGroupArray = new int[FaceCount];
    SmoothingGroupsExist = true;
}

void
HEMAX_Mesh::AllocateMaterialIDArray()
{
    MaterialIDArray = new int[FaceCount];
    MaterialIDsExist = true;
}

HEMAX_Mesh::~HEMAX_Mesh()
{
    if (FaceVertexCounts)
    {
        delete[] FaceVertexCounts;
        FaceVertexCounts = nullptr;
    }
    
    if (VertexList)
    {
        delete[] VertexList;
        VertexList = nullptr;
    }

    if (PointList)
    {
        delete[] PointList;
        PointList = nullptr;
    }
    
    if (PointNormals)
    {
        delete[] PointNormals;
        PointNormals = nullptr;
    }

    if (VertexNormals)
    {
        delete[] VertexNormals;
        VertexNormals = nullptr;
    }

    if (FaceMaterialIds)
    {
        delete[] FaceMaterialIds;
        FaceMaterialIds = nullptr;
    }

    if (UVPointArray)
    {
        delete[] UVPointArray;
        UVPointArray = nullptr;
    }

    if (UVVertArray)
    {
        delete[] UVVertArray;
        UVVertArray = nullptr;
    }

    if (PointCdArray)
    {
        delete[] PointCdArray;
        PointCdArray = nullptr;
    }

    if (VertexCdArray)
    {
        delete[] VertexCdArray;
        VertexCdArray = nullptr;
    }

    if (AlphaArray)
    {
        delete[] AlphaArray;
        AlphaArray = nullptr;
    }

    if (IlluminationArray)
    {
        delete[] IlluminationArray;
        IlluminationArray = nullptr;
    }

    if (SmoothingGroupArray)
    {
        delete[] SmoothingGroupArray;
        SmoothingGroupArray = nullptr;
    }

    if (MaterialIDArray)
    {
        delete[] MaterialIDArray;
        MaterialIDArray = nullptr;
    }

    SecondaryUVCount = 0;
    SecondaryPointUVs.clear();
    SecondaryVertexUVs.clear();
}

void
HEMAX_Mesh::SetFaceCount( int Count )
{
    FaceCount = Count;

    if ( FaceVertexCounts )
    {
        delete [] FaceVertexCounts;
    }
    if (FaceMaterialIds)
    {
        delete[] FaceMaterialIds;
    }

    if ( Count > 0 )
    {
        FaceVertexCounts = new int[ FaceCount ];
    }
}

void
HEMAX_Mesh::SetVertexCount( int Count )
{
    VertexCount = Count;

    if ( VertexList )
    {
        delete [] VertexList;
    }

    if ( Count > 0 )
    {
        VertexList = new int[ VertexCount ];
    }
}

void
HEMAX_Mesh::SetPointCount( int Count, int TupleSize )
{
    PointCount = Count;
    PointTupleSize = TupleSize;

    if ( PointList )
    {
        delete [] PointList;
    }

    if ( Count > 0 )
    {
        PointList = new float[ PointCount * TupleSize ];
    }
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
HEMAX_Mesh::GetPointTupleSize()
{
    return PointTupleSize;
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
    std::unordered_map<HEMAX_NodeId, bool> SeenMaterials;
    int NumMaterials = 0;

    for (int f = 0; f < FaceCount; f++)
    {
        HEMAX_NodeId MatId = FaceMaterialIds[f];
        auto Search = SeenMaterials.find(MatId);
        if (Search == SeenMaterials.end())
        {
            NumMaterials++;
            SeenMaterials.insert({ MatId, true });
        }
    }

    return NumMaterials;
}

void
HEMAX_Mesh::SetFaceVertexCounts( int* FVCounts )
{
    for ( int i = 0; i < FaceCount; ++i )
    {
        FaceVertexCounts[ i ] = FVCounts[ i ];
    }
}

void
HEMAX_Mesh::SetVertexList( int* VList )
{
    for ( int i = 0; i < VertexCount; ++i )
    {
        VertexList[ i ] = VList[ i ];
    }
}

void
HEMAX_Mesh::SetPointList( float* PList )
{
    for ( int i = 0; i < PointCount * PointTupleSize; ++i )
    {
        PointList[ i ] = PList[ i ];
    }
}

void
HEMAX_Mesh::SetPointNormalsList(float* PNList)
{
    for (int i = 0; i < PointCount * PointTupleSize; i++)
    {
        PointNormals[i] = PNList[i];
    }
}

void
HEMAX_Mesh::SetVertexNormalsList(float* VNList)
{
    for (int i = 0; i < VertexCount * PointTupleSize; i++)
    {
        VertexNormals[i] = VNList[i];
    }
}

int*
HEMAX_Mesh::GetFaceVertexCountsArray()
{
    return FaceVertexCounts;
}

int*
HEMAX_Mesh::GetVertexListArray()
{
    return VertexList;
}

float*
HEMAX_Mesh::GetPointListArray()
{
    return PointList;
}

float*
HEMAX_Mesh::GetPointNormalsListArray()
{
    return PointNormals;
}

float*
HEMAX_Mesh::GetVertexNormalsListArray()
{
    return VertexNormals;
}

HEMAX_NodeId*
HEMAX_Mesh::GetMaterialIdsArray()
{
    return FaceMaterialIds;
}

float*
HEMAX_Mesh::GetPointUVArray()
{
    return UVPointArray;
}

float*
HEMAX_Mesh::GetVertexUVArray()
{
    return UVVertArray;
}

float*
HEMAX_Mesh::GetPointCdArray()
{
    return PointCdArray;
}

float*
HEMAX_Mesh::GetVertexCdArray()
{
    return VertexCdArray;
}

float*
HEMAX_Mesh::GetAlphaArray()
{
    return AlphaArray;
}

float*
HEMAX_Mesh::GetIlluminationArray()
{
    return IlluminationArray;
}

int*
HEMAX_Mesh::GetSmoothingGroupArray()
{
    return SmoothingGroupArray;
}

int*
HEMAX_Mesh::GetMaterialIDArray()
{
    return MaterialIDArray;
}

int
HEMAX_Mesh::GetFaceVertexCount(int Index)
{
    return FaceVertexCounts[Index];
}

void
HEMAX_Mesh::GetPointAtIndex( int Index, float* Point )
{
    for ( int i = 0; i < PointTupleSize; ++i )
    {
        Point[ i ] = PointList [ ( Index * PointTupleSize ) + i ];
    }
}

void
HEMAX_Mesh::GetPointNormalAtIndex(int Index, float* Normal)
{
    for (int i = 0; i < PointTupleSize; ++i)
    {
        Normal[i] = PointNormals[(Index * PointTupleSize) + i];
    }
}

void 
HEMAX_Mesh::GetVertexNormalAtIndex(int Index, float* Normal)
{
    for (int i = 0; i < PointTupleSize; ++i)
    {
        Normal[i] = VertexNormals[(Index * PointTupleSize) + i];
    }
}

int
HEMAX_Mesh::GetVertex( int Index )
{
    return VertexList[ Index ];
}

void
HEMAX_Mesh::GetPointUVAtIndex(int Index, float* UVVals)
{
    for (int i = 0; i < UVTupleSize; ++i)
    {
        UVVals[i] = UVPointArray[(Index * PointTupleSize) + i];
    }
}

void
HEMAX_Mesh::GetVertexUVAtIndex(int Index, float* UVVals)
{
    for (int i = 0; i < UVTupleSize; ++i)
    {
        UVVals[i] = UVVertArray[(Index * PointTupleSize) + i];
    }
}

void
HEMAX_Mesh::GetPointCdAtIndex(int Index, float* CdVals)
{
    for (int i = 0; i < 3; i++)
    {
        CdVals[i] = PointCdArray[(Index * 3) + i];
    }
}

void
HEMAX_Mesh::GetVertexCdAtIndex(int Index, float* CdVals)
{
    for (int i = 0; i < 3; i++)
    {
        CdVals[i] = VertexCdArray[(Index * 3) + i];
    }
}

float
HEMAX_Mesh::GetAlphaAtIndex(int Index)
{
    return (AlphaArray[Index]);
}

void
HEMAX_Mesh::GetIlluminationAtIndex(int Index, float* IlluminationVals)
{
    for (int i = 0; i < 3; i++)
    {
        IlluminationVals[i] = IlluminationArray[(Index * 3) + i];
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

HEMAX_AttributeOwner
HEMAX_Mesh::GetCdAttrOwner()
{
    return ColorAttrOwner;
}

bool
HEMAX_Mesh::DoesAlphaAttrExist()
{
    return AlphaAttrExists;
}

HEMAX_AttributeOwner
HEMAX_Mesh::GetAlphaAttrOwner()
{
    return AlphaAttrOwner;
}

bool
HEMAX_Mesh::DoesIlluminationAttrExist()
{
    return IlluminationAttrExists;
}

HEMAX_AttributeOwner
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

void
HEMAX_Mesh::MarshallDataInto3dsMaxMesh(Mesh& MaxMesh)
{
    MaxMesh.FreeAll();

    MaxMesh.setNumVerts(GetPointCount());
    MaxMesh.setNumFaces(GetPostTriangulationFaceCount());

    if (DoUVsExist())
    {
        if (GetUVType() == HEMAX_POINT_UV)
        {
            MaxMesh.setNumTVerts(GetPointUVCount());
        }
        else if (GetUVType() == HEMAX_VERTEX_UV)
        {
            MaxMesh.setNumTVerts(GetVertexUVCount());
        }
        MaxMesh.setNumTVFaces(GetPostTriangulationFaceCount());
    }

    float* Point = new float[GetPointTupleSize()];
    float* UVVals = new float[GetUVTupleSize()];

    float ScaleConversion = HEMAX_Utilities::GetHoudiniToMaxScale();

    std::unordered_map<int, std::deque<int>> UVVertMap;

    for (int p = 0; p < GetPointCount(); ++p)
    {
        GetPointAtIndex(p, Point);
        MaxMesh.setVert(p, Point3(Point[0] * ScaleConversion,
            -Point[2] * ScaleConversion,
            Point[1] * ScaleConversion));

        if (DoUVsExist())
        {
            if (GetUVType() == HEMAX_POINT_UV)
            {
                GetPointUVAtIndex(p, UVVals);
                MaxMesh.setTVert(p, UVVals[0], UVVals[1], UVVals[2]);
            }
        }
    }

    // Quad and N-order meshes
    int VertIndex = 0;
    int triFace = 0;

    for (int f = 0; f < GetFaceCount(); ++f)
    {
        for (int n = 0; n < GetFaceVertexCount(f) - 2; n++)
        {
            MaxMesh.faces[triFace].setVerts(GetVertex(VertIndex + n + 2),
                                            GetVertex(VertIndex + n + 1),
                                            GetVertex(VertIndex));

            if (GetFaceVertexCount(f) > 3)
            {
                if (n == 0)
                {
                    MaxMesh.faces[triFace].setEdgeVisFlags(1, 1, 0);
                }
                else if (n == GetFaceVertexCount(f) - 3)
                {
                    MaxMesh.faces[triFace].setEdgeVisFlags(1, 0, 1);
                }
                else
                {
                    MaxMesh.faces[triFace].setEdgeVisFlags(1, 0, 0);
                }
            }
            else
            {
                MaxMesh.faces[triFace].setEdgeVisFlags(1, 1, 1);
            }
            
            MaxMesh.faces[triFace].setSmGroup(1);

            if (DoUVsExist())
            {
                if (GetUVType() == HEMAX_POINT_UV)
                {
                    MaxMesh.tvFace[triFace].setTVerts(GetVertex(VertIndex + n + 2),
                        GetVertex(VertIndex + n + 1),
                        GetVertex(VertIndex));
                }
                else if (GetUVType() == HEMAX_VERTEX_UV)
                {
                    GetVertexUVAtIndex(VertIndex + n + 2, UVVals);
                    MaxMesh.setTVert(VertIndex + n + 2, UVVals[0], UVVals[1], UVVals[2]);
                    GetVertexUVAtIndex(VertIndex + n + 1, UVVals);
                    MaxMesh.setTVert(VertIndex + n + 1, UVVals[0], UVVals[1], UVVals[2]);
                    GetVertexUVAtIndex(VertIndex, UVVals);
                    MaxMesh.setTVert(VertIndex, UVVals[0], UVVals[1], UVVals[2]);

                    MaxMesh.tvFace[triFace].setTVerts(VertIndex + n + 2, VertIndex + n + 1, VertIndex);
                }
            }

            triFace++;
        }
        VertIndex += GetFaceVertexCount(f);
    }

    MaxMesh.buildNormals();
    float NormalVals[3];
    MeshNormalSpec* SpecNormals = nullptr;

    if (DoNormalsExist() && GetNormalType() == HEMAX_VERTEX_NORMAL)
    {
        MaxMesh.SpecifyNormals();

        SpecNormals = MaxMesh.GetSpecifiedNormals();

        int NormalFaceCount = GetPostTriangulationFaceCount();
        SpecNormals->SetNumFaces(NormalFaceCount);

        SpecNormals->SetNumNormals(NormalFaceCount * 3);

        SpecNormals->SetAllExplicit();

        int VertIndex = 0;
        int triFace = 0;

        for (int f = 0; f < GetFaceCount(); ++f)
        {
            for (int n = 0; n < GetFaceVertexCount(f) - 2; n++)
            {
                GetVertexNormalAtIndex(VertIndex, NormalVals);
                SpecNormals->SetNormal(triFace, 2, Point3(NormalVals[0], -NormalVals[2], NormalVals[1]));

                GetVertexNormalAtIndex(VertIndex + n + 1, NormalVals);
                SpecNormals->SetNormal(triFace, 1, Point3(NormalVals[0], -NormalVals[2], NormalVals[1]));

                GetVertexNormalAtIndex(VertIndex + n + 2, NormalVals);
                SpecNormals->SetNormal(triFace, 0, Point3(NormalVals[0], -NormalVals[2], NormalVals[1]));

                triFace++;
            }
            VertIndex += GetFaceVertexCount(f);
        }
    }

    float CdVals[3];
    float Alpha;
    float IllumeVals[3];

    UVVert* CdData = nullptr;
    TVFace* CdFaces = nullptr;

    UVVert* AlphaData = nullptr;
    TVFace* AlphaFaces = nullptr;

    UVVert* IllumeData = nullptr;
    TVFace* IllumeFaces = nullptr;

    if (DoesCdAttrExist())
    {
        MaxMesh.setMapSupport(HEMAX_MAPPING_CHANNEL_COLOR, true);

        if (GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            MaxMesh.setNumMapVerts(HEMAX_MAPPING_CHANNEL_COLOR, GetPointCount());
            MaxMesh.setNumMapFaces(HEMAX_MAPPING_CHANNEL_COLOR, GetPostTriangulationFaceCount());
        }
        else if (GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
        {
            MaxMesh.setNumMapVerts(HEMAX_MAPPING_CHANNEL_COLOR, GetVertexCount());
            MaxMesh.setNumMapFaces(HEMAX_MAPPING_CHANNEL_COLOR, GetPostTriangulationFaceCount());
        }

        CdData = MaxMesh.mapVerts(HEMAX_MAPPING_CHANNEL_COLOR);
        CdFaces = MaxMesh.mapFaces(HEMAX_MAPPING_CHANNEL_COLOR);
    }

    if (DoesAlphaAttrExist())
    {
        MaxMesh.setMapSupport(HEMAX_MAPPING_CHANNEL_ALPHA, true);

        if (GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            MaxMesh.setNumMapVerts(HEMAX_MAPPING_CHANNEL_ALPHA, GetPointCount());
            MaxMesh.setNumMapFaces(HEMAX_MAPPING_CHANNEL_ALPHA, GetPostTriangulationFaceCount());
        }
        else if (GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
        {
            MaxMesh.setNumMapVerts(HEMAX_MAPPING_CHANNEL_ALPHA, GetVertexCount());
            MaxMesh.setNumMapFaces(HEMAX_MAPPING_CHANNEL_ALPHA, GetPostTriangulationFaceCount());
        }

        AlphaData = MaxMesh.mapVerts(HEMAX_MAPPING_CHANNEL_ALPHA);
        AlphaFaces = MaxMesh.mapFaces(HEMAX_MAPPING_CHANNEL_ALPHA);
    }

    if (DoesIlluminationAttrExist())
    {
        MaxMesh.setMapSupport(HEMAX_MAPPING_CHANNEL_ILLUMINATION, true);

        if (GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            MaxMesh.setNumMapVerts(HEMAX_MAPPING_CHANNEL_ILLUMINATION, GetPointCount());
            MaxMesh.setNumMapFaces(HEMAX_MAPPING_CHANNEL_ILLUMINATION, GetPostTriangulationFaceCount());
        }
        else if (GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
        {
            MaxMesh.setNumMapVerts(HEMAX_MAPPING_CHANNEL_ILLUMINATION, GetVertexCount());
            MaxMesh.setNumMapFaces(HEMAX_MAPPING_CHANNEL_ILLUMINATION, GetPostTriangulationFaceCount());
        }

        IllumeData = MaxMesh.mapVerts(HEMAX_MAPPING_CHANNEL_ILLUMINATION);
        IllumeFaces = MaxMesh.mapFaces(HEMAX_MAPPING_CHANNEL_ILLUMINATION);
    }

    for (int p = 0; p < GetPointCount(); ++p)
    {
        if (DoesCdAttrExist() && GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            GetPointCdAtIndex(p, CdVals);
            CdData[p] = Point3(CdVals[0], CdVals[1], CdVals[2]);
        }
        if (DoesAlphaAttrExist() && GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            Alpha = GetAlphaAtIndex(p);
            AlphaData[p] = Point3(Alpha, Alpha, Alpha);
        }
        if (DoesIlluminationAttrExist() && GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            GetIlluminationAtIndex(p, IllumeVals);
            IllumeData[p] = Point3(IllumeVals[0], IllumeVals[1], IllumeVals[2]);
        }
    }

    VertIndex = 0;
    triFace = 0;

    for (int f = 0; f < GetFaceCount(); f++)
    {
        for (int n = 0; n < GetFaceVertexCount(f) - 2; n++)
        {
            if (DoesCdAttrExist())
            {
                if (GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
                {
                    CdFaces[triFace].t[0] = MaxMesh.faces[triFace].v[0];
                    CdFaces[triFace].t[1] = MaxMesh.faces[triFace].v[1];
                    CdFaces[triFace].t[2] = MaxMesh.faces[triFace].v[2];
                }
                else if (GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
                {
                    GetVertexCdAtIndex(VertIndex + n + 2, CdVals);
                    CdData[VertIndex + n + 2].x = CdVals[0];
                    CdData[VertIndex + n + 2].y = CdVals[1];
                    CdData[VertIndex + n + 2].z = CdVals[2];

                    GetVertexCdAtIndex(VertIndex + n + 1, CdVals);
                    CdData[VertIndex + n + 1].x = CdVals[0];
                    CdData[VertIndex + n + 1].y = CdVals[1];
                    CdData[VertIndex + n + 1].z = CdVals[2];

                    GetVertexCdAtIndex(VertIndex, CdVals);
                    CdData[VertIndex].x = CdVals[0];
                    CdData[VertIndex].y = CdVals[1];
                    CdData[VertIndex].z = CdVals[2];

                    CdFaces[triFace].t[0] = VertIndex + n + 2;
                    CdFaces[triFace].t[1] = VertIndex + n + 1;
                    CdFaces[triFace].t[2] = VertIndex;
                }
            }

            if (DoesAlphaAttrExist())
            {
                if (GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
                {
                    AlphaFaces[triFace].t[0] = MaxMesh.faces[triFace].v[0];
                    AlphaFaces[triFace].t[1] = MaxMesh.faces[triFace].v[1];
                    AlphaFaces[triFace].t[2] = MaxMesh.faces[triFace].v[2];
                }
                else if (GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
                {
                    Alpha = GetAlphaAtIndex(VertIndex + n + 2);
                    AlphaData[VertIndex + n + 2].x = Alpha;
                    AlphaData[VertIndex + n + 2].y = Alpha;
                    AlphaData[VertIndex + n + 2].z = Alpha;

                    Alpha = GetAlphaAtIndex(VertIndex + n + 1);
                    AlphaData[VertIndex + n + 1].x = Alpha;
                    AlphaData[VertIndex + n + 1].y = Alpha;
                    AlphaData[VertIndex + n + 1].z = Alpha;

                    Alpha = GetAlphaAtIndex(VertIndex);
                    AlphaData[VertIndex].x = Alpha;
                    AlphaData[VertIndex].y = Alpha;
                    AlphaData[VertIndex].z = Alpha;

                    AlphaFaces[triFace].t[0] = VertIndex + n + 2;
                    AlphaFaces[triFace].t[1] = VertIndex + n + 1;
                    AlphaFaces[triFace].t[2] = VertIndex;
                }
            }

            if (DoesIlluminationAttrExist())
            {
                if (GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
                {
                    IllumeFaces[triFace].t[0] = MaxMesh.faces[triFace].v[0];
                    IllumeFaces[triFace].t[1] = MaxMesh.faces[triFace].v[1];
                    IllumeFaces[triFace].t[2] = MaxMesh.faces[triFace].v[2];
                }
                else if (GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
                {
                    GetIlluminationAtIndex(VertIndex + n + 2, IllumeVals);
                    IllumeData[VertIndex + n + 2].x = IllumeVals[0];
                    IllumeData[VertIndex + n + 2].y = IllumeVals[1];
                    IllumeData[VertIndex + n + 2].z = IllumeVals[2];

                    GetIlluminationAtIndex(VertIndex + n + 1, IllumeVals);
                    IllumeData[VertIndex + n + 1].x = IllumeVals[0];
                    IllumeData[VertIndex + n + 1].y = IllumeVals[1];
                    IllumeData[VertIndex + n + 1].z = IllumeVals[2];

                    GetIlluminationAtIndex(VertIndex, IllumeVals);
                    IllumeData[VertIndex].x = IllumeVals[0];
                    IllumeData[VertIndex].y = IllumeVals[1];
                    IllumeData[VertIndex].z = IllumeVals[2];

                    IllumeFaces[triFace].t[0] = VertIndex + n + 2;
                    IllumeFaces[triFace].t[1] = VertIndex + n + 1;
                    IllumeFaces[triFace].t[2] = VertIndex;
                }
            }

            triFace++;
        }
        VertIndex += GetFaceVertexCount(f);
    }

    MaxMesh.InvalidateGeomCache();
    MaxMesh.InvalidateTopologyCache();

    delete[] Point;
    delete[] UVVals;
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
        MaxMesh.v[p].p = Point3(Point[0] * ScaleConversion, -Point[2] * ScaleConversion, Point[1] * ScaleConversion);
    }

    int CurrentIndex = 0;

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
                    Point3 NormalVec = Point3(NormalVals[0], -NormalVals[2], NormalVals[1]);
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
        TotalMapCount += 2;
    }
    if (DoesCdAttrExist())
    {
        TotalMapCount++;
    }
    if (DoesAlphaAttrExist())
    {
        TotalMapCount++;
    }
    if (DoesIlluminationAttrExist())
    {
        TotalMapCount++;
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
                UVMap->setNumFaces(GetFaceCount());
                UVMap->setNumVerts(GetVertexCount());
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

            if (GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
            {
                CdMap->setNumFaces(GetFaceCount());
                CdMap->setNumVerts(GetPointCount());
            }
            else if (GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
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

            if (GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
            {
                AlphaMap->setNumFaces(GetFaceCount());
                AlphaMap->setNumVerts(GetPointCount());
            }
            else if (GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
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

            if (GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
            {
                IlluminationMap->setNumFaces(GetFaceCount());
                IlluminationMap->setNumVerts(GetPointCount());
            }
            else if (GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
            {
                IlluminationMap->setNumFaces(GetFaceCount());
                IlluminationMap->setNumVerts(GetVertexCount());
            }
        }
    }

    float UVVals[3];
    float CdVals[3];
    float AlphaVal;
    float IlluminationVals[3];

    for (int p = 0; p < GetPointCount(); p++)
    {
        if (DoUVsExist() && GetUVType() == HEMAX_POINT_UV)
        {
            GetPointUVAtIndex(p, UVVals);
            UVMap->v[p].x = UVVals[0];
            UVMap->v[p].y = UVVals[1];
            UVMap->v[p].z = UVVals[2];
        }
        if (DoesCdAttrExist() && GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            GetPointCdAtIndex(p, CdVals);
            CdMap->v[p].x = CdVals[0];
            CdMap->v[p].y = CdVals[1];
            CdMap->v[p].z = CdVals[2];
        }
        if (DoesAlphaAttrExist() && GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            AlphaVal = GetAlphaAtIndex(p);
            AlphaMap->v[p].x = AlphaVal;
            AlphaMap->v[p].y = AlphaVal;
            AlphaMap->v[p].z = AlphaVal;
        }
        if (DoesIlluminationAttrExist() && GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            GetIlluminationAtIndex(p, IlluminationVals);
            IlluminationMap->v[p].x = IlluminationVals[0];
            IlluminationMap->v[p].y = IlluminationVals[1];
            IlluminationMap->v[p].z = IlluminationVals[2];
        }
    }

    int VertexIndex = 0;

    for (int f = 0; f < GetFaceCount(); f++)
    {
        if (DoUVsExist())
        {
            UVMap->F(f)->SetSize(GetFaceVertexCount(f));
        }
        if (DoesCdAttrExist())
        {
            CdMap->F(f)->SetSize(GetFaceVertexCount(f));
        }
        if (DoesAlphaAttrExist())
        {
            AlphaMap->F(f)->SetSize(GetFaceVertexCount(f));
        }
        if (DoesIlluminationAttrExist())
        {
            IlluminationMap->F(f)->SetSize(GetFaceVertexCount(f));
        }

        for (int v = GetFaceVertexCount(f) - 1; v >= 0; v--)
        {
            if (DoUVsExist())
            {
                if (GetUVType() == HEMAX_POINT_UV)
                {
                    UVMap->F(f)->tv[v] = MaxMesh.F(f)->vtx[v];
                }
                else if (GetUVType() == HEMAX_VERTEX_UV)
                {
                    GetVertexUVAtIndex(VertexIndex, UVVals);
                    UVMap->v[VertexIndex].x = UVVals[0];
                    UVMap->v[VertexIndex].y = UVVals[1];
                    UVMap->v[VertexIndex].z = UVVals[2];

                    UVMap->F(f)->tv[v] = VertexIndex;
                }
            }
            if (DoesCdAttrExist())
            {
                if (GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
                {
                    CdMap->F(f)->tv[v] = MaxMesh.F(f)->vtx[v];
                }
                else if (GetCdAttrOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
                {
                    GetVertexCdAtIndex(VertexIndex, CdVals);
                    CdMap->v[VertexIndex].x = CdVals[0];
                    CdMap->v[VertexIndex].y = CdVals[1];
                    CdMap->v[VertexIndex].z = CdVals[2];

                    CdMap->F(f)->tv[v] = VertexIndex;
                }
            }
            if (DoesAlphaAttrExist())
            {
                if (GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
                {
                    AlphaMap->F(f)->tv[v] = MaxMesh.F(f)->vtx[v];
                }
                else if (GetAlphaAttrOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
                {
                    AlphaVal = GetAlphaAtIndex(VertexIndex);
                    AlphaMap->v[VertexIndex].x = AlphaVal;
                    AlphaMap->v[VertexIndex].y = AlphaVal;
                    AlphaMap->v[VertexIndex].z = AlphaVal;

                    AlphaMap->F(f)->tv[v] = VertexIndex;
                }
            }
            if (DoesIlluminationAttrExist())
            {
                if (GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_POINT)
                {
                    IlluminationMap->F(f)->tv[v] = MaxMesh.F(f)->vtx[v];
                }
                else if (GetIlluminationOwner() == HEMAX_ATTRIBUTEOWNER_VERTEX)
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
        if (DoesSecondaryUVLayerExist(HEMAX_ATTRIBUTEOWNER_POINT, Layer))
        {
            MNMap* UVMap = MaxMesh.M(Layer);

            if (!UVMap)
            {
                MaxMesh.InitMap(Layer);
                UVMap = MaxMesh.M(Layer);
            }

            if (UVMap)
            {
                UVMap->ClearAllFlags();
                UVMap->setNumFaces(GetFaceCount());
                UVMap->setNumVerts(GetPointCount());

                std::vector<float>& UVWValues = GetSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_POINT, Layer);

                for (int p = 0; p < GetPointCount(); p++)
                {
                    UVMap->v[p].x = UVWValues[(p * 3)];
                    UVMap->v[p].y = UVWValues[(p * 3) + 1];
                    UVMap->v[p].z = UVWValues[(p * 3) + 2];
                }

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
        else if (DoesSecondaryUVLayerExist(HEMAX_ATTRIBUTEOWNER_VERTEX, Layer))
        {
            MNMap* UVMap = MaxMesh.M(Layer);

            if (!UVMap)
            {
                MaxMesh.InitMap(Layer);
                UVMap = MaxMesh.M(Layer);
            }

            if (UVMap)
            {
                UVMap->ClearAllFlags();
                UVMap->setNumFaces(GetFaceCount());
                UVMap->setNumVerts(GetVertexCount());

                std::vector<float>& UVWValues = GetSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_VERTEX, Layer);
                int VertexIndex = 0;

                for (int f = 0; f < GetFaceCount(); f++)
                {
                    UVMap->F(f)->SetSize(GetFaceVertexCount(f));

                    for (int v = GetFaceVertexCount(f) - 1; v >= 0; v--)
                    {
                        UVMap->v[VertexIndex].x = UVWValues[(VertexIndex * 3)];
                        UVMap->v[VertexIndex].y = UVWValues[(VertexIndex * 3) + 1];
                        UVMap->v[VertexIndex].z = UVWValues[(VertexIndex * 3) + 2];

                        UVMap->F(f)->tv[v] = VertexIndex;

                        VertexIndex++;
                    }
                }
            }
        }
    }

    MaxMesh.InvalidateGeomCache();
    MaxMesh.InvalidateTopoCache();

    MaxMesh.FillInMesh();
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
HEMAX_Mesh::CreateSecondaryUVLayer(HEMAX_AttributeOwner Owner, int Layer, size_t Size)
{
    std::vector<float> SizedList;
    SizedList.resize(Size);

    if (Owner == HEMAX_ATTRIBUTEOWNER_POINT)
    {
        SecondaryPointUVs.insert({ Layer, SizedList });
        SecondaryUVCount++;
    }
    else if (Owner == HEMAX_ATTRIBUTEOWNER_VERTEX)
    {
        SecondaryVertexUVs.insert({ Layer, SizedList });
        SecondaryUVCount++;
    }
}

std::vector<float>&
HEMAX_Mesh::GetSecondaryUVLayer(HEMAX_AttributeOwner Owner, int Layer)
{
    if (Owner == HEMAX_ATTRIBUTEOWNER_POINT)
    {
        auto Search = SecondaryPointUVs.find(Layer);
        return Search->second;
    }
    else
    {
        // (Owner == HEMAX_ATTRIBUTEOWNER_VERTEX)
        auto Search = SecondaryVertexUVs.find(Layer);
        return Search->second;
    }
}

bool
HEMAX_Mesh::DoesSecondaryUVLayerExist(HEMAX_AttributeOwner Owner, int Layer)
{
    if (Owner == HEMAX_ATTRIBUTEOWNER_POINT)
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
    else if (Owner == HEMAX_ATTRIBUTEOWNER_VERTEX)
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