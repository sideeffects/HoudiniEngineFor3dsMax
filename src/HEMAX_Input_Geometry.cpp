#include "HEMAX_Input_Geometry.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"

#include "triobj.h"
#include "polyobj.h"
#include "MeshNormalSpec.h"
#include "MNNormalSpec.h"

#ifdef HEMAX_VERSION_2023
#include "geom/VertexNormal.h"
#else
#include "VertexNormal.h"
#endif

#include "stdmat.h"

HEMAX_Input_Geometry::HEMAX_Input_Geometry(ULONG MaxNode)
    : HEMAX_Input(MaxNode)
{
    BuildInputNode();
}

HEMAX_Input_Geometry::HEMAX_Input_Geometry(HEMAX_InputType Type,
                                           int Id, ULONG MaxNode)
    : HEMAX_Input(Type, Id, MaxNode)
{
    BuildInputNode();
}

HEMAX_Input_Geometry::HEMAX_Input_Geometry(HEMAX_InputType Type,
                                           PolyObject* MaxPolyObject,
                                           ULONG MaxNode)
    : HEMAX_Input(Type, -1, MaxNode)
{
    if (MaxPolyObject)
    {
        BuildPolyGeometryForInputNode(Node, MaxPolyObject->GetMesh(),
            "modifier_input", HEMAX_Utilities::GetIdentityTransform(),
            GetCOREInterface()->GetINodeByHandle(MaxNode));
    }
}

HEMAX_Input_Geometry::~HEMAX_Input_Geometry()
{
    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();

    if (SessionManager.IsSessionActive())
    {
	HAPI_NodeId ParentNodeId = Node->Info.parentId;
	Node->Delete();
        HEMAX_HoudiniApi::DeleteNode(SessionManager.Session, ParentNodeId);
    }
}

void
HEMAX_Input_Geometry::RebuildAfterChange()
{
    HEMAX_SessionManager& SessionManager =
        HEMAX_SessionManager::GetSessionManager();

    HAPI_NodeId ParentNodeId = Node->Info.parentId;
    Node->Delete();
    HEMAX_HoudiniApi::DeleteNode(SessionManager.Session, ParentNodeId);
    BuildInputNode();
}

void
HEMAX_Input_Geometry::BuildInputNode()
{
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    if (MaxInputNode)
    {
	ObjectState MaxObjectState =
            MaxInputNode->EvalWorldState(GetCOREInterface()->GetTime());
	Object* MaxObject = MaxObjectState.obj;

	if (MaxObject->CanConvertToType(Class_ID(POLYOBJ_CLASS_ID, 0)))
	{
	    PolyObject* MaxPolyObject = (PolyObject*)MaxObject->ConvertToType(
                GetCOREInterface()->GetTime(), Class_ID(POLYOBJ_CLASS_ID, 0));
	    INode* InputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

	    BuildPolyGeometryForInputNode(Node, MaxPolyObject->GetMesh(),
                    GetInputNodeName(),
                    HEMAX_Utilities::BuildMaxTransformFromINode(InputNode),
                    MaxInputNode);

	    if (MaxObject != MaxPolyObject)
	    {
		delete MaxPolyObject;
	    }
	}
	else
	{
	    std::string Msg = "Cannot build a geometry input node from the supplied object";
	    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
	}
    }
}

void
HEMAX_Input_Geometry::BuildPolyGeometryForInputNode(HEMAX_Node* Node,
    MNMesh& MaxMesh, std::string InputNodeName,
    HEMAX_MaxTransform NodeTransform, INode* MaxNode)
{
    CreateInputNode(InputNodeName + "_" + std::to_string(rand()) + std::to_string(rand()));

    MaxMesh.CollapseDeadStructs();
    MaxMesh.buildNormals();

    // Look up material (if it exists)
    INode* SourceNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    MarshalNodeNameDetailAttribute();

    bool SingleMaterial = false;
    const char* SingleMaterialName;
    std::vector<const char*> SingleMaterialNameData;

    std::unordered_map<int, const char*> SubMatNames;
    std::vector<const char*> FaceMaterialNames;
    std::string MultiMaterialName;

    if (SourceNode)
    {
	Mtl* MeshMat = SourceNode->GetMtl();

	if (MeshMat)
	{
	    // We have a multi-material
	    if (MeshMat->NumSubMtls() > 0)
	    {
                MultiMaterialName = MeshMat->GetName().ToCStr();
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
		SingleMaterialNameData = std::vector<const char*>(MaxMesh.FNum(),
                                            SingleMaterialName);
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
    std::vector<int> FaceSelections;
    std::vector<int> VertexSelections;
    std::vector<int> EdgeSelections;

    float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

    BitArray VertexSelectionsArray;
    MaxMesh.getVertexSel(VertexSelectionsArray);
    bool HasVertexSelections = (!VertexSelectionsArray.IsEmpty() &&
        VertexSelectionsArray.GetSize() == VertCount);

    for (int i = 0; i < VertCount; i++)
    {
        PointArray[(i * 3)] = MaxMesh.V(i)->p.x * ScaleConversion;
        PointArray[(i * 3) + 1] = MaxMesh.V(i)->p.z * ScaleConversion;
        PointArray[(i * 3) + 2] = -MaxMesh.V(i)->p.y * ScaleConversion;

        if (HasVertexSelections)
        {
            VertexSelections.push_back(VertexSelectionsArray[i]); 
        }
    }

    int VertIndexCount = 0;

    for (int i = 0; i < FaceCount; i++)
    {
	FaceCountArray[i] = MaxMesh.F(i)->deg;
	VertIndexCount += MaxMesh.F(i)->deg;
    }

    int* VertIndexArray = new int[VertIndexCount];
    int CurIndex = 0;

    BitArray FaceSelectionsArray;
    MaxMesh.getFaceSel(FaceSelectionsArray);
    bool HasFaceSelections = (!FaceSelectionsArray.IsEmpty() &&
        FaceSelectionsArray.GetSize() == FaceCount);

    BitArray EdgeSelectionsArray;
    MaxMesh.getEdgeSel(EdgeSelectionsArray);
    bool HasEdgeSelections = !EdgeSelectionsArray.IsEmpty();

    for (int i = 0; i < FaceCount; i++)
    {
	SmoothingGroupArray[i] = MaxMesh.F(i)->smGroup;
	MaterialIDArray[i] = MaxMesh.F(i)->material;

        if (HasFaceSelections)
        {
            FaceSelections.push_back(FaceSelectionsArray[i]);
        }

	if (!SingleMaterial)
	{
	    auto MatName = SubMatNames.find(MaxMesh.F(i)->material);

	    if (MatName != SubMatNames.end())
	    {
		FaceMaterialNames[i] = MatName->second;
	    }
	    else if (FaceMaterialNames.size() > 0)
	    {
		FaceMaterialNames[i] = "";
	    }
	}

	for (int v = (MaxMesh.F(i)->deg - 1), c = 0; v >= 0; v--, c++)
	{
	    VertIndexArray[CurIndex + c] = MaxMesh.F(i)->vtx[v];

            if (HasEdgeSelections)
            {
                int EdgeNum = MaxMesh.F(i)->edg[v];
                if (EdgeSelectionsArray[EdgeNum])
                {
                    MNEdge* Edge = MaxMesh.E(EdgeNum);
                    EdgeSelections.push_back(Edge->v1);
                    EdgeSelections.push_back(Edge->v2);

                    // Prevent adding the same edge multiple times.
                    EdgeSelectionsArray.Clear(EdgeNum);
                }
            }
	}

	CurIndex += MaxMesh.F(i)->deg;
    }

    AddNewPart(HAPI_PARTTYPE_MESH, FaceCount, VertIndexCount, VertCount);

    HAPI_AttributeInfo PointAttributeInfo = AddNewPointAttribute(VertCount, 3,
                                                HEMAX_POSITION_ATTRIBUTE);
    SendPointAttributeData(PointAttributeInfo, PointArray, VertIndexArray,
            FaceCountArray, FaceCount, VertIndexCount, VertCount,
            HEMAX_POSITION_ATTRIBUTE);

    HAPI_AttributeInfo SmoothingGroupAttributeInfo =
        AddNewPrimitiveIntAttribute(FaceCount, 1,
                HEMAX_SMOOTHING_GROUP_ATTRIBUTE);
    SendIntAttributeData(HEMAX_SMOOTHING_GROUP_ATTRIBUTE,
            SmoothingGroupAttributeInfo, SmoothingGroupArray, FaceCount);

    if (HasFaceSelections)
    {
        HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
        HEMAX_HoudiniApi::AddGroup(SM.Session, Node->Info.id, 0,
            HAPI_GROUPTYPE_PRIM, HEMAX_SELECTION_FACE);
        HEMAX_HoudiniApi::SetGroupMembership(SM.Session, Node->Info.id, 0,
            HAPI_GROUPTYPE_PRIM, HEMAX_SELECTION_FACE, FaceSelections.data(), 0,
            FaceCount);
    }

    if (HasVertexSelections)
    {
        HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
        HEMAX_HoudiniApi::AddGroup(SM.Session, Node->Info.id, 0,
            HAPI_GROUPTYPE_POINT, HEMAX_SELECTION_VERTEX);
        HEMAX_HoudiniApi::SetGroupMembership(SM.Session, Node->Info.id, 0,
            HAPI_GROUPTYPE_POINT, HEMAX_SELECTION_VERTEX,
            VertexSelections.data(), 0, VertCount);
    }

    if (HasEdgeSelections)
    {
        HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
        HEMAX_HoudiniApi::AddGroup(SM.Session, Node->Info.id, 0,
            HAPI_GROUPTYPE_EDGE, HEMAX_SELECTION_EDGE);
        HEMAX_HoudiniApi::SetGroupMembership(SM.Session, Node->Info.id, 0,
            HAPI_GROUPTYPE_EDGE, HEMAX_SELECTION_EDGE, EdgeSelections.data(), 0,
            static_cast<int>(EdgeSelections.size()));
    }

    HAPI_AttributeInfo MaterialIDAttributeInfo = AddNewPrimitiveIntAttribute(
            FaceCount, 1, HEMAX_MATERIAL_ID_ATTRIBUTE);
    SendIntAttributeData(HEMAX_MATERIAL_ID_ATTRIBUTE, MaterialIDAttributeInfo,
            MaterialIDArray, FaceCount);

    if (SingleMaterial && SingleMaterialNameData.size() > 0)
    {
	HAPI_AttributeInfo MaterialNamesAttrInfo =
            AddNewPrimitiveStringAttribute(FaceCount, 1,
                    HEMAX_MATERIAL_PATH_ATTRIBUTE);
	SendStringAttributeData(HEMAX_MATERIAL_PATH_ATTRIBUTE,
                MaterialNamesAttrInfo, SingleMaterialNameData.data(),
                FaceCount);
    }
    else if (FaceMaterialNames.size() > 0)
    {
	HAPI_AttributeInfo MaterialNamesAttrInfo =
            AddNewPrimitiveStringAttribute(FaceCount, 1,
                HEMAX_MATERIAL_PATH_ATTRIBUTE);
	SendStringAttributeData(HEMAX_MATERIAL_PATH_ATTRIBUTE,
                MaterialNamesAttrInfo, FaceMaterialNames.data(), FaceCount);

        if (!MultiMaterialName.empty())
        {
            HAPI_AttributeInfo MultiMatNameAttrInfo =
                AddNewDetailStringAttribute(1, 1,
                        HEMAX_MATERIAL_PATH_ATTRIBUTE);
            const char* MultiMatNameCStr = MultiMaterialName.c_str();
            SendStringAttributeData(HEMAX_MATERIAL_PATH_ATTRIBUTE,
                MultiMatNameAttrInfo, &MultiMatNameCStr, 1);
        }
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
		VertexNormals[MaxMesh.F(i)->vtx[j]]->AddNormal(FaceNormals[i],
                                                        MaxMesh.F(i)->smGroup);

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
		    NormalizedNormal = Normalize(VertexNormals[Vert]->GetNormal(
                                            MaxMesh.F(i)->smGroup));
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
		Point3 NormalizedNormal = Normalize(SpecifiedNormals->GetNormal(
                                                                        f, v));

		NormalArray.push_back(NormalizedNormal.x);
		NormalArray.push_back(NormalizedNormal.z);
		NormalArray.push_back(-NormalizedNormal.y);
	    }
	}
    }

    HAPI_AttributeInfo NormalAttributeInfo =
        AddNewVertexAttribute(VertIndexCount, 3, HEMAX_NORMAL_ATTRIBUTE);
    SendFloatAttributeData(HEMAX_NORMAL_ATTRIBUTE, NormalAttributeInfo,
            &NormalArray.front(), VertIndexCount);

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

		std::string UVAttrName = (texMap == 1 ?
                                          HEMAX_UV_ATTRIBUTE :
                                          HEMAX_UV_ATTRIBUTE + std::to_string(texMap));

		HAPI_AttributeInfo UVAttributeInfo =
                    AddNewVertexAttribute(VertIndexCount, 3, UVAttrName);
		SendFloatAttributeData(UVAttrName, UVAttributeInfo,
                    UVArray.data(), VertIndexCount);
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
    HAPI_AttributeOwner CdOwner = HAPI_ATTROWNER_INVALID;

    MNMap* ColorMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_COLOR);
    if (ColorMap && ColorMap->numv > 0)
    {
	if (ColorMap->numv == VertCount)
	{
	    CdOwner = HAPI_ATTROWNER_POINT;
	    CdArray = new float[VertCount * 3];

	    UVVert* MapData = ColorMap->v;

	    for (int c = 0; c < VertCount; c++)
	    {
		CdArray[(c * 3)] = MapData[c].x;
		CdArray[(c * 3) + 1] = MapData[c].y;
		CdArray[(c * 3) + 2] = MapData[c].z;
	    }
	}
        else if (ColorMap->numv <= VertIndexCount)
	{
	    CdOwner = HAPI_ATTROWNER_VERTEX;
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
    }

    float* AlphaArray = nullptr;
    HAPI_AttributeOwner AlphaOwner = HAPI_ATTROWNER_INVALID;

    MNMap* AlphaMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_ALPHA);
    if (AlphaMap && AlphaMap->numv > 0)
    {
	if (AlphaMap->numv == VertCount)
	{
	    AlphaOwner = HAPI_ATTROWNER_POINT;
	    AlphaArray = new float[VertCount];

	    UVVert* MapData = AlphaMap->v;

	    for (int c = 0; c < VertCount; c++)
	    {
		float ConvertedAlpha = (0.2126f * MapData[c].x) +
                                       (0.7152f * MapData[c].y) +
                                       (0.0722f * MapData[c].z);
		AlphaArray[c] = ConvertedAlpha;
	    }
	}
        else if (AlphaMap->numv <= VertIndexCount)
	{
	    AlphaOwner = HAPI_ATTROWNER_VERTEX;
	    AlphaArray = new float[VertIndexCount];

	    UVVert* MapData = AlphaMap->v;

	    int AlphaVertIndex = 0;

	    for (int f = 0; f < FaceCount; f++)
	    {
		for (int v = MaxMesh.F(f)->deg - 1; v >= 0; v--)
		{
		    float ConvertedAlpha =
                        (0.2126f * MapData[AlphaMap->F(f)->tv[v]].x) +
                        (0.7152f * MapData[AlphaMap->F(f)->tv[v]].y) +
                        (0.0722f * MapData[AlphaMap->F(f)->tv[v]].z);

		    AlphaArray[AlphaVertIndex] = ConvertedAlpha;

		    AlphaVertIndex++;
		}
	    }
	}
    }

    float* IlluminationArray = nullptr;
    HAPI_AttributeOwner IlluminationOwner = HAPI_ATTROWNER_INVALID;

    MNMap* IlluminationMap = MaxMesh.M(HEMAX_MAPPING_CHANNEL_ILLUMINATION);
    if (IlluminationMap && IlluminationMap->numv > 0)
    {
	if (IlluminationMap->numv == VertCount)
	{
	    IlluminationOwner = HAPI_ATTROWNER_POINT;
	    IlluminationArray = new float[VertCount * 3];

	    UVVert* MapData = IlluminationMap->v;

	    for (int c = 0; c < VertCount; c++)
	    {
		IlluminationArray[(c * 3)] = MapData[c].x;
		IlluminationArray[(c * 3) + 1] = MapData[c].y;
		IlluminationArray[(c * 3) + 2] = MapData[c].z;
	    }
	}
        else if (IlluminationMap->numv <= VertIndexCount)
	{
	    IlluminationOwner = HAPI_ATTROWNER_VERTEX;
	    IlluminationArray = new float[VertIndexCount * 3];

	    UVVert* MapData = IlluminationMap->v;

	    int CurrentIndex = 0;

	    for (int f = 0; f < FaceCount; f++)
	    {
		for (int v = MaxMesh.F(f)->deg - 1; v >= 0; v--)
		{
		    IlluminationArray[CurrentIndex] =
                        MapData[IlluminationMap->F(f)->tv[v]].x;
		    IlluminationArray[CurrentIndex + 1] =
                        MapData[IlluminationMap->F(f)->tv[v]].y;
		    IlluminationArray[CurrentIndex + 2] =
                        MapData[IlluminationMap->F(f)->tv[v]].z;

		    CurrentIndex += 3;
		}
	    }
	}
    }

    if (IlluminationArray)
    {
	if (IlluminationOwner == HAPI_ATTROWNER_VERTEX)
	{
	    HAPI_AttributeInfo IlluminationAttributeInfo =
                AddNewVertexAttribute(VertIndexCount, 3,
                        HEMAX_ILLUMINATION_ATTRIBUTE);
	    SendFloatAttributeData(HEMAX_ILLUMINATION_ATTRIBUTE,
                    IlluminationAttributeInfo, IlluminationArray,
                    VertIndexCount);
	}
	else if (IlluminationOwner == HAPI_ATTROWNER_POINT)
	{
	    HAPI_AttributeInfo IlluminationAttributeInfo =
                AddNewPointAttribute(VertCount, 3,
                        HEMAX_ILLUMINATION_ATTRIBUTE);
	    SendFloatAttributeData(HEMAX_ILLUMINATION_ATTRIBUTE,
                    IlluminationAttributeInfo, IlluminationArray, VertCount);
	}

	delete[] IlluminationArray;
    }

    if (AlphaArray)
    {
	if (AlphaOwner == HAPI_ATTROWNER_VERTEX)
	{
	    HAPI_AttributeInfo AlphaAttributeInfo =
                AddNewVertexAttribute(VertIndexCount, 1, HEMAX_ALPHA_ATTRIBUTE);
	    SendFloatAttributeData(HEMAX_ALPHA_ATTRIBUTE, AlphaAttributeInfo,
                    AlphaArray, VertIndexCount);
	}
	else if (AlphaOwner == HAPI_ATTROWNER_POINT)
	{
	    HAPI_AttributeInfo AlphaAttributeInfo =
                AddNewPointAttribute(VertCount, 1, HEMAX_ALPHA_ATTRIBUTE);
	    SendFloatAttributeData(HEMAX_ALPHA_ATTRIBUTE, AlphaAttributeInfo,
                    AlphaArray, VertCount);
	}

	delete[] AlphaArray;
    }

    if (CdArray)
    {
	if (CdOwner == HAPI_ATTROWNER_VERTEX)
	{
	    HAPI_AttributeInfo CdAttributeInfo = AddNewVertexAttribute(
                    VertIndexCount, 3, HEMAX_COLOR_ATTRIBUTE);
	    SendFloatAttributeData(HEMAX_COLOR_ATTRIBUTE, CdAttributeInfo,
                    CdArray, VertIndexCount);
	}
	else if (CdOwner == HAPI_ATTROWNER_POINT)
	{
	    HAPI_AttributeInfo CdAttributeInfo = AddNewPointAttribute(
                    VertCount, 3, HEMAX_COLOR_ATTRIBUTE);
	    SendFloatAttributeData(HEMAX_COLOR_ATTRIBUTE, CdAttributeInfo,
                    CdArray, VertCount);
	}

	delete[] CdArray;
    }

    if (SoftSelectionArray)
    {
	HAPI_AttributeInfo SoftSelectAttributeInfo = AddNewPointAttribute(
                VertCount, 1, HEMAX_SOFT_SELECTION_ATTRIBUTE);
	SendFloatAttributeData(HEMAX_SOFT_SELECTION_ATTRIBUTE,
                SoftSelectAttributeInfo, SoftSelectionArray, VertCount);

	delete[] SoftSelectionArray;
    }

    // Transform Detail Attributes

    if (MaxNode)
    {
        AddNodeTransformAttributes(MaxNode);
    }

    Node->SetParentTransform(NodeTransform);
    FinalizeInputGeometry();

    delete[] VertIndexArray;
    delete[] MaterialIDArray;
    delete[] SmoothingGroupArray;
    delete[] FaceCountArray;
    delete[] PointArray;
}
