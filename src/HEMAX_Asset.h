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
	HAPI_AssetLibraryId Id;
	int AssetCount;
	std::vector<std::string> Names;
};
