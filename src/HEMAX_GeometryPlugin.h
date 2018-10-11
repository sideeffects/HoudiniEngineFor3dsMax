#pragma once

#include <istdplug.h>
#include <iparamb2.h>
#include <mnmesh.h>
#include <polyobj.h>
#include <SimplePolyObj.h>
#include "HEMAX_Mesh.h"

#define HEMAX_GeometryPlugin_CLASS_ID Class_ID(0xcb0204a, 0x5d3a60e4)

class HEMAX_GeometryPlugin : public SimplePolyObject
{
public:
    HEMAX_GeometryPlugin();
    ~HEMAX_GeometryPlugin();

    virtual Class_ID ClassID() override;
    ReferenceTarget* Clone(RemapDir& Remap) override;
    CreateMouseCallBack* GetCreateMouseCallBack() override;
    void BuildMesh(TimeValue Time) override;

    void SetFaceMaterialId(int Face, int MatId);

    INode* MaxNode;
    HEMAX_Mesh* Mesh;

    bool IsStranded;
};

class HEMAX_GeometryPluginClassDesc : public ClassDesc2
{
public:
    virtual int IsPublic() { return TRUE; }
    virtual void* Create(BOOL) { return new HEMAX_GeometryPlugin(); }
    virtual const TCHAR* ClassName() { return L"HDA"; }
    virtual SClass_ID SuperClassID() { return GEOMOBJECT_CLASS_ID; }
    virtual Class_ID ClassID() { return HEMAX_GeometryPlugin_CLASS_ID; }
    virtual const TCHAR* Category() { return L"Houdini Engine"; }
    virtual const TCHAR* InternalName() { return _T("HEMAX_GEOM"); }
};