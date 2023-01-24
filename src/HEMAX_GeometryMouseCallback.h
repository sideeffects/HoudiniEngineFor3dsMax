#pragma once

#include "HEMAX_GeometryPlugin.h"

#include "resource.h"

#pragma warning(push, 0)
#include <istdplug.h>
#include <mouseman.h>
#pragma warning(pop)

class HEMAX_GeometryMouseCallback : public CreateMouseCallBack {

    public:
	int proc(ViewExp *Vpt, int Msg, int Point, int Flags, IPoint2 M, Matrix3& Mat);
	void SetGeomObject(HEMAX_GeometryPlugin* GeomObject);

    private:
	HEMAX_GeometryPlugin* TheObject;

	Point3 CreationPoint;
};
