#pragma once

#include "HEMAX_3dsMaxInput.h"
#include "HEMAX_3dsmaxHda.h"
#include "HEMAX_Asset.h"
#include <unordered_map>
#include <list>
#include <simpmod.h>

const char* const HEMAX_ENV_HDA_PATH = "HEMAX_HDA_PATH";

class HEMAX_Store
{
public:
    HEMAX_Store();
    ~HEMAX_Store();

    std::vector<std::string> GetListOfLoadedAssets();
    HEMAX_Asset* FindAsset(std::string AssetPath);
    std::string LoadNewAsset(std::string Path, bool& Success);
    void RemoveAllAssets();
    bool RemoveAsset(std::string AssetPath);

    void EmptyOutStore();
    void DeleteAll3dsMaxInputs();

    HEMAX_3dsMaxInput* Find3dsMaxInput(ULONG MaxNodeHandle);
    void Add3dsMaxInput(HEMAX_3dsMaxInput* Input);
    void Delete3dsMaxInput(ULONG MaxNodeHandle);

    void Add3dsmaxHda(ULONG NodeHandle, HEMAX_3dsmaxHda* Hda);

    HEMAX_3dsmaxHda* CreateGeometryHda(std::string Path, int AssetIndex);
    HEMAX_3dsmaxHda* CreateModifierHda(INode* Node, std::string Path, int AssetIndex);

    // Deletes the display geometries
    void Delete3dsmaxHda(ULONG NodeHandle);
    void Delete3dsmaxHda(ULONG NodeHandle, std::vector<ULONG>& ScheduledDeletionList);
    void Delete3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin);

    // Doesn't delete the display geometries (e.g. in the case of a modifier collapse. If you delete a modifier
    // when it's not 'really' supposed to be deleted (e.g. during a mod collapse) 3dsmax will most likely crash on you
    void Remove3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin);

    HEMAX_3dsmaxHda* Find3dsmaxHda(ULONG NodeHandle);
    HEMAX_3dsmaxHda* Find3dsmaxHda(HEMAX_Node Node);
    HEMAX_3dsmaxHda* Find3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin);

    HEMAX_3dsmaxHda* Find3dsmaxHdaViaChildGeometry(ULONG NodeHandle);

    std::vector<HEMAX_3dsmaxHda*> GetAllHdas();

    std::vector<HEMAX_3dsmaxHda*> FindAllHdasUsingAssetDefinition(HEMAX_Asset* Asset);

    std::string UserHdaRepository;

private:
    std::unordered_map<std::string, HEMAX_Asset> LoadedAssetLibraries;
    std::unordered_map<ULONG, HEMAX_3dsMaxInput*> InputStore;
    std::unordered_map<ULONG, HEMAX_3dsmaxHda*> MaxHdaStore;
    std::unordered_map<ULONG, std::list<HEMAX_3dsmaxHda*>> ModifierHdaStore;

    void DeleteMaxNode(ULONG MaxNodeHandle, std::vector<ULONG>& ScheduledDeletionList);
};
