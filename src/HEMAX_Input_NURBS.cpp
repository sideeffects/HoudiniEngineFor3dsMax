#include "HEMAX_Input_NURBS.h"
#include "HEMAX_SessionManager.h"

HEMAX_Input_NURBS::HEMAX_Input_NURBS(ULONG MaxNode)
    : HEMAX_Input(MaxNode)
{
    BuildInputNode();
}

HEMAX_Input_NURBS::HEMAX_Input_NURBS(HEMAX_InputType Type, int Id, ULONG MaxNode)
    : HEMAX_Input(Type, Id, MaxNode)
{
    BuildInputNode();
}

HEMAX_Input_NURBS::HEMAX_Input_NURBS(HEMAX_Node* EditableNode, ULONG MaxNode)
    : HEMAX_Input(EditableNode, MaxNode)
{
    BuildInputNode();
    Node = nullptr;
}

HEMAX_Input_NURBS::HEMAX_Input_NURBS(HEMAX_InputType Type, Object* MaxObject)
    : HEMAX_Input(Type, -1, -1)
{
    if (MaxObject)
    {
        NURBSSet CurveSet;
        BOOL Success = GetNURBSSet(MaxObject, GetCOREInterface()->GetTime(), CurveSet, TRUE);

        if (Success)
        {
            NURBSObject* MaxCurve = CurveSet.GetNURBSObject(0);
            if (MaxCurve)
            {
                if (MaxCurve->GetKind() == kNURBSCurve && CurveSet.GetNumObjects() == 1)
                {
                    BuildCurveForInputNode(Node, MaxCurve, "modifier_input");
                }
                else if (MaxCurve->GetKind() == kNURBSPoint && CurveSet.GetNumObjects() >= 1)
                {
                    std::vector<NURBSPoint*> Points;
                    for (int n = 0; n < CurveSet.GetNumObjects(); ++n)
                    {
                        NURBSObject* AnObject = CurveSet.GetNURBSObject(n);
                        if (AnObject->GetKind() == kNURBSPoint)
                        {
                            Points.push_back((NURBSPoint*)AnObject);
                        }
                    }
                    BuildPointCurveForInputNode(Node, Points, "modifier_input");
                }
            }
        }
    }
}

HEMAX_Input_NURBS::~HEMAX_Input_NURBS()
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
HEMAX_Input_NURBS::RebuildAfterChange()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    DeleteNode(*Node);
    BuildInputNode();
}

void
HEMAX_Input_NURBS::BuildInputNode()
{
    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    if (MaxInputNode)
    {
        ObjectState MaxObjectState = MaxInputNode->EvalWorldState(0);
        Object* MaxObject = MaxObjectState.obj;

        if (MaxObject->CanConvertToType(EDITABLE_CVCURVE_CLASS_ID))
        {
            NURBSSet CurveSet;
            BOOL Success = GetNURBSSet(MaxObject, 0, CurveSet, TRUE);

            if (Success)
            {
                NURBSObject* MaxCurve = CurveSet.GetNURBSObject(0);

                if (MaxCurve)
                {
                    if (MaxCurve->GetKind() == kNURBSCurve && CurveSet.GetNumObjects() == 1)
                    {
                        if (InputNodeType == HEMAX_INPUT_EDITABLENODE)
                        {
                            BuildCurveForEditableNode(Node, MaxCurve, GetInputNodeName());
                        }
                        else
                        {
                            BuildCurveForInputNode(Node, MaxCurve, GetInputNodeName());
                        }
                    }
                    else if (MaxCurve->GetKind() == kNURBSPoint && CurveSet.GetNumObjects() >= 1)
                    {
                        std::vector<NURBSPoint*> Points;
                        for (int n = 0; n < CurveSet.GetNumObjects(); ++n)
                        {
                            NURBSObject* AnObject = CurveSet.GetNURBSObject(n);
                            if (AnObject->GetKind() == kNURBSPoint)
                            {
                                Points.push_back((NURBSPoint*)AnObject);
                            }
                        }
                        BuildPointCurveForInputNode(Node, Points, GetInputNodeName());
                    }
                }
            }
        }
    }
}

void
HEMAX_Input_NURBS::BuildPointCurveForInputNode(HEMAX_Node* Node, std::vector<NURBSPoint*>& NURBSPoints, std::string InputNodeName)
{
    CreateInputNode(*Node, InputNodeName + "_" + std::to_string(rand()) + std::to_string(rand()));
    
    int PointCount = (int)NURBSPoints.size();

    // Add the part to new input node
    AddNewPart(*Node, HEMAX_PARTTYPE_CURVE, 1, PointCount, PointCount);

    HEMAX_CurveInfo CurveInfo;
    CurveInfo.curveCount = 1;
    CurveInfo.curveType = HAPI_CURVETYPE_NURBS;
    CurveInfo.hasKnots = false;
    CurveInfo.isPeriodic = false;
    CurveInfo.knotCount = 0;
    CurveInfo.order = PointCount + 1;
    CurveInfo.vertexCount = PointCount;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    SM.Session->SetCurveInfo(Node->Info.id, 0, &CurveInfo);

    int CountsArray[] = { PointCount };
    SM.Session->SetCurveCounts(Node->Info.id, 0, CountsArray, 0, 1);

    int OrdersArray[] = { PointCount + 1 };
    SM.Session->SetCurveOrders(Node->Info.id, 0, OrdersArray, 0, 1);

    float* CVPoints = new float[PointCount * 3];

    float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

    for (int c = 0; c < PointCount; ++c)
    {
        Point3 Pos = NURBSPoints[c]->GetPosition(0);

        CVPoints[c * 3] = Pos.x * ScaleConversion;
        CVPoints[c * 3 + 1] = Pos.z * ScaleConversion;
        CVPoints[c * 3 + 2] = -Pos.y * ScaleConversion;
    }

    HAPI_AttributeInfo PointAttributeInfo = AddNewPointAttribute(*Node, PointCount, 3, HEMAX_POSITION_ATTRIBUTE);
    SendFloatAttributeData(*Node, HEMAX_POSITION_ATTRIBUTE, PointAttributeInfo, CVPoints, PointCount);
    FinalizeInputGeometry(*Node);

    delete[] CVPoints;
}

std::vector<float>&
HEMAX_Input_NURBS::GetKnotsArray()
{
    return Knots;
}

void
HEMAX_Input_NURBS::BuildCurveForInputNode(HEMAX_Node* Node, NURBSObject* CurveObj, std::string InputNodeName)
{
    NURBSCVCurve* TheCurve = (NURBSCVCurve*)CurveObj;

    CreateInputNode(*Node, InputNodeName + "_" + std::to_string(rand()) + std::to_string(rand()));

    int CurveOrder = TheCurve->GetOrder();
    int KnotCount = TheCurve->GetNumKnots();
    int CVCount = TheCurve->GetNumCVs();

    // Add the part to new input node
    AddNewPart(*Node, HEMAX_PARTTYPE_CURVE, 1, CVCount, CVCount);

    // Set curve info
    HEMAX_CurveInfo CurveInfo;
    CurveInfo.curveCount = 1;
    CurveInfo.curveType = HAPI_CURVETYPE_NURBS;
    CurveInfo.hasKnots = (KnotCount > 0);
    CurveInfo.isPeriodic = false;
    CurveInfo.knotCount = KnotCount;
    CurveInfo.order = CurveOrder;
    CurveInfo.vertexCount = CVCount;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    SM.Session->SetCurveInfo(Node->Info.id, 0, &CurveInfo);

    int CountsArray[] = { CVCount };
    SM.Session->SetCurveCounts(Node->Info.id, 0, CountsArray, 0, 1);

    int OrdersArray[] = { CurveOrder };
    SM.Session->SetCurveOrders(Node->Info.id, 0, OrdersArray, 0, 1);

    float* KnotsArray = new float[KnotCount];

    for (int k = 0; k < KnotCount; ++k)
    {
        KnotsArray[k] = (float)TheCurve->GetKnot(k);
    }

    SM.Session->SetCurveKnots(Node->Info.id, 0, KnotsArray, 0, KnotCount);

    float* CVPoints = new float[CVCount * 3];

    float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

    for (int c = 0; c < CVCount; ++c)
    {
        NURBSControlVertex* CV = TheCurve->GetCV(c);

        Point3 CVPos = CV->GetPosition(0);

        CVPoints[c * 3] = CVPos.x * ScaleConversion;
        CVPoints[(c * 3) + 1] = CVPos.z * ScaleConversion;
        CVPoints[(c * 3) + 2] = -CVPos.y * ScaleConversion;
    }

    HAPI_AttributeInfo PointAttributeInfo = AddNewPointAttribute(*Node, CVCount, 3, HEMAX_POSITION_ATTRIBUTE);
    SendFloatAttributeData(*Node, HEMAX_POSITION_ATTRIBUTE, PointAttributeInfo, CVPoints, CVCount);
    FinalizeInputGeometry(*Node);

    delete[] KnotsArray;
    delete[] CVPoints;
}

void
HEMAX_Input_NURBS::BuildCurveForEditableNode(HEMAX_Node* Node, NURBSObject* CurveObj, std::string InputNodeName)
{
    Cook(*Node);

    NURBSCVCurve* TheCurve = (NURBSCVCurve*)CurveObj;

    int CurveOrder = TheCurve->GetOrder();
    int KnotCount = TheCurve->GetNumKnots();
    int CVCount = TheCurve->GetNumCVs();

    // Add the part to new input node
    AddNewPart(*Node, HEMAX_PARTTYPE_CURVE, 1, CVCount, CVCount);

    // Set curve info
    HEMAX_CurveInfo CurveInfo;
    CurveInfo.curveCount = 1;
    CurveInfo.curveType = HAPI_CURVETYPE_NURBS;
    CurveInfo.hasKnots = (KnotCount > 0);
    CurveInfo.isPeriodic = false;
    CurveInfo.knotCount = KnotCount;
    CurveInfo.order = CurveOrder;
    CurveInfo.vertexCount = CVCount;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    SM.Session->SetCurveInfo(Node->Info.id, 0, &CurveInfo);

    int CountsArray[] = { CVCount };
    SM.Session->SetCurveCounts(Node->Info.id, 0, CountsArray, 0, 1);

    int OrdersArray[] = { CurveOrder };
    SM.Session->SetCurveOrders(Node->Info.id, 0, OrdersArray, 0, 1);

    float* KnotsArray = new float[KnotCount];
    Knots.clear();

    for (int k = 0; k < KnotCount; ++k)
    {
        KnotsArray[k] = (float)TheCurve->GetKnot(k);
        Knots.push_back(KnotsArray[k]);
    }

    SM.Session->SetCurveKnots(Node->Info.id, 0, KnotsArray, 0, KnotCount);

    float* CVPoints = new float[CVCount * 3];

    float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

    for (int c = 0; c < CVCount; ++c)
    {
        NURBSControlVertex* CV = TheCurve->GetCV(c);

        Point3 CVPos = CV->GetPosition(0);

        CVPoints[c * 3] = CVPos.x * ScaleConversion;
        CVPoints[(c * 3) + 1] = CVPos.z * ScaleConversion;
        CVPoints[(c * 3) + 2] = -CVPos.y * ScaleConversion;
    }

    std::string PointString;
    HEMAX_Utilities::ConstructPointString(CVPoints, CVCount, PointString);

    HEMAX_Parameter CoordParam = GetParameter(*Node, "coords");

    if (CoordParam.Type != HEMAX_PARAM_INVALID)
    {
        std::vector<std::string> StringValues = { PointString };
        UpdateParameterStringValues(CoordParam, StringValues);
    }

    delete[] KnotsArray;
    delete[] CVPoints;
}