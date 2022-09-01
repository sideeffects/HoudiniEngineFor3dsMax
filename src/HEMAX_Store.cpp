#include "HEMAX_Store.h"

#include "HEMAX_3dsmaxHda.h"
#include "HEMAX_Logger.h"
#include "HEMAX_GeometryHda.h"
#include "HEMAX_Modifier.h"
#include "HEMAX_ModifierHda.h"
#include <icustattribcontainer.h>

#include <QtCore\qfileinfo.h>
#include <QtCore\qdir.h>

std::vector<std::string>
HEMAX_Store::GetListOfLoadedAssets()
{
    std::vector<std::string> Paths;
    Paths.reserve(LoadedAssetLibraries.size());

    for (auto Key : LoadedAssetLibraries)
    {
	Paths.push_back(Key.first);
    }

    return Paths;
}

HEMAX_Asset*
HEMAX_Store::FindAsset(std::string AssetPath)
{
    auto Search = LoadedAssetLibraries.find(AssetPath);

    if (Search != LoadedAssetLibraries.end())
	return &Search->second;

    return nullptr;
}

std::string
HEMAX_Store::LoadNewAsset(std::string Path, bool& Success)
{
    HEMAX_Asset NewAsset(Path);
    HEMAX_AssetLoadStatus AssetLoadStatus;

    // First try the path that is given
    AssetLoadStatus = NewAsset.LoadAsset();

    if (AssetLoadStatus == HEMAX_ASSET_NO_STATUS)
    {
	std::replace(NewAsset.Path.begin(), NewAsset.Path.end(), '\\', '/');
	LoadedAssetLibraries.insert({ NewAsset.Path, NewAsset });
	Success = true;
	return Path;
    }

    if (AssetLoadStatus != HEMAX_ASSET_ALREADY_LOADED)
    {
	// Try HEMAX_HDA_PATH first
	std::string HdaPath = HEMAX_Utilities::GetEnvVar(HEMAX_ENV_HDA_PATH);

	if (!HdaPath.empty())
	{
	    QString PathString(HdaPath.c_str());
	    QFileInfo HdaFileInfo(Path.c_str());
	    QString HdaFile = HdaFileInfo.fileName();

	    QString NewHdaFilePath = PathString.append("/") + HdaFile;

	    NewAsset.Path = NewHdaFilePath.toStdString();
	    HEMAX_AssetLoadStatus LoadSt = NewAsset.LoadAsset();

	    if (LoadSt == HEMAX_ASSET_NO_STATUS)
	    {
		std::replace(NewAsset.Path.begin(), NewAsset.Path.end(), '\\', '/');
		LoadedAssetLibraries.insert({ NewAsset.Path, NewAsset });
		Success = true;
		return NewHdaFilePath.toStdString();
	    }
	}

	// If it was not found, next try the user's HDA repository directory
	if (!UserHdaRepository.empty() && AssetLoadStatus != HEMAX_ASSET_ALREADY_LOADED)
	{
	    QString PathString(UserHdaRepository.c_str());
	    QFileInfo HdaFileInfo(Path.c_str());
	    QString HdaFile = HdaFileInfo.fileName();

	    QString NewHdaFilePath = PathString.append("/") + HdaFile;

	    NewAsset.Path = NewHdaFilePath.toStdString();
	    HEMAX_AssetLoadStatus LoadSt = NewAsset.LoadAsset();

	    if (LoadSt == HEMAX_ASSET_NO_STATUS)
	    {
		std::replace(NewAsset.Path.begin(), NewAsset.Path.end(), '\\', '/');
		LoadedAssetLibraries.insert({ NewAsset.Path, NewAsset });
		Success = true;
		return NewHdaFilePath.toStdString();
	    }
	}

	if (AssetLoadStatus != HEMAX_ASSET_ALREADY_LOADED)
	{
	    // Last, try the directory where the current 3dsmax scene is saved
	    std::wstring CurFilePathString(GetCOREInterface()->GetCurFilePath());
	    std::string CurFilePathStringA(CurFilePathString.begin(), CurFilePathString.end());

	    if (!CurFilePathStringA.empty())
	    {
		QFileInfo CurFileInfo(CurFilePathStringA.c_str());
		QString PathString = CurFileInfo.dir().absolutePath();

		QFileInfo HdaFileInfo(Path.c_str());
		QString HdaFile = HdaFileInfo.fileName();

		QString NewHdaFilePath = PathString.append("/") + HdaFile;

		NewAsset.Path = NewHdaFilePath.toStdString();
		HEMAX_AssetLoadStatus LoadSt = NewAsset.LoadAsset();

		if (LoadSt == HEMAX_ASSET_NO_STATUS)
		{
		    std::replace(NewAsset.Path.begin(), NewAsset.Path.end(), '\\', '/');
		    LoadedAssetLibraries.insert({ NewAsset.Path, NewAsset });
		    Success = true;
		    return NewHdaFilePath.toStdString();
		}
	    }
	    else
	    {
		std::string LogMsg = "Failed to load asset <" + Path + ">: ";

		if (AssetLoadStatus == HEMAX_ASSET_ALREADY_LOADED)
		{
		    LogMsg += "ASSET ALREADY LOADED";
		}
		else if (AssetLoadStatus == HEMAX_ASSET_NOT_FOUND)
		{
		    LogMsg += "ASSET FILE NOT FOUND";
		}
		else if (AssetLoadStatus == HEMAX_ASSET_INVALID)
		{
		    LogMsg += "ASSET INVALID";
		}

		HEMAX_Logger::Instance().AddEntry(LogMsg, HEMAX_LOG_LEVEL_WARN);
		Success = false;
		return "";
	    }
	}
    }

    Success = false;
    return "";
}

void
HEMAX_Store::RemoveAllAssets()
{
    LoadedAssetLibraries.clear();
}

bool
HEMAX_Store::RemoveAsset(std::string AssetPath)
{
    HEMAX_Asset* Asset = FindAsset(AssetPath);

    if (Asset)
    {
	std::vector<HEMAX_3dsmaxHda*> Hdas = GetAllHdas();
	for (int i = 0; i < Hdas.size(); i++)
	{
	    if (Hdas[i]->Hda.HdaAsset.Id == Asset->Id)
	    {
		return false;
	    }
	}

	LoadedAssetLibraries.erase(AssetPath);
	return true;
    }
    else
    {
        std::string Msg = "HDA with path <" + AssetPath + "> has already been "
            "removed.";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
    }

    return false;
}

void
HEMAX_Store::LoadAllAssetsInDirectory(std::string Directory)
{
    if (!Directory.empty())
    {
	DWORD DirResult = GetFileAttributesA(Directory.c_str());

	if (DirResult != INVALID_FILE_ATTRIBUTES && (DirResult & FILE_ATTRIBUTE_DIRECTORY))
	{
	    std::string PathHDA = Directory + "\\*.hda";
	    std::string PathOTL = Directory + "\\*.otl";

	    WIN32_FIND_DATAA FileData;
	    HANDLE ResultHandle = FindFirstFileA(PathHDA.c_str(), &FileData);

	    if (ResultHandle != INVALID_HANDLE_VALUE)
	    {
		std::string FilePath(Directory + "\\" + FileData.cFileName);
		bool Success;
		LoadNewAsset(FilePath, Success);

		while (FindNextFileA(ResultHandle, &FileData))
		{
		    std::string FilePath(Directory + "\\" + FileData.cFileName);
		    LoadNewAsset(FilePath, Success);
		}
	    }

	    ResultHandle = FindFirstFileA(PathOTL.c_str(), &FileData);

	    if (ResultHandle != INVALID_HANDLE_VALUE)
	    {
		std::string FilePath(Directory + "\\" + FileData.cFileName);
		bool Success;
		LoadNewAsset(FilePath, Success);

		while (FindNextFileA(ResultHandle, &FileData))
		{
		    std::string FilePath(Directory + "\\" + FileData.cFileName);
		    LoadNewAsset(FilePath, Success);
		}
	    }
	}
    } 
}

bool
HEMAX_Store::IsAssetLoaded(std::string Path)
{
    auto Search = LoadedAssetLibraries.find(Path);

    if (Search != LoadedAssetLibraries.end())
    {
	return true;
    }
    else
    {
	return false;
    }
}

void
HEMAX_Store::EmptyOutStore()
{
    RemoveAllAssets();
    DeleteAll3dsMaxInputs();

    for (auto It = MaxHdaStore.begin(); It != MaxHdaStore.end(); It++)
    {
	if (It->second)
	{
	    HEMAX_GeometryHda* GeomHda = It->second;
	    MaxHdaStore.erase(It);
	    GeomHda->Hda.Delete();
	    delete GeomHda;
	    GeomHda = nullptr;
	}
    }

    for (auto It = ModifierHdaStore.begin(); It != ModifierHdaStore.end(); It++)
    {
	for (auto ListIt = It->second.begin(); ListIt != It->second.end(); ListIt++)
	{
	    if ((*ListIt))
	    {
		(*ListIt)->DeleteMainInput();
		(*ListIt)->Hda.Delete();
		delete (*ListIt);
	    }
	}
    }

    ModifierHdaStore.clear();
}

void
HEMAX_Store::DeleteAll3dsMaxInputs()
{
    for (auto It = InputStore.begin(); It != InputStore.end(); It++)
    {
	if (It->second)
	{
	    delete It->second;
	}
    }

    InputStore.clear();
}

HEMAX_3dsMaxInput*
HEMAX_Store::Find3dsMaxInput(ULONG MaxNodeHandle)
{
    auto Search = InputStore.find(MaxNodeHandle);

    if (Search != InputStore.end())
    {
	return Search->second;
    }

    return nullptr;
}

void
HEMAX_Store::Add3dsMaxInput(HEMAX_3dsMaxInput* Input)
{
    if (!Find3dsMaxInput(Input->Get3dsMaxNodeHandle()))
    {
	InputStore.insert({ Input->Get3dsMaxNodeHandle(), Input });
    }
    else
    {
	HEMAX_Logger::Instance().AddEntry("HEMAX_Store: trying to add 3dsMaxInput to InputStore, but there is already an entry for that node.", HEMAX_LOG_LEVEL_ERROR);
    }
}

void
HEMAX_Store::Delete3dsMaxInput(ULONG MaxNodeHandle)
{
    HEMAX_3dsMaxInput* ToBeDeleted = Find3dsMaxInput(MaxNodeHandle);

    if (ToBeDeleted)
    {
	delete ToBeDeleted;
	ToBeDeleted = nullptr;
	InputStore.erase(MaxNodeHandle);
    }
}

void
HEMAX_Store::Add3dsmaxHda(ULONG NodeHandle, HEMAX_3dsmaxHda* Hda)
{
    if (Hda->Type == HEMAX_GEOMETRY_HDA)
    {
	MaxHdaStore.insert({ NodeHandle, static_cast<HEMAX_GeometryHda*>(Hda) });
    }
    else if (Hda->Type == HEMAX_MODIFIER_HDA)
    {
	AddModifier(NodeHandle, static_cast<HEMAX_ModifierHda*>(Hda));	
    }
}

void
HEMAX_Store::AddModifier(ULONG NodeHandle, HEMAX_ModifierHda* Hda)
{
    auto Search = ModifierHdaStore.find(NodeHandle);

    if (Search != ModifierHdaStore.end())
    {
	Search->second.push_back(Hda);
    }
    else
    {
	std::list<HEMAX_ModifierHda*> NewList;
	NewList.push_back(Hda);
	ModifierHdaStore.insert({ NodeHandle, NewList });
    }
}

HEMAX_GeometryHda*
HEMAX_Store::CreateGeometryHda(std::string Path,
                               int AssetIndex)
{
    HEMAX_GeometryHda* GeoHda = nullptr;
    HEMAX_Asset* TheAsset = FindAsset(Path);

    if (TheAsset)
    {
	GeoHda = new HEMAX_GeometryHda();
	GeoHda->Create(*TheAsset, AssetIndex);

	if (GeoHda->ContainerNode)
	{
	    Add3dsmaxHda(GeoHda->ContainerNode->GetHandle(), GeoHda);
	}
	else
	{
	    delete GeoHda;
	    GeoHda = nullptr;
	}
    }
    else
    {
        std::string Msg = "HDA with path <" + Path + "> could not be created "
            "because it has not been loaded.";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR); 
    }

    return GeoHda;
}

HEMAX_ModifierHda*
HEMAX_Store::CreateModifierHda(INode* Node, std::string Path, int AssetIndex)
{
    HEMAX_ModifierHda* ModifierHda = nullptr;
    HEMAX_Asset* TheAsset = FindAsset(Path);

    if (TheAsset)
    {
	HEMAX_Modifier* ModifierPlugin =
            (HEMAX_Modifier*)GetCOREInterface()->CreateInstance(OSM_CLASS_ID,
                    HEMAX_Modifier_CLASS_ID);
	GetCOREInterface12()->AddModifier((*Node), (*ModifierPlugin));

	ModifierHda = new HEMAX_ModifierHda;

	if (ModifierHda->Create(*TheAsset, AssetIndex, ModifierPlugin, Node))
	{
	    Add3dsmaxHda(Node->GetHandle(), ModifierHda);
	    return ModifierHda;
	}
	else
	{
	    GetCOREInterface7()->DeleteModifier((*Node), (*ModifierPlugin));
	    delete ModifierPlugin;
            delete ModifierHda;
	}
    }
    else
    {
        std::string Msg = "HDA with path <" + Path + "> could not be created "
            "because it has not been loaded.";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR);
    }

    return nullptr;
}

void
HEMAX_Store::Delete3dsmaxHda(ULONG NodeHandle)
{
    HEMAX_3dsmaxHda* Hda = Find3dsmaxHda(NodeHandle);

    if (Hda)
    {
	if (Hda->Type == HEMAX_GEOMETRY_HDA)
	{
	    HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(Hda);
	    GeometryHda->ClearAnyInstances();
	    GetCOREInterface()->DeleteNode(GeometryHda->ContainerNode);
	}

	Hda->Hda.Delete();
	delete Hda;
	MaxHdaStore.erase(NodeHandle);
    }
}

void
HEMAX_Store::Delete3dsmaxHda(ULONG NodeHandle, std::vector<ULONG>& ScheduledDeletionList)
{
    HEMAX_3dsmaxHda* Hda = Find3dsmaxHda(NodeHandle);

    if (Hda)
    {
	if (Hda->Type == HEMAX_GEOMETRY_HDA)
	{
	    HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(Hda);

	    for (int i = 0; i < GeometryHda->InstanceClones.size(); i++)
	    {
		DeleteMaxNode(GeometryHda->InstanceClones[i]->GetHandle(), ScheduledDeletionList);
	    }

            for (auto It = GeometryHda->PackedPrimClones.begin(); It != GeometryHda->PackedPrimClones.end(); It++)
            {
                DeleteMaxNode(It->first->GetHandle(), ScheduledDeletionList);
            }

	    bool ContainerNodeAlreadyBeingDeleted = false;
	    for (int s = 0; s < ScheduledDeletionList.size(); s++)
	    {
		if (GeometryHda->ContainerNode->GetHandle() == ScheduledDeletionList[s])
		{
		    ContainerNodeAlreadyBeingDeleted = true;
		    break;
		}
	    }

	    if (!ContainerNodeAlreadyBeingDeleted)
	    {
		GetCOREInterface()->DeleteNode(GeometryHda->ContainerNode);
	    }
	}

	Hda->Hda.Delete();

	delete Hda;
	MaxHdaStore.erase(NodeHandle);
    }
}

void
HEMAX_Store::Delete3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin)
{
    auto Search = ModifierHdaStore.find(NodeHandle);

    if (Search != ModifierHdaStore.end())
    {
	for (auto ListIt = Search->second.begin(); ListIt != Search->second.end(); ListIt++)
	{
	    if ((*ListIt)->DisplayGeometry == (void*)ModifierPlugin)
	    {
		delete (*ListIt)->DisplayGeometry;
		(*ListIt)->DisplayGeometry = nullptr;
		(*ListIt)->Hda.Delete();
		delete (*ListIt);

		Search->second.erase(ListIt);

		if (Search->second.size() <= 0)
		{
		    ModifierHdaStore.erase(Search);
		}
	    }
	}
    }
}

void
HEMAX_Store::Remove3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin)
{
    auto Search = ModifierHdaStore.find(NodeHandle);

    if (Search != ModifierHdaStore.end())
    {
	for (auto ListIt = Search->second.begin(); ListIt != Search->second.end(); ListIt++)
	{
	    HEMAX_Modifier* ModifierHdaPlugin = static_cast<HEMAX_Modifier*>(ModifierPlugin);
	    if ((*ListIt)->Hda.MainNode.Info.id == ModifierHdaPlugin->GetHda()->MainNode.Info.id)
	    {
		(*ListIt)->DisplayGeometry = nullptr;
		(*ListIt)->Hda.Delete();
		delete (*ListIt);

		Search->second.erase(ListIt);

		if (Search->second.size() <= 0)
		{
		    ModifierHdaStore.erase(Search);
		}
	    }
	}
    }
}

HEMAX_3dsmaxHda*
HEMAX_Store::Find3dsmaxHda(ULONG NodeHandle)
{
    auto Search = MaxHdaStore.find(NodeHandle);

    if (Search != MaxHdaStore.end())
    {
	return Search->second;
    }

    return nullptr;
}

HEMAX_3dsmaxHda*
HEMAX_Store::Find3dsmaxHda(HEMAX_Node Node)
{
    for (auto It = MaxHdaStore.begin(); It != MaxHdaStore.end(); It++)
    {
	if (It->second->Hda.MainNode.Info.id == Node.Info.id)
	{
	    return It->second;
	}
    }

    for (auto It = ModifierHdaStore.begin(); It != ModifierHdaStore.end(); It++)
    {
	for (auto ListIt = It->second.begin(); ListIt != It->second.end(); ListIt++)
	{
	    if ((*ListIt) && (*ListIt)->Hda.MainNode.Info.id == Node.Info.id)
	    {
		return (*ListIt);
	    }
	}
    }

    return nullptr;
}

HEMAX_ModifierHda*
HEMAX_Store::Find3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin)
{
    auto Search = ModifierHdaStore.find(NodeHandle);

    if (Search != ModifierHdaStore.end())
    {
	for (auto It = Search->second.begin(); It != Search->second.end(); It++)
	{
	    if ((*It)->DisplayGeometry == (void*)ModifierPlugin)
	    {
		return static_cast<HEMAX_ModifierHda*>((*It));
	    }
	}
    }

    return nullptr;
}

HEMAX_GeometryHda*
HEMAX_Store::Find3dsmaxHdaViaChildGeometry(ULONG NodeHandle)
{
    INode* MaxNode = GetCOREInterface()->GetINodeByHandle(NodeHandle);

    ICustAttribContainer* CustomAttributes = MaxNode->GetCustAttribContainer();

    if (CustomAttributes)
    {
	if (CustomAttributes->GetNumCustAttribs() > HEMAX_MAX_GEO_MAX_INDEX)
	{
	    CustAttrib* StampLookup = CustomAttributes->GetCustAttrib(HEMAX_MAX_GEO_STAMP_INDEX);
	    std::wstring CustAttribName = StampLookup->GetName();
	    std::string Name = std::string(CustAttribName.begin(), CustAttribName.end());
	    std::string GeoStampName = std::string(HEMAX_MAX_GEO_STAMP_NAME);

	    if (Name == GeoStampName)
	    {
		INode* ParentNode = MaxNode->GetParentNode();

		if (ParentNode)
		{
		    return FindGeometryHda(ParentNode->GetHandle());
		}
	    }
	}
    }

    return nullptr;
}

HEMAX_GeometryHda*
HEMAX_Store::FindGeometryHda(ULONG NodeHandle)
{
    auto Search = MaxHdaStore.find(NodeHandle);

    if (Search != MaxHdaStore.end())
    {
	return Search->second;
    }

    return nullptr;
}

std::vector<HEMAX_3dsmaxHda*>
HEMAX_Store::GetAllHdas()
{
    std::vector<HEMAX_3dsmaxHda*> Hdas;
    for (auto It = MaxHdaStore.begin(); It != MaxHdaStore.end(); It++)
    {
	if (It->second)
	{
	    Hdas.push_back(It->second);
	}
    }

    for (auto It = ModifierHdaStore.begin(); It != ModifierHdaStore.end(); It++)
    {
	for (auto ListIt = It->second.begin(); ListIt != It->second.end(); ListIt++)
	{
	    if ((*ListIt))
	    {
		Hdas.push_back((*ListIt));
	    }
	}
    }

    return Hdas;
}

void
HEMAX_Store::GetAllGeometryHdas(std::vector<HEMAX_GeometryHda*>& HdaList)
{
    for (auto It = MaxHdaStore.begin(); It != MaxHdaStore.end(); It++)
    {
        if (It->second)
        {
            HdaList.push_back(It->second);
        }
    }
}

std::vector<HEMAX_3dsmaxHda *>
HEMAX_Store::FindAllHdasUsingAssetDefinition(HEMAX_Asset *Asset)
{
    std::vector<HEMAX_3dsmaxHda *> Hdas;

    std::vector<HEMAX_3dsmaxHda *> All = GetAllHdas();
    for (int i = 0; i < All.size(); i++)
    {
	if (All[i])
	{
	    if (All[i]->Hda.HdaAsset.Path == Asset->Path)
	    {
		Hdas.push_back(All[i]);
	    }
	}
    }

    return Hdas;
}

void
HEMAX_Store::DeleteMaxNode(ULONG MaxNodeHandle, std::vector<ULONG>& ScheduledDeletionList)
{
    bool NodeScheduledForDeletion = false;
    for (int i = 0; i < ScheduledDeletionList.size(); i++)
    {
	if (MaxNodeHandle == ScheduledDeletionList[i])
	{
	    NodeScheduledForDeletion = true;
	    break;
	}
    }

    if (!NodeScheduledForDeletion)
    {
	GetCOREInterface()->DeleteNode(GetCOREInterface()->GetINodeByHandle(MaxNodeHandle));
    }
}
