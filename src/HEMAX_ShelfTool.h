#pragma once

#include <string>

#include "HEMAX_Store.h"
#include <json.hpp>

enum ShelfToolType
{
    GENERATOR,
    OPERATOR_SINGLE,
    OPERATOR_MULTI,
    BATCH,
    ANY
};

struct HEMAX_ShelfTool
{
    public:
	HEMAX_ShelfTool();
	~HEMAX_ShelfTool();

	bool Init(std::string JsonFilePath, HEMAX_Store* Store);

	bool LoadAsset(HEMAX_Store* Store);

	bool Valid;

	std::string Name;
	ShelfToolType ToolType;
	std::string Description;
	std::string IconPath;
	std::string AssetPath;
	std::string HelpUrl;

    private:
	bool ParseJsonFile(nlohmann::json &JsonData,
                           std::string ToolDefinitionPath);
};
