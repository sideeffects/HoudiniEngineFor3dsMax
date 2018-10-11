#include "HEMAX_Hda.h"

#include "HEMAX_SessionManager.h"

void
InstantiateHda(HEMAX_Asset& Asset, int AssetIndex, HEMAX_Hda& Hda)
{
    Hda.HdaAsset = Asset;
    Hda.HdaAssetIndex = AssetIndex;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    CreateNode(Hda.MainNode, Asset.Names[AssetIndex], false);
    Cook(Hda.MainNode);

    if (Hda.MainNode.Type == HEMAX_NODE_OBJ)
    {
        Hda.ObjLevelHda = true;
        Hda.SopLevelHda = false;

        HEMAX_Object ObjectNode;
        if (GetObjectInfo(Hda.MainNode.Info.id, ObjectNode))
        {
            Hda.TopLevelObjectNode = ObjectNode;
            Hda.Name = ObjectNode.NodeName;
        }
    }
    else if (Hda.MainNode.Type = HEMAX_NODE_SOP)
    {
        Hda.ObjLevelHda = false;
        Hda.SopLevelHda = true;

        HEMAX_DisplayGeoNode SopNode;
        if (GetDisplayGeoNodeData(Hda.MainNode.Info.id, SopNode))
        {
            Hda.TopLevelSopNode = SopNode;

            HEMAX_NodeInfo ParentInfo;
            SM.Session->GetNodeInfo(Hda.MainNode.Info.parentId, &ParentInfo);
            Hda.Name = SM.Session->GetHAPIString(ParentInfo.nameSH);
        }
    }

    GetAllMaterialNodes(Hda);
    GetAllEditableNodes(Hda);
}

void
DeleteHda(HEMAX_Hda& Hda)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (Hda.MainNode.Type == HEMAX_NODE_OBJ)
    {
        SM.Session->DeleteNode(Hda.MainNode.Info.id);
    }
    else if (Hda.MainNode.Type == HEMAX_NODE_SOP)
    {
        SM.Session->DeleteNode(Hda.MainNode.Info.parentId);
    }
}

void
UpdateHda(HEMAX_Hda& Hda)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (Hda.MainNode.Type == HEMAX_NODE_OBJ)
    {
        UpdateObject(Hda.TopLevelObjectNode);
    }
    else if (Hda.MainNode.Type == HEMAX_NODE_SOP)
    {
        UpdateDisplayGeometryNode(Hda.MainNode.Info.id, Hda.TopLevelSopNode);
    }
}

void
GetAllMaterialNodes(HEMAX_Hda& Hda)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    int Count;
    SM.Session->ComposeChildNodeList(Hda.MainNode.Info.id, HEMAX_NODE_SHOP, HEMAX_NODEFLAG_ANY, true, &Count);

    std::vector<HEMAX_NodeId> ShopNodes(Count);
    SM.Session->GetComposedChildNodeList(Hda.MainNode.Info.id, ShopNodes.data(), Count);

    for (int i = 0; i < ShopNodes.size(); i++)
    {
        HEMAX_MaterialNode MatNode(ShopNodes[i]);
        Hda.ShopNodes.insert({ MatNode.NodeId, MatNode });
    }
}

void
GetAllEditableNodes(HEMAX_Hda& Hda)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    int Count;
    SM.Session->ComposeChildNodeList(Hda.MainNode.Info.id, HEMAX_NODE_SOP, HEMAX_NODEFLAG_EDITABLE, true, &Count);

    if (Count > 0)
    {
        std::vector<HEMAX_NodeId> EditableNodes(Count);
        SM.Session->GetComposedChildNodeList(Hda.MainNode.Info.id, &EditableNodes.front(), Count);

        for (int e = 0; e < Count; e++)
        {
            HEMAX_EditableNode EditableNode;
            EditableNode.Node.Type = HEMAX_NODE_SOP;
            SM.Session->GetNodeInfo(EditableNodes[e], &EditableNode.Node.Info);
            Cook(EditableNode.Node);

            if (SM.Session->GetGeometryInfo(EditableNode.Node.Info.id, &EditableNode.GeoInfo))
            {
                switch (EditableNode.GeoInfo.type)
                {
                case (HEMAX_GEOTYPE_CURVE):
                {
                    HEMAX_Part EditablePart;
                    for (int p = 0; p < EditableNode.GeoInfo.partCount; p++)
                    {
                        SM.Session->GetPartInfo(EditableNode.GeoInfo.nodeId, p, &EditablePart.Info);
                        if (EditablePart.Info.type == HEMAX_PARTTYPE_CURVE)
                        {
                            EditableNode.Parts.push_back(EditablePart);
                        }
                        else if (EditablePart.Info.type == HEMAX_PARTTYPE_MESH)
                        {
                            EditableNode.Parts.push_back(EditablePart);
                        }
                    }
                    Hda.EditableNodes.push_back(EditableNode);
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

HEMAX_EditableNode
FindEditableNodeFromName(HEMAX_Hda& Hda, std::string Name, bool& Success)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    for (int i = 0; i < Hda.EditableNodes.size(); i++)
    {
        std::string NodeName = SM.Session->GetHAPIString(Hda.EditableNodes[i].GeoInfo.nameSH);
        if (NodeName == Name)
        {
            Success = true;
            return Hda.EditableNodes[i];
        }
    }

    Success = false;
    HEMAX_EditableNode EmptyNode;
    return EmptyNode;
}