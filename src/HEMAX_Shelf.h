#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "HEMAX_Store.h"

const char * const HEMAX_Shelf_SaveFilename = "hemax_shelves.json";

struct HEMAX_ShelfTool;
class HEMAX_ShelfDirectory;

class HEMAX_Shelf
{
public:
    HEMAX_Shelf(HEMAX_Store* Store);
    ~HEMAX_Shelf();

    void AddShelfDirectory(const std::string& Dir, const std::string& Name, bool DefaultShelf = false);
    void RemoveShelfDirectory(const std::string& Dir);

    std::vector<std::string> GetShelfDirectories();
    HEMAX_ShelfDirectory GetShelfDirectory(std::string Dir);
    std::vector<HEMAX_ShelfTool>* GetShelf(std::string ShelfDir);

    void SaveShelfToJson(std::string Dir);
    void LoadShelvesFromJson(std::string Dir);

    void LoadToolAssets();

private:
    std::unordered_map<std::string, HEMAX_ShelfDirectory> Shelves;

    HEMAX_Store* HdaStore;

    HEMAX_ShelfTool DeserializeJsonObj(std::string JsonFilePath);
};