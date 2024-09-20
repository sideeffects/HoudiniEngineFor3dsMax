#pragma once

#include "HEMAX_3dsMaxInput.h"
#include "HEMAX_Events.h"
#include "HEMAX_Shelf.h"

#include <list>
#include <unordered_map>

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <simpmod.h>
#pragma warning(pop)

class HEMAX_3dsmaxHda;
class HEMAX_GeometryHda;
class HEMAX_ModifierHda;
class HEMAX_UserPrefs;

const char* const HEMAX_ENV_HDA_PATH = "HEMAX_HDA_PATH";

class HEMAX_Store : public HEMAX_EventHandler
{
    public:
        HEMAX_Store(HEMAX_Events& EventSystem);
	~HEMAX_Store() = default;

        HEMAX_Shelf& GetShelf() { return ToolShelf; }

	std::vector<std::string> GetListOfLoadedAssets();
	HAPI_AssetLibraryId FindAsset(std::string AssetPath);
	std::string LoadNewAsset(std::string Path, bool& Success);
	bool RemoveAsset(std::string AssetPath);

        void LoadAssetsInHdaLoadPath();
	void LoadAllAssetsInDirectory(std::string Directory);

        std::string GetAssetPath(HAPI_AssetLibraryId AssetId);
	bool IsAssetLoaded(std::string Path);

        HAPI_AssetLibraryId UpdateAssetDefinition(const std::string& Asset);

        void DeleteStore();
	void DeleteAll3dsMaxInputs();

	HEMAX_3dsMaxInput* Find3dsMaxInput(ULONG MaxNodeHandle);
	void Add3dsMaxInput(HEMAX_3dsMaxInput* Input);
	void Delete3dsMaxInput(ULONG MaxNodeHandle);

	void Add3dsmaxHda(ULONG NodeHandle, HEMAX_3dsmaxHda* Hda);
	void AddModifier(ULONG NodeHandle, HEMAX_ModifierHda* Hda);

	HEMAX_GeometryHda* CreateGeometryHda(std::string Path,
                                             int AssetIndex);
	HEMAX_ModifierHda* CreateModifierHda(INode* Node, std::string Path, int AssetIndex);

	// Deletes the display geometries
	void Delete3dsmaxHda(ULONG NodeHandle);
	void Delete3dsmaxHda(ULONG NodeHandle, std::vector<ULONG>& ScheduledDeletionList);
	void Delete3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin);

	// Doesn't delete the display geometries
        // (e.g. in the case of a modifier collapse. If you delete a modifier
	// when it's not 'really' supposed to be deleted
        // (e.g. during a mod collapse) 3dsmax will most likely crash on you
	void Remove3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin);

	HEMAX_3dsmaxHda* Find3dsmaxHda(ULONG NodeHandle);
	HEMAX_3dsmaxHda* Find3dsmaxHda(HEMAX_Node Node);

	HEMAX_ModifierHda* Find3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin);

	HEMAX_GeometryHda* Find3dsmaxHdaViaChildGeometry(ULONG NodeHandle);

	HEMAX_GeometryHda* FindGeometryHda(ULONG NodeHandle);

	std::vector<HEMAX_3dsmaxHda*> GetAllHdas();
        void GetAllGeometryHdas(std::vector<HEMAX_GeometryHda*>& HdaList);

	std::vector<HEMAX_3dsmaxHda*> FindAllHdasUsingAssetDefinition(
                HAPI_AssetLibraryId AssetId);

	std::string UserHdaRepository;

    private:

        HEMAX_Shelf ToolShelf;

	std::unordered_map<std::string, HAPI_AssetLibraryId> LoadedAssetLibraries;
	std::unordered_map<ULONG, HEMAX_3dsMaxInput*> InputStore;
	std::unordered_map<ULONG, HEMAX_GeometryHda*> MaxHdaStore;
	std::unordered_map<ULONG, std::list<HEMAX_ModifierHda*>> ModifierHdaStore;

        HAPI_Result LoadAssetLibrary(const std::string& Path,
                                     bool AllowOverwrite,
                                     HAPI_AssetLibraryId& AssetLibId);
	void DeleteMaxNode(ULONG MaxNodeHandle, std::vector<ULONG>& ScheduledDeletionList);
};
