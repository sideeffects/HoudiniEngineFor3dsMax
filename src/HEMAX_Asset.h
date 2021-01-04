#pragma once

#include <string>
#include <vector>

#include "HEMAX_Types.h"

class HEMAX_Asset
{
    public:
	HEMAX_Asset() = default;
	HEMAX_Asset(std::string Filepath);

	void UpdateAssetDefinition();

	HEMAX_AssetLoadStatus LoadAsset(bool AllowOverwrite = false);

	std::string Path;
	HEMAX_AssetId Id;
	int AssetCount;
	std::vector<std::string> Names;
};
