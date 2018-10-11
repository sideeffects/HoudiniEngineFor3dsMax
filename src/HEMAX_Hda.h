#pragma once

#include "HEMAX_Asset.h"
#include "HEMAX_Node.h"
#include "HEMAX_Object.h"
#include "HEMAX_DisplayGeoNode.h"
#include "HEMAX_EditableNode.h"

#include <vector>

struct HEMAX_Hda
{
    HEMAX_Node MainNode;
    HEMAX_Object TopLevelObjectNode;
    HEMAX_DisplayGeoNode TopLevelSopNode;

    bool ObjLevelHda;
    bool SopLevelHda;

    HEMAX_Asset HdaAsset;
    int HdaAssetIndex;
    std::string Name;

    std::unordered_map<HEMAX_NodeId, HEMAX_MaterialNode> ShopNodes;
    std::vector<HEMAX_EditableNode> EditableNodes;
};

void
InstantiateHda(HEMAX_Asset& Asset, int AssetIndex, HEMAX_Hda& Hda);

void
DeleteHda(HEMAX_Hda& Hda);

void
UpdateHda(HEMAX_Hda& Hda);

static void
GetAllMaterialNodes(HEMAX_Hda& Hda);

static void
GetAllEditableNodes(HEMAX_Hda& Hda);

HEMAX_EditableNode
FindEditableNodeFromName(HEMAX_Hda& Hda, std::string Name, bool& Success);