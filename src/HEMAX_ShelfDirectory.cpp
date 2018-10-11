#include "HEMAX_ShelfDirectory.h"

HEMAX_ShelfDirectory::HEMAX_ShelfDirectory(std::string Dir, std::string ShelfName)
    : Directory(Dir), Name(ShelfName)
{
    DefaultShelf = false;
}

HEMAX_ShelfDirectory::~HEMAX_ShelfDirectory()
{

}

void
HEMAX_ShelfDirectory::AddShelfTool(const HEMAX_ShelfTool &Shelf)
{
    ShelfTools.push_back(Shelf);
}

std::vector<HEMAX_ShelfTool>*
HEMAX_ShelfDirectory::GetShelf()
{
    return &ShelfTools;
}

std::string
HEMAX_ShelfDirectory::GetDirectory()
{
    return Directory;
}

std::string
HEMAX_ShelfDirectory::GetName()
{
    return Name;
}

void
HEMAX_ShelfDirectory::LoadAllShelfAssets(HEMAX_Store* AssetStore)
{
    for (int i = 0; i < ShelfTools.size(); i++)
    {
        ShelfTools[i].LoadAsset(AssetStore);
    }
}

void
HEMAX_ShelfDirectory::MarkAsDefaultShelf()
{
    DefaultShelf = true;
}

bool
HEMAX_ShelfDirectory::IsDefaultShelf()
{
    return DefaultShelf;
}