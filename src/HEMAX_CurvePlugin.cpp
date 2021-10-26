#include "HEMAX_CurvePlugin.h"

#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"

HEMAX_CurvePlugin::HEMAX_CurvePlugin()
{
    Shape = nullptr;
    NurbsCurveNode = nullptr;
}

void
HEMAX_CurvePlugin::SetPart(HEMAX_NodeId _NodeId, HEMAX_PartId _PartId, HEMAX_CurveInfo _CurveInfo)
{
    NodeId = _NodeId;
    PartId = _PartId;
    CurveInfo = _CurveInfo;
}

void
HEMAX_CurvePlugin::SetLinearShape(LinearShape* _Shape)
{
    Shape = _Shape;
}

void
HEMAX_CurvePlugin::SetNurbsCurve(INode* Node)
{
    NurbsCurveNode = Node;
}

INode*
HEMAX_CurvePlugin::GetINode()
{
    INode* Node = nullptr;

    if (CurveInfo.curveType == HEMAX_CURVETYPE_LINEAR && Shape)
    {
	ULONG Handle = 0;
	Shape->NotifyDependents(FOREVER, (PartID)&Handle, REFMSG_GET_NODE_HANDLE);
	Node = GetCOREInterface()->GetINodeByHandle(Handle);
    }
    else if (CurveInfo.curveType == HEMAX_CURVETYPE_NURBS && NurbsCurveNode)
    {
	Node = NurbsCurveNode;
    }

    return Node;
}

bool
HEMAX_CurvePlugin::BuildCurve()
{
    if (CurveInfo.curveType == HEMAX_CURVETYPE_LINEAR)
    {
	return BuildLinearShape();
    }
    else if (CurveInfo.curveType == HEMAX_CURVETYPE_NURBS)
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

	INode* CurveNode = GetCOREInterface()->CreateObjectNode(Shape);
	if (!CurveNode)
	    Success = false;
    }

    if (Shape && Success)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

	std::vector<int> SegmentsCVCount(CurveInfo.curveCount);
	SM.Session->GetCurveCounts(NodeId, PartId, SegmentsCVCount.data(), 0, CurveInfo.curveCount); 

	std::vector<float> CurvePoints(CurveInfo.vertexCount * 3);
        std::vector<int> MatIds(CurveInfo.vertexCount);

	HEMAX_AttributeInfo PositionAttributeInfo;
	SM.Session->GetAttributeInfo(NodeId, PartId,
			HEMAX_POSITION_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT,
			&PositionAttributeInfo);

        SM.Session->GetAttributeFloatData(NodeId, PartId,
            HEMAX_POSITION_ATTRIBUTE, &PositionAttributeInfo,
            -1, CurvePoints.data(), 0, CurveInfo.vertexCount);

        HEMAX_AttributeInfo MatIdAttrInfo;
        SM.Session->GetAttributeInfo(NodeId, PartId,
            HEMAX_MATERIAL_ID_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT,
            &MatIdAttrInfo);
        if (MatIdAttrInfo.exists)
        {
            SM.Session->GetAttributeIntData(NodeId, PartId,
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

    HEMAX_AttributeInfo PositionAttributeInfo;
    SM.Session->GetAttributeInfo(NodeId, PartId,
	HEMAX_POSITION_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT,
        &PositionAttributeInfo);

    SM.Session->GetAttributeFloatData(NodeId, PartId,
		    HEMAX_POSITION_ATTRIBUTE, &PositionAttributeInfo,
		    -1, CurvePoints.data(), 0, CurveInfo.vertexCount);

    HEMAX_AttributeInfo MatIdAttrInfo;
    SM.Session->GetAttributeInfo(NodeId, PartId, HEMAX_MATERIAL_ID_ATTRIBUTE,
        HEMAX_ATTRIBUTEOWNER_DETAIL, &MatIdAttrInfo);
    int MatId = -1;
    if (MatIdAttrInfo.exists)
    {
        SM.Session->GetAttributeIntData(NodeId, PartId,
            HEMAX_MATERIAL_ID_ATTRIBUTE, &MatIdAttrInfo, -1, &MatId, 0, 1);
    }

    std::vector<int> CurvesCVCount(CurveInfo.curveCount);
    SM.Session->GetCurveCounts(NodeId, PartId, CurvesCVCount.data(), 0, CurveInfo.curveCount);

    std::vector<int> CurvesOrder(CurveInfo.curveCount);
    SM.Session->GetCurveOrders(NodeId, PartId, CurvesOrder.data(), 0, CurveInfo.curveCount);

    std::vector<float> CurvesKnots(CurveInfo.knotCount);
    SM.Session->GetCurveKnots(NodeId, PartId, CurvesKnots.data(), 0, CurveInfo.knotCount);

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

    if (!NurbsCurveNode)
    {
	INode* Node = GetCOREInterface()->CreateObjectNode(Obj);

	if (Node)
	{
	    NurbsCurveNode = Node;
	}
	else
	{
	    Success = false;
	}
    }
    else
    {
	Object* OldCurve = NurbsCurveNode->GetObjectRef();
	delete OldCurve;
	NurbsCurveNode->SetObjectRef(Obj);
    }

    return Success;
}
