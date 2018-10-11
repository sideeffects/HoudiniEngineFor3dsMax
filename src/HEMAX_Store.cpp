#include "HEMAX_Store.h"

#include "HEMAX_Logger.h"
#include "HEMAX_Modifier.h"
#include <icustattribcontainer.h>

#include <QtCore\qfileinfo.h>
#include <QtCore\qdir.h>

HEMAX_Store::HEMAX_Store() {}

HEMAX_Store::~HEMAX_Store()
{
    EmptyOutStore();
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

HEMAX_Asset*
HEMAX_Store::FindAsset(std::string AssetPath)
{
    auto Search = LoadedAssetLibraries.find(AssetPath);

    if (Search != LoadedAssetLibraries.end())
    {
        return &Search->second;
    }
    else
    {
        std::string LogEntry = "Attempted to find asset <" + AssetPath + "> in the loaded asset store, but it could not be found.";
        HEMAX_Logger::Instance().AddEntry(LogEntry, HEMAX_LOG_LEVEL_WARN);
    }

    return nullptr;
}

std::string
HEMAX_Store::LoadNewAsset(std::string Path, bool& Success)
{
    HEMAX_Asset NewAsset;
    HEMAX_AssetLoadStatus AssetLoadStatus;

    // First try the path that is given
    if (LoadAsset(NewAsset, Path, &AssetLoadStatus))
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

			if (LoadAsset(NewAsset, NewHdaFilePath.toStdString(), &AssetLoadStatus))
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

            if (LoadAsset(NewAsset, NewHdaFilePath.toStdString(), &AssetLoadStatus))
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

                if (LoadAsset(NewAsset, NewHdaFilePath.toStdString(), &AssetLoadStatus))
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

    if (AssetLoadStatus == HEMAX_ASSET_ALREADY_LOADED)
    {
        std::string Message = "Attempting to load asset [" + Path + "], but the asset definition is already loaded.";
        HEMAX_Logger::Instance().AddEntry(Message, HEMAX_LOG_LEVEL_WARN);
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

    return false;
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
            DeleteHda(It->second->Hda);
            delete It->second;
        }
    }

    MaxHdaStore.clear();

    for (auto It = ModifierHdaStore.begin(); It != ModifierHdaStore.end(); It++)
    {
        for (auto ListIt = It->second.begin(); ListIt != It->second.end(); ListIt++)
        {
            if ((*ListIt))
            {
                DeleteMainInput((*ListIt)->ModifierHda);
                DeleteHda((*ListIt)->Hda);
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
        MaxHdaStore.insert({ NodeHandle, Hda });
    }
    else if (Hda->Type == HEMAX_MODIFIER_HDA)
    {
        auto Search = ModifierHdaStore.find(NodeHandle);

        if (Search != ModifierHdaStore.end())
        {
            Search->second.push_back(Hda);
        }
        else
        {
            std::list<HEMAX_3dsmaxHda*> NewList;
            NewList.push_back(Hda);
            ModifierHdaStore.insert({ NodeHandle, NewList });
        }
    }
}

HEMAX_3dsmaxHda*
HEMAX_Store::CreateGeometryHda(std::string Path, int AssetIndex)
{
    HEMAX_3dsmaxHda* GeoHda = nullptr;
    HEMAX_Asset* TheAsset = FindAsset(Path);

    if (TheAsset)
    {
        GeoHda = new HEMAX_3dsmaxHda;
        CreateGeometry3dsmaxHda(*GeoHda, *TheAsset, AssetIndex);
        
        if (GeoHda->GeometryHda.ContainerNode)
        {
            Add3dsmaxHda(GeoHda->GeometryHda.ContainerNode->GetHandle(), GeoHda);
        }
        else
        {
            delete GeoHda;
            GeoHda = nullptr;
        }
    }

    return GeoHda;
}

HEMAX_3dsmaxHda*
HEMAX_Store::CreateModifierHda(INode* Node, std::string Path, int AssetIndex)
{
    HEMAX_3dsmaxHda* ModifierHda = nullptr;
    HEMAX_Asset* TheAsset = FindAsset(Path);
    if (TheAsset)
    {
        HEMAX_Modifier* ModifierPlugin = (HEMAX_Modifier*)GetCOREInterface()->CreateInstance(OSM_CLASS_ID, HEMAX_Modifier_CLASS_ID);
        GetCOREInterface12()->AddModifier((*Node), (*ModifierPlugin));

        ModifierHda = new HEMAX_3dsmaxHda;

        if (CreateModifier3dsmaxHda(*ModifierHda, *TheAsset, AssetIndex, ModifierPlugin, Node))
        {
            Add3dsmaxHda(Node->GetHandle(), ModifierHda);
            return ModifierHda;
        }
        else
        {
            GetCOREInterface7()->DeleteModifier((*Node), (*ModifierPlugin));
            delete ModifierPlugin;
        }
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
	    int ChildCount = Hda->GeometryHda.ContainerNode->NumberOfChildren();
	    for (int i = ChildCount - 1; i >= 0; i--)
	    {
		GetCOREInterface()->DeleteNode(Hda->GeometryHda.ContainerNode->GetChildNode(i));
	    }

	    GetCOREInterface()->DeleteNode(Hda->GeometryHda.ContainerNode);
	}

	DeleteHda(Hda->Hda);
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
            std::vector<ULONG> NodesToDelete = GetListOfAllChildINodes(Hda->GeometryHda);

            for (int i = 0; i < Hda->GeometryHda.InstanceClones.size(); i++)
            {
                DeleteMaxNode(Hda->GeometryHda.InstanceClones[i]->GetHandle(), ScheduledDeletionList);
            }

            for (int i = 0; i < Hda->GeometryHda.PackedPrimClones.size(); i++)
            {
                DeleteMaxNode(Hda->GeometryHda.PackedPrimClones[i]->GetHandle(), ScheduledDeletionList);
            }

            for (int i = 0; i < NodesToDelete.size(); i++)
            {
                bool NodeAlreadyBeingDeleted = false;
                for (int s = 0; s < ScheduledDeletionList.size(); s++)
                {
                    if (NodesToDelete[i] == ScheduledDeletionList[s])
                    {
                        NodeAlreadyBeingDeleted = true;
                        break;
                    }
                }

                if (!NodeAlreadyBeingDeleted)
                {
                    INode* TheNode = GetCOREInterface()->GetINodeByHandle(NodesToDelete[i]);
                    GetCOREInterface()->DeleteNode(TheNode);
                }
            }

            bool ContainerNodeAlreadyBeingDeleted = false;
            for (int s = 0; s < ScheduledDeletionList.size(); s++)
            {
                if (Hda->GeometryHda.ContainerNode->GetHandle() == ScheduledDeletionList[s])
                {
                    ContainerNodeAlreadyBeingDeleted = true;
                    break;
                }
            }

            if (!ContainerNodeAlreadyBeingDeleted)
            {
                GetCOREInterface()->DeleteNode(Hda->GeometryHda.ContainerNode);
            }
        }

        DeleteHda(Hda->Hda);

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
            if ((*ListIt)->ModifierHda.DisplayGeometry == (void*)ModifierPlugin)
            {
                delete (*ListIt)->ModifierHda.DisplayGeometry;
                (*ListIt)->ModifierHda.DisplayGeometry = nullptr;
                DeleteHda((*ListIt)->Hda);
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
            if ((*ListIt)->ModifierHda.DisplayGeometry == (void*)ModifierPlugin)
            {
                (*ListIt)->ModifierHda.DisplayGeometry = nullptr;
                DeleteHda((*ListIt)->Hda);
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

HEMAX_3dsmaxHda*
HEMAX_Store::Find3dsmaxHda(ULONG NodeHandle, Modifier* ModifierPlugin)
{
    auto Search = ModifierHdaStore.find(NodeHandle);

    if (Search != ModifierHdaStore.end())
    {
        for (auto It = Search->second.begin(); It != Search->second.end(); It++)
        {
            if ((*It)->ModifierHda.DisplayGeometry == (void*)ModifierPlugin)
            {
                return (*It);
            }
        }
    }

    return nullptr;
}

HEMAX_3dsmaxHda*
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
                    return Find3dsmaxHda(ParentNode->GetHandle());
                }
            }
        }
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