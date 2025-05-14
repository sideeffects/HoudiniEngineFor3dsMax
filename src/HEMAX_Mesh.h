#pragma once

#include "HEMAX_Types.h"
#include "HEMAX_MaterialNode.h"

#include <HAPI.h>
#include <unordered_map>
#include <unordered_set>

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <inode.h>
#include <mnmesh.h>
#pragma warning(pop)

template<typename T>
class HEMAX_MeshList
{
public:
                                    HEMAX_MeshList() = default;
                                    ~HEMAX_MeshList() = default;

    void                            Init(std::size_t _Size,
                                            std::size_t _TupleSize,
                                            HAPI_AttributeOwner _Owner);
    T*                              Data();
    const T*                        DataConst() const;
    void                            Value(int Index, std::vector<T>& DataOut);
    std::size_t                     DataSize();
    std::size_t                     DataTupleSize();
    HAPI_AttributeOwner             DataOwner();
    bool                            DataExists() const { return Exists; }

    void			        MergeEqualTuples();

    const T*                        MergedDataConst() const;
    std::size_t                     MergedDataSize();
    std::size_t                     GetMergedIndex(int Index);	

    void                            Clear();

private:
    std::vector<T>                  List;
    HAPI_AttributeOwner             Owner           = HAPI_ATTROWNER_INVALID;
    std::size_t                     Size            = 0;
    std::size_t                     TupleSize       = 0;

    std::vector<T>		    TupleSet;
    std::vector<std::size_t>        IndexMapping;

    bool                            Exists          = false;
    bool                            IsMerged        = false;
};

class HEMAX_Mesh
{
public:

    enum MetadataType
    {
        INT,
        FLOAT,
        STRING
    };

public:

                                        HEMAX_Mesh() = default;
                                        ~HEMAX_Mesh() = default;

    void                                Clear();
    void                                InitFromPart(const HAPI_NodeId Node,
                                                const HAPI_PartInfo& PartInfo);
    void                                BuildMNMesh(MNMesh& MaxMesh);

    std::size_t                         GetFaceCount() const;

    const std::string&                  GetMaterialPath() const;
    std::size_t                         GetNumMaterials() const;
    const HAPI_NodeId*                  GetMaterialNodeIds() const;

    void                                ApplyMetadataToINode(INode& Node) const;

private:

    void                                SetPointCount(const std::size_t Count);
    void                                SetVertexCount(const std::size_t Count);
    void                                SetFaceCount(const std::size_t Count);

    bool                                AddMetadata(
                                            MetadataType Type,
                                            const std::string& AttribName,
                                            const HAPI_AttributeInfo& AttribInfo);

private:
    
    std::size_t                         MyNumPoints                     = 0;
    std::size_t                         MyNumVertices                   = 0;
    std::size_t                         MyNumFaces                      = 0;

    int                                 MyMaxMapLayer                   = -1;

    HEMAX_MeshList<float>               MyPositions;
    std::vector<int>                    MyFaceCounts;
    std::vector<int>                    MyVertices;
    HEMAX_MeshList<float>               MyNormals;
    HEMAX_MeshList<float>               MyPrimaryUVs;
    HEMAX_MeshList<float>               MyColors;
    HEMAX_MeshList<float>               MyAlpha;
    HEMAX_MeshList<float>               MyIllumination;
    HEMAX_MeshList<int>                 MySmoothingGroups;
    HEMAX_MeshList<int>                 MyMaterialIds;
    HEMAX_MeshList<HAPI_NodeId>         MyMaterialNodeIds;

    std::vector<int>                    MyFaceSelections;
    std::vector<int>                    MyVertexSelections;
    std::vector<int>                    MyEdgeSelections;

    std::unordered_map<
        std::size_t,
        HEMAX_MeshList<float>>          MySecondaryUVs;

    std::string                         MyMaterialPath;
    std::size_t                         MyNumMaterials                  = 0;

    std::unordered_map<
        std::string,
        HEMAX_MeshList<int>>            MyIntMetadata;
    std::unordered_map<
        std::string,
        HEMAX_MeshList<float>>          MyFloatMetadata;
    std::unordered_map<
        std::string,
        HEMAX_MeshList<std::string>>    MyStringMetadata;
};
