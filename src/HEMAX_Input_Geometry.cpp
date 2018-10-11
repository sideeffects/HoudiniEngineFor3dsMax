#include "HEMAX_Input_Geometry.h"
#include "HEMAX_SessionManager.h"
#include "triobj.h"
#include "polyobj.h"
#include "MeshNormalSpec.h"
#include "MNNormalSpec.h"
#include "VertexNormal.h"

#include "stdmat.h"

HEMAX_Input_Geometry::HEMAX_Input_Geometry(ULONG MaxNode)
    : HEMAX_Input(MaxNode)
{
    BuildInputNode();
}

HEMAX_Input_Geometry::HEMAX_Input_Geometry(HEMAX_InputType Type, int Id, ULONG MaxNode) : HEMAX_Input(Type, Id, MaxNode)
{
    BuildInputNode();
}

HEMAX_Input_Geometry::HEMAX_Input_Geometry(HEMAX_InputType Type, TriObject* MaxTriObject) : HEMAX_Input(Type, -1, -1)
{
    if (MaxTriObject)
    {
        BuildGeometryForInputNode(Node, MaxTriObject->mesh, "modifier_input", HEMAX_Utilities::GetIdentityTransform());
    }
}

HEMAX_Input_Geometry::HEMAX_Input_Geometry(HEMAX_InputType Type, PolyObject* MaxPolyObject, INode* MaxNode) : HEMAX_Input(Type, -1, -1)
{
    if (MaxPolyObject)
    {
        BuildPolyGeometryForInputNode(Node, MaxPolyObject->GetMesh(), "modifier_input", HEMAX_Utilities::GetIdentityTransform(), MaxNode);
    }
}

HEMAX_Input_Geometry::~HEMAX_Input_Geometry()
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    if (SessionManager.IsSessionActive())
    {
        HEMAX_NodeId ParentNodeId = Node->Info.parentId;
        DeleteNode(*Node);
        HEMAX_SessionManager::GetSessionManager().Session->DeleteNode(ParentNodeId);
    }
}

void
HEMAX_Input_Geometry::RebuildAfterChange()
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HEMAX_NodeId ParentNodeId = Node->Info.parentId;
    DeleteNode(*Node);
    HEMAX_SessionManager::GetSessionManager().Session->DeleteNode(ParentNodeId);
    BuildInputNode();
}

void
HEMAX_Input_Geometry::BuildInputNode()
{
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    if (MaxInputNode)
    {
        ObjectState MaxObjectState = MaxInputNode->EvalWorldState(GetCOREInterface()->GetTime());
        Object* MaxObject = MaxObjectState.obj;

        if (MaxObject->CanConvertToType(Class_ID(POLYOBJ_CLASS_ID, 0)))
        {
            PolyObject* MaxPolyObject = (PolyObject*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), Class_ID(POLYOBJ_CLASS_ID, 0));
            INode* InputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

            BuildPolyGeometryForInputNode(Node, MaxPolyObject->GetMesh(), GetInputNodeName(), HEMAX_Utilities::BuildMaxTransformFromINode(InputNode), MaxInputNode);

            if (MaxObject != MaxPolyObject)
            {
                delete MaxPolyObject;
            }
        }
        else if (MaxObject->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
        {
            TriObject* MaxTriObject = (TriObject*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), Class_ID(TRIOBJ_CLASS_ID, 0));

            INode* InputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

            BuildGeometryForInputNode(Node, MaxTriObject->mesh, GetInputNodeName(), HEMAX_Utilities::BuildMaxTransformFromINode(InputNode));

            if (MaxObject != MaxTriObject)
            {
                delete MaxTriObject;
            }
        }
    }
}

void
HEMAX_Input_Geometry::BuildPolyGeometryForInputNode(HEMAX_Node* Node, MNMesh& MaxMesh, std::string InputNodeName, HEMAX_MaxTransform NodeTransform, INode* MaxNode)
{
    CreateInputNode(*Node, InputNodeName + "_" + std::to_string(rand()) + std::to_string(rand()));

    MaxMesh.buildNormals();

    // Look up material (if it exists)
    INode* SourceNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    bool SingleMaterial = false;
    const char* SingleMaterialName;
    std::vector<const char*> SingleMaterialNameData;

    std::unordered_map<int, const char*> SubMatNames;
    std::vector<const char*> FaceMaterialNames;

    if (SourceNode)
    {
        Mtl* MeshMat = SourceNode->GetMtl();

        if (MeshMat)
        {
            // We have a multi-material
            if (MeshMat->NumSubMtls() > 0)
            {
                FaceMaterialNames.resize(MaxMesh.FNum());
                for (int m = 0; m < MeshMat->NumSubMtls(); m++)
                {
                    Mtl* SubMat = MeshMat->GetSubMtl(m);

                    if (SubMat)
                    {
                        SubMatNames.insert({ m, SubMat->GetName().ToCStr() });
                    }
                }
            }
            else
            {
                SingleMaterial = true;
                SingleMaterialName = MeshMat->GetName().ToCStr();
                SingleMaterialNameData = std::vector<const char*>(MaxMesh.FNum(), SingleMaterialName);
            }
        }
    }

    // Positions

    int FaceCount = MaxMesh.FNum();
    int VertCount = MaxMesh.VNum();

    float* PointArray = new float[VertCount * 3];
    int* FaceCountArray = new int[FaceCount];
    int* SmoothingGroupArray = new int[FaceCount];
    int* MaterialIDArray = new int[FaceCount];

    float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

    for (int i = 0; i < VertCount; i++)
    {
        PointArray[i * 3] = MaxMesh.V(i)->p.x * ScaleConversion;
        PointArray[(i * 3) + 1] = MaxMesh.V(i)->p.z * ScaleConversion;
        PointArray[(i * 3) + 2] = -MaxMesh.V(i)->p.y * ScaleConversion;
    }

    int VertIndexCount = 0;

    for (int i = 0; i < FaceCount; i++)
    {
        FaceCountArray[i] = MaxMesh.F(i)->deg;
        VertIndexCount += MaxMesh.F(i)->deg;
    }

    int* VertIndexArray = new int[VertIndexCount];
    int CurIndex = 0;

    for (int i = 0; i < FaceCount; i++)
    {
        // Smoothing group
        SmoothingGroupArray[i] = MaxMesh.F(i)->smGroup;
        MaterialIDArray[i] = MaxMesh.F(i)->material;

        if (!SingleMaterialName)
        {
            auto MatName = SubMatNames.find(MaxMesh.F(i)->material);

            if (MatName != SubMatNames.end())
            {
                FaceMaterialNames[i] = MatName->second;
            }
        }

        for (int v = (MaxMesh.F(i)->deg - 1), c = 0; v >= 0; v--, c++)
        {
            VertIndexArray[CurIndex + c] = MaxMesh.F(i)->vtx[v];
        }
        CurIndex += MaxMesh.F(i)->deg;
    }

    AddNewPart(*Node, HEMAX_PARTTYPE_MESH, FaceCount, VertIndexCount, VertCount);
    HEMAX_AttributeInfo PointAttributeInfo = AddNewPointAttribute(*Node, VertCount, 3, HEMAX_POSITION_ATTRIBUTE);
    SendPointAttributeData(*Node, PointAttributeInfo, PointArray, VertIndexArray, FaceCountArray, FaceCount, VertIndexCount, VertCount, HEMAX_POSITION_ATTRIBUTE);

    HEMAX_AttributeInfo SmoothingGroupAttributeInfo = AddNewPrimitiveIntAttribute(*Node, FaceCount, 1, HEMAX_SMOOTHING_GROUP_ATTRIBUTE);
    SendIntAttributeData(*Node, HEMAX_SMOOTHING_GROUP_ATTRIBUTE, SmoothingGroupAttributeInfo, SmoothingGroupArray, FaceCount);

    HEMAX_AttributeInfo MaterialIDAttributeInfo = AddNewPrimitiveIntAttribute(*Node, FaceCount, 1, HEMAX_MATERIAL_ID_ATTRIBUTE);
    SendIntAttributeData(*Node, HEMAX_MATERIAL_ID_ATTRIBUTE, MaterialIDAttributeInfo, MaterialIDArray, FaceCount);

    if (SingleMaterial && SingleMaterialNameData.size() > 0)
    {
        HEMAX_AttributeInfo MaterialNamesAttrInfo = AddNewPrimitiveStringAttribute(*Node, FaceCount, 1, HEMAX_MATERIAL_PATH_ATTRIBUTE);
        SendStringAttributeData(*Node, HEMAX_MATERIAL_PATH_ATTRIBUTE, MaterialNamesAttrInfo, SingleMaterialNameData.data(), FaceCount);
    }
    else if (FaceMaterialNames.size() > 0)
    {
        HEMAX_AttributeInfo MaterialNamesAttrInfo = AddNewPrimitiveStringAttribute(*Node, FaceCount, 1, HEMAX_MATERIAL_PATH_ATTRIBUTE);
        SendStringAttributeData(*Node, HEMAX_MATERIAL_PATH_ATTRIBUTE, MaterialNamesAttrInfo, FaceMaterialNames.data(), FaceCount);
    }

    // Normals
    std::vector<float> NormalArray;

    MNNormalSpec* SpecifiedNormals = MaxMesh.GetSpecifiedNormals();

    if (!SpecifiedNormals || SpecifiedNormals->GetNumNormals() == 0)
    {
        std::unordered_map<int, int> NoSGTable;

        MNVert* Vertices = MaxMesh.v;

        Tab<MaxSDK::VertexNormal*> VertexNormals;
        Tab<Point3> FaceNormals;

        VertexNormals.SetCount(VertIndexCount);
        FaceNormals.SetCount(FaceCount);

        for (int i = 0; i < VertIndexCount; i++)
        {
            VertexNormals[i] = new MaxSDK::VertexNormal();
        }

        for (int i = 0; i < FaceCount; i++)
        {
            FaceNormals[i] = MaxMesh.GetFaceNormal(i, true);

            for (int j = MaxMesh.F(i)->deg - 1; j >= 0; j--)
            {
                VertexNormals[MaxMesh.F(i)->vtx[j]]->AddNormal(FaceNormals[i], MaxMesh.F(i)->smGroup);

                if (MaxMesh.F(i)->smGroup == 0)
                {
                    NoSGTable.insert({ MaxMesh.F(i)->vtx[j], 0 });
                }
            }
        }

        for (int i = 0; i < FaceCount; i++)
        {
            for (int j = MaxMesh.F(i)->deg - 1; j >= 0; j--)
            {
                int Vert = MaxMesh.F(i)->vtx[j];

                Point3 NormalizedNormal;

                if (MaxMesh.F(i)->smGroup == 0)
                {
                    auto Search = NoSGTable.find({ Vert });
                    int Index = Search->second;

                    MaxSDK::VertexNormal* VNorm = VertexNormals[Vert];

                    while (Index > 0)
                    {
                        VNorm = VNorm->next;
                        --Index;
                    }

                    NormalizedNormal = Normalize(VNorm->norm);

                    ++(Search->second);
                }
                else
                {
                    NormalizedNormal = Normalize(VertexNormals[Vert]->GetNormal(MaxMesh.F(i)->smGroup));
                }

                NormalArray.push_back(NormalizedNormal.x);
                NormalArray.push_back(NormalizedNormal.z);
                NormalArray.push_back(-NormalizedNormal.y);
            }
        }

        for (int z = 0; z < VertexNormals.Count(); z++)
        {
            delete VertexNormals[z];
        }
    }
    else
    {
        int SpecNormalCount = SpecifiedNormals->GetNumNormals();
        int SpecFaceCount = SpecifiedNormals->GetNumFaces();

        for (int f = 0; f < SpecFaceCount; f++)
        {
            for (int v = SpecifiedNormals->Face(f).GetDegree() - 1; v >= 0; v--)
            {
                Point3 NormalizedNormal = Normalize(SpecifiedNormals->GetNormal(f, v));

                NormalArray.push_back(NormalizedNormal.x);
                NormalArray.push_back(NormalizedNormal.z);
                NormalArray.push_back(-NormalizedNormal.y);
            }
        }
    }

    HEMAX_AttributeInfo NormalAttributeInfo = AddNewVertexAttribute(*Node, VertIndexCount, 3, HEMAX_NORMAL_ATTRIBUTE);
    SendFloatAttributeData(*Node, HEMAX_NORMAL_ATTRIBUTE, NormalAttributeInfo, &NormalArray.front(), VertIndexCount);

    // UVs
    std::vector<float> UVArray;

    for (int texMap = 1; texMap < MAX_MESHMAPS; texMap++)
    {
        MNMap* UVMap = MaxMesh.M(texMap);
        UVArray.clear();

        if (UVMap)
        {
            UVVert* MapData = UVMap->v;

            if (MapData)
            {
                for (int f = 0; f < FaceCount; f++)
                {
                    for (int v = MaxMesh.F(f)->deg - 1; v >= 0; v--)
                    {
                        UVArray.push_back(MapData[UVMap->F(f)->tv[v]].x);
                        UVArray.push_back(MapData[UVMap->F(f)->tv[v]].y);
                        UVArray.push_back(MapData[UVMap->F(f)->tv[v]].z);
                    }
                }

                std::string UVAttrName = (texMap == 1 ? HEMAX_UV_ATTRIBUTE : HEMAX_UV_ATTRIBUTE + std::to_string(texMap));

                HEMAX_AttributeInfo UVAttributeInfo = AddNewVertexAttribute(*Node, VertIndexCount, 3, UVAttrName);
                SendFloatAttributeData(*Node, UVAttrName, UVAttributeInfo, UVArray.data(), VertIndexCount);
            }
        }
    }

    // Soft Selection
    float* SoftSelectionArray = nullptr;

    if (MaxMesh.vDataSupport(VDATA_SELECT))
    {
        SoftSelectionArray = new float[VertCount];
        float* Data = MaxMesh.getVSelectionWeights();

        for (int i = 0; i < VertCount; i++)
        {
            SoftSelectionArray[i] = Data[i];
        }
    }

    float* CdArray = nullptr;
    HEMAX_AttributeOwner CdOwner = HEMAX_ATTRIBUTEOWNER_INVALID;

    MNMap* ColorMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_COLOR);
    if (ColorMap)
    {
        if (ColorMap->numv == VertIndexCount)
        {
            CdOwner = HEMAX_ATTRIBUTEOWNER_VERTEX;
            CdArray = new float[VertIndexCount * 3];

            UVVert* MapData = ColorMap->v;

            int CdVertIndex = 0;

            for (int f = 0; f < FaceCount; f++)
            {
                for (int v = MaxMesh.F(f)->deg - 1; v >= 0; v--)
                {
                    CdArray[CdVertIndex] = MapData[ColorMap->F(f)->tv[v]].x;
                    CdArray[CdVertIndex + 1] = MapData[ColorMap->F(f)->tv[v]].y;
                    CdArray[CdVertIndex + 2] = MapData[ColorMap->F(f)->tv[v]].z;

                    CdVertIndex += 3;
                }
            }
        }
        else if (ColorMap->numv == VertCount)
        {
            CdOwner = HEMAX_ATTRIBUTEOWNER_POINT;
            CdArray = new float[VertCount * 3];

            UVVert* MapData = ColorMap->v;

            for (int c = 0; c < VertCount; c++)
            {
                CdArray[(c * 3)] = MapData[c].x;
                CdArray[(c * 3) + 1] = MapData[c].y;
                CdArray[(c * 3) + 2] = MapData[c].z;
            }
        }
    }

    float* AlphaArray = nullptr;
    HEMAX_AttributeOwner AlphaOwner = HEMAX_ATTRIBUTEOWNER_INVALID;

    MNMap* AlphaMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_ALPHA);
    if (AlphaMap)
    {
        if (AlphaMap->numv == VertIndexCount)
        {
            AlphaOwner = HEMAX_ATTRIBUTEOWNER_VERTEX;
            AlphaArray = new float[VertIndexCount];

            UVVert* MapData = AlphaMap->v;

            int AlphaVertIndex = 0;

            for (int f = 0; f < FaceCount; f++)
            {
                for (int v = MaxMesh.F(f)->deg - 1; v >= 0; v--)
                {
                    float ConvertedAlpha = (0.2126f * MapData[AlphaMap->F(f)->tv[v]].x) + (0.7152f * MapData[AlphaMap->F(f)->tv[v]].y) + (0.0722f * MapData[AlphaMap->F(f)->tv[v]].z);

                    AlphaArray[AlphaVertIndex] = ConvertedAlpha;

                    AlphaVertIndex++;
                }
            }
        }
        else if (AlphaMap->numv == VertCount)
        {
            AlphaOwner = HEMAX_ATTRIBUTEOWNER_POINT;
            AlphaArray = new float[VertCount];

            UVVert* MapData = AlphaMap->v;

            for (int c = 0; c < VertCount; c++)
            {
                float ConvertedAlpha = (0.2126f * MapData[c].x) + (0.7152f * MapData[c].y) + (0.0722f * MapData[c].z);
                AlphaArray[c] = ConvertedAlpha;
            }
        }
    }

    float* IlluminationArray = nullptr;
    HEMAX_AttributeOwner IlluminationOwner = HEMAX_ATTRIBUTEOWNER_INVALID;

    MNMap* IlluminationMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_ILLUMINATION);
    if (IlluminationMap)
    {
        if (IlluminationMap->numv == VertIndexCount)
        {
            IlluminationOwner = HEMAX_ATTRIBUTEOWNER_VERTEX;
            IlluminationArray = new float[VertIndexCount * 3];

            UVVert* MapData = IlluminationMap->v;

            int CurrentIndex = 0;

            for (int f = 0; f < FaceCount; f++)
            {
                for (int v = MaxMesh.F(f)->deg - 1; v >= 0; v--)
                {
                    IlluminationArray[CurrentIndex] = MapData[IlluminationMap->F(f)->tv[v]].x;
                    IlluminationArray[CurrentIndex + 1] = MapData[IlluminationMap->F(f)->tv[v]].y;
                    IlluminationArray[CurrentIndex + 2] = MapData[IlluminationMap->F(f)->tv[v]].z;

                    CurrentIndex += 3;
                }
            }
        }
        else if (IlluminationMap->numv == VertCount)
        {
            IlluminationOwner = HEMAX_ATTRIBUTEOWNER_POINT;
            IlluminationArray = new float[VertCount * 3];

            UVVert* MapData = IlluminationMap->v;

            for (int c = 0; c < VertCount; c++)
            {
                IlluminationArray[(c * 3)] = MapData[c].x;
                IlluminationArray[(c * 3) + 1] = MapData[c].y;
                IlluminationArray[(c * 3) + 2] = MapData[c].z;
            }
        }
    }

    if (IlluminationArray)
    {
        if (IlluminationOwner == HEMAX_ATTRIBUTEOWNER_VERTEX)
        {
            HEMAX_AttributeInfo IlluminationAttributeInfo = AddNewVertexAttribute(*Node, VertIndexCount, 3, HEMAX_ILLUMINATION_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_ILLUMINATION_ATTRIBUTE, IlluminationAttributeInfo, IlluminationArray, VertIndexCount);
        }
        else if (IlluminationOwner = HEMAX_ATTRIBUTEOWNER_POINT)
        {
            HEMAX_AttributeInfo IlluminationAttributeInfo = AddNewPointAttribute(*Node, VertCount, 3, HEMAX_ILLUMINATION_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_ILLUMINATION_ATTRIBUTE, IlluminationAttributeInfo, IlluminationArray, VertCount);
        }

        delete[] IlluminationArray;
    }

    if (AlphaArray)
    {
        if (AlphaOwner == HEMAX_ATTRIBUTEOWNER_VERTEX)
        {
            HEMAX_AttributeInfo AlphaAttributeInfo = AddNewVertexAttribute(*Node, VertIndexCount, 1, HEMAX_ALPHA_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_ALPHA_ATTRIBUTE, AlphaAttributeInfo, AlphaArray, VertIndexCount);
        }
        else if (AlphaOwner == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            HEMAX_AttributeInfo AlphaAttributeInfo = AddNewPointAttribute(*Node, VertCount, 1, HEMAX_ALPHA_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_ALPHA_ATTRIBUTE, AlphaAttributeInfo, AlphaArray, VertCount);
        }

        delete[] AlphaArray;
    }

    if (CdArray)
    {
        if (CdOwner == HEMAX_ATTRIBUTEOWNER_VERTEX)
        {
            HEMAX_AttributeInfo CdAttributeInfo = AddNewVertexAttribute(*Node, VertIndexCount, 3, HEMAX_COLOR_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_COLOR_ATTRIBUTE, CdAttributeInfo, CdArray, VertIndexCount);
        }
        else if (CdOwner == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            HEMAX_AttributeInfo CdAttributeInfo = AddNewPointAttribute(*Node, VertCount, 3, HEMAX_COLOR_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_COLOR_ATTRIBUTE, CdAttributeInfo, CdArray, VertCount);
        }

        delete[] CdArray;
    }

    if (SoftSelectionArray)
    {
        HEMAX_AttributeInfo SoftSelectAttributeInfo = AddNewPointAttribute(*Node, VertCount, 1, HEMAX_SOFT_SELECTION_ATTRIBUTE);
        SendFloatAttributeData(*Node, HEMAX_SOFT_SELECTION_ATTRIBUTE, SoftSelectAttributeInfo, SoftSelectionArray, VertCount);

        delete[] SoftSelectionArray;
    }

    // Transform Detail Attributes

    if (MaxNode)
    {
        HEMAX_MaxTransform NodeTM = HEMAX_Utilities::BuildMaxTransformFromINode(MaxNode);
        EulerTM = HEMAX_Utilities::MaxTransformToHAPITransformEuler(NodeTM);

        HEMAX_AttributeInfo TranslateAttrInfo = AddNewDetailFloatAttribute(*Node, 1, 3, HEMAX_TRANSLATE_ATTR);
        HEMAX_AttributeInfo RotateAttrInfo = AddNewDetailFloatAttribute(*Node, 1, 3, HEMAX_ROTATE_ATTR);
        HEMAX_AttributeInfo ScaleAttrInfo = AddNewDetailFloatAttribute(*Node, 1, 3, HEMAX_SCALE_ATTR);

        SendFloatAttributeData(*Node, HEMAX_TRANSLATE_ATTR, TranslateAttrInfo, EulerTM.position, 1);
        SendFloatAttributeData(*Node, HEMAX_ROTATE_ATTR, RotateAttrInfo, EulerTM.rotationEuler, 1);
        SendFloatAttributeData(*Node, HEMAX_SCALE_ATTR, ScaleAttrInfo, EulerTM.scale, 1);
    }

    SetParentTransform(*Node, NodeTransform);
    FinalizeInputGeometry(*Node);

    delete[] VertIndexArray;
    delete[] MaterialIDArray;
    delete[] SmoothingGroupArray;
    delete[] FaceCountArray;
    delete[] PointArray;
}

void
HEMAX_Input_Geometry::BuildGeometryForInputNode(HEMAX_Node* Node, Mesh& MaxMesh, std::string InputNodeName, HEMAX_MaxTransform NodeTransform)
{
    CreateInputNode(*Node, InputNodeName + "_" + std::to_string(rand()) + std::to_string(rand()));

    MaxMesh.buildNormals();

    // Positions

    int FaceCount = MaxMesh.getNumFaces();
    int PointCount = MaxMesh.getNumVerts();

    Face* Faces = MaxMesh.faces;
    Point3* Points = MaxMesh.verts;

    float* PointArray = new float[PointCount * 3];
    std::vector<float> NormalArray;
    int* FaceCountArray = new int[FaceCount];
    int* VertexArray = new int[FaceCount * 3];
    int* SmoothingGroupArray = new int[FaceCount];

    float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

    for (int i = 0; i < PointCount; ++i)
    {
        PointArray[i * 3] = Points[i][0] * ScaleConversion;
        PointArray[i * 3 + 1] = Points[i][2] * ScaleConversion;
        PointArray[i * 3 + 2] = -Points[i][1] * ScaleConversion;
    }

    for (int i = 0; i < FaceCount; ++i)
    {
        FaceCountArray[i] = 3;
    }

    for (int i = 0; i < FaceCount; ++i)
    {
        VertexArray[i * 3] = Faces[i].getVert(2);
        VertexArray[i * 3 + 1] = Faces[i].getVert(1);
        VertexArray[i * 3 + 2] = Faces[i].getVert(0);

        // Smoothing groups
        SmoothingGroupArray[i] = Faces[i].smGroup;
    }

    AddNewPart(*Node, HEMAX_PARTTYPE_MESH, FaceCount, FaceCount * 3, PointCount);

    HAPI_AttributeInfo PointAttributeInfo = AddNewPointAttribute(*Node, PointCount, 3, HEMAX_POSITION_ATTRIBUTE);
    SendPointAttributeData(*Node, PointAttributeInfo, PointArray, VertexArray, FaceCountArray, FaceCount, FaceCount * 3, PointCount, HEMAX_POSITION_ATTRIBUTE);

    HEMAX_AttributeInfo SmoothingGroupAttrInfo = AddNewPrimitiveIntAttribute(*Node, FaceCount, 1, HEMAX_SMOOTHING_GROUP_ATTRIBUTE);
    SendIntAttributeData(*Node, HEMAX_SMOOTHING_GROUP_ATTRIBUTE, SmoothingGroupAttrInfo, SmoothingGroupArray, FaceCount);

    // Normals

    MeshNormalSpec* SpecifiedNormals = MaxMesh.GetSpecifiedNormals();

    if (!SpecifiedNormals || SpecifiedNormals->GetNumNormals() == 0)
    {
        std::unordered_map<int, int> NoSGTable;

        // Basically aliasing these because we need to think in 3dsmax terms while working with the vertices here
        Point3* Vertices = Points;
        int VertexCount = PointCount;

        Point3 V0, V1, V2;

        Tab<MaxSDK::VertexNormal*> VertexNormals;
        Tab<Point3> FaceNormals;

        VertexNormals.SetCount(VertexCount);
        FaceNormals.SetCount(FaceCount);

        for (int i = 0; i < VertexCount; ++i)
        {
            VertexNormals[i] = new MaxSDK::VertexNormal();
        }

        for (int i = 0; i < FaceCount; ++i)
        {
            V0 = Vertices[Faces[i].v[0]];
            V1 = Vertices[Faces[i].v[1]];
            V2 = Vertices[Faces[i].v[2]];

            FaceNormals[i] = (V1 - V0) ^ (V2 - V1);

            for (int j = 2; j >= 0; --j) {
                VertexNormals[Faces[i].v[j]]->AddNormal(FaceNormals[i], Faces[i].smGroup);

                if (Faces[i].smGroup == 0)
                {
                    NoSGTable.insert({ Faces[i].v[j], 0 });
                }
            }
        }

        for (int i = 0; i < FaceCount; ++i)
        {
            for (int j = 2; j >= 0; --j)
            {
                int Vert = Faces[i].v[j];

                Point3 NormalizedNormal;

                if (Faces[i].smGroup == 0)
                {
                    auto Search = NoSGTable.find({ Vert });
                    int Index = Search->second;

                    MaxSDK::VertexNormal* VNorm = VertexNormals[Vert];

                    while (Index > 0)
                    {
                        VNorm = VNorm->next;
                        --Index;
                    }

                    NormalizedNormal = Normalize(VNorm->norm);

                    ++(Search->second);
                }
                else
                {
                    NormalizedNormal = Normalize(VertexNormals[Vert]->GetNormal(Faces[i].smGroup));
                }

                NormalArray.push_back(NormalizedNormal.x);
                NormalArray.push_back(NormalizedNormal.z);
                NormalArray.push_back(-NormalizedNormal.y);
            }
        }

        for (int z = 0; z < VertexNormals.Count(); ++z)
        {
            delete VertexNormals[z];
        }
    }
    else
    {
        // If there ARE specified normals, we just use those to construct the normal list
        int SpecNormalCount = SpecifiedNormals->GetNumNormals();
        int SpecFaceCount = SpecifiedNormals->GetNumFaces();

        for (int f = 0; f < SpecFaceCount; f++)
        {
            for (int v = 2; v >= 0; v--)
            {
                Point3 NormalizedNormal = Normalize(SpecifiedNormals->GetNormal(f, v));

                NormalArray.push_back(NormalizedNormal.x);
                NormalArray.push_back(NormalizedNormal.z);
                NormalArray.push_back(-NormalizedNormal.y);
            }
        }
    }

    HEMAX_AttributeInfo NormalAttributeInfo = AddNewVertexAttribute(*Node, FaceCount * 3, 3, HEMAX_NORMAL_ATTRIBUTE);
    SendFloatAttributeData(*Node, HEMAX_NORMAL_ATTRIBUTE, NormalAttributeInfo, &NormalArray.front(), FaceCount * 3);

    // All UV Layers //

    std::vector<float> UVArray;

    for (int texMap = 1; texMap < MAX_MESHMAPS; texMap++)
    {
        if (MaxMesh.mapSupport(texMap))
        {
            UVArray.clear();

            if (MaxMesh.getNumMapVerts(texMap) == PointCount)
            {
                for (int u = 0; u < PointCount; u++)
                {
                    UVArray.push_back(MaxMesh.mapVerts(texMap)[u].x);
                    UVArray.push_back(MaxMesh.mapVerts(texMap)[u].y);
                    UVArray.push_back(MaxMesh.mapVerts(texMap)[u].z);
                }

                std::string UVAttrName = (texMap == 1 ? HEMAX_UV_ATTRIBUTE : HEMAX_UV_ATTRIBUTE + std::to_string(texMap));

                HEMAX_AttributeInfo UVAttributeInfo = AddNewPointAttribute(*Node, PointCount, 3, UVAttrName);
                SendFloatAttributeData(*Node, UVAttrName, UVAttributeInfo, UVArray.data(), PointCount);
            }
            else
            {
                for (int f = 0; f < FaceCount; f++)
                {
                    for (int v = 2; v >= 0; v--)
                    {
                        int VertexIndex = MaxMesh.mapFaces(texMap)[f].getTVert(v);

                        UVArray.push_back(MaxMesh.mapVerts(texMap)[VertexIndex].x);
                        UVArray.push_back(MaxMesh.mapVerts(texMap)[VertexIndex].y);
                        UVArray.push_back(MaxMesh.mapVerts(texMap)[VertexIndex].z);
                    }
                }

                std::string UVAttrName = (texMap == 1 ? HEMAX_UV_ATTRIBUTE : HEMAX_UV_ATTRIBUTE + std::to_string(texMap));

                HEMAX_AttributeInfo UVAttributeInfo = AddNewVertexAttribute(*Node, FaceCount * 3, 3, UVAttrName);
                SendFloatAttributeData(*Node, UVAttrName, UVAttributeInfo, UVArray.data(), FaceCount * 3);
            }
        }
    }

    ///////////////////

    // Soft selection
    float* SoftSelectArr = nullptr;

    if (MaxMesh.vDataSupport(VDATA_SELECT))
    {
        SoftSelectArr = new float[PointCount];
        float* Data = MaxMesh.getVSelectionWeights();

        for (int i = 0; i < PointCount; i++)
        {
            SoftSelectArr[i] = Data[i];
        }
    }

    // Colours

    float* CdArray = nullptr;
    HEMAX_AttributeOwner CdOwner = HEMAX_ATTRIBUTEOWNER_INVALID;

    if (MaxMesh.mapSupport(HEMAX_MAPPING_CHANNEL_COLOR))
    {
        if (MaxMesh.getNumVertCol() == FaceCount * 3)
        {
            CdOwner = HEMAX_ATTRIBUTEOWNER_VERTEX;
            int VertCount = FaceCount * 3;
            CdArray = new float[VertCount * 3];

            UVVert* MapData = MaxMesh.mapVerts(HEMAX_MAPPING_CHANNEL_COLOR);

            int CdVertIndex = 0;
            for (int f = 0; f < FaceCount; f++)
            {
                for (int v = 2; v >= 0; v--)
                {
                    CdArray[CdVertIndex] = MapData[MaxMesh.vcFace[f].t[v]].x;
                    CdArray[CdVertIndex + 1] = MapData[MaxMesh.vcFace[f].t[v]].y;
                    CdArray[CdVertIndex + 2] = MapData[MaxMesh.vcFace[f].t[v]].z;

                    CdVertIndex += 3;
                }
            }
        }
        else if (MaxMesh.getNumVertCol() == PointCount)
        {
            CdOwner = HEMAX_ATTRIBUTEOWNER_POINT;
            CdArray = new float[PointCount];

            UVVert* MapData = MaxMesh.mapVerts(HEMAX_MAPPING_CHANNEL_COLOR);

            for (int c = 0; c < PointCount; c++)
            {
                CdArray[(c * 3)] = MapData[c].x;
                CdArray[(c * 3) + 1] = MapData[c].y;
                CdArray[(c * 3) + 2] = MapData[c].z;
            }
        }
    }

    // Vertex alpha
    float* VertexAlphaArray = nullptr;
    HEMAX_AttributeOwner VertAlphaOwner = HEMAX_ATTRIBUTEOWNER_INVALID;

    if (MaxMesh.mapSupport(HEMAX_MAPPING_CHANNEL_ALPHA))
    {
        if (MaxMesh.getNumMapVerts(HEMAX_MAPPING_CHANNEL_ALPHA) == FaceCount * 3)
        {
            VertAlphaOwner = HEMAX_ATTRIBUTEOWNER_VERTEX;
            int VertCount = FaceCount * 3;
            VertexAlphaArray = new float[VertCount];

            UVVert* MapData = MaxMesh.mapVerts(HEMAX_MAPPING_CHANNEL_ALPHA);

            int CurrentIndex = 0;
            for (int f = 0; f < FaceCount; f++)
            {
                for (int v = 2; v >= 0; v--)
                {
                    float ConvertedAlpha = (0.2126f * MapData[(f * 3) + v].x) + (0.7152f * MapData[(f * 3) + v].y) + (0.0722f * MapData[(f * 3) + v].z);
                    VertexAlphaArray[CurrentIndex++] = ConvertedAlpha;
                }
            }
        }
        else if (MaxMesh.getNumMapVerts(HEMAX_MAPPING_CHANNEL_ALPHA) == PointCount)
        {
            VertAlphaOwner = HEMAX_ATTRIBUTEOWNER_POINT;
            VertexAlphaArray = new float[PointCount];

            UVVert* MapData = MaxMesh.mapVerts(HEMAX_MAPPING_CHANNEL_ALPHA);

            for (int i = 0; i < PointCount; i++)
            {
                float ConvertedAlpha = (0.2126f * MapData[i].x) + (0.7152f * MapData[i].y) + (0.0722f * MapData[i].z);
                VertexAlphaArray[i] = ConvertedAlpha;
            }
        }
    }

    // Vertex illumination
    float* IlluminationArray = nullptr;
    HEMAX_AttributeOwner IlluminationOwner = HEMAX_ATTRIBUTEOWNER_INVALID;

    if (MaxMesh.mapSupport(HEMAX_MAPPING_CHANNEL_ILLUMINATION))
    {
        if (MaxMesh.getNumMapVerts(HEMAX_MAPPING_CHANNEL_ILLUMINATION) == FaceCount * 3)
        {
            IlluminationOwner = HEMAX_ATTRIBUTEOWNER_VERTEX;
            int VertCount = FaceCount * 3;
            IlluminationArray = new float[VertCount * 3];

            UVVert* MapData = MaxMesh.mapVerts(HEMAX_MAPPING_CHANNEL_ILLUMINATION);

            int CurrentIndex = 0;
            for (int f = 0; f < FaceCount; f++)
            {
                for (int v = 2; v >= 0; v--)
                {
                    IlluminationArray[CurrentIndex] = MapData[(f * 3) + v].x;
                    IlluminationArray[CurrentIndex + 1] = MapData[(f * 3) + v].y;
                    IlluminationArray[CurrentIndex + 2] = MapData[(f * 3) + v].z;

                    CurrentIndex += 3;
                }
            }
        }
        else if (MaxMesh.getNumMapVerts(HEMAX_MAPPING_CHANNEL_ILLUMINATION) == PointCount)
        {
            IlluminationOwner = HEMAX_ATTRIBUTEOWNER_POINT;
            IlluminationArray = new float[PointCount * 3];

            UVVert* MapData = MaxMesh.mapVerts(HEMAX_MAPPING_CHANNEL_ILLUMINATION);

            for (int i = 0; i < PointCount; i++)
            {
                IlluminationArray[(i * 3)] = MapData[i].x;
                IlluminationArray[(i * 3) + 1] = MapData[i].y;
                IlluminationArray[(i * 3) + 2] = MapData[i].z;
            }
        }
    }

    if (CdArray)
    {
        if (CdOwner == HEMAX_ATTRIBUTEOWNER_VERTEX)
        {
            HEMAX_AttributeInfo CdAttributeInfo = AddNewVertexAttribute(*Node, FaceCount * 3, 3, HEMAX_COLOR_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_COLOR_ATTRIBUTE, CdAttributeInfo, CdArray, FaceCount * 3);
        }
        else if (CdOwner == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            HEMAX_AttributeInfo CdAttributeInfo = AddNewPointAttribute(*Node, PointCount, 3, HEMAX_COLOR_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_COLOR_ATTRIBUTE, CdAttributeInfo, CdArray, PointCount);
        }

        delete[] CdArray;
    }

    if (SoftSelectArr)
    {
        HEMAX_AttributeInfo SoftSelectAttributeInfo = AddNewPointAttribute(*Node, PointCount, 1, HEMAX_SOFT_SELECTION_ATTRIBUTE);
        SendFloatAttributeData(*Node, HEMAX_SOFT_SELECTION_ATTRIBUTE, SoftSelectAttributeInfo, SoftSelectArr, PointCount);

        delete[] SoftSelectArr;
    }

    if (VertexAlphaArray)
    {
        if (VertAlphaOwner == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            HEMAX_AttributeInfo VertexAlphaAttributeInfo = AddNewPointAttribute(*Node, PointCount, 1, HEMAX_ALPHA_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_ALPHA_ATTRIBUTE, VertexAlphaAttributeInfo, VertexAlphaArray, PointCount);
        }
        else if (VertAlphaOwner == HEMAX_ATTRIBUTEOWNER_VERTEX)
        {
            HEMAX_AttributeInfo VertexAlphaAttributeInfo = AddNewVertexAttribute(*Node, FaceCount * 3, 1, HEMAX_ALPHA_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_ALPHA_ATTRIBUTE, VertexAlphaAttributeInfo, VertexAlphaArray, FaceCount * 3);
        }

        delete[] VertexAlphaArray;
    }

    if (IlluminationArray)
    {
        if (IlluminationOwner == HEMAX_ATTRIBUTEOWNER_POINT)
        {
            HEMAX_AttributeInfo IlluminationAttribInfo = AddNewPointAttribute(*Node, PointCount, 3, HEMAX_ILLUMINATION_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_ILLUMINATION_ATTRIBUTE, IlluminationAttribInfo, IlluminationArray, PointCount);
        }
        else if (IlluminationOwner == HEMAX_ATTRIBUTEOWNER_VERTEX)
        {
            HEMAX_AttributeInfo IlluminationAttribInfo = AddNewVertexAttribute(*Node, FaceCount * 3, 3, HEMAX_ILLUMINATION_ATTRIBUTE);
            SendFloatAttributeData(*Node, HEMAX_ILLUMINATION_ATTRIBUTE, IlluminationAttribInfo, IlluminationArray, FaceCount * 3);
        }

        delete[] IlluminationArray;
    }

    SetParentTransform(*Node, NodeTransform);
    FinalizeInputGeometry(*Node);
    
    delete[] SmoothingGroupArray;
    delete[] PointArray;
    delete[] FaceCountArray;
    delete[] VertexArray;
}