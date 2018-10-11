#include "HEMAX_Asset.h"
#include "HEMAX_SessionManager.h"

bool
LoadAsset(HEMAX_Asset& Asset, std::string Path, HEMAX_AssetLoadStatus* LoadStatus)
{
    Asset.Path = Path;

    std::replace(Path.begin(), Path.end(), '\\', '/');
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.Session->LoadHoudiniDigitalAsset(Path.c_str(), false, &Asset.Id, LoadStatus))
    {
        if (SM.Session->GetAvailableAssetCount(Asset.Id, &Asset.AssetCount))
        {
            HEMAX_StringHandle* AssetNameStringHandles = new HEMAX_StringHandle[Asset.AssetCount];

            if (SM.Session->GetAvailableAssets(Asset.Id, AssetNameStringHandles, Asset.AssetCount))
            {
                for (int i = 0; i < Asset.AssetCount; i++)
                {
                    Asset.Names.push_back(SM.Session->GetHAPIString(AssetNameStringHandles[i]));
                }

                delete[] AssetNameStringHandles;

                return true;
            }
        }
    }

    return false;
}

void
UpdateAssetDefinition(HEMAX_Asset &Asset, std::string Path, HEMAX_AssetLoadStatus* LoadStatus)
{
    Asset.Names.clear();
    LoadAsset(Asset, Path, LoadStatus);
}
