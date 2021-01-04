#pragma once

#include "HEMAX_ShelfTool.h"
#include "HEMAX_Store.h"

#include <string>
#include <vector>

class HEMAX_ShelfDirectory
{
    public:
	HEMAX_ShelfDirectory() = default;
	HEMAX_ShelfDirectory(std::string Dir, std::string ShelfName);
	~HEMAX_ShelfDirectory();

	void AddShelfTool(const HEMAX_ShelfTool &Shelf);
	std::vector<HEMAX_ShelfTool>* GetShelf();

	std::string GetDirectory();
	std::string GetName();

	void LoadAllShelfAssets(HEMAX_Store* AssetStore);

	void MarkAsDefaultShelf();
	bool IsDefaultShelf();

    private:
	std::string Directory;
	std::string Name;

	std::vector<HEMAX_ShelfTool> ShelfTools;

	bool DefaultShelf;
};
