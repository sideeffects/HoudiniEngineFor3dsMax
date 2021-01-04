#include "HEMAX_GeometryPlugin.h"

#include "HEMAX_GeometryMouseCallback.h"

static HEMAX_GeometryMouseCallback GlobalGeometryPluginMouseCallback;

HEMAX_GeometryPlugin::HEMAX_GeometryPlugin(bool TimeDependent)
{
    Mesh = nullptr;
    IsStranded = true;
    IsTimeDependent = TimeDependent;
}

HEMAX_GeometryPlugin::~HEMAX_GeometryPlugin()
{
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
    ReferenceTarget* TheClone = new HEMAX_GeometryPlugin(IsTimeDependent);
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
	Mesh->MarshallDataInto3dsMaxMNMesh(polyMesh);
    }
    else
    {
	polyMesh.ClearAndFree();
    }

    if (IsTimeDependent)
        ivalid = Interval(Time, Time);
    else
        ivalid = FOREVER;
}

void
HEMAX_GeometryPlugin::SetFaceMaterialId(int Face, int MatId)
{
    polyMesh.F(Face)->material = MatId;
}
