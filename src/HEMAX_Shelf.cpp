#include "HEMAX_Shelf.h"

#include <algorithm>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <json.hpp>

#include "HEMAX_Logger.h"
#include "HEMAX_ShelfTool.h"
#include "HEMAX_ShelfDirectory.h"

using json = nlohmann::json;

HEMAX_Shelf::HEMAX_Shelf(HEMAX_Store* Store)
{
    HdaStore = Store;
}

HEMAX_Shelf::~HEMAX_Shelf()
{

}

void
HEMAX_Shelf::AddShelfDirectory(const std::string& Dir, const std::string &Name, bool DefaultShelf)
{
    auto Search = Shelves.find(Dir);
    if (Search == Shelves.end())
    {
        HEMAX_ShelfDirectory ShelfDirectory(Dir, Name);

        DWORD DirResult = GetFileAttributesA(Dir.c_str());
        if (DirResult != INVALID_FILE_ATTRIBUTES && (DirResult & FILE_ATTRIBUTE_DIRECTORY))
        {
            std::string jsonPath = Dir + "\\*.json";

            WIN32_FIND_DATAA FileData;
            HANDLE ResultHandle = FindFirstFileA(jsonPath.c_str(), &FileData);

            if (ResultHandle != INVALID_HANDLE_VALUE)
            {
                std::string jsonFilePath = Dir + "\\" + FileData.cFileName;

                HEMAX_ShelfTool Tool = DeserializeJsonObj(jsonFilePath);
                if (Tool.Valid)
                {
                    ShelfDirectory.AddShelfTool(Tool);
                }

                while (FindNextFileA(ResultHandle, &FileData))
                {
                    jsonFilePath = Dir + "\\" + FileData.cFileName;
                    Tool = DeserializeJsonObj(jsonFilePath);
                    if (Tool.Valid)
                    {
                        ShelfDirectory.AddShelfTool(Tool);
                    }
                }
            }
        }

        if (DefaultShelf)
        {
            ShelfDirectory.MarkAsDefaultShelf();
        }

        Shelves.insert({ Dir, ShelfDirectory });
    }
}

void
HEMAX_Shelf::RemoveShelfDirectory(const std::string& Dir)
{
    auto DirLocation = Shelves.find(Dir);

    if (DirLocation != Shelves.end())
    {
        Shelves.erase(DirLocation);
    }
}

std::vector<std::string>
HEMAX_Shelf::GetShelfDirectories()
{
    std::vector<std::string> ShelfDirectories;
    for (auto It = Shelves.begin(); It != Shelves.end(); It++)
    {
        ShelfDirectories.push_back(It->first);
    }
    return ShelfDirectories;
}

HEMAX_ShelfDirectory
HEMAX_Shelf::GetShelfDirectory(std::string Dir)
{
    auto Search = Shelves.find(Dir);
    if (Search != Shelves.end())
    {
        return Search->second;
    }

    return HEMAX_ShelfDirectory();
}

std::vector<HEMAX_ShelfTool>*
HEMAX_Shelf::GetShelf(std::string ShelfDir)
{
    auto Search = Shelves.find(ShelfDir);
    if (Search != Shelves.end())
    {
        return Search->second.GetShelf();
    }
    else
    {
        return nullptr;
    }
}

void
HEMAX_Shelf::SaveShelfToJson(std::string Dir)
{
    std::string Path = Dir + "\\" + HEMAX_Shelf_SaveFilename;
    std::ofstream JsonFile(Path, std::ios::out);

    if (JsonFile.is_open())
    {
        json JsonObj;
        json ShelfDirectories;
        json ShelfDir;

        for (auto It = Shelves.begin(); It != Shelves.end(); It++)
        {
            if (!It->second.IsDefaultShelf())
            {
                ShelfDir["path"] = It->second.GetDirectory();
                ShelfDir["name"] = It->second.GetName();

                ShelfDirectories.push_back(ShelfDir);
            }
        }

        JsonObj["shelves"] = ShelfDirectories;

        JsonFile << JsonObj.dump(4) << std::endl;
    }
    else
    {
        std::string Warning = "Attempting to save Houdini Engine shelf information to [" + Path + "], but could not open the file";
        HEMAX_Logger::Instance().AddEntry(Warning, HEMAX_LOG_LEVEL_WARN);
    }
}

void
HEMAX_Shelf::LoadShelvesFromJson(std::string Dir)
{
    std::string Path = Dir + "\\" + HEMAX_Shelf_SaveFilename;
    std::ifstream JsonFile(Path);

    if (JsonFile.is_open())
    {
        json JsonData;
        
        try
        {
            JsonFile >> JsonData;
            json ShelfObj = JsonData.at("shelves");

            for (json::iterator It = ShelfObj.begin(); It != ShelfObj.end(); It++)
            {
                std::string ShelfName = (*It).at("name").get<std::string>();
                std::string ShelfDir = (*It).at("path").get<std::string>();

                AddShelfDirectory(ShelfDir, ShelfName);
            }
        }
        catch (...)
        {
            std::string Warning = "Problem encountered while attempting to deserialize [" + Path + "]";
            HEMAX_Logger::Instance().AddEntry(Warning, HEMAX_LOG_LEVEL_WARN);
        }
    }
    else
    {
        std::string Warning = "Attempting to read Houdini Engine shelf information from [" + Path + "], but could not open the file";
        HEMAX_Logger::Instance().AddEntry(Warning, HEMAX_LOG_LEVEL_WARN);
    }
}

void
HEMAX_Shelf::LoadToolAssets()
{
    for (auto It = Shelves.begin(); It != Shelves.end(); It++)
    {
        It->second.LoadAllShelfAssets(HdaStore);
    }
}

HEMAX_ShelfTool
HEMAX_Shelf::DeserializeJsonObj(std::string JsonFilePath)
{
    HEMAX_ShelfTool Tool;
    Tool.Init(JsonFilePath, HdaStore);
    return Tool;
}