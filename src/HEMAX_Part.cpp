#include "HEMAX_Part.h"

#include "HEMAX_SessionManager.h"

void
LoadPartInfo(HEMAX_Part& Part, HEMAX_NodeId Node, int PartNum)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    SM.Session->GetPartInfo(Node, PartNum, &Part.Info);

    if (Part.Info.type == HEMAX_PARTTYPE_INSTANCER)
    {
        Part.PackedPrimInfo.InstanceCount = Part.Info.instanceCount;
        Part.PackedPrimInfo.InstancedPartCount = Part.Info.instancedPartCount;

        Part.PackedPrimInfo.InstancedPartIds.resize(Part.PackedPrimInfo.InstancedPartCount);
        SM.Session->GetInstancedPartIds(Node, PartNum, &Part.PackedPrimInfo.InstancedPartIds.front(), 0, Part.PackedPrimInfo.InstancedPartCount);
        Part.PackedPrimInfo.InstancedTransforms = SM.GetInstancedPartTransforms(Node, PartNum, Part.PackedPrimInfo.InstanceCount);
    }
}