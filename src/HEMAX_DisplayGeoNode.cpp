#include "HEMAX_DisplayGeoNode.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"
#include <set>

HEMAX_DisplayGeoNode::HEMAX_DisplayGeoNode()
{
    Info.type = HAPI_GEOTYPE_INVALID;
    Info.nameSH = 0;
    Info.nodeId = -1;
    Info.isEditable = false;
    Info.isTemplated = false;
    Info.isDisplayGeo = true;
    Info.hasGeoChanged = true;
    Info.hasMaterialChanged = true;
    Info.pointGroupCount = 0;
    Info.primitiveGroupCount = 0;
    Info.partCount = 1;

    IsVisible = false;
}

void
HEMAX_DisplayGeoNode::Init(HAPI_NodeId NodeId)
{
    Node = NodeId;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_NodeInfo NodeInfo;
    HEMAX_HoudiniApi::GetNodeInfo(SM.Session, NodeId, &NodeInfo);
    Name = SM.Session->GetHAPIString(NodeInfo.nameSH);

    if (HEMAX_HoudiniApi::GetDisplayGeoInfo(SM.Session, NodeId, &Info))
    {
	Parts.resize(Info.partCount);
	for (int p = 0; p < Info.partCount; p++)
	{
	    Parts[p].Update(Info.nodeId, p);
	}
    }
}

void
HEMAX_DisplayGeoNode::Update(bool Cook)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager(); 

    if (Cook)
    {
        HEMAX_HoudiniApi::CookNode(SM.Session, Node, SM.Session->GetCookOptions());
    }

    if (HEMAX_HoudiniApi::GetDisplayGeoInfo(SM.Session, Node, &Info))
    {
	if (Parts.size() != Info.partCount)
	{
	    Parts.resize(Info.partCount);
	}

	for (int p = 0; p < Parts.size(); p++)
	{
	    Parts[p].Update(Info.nodeId, p);
	}
    }
}

HEMAX_Part*
HEMAX_DisplayGeoNode::GetPart(HAPI_PartId Id)
{
    for (int i = 0; i < Parts.size(); i++)
    {
	if (Parts[i].Info.id == Id)
	{
	    return &Parts[i];
	}
    }

    return nullptr;
}
