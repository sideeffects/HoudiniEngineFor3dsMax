#pragma once

#include <HAPI.h>
#include <string>
#include <vector>

#include "HEMAX_Types.h"

struct HEMAX_Asset
{
    std::string Path;
    HEMAX_AssetId Id;
    int AssetCount;
    std::vector<std::string> Names;
};

bool
LoadAsset(HEMAX_Asset& Asset, std::string Path, HEMAX_AssetLoadStatus* LoadStatus = nullptr);

void
UpdateAssetDefinition(HEMAX_Asset& Asset, std::string Path, HEMAX_AssetLoadStatus* LoadStatus = nullptr);
