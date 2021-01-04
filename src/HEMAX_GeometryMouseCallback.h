#pragma once

#include "HEMAX_GeometryPlugin.h"
#include "3dsmaxsdk_preinclude.h"
#include "resource.h"
#include <istdplug.h>
#include <mouseman.h>

class HEMAX_GeometryMouseCallback : public CreateMouseCallBack {

    public:
	int proc(ViewExp *Vpt, int Msg, int Point, int Flags, IPoint2 M, Matrix3& Mat);
	void SetGeomObject(HEMAX_GeometryPlugin* GeomObject);

    private:
	HEMAX_GeometryPlugin* TheObject;

	Point3 CreationPoint;
};
