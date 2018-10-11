#include "HEMAX_GeometryPlugin.h"

#include "HEMAX_GeometryMouseCallback.h"

static HEMAX_GeometryMouseCallback GlobalGeometryPluginMouseCallback;

HEMAX_GeometryPlugin::HEMAX_GeometryPlugin()
{
    Mesh = nullptr;
    IsStranded = true;
}

HEMAX_GeometryPlugin::~HEMAX_GeometryPlugin()
{
    if (Mesh)
    {
        delete Mesh;
        Mesh = nullptr;
    }
}

ClassDesc2*
GetHEMAX_GeometryPlugin_ClassDesc() {
    static HEMAX_GeometryPluginClassDesc GeometryPluginDesc;
    return &GeometryPluginDesc;
}

Class_ID
HEMAX_GeometryPlugin::ClassID()
{
    return HEMAX_GeometryPlugin_CLASS_ID;
}

ReferenceTarget*
HEMAX_GeometryPlugin::Clone(RemapDir& Remap)
{
    ReferenceTarget* TheClone = new HEMAX_GeometryPlugin();
    BaseClone(this, TheClone, Remap);
    return TheClone;
}

CreateMouseCallBack*
HEMAX_GeometryPlugin::GetCreateMouseCallBack()
{
    GlobalGeometryPluginMouseCallback.SetGeomObject(this);
    return &GlobalGeometryPluginMouseCallback;
}

void
HEMAX_GeometryPlugin::BuildMesh(TimeValue Time)
{
    if (Mesh)
    {
        ivalid = Interval(Time, Time);
        Mesh->MarshallDataInto3dsMaxMNMesh(polyMesh);
    }
}

void
HEMAX_GeometryPlugin::SetFaceMaterialId(int Face, int MatId)
{
    polyMesh.F(Face)->material = MatId;
}