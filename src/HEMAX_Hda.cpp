#include "HEMAX_Hda.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_SessionManager.h"

HEMAX_Hda::HEMAX_Hda()
{
    HdaType = UNKNOWN_LEVEL_HDA;
    MyHasTimeDependentNodes = false;
}

void
HEMAX_Hda::Init(const HEMAX_Asset& Asset, int AssetIndex)
{
    HdaAsset = Asset;
    HdaAssetIndex = AssetIndex;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    MainNode.Init(Asset.Names[AssetIndex]);

    // If the node is invalid, it failed to initialize for some reason
    if (MainNode.Type == HAPI_NODETYPE_NONE)
        return;

    MainNode.Cook();

    if (MainNode.Type == HAPI_NODETYPE_OBJ)
    {
	HdaType = OBJ_LEVEL_HDA;

	TopLevelObjectNode.Init(MainNode.Info.id);
	Name = TopLevelObjectNode.Name;
    }
    else if (MainNode.Type == HAPI_NODETYPE_SOP)
    {
	HdaType = SOP_LEVEL_HDA;

	TopLevelSopNode.Init(MainNode.Info.id);
	TopLevelSopNode.IsVisible = true;

        Name = TopLevelSopNode.Name;
    }

    InitializeMaterialNodes();
    GetAllEditableNodes();
    CheckForTimeDependentNodes();
}

void
HEMAX_Hda::Delete()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (HdaType == OBJ_LEVEL_HDA)
    {
        HEMAX_HoudiniApi::DeleteNode(SM.Session, MainNode.Info.id);
    }
    else if (HdaType == SOP_LEVEL_HDA)
    {
        HEMAX_HoudiniApi::DeleteNode(SM.Session, MainNode.Info.parentId);
    }
}

void
HEMAX_Hda::Update()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    UpdateShopNodes();

    if (HdaType == OBJ_LEVEL_HDA)
    {
	TopLevelObjectNode.Update();
    }
    else if (HdaType == SOP_LEVEL_HDA)
    {
	TopLevelSopNode.Update(false);
    }
}

HEMAX_EditableNode
HEMAX_Hda::FindEditableNodeFromName(std::string Name, bool& Success)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    for (int i = 0; i < EditableNodes.size(); i++)
    {
	std::string NodeName = SM.Session->GetHAPIString(EditableNodes[i].GeoInfo.nameSH);
	if (NodeName == Name)
	{
	    Success = true;
	    return EditableNodes[i];
	}
    }

    Success = false;
    HEMAX_EditableNode EmptyNode;
    return EmptyNode;
}

void
HEMAX_Hda::UpdateShopNodes()
{
    for (auto It = ShopNodes.begin(); It != ShopNodes.end(); It++)
    {
        HEMAX_MaterialNode& MatNode = It->second;
        MatNode.UpdateInfo();
        if (MatNode.MatInfo.hasChanged)
        {
            HEMAX_HoudiniApi::CookNode(
                HEMAX_SessionManager::GetSessionManager().Session,
                MatNode.NodeId,
                HEMAX_SessionManager::GetSessionManager().Session->GetCookOptions());
        }
        MatNode.GetMaterialInformation();
    }
}

bool
HEMAX_Hda::HasTimeDependentNodes()
{
    CheckForTimeDependentNodes();
    return MyHasTimeDependentNodes; 
}

std::vector<HEMAX_DisplayGeoNode*>
HEMAX_Hda::AllDisplayNodes()
{
    std::vector<HEMAX_DisplayGeoNode*> DisplayNodes;

    if (HdaType == OBJ_LEVEL_HDA)
    {
	for (auto It = TopLevelObjectNode.SopNodes.begin(); It != TopLevelObjectNode.SopNodes.end(); It++)
	{
	    DisplayNodes.push_back(&It->second);
	}
    }
    else if (HdaType == SOP_LEVEL_HDA)
    {
	DisplayNodes.push_back(&TopLevelSopNode);
    }

    return DisplayNodes;
}

void
HEMAX_Hda::InitializeMaterialNodes()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    int Count;
    HEMAX_HoudiniApi::ComposeChildNodeList(SM.Session,
                                           MainNode.Info.id,
                                           HAPI_NODETYPE_SHOP,
                                           HAPI_NODEFLAGS_ANY,
                                           true,
                                           &Count);

    std::vector<HAPI_NodeId> ShopNodeIds(Count);
    HEMAX_HoudiniApi::GetComposedChildNodeList(SM.Session,
                                               MainNode.Info.id,
                                               ShopNodeIds.data(),
                                               Count);

    for (int i = 0; i < ShopNodeIds.size(); i++)
    {
	HEMAX_MaterialNode MatNode(ShopNodeIds[i]);
	ShopNodes.insert({ MatNode.NodeId, MatNode });
        HEMAX_MaterialNode& CreatedMatNode = ShopNodes.find(
                                                ShopNodeIds[i])->second;
        CreatedMatNode.UpdateInfo();
        HEMAX_HoudiniApi::CookNode(SM.Session, CreatedMatNode.NodeId,
            SM.Session->GetCookOptions());
        CreatedMatNode.GetMaterialInformation();
    }

    HEMAX_HoudiniApi::ComposeChildNodeList(SM.Session,
                                           MainNode.Info.id,
                                           HAPI_NODETYPE_VOP,
                                           HAPI_NODEFLAGS_ANY,
                                           true,
                                           &Count);

    std::vector<HAPI_NodeId> VopNodeIds(Count);
    HEMAX_HoudiniApi::GetComposedChildNodeList(SM.Session,
                                               MainNode.Info.id,
                                               VopNodeIds.data(),
                                               Count);

    for (int i = 0; i < VopNodeIds.size(); i++)
    {
        HEMAX_MaterialNode MatNode(VopNodeIds[i]);
        ShopNodes.insert({MatNode.NodeId, MatNode});
        
        HEMAX_MaterialNode& CreatedMatNode = ShopNodes.find(
                                                VopNodeIds[i])->second;
        CreatedMatNode.UpdateInfo();
        HEMAX_HoudiniApi::CookNode(SM.Session, CreatedMatNode.NodeId,
            SM.Session->GetCookOptions());
        CreatedMatNode.GetMaterialInformation();
    }
}

void
HEMAX_Hda::GetAllEditableNodes()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    int Count;
    HEMAX_HoudiniApi::ComposeChildNodeList(SM.Session, MainNode.Info.id,
        HAPI_NODETYPE_SOP, HAPI_NODEFLAGS_EDITABLE, true, &Count);

    if (Count > 0)
    {
	std::vector<HAPI_NodeId> EditableNodeIds(Count);
        HEMAX_HoudiniApi::GetComposedChildNodeList(SM.Session, MainNode.Info.id,
            &EditableNodeIds.front(), Count);

	for (int e = 0; e < Count; e++)
	{
	    HEMAX_EditableNode EditableNode;
	    EditableNode.Node.Type = HAPI_NODETYPE_SOP;
            HEMAX_HoudiniApi::GetNodeInfo(SM.Session, EditableNodeIds[e],
                &EditableNode.Node.Info);
	    EditableNode.Node.Cook();

	    if (HEMAX_HoudiniApi::GetGeometryInfo(SM.Session,
                    EditableNode.Node.Info.id, &EditableNode.GeoInfo))
	    {
		switch (EditableNode.GeoInfo.type)
		{
		    case (HAPI_GEOTYPE_CURVE):
			{
			    EditableNode.Parts.resize(EditableNode.GeoInfo.partCount);
			    for (int p = 0; p < EditableNode.GeoInfo.partCount; p++)
			    {
				EditableNode.Parts[p].Update(EditableNode.GeoInfo.nodeId, p);
			    }
			    EditableNodes.push_back(EditableNode);
			} break;
		    default:
			{
			    // Don't add the node since it's an unsupported type (at least for now...)
			} break;
		}
	    }
	}
    }
}

void
HEMAX_Hda::CheckForTimeDependentNodes()
{
    MainNode.UpdateInfo();

    if (MainNode.Info.isTimeDependent)
    {
        MyHasTimeDependentNodes = true;
        return;
    }

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    int NodeCount = 0;
    HEMAX_HoudiniApi::ComposeChildNodeList(SM.Session, MainNode.Info.id,
        HAPI_NODETYPE_ANY, HAPI_NODEFLAGS_ANY, true, &NodeCount);

    std::vector<HAPI_NodeId> NodeIds(NodeCount);
    HEMAX_HoudiniApi::GetComposedChildNodeList(SM.Session, MainNode.Info.id,
        NodeIds.data(), NodeCount);

    for (int i = 0; i < NodeCount; i++)
    {
        HAPI_NodeInfo NodeInfo;
        HEMAX_HoudiniApi::GetNodeInfo(SM.Session, NodeIds[i], &NodeInfo);

        if (NodeInfo.isTimeDependent)
        {
            MyHasTimeDependentNodes = true;
            return;
        }
    }
}
