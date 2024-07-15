#pragma once

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

	void Init(const std::string& OpName,
                  const std::string& AssetPath,
                  const int SelectedAssetIndex);
	void Delete();
	void Update();

        const std::string& GetAssetPath() { return StoreAssetPath; }
        int GetAssetIndex() { return AssetIndex; }

	HEMAX_EditableNode FindEditableNodeFromName(std::string Name,
						    bool& Success);

	void UpdateShopNodes();

        bool HasTimeDependentNodes();

	std::vector<HEMAX_DisplayGeoNode*> AllDisplayNodes();

	HEMAX_HdaLevel HdaType;

	HEMAX_Node MainNode;
	HEMAX_Object TopLevelObjectNode;
	HEMAX_DisplayGeoNode TopLevelSopNode;

	std::string Name;

	std::unordered_map<HAPI_NodeId, HEMAX_MaterialNode> ShopNodes;
	std::vector<HEMAX_EditableNode> EditableNodes;

	std::unordered_map<std::string, HEMAX_Parameter> Parameters;

    private:
	void InitializeMaterialNodes();
	void GetAllEditableNodes();
        void CheckForTimeDependentNodes();

        bool MyHasTimeDependentNodes;

        std::string StoreAssetPath;
        int         AssetIndex = -1;
};
