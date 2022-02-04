#include "HEMAX_EditableNode.h"

#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"
#include <surf_api.h>
#include <linshape.h>

INode*
MarshallDataInto3dsMaxLinearCurve(HEMAX_EditableCurve& EditableCurve, HEMAX_EditableNode& EditableNode, int PartNum)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (EditableCurve.CurveInfo.curveCount == 1)
    {
	float* ShapePoints = new float[EditableCurve.CurveInfo.vertexCount * 3];

	HAPI_AttributeInfo PositionAttributeInfo;
	SM.Session->GetAttributeInfo(EditableNode.GeoInfo.nodeId,
                EditableNode.Parts[PartNum].Info.id, HEMAX_POSITION_ATTRIBUTE,
                HEMAX_ATTRIBUTEOWNER_POINT, &PositionAttributeInfo);
	SM.Session->GetAttributeFloatData(EditableNode.GeoInfo.nodeId,
                EditableNode.Parts[PartNum].Info.id,
                HEMAX_POSITION_ATTRIBUTE, &PositionAttributeInfo, -1,
                ShapePoints, 0, EditableCurve.CurveInfo.vertexCount);

        float ScaleConversion = HEMAX_Utilities::GetHoudiniToMaxScale();
	std::vector<PolyPt> PolyPoints;

	for (int p = 0; p < EditableCurve.CurveInfo.vertexCount; ++p)
	{
	    Point3 NewPoint;
            NewPoint.x = ShapePoints[(p * 3)];
            NewPoint.y = -ShapePoints[(p * 3) + 2] * ScaleConversion;
            NewPoint.z = ShapePoints[(p * 3) + 1] * ScaleConversion;

	    PolyPt NewPt;
	    NewPt.p = NewPoint;

	    PolyPoints.push_back(NewPt);
	}

	LinearShape* TheCurve = new LinearShape();
	TheCurve->shape.NewShape();

	TheCurve->shape.SetNumLines(1, false);

	PolyLine NewLine;
	NewLine.SetNumPts(EditableCurve.CurveInfo.vertexCount);

	for (int l = 0; l < (int)PolyPoints.size(); ++l)
	{
	    NewLine.pts[l] = PolyPoints[l];
	}

	TheCurve->shape.lines[0] = NewLine;

	delete[] ShapePoints;

	INode* TheNode = GetCOREInterface()->CreateObjectNode(TheCurve);

	return TheNode;
    }
    else if (EditableCurve.CurveInfo.curveCount > 1)
    {
	HEMAX_Logger::Instance().AddEntry("HEMAX_Input::Creating the 3ds max representation of an editable linear curve, but the process does not currently support more "
		"than one curve per node", HEMAX_LOG_LEVEL_WARN);
    }

    return nullptr;
}

INode*
MarshallDataInto3dsMaxNURBSCVCurve(HEMAX_EditableCurve& EditableCurve, HEMAX_EditableNode& EditableNode, int PartNum)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (EditableCurve.CurveInfo.curveCount == 1)
    {
	int CurveOrder = EditableCurve.CurveInfo.order;
	int KnotCount = EditableCurve.CurveInfo.knotCount;
	int CVCount = EditableCurve.CurveInfo.vertexCount;

	float* KnotsArray = new float[KnotCount];

	SM.Session->GetCurveKnots(EditableNode.GeoInfo.nodeId, EditableNode.Parts[PartNum].Info.id, KnotsArray, 0, KnotCount);

	// Find out if curve is closed
	HEMAX_Parameter* ClosedParameter = EditableNode.Node.GetParameter(EditableNodeCurveClosedParm);
	std::vector<int> ClosedValue = ClosedParameter->GetIntVals();

	bool ClosedCurve = ClosedValue[0];

	NURBSCVCurve* TheCurve = new NURBSCVCurve();

	TheCurve->SetOrder(CurveOrder);
	TheCurve->SetNumCVs(CVCount);
	TheCurve->SetNumKnots(KnotCount);

	for (int k = 0; k < KnotCount; ++k)
	{
	    TheCurve->SetKnot(k, (double)KnotsArray[k]);
	}

	float* CVPoints = new float[CVCount * 3];

	HAPI_AttributeInfo CVAttributeInfo;
	SM.Session->GetAttributeInfo(EditableNode.GeoInfo.nodeId,
                EditableNode.Parts[PartNum].Info.id, HEMAX_POSITION_ATTRIBUTE,
                HEMAX_ATTRIBUTEOWNER_POINT, &CVAttributeInfo);
	SM.Session->GetAttributeFloatData(EditableNode.GeoInfo.nodeId,
                EditableNode.Parts[PartNum].Info.id,
                HEMAX_POSITION_ATTRIBUTE, &CVAttributeInfo, -1, CVPoints, 0,
                CVCount);

        float ScaleConversion = HEMAX_Utilities::GetHoudiniToMaxScale();

	for (int c = 0; c < CVCount; ++c)
	{
	    Point3 CVPos;

            CVPos.x = CVPoints[(c * 3)] * ScaleConversion;
            CVPos.y = -CVPoints[(c * 3) + 2] * ScaleConversion;
            CVPos.z = CVPoints[(c * 3) + 1] * ScaleConversion;

	    NURBSControlVertex CV;
	    CV.SetPosition(HEMAX_FRAME_ZERO, CVPos);

	    TheCurve->SetCV(c, CV);
	}

	if (ClosedCurve)
	{
	    TheCurve->Close();
	}

	NURBSSet CurveSet;

	CurveSet.AppendObject(TheCurve);

	Matrix3 Mat(1);
	Object* Obj = CreateNURBSObject((IObjParam*)GetCOREInterface(), &CurveSet, Mat);
	Object* NurbsCVObj = Obj->ConvertToType(HEMAX_FRAME_ZERO, EDITABLE_CVCURVE_CLASS_ID);

	INode* TheNode = GetCOREInterface()->CreateObjectNode(NurbsCVObj);

	delete[] CVPoints;
	delete[] KnotsArray;

	return TheNode;
    }
    else if (EditableCurve.CurveInfo.curveCount > 1)
    {
	HEMAX_Logger::Instance().AddEntry("HEMAX_Input::Creating the 3ds max representation of an editable NURBS curve, but the process does not currently support more "
		"than one curve per node", HEMAX_LOG_LEVEL_WARN);
    }

    return nullptr;
}
