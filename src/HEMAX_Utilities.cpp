#include "HEMAX_Utilities.h"

#include "HEMAX_Types.h"

#include "simpobj.h"
#include "units.h"

#include <iostream>
#include <sstream>
#include <Windows.h>

HEMAX_MaxTransform
HEMAX_Utilities::HAPITransformToMaxTransform(HAPI_Transform& Transform)
{
    HEMAX_MaxTransform MaxTransform;

    float Scale = GetHoudiniToMaxScale();

    MaxTransform.Position[0] = Transform.position[0] * Scale;
    MaxTransform.Position[1] = -Transform.position[2] * Scale;
    MaxTransform.Position[2] = Transform.position[1] * Scale;

    MaxTransform.Quaternion[0] = Transform.rotationQuaternion[0];
    MaxTransform.Quaternion[1] = -Transform.rotationQuaternion[2];
    MaxTransform.Quaternion[2] = Transform.rotationQuaternion[1];
    MaxTransform.Quaternion[3] = -Transform.rotationQuaternion[3];

    MaxTransform.Scale[0] = Transform.scale[0];
    MaxTransform.Scale[1] = Transform.scale[2];
    MaxTransform.Scale[2] = Transform.scale[1];

    return MaxTransform;
}

HEMAX_MaxTransform
HEMAX_Utilities::GetIdentityTransform()
{
    HEMAX_MaxTransform Transform;

    Transform.Position[0] = 0;
    Transform.Position[1] = 0;
    Transform.Position[2] = 0;

    Transform.Quaternion[0] = 0;
    Transform.Quaternion[1] = 0;
    Transform.Quaternion[2] = 0;
    Transform.Quaternion[3] = 0;

    Transform.Scale[0] = 1;
    Transform.Scale[1] = 1;
    Transform.Scale[2] = 1;

    return Transform;
}

HAPI_TransformEuler
HEMAX_Utilities::HAPITransformToHAPITransformEuler(HAPI_Transform& Transform,
                    HEMAX_RSTOrder RSTOrder)
{
    Quat Rotation = Quat(Transform.rotationQuaternion[0],
                        Transform.rotationQuaternion[1],
                        Transform.rotationQuaternion[2],
                        Transform.rotationQuaternion[3]);

    float XRot, YRot, ZRot;
    Rotation.GetEuler(&XRot, &YRot, &ZRot);

    HAPI_TransformEuler TransformEuler;

    float Scale = GetMaxToHoudiniScale();

    TransformEuler.position[0] = Transform.position[0] * Scale;
    TransformEuler.position[1] = Transform.position[1] * Scale;
    TransformEuler.position[2] = Transform.position[2] * Scale;

    TransformEuler.rotationEuler[0] = -XRot * HEMAX_RAD2DEG;
    TransformEuler.rotationEuler[1] = -YRot * HEMAX_RAD2DEG;
    TransformEuler.rotationEuler[2] = -ZRot * HEMAX_RAD2DEG;

    TransformEuler.scale[0] = Transform.scale[0];
    TransformEuler.scale[1] = Transform.scale[1];
    TransformEuler.scale[2] = Transform.scale[2];

    TransformEuler.shear[0] = 0.0f;
    TransformEuler.shear[1] = 0.0f;
    TransformEuler.shear[2] = 0.0f;

    TransformEuler.rotationOrder = HAPI_ZYX;
    TransformEuler.rstOrder = HAPI_RSTOrder(RSTOrder);

    return TransformEuler;
}

HAPI_Transform
HEMAX_Utilities::MaxTransformToHAPITransform(HEMAX_MaxTransform& Transform,
                    HEMAX_RSTOrder RSTOrder)
{
    HAPI_Transform HAPITransform;

    HAPITransform.position[0] = Transform.Position[0];
    HAPITransform.position[1] = Transform.Position[2];
    HAPITransform.position[2] = -Transform.Position[1];

    HAPITransform.rotationQuaternion[0] = Transform.Quaternion[0];
    HAPITransform.rotationQuaternion[1] = Transform.Quaternion[2];
    HAPITransform.rotationQuaternion[2] = -Transform.Quaternion[1];
    HAPITransform.rotationQuaternion[3] = -Transform.Quaternion[3];

    HAPITransform.scale[0] = Transform.Scale[0];
    HAPITransform.scale[1] = Transform.Scale[2];
    HAPITransform.scale[2] = Transform.Scale[1];

    HAPITransform.shear[0] = 0.0;
    HAPITransform.shear[1] = 0.0;
    HAPITransform.shear[2] = 0.0;

    HAPITransform.rstOrder = HAPI_RSTOrder(RSTOrder);

    return HAPITransform;
}

HAPI_TransformEuler
HEMAX_Utilities::MaxTransformToHAPITransformEuler(HEMAX_MaxTransform& Transform,
        HEMAX_RSTOrder RSTOrder)
{
    return HAPITransformToHAPITransformEuler(MaxTransformToHAPITransform(
                                                Transform, RSTOrder), RSTOrder);
}

HEMAX_MaxTransform
HEMAX_Utilities::BuildMaxTransformFromINode(INode* Node)
{
    TimeValue Time = GetCOREInterface()->GetTime();

    Point3 Translation;
    Node->GetTMController()->GetPositionController()->GetValue(Time,
            Translation, FOREVER);

    Quat Rotation;
    Node->GetTMController()->GetRotationController()->GetValue(Time,
            Rotation, FOREVER);

    Point3 Scale;
    Node->GetTMController()->GetScaleController()->GetValue(Time,
            Scale, FOREVER);

    HEMAX_MaxTransform MaxTransform;

    MaxTransform.Position[0] = Translation.x;
    MaxTransform.Position[1] = Translation.y;
    MaxTransform.Position[2] = Translation.z;

    MaxTransform.Quaternion[0] = Rotation.x;
    MaxTransform.Quaternion[1] = Rotation.y;
    MaxTransform.Quaternion[2] = Rotation.z;
    MaxTransform.Quaternion[3] = Rotation.w;

    MaxTransform.Scale[0] = Scale.x;
    MaxTransform.Scale[1] = Scale.y;
    MaxTransform.Scale[2] = Scale.z;

    return MaxTransform;
}

float
HEMAX_Utilities::GetHoudiniToMaxScale()
{
    int SystemUnitType;
    float SystemUnitScale;
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
    GetSystemUnitInfo(&SystemUnitType, &SystemUnitScale);
#else
    GetMasterUnitInfo(&SystemUnitType, &SystemUnitScale);
#endif
    return (float)GetRelativeScale(UNITS_METERS, 1, SystemUnitType,
                SystemUnitScale);
}

float
HEMAX_Utilities::GetMaxToHoudiniScale()
{
    return (float)(1.0 / GetHoudiniToMaxScale());
}

void
HEMAX_Utilities::SetINodeTransform(INode* Node, HEMAX_MaxTransform& Transform)
{
    Matrix3 TransformationMatrix = MaxTransformToMatrix3(Transform);

    TimeValue TimeStamp(0);

    Node->SetNodeTM(TimeStamp, TransformationMatrix);
}

void
HEMAX_Utilities::ApplyTransformToINode(INode* Node,
        HEMAX_MaxTransform& Transform)
{
    Matrix3 CurrentTM = Node->GetNodeTM(1);
    Matrix3 Transformation = MaxTransformToMatrix3(Transform);

    TimeValue TimeStamp(0);

    Node->SetNodeTM(TimeStamp, (Transformation*CurrentTM));
}

Matrix3
HEMAX_Utilities::MaxTransformToMatrix3(HEMAX_MaxTransform& Transform)
{
    Matrix3 TransformationMatrix(1);

    Matrix3 Scale(1);
    Scale.SetScale(Point3(Transform.Scale[0],
                    Transform.Scale[1], Transform.Scale[2]));

    Matrix3 Rotation(1);
    Rotation.SetRotate(Quat(Transform.Quaternion[0], Transform.Quaternion[1],
		Transform.Quaternion[2], Transform.Quaternion[3]));

    Matrix3 Translate(1);
    Translate.Translate(Point3(Transform.Position[0], Transform.Position[1],
        Transform.Position[2]));

    return (Scale * Rotation * Translate);
}

HEMAX_MaxTransform
HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(INode* OriginNode,
        INode* SecondaryNode)
{
    Matrix3 PrimaryMatrix = OriginNode->GetNodeTM(GetCOREInterface()->GetTime());
    Matrix3 SecondaryMatrix = SecondaryNode->GetNodeTM(GetCOREInterface()->GetTime());

    PrimaryMatrix.Invert();

    Matrix3 RelativeOffset = SecondaryMatrix * PrimaryMatrix;
    Quat RelativeQuat(RelativeOffset);

    HEMAX_MaxTransform InputTransform =
        HEMAX_Utilities::BuildMaxTransformFromINode(SecondaryNode);

    InputTransform.Position[0] = RelativeOffset.GetTrans()[0];
    InputTransform.Position[1] = RelativeOffset.GetTrans()[1];
    InputTransform.Position[2] = RelativeOffset.GetTrans()[2];

    InputTransform.Quaternion[0] = RelativeQuat.x;
    InputTransform.Quaternion[1] = RelativeQuat.y;
    InputTransform.Quaternion[2] = RelativeQuat.z;
    InputTransform.Quaternion[3] = RelativeQuat.w;

    return InputTransform;
}

Matrix3
HEMAX_Utilities::GetINodeTransformationMatrix(INode* Node)
{
    TimeValue Time = GetCOREInterface()->GetTime();

    return Node->GetNodeTM(Time);
}

Matrix3
HEMAX_Utilities::GetINodeLocalTransformationMatrix(INode* Node)
{
    TimeValue Time = GetCOREInterface()->GetTime();

    Matrix3 NodeTM = Node->GetNodeTM(Time);
    INode* ParentNode = Node->GetParentNode();
    Matrix3 ParentNodeTM = ParentNode->GetNodeTM(Time);
    Matrix3 LocalTM = NodeTM * Inverse(ParentNodeTM);

    return LocalTM;
}

void
HEMAX_Utilities::Matrix3ToFlatArray(Matrix3& In, std::vector<float>& Out)
{
    for (int i = 0; i < 4; i++)
    {
	Point3 Row = In.GetRow(i);
	Out.push_back(Row.x);
	Out.push_back(Row.y);
	Out.push_back(Row.z);
    }
}

void
HEMAX_Utilities::ConstructPointString(float* Points, int PointCount,
        std::string& PointString)
{
    for (int p = 0; p < PointCount; ++p)
    {
	PointString.append(std::to_string(Points[p * 3]));
	PointString.append(",");
	PointString.append(std::to_string(Points[(p * 3) + 1]));
	PointString.append(",");
	PointString.append(std::to_string(Points[(p * 3) + 2]));
	PointString.append(" ");
    }
}

void
HEMAX_Utilities::ExtractPointsFromParmString(std::vector<Point3>& Points,
        std::string& PointString)
{
    std::istringstream PointStream(PointString);
    std::string APoint;

    while (std::getline(PointStream, APoint, ' '))
    {
	std::istringstream ValueStream(APoint);
	std::string ValueString;

	Point3 Point;
	int Index = 0;

	while (std::getline(ValueStream, ValueString, ','))
	{
	    Point[Index++] = std::stof(ValueString);
	}

	Points.push_back(Point);
    }
}

std::string
HEMAX_Utilities::CreateHoudiniVersionString(int HoudiniMajorVersion,
        int HoudiniMinorVersion, int HoudiniBuildVersion, int PatchNumber)
{
    std::string VersionString = "Houdini " +
        std::to_string(HoudiniMajorVersion) + "." +
        std::to_string(HoudiniMinorVersion) + "." +
        std::to_string(HoudiniBuildVersion);

    if (PatchNumber > 0)
    {
	VersionString += "." + std::to_string(PatchNumber);
    }

    return VersionString;
}

std::string
HEMAX_Utilities::GetHoudiniRegistryPath(std::string VersionString)
{
    return std::string(HEMAX_HOUDINI_REGISTRY_KEY_PREFIX + VersionString);
}

std::string
HEMAX_Utilities::GetHoudiniSteamRegistryPath(std::string VersionString)
{
    return std::string(HEMAX_HOUDINI_STEAM_REGISTRY_KEY_PREFIX + VersionString);
}

std::string
HEMAX_Utilities::GetEnvVar(std::string Var)
{
    char VarBuffer[4096];

    DWORD Count = GetEnvironmentVariableA(Var.c_str(), VarBuffer, 4096);

    if (Count && Count <= 4096)
    {
	std::string Value(VarBuffer);
	return Value;
    }
    else
    {
	char* VarBufferLong = new char[Count];
        Count = GetEnvironmentVariableA(Var.c_str(), VarBufferLong, Count);

        if (!Count)
        {
            delete [] VarBufferLong;
            return "";
        }
        else
        {
            std::string Value(VarBufferLong);
            delete [] VarBufferLong;
            return Value;
        }
    }
}

void
HEMAX_Utilities::SetEnvVar(std::string Var, std::string Val)
{
    SetEnvironmentVariableA(Var.c_str(), Val.c_str());
}

bool
HEMAX_Utilities::IsOnlyClosedSplines(LinearShape* Curve)
{
#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
    int CurveCount = Curve->NumberOfCurves(GetCOREInterface()->GetTime());
#endif
#ifdef HEMAX_VERSION_2017
    int CurveCount = Curve->NumberOfCurves();
#endif

    for (int i = 0; i < CurveCount; i++)
    {
        if (!Curve->CurveClosed(GetCOREInterface()->GetTime(), i))
            return false;
    }

    return true;
}

bool
HEMAX_Utilities::IsOnlyOpenSplines(LinearShape* Curve)
{
#if defined(HEMAX_VERSION_2018) || \
    defined(HEMAX_VERSION_2019) || \
    defined(HEMAX_VERSION_2020) || \
    defined(HEMAX_VERSION_2021) || \
    defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
    int CurveCount = Curve->NumberOfCurves(GetCOREInterface()->GetTime());
#endif
#ifdef HEMAX_VERSION_2017
    int CurveCount = Curve->NumberOfCurves();
#endif

    for (int i = 0; i < CurveCount; i++)
    {
        if (Curve->CurveClosed(GetCOREInterface()->GetTime(), i))
            return false;
    }

    return true;
}

void
HEMAX_Utilities::GetListOfAllSceneNodes(std::vector<std::wstring>& NodeNames)
{
    NodeNames.clear();
    INode* CurrentNode = GetCOREInterface()->GetRootNode();

    if (CurrentNode)
    {
	int ChildCount = CurrentNode->NumberOfChildren();
	for (int c = 0; c < ChildCount; c++)
	{
	    GetListOfChildNodes(CurrentNode->GetChildNode(c), NodeNames);
	}
    }
}

void
HEMAX_Utilities::GetListOfChildNodes(INode* Node,
        std::vector<std::wstring>& NodeNames)
{
    if (Node)
    {
	NodeNames.push_back(Node->GetName());

	int ChildCount = Node->NumberOfChildren();
	for (int c = 0; c < ChildCount; c++)
	{
	    GetListOfChildNodes(Node->GetChildNode(c), NodeNames);
	}
    }
}

std::string
HEMAX_Utilities::WideStringToStringUnsafe(const std::wstring& In)
{
#pragma warning(disable:4244)
    std::string Out(In.begin(), In.end());
#pragma warning(default:4244)
    return Out;
}

bool
HEMAX_Utilities::ParmIsIntType(HEMAX_ParameterType Type)
{
    if (Type == HEMAX_PARAM_INTEGER ||
        Type == HEMAX_PARAM_BUTTON ||
        Type == HEMAX_PARAM_TOGGLE ||
        Type == HEMAX_PARAM_MULTIPARMLIST)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
HEMAX_Utilities::ParmIsFloatType(HEMAX_ParameterType Type)
{
    if (Type == HEMAX_PARAM_FLOAT ||
        Type == HEMAX_PARAM_COLOR)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
HEMAX_Utilities::ParmIsStringType(HEMAX_ParameterType Type)
{
    if (Type == HEMAX_PARAM_STRING ||
        Type == HEMAX_PARAM_PATH_FILE ||
        Type == HEMAX_PARAM_PATH_FILE_DIR ||
        Type == HEMAX_PARAM_PATH_FILE_GEO ||
        Type == HEMAX_PARAM_PATH_FILE_IMAGE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
HEMAX_Utilities::ParmIsNodeType(HEMAX_ParameterType Type)
{
    if (Type == HEMAX_PARAM_NODE)
        return true;
    else
        return false;
}
