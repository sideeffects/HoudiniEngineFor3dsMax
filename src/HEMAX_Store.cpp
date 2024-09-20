#include "HEMAX_Store.h"

#include "HEMAX_3dsmaxHda.h"
#include "HEMAX_Logger.h"
#include "HEMAX_GeometryHda.h"
#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Modifier.h"
#include "HEMAX_ModifierHda.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_UserPrefs.h"

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <icustattribcontainer.h>
#pragma warning(pop)

#pragma warning(push, 0)
#include <QtCore\qfileinfo.h>
#include <QtCore\qdir.h>
#pragma warning(pop)

#include <sstream>
#include <string>

HEMAX_Store::HEMAX_Store(HEMAX_Events& EventSystem)
    : HEMAX_EventHandler(EventSystem)
    , ToolShelf(this)
{
    RegisterCallback(HEMAX_EventType::PreSessionStopped,
            [this](HEMAX_EventData* Data) {
        this->DeleteStore();
    });
}

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

HAPI_AssetLibraryId
HEMAX_Store::FindAsset(std::string AssetPath)
{
    auto Search = LoadedAssetLibraries.find(AssetPath);

    if (Search != LoadedAssetLibraries.end())
	return Search->second;

    return -1;
}

std::string
HEMAX_Store::LoadNewAsset(std::string Path, bool& Success)
{
    HAPI_AssetLibraryId AssetLibId;
    HAPI_Result AssetLoadResult = LoadAssetLibrary(Path, false, AssetLibId);

    if (AssetLoadResult == HAPI_RESULT_SUCCESS)
    {
	std::replace(Path.begin(), Path.end(), '\\', '/');
	LoadedAssetLibraries.insert({ Path, AssetLibId });
	Success = true;
	return Path;
    }

    if (AssetLoadResult == HAPI_RESULT_ASSET_DEF_ALREADY_LOADED)
    {
        Success = false;
        return "";
    }

    // Try HEMAX_HDA_PATH first
    std::string HdaPath = HEMAX_Utilities::GetEnvVar(HEMAX_ENV_HDA_PATH);
    if (!HdaPath.empty())
    {
        QString PathString(HdaPath.c_str());
        QFileInfo HdaFileInfo(Path.c_str());
        QString HdaFile = HdaFileInfo.fileName();
        QString NewHdaFilePathQString = PathString.append("/") + HdaFile;
        std::string NewHdaFilePath = NewHdaFilePathQString.toStdString();

        AssetLoadResult = LoadAssetLibrary(NewHdaFilePath, false, AssetLibId);

        if (AssetLoadResult == HAPI_RESULT_SUCCESS)
        {
            std::replace(NewHdaFilePath.begin(), NewHdaFilePath.end(), '\\', '/');
            LoadedAssetLibraries.insert({ NewHdaFilePath, AssetLibId });
            Success = true;
            return NewHdaFilePath;
        }
    }

    // If it was not found, next try the user's HDA repository directory
    if (!UserHdaRepository.empty())
    {
        QString PathString(UserHdaRepository.c_str());
        QFileInfo HdaFileInfo(Path.c_str());
        QString HdaFile = HdaFileInfo.fileName();
        QString NewHdaFilePathQString = PathString.append("/") + HdaFile;
        std::string NewHdaFilePath = NewHdaFilePathQString.toStdString();

        AssetLoadResult = LoadAssetLibrary(NewHdaFilePath, false, AssetLibId);

        if (AssetLoadResult == HAPI_RESULT_SUCCESS)
        {
            std::replace(NewHdaFilePath.begin(), NewHdaFilePath.end(), '\\', '/');
            LoadedAssetLibraries.insert({ NewHdaFilePath, AssetLibId });
            Success = true;
            return NewHdaFilePath;
        }
    }

    // Last, try the directory where the current 3dsmax scene is saved
    std::wstring CurFilePathString(GetCOREInterface()->GetCurFilePath());
    std::string CurFilePathStringA(CurFilePathString.begin(), CurFilePathString.end());

    if (!CurFilePathStringA.empty())
    {
        QFileInfo CurFileInfo(CurFilePathStringA.c_str());
        QString PathString = CurFileInfo.dir().absolutePath();
        QFileInfo HdaFileInfo(Path.c_str());
        QString HdaFile = HdaFileInfo.fileName();
        QString NewHdaFilePathQString = PathString.append("/") + HdaFile;
        std::string NewHdaFilePath = NewHdaFilePathQString.toStdString();

        AssetLoadResult = LoadAssetLibrary(NewHdaFilePath, false, AssetLibId);

        if (AssetLoadResult == HAPI_RESULT_SUCCESS)
        {
            std::replace(NewHdaFilePath.begin(), NewHdaFilePath.end(), '\\', '/');
            LoadedAssetLibraries.insert({ NewHdaFilePath, AssetLibId });
            Success = true;
            return NewHdaFilePath;
        }
    }

    Success = false;
    return "";
}

bool
HEMAX_Store::RemoveAsset(std::string AssetPath)
{
    HAPI_AssetLibraryId AssetLibId = FindAsset(AssetPath);

    if (AssetLibId != -1)
    {
	LoadedAssetLibraries.erase(AssetPath);
	return true;
    }
    else
    {
        std::string Msg = "Tried to move HDA with path <" + AssetPath + ">, "
            "but it could not be found or has already been removed.";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
        return false;
    }
}

void
HEMAX_Store::LoadAssetsInHdaLoadPath()
{
    std::string HdaLoadPath;
    if (!HEMAX_UserPrefs::Get().GetStringSetting(HEMAX_SETTING_HDA_LOAD_PATH,
            HdaLoadPath))
        return;
    
    LoadAllAssetsInDirectory(HdaLoadPath);
    
    // Also load assets HEMAX_ENV_HDA_PATH environment variable (legacy)
    LoadAllAssetsInDirectory(HEMAX_Utilities::GetEnvVar(HEMAX_ENV_HDA_PATH));
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

std::string
HEMAX_Store::GetAssetPath(HAPI_AssetLibraryId AssetId)
{
    for (auto It = LoadedAssetLibraries.begin();
         It != LoadedAssetLibraries.end();
         It++)
    {
        if (It->second == AssetId)
            return It->first;
    }

    return "";
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

HAPI_AssetLibraryId
HEMAX_Store::UpdateAssetDefinition(const std::string& Asset)
{
    // First make sure the asset is in the store.
    HAPI_AssetLibraryId AssetId = FindAsset(Asset);

    if (AssetId == -1)
    {
        std::string Msg = "Could not update asset definition for <" + Asset
            + "> because the asset could not be found in the session.";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR);
    }

    LoadAssetLibrary(Asset, true, AssetId);
    return AssetId;
}

void
HEMAX_Store::DeleteStore()
{
    LoadedAssetLibraries.clear();
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
    HAPI_AssetLibraryId AssetId = FindAsset(Path);

    if (AssetId != -1)
    {
	GeoHda = new HEMAX_GeometryHda();
	GeoHda->Create(AssetId, Path, AssetIndex);

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
    HAPI_AssetLibraryId AssetId = FindAsset(Path);

    if (AssetId == -1)
    {
        std::string Msg = "HDA with path <" + Path + "> could not be created "
            "because it has not been loaded.";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR);
        return nullptr;
    }

    HEMAX_Modifier* ModifierPlugin =
        (HEMAX_Modifier*)GetCOREInterface()->CreateInstance(OSM_CLASS_ID,
                HEMAX_Modifier_CLASS_ID);
    GetCOREInterface12()->AddModifier((*Node), (*ModifierPlugin));

    ModifierHda = new HEMAX_ModifierHda;

    if (ModifierHda->Create(AssetId, Path, AssetIndex, ModifierPlugin, Node))
    {
        Add3dsmaxHda(Node->GetHandle(), ModifierHda);
        return ModifierHda;
    }
    else
    {
        GetCOREInterface7()->DeleteModifier((*Node), (*ModifierPlugin));
        delete ModifierPlugin;
        delete ModifierHda;
        return nullptr;
    }
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
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025)
            std::wstring CustAttribName = StampLookup->GetName(false);
#else
	    std::wstring CustAttribName = StampLookup->GetName();
#endif
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
HEMAX_Store::FindAllHdasUsingAssetDefinition(HAPI_AssetLibraryId AssetId)
{
    std::vector<HEMAX_3dsmaxHda *> Hdas;
    std::vector<HEMAX_3dsmaxHda *> All = GetAllHdas();

    for (std::size_t i = 0; i < All.size(); ++i)
    {
        if (All[i] && AssetId == FindAsset(All[i]->Hda.GetAssetPath()))
            Hdas.push_back(All[i]);
    }

    return Hdas;
}

HAPI_Result
HEMAX_Store::LoadAssetLibrary(const std::string& Path,
                              bool AllowOverwrite,
                              HAPI_AssetLibraryId& AssetLibId)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HAPI_Result Result = HEMAX_HoudiniApi::LoadAssetLibraryFromFile(&SM.Session,
        Path.c_str(), AllowOverwrite, &AssetLibId);

    if (Result != HAPI_RESULT_SUCCESS)
    {
        std::stringstream sstream;
        sstream << "Could not load asset library at path \"" << Path
            << "\" because ";

        if (Result == HAPI_RESULT_CANT_LOADFILE)
            sstream << "the file could not be found on disk.";
        else if (Result == HAPI_RESULT_ASSET_DEF_ALREADY_LOADED)
            sstream << "the asset library has already been loaded into the "
                "session and overwriting is disabled.";
        else if (Result == HAPI_RESULT_ASSET_INVALID)
            sstream << "the asset is invalid.";
        else
            sstream << "of an unknown reason.";

        std::string Msg = sstream.str();
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
    }

    return Result;
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
