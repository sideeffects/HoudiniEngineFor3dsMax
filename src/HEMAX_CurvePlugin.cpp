#include "HEMAX_CurvePlugin.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"

HEMAX_CurvePlugin::HEMAX_CurvePlugin()
    : Shape(nullptr)
    , CurveNode(nullptr)
{
}

void
HEMAX_CurvePlugin::SetPart(HAPI_NodeId _NodeId, HAPI_PartId _PartId, HAPI_CurveInfo _CurveInfo)
{
    NodeId = _NodeId;
    PartId = _PartId;
    CurveInfo = _CurveInfo;
}

void
HEMAX_CurvePlugin::SetCurveNode(INode* Node)
{
    CurveNode = Node;

    if (CurveNode)
    {
        if (CurveNode->GetObjectRef()->CanConvertToType(linearShapeClassID))
        {
            Shape = dynamic_cast<LinearShape*>(CurveNode->GetObjectRef());
        }
        else if (CurveNode->GetObjectRef()->CanConvertToType(EDITABLE_SURF_CLASS_ID))
        {
            GetNURBSSet(CurveNode->GetObjectRef(), 
                GetCOREInterface()->GetTime(),
                CurveSet,
                false);
        }
    }
    else
    {
        Shape = nullptr;
    }
}

INode*
HEMAX_CurvePlugin::GetINode()
{
    return CurveNode;
}

bool
HEMAX_CurvePlugin::BuildCurve()
{
    if (CurveInfo.curveType == HAPI_CURVETYPE_LINEAR)
    {
	return BuildLinearShape();
    }
    else if (CurveInfo.curveType == HAPI_CURVETYPE_NURBS)
    {
	return BuildNURBSObject();
    }
    else
    {
	HEMAX_Logger::Instance().AddEntry("Cannot build an unsupported curve type",
					  HEMAX_LOG_LEVEL_WARN);
	
	return false;
    }
}

bool
HEMAX_CurvePlugin::BuildLinearShape()
{
    bool Success = true;

    if (!Shape)
    {
	Shape = new LinearShape;

	CurveNode = GetCOREInterface()->CreateObjectNode(Shape);
	if (!CurveNode)
	    Success = false;
    }

    if (Shape && Success)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

	std::vector<int> SegmentsCVCount(CurveInfo.curveCount);
        HEMAX_HoudiniApi::GetCurveCounts(SM.Session, NodeId, PartId,
            SegmentsCVCount.data(), 0, CurveInfo.curveCount); 

	std::vector<float> CurvePoints(CurveInfo.vertexCount * 3);
        std::vector<int> MatIds(CurveInfo.vertexCount);

	HAPI_AttributeInfo PositionAttributeInfo;
        HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, PartId,
			HEMAX_POSITION_ATTRIBUTE, HAPI_ATTROWNER_POINT,
			&PositionAttributeInfo);

        HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, PartId,
            HEMAX_POSITION_ATTRIBUTE, &PositionAttributeInfo,
            -1, CurvePoints.data(), 0, CurveInfo.vertexCount);

        HAPI_AttributeInfo MatIdAttrInfo;
        HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, PartId,
            HEMAX_MATERIAL_ID_ATTRIBUTE, HAPI_ATTROWNER_POINT,
            &MatIdAttrInfo);
        if (MatIdAttrInfo.exists)
        {
            HEMAX_HoudiniApi::GetAttributeIntData(SM.Session, NodeId, PartId,
                HEMAX_MATERIAL_ID_ATTRIBUTE, &MatIdAttrInfo, -1, MatIds.data(), 0,
                CurveInfo.vertexCount);
        }

        float ScaleConversion = HEMAX_Utilities::GetHoudiniToMaxScale();
	std::vector<PolyPt> PolyPoints;

	for (int p = 0; p < CurveInfo.vertexCount; p++)
	{
	    Point3 NewPoint;
            NewPoint.x = CurvePoints[(p * 3)] * ScaleConversion;
            NewPoint.y = -CurvePoints[(p * 3) + 2] * ScaleConversion;
            NewPoint.z = CurvePoints[(p * 3) + 1] * ScaleConversion;

	    PolyPt NewPolyPt;
	    NewPolyPt.p = NewPoint;

            if (MatIdAttrInfo.exists)
            {
                NewPolyPt.SetMatID(MatIds[p]);
            }

	    PolyPoints.push_back(NewPolyPt);
	}

	Shape->shape.NewShape();
	Shape->shape.SetNumLines(CurveInfo.curveCount, false);

	int PolyPtIndex = 0;

	for (int c = 0; c < CurveInfo.curveCount; c++)
	{
	    PolyLine NewLine;
	    NewLine.SetNumPts(SegmentsCVCount[c]);
	    
	    for (int p = 0; p < SegmentsCVCount[c]; p++, PolyPtIndex++)
	    {
		NewLine.pts[p] = PolyPoints[PolyPtIndex];
	    }

	    Shape->shape.lines[c] = NewLine;
	}
    }
    else
    {
	HEMAX_Logger::Instance().AddEntry("No linear spline supplied to " \
				    "construct the curve with.", HEMAX_LOG_LEVEL_WARN);
    }

    return Success;
}

bool
HEMAX_CurvePlugin::BuildNURBSObject()
{
    bool Success = true;

    if (CurveSet.GetNumObjects() > 0)
    {
	CurveSet.DeleteObjects();
	CurveSet.Clean();
    }

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    std::vector<float> CurvePoints(CurveInfo.vertexCount * 3);

    HAPI_AttributeInfo PositionAttributeInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, PartId,
	HEMAX_POSITION_ATTRIBUTE, HAPI_ATTROWNER_POINT,
        &PositionAttributeInfo);

    HEMAX_HoudiniApi::GetAttributeFloatData(SM.Session, NodeId, PartId,
		    HEMAX_POSITION_ATTRIBUTE, &PositionAttributeInfo,
		    -1, CurvePoints.data(), 0, CurveInfo.vertexCount);

    HAPI_AttributeInfo MatIdAttrInfo;
    HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, NodeId, PartId,
        HEMAX_MATERIAL_ID_ATTRIBUTE,
        HAPI_ATTROWNER_DETAIL, &MatIdAttrInfo);
    int MatId = -1;
    if (MatIdAttrInfo.exists)
    {
        HEMAX_HoudiniApi::GetAttributeIntData(SM.Session, NodeId, PartId,
            HEMAX_MATERIAL_ID_ATTRIBUTE, &MatIdAttrInfo, -1, &MatId, 0, 1);
    }

    std::vector<int> CurvesCVCount(CurveInfo.curveCount);
    HEMAX_HoudiniApi::GetCurveCounts(SM.Session, NodeId, PartId,
        CurvesCVCount.data(), 0, CurveInfo.curveCount);

    std::vector<int> CurvesOrder(CurveInfo.curveCount);
    HEMAX_HoudiniApi::GetCurveOrders(SM.Session, NodeId, PartId,
        CurvesOrder.data(), 0, CurveInfo.curveCount);

    std::vector<float> CurvesKnots(CurveInfo.knotCount);
    HEMAX_HoudiniApi::GetCurveKnots(SM.Session, NodeId, PartId,
        CurvesKnots.data(), 0, CurveInfo.knotCount);

    int CVIndex = 0;
    int KnotIndex = 0;

    for (int c = 0; c < CurveInfo.curveCount; c++)
    {
	NURBSCVCurve* Curve = new NURBSCVCurve;

	int NumKnots = CurvesOrder[c] + CurvesCVCount[c];

	Curve->SetNumCVs(CurvesCVCount[c]);
	Curve->SetOrder(CurvesOrder[c]);
	Curve->SetNumKnots(NumKnots);

	for (int k = 0; k < NumKnots; k++)
	{
	    Curve->SetKnot(k, CurvesKnots[KnotIndex++]);	
	}

        float ScaleConversion = HEMAX_Utilities::GetHoudiniToMaxScale();

	for (int p = 0; p < CurvesCVCount[c]; p++)
	{
	    NURBSControlVertex CV;

	    Point3 CVPoint;
            CVPoint.x = CurvePoints[(CVIndex * 3)] * ScaleConversion;
            CVPoint.y = -CurvePoints[(CVIndex * 3) + 2] * ScaleConversion;
            CVPoint.z = CurvePoints[(CVIndex * 3) + 1] * ScaleConversion;

	    CV.SetPosition(0, CVPoint);
	    Curve->SetCV(p, CV);

	    CVIndex += 1;
	}		

        if (MatId > -1)
        {
            Curve->MatID(MatId);
        }

	CurveSet.AppendObject(Curve);
    } 

    Matrix3 Mat(1);
    Object* Obj = CreateNURBSObject(GetCOREInterface()->GetIObjParam(), &CurveSet, Mat);

    if (!CurveNode)
    {
	INode* Node = GetCOREInterface()->CreateObjectNode(Obj);

	if (Node)
	{
	    CurveNode = Node;
	}
	else
	{
	    Success = false;
	}
    }
    else
    {
	Object* OldCurve = CurveNode->GetObjectRef();
	delete OldCurve;
	CurveNode->SetObjectRef(Obj);
    }

    return Success;
}
