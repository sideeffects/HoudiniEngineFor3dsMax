#pragma once

#include <HAPI.h>
#include <istdplug.h>
#include <string>
#include <vector>
#include <linshape.h>

#include "HEMAX_Types.h"

#define HEMAX_RAD2DEG 57.2958f

#define HEMAX_HOUDINI_REGISTRY_KEY_PREFIX "SOFTWARE\\Side Effects Software\\"

struct HEMAX_MaxTransform
{
    float Position[3];
    float Quaternion[4];
    float Scale[3];
};

struct HEMAX_Utilities
{
    static HEMAX_MaxTransform HAPITransformToMaxTransform(HAPI_Transform& Transform);
    static HEMAX_MaxTransform GetIdentityTransform();
    static HAPI_Transform MaxTransformToHAPITransform(HEMAX_MaxTransform& Transform, HEMAX_RSTOrder RSTOrder = HEMAX_RSTORDER_DEFAULT);
    static HAPI_TransformEuler HAPITransformToHAPITransformEuler(HAPI_Transform& Transform, HEMAX_RSTOrder RSTOrder = HEMAX_RSTORDER_DEFAULT);
    static HAPI_TransformEuler MaxTransformToHAPITransformEuler(HEMAX_MaxTransform& Transform, HEMAX_RSTOrder RSTOrder = HEMAX_RSTORDER_DEFAULT);
    static HEMAX_MaxTransform BuildMaxTransformFromINode(INode* Node);
    static void SetINodeTransform(INode* Node, HEMAX_MaxTransform& Transform);
    static void ApplyTransformToINode(INode* Node, HEMAX_MaxTransform& Transform);
    static Matrix3 MaxTransformToMatrix3(HEMAX_MaxTransform& Transform);
    static HEMAX_MaxTransform CalculateRelativeTransformBetweenINodes(INode* OriginNode, INode* SecondaryNode);

    static float GetHoudiniToMaxScale();
    static float GetMaxToHoudiniScale();

    static void ConstructPointString(float* Points, int PointCount, std::string& PointString);
    static void ExtractPointsFromParmString(std::vector<Point3>& Points, std::string& PointString);

    static std::string CreateHoudiniVersionString(int HoudiniMajorVersion, int HoudiniMinorVersion, int HoudiniBuildVersion, int PatchNumber);
    static std::string GetHoudiniRegistryPath(std::string VersionString);

	static std::string GetEnvVar(std::string Var);

    static bool IsLinearSplineClosed(LinearShape* Curve);

    static void GetListOfAllSceneNodes(std::vector<std::wstring>& NodeNames);
    static void GetListOfChildNodes(INode* Node, std::vector<std::wstring>& NodeNames);
};