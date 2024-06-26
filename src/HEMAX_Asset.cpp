#include "HEMAX_Asset.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Utilities.h"

#include <sstream>
#include <string>
#include <vector>

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

HAPI_Result
HEMAX_Asset::LoadAsset(bool AllowOverwrite)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HAPI_Result Result = HEMAX_HoudiniApi::LoadAssetLibraryFromFile(
        &SM.Session, Path.c_str(), AllowOverwrite, &Id);

    if (Result == HAPI_RESULT_SUCCESS)
    {
        HEMAX_HoudiniApi::GetAvailableAssetCount(&SM.Session, Id, &AssetCount);
        std::vector<HAPI_StringHandle> AssetNameStringHandles(AssetCount);
        HEMAX_HoudiniApi::GetAvailableAssets(&SM.Session, Id,
            &AssetNameStringHandles.front(), AssetCount);
        for (int i = 0; i < AssetCount; ++i)
            Names.push_back(HEMAX_Utilities::GetHAPIString(AssetNameStringHandles[i]));    
    }
    else
    {
        std::stringstream sstream;
        sstream << "Could not load asset at path <" << Path << "> because ";     

        if (Result == HAPI_RESULT_CANT_LOADFILE)
            sstream << "the file could not be found on disk.";
        else if (Result == HAPI_RESULT_ASSET_DEF_ALREADY_LOADED)
            sstream << "the asset has already been loaded into the session.";
        else if (Result == HAPI_RESULT_ASSET_INVALID)
            sstream << "the asset is invalid.";
        else
            sstream << "of an unknown reason.";

        std::string ErrorMsg = sstream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_WARN);
    }
    
    return Result;
}
