#include "HEMAX_Object.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_SessionManager.h"

#include "HEMAX_Utilities.h"

HEMAX_Object::HEMAX_Object()
{
    Info.nameSH = 0;
    Info.objectInstancePathSH = 0;

    Info.hasTransformChanged = true;
    Info.haveGeosChanged = true;

    Info.isVisible = true;
    Info.isInstancer = false;
    Info.isInstanced = false;

    Info.geoCount = 0;

    Info.nodeId = -1;
    Info.objectToInstanceId = -1;
}

void
HEMAX_Object::Init(HAPI_NodeId NodeId)
{
    Node = NodeId;
    Create(NodeId);
}

void
HEMAX_Object::Update()
{
    Instances.clear();
    Create(Node);
}

HEMAX_DisplayGeoNode*
HEMAX_Object::FindSopNodeFromName(const std::string& Name)
{
    for (int i = 0; i < SopNodes.size(); i++)
    {
	if (SopNodes[i].Name == Name)
	{
	    return &SopNodes[i];
	}
    }

    // Not found
    return nullptr;
}

void
HEMAX_Object::Create(HAPI_NodeId NodeId)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (HEMAX_HoudiniApi::GetObjectInfo(SM.Session, NodeId, &Info))
    {
        HEMAX_HoudiniApi::GetObjectTransform(SM.Session, Info.nodeId, -1,
            HAPI_RSTORDER_DEFAULT, &Transform);
	Name = SM.Session->GetHAPIString(Info.nameSH);

	int ChildObjectNodeCount;
        HEMAX_HoudiniApi::ComposeObjectList(SM.Session, Info.nodeId, nullptr,
            &ChildObjectNodeCount);

	if (ChildObjectNodeCount > 0)
	{
	    std::vector<HAPI_ObjectInfo> ObjectInfos(ChildObjectNodeCount); 
            HEMAX_HoudiniApi::GetComposedObjectList(SM.Session, Info.nodeId,
                &ObjectInfos.front(), 0, ChildObjectNodeCount);

	    for (int i = 0; i < ChildObjectNodeCount; i++)
	    {
		if (ObjectInfos[i].isInstancer)
		{
		    HEMAX_InstancingInformation InstanceInfo;
		    if (GetInstancingInfo(ObjectInfos[i], InstanceInfo))
		    {
			auto InstancesSearch = Instances.find(ObjectInfos[i].nodeId);
			if (InstancesSearch != Instances.end())
			{
			    InstancesSearch->second = InstanceInfo;
			    auto SopSearch = SopNodes.find(ObjectInfos[i].nodeId);
			    if (SopSearch != SopNodes.end())
			    {
				SopSearch->second.IsVisible = ObjectInfos[i].isVisible;
			    }
			}
			else
			{
			    Instances.insert({ ObjectInfos[i].nodeId, InstanceInfo });
			    auto Result = SopNodes.emplace(ObjectInfos[i].nodeId, HEMAX_DisplayGeoNode());
			    HEMAX_DisplayGeoNode& GeoNode = Result.first->second;
			    GeoNode.IsVisible = ObjectInfos[i].isVisible;
			}
		    }
		}
		else
		{
		    auto SopSearch = SopNodes.find(ObjectInfos[i].nodeId);
		    if (SopSearch != SopNodes.end())
		    {
			SopSearch->second.Update(true);
			SopSearch->second.IsVisible = ObjectInfos[i].isVisible;	
		    }
		    else
		    {
			auto Result = SopNodes.emplace(ObjectInfos[i].nodeId, HEMAX_DisplayGeoNode());
			HEMAX_DisplayGeoNode& GeoNode = Result.first->second;
			GeoNode.Init(ObjectInfos[i].nodeId);
			GeoNode.IsVisible = ObjectInfos[i].isVisible;
		    }
		}
	    }
	}
	else
	{
	    // The object node itself may have display geometry directly
	    // embedded
	    
	    auto SopSearch = SopNodes.find(Info.nodeId);
	    if (SopSearch != SopNodes.end())
	    {
		SopSearch->second.Update(true);
		SopSearch->second.IsVisible = Info.isVisible;
	    }
	    else
	    {
		auto Result = SopNodes.emplace(Info.nodeId, HEMAX_DisplayGeoNode());
		HEMAX_DisplayGeoNode& GeoNode = Result.first->second;
                HEMAX_HoudiniApi::CookNode(SM.Session, Info.nodeId,
                    SM.Session->GetCookOptions());
		GeoNode.Init(Info.nodeId);
		GeoNode.IsVisible = Info.isVisible;
	    }
	}
    }
}

static bool
GetInstancingInfo(HAPI_ObjectInfo& InstancerObjInfo, HEMAX_InstancingInformation& Info)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_GeoInfo GeoInfo;
    if (HEMAX_HoudiniApi::GetDisplayGeoInfo(SM.Session, InstancerObjInfo.nodeId, &GeoInfo))
    {
	Info.InstanceTransforms = GetInstanceTransforms(InstancerObjInfo.nodeId);

	HAPI_AttributeInfo InstanceObjInfo;
        HEMAX_HoudiniApi::GetAttributeInfo(SM.Session, GeoInfo.nodeId, 0,
			HEMAX_INSTANCE_ATTRIBUTE, HAPI_ATTROWNER_POINT,
			&InstanceObjInfo);

	if (InstanceObjInfo.exists)
	{
            Info.HasMultipleInstancees = true;
	    Info.InstanceNodeIds = std::vector<HAPI_NodeId>(InstanceObjInfo.count);
            HEMAX_HoudiniApi::GetInstancedObjectIds(SM.Session,
                InstancerObjInfo.nodeId, &Info.InstanceNodeIds.front(), 0,
                InstanceObjInfo.count);
	}
	else
	{
            Info.HasMultipleInstancees = false;
	    Info.InstanceeId = InstancerObjInfo.objectToInstanceId;
	}

	return true;
    }

    return false;    
}

static std::vector<HEMAX_MaxTransform>
GetInstanceTransforms(HAPI_NodeId InstancerNode)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_GeoInfo GeoInfo;
    HEMAX_HoudiniApi::GetDisplayGeoInfo(SM.Session, InstancerNode, &GeoInfo);

    HAPI_PartInfo InstancerPointInfo;
    HEMAX_HoudiniApi::GetPartInfo(SM.Session, GeoInfo.nodeId, 0, &InstancerPointInfo);

    std::vector<HEMAX_MaxTransform> Transforms;

    if (InstancerPointInfo.pointCount > 0)
    {
	std::vector<HAPI_Transform> InstanceTransforms(InstancerPointInfo.pointCount);
        HEMAX_HoudiniApi::GetInstanceTransformsOnPart(SM.Session,
                GeoInfo.nodeId, 0, HAPI_SRT, &InstanceTransforms.front(), 0,
                InstancerPointInfo.pointCount);

	for (int p = 0; p < InstancerPointInfo.pointCount; ++p)
	{
	    Transforms.push_back(
                HEMAX_Utilities::HAPITransformToMaxTransform(
                    InstanceTransforms[p]));
	}
    }

    return Transforms;
}
