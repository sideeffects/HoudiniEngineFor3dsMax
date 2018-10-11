#pragma once

#include "HEMAX_Types.h"
#include "HEMAX_MaterialNode.h"
#include <unordered_map>
#include "mnmesh.h"

class HEMAX_Mesh
{
public:

    HEMAX_Mesh();
    HEMAX_Mesh( int FCount, int VCount, int PCount, int PTupleSize );
    ~HEMAX_Mesh();

    void SetFaceCount( int Count );
    void SetVertexCount ( int Count );
    void SetPointCount ( int Count, int TupleSize );

    int GetFaceCount();
    int GetVertexCount();
    int GetPointCount();
    int GetPointTupleSize();
    int GetPointUVCount();
    int GetVertexUVCount();
    int GetUVTupleSize();
    int GetNumMaterials();

    void SetFaceVertexCounts( int* FVCounts );
    void SetVertexList( int* VList );
    void SetPointList( float* PList );
    void SetPointNormalsList(float* PNList);
    void SetVertexNormalsList(float* VNList);

    int* GetFaceVertexCountsArray();
    int* GetVertexListArray();
    float* GetPointListArray();
    float* GetPointNormalsListArray();
    float* GetVertexNormalsListArray();
    HEMAX_NodeId* GetMaterialIdsArray();
    float* GetPointUVArray();
    float* GetVertexUVArray();
    float* GetPointCdArray();
    float* GetVertexCdArray();
    float* GetAlphaArray();
    float* GetIlluminationArray();
    int* GetSmoothingGroupArray();
    int* GetMaterialIDArray();

    int GetFaceVertexCount(int Index);
    void GetPointAtIndex( int Index, float* Point );
    void GetPointNormalAtIndex(int Index, float* Normal);
    void GetVertexNormalAtIndex(int Index, float* Normal);
    int GetVertex( int Index );
    void GetPointUVAtIndex(int Index, float* UVVals);
    void GetVertexUVAtIndex(int Index, float* UVVals);

    void GetPointCdAtIndex(int Index, float* CdVals);
    void GetVertexCdAtIndex(int Index, float* CdVals);

    float GetAlphaAtIndex(int Index);

    void GetIlluminationAtIndex(int Index, float* IlluminationVals);

    void SetNormalsExist(bool Exist);
    bool DoNormalsExist();

    void AllocatePointNormalArray();
    void AllocateVertexNormalArray();
    void AllocateMaterialIdsArray();
    void AllocatePointUVArray(int TupleSize);
    void AllocateVertexUVArray(int TupleSize);

    void AllocatePointCdArray();
    void AllocateVertexCdArray();

    void AllocateAlphaArray(HEMAX_AttributeOwner Owner);
    void AllocateIlluminationArray(HEMAX_AttributeOwner Owner);

    void AllocateSmoothingGroupsArray();
    void AllocateMaterialIDArray();

    HEMAX_NormalType GetNormalType();
    HEMAX_UVType GetUVType();

    bool DoesCdAttrExist();
    HEMAX_AttributeOwner GetCdAttrOwner();

    bool DoesAlphaAttrExist();
    HEMAX_AttributeOwner GetAlphaAttrOwner();

    bool DoesIlluminationAttrExist();
    HEMAX_AttributeOwner GetIlluminationOwner();

    bool DoesSmoothingGroupAttrExist();
    bool DoesMaterialIDAttrExist();

    bool DoUVsExist();

    void MarshallDataInto3dsMaxMesh(Mesh& MaxMesh);
    void MarshallDataInto3dsMaxMNMesh(MNMesh& MaxMesh);

    int GetPostTriangulationFaceCount();

    void CreateSecondaryUVLayer(HEMAX_AttributeOwner Owner, int Layer, size_t Size);
    std::vector<float>& GetSecondaryUVLayer(HEMAX_AttributeOwner Owner, int Layer);
    bool DoesSecondaryUVLayerExist(HEMAX_AttributeOwner Owner, int Layer);

    std::string MaterialPath;

    bool AreMaterialIdsSame;

private:
    
    bool NormalsExist;
    HEMAX_NormalType NormalType;

    int FaceCount;
    int VertexCount;
    int PointCount;
    int PointTupleSize;

    HEMAX_NodeId* FaceMaterialIds;

    int* FaceVertexCounts;
    int* VertexList;
    float* PointList;
    float* PointNormals;
    float* VertexNormals;

    float* UVPointArray;
    float* UVVertArray;
    int UVTupleSize;
    bool HasUVs;
    HEMAX_UVType UVType;

    bool ColorAttrExists;
    HEMAX_AttributeOwner ColorAttrOwner;

    float* PointCdArray;
    float* VertexCdArray;

    float* AlphaArray;
    bool AlphaAttrExists;
    HEMAX_AttributeOwner AlphaAttrOwner;

    float* IlluminationArray;
    bool IlluminationAttrExists;
    HEMAX_AttributeOwner IlluminationAttrOwner;

    int* SmoothingGroupArray;
    bool SmoothingGroupsExist;

    int* MaterialIDArray;
    bool MaterialIDsExist;

    std::unordered_map<int, std::vector<float>> SecondaryVertexUVs;
    std::unordered_map<int, std::vector<float>> SecondaryPointUVs;

    int SecondaryUVCount;
};
