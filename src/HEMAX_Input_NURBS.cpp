#include "HEMAX_Input_NURBS.h"

#include "HEMAX_HoudiniApi.h"
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

HEMAX_Input_NURBS::HEMAX_Input_NURBS(HEMAX_InputType Type,
    Object* MaxObject, ULONG MaxNode)
    : HEMAX_Input(Type, -1, MaxNode)
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
		    BuildCurveForInputNode(Node, MaxCurve, "modifier_input",
			HEMAX_Utilities::GetIdentityTransform());
		}
		else if (MaxCurve->GetKind() == kNURBSPoint && CurveSet.GetNumObjects() >= 1)
		{
		    for (int n = 0; n < CurveSet.GetNumObjects(); n++)
		    {
			NURBSObject* AnObject = CurveSet.GetNURBSObject(n);
			NURBSKind Kind = AnObject->GetKind();

			if (Kind == kNURBSCurve)
			{
			    BuildPointCurveForInputNode(Node, AnObject, "modifier_input",
				HEMAX_Utilities::GetIdentityTransform());
			}
		    }
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
	    Node->Delete();
	}
    }
}

void
HEMAX_Input_NURBS::RebuildAfterChange()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    Node->Delete();
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
			    BuildCurveForInputNode(Node, MaxCurve, GetInputNodeName(),
				    HEMAX_Utilities::BuildMaxTransformFromINode(MaxInputNode));
			}
		    }
		    else if (MaxCurve->GetKind() == kNURBSPoint && CurveSet.GetNumObjects() >= 1)
		    {
			for (int n = 0; n < CurveSet.GetNumObjects(); n++)
			{
			    NURBSObject* AnObject = CurveSet.GetNURBSObject(n);
			    NURBSKind Kind = AnObject->GetKind();
			    
			    if (Kind == kNURBSCurve)
			    {
				BuildPointCurveForInputNode(Node, AnObject, GetInputNodeName(),
				    HEMAX_Utilities::BuildMaxTransformFromINode(MaxInputNode));
			    }
			}
		    }
		}
	    }
	}
    }
}

void
HEMAX_Input_NURBS::BuildPointCurveForInputNode(HEMAX_Node* Node, NURBSObject* CurveObj,
    std::string InputNodeName, HEMAX_MaxTransform NodeTransform)
{
    CreateInputNode(InputNodeName + "_" + std::to_string(rand()) + std::to_string(rand()));

    MarshalNodeNameDetailAttribute();

    NURBSPointCurve* TheCurve = dynamic_cast<NURBSPointCurve*>(CurveObj);

    if (TheCurve)
    {
	int Degree, NumCVs, NumKnots;
	NURBSCVTab CVs;
	NURBSKnotTab Knots;

	TheCurve->GetNURBSData(GetCOREInterface()->GetTime(), Degree, NumCVs,
			       CVs, NumKnots, Knots);

	AddNewPart(HAPI_PARTTYPE_CURVE, 1, NumCVs, NumCVs);

	HAPI_CurveInfo CurveInfo;
	CurveInfo.curveCount = 1;
	CurveInfo.curveType = HAPI_CURVETYPE_NURBS;
	CurveInfo.hasKnots = (NumKnots > 0);
	CurveInfo.isPeriodic = false;
	CurveInfo.knotCount = NumKnots;
	CurveInfo.order = Degree;
	CurveInfo.vertexCount = NumCVs;
        CurveInfo.isClosed = TheCurve->IsClosed();

	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

        HEMAX_HoudiniApi::SetCurveInfo(SM.Session, Node->Info.id, 0,
            &CurveInfo);

	std::vector<int> CurveCountsArr = { NumCVs };
	std::vector<int> OrdersArr = { Degree };

        HEMAX_HoudiniApi::SetCurveCounts(SM.Session, Node->Info.id, 0,
            &CurveCountsArr.front(), 0, 1);
        HEMAX_HoudiniApi::SetCurveOrders(SM.Session, Node->Info.id, 0,
            &OrdersArr.front(), 0, 1);

	std::vector<float> KnotsArr(NumKnots);

	for (int k = 0; k < NumKnots; k++)
	{
	    KnotsArr[k] = (float)Knots[k];
	}

        HEMAX_HoudiniApi::SetCurveKnots(SM.Session, Node->Info.id, 0,
            &KnotsArr.front(), 0, NumKnots);

	std::vector<float> CVArr(NumCVs*3);

	float ScaleConversion = HEMAX_Utilities::GetMaxToHoudiniScale();

	for (int c = 0; c < NumCVs; c++)
	{
	    NURBSControlVertex& CV = CVs[c];
	    Point3 CVPos = CV.GetPosition(GetCOREInterface()->GetTime());

	    CVArr[c*3] = CVPos.x * ScaleConversion;
	    CVArr[c*3 + 1] = CVPos.z * ScaleConversion;
	    CVArr[c*3 + 2] = -CVPos.y * ScaleConversion;
	}

	HAPI_AttributeInfo PointAttributeInfo =
            AddNewPointAttribute(NumCVs, 3, HEMAX_POSITION_ATTRIBUTE);
	SendFloatAttributeData(HEMAX_POSITION_ATTRIBUTE, PointAttributeInfo,
            &CVArr.front(), NumCVs);

        int MatId = TheCurve->MatID();
        HAPI_AttributeInfo MatIdAttrInfo = AddNewDetailIntAttribute(1, 1,
            HEMAX_MATERIAL_ID_ATTRIBUTE);
        SendIntAttributeData(HEMAX_MATERIAL_ID_ATTRIBUTE, MatIdAttrInfo,
            &MatId, 1);

        INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);
        if (MaxInputNode)
            AddNodeTransformAttributes(MaxInputNode);

	FinalizeInputGeometry();

	Node->SetParentTransform(NodeTransform);
    }
}

std::vector<float>&
HEMAX_Input_NURBS::GetKnotsArray()
{
    return Knots;
}

void
HEMAX_Input_NURBS::BuildCurveForInputNode(HEMAX_Node* Node, NURBSObject* CurveObj,
    std::string InputNodeName, HEMAX_MaxTransform NodeTransform)
{
    NURBSCVCurve* TheCurve = (NURBSCVCurve*)CurveObj;

    CreateInputNode(InputNodeName + "_" + std::to_string(rand()) + std::to_string(rand()));

    MarshalNodeNameDetailAttribute();

    int CurveOrder = TheCurve->GetOrder();
    int KnotCount = TheCurve->GetNumKnots();
    int CVCount = TheCurve->GetNumCVs();

    // Add the part to new input node
    AddNewPart(HAPI_PARTTYPE_CURVE, 1, CVCount, CVCount);

    // Set curve info
    HAPI_CurveInfo CurveInfo;
    CurveInfo.curveCount = 1;
    CurveInfo.curveType = HAPI_CURVETYPE_NURBS;
    CurveInfo.hasKnots = (KnotCount > 0);
    CurveInfo.isPeriodic = false;
    CurveInfo.knotCount = KnotCount;
    CurveInfo.order = CurveOrder;
    CurveInfo.vertexCount = CVCount;
    CurveInfo.isClosed = TheCurve->IsClosed();

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::SetCurveInfo(SM.Session, Node->Info.id, 0, &CurveInfo);

    int CountsArray[] = { CVCount };
    HEMAX_HoudiniApi::SetCurveCounts(SM.Session, Node->Info.id, 0, CountsArray,
        0, 1);

    int OrdersArray[] = { CurveOrder };
    HEMAX_HoudiniApi::SetCurveOrders(SM.Session, Node->Info.id, 0, OrdersArray,
        0, 1);

    float* KnotsArray = new float[KnotCount];

    for (int k = 0; k < KnotCount; ++k)
    {
	KnotsArray[k] = (float)TheCurve->GetKnot(k);
    }

    HEMAX_HoudiniApi::SetCurveKnots(SM.Session, Node->Info.id, 0, KnotsArray,
        0, KnotCount);

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

    HAPI_AttributeInfo PointAttributeInfo =
        AddNewPointAttribute(CVCount, 3, HEMAX_POSITION_ATTRIBUTE);
    SendFloatAttributeData(HEMAX_POSITION_ATTRIBUTE, PointAttributeInfo,
        CVPoints, CVCount);

    int MatId = TheCurve->MatID();
    HAPI_AttributeInfo MatIdAttrInfo = AddNewDetailIntAttribute(1, 1,
        HEMAX_MATERIAL_ID_ATTRIBUTE);
    SendIntAttributeData(HEMAX_MATERIAL_ID_ATTRIBUTE, MatIdAttrInfo,
        &MatId, 1);

    INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);
    if (MaxInputNode)
        AddNodeTransformAttributes(MaxInputNode);

    FinalizeInputGeometry();

    Node->SetParentTransform(NodeTransform);

    delete[] KnotsArray;
    delete[] CVPoints;
}

void
HEMAX_Input_NURBS::BuildCurveForEditableNode(HEMAX_Node* Node, NURBSObject* CurveObj, std::string InputNodeName)
{
    Node->Cook();

    NURBSCVCurve* TheCurve = (NURBSCVCurve*)CurveObj;

    int CurveOrder = TheCurve->GetOrder();
    int KnotCount = TheCurve->GetNumKnots();
    int CVCount = TheCurve->GetNumCVs();

    // Add the part to new input node
    AddNewPart(HAPI_PARTTYPE_CURVE, 1, CVCount, CVCount);

    // Set curve info
    HAPI_CurveInfo CurveInfo;
    CurveInfo.curveCount = 1;
    CurveInfo.curveType = HAPI_CURVETYPE_NURBS;
    CurveInfo.hasKnots = (KnotCount > 0);
    CurveInfo.isPeriodic = false;
    CurveInfo.knotCount = KnotCount;
    CurveInfo.order = CurveOrder;
    CurveInfo.vertexCount = CVCount;
    CurveInfo.isClosed = TheCurve->IsClosed();

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::SetCurveInfo(SM.Session, Node->Info.id, 0, &CurveInfo);

    int CountsArray[] = { CVCount };
    HEMAX_HoudiniApi::SetCurveCounts(SM.Session, Node->Info.id, 0, CountsArray,
        0, 1);

    int OrdersArray[] = { CurveOrder };
    HEMAX_HoudiniApi::SetCurveOrders(SM.Session, Node->Info.id, 0, OrdersArray,
        0, 1);

    float* KnotsArray = new float[KnotCount];
    Knots.clear();

    for (int k = 0; k < KnotCount; ++k)
    {
	KnotsArray[k] = (float)TheCurve->GetKnot(k);
	Knots.push_back(KnotsArray[k]);
    }

    HEMAX_HoudiniApi::SetCurveKnots(SM.Session, Node->Info.id, 0, KnotsArray,
        0, KnotCount);

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

    HEMAX_Parameter* CoordParam = Node->GetParameter("coords");

    if (CoordParam)
    {
	std::vector<std::string> StringValues = { PointString };
	CoordParam->UpdateStringVals(StringValues);
    }

    delete[] KnotsArray;
    delete[] CVPoints;
}
