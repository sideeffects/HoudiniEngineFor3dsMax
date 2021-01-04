#pragma once

#include "HEMAX_Asset.h"
#include "HEMAX_Node.h"
#include "HEMAX_Object.h"
#include "HEMAX_DisplayGeoNode.h"
#include "HEMAX_EditableNode.h"
#include "HEMAX_Parameter.h"
#include "HEMAX_Types.h"

#include <vector>
#include <unordered_map>

class HEMAX_Hda
{
    public:
	HEMAX_Hda();

	void Init(const HEMAX_Asset& Asset, int AssetIndex);
	void Delete();
	void Update();

	HEMAX_EditableNode FindEditableNodeFromName(std::string Name,
						    bool& Success);

	void UpdateShopNodes();

        bool HasTimeDependentNodes();

	std::vector<HEMAX_DisplayGeoNode*> AllDisplayNodes();

	HEMAX_HdaLevel HdaType;

	HEMAX_Node MainNode;
	HEMAX_Object TopLevelObjectNode;
	HEMAX_DisplayGeoNode TopLevelSopNode;

	HEMAX_Asset HdaAsset;
	int HdaAssetIndex;
	std::string Name;

	std::unordered_map<HEMAX_NodeId, HEMAX_MaterialNode> ShopNodes;
	std::vector<HEMAX_EditableNode> EditableNodes;

	std::unordered_map<std::string, HEMAX_Parameter> Parameters;

    private:
	void InitializeMaterialNodes();
	void GetAllEditableNodes();
        void CheckForTimeDependentNodes();

        bool MyHasTimeDependentNodes;
};
