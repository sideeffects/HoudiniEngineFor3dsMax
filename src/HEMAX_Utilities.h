#pragma once

#include <HAPI.h>
#include <string>
#include <vector>

#pragma warning(push, 0)
#include <istdplug.h>
#include <linshape.h>
#pragma warning(pop)

#include "HEMAX_Types.h"

#define HEMAX_RAD2DEG 57.2958f

#define HEMAX_HOUDINI_REGISTRY_KEY_PREFIX "SOFTWARE\\Side Effects Software\\"
#define HEMAX_HOUDINI_STEAM_REGISTRY_KEY_PREFIX "SOFTWARE\\WOW6432Node\\Side Effects Software\\"

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
    static HAPI_Transform MaxTransformToHAPITransform(HEMAX_MaxTransform& Transform, HAPI_RSTOrder RSTOrder = HAPI_RSTORDER_DEFAULT);
    static HAPI_TransformEuler HAPITransformToHAPITransformEuler(HAPI_Transform& Transform, HAPI_RSTOrder RSTOrder = HAPI_RSTORDER_DEFAULT);
    static HAPI_TransformEuler MaxTransformToHAPITransformEuler(HEMAX_MaxTransform& Transform, HAPI_RSTOrder RSTOrder = HAPI_RSTORDER_DEFAULT);
    static HEMAX_MaxTransform BuildMaxTransformFromINode(INode* Node);
    static void SetINodeTransform(INode* Node, HEMAX_MaxTransform& Transform);
    static void ApplyTransformToINode(INode* Node, HEMAX_MaxTransform& Transform);
    static Matrix3 MaxTransformToMatrix3(HEMAX_MaxTransform& Transform);
    static HEMAX_MaxTransform CalculateRelativeTransformBetweenINodes(INode* OriginNode, INode* SecondaryNode);
    static Matrix3 GetINodeTransformationMatrix(INode* Node);
    static Matrix3 GetINodeLocalTransformationMatrix(INode* Node);
    static void Matrix3ToFlatArray(Matrix3& In, std::vector<float>& Out);

    static float GetHoudiniToMaxScale();
    static float GetMaxToHoudiniScale();

    static void ConstructPointString(float* Points, int PointCount, std::string& PointString);
    static void ExtractPointsFromParmString(std::vector<Point3>& Points, std::string& PointString);

    static std::string CreateHoudiniVersionString(int HoudiniMajorVersion, int HoudiniMinorVersion, int HoudiniBuildVersion, int PatchNumber);
    static std::string GetHoudiniRegistryPath(std::string VersionString);
    static std::string GetHoudiniSteamRegistryPath(std::string VersionString);

    static std::string GetEnvVar(std::string Var);
    static void SetEnvVar(std::string Var, std::string Val);

    static bool IsOnlyClosedSplines(LinearShape* Curve);
    static bool IsOnlyOpenSplines(LinearShape* Curve);

    static void GetListOfAllSceneNodes(std::vector<std::wstring>& NodeNames);
    static void GetListOfChildNodes(INode* Node, std::vector<std::wstring>& NodeNames);

    static std::string WideStringToStringUnsafe(const std::wstring& In);

    static bool ParmIsIntType(HAPI_ParmType Type);
    static bool ParmIsFloatType(HAPI_ParmType Type);
    static bool ParmIsStringType(HAPI_ParmType Type);
    static bool ParmIsNodeType(HAPI_ParmType Type);
};
