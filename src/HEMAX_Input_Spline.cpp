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

HEMAX_Input_Spline::HEMAX_Input_Spline(HEMAX_InputType Type, LinearShape* TheShape)
    : HEMAX_Input(Type, -1, -1)
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
            DeleteNode(*Node);
        }
    }
}

void
HEMAX_Input_Spline::RebuildAfterChange()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    DeleteNode(*Node);
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
            LinearShape* TheShape = (LinearShape*)MaxObject->ConvertToType(0, Class_ID(LINEARSHAPE_CLASS_ID, 0));

            if (InputNodeType == HEMAX_INPUT_EDITABLENODE)
            {
                BuildLinearCurveForEditableNode(Node, TheShape, GetInputNodeName());
            }
            else
            {
                BuildLinearCurveForInputNode(Node, TheShape, GetInputNodeName());
            }

            if (MaxObject != TheShape)
            {
                delete TheShape;
            }

            INode* InputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

            if (InputNode)
            {
                HAPI_Transform HAPITransform = HEMAX_Utilities::MaxTransformToHAPITransform(HEMAX_Utilities::BuildMaxTransformFromINode(InputNode));

                HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

                SM.Session->SetObjectTransform(Node->Info.parentId, &HEMAX_Utilities::HAPITransformToHAPITransformEuler(HAPITransform));
            }
        }
    }
}

void
HEMAX_Input_Spline::BuildLinearCurveForInputNode(HEMAX_Node* Node, LinearShape* TheShape, std::string InputNodeName)
{
    CreateInputNode(*Node, InputNodeName + "_" + std::to_string(rand()) + std::to_string(rand()));

#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
    int CurveCount = TheShape->NumberOfCurves(GetCOREInterface()->GetTime());
#endif
#ifdef HEMAX_VERSION_2017
    int CurveCount = TheShape->NumberOfCurves();
#endif

    if (CurveCount > 1)
    {
        HEMAX_Logger::Instance().AddEntry("HEMAX_Input: user chose a shape with more than 1 curve as a HAPI input. Only the first curve will be used.", HEMAX_LOG_LEVEL_INFO);
    }

    if (CurveCount > 0)
    {
        int PointCount = TheShape->NumberOfVertices(GetCOREInterface()->GetTime(), 0);

        AddNewPart(*Node, HEMAX_PARTTYPE_CURVE, 1, PointCount, PointCount);

        HEMAX_CurveInfo CurveInfo;
        CurveInfo.curveCount = 1;
        CurveInfo.curveType = HAPI_CURVETYPE_LINEAR;
        CurveInfo.hasKnots = false;
        CurveInfo.isPeriodic = false;
        CurveInfo.knotCount = 0;
        CurveInfo.order = 0;
        CurveInfo.vertexCount = PointCount;

        HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

        SM.Session->SetCurveInfo(Node->Info.id, 0, &CurveInfo);

        int CountsArray[] = { PointCount };
        SM.Session->SetCurveCounts(Node->Info.id, 0, CountsArray, 0, 1);

        int OrdersArray[] = { 0 };
        SM.Session->SetCurveOrders(Node->Info.id, 0, OrdersArray, 0, 1);

        float* Points = new float[PointCount * 3];

        float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

        PolyShape LinearLines = TheShape->shape;
        PolyLine* Lines = LinearLines.lines;
        PolyPt* ThePoints = Lines->pts;

        int p;

        for (p = 0; p < Lines->numPts; ++p)
        {
            Points[p * 3] = ThePoints[p].p.x * ScaleConversion;
            Points[p * 3 + 1] = ThePoints[p].p.z * ScaleConversion;
            Points[p * 3 + 2] = -ThePoints[p].p.y * ScaleConversion;
        }

        HEMAX_AttributeInfo PointAttributeInfo = AddNewPointAttribute(*Node, PointCount, 3, HEMAX_POSITION_ATTRIBUTE);
        SendFloatAttributeData(*Node, HEMAX_POSITION_ATTRIBUTE, PointAttributeInfo, Points, PointCount);
        FinalizeInputGeometry(*Node);

        delete[] Points;
    }
}

void
HEMAX_Input_Spline::BuildLinearCurveForEditableNode(HEMAX_Node* Node, LinearShape* TheShape, std::string InputNodeName)
{
#if defined(HEMAX_VERSION_2018) || defined(HEMAX_VERSION_2019)
    int CurveCount = TheShape->NumberOfCurves(HEMAX_FRAME_ZERO);
#endif
#ifdef HEMAX_VERSION_2017
    int CurveCount = TheShape->NumberOfCurves();
#endif

    if (CurveCount > 1)
    {
        HEMAX_Logger::Instance().AddEntry("HEMAX_Input: user chose a shape with more than 1 curve as a HAPI input. Only the first curve will be used.", HEMAX_LOG_LEVEL_INFO);
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

        HEMAX_Parameter CoordParam = GetParameter(*Node, "coords");

        if (CoordParam.Type != HEMAX_PARAM_INVALID)
        {
            std::vector<std::string> StringValues = { PointString };
            UpdateParameterStringValues(CoordParam, StringValues);
        }

        delete[] Points;
    }
}