#pragma once

#include "HEMAX_Types.h"
#include "HEMAX_MaterialNode.h"
#include <unordered_map>
#include <unordered_set>
#include <inode.h>
#include "mnmesh.h"

enum HEMAX_Mesh_MetadataType
{
    HEMAX_MESH_INT_METADATA,
    HEMAX_MESH_FLOAT_METADATA,
    HEMAX_MESH_STRING_METADATA
};

template<typename T>
class HEMAX_MeshList
{
    public:
	HEMAX_MeshList();
	~HEMAX_MeshList();

	void                        Init(unsigned int _Size, unsigned int _TupleSize, HAPI_AttributeOwner _Owner);
	T*                          Data();
	std::vector<T>              Value(int Index);
	unsigned int                DataSize();
	unsigned int                DataTupleSize();
	HAPI_AttributeOwner        DataOwner();

	void			    MergeEqualTuples();

	unsigned int		    MergedDataSize();
	std::vector<T>&		    MergedValues();
	unsigned int		    GetMergedIndex(int Index);	

    private:
	T*                          List;
	HAPI_AttributeOwner        Owner;
	unsigned int                Size;
	unsigned int                TupleSize;

	std::vector<T>		    TupleSet;
	std::vector<unsigned int>   IndexMapping;

        bool                        IsMerged;
};

class HEMAX_Mesh
{
    public:

	HEMAX_Mesh();
	HEMAX_Mesh( int FCount, int VCount, int PCount );
	~HEMAX_Mesh();

	int GetFaceCount();
	int GetVertexCount();
	int GetPointCount();
	int GetPointUVCount();
	int GetVertexUVCount();
	int GetNumMaterials();

	int* GetFaceVertexCountsArray();
	int* GetVertexListArray();
	float* GetPointListArray();
	float* GetPointNormalsListArray();
	float* GetVertexNormalsListArray();
	HAPI_NodeId* GetMaterialIdsArray();
	float* GetPointUVArray();
	float* GetVertexUVArray();
        float* GetSecondaryPointUVArray(int Layer);
        float* GetSecondaryVertexUVArray(int Layer);
	float* GetPointCdArray();
	float* GetVertexCdArray();
	float* GetAlphaArray();
	float* GetIlluminationArray();
	int* GetSmoothingGroupArray();
	int* GetMaterialIDArray();
        int* GetFaceSelectionsArray();
        int* GetVertexSelectionsArray();
        int* GetEdgeSelectionsArray();

	int GetFaceVertexCount(int Index);
	void GetPointAtIndex( int Index, float* Point );
	void GetPointNormalAtIndex(int Index, float* Normal);
	void GetVertexNormalAtIndex(int Index, float* Normal);
	int GetVertex( int Index );
	void GetPointUVAtIndex(int Index, float* UVVals);

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

	void AllocatePointCdArray(int TupleSize);
	void AllocateVertexCdArray(int TupleSize);

	void AllocateAlphaArray(HAPI_AttributeOwner Owner);
	void AllocateIlluminationArray(HAPI_AttributeOwner Owner);

	void AllocateSmoothingGroupsArray();
	void AllocateMaterialIDArray();
        void AllocateFaceSelectionsArray();
        void AllocateVertexSelectionsArray();
        void AllocateEdgeSelectionsArray(int EdgeCount);

	HEMAX_NormalType GetNormalType();
	HEMAX_UVType GetUVType();

	bool DoesCdAttrExist();
	HAPI_AttributeOwner GetCdAttrOwner();

	bool DoesAlphaAttrExist();
	HAPI_AttributeOwner GetAlphaAttrOwner();

	bool DoesIlluminationAttrExist();
	HAPI_AttributeOwner GetIlluminationOwner();

	bool DoesSmoothingGroupAttrExist();
	bool DoesMaterialIDAttrExist();

	bool DoUVsExist();

        bool HasFaceSelections() const;
        bool HasVertexSelections() const;
        bool HasEdgeSelections() const;

	int GetPostTriangulationFaceCount();

        void CreateSecondaryUVLayer(int Layer, const HAPI_AttributeInfo& Attr);
        HEMAX_MeshList<float>* GetSecondaryUVLayer(HAPI_AttributeOwner Owner, int Layer);
	bool DoesSecondaryUVLayerExist(HAPI_AttributeOwner Owner, int Layer);

	void AddMetadata(std::string Name, HEMAX_Mesh_MetadataType Type, unsigned int Size, unsigned int TupleSize, HAPI_AttributeOwner Owner);

	HEMAX_MeshList<int>& GetIntMetadata(std::string Name);
	HEMAX_MeshList<float>& GetFloatMetadata(std::string Name);
	HEMAX_MeshList<std::string>& GetStringMetadata(std::string Name);

	void ApplyDetailMetadata(INode* Node);

	void MarshallDataInto3dsMaxMNMesh(MNMesh& MaxMesh);

	std::string MaterialPath;

	bool AreMaterialIdsSame;

    private:

	bool NormalsExist;
	HEMAX_NormalType NormalType;

	int FaceCount;
	int VertexCount;
	int PointCount;

	HEMAX_MeshList<HAPI_NodeId> FaceMaterialIds;
	HEMAX_MeshList<int> FaceVertexCounts;
	HEMAX_MeshList<int> VertexList;
	HEMAX_MeshList<float> PointList;
	HEMAX_MeshList<float> Normals;
	HEMAX_MeshList<float> UVList;
	HEMAX_MeshList<float> CdList;
	HEMAX_MeshList<float> AlphaList;
	HEMAX_MeshList<float> IlluminationList;
	HEMAX_MeshList<int> SmoothingGroupList;
	HEMAX_MeshList<int> MaterialIDList;
        HEMAX_MeshList<int> FaceSelectionsList;
        HEMAX_MeshList<int> VertexSelectionsList;
        HEMAX_MeshList<int> EdgeSelectionsList;

	std::unordered_map<std::string, HEMAX_MeshList<int>> IntMetadata;
	std::unordered_map<std::string, HEMAX_MeshList<float>> FloatMetadata;
	std::unordered_map<std::string, HEMAX_MeshList<std::string>> StringMetadata;

	bool HasUVs;
	HEMAX_UVType UVType;

	bool ColorAttrExists;
	HAPI_AttributeOwner ColorAttrOwner;

	bool AlphaAttrExists;
	HAPI_AttributeOwner AlphaAttrOwner;

	bool IlluminationAttrExists;
	HAPI_AttributeOwner IlluminationAttrOwner;

	bool SmoothingGroupsExist;
	bool MaterialIDsExist;
        bool FaceSelectionsExist;
        bool VertexSelectionsExist;
        bool EdgeSelectionsExist;

        std::unordered_map<int, HEMAX_MeshList<float>> SecondaryVertexUVs;
        std::unordered_map<int, HEMAX_MeshList<float>> SecondaryPointUVs;

        int MaxMapLayer;
};
