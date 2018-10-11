#include "HEMAX_GeometryMouseCallback.h"

#include "UI/HEMAX_HDASelectionDialog.h"

int
HEMAX_GeometryMouseCallback::proc(ViewExp *Vpt, int Msg, int Point, int Flags, IPoint2 M, Matrix3& Mat)
{
    if (Msg == MOUSE_POINT)
    {
        CreationPoint = Vpt->SnapPoint(M, M, nullptr, SNAP_IN_PLANE);
        Mat.IdentityMatrix();
        Mat.Translate(CreationPoint);

        return CREATE_STOP;
    }
    else if (Msg == MOUSE_ABORT)
    {
        return CREATE_ABORT;
    }

    return CREATE_CONTINUE;
}

void
HEMAX_GeometryMouseCallback::SetGeomObject(HEMAX_GeometryPlugin* GeomObject)
{
    TheObject = GeomObject;
}