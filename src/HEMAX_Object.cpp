#include "HEMAX_Object.h"

#include "HEMAX_SessionManager.h"

bool
GetObjectInfo(HEMAX_NodeId NodeId, HEMAX_Object& Object)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    if (SM.Session->GetObjectInfo(NodeId, &Object.Info))
    {
        SM.Session->GetObjectTransform(Object.Info.nodeId, -1, HAPI_RSTORDER_DEFAULT, &Object.Transform);
        Object.NodeName = SM.Session->GetHAPIString(Object.Info.nameSH);

        // Check child nodes for display geometry
        int ChildObjectNodeCount;
        SM.Session->ComposeObjectList(Object.Info.nodeId, nullptr, &ChildObjectNodeCount);

        if (ChildObjectNodeCount > 0)
        {
            HEMAX_ObjectInfo* ObjectInfos = new HEMAX_ObjectInfo[ChildObjectNodeCount];
            SM.Session->GetComposedObjectList(Object.Info.nodeId, ObjectInfos, 0, ChildObjectNodeCount);

            for (int i = 0; i < ChildObjectNodeCount; i++)
            {    
                HEMAX_DisplayGeoNode NewGeoNode;
                NewGeoNode.IsVisible = ObjectInfos[i].isVisible;

                if (ObjectInfos[i].isInstancer)
                {
                    HEMAX_InstancingInformation InstanceInfo;
                    if (GetInstancingInfo(ObjectInfos[i], InstanceInfo))
                    {
                        Object.Instances.insert({ ObjectInfos[i].nodeId, InstanceInfo });
                        Object.SopNodes.insert({ ObjectInfos[i].nodeId, NewGeoNode });
                    }
                }
                else if (GetDisplayGeoNodeData(ObjectInfos[i].nodeId, NewGeoNode))
                {
                    Object.SopNodes.insert({ ObjectInfos[i].nodeId, NewGeoNode });
                }
            }

            delete[] ObjectInfos;
        }
        else
        {
            // The object node itself may have display geometry directly
            // embedded
            HEMAX_DisplayGeoNode ObjectDisplayGeo;
            if (GetDisplayGeoNodeData(Object.Info.nodeId, ObjectDisplayGeo))
            {
                ObjectDisplayGeo.IsVisible = Object.Info.isVisible;
                Object.SopNodes.insert({Object.Info.nodeId, ObjectDisplayGeo});
            }
        }

        return true;
    }
    return false;
}

bool
GetInstancingInfo(HEMAX_ObjectInfo& InstancerObjInfo, HEMAX_InstancingInformation& Info)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HEMAX_GeometryInfo GeoInfo;
    if (SM.Session->GetDisplayGeoInfo(InstancerObjInfo.nodeId, &GeoInfo))
    {
        Info.InstanceTransforms = SM.GetInstanceTransforms(InstancerObjInfo.nodeId);

        HEMAX_AttributeInfo InstanceObjInfo;
        SM.Session->GetAttributeInfo(GeoInfo.nodeId, 0, HEMAX_INSTANCE_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT, &InstanceObjInfo);

        if (InstanceObjInfo.exists)
        {
            Info.InstanceAttributeExists = true;
            Info.InstanceNodeIds = std::vector<HEMAX_NodeId>(InstanceObjInfo.count);
            SM.Session->GetInstancedObjectIds(InstancerObjInfo.nodeId, &Info.InstanceNodeIds.front(), 0, InstanceObjInfo.count);
        }
        else
        {
            Info.InstanceAttributeExists = false;
            Info.InstanceeId = InstancerObjInfo.objectToInstanceId;
        }
        return true;
    }
    return false;
}

void
UpdateObject(HEMAX_Object& Object)
{
    HEMAX_NodeId NodeId = Object.Info.nodeId;
    ClearObject(Object);
    GetObjectInfo(NodeId, Object);
}

void
UpdateInstancingInformation(HEMAX_ObjectInfo& InstancerObjInfo, HEMAX_InstancingInformation& Info)
{
    Info.InstanceNodeIds.clear();
    Info.InstanceTransforms.clear();
    GetInstancingInfo(InstancerObjInfo, Info);
}

HEMAX_DisplayGeoNode
FindSopNodeFromName(HEMAX_Object& Object, std::string Name)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    
    for (int i = 0; i < Object.SopNodes.size(); i++)
    {
        if (Object.SopNodes[i].NodeName == Name)
        {
            return Object.SopNodes[i];
        }
    }

    HEMAX_DisplayGeoNode Invalid;
    Invalid.NodeName == "Invalid";
    return Invalid;
}

void
ClearObject(HEMAX_Object& Object)
{
    Object.SopNodes.clear();
    Object.Instances.clear();
}