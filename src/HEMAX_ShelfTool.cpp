#include "HEMAX_ShelfTool.h"

#include <fstream>
#include <algorithm>

#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"
#include "HEMAX_Path.h"

#include <json.hpp>
using json = nlohmann::json;

HEMAX_ShelfTool::HEMAX_ShelfTool()
{
    Valid = false;
}

HEMAX_ShelfTool::~HEMAX_ShelfTool()
{

}

bool
HEMAX_ShelfTool::Init(std::string JsonFilePath, HEMAX_Store* Store)
{
    try
    {
	std::ifstream JsonFile(JsonFilePath);
	json JsonData;
	JsonFile >> JsonData;

	if (ParseJsonFile(JsonData, JsonFilePath))
	{
	    bool Success;

	    if (HEMAX_SessionManager::GetSessionManager().IsSessionActive())
	    {
		Store->LoadNewAsset(AssetPath, Success);
	    }
	    else
	    {
		Success = true;
	    }

	    return Success;
	}
	else
	{
	    return false;
	}
    }
    catch (...)
    {
	std::string WarnMsg = "Invalid shelf tool json found: [" +
                              JsonFilePath + "]: file has been skipped";
	HEMAX_Logger::Instance().AddEntry(WarnMsg.c_str(),
                                          HEMAX_LOG_LEVEL_WARN);

	return false;
    }
}

bool
HEMAX_ShelfTool::LoadAsset(HEMAX_Store* Store)
{
    if (Valid)
    {
	bool Success;
	Store->LoadNewAsset(AssetPath, Success);

	return Success;
    }

    return false;
}

bool
HEMAX_ShelfTool::ParseJsonFile(nlohmann::json &JsonData,
                               std::string ToolDefinitionPath)
{
    for (json::iterator It = JsonData.begin(); It != JsonData.end(); It++)
    {
	try
	{
	    std::vector<std::string> ToolTargets =
                JsonData.at("target").get<std::vector<std::string>>();

	    if (std::find(ToolTargets.begin(),
                          ToolTargets.end(), "all") == ToolTargets.end() &&
		std::find(ToolTargets.begin(),
                          ToolTargets.end(), "max") == ToolTargets.end())
	    {
		return false;
	    }

	    Name = JsonData.at("name").get<std::string>();

	    if (JsonData.find("toolTip") != JsonData.end())
	    {
		Description = JsonData.at("toolTip").get<std::string>();
	    }
	    else
	    {
		Description = "";
	    }

	    if (JsonData.find("iconPath") != JsonData.end())
	    {
		IconPath = JsonData.at("iconPath").get<std::string>();
		IconPath = HEMAX_Path::ResolvePathPrefixes(IconPath);

		if (HEMAX_Path::IsPathRelative(IconPath))
		{
		    IconPath = HEMAX_Path::GetAbsoluteFilePath(
                            HEMAX_Path::GetDirectory(ToolDefinitionPath),
                            IconPath);
		}
	    }
	    else
	    {
		IconPath = std::string(ToolDefinitionPath.begin(),
                                       ToolDefinitionPath.end() - 1);

		auto ExtensionBeg = IconPath.begin() + (IconPath.size() - 3);
		IconPath.replace(ExtensionBeg, IconPath.end(), "png");
	    }

	    if (JsonData.find("assetPath") != JsonData.end())
	    {
		AssetPath = JsonData.at("assetPath").get<std::string>();
		AssetPath = HEMAX_Path::ResolvePathPrefixes(AssetPath);

		if (HEMAX_Path::IsPathRelative(AssetPath))
		{
		    AssetPath = HEMAX_Path::GetAbsoluteFilePath(
                                    HEMAX_Path::GetDirectory(ToolDefinitionPath),
                                    AssetPath);
		}
	    }
	    else
	    {
                // Try .hda extension first
		AssetPath = std::string(ToolDefinitionPath.begin(),
                                        ToolDefinitionPath.end() - 1);

		auto ExtensionBeg = AssetPath.begin() + (AssetPath.size() - 3);
		AssetPath.replace(ExtensionBeg, AssetPath.end(), "hda");
		std::replace(AssetPath.begin(), AssetPath.end(), '\\', '/');

                bool Done = HEMAX_Path::DoesFileExist(AssetPath);

                // Try .otl extension next
		if (!Done)
		{
		    std::string OtlAssetPath = AssetPath;
		    ExtensionBeg = OtlAssetPath.begin() +
                                        (AssetPath.size() - 3);
		    OtlAssetPath.replace(ExtensionBeg,
                                         OtlAssetPath.end(),
                                         "otl");

                    Done = HEMAX_Path::DoesFileExist(OtlAssetPath);

		    if (Done)
		    {
			AssetPath = OtlAssetPath;
		    }
		}

                // Now try .hdalc extension
                if (!Done)
                {
                    std::string HdalcAssetPath = AssetPath;
                    ExtensionBeg = HdalcAssetPath.begin() +
                                   (AssetPath.size() - 3);
                    HdalcAssetPath.replace(ExtensionBeg,
                                           HdalcAssetPath.end(),
                                           "hdalc");

                    Done = HEMAX_Path::DoesFileExist(HdalcAssetPath);

                    if (Done)
                    {
                        AssetPath = HdalcAssetPath;
                    }
                }

                if (!Done)
                {
                    HEMAX_Logger::Instance().AddEntry(
                            "Could not find the HDA file for " + Name +
                            " shelf tool. Trying setting the 'assetPath' field"
                            " in the shelf tool definition file.",
                            HEMAX_LOG_LEVEL_WARN);
                }
	    }

	    if (JsonData.find("helpURL") != JsonData.end())
	    {
		HelpUrl = JsonData.at("helpURL").get<std::string>();
	    }
	    else
	    {
		HelpUrl = "";
	    }

	    if (JsonData.find("toolType") != JsonData.end())
	    {
		std::string Type = JsonData.at("toolType").get<std::string>();

		if (Type == "GENERATOR")
		{
		    ToolType = GENERATOR;
		}
		else if (Type == "OPERATOR_SINGLE")
		{
		    ToolType = OPERATOR_SINGLE;
		}
		else if (Type == "OPERATOR_MULTI")
		{
		    ToolType = OPERATOR_MULTI;
		}
		else if (Type == "BATCH")
		{
		    ToolType = BATCH;
		}
	    }
	    else
	    {
		ToolType = ANY;
	    }

	    Valid = true;
	    return true;
	}
	catch (...)
	{
	    return false;
	}
    }

    return false;
}
