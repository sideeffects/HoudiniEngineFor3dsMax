#include "HEMAX_Asset.h"

#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"

HEMAX_Asset::HEMAX_Asset(std::string Filepath)
{
    Id = -1;
    AssetCount = 0;
    Path = Filepath;
}

void
HEMAX_Asset::UpdateAssetDefinition()
{
    Names.clear();
    LoadAsset(true);
}

HEMAX_AssetLoadStatus
HEMAX_Asset::LoadAsset(bool AllowOverwrite)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_AssetLoadStatus LoadStatus;

    if (SM.Session->LoadHoudiniDigitalAsset(Path.c_str(), AllowOverwrite, &Id, &LoadStatus))
    {
	SM.Session->GetAvailableAssetCount(Id, &AssetCount);
	std::vector<HEMAX_StringHandle> AssetNamesSH(AssetCount);
	SM.Session->GetAvailableAssets(Id, &AssetNamesSH.front(), AssetCount);
	for (int i = 0; i < AssetCount; i++)
	{
	    Names.push_back(SM.Session->GetHAPIString(AssetNamesSH[i]));
	}	    
    }
    else
    {
	std::string Msg = std::string("Could not load asset <") + Path + ">"; 
	
	switch (LoadStatus)
	{
	    case HEMAX_ASSET_NOT_FOUND:
	    {
		Msg += "\nAsset could not be found on disk.";
	    } break;
	    case HEMAX_ASSET_ALREADY_LOADED:
	    {
		Msg += "\nAsset is already loaded.";
	    } break;
	    case HEMAX_ASSET_INVALID:
	    {
		Msg += "\nAsset is invalid";
	    } break;
	    default:
	    {
		// No additional logging for default
	    } break;
	}

	HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
    }

    return LoadStatus;
}
