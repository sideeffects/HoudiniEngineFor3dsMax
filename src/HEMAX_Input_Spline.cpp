#include "HEMAX_Input_Spline.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"

HEMAX_Input_Spline::HEMAX_Input_Spline(ULONG MaxNode)
    : HEMAX_Input(MaxNode)
{
    BuildInputNode();
}

HEMAX_Input_Spline::HEMAX_Input_Spline(HEMAX_InputType Type, int Id, ULONG MaxNode)
    : HEMAX_Input(Type, Id, MaxNode)
{
    BuildInputNode();
}

HEMAX_Input_Spline::HEMAX_Input_Spline(HEMAX_InputType Type,
    LinearShape* TheShape, ULONG MaxNode)
    : HEMAX_Input(Type, -1, MaxNode)
{
    if (TheShape)
    {
	BuildLinearCurveForInputNode(Node, TheShape, "modifier_input");
    }
}

HEMAX_Input_Spline::HEMAX_Input_Spline(HEMAX_Node* EditableNode, ULONG MaxNode)
    : HEMAX_Input(EditableNode, MaxNode)
{
    BuildInputNode();
    Node = nullptr;
}

HEMAX_Input_Spline::~HEMAX_Input_Spline()
{
    if (Node)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

	if (SM.IsSessionActive())
	{
	    Node->Delete();
	}
    }
}

void
HEMAX_Input_Spline::RebuildAfterChange()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    Node->Delete();
    BuildInputNode();
}

void
HEMAX_Input_Spline::BuildInputNode()
{
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    if (MaxInputNode)
    {
	ObjectState MaxObjectState = MaxInputNode->EvalWorldState(0);
	Object* MaxObject = MaxObjectState.obj;

	if (MaxObject->CanConvertToType(Class_ID(LINEARSHAPE_CLASS_ID, 0)))
	{
	    LinearShape* TheShape = (LinearShape*)MaxObject->ConvertToType(0,
                Class_ID(LINEARSHAPE_CLASS_ID, 0));

	    if (InputNodeType == HEMAX_INPUT_EDITABLENODE)
	    {
		BuildLinearCurveForEditableNode(Node, TheShape,
                    GetInputNodeName());
	    }
	    else
	    {
		BuildLinearCurveForInputNode(Node, TheShape,
                    GetInputNodeName());
	    }

	    if (MaxObject != TheShape)
	    {
		delete TheShape;
	    }

	    INode* InputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

	    if (InputNode)
	    {
		HAPI_Transform HAPITransform =
                    HEMAX_Utilities::MaxTransformToHAPITransform(
                            HEMAX_Utilities::BuildMaxTransformFromINode(
                                InputNode));

		HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

		SM.Session->SetObjectTransform(Node->Info.parentId,
                    &HEMAX_Utilities::HAPITransformToHAPITransformEuler(
                        HAPITransform));
	    }
	}
    }
}

void
HEMAX_Input_Spline::BuildLinearCurveForInputNode(HEMAX_Node* Node,
    LinearShape* TheShape, std::string InputNodeName)
{
    CreateInputNode(InputNodeName + "_" +
                    std::to_string(rand()) +
                    std::to_string(rand()));

    MarshalNodeNameDetailAttribute();

#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022)
    int CurveCount = TheShape->NumberOfCurves(GetCOREInterface()->GetTime());
#endif
#ifdef HEMAX_VERSION_2017
    int CurveCount = TheShape->NumberOfCurves();
#endif

    if (CurveCount > 0)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

        if (HEMAX_Utilities::IsOnlyOpenSplines(TheShape))
        {
            std::vector<int> CountsArray(CurveCount);
            std::vector<int> OrdersArray(CurveCount, 0);
            std::vector<float> PointsArray;
            std::vector<int> MatIdArray;

            float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

            for (int c = 0; c < CurveCount; c++)
            {
                PolyLine& Line = TheShape->shape.lines[c];

                CountsArray[c] = Line.Verts();
                PolyPt* LinePoints = Line.pts;

                for (int p = 0; p < Line.Verts(); p++)
                {
                    PointsArray.push_back(LinePoints[p].p.x * ScaleConversion);
                    PointsArray.push_back(LinePoints[p].p.z * ScaleConversion);
                    PointsArray.push_back(-LinePoints[p].p.y * ScaleConversion);

                    MatIdArray.push_back(LinePoints[p].GetMatID());
                }
            }

            int PointCount = (int)PointsArray.size()/3;

            AddNewPart(HEMAX_PARTTYPE_CURVE, 1, PointCount, PointCount);

            HEMAX_CurveInfo CurveInfo;
            CurveInfo.curveCount = CurveCount;
            CurveInfo.curveType = HAPI_CURVETYPE_LINEAR;
            CurveInfo.hasKnots = false;
            CurveInfo.isPeriodic = false;
            CurveInfo.knotCount = 0;
            CurveInfo.order = 0;
            CurveInfo.vertexCount = PointCount;

            SM.Session->SetCurveInfo(Node->Info.id, 0, &CurveInfo);
            SM.Session->SetCurveCounts(Node->Info.id, 0, &CountsArray.front(),
                0, (int)CountsArray.size());
            SM.Session->SetCurveOrders(Node->Info.id, 0, &OrdersArray.front(),
                0, (int)OrdersArray.size());

            HEMAX_AttributeInfo PointAttributeInfo = AddNewPointAttribute(
                PointCount, 3, HEMAX_POSITION_ATTRIBUTE);
            SendFloatAttributeData(
                HEMAX_POSITION_ATTRIBUTE, PointAttributeInfo,
                &PointsArray.front(), PointCount);

            HEMAX_AttributeInfo MatIdAttributeInfo = AddNewPointAttribute(
                PointCount, 1, HEMAX_MATERIAL_ID_ATTRIBUTE,
                HAPI_STORAGETYPE_INT);
            SendIntAttributeData(HEMAX_MATERIAL_ID_ATTRIBUTE,
                MatIdAttributeInfo, MatIdArray.data(), PointCount);

	    INode* InputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);
            if (InputNode)
                AddNodeTransformAttributes(InputNode);

            FinalizeInputGeometry();
        }
        else if (HEMAX_Utilities::IsOnlyClosedSplines(TheShape))
        {
            float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

            std::vector<int> CountsArray(CurveCount);
            std::vector<float> PointsArray;
            std::vector<int> VerticesArray;
            std::vector<int> FaceCountArray;
            std::vector<int> MatIdArray;
            
            for (int c = 0; c < CurveCount; c++)
            {
                PolyLine& Line = TheShape->shape.lines[c];
                CountsArray[c] = Line.Verts();
                PolyPt* LinePoints = Line.pts;
                for (int p = 0; p < Line.Verts(); p++)
                {
                    PointsArray.push_back(LinePoints[p].p.x * ScaleConversion);
                    PointsArray.push_back(LinePoints[p].p.z * ScaleConversion);
                    PointsArray.push_back(-LinePoints[p].p.y * ScaleConversion);

                    VerticesArray.push_back((int)VerticesArray.size());

                    MatIdArray.push_back(LinePoints[p].GetMatID());
                }

                FaceCountArray.push_back(Line.Verts());
            }

            int PointCount = (int)PointsArray.size()/3;

            AddNewPart(HEMAX_PARTTYPE_MESH, CurveCount,
                (int)VerticesArray.size(), PointCount);

            HEMAX_AttributeInfo PointAttrInfo = AddNewPointAttribute(PointCount,
                3, HEMAX_POSITION_ATTRIBUTE);
            SendPointAttributeData(PointAttrInfo, PointsArray.data(),
                VerticesArray.data(), FaceCountArray.data(), CurveCount,
                (int)VerticesArray.size(), PointCount,
                HEMAX_POSITION_ATTRIBUTE);

            HEMAX_AttributeInfo MatIdAttrInfo = AddNewPointAttribute(PointCount,
                1, HEMAX_MATERIAL_ID_ATTRIBUTE, HAPI_STORAGETYPE_INT);
            SendIntAttributeData(HEMAX_MATERIAL_ID_ATTRIBUTE,
                MatIdAttrInfo, MatIdArray.data(), PointCount);

            INode* InputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);
            if (InputNode)
                AddNodeTransformAttributes(InputNode);

            FinalizeInputGeometry();
        }
    }
    else
    {
	HEMAX_Logger::Instance().AddEntry("Invalid input: provided shape has no curves",
					  HEMAX_LOG_LEVEL_WARN);
    }
}

void
HEMAX_Input_Spline::BuildLinearCurveForEditableNode(HEMAX_Node* Node,
    LinearShape* TheShape, std::string InputNodeName)
{
#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022)
    int CurveCount = TheShape->NumberOfCurves(HEMAX_FRAME_ZERO);
#endif
#ifdef HEMAX_VERSION_2017
    int CurveCount = TheShape->NumberOfCurves();
#endif

    if (CurveCount > 1)
    {
	HEMAX_Logger::Instance().AddEntry("HEMAX_Input: user chose a shape "
            "with more than 1 curve as a HAPI input. Only the first curve will "
            "be used.", HEMAX_LOG_LEVEL_INFO);
    }

    if (CurveCount > 0)
    {
	int PointCount = TheShape->NumberOfVertices(HEMAX_FRAME_ZERO, 0);

	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

	HEMAX_GeometryInfo CurveGeoInfo;
	SM.Session->GetGeometryInfo(Node->Info.id, &CurveGeoInfo);

	HEMAX_PartInfo CurvePartInfo;
	SM.Session->GetPartInfo(CurveGeoInfo.nodeId, 0, &CurvePartInfo);

	HEMAX_CurveInfo CurveInfo;
	CurveInfo.curveCount = 1;
	CurveInfo.curveType = HAPI_CURVETYPE_LINEAR;
	CurveInfo.hasKnots = false;
	CurveInfo.isPeriodic = false;
	CurveInfo.knotCount = 0;
	CurveInfo.order = 0;
	CurveInfo.vertexCount = PointCount;

	SM.Session->SetCurveInfo(CurveGeoInfo.nodeId, 0, &CurveInfo);

	int CountsArray[] = { PointCount };
	SM.Session->SetCurveCounts(CurveGeoInfo.nodeId, 0, CountsArray, 0, 1);

	int OrdersArray[] = { 0 };
	SM.Session->SetCurveOrders(CurveGeoInfo.nodeId, 0, OrdersArray, 0, 1);

	float* Points = new float[PointCount * 3];

	float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

	PolyShape LinearLines = TheShape->shape;
	PolyLine* Lines = LinearLines.lines;
	PolyPt* ThePoints = Lines->pts;

	for (int p = 0; p < Lines->numPts; ++p)
	{
	    Points[p * 3] = ThePoints[p].p.x * ScaleConversion;
	    Points[p * 3 + 1] = ThePoints[p].p.z * ScaleConversion;
	    Points[p * 3 + 2] = -ThePoints[p].p.y * ScaleConversion;
	}

	std::string PointString;
	HEMAX_Utilities::ConstructPointString(Points, PointCount, PointString);

	HEMAX_Parameter* CoordParam = Node->GetParameter("coords");

	if (CoordParam && CoordParam->Type != HEMAX_PARAM_INVALID)
	{
	    std::vector<std::string> StringValues = { PointString };
	    CoordParam->UpdateStringVals(StringValues);
	}

	delete[] Points;
    }
}
