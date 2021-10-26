#include "HEMAX_Plugin.h"

#include "HEMAX_3dsmaxHda.h"
#include "HEMAX_3dsMaxInput.h"
#include "HEMAX_CustomAttributeContainer.h"
#include "HEMAX_Events.h"
#include "HEMAX_GeometryHda.h"
#include "HEMAX_GeometryMouseCallback.h"
#include "HEMAX_Hda.h"
#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"
#include "HEMAX_ModifierHda.h"
#include "HEMAX_ReferenceMaker.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Time.h"
#include "HEMAX_Undo.h"
#include "HEMAX_UserPrefs.h"

#include "UI/HEMAX_AssetSelection.h"
#include "UI/HEMAX_HDASelectionDialog.h"

#include "icustattribcontainer.h"
#include "custattrib.h"
#include "modstack.h"

#include <string>

extern ClassDesc2* GetHEMAX_ModifierClassDesc();

void
MAXSelectionSetChangedCallback(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->SelectionSetChangeHandler();
}

void
FilePreOpenHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandleSceneFileOpening();
}

void
FilePostOpenHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandleSceneFilePostOpen();
}

void
PreNewAllHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandlePreNewAll();
}

void
NodePreDeleteHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandleNodePreDelete(static_cast<Tab<INode*>*>(Info->callParam));
}

void
ModifierPostAddHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandleModifierPostAdd(static_cast<HEMAX_ModifierEvent*>(Info->callParam));
}

void
ModifierPreDeleteHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandleModifierPreDelete(static_cast<HEMAX_ModifierEvent*>(Info->callParam));
}

void
RenderPreEvalHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandleRenderPreEval(static_cast<TimeValue*>(Info->callParam));
}

void
NodeCreatedHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandleNodeCreated(static_cast<INode*>(Info->callParam));
}

HEMAX_Plugin::HEMAX_Plugin(Interface* Interface, HMODULE LibHAPIL)
    : MaxInterface(Interface)
    , ManualModifierAddInProgress(false)
{
    HEMAX_HoudiniApi::InitializeHAPI(LibHAPIL);

    if (HEMAX_HoudiniApi::IsHAPIInitialized())
    {
        PluginStore = new HEMAX_Store;

        // Initialize User Prefs
        HEMAX_UserPrefs::Get();
        ToolShelf = new HEMAX_Shelf(PluginStore);

        InitializeLogPrintLevels();
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(
                "HEMAX_Plugin: libHAPIL.dll was loaded, but the function "
                "addresses could not be determined.",
                HEMAX_LOG_LEVEL_ERROR);
    }
}

HEMAX_Plugin::~HEMAX_Plugin()
{
    ToolShelf->SaveShelfToJson(HEMAX_UserPrefs::Get().GetPluginConfigFolder());

    delete PluginStore;
}

void
HEMAX_Plugin::Init(std::string HapiToolsDir)
{
        HEMAX_UserPrefs& UserPreferences = HEMAX_UserPrefs::Get();

        bool AutoStartSession;
        UserPreferences.GetBoolSetting(HEMAX_SETTING_AUTO_START_SESSION,
                                       AutoStartSession);

        int SessionTypePref;
        UserPreferences.GetIntSetting(HEMAX_SETTING_SESSION_TYPE,
                                      SessionTypePref);

        HEMAX_SessionTypePref SessionType =
                        static_cast<HEMAX_SessionTypePref>(SessionTypePref);

        if (AutoStartSession && SessionType == HEMAX_SessionTypePref::AutoStart)
        {
            HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
            SM.SetSessionType(HEMAX_THRIFT_PIPE); 
            SM.StartThriftNamedPipeThinClient();
            StartSession();
        }

        std::string HdaRepoDir;
        UserPreferences.GetStringSetting(HEMAX_SETTING_AUTO_START_SESSION, 
                                         HdaRepoDir);
        PluginStore->UserHdaRepository = HdaRepoDir;

        ToolShelf->LoadShelvesFromJson(
                    UserPreferences.GetPluginConfigFolder());
        ToolShelf->AddShelfDirectory(HapiToolsDir, "Default", true);
        PluginEvents->ShelfUpdated();

        RegisterNotification((NOTIFYPROC)MAXSelectionSetChangedCallback,
                             (void*)this,
                             NOTIFY_SELECTIONSET_CHANGED);

        RegisterNotification((NOTIFYPROC)FilePreOpenHandler,
                             (void*)this,
                             NOTIFY_FILE_PRE_OPEN);

        RegisterNotification((NOTIFYPROC)FilePostOpenHandler,
                             (void*)this,
                             NOTIFY_FILE_POST_OPEN);
        
        RegisterNotification((NOTIFYPROC)PreNewAllHandler,
                             (void*)this,
                             NOTIFY_SYSTEM_PRE_NEW);

        RegisterNotification((NOTIFYPROC)NodePreDeleteHandler,
                             (void*)this,
                             NOTIFY_SEL_NODES_PRE_DELETE);

        RegisterNotification((NOTIFYPROC)ModifierPostAddHandler,
                             (void*)this,
                             NOTIFY_POST_MODIFIER_ADDED);

        RegisterNotification((NOTIFYPROC)ModifierPreDeleteHandler,
                             (void*)this,
                             NOTIFY_PRE_MODIFIER_DELETED);

        RegisterNotification((NOTIFYPROC)NodeCreatedHandler,
                             (void*)this,
                             NOTIFY_NODE_CREATED);

        RegisterNotification((NOTIFYPROC)RenderPreEvalHandler,
                             (void*)this,
                             NOTIFY_RENDER_PREEVAL);

        GetCOREInterface()->RegisterTimeChangeCallback(this);
}

void
HEMAX_Plugin::SetEventHub(HEMAX_Events* _PluginEvents)
{
    PluginEvents = _PluginEvents;
}

HEMAX_Events*
HEMAX_Plugin::GetEventHub()
{
    return PluginEvents;
}

void
HEMAX_Plugin::TimeChanged(TimeValue Time)
{
    if (HEMAX_SessionManager::GetSessionManager().IsSessionActive())
    {
	PushCurrentTime(Time);

	if (HasHAPITimeChanged())
	{
	    SyncEntireScene();
	    MarkSynchronized();
	}
    }
}

void
HEMAX_Plugin::SyncEntireScene()
{
    std::vector<HEMAX_3dsmaxHda*> Hdas = PluginStore->GetAllHdas();

    for (int i = 0; i < Hdas.size(); i++)
    {
        if (Hdas[i]->Hda.HasTimeDependentNodes())
        {
            UpdateEntireHda(Hdas[i], false);
        }
    }
}

void
HEMAX_Plugin::SelectionSetChangeHandler()
{
    HEMAX_GeometryHda* AssetToSelect = nullptr;
    int SelectedMaxNodeCount = GetCOREInterface()->GetSelNodeCount();

    if (SelectedMaxNodeCount == 1)
    {
	bool HEMAXNodeFound = false;

	ULONG SelectedNode = GetCOREInterface()->GetSelNode(0)->GetHandle();
	AssetToSelect = PluginStore->FindGeometryHda(SelectedNode);

	if (!AssetToSelect)
	{
	    bool AutoSelectRootNode;
            HEMAX_UserPrefs::Get().GetBoolSetting(HEMAX_SETTING_GRAB_ROOT,
                AutoSelectRootNode);
	    if (AutoSelectRootNode)
	    {
		AssetToSelect = PluginStore->Find3dsmaxHdaViaChildGeometry(SelectedNode);

		if (AssetToSelect)
		{
		    ULONG AssetNode = AssetToSelect->ContainerNode->GetHandle();
		    INode* SelectedMaxAssetNode = GetCOREInterface()->GetINodeByHandle(AssetNode);

		    if (SelectedMaxAssetNode)
		    {
			GetCOREInterface()->SelectNode(SelectedMaxAssetNode, 0);
		    }
		}
	    }
	}
    }
    else if (SelectedMaxNodeCount > 1)
    {
	for (int i = 0; i < SelectedMaxNodeCount; ++i)
	{
	    ULONG SelectedNode = GetCOREInterface()->GetSelNode(i)->GetHandle();
	    AssetToSelect = PluginStore->FindGeometryHda(SelectedNode);

	    if (AssetToSelect)
	    {
		break;
	    }
	}
    }

    PluginEvents->SelectionSetChanged(AssetToSelect);
}

void
HEMAX_Plugin::LoadNewAsset(std::string Path)
{
    if (HEMAX_SessionManager::GetSessionManager().IsSessionActive())
    {
	if (!PluginStore->FindAsset(Path))
	{
	    bool Success;
	    PluginStore->LoadNewAsset(Path, Success);
	    if (Success)
	    {
		PluginEvents->AssetLoaded();
	    }
	}
    }
}

void
HEMAX_Plugin::CreateGeometryHDA(std::string Path)
{
    HEMAX_Asset* TheAsset = PluginStore->FindAsset(Path);

    if (TheAsset)
    {
	if (TheAsset->AssetCount == 1)
	{
	    CreateGeometryHDA(Path, 0);
	}
	else
	{
	    std::vector<std::string>& AssetNames = TheAsset->Names;

	    HEMAX_AssetSelection AssetSelector(AssetNames);

	    if (AssetSelector.exec())
	    {
		for (int s = 0; s < AssetNames.size(); ++s)
		{
		    if (AssetNames[s] == AssetSelector.GetSelectedAssetName())
		    {
			CreateGeometryHDA(Path, s);
		    }
		}
	    }
	}
    }
    else
    {
        std::string Msg = "HDA with path <" + Path + "> could not be created "
            "because it has not been loaded.";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR); 
    }
}

HEMAX_GeometryHda*
HEMAX_Plugin::CreateGeometryHDA(std::string Path, int AssetIndex)
{
    HEMAX_GeometryHda* CreatedAsset = PluginStore->CreateGeometryHda(
                                        Path, AssetIndex);

    if (CreatedAsset)
    {
	ConstructReferencesForEditableCurve(CreatedAsset);
	GetCOREInterface()->ForceCompleteRedraw();
	GetCOREInterface()->SelectNode(CreatedAsset->ContainerNode);
    }

    return CreatedAsset;
}

HEMAX_ModifierHda*
HEMAX_Plugin::CreateModifierHDA(INode* Node, std::string Path, int AssetIndex)
{
    ManualModifierAddInProgress = true;
    HEMAX_ModifierHda* ModifierHda = nullptr;

    theHold.Begin();
    ModifierHda = PluginStore->CreateModifierHda(Node, Path, AssetIndex);
    theHold.Accept(HEMAX_ADD_MODIFIER_HDA);

    if (ModifierHda)
    {
	ModifierHda->DisplayGeometry->RegisterPlugin(this);
	UpdateEntireHda(ModifierHda);
    }

    ManualModifierAddInProgress = false;
    return ModifierHda;
}

void
HEMAX_Plugin::CreateModifierHDAs(std::string Path)
{
    ManualModifierAddInProgress = true;

    // Get all current selections
    int NumSelection = GetCOREInterface()->GetSelNodeCount();
    std::vector<INode*> SelectedNodes;
    for (int i = 0; i < NumSelection; i++)
    {
	SelectedNodes.push_back(GetCOREInterface()->GetSelNode(i));
    }

    HEMAX_Asset* TheAsset = PluginStore->FindAsset(Path);

    if (TheAsset)
    {
	if (TheAsset->AssetCount == 1)
	{
	    for (int i = 0; i < NumSelection; i++)
	    {
		HEMAX_3dsmaxHda* ModifierHda = CreateModifierHDA(
                    SelectedNodes[i], Path, 0);
		if (ModifierHda)
		{
                    PluginEvents->SelectionSetChanged(ModifierHda);
		}
	    }
	}
	else
	{
	    std::vector<std::string>& AssetNames = TheAsset->Names;
	    HEMAX_AssetSelection AssetSelector(AssetNames);

	    if (AssetSelector.exec())
	    {
		for (int s = 0; s < AssetNames.size(); s++)
		{
		    if (AssetNames[s] == AssetSelector.GetSelectedAssetName())
		    {
			for (int i = 0; i < NumSelection; i++)
			{
			    CreateModifierHDA(SelectedNodes[i], Path, s);
			}
		    }
		}
	    }
	}
    }
    else
    {
        std::string Msg = "HDA with path <" + Path + "> could not be created "
            "because it has not been loaded.";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR); 
    }

    ManualModifierAddInProgress = false;
}

INode*
HEMAX_Plugin::CloneHda(HEMAX_3dsmaxHda* MaxHda)
{
    INode* Clone = nullptr;
    if (MaxHda)
    {
	if (MaxHda->Type == HEMAX_GEOMETRY_HDA)
	{
            HEMAX_GeometryHda* MaxGeoHda = dynamic_cast<HEMAX_GeometryHda*>(
                                                                    MaxHda);
            if (MaxGeoHda)
            {
                Clone = CloneGeometryHda(MaxGeoHda);
            }
	}
	else if (MaxHda->Type == HEMAX_MODIFIER_HDA)
	{
	    HEMAX_ModifierHda* ModifierHda = static_cast<HEMAX_ModifierHda*>(MaxHda);
	    CloneModifierHda(MaxHda, ModifierHda->ContainerNode);
	    Clone = ModifierHda->ContainerNode;
	}
    }

    return Clone;
}

Modifier*
HEMAX_Plugin::CopyHdaToNode(HEMAX_3dsmaxHda* MaxHda, INode* Node)
{
    Modifier* Copy = nullptr;

    if (MaxHda && Node)
    {
	GetCOREInterface()->SelectNode(Node);
	Copy = CloneModifierHda(MaxHda, Node);
    }

    return Copy;
}

void
HEMAX_Plugin::HandleSceneFileOpening()
{
    PluginEvents->SelectionSetChanged(nullptr, true);
    DestroyAllEditableNodeReferences();
    PluginStore->EmptyOutStore();
}

void
HEMAX_Plugin::HandleSceneFilePostOpen()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.IsSessionActive())
    {
	TraverseHdaLoadPaths();
	ToolShelf->LoadToolAssets();

	INode* RootNode = GetCOREInterface()->GetRootNode();
	ReconnectAllStrandedHdas(RootNode);

        PluginEvents->AssetLoaded();
    }
}

void
HEMAX_Plugin::HandlePreNewAll()
{
    PluginEvents->SelectionSetChanged(nullptr, true);
    DestroyAllEditableNodeReferences();
    PluginStore->EmptyOutStore();

    if (HEMAX_SessionManager::GetSessionManager().IsSessionActive())
    {
	TraverseHdaLoadPaths();
	ToolShelf->LoadToolAssets();
        PluginEvents->AssetLoaded();
    }
}

void
HEMAX_Plugin::HandleNodePreDelete(Tab<INode*>* NodesBeingDeleted)
{
    std::vector<ULONG> ScheduledForDeletion;

    for (int i = 0; i < NodesBeingDeleted->Count(); ++i)
    {
	ScheduledForDeletion.push_back((*(NodesBeingDeleted->Addr(i)))->GetHandle());

	HEMAX_3dsMaxInput* MaxInput = PluginStore->Find3dsMaxInput((*(NodesBeingDeleted->Addr(i)))->GetHandle());

	if (MaxInput)
	{
	    Handle3dsMaxInputSourceDeletion(MaxInput);
	    PluginStore->Delete3dsMaxInput((*(NodesBeingDeleted->Addr(i)))->GetHandle());
	}
    }

    for (int i = 0; i < NodesBeingDeleted->Count(); ++i)
    {
	INode** TheNode = NodesBeingDeleted->Addr(i);

	HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda((*TheNode)->GetHandle());
	if (Hda)
	{
            PluginEvents->HdaPreDelete(Hda);
	    Clear3dsmaxHdaInputConnections(Hda, false);
	    PluginStore->Delete3dsmaxHda((*TheNode)->GetHandle(), ScheduledForDeletion);
	}
    }
}

void
HEMAX_Plugin::HandleModifierPostAdd(HEMAX_ModifierEvent* ModEvent)
{
    if (!ManualModifierAddInProgress)
    {
	std::wstring ModStackPluginName(_T(HEMAX_MODIFIER_STACK_PLUGIN_NAME));
	std::wstring ModifierName(ModEvent->mod->GetName());

	if (ModifierName == ModStackPluginName)
	{
	    HEMAX_HDASelectionDialog ModSelectionDialog(PluginStore->GetListOfLoadedAssets());

	    if (ModSelectionDialog.exec())
	    {
		std::string SelectedAssetPath = ModSelectionDialog.GetSelectedAssetPath();

		HEMAX_Asset* TheAsset = PluginStore->FindAsset(SelectedAssetPath);

		if (TheAsset)
		{
		    if (TheAsset->AssetCount == 1)
		    {
			HEMAX_Modifier* ModifierPlugin = (HEMAX_Modifier*)ModEvent->mod;
			HEMAX_ModifierHda* ModifierHda = new HEMAX_ModifierHda;

			if (ModifierHda->Create(*TheAsset, 0, ModifierPlugin, ModEvent->node))
			{
			    ModifierPlugin->RegisterPlugin(this);
			    PluginStore->Add3dsmaxHda(ModEvent->node->GetHandle(), ModifierHda);
			    GetCOREInterface()->ForceCompleteRedraw();
			    ModifierHda->DisplayGeometry->ApplyMaterialsToNode();
			    ModifierHda->DisplayGeometry->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
			}
			else
			{
			    GetCOREInterface7()->DeleteModifier(*(ModEvent->node), *(ModEvent->mod));
			    delete ModifierHda;
			}
		    }
		    else
		    {
			std::vector<std::string>& AssetNames = TheAsset->Names;

			HEMAX_AssetSelection AssetSelector(AssetNames);

			if (AssetSelector.exec())
			{
			    for (int s = 0; s < AssetNames.size(); ++s)
			    {
				if (AssetNames[s] == AssetSelector.GetSelectedAssetName())
				{
				    HEMAX_Modifier* ModifierPlugin = (HEMAX_Modifier*)ModEvent->mod;
				    HEMAX_ModifierHda* ModifierHda = new HEMAX_ModifierHda;

				    if (ModifierHda->Create(*TheAsset, s, ModifierPlugin, ModEvent->node))
				    {
					ModifierPlugin->RegisterPlugin(this);
					PluginStore->Add3dsmaxHda(ModEvent->node->GetHandle(), ModifierHda);
					GetCOREInterface()->ForceCompleteRedraw();
					ModifierHda->DisplayGeometry->ApplyMaterialsToNode();
					ModifierHda->DisplayGeometry->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
				    }
				    else
				    {
					GetCOREInterface7()->DeleteModifier(*(ModEvent->node), *(ModEvent->mod));
					delete ModifierHda;
				    }
				}
			    }
			}
		    }
		}
		else
		{
                    std::string Msg = "HDA with path <" + SelectedAssetPath +
                        "> could not be created because it has not been loaded.";
                    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR); 
		    GetCOREInterface7()->DeleteModifier(*(ModEvent->node),
                        *(ModEvent->mod));
		}
	    }
	    else
	    {
		GetCOREInterface7()->DeleteModifier(*(ModEvent->node), *(ModEvent->mod));
	    }
	}
	else
	{
	    // There's this really weird case where you drag and drop a
	    // a modifier. It will emit a modifier post-add signal and a mod
	    // pre-delete signal.
	    if (ModEvent->node && ModEvent->mod)
	    {
		HEMAX_ModifierHda* ModifierHda = PluginStore->Find3dsmaxHda(
						    ModEvent->node->GetHandle(),
						    ModEvent->mod);

		if (ModifierHda)
		{
		    ModifierHda->DisplayGeometry->SetIsBeingDragged(true);
		}
	    }
	}
    }
}

void
HEMAX_Plugin::HandleModifierPreDelete(HEMAX_ModifierEvent* ModEvent)
{
    HEMAX_ModifierHda* ModifierHda = PluginStore->Find3dsmaxHda(ModEvent->node->GetHandle(), ModEvent->mod);

    if (ModifierHda)
    {
        PluginEvents->HdaPreDelete(ModifierHda);

	HEMAX_Modifier* TheModifier = dynamic_cast<HEMAX_Modifier*>(ModEvent->mod);

	if (TheModifier->IsBeingDragged())
	{
	    TheModifier->SetIsBeingDragged(false);
	    return;
	}

	if (!TheModifier->IsCollapsing())
	{
	    Clear3dsmaxHdaInputConnections(ModifierHda);
	    ModifierHda->DisplayGeometry->CleanupHAPIComponents();
	    PluginStore->Delete3dsmaxHda(ModEvent->node->GetHandle(), ModEvent->mod);
	}
    }
}

void
HEMAX_Plugin::HandleRenderPreEval(TimeValue* Time)
{
    TimeChanged(*Time);
}

void
HEMAX_Plugin::HandleNodeCreated(INode* AddedNode)
{
    Object* NodeObject = AddedNode->GetObjectRef();

    if (NodeObject)
    {
	if (NodeObject->ClassID() == HEMAX_GeometryPlugin_CLASS_ID)
	{
	    HEMAX_GeometryPlugin* Geom = static_cast<HEMAX_GeometryPlugin*>(NodeObject);

	    if (Geom->IsStranded)
	    {
		HEMAX_Asset* CurrentAsset = PluginStore->FindAsset(
                                                HEMAX_CurrentAssetSelection);

		if (CurrentAsset)
		{
		    if (CurrentAsset->AssetCount == 1)
		    {
			HEMAX_GeometryHda* GeometryHda =
                            new HEMAX_GeometryHda();
			AddedNode->SetObjectRef(nullptr);
			GeometryHda->Dummy = new DummyObject;
			AddedNode->SetObjectRef(GeometryHda->Dummy);

			GeometryHda->Create(AddedNode, *CurrentAsset, 0);
			ConstructReferencesForEditableCurve(GeometryHda);
			PluginStore->Add3dsmaxHda(GeometryHda->ContainerNode->GetHandle(), GeometryHda);
			GetCOREInterface()->ForceCompleteRedraw();
			GetCOREInterface()->SelectNode(GetCOREInterface()->GetINodeByHandle(GeometryHda->ContainerNode->GetHandle()));

			NodeObject->MaybeAutoDelete();
		    }
		    else if (CurrentAsset->AssetCount > 1)
		    {
			std::vector<std::string>& AssetNames = CurrentAsset->Names;

			HEMAX_AssetSelection AssetSelector(AssetNames);

			if (AssetSelector.exec())
			{
			    for (int s = 0; s < AssetNames.size(); ++s)
			    {
				if (AssetNames[s] == AssetSelector.GetSelectedAssetName())
				{
				    HEMAX_GeometryHda* GeometryHda =
                                        new HEMAX_GeometryHda();
				    AddedNode->SetObjectRef(nullptr);
				    GeometryHda->Dummy = new DummyObject;
				    AddedNode->SetObjectRef(GeometryHda->Dummy);

				    GeometryHda->Create(AddedNode, *CurrentAsset, 0);
				    ConstructReferencesForEditableCurve(GeometryHda);
				    PluginStore->Add3dsmaxHda(GeometryHda->ContainerNode->GetHandle(), GeometryHda);
				    GetCOREInterface()->ForceCompleteRedraw();
				    GetCOREInterface()->SelectNode(GetCOREInterface()->GetINodeByHandle(GeometryHda->ContainerNode->GetHandle()));

				    NodeObject->MaybeAutoDelete();
				}
			    }
			}
		    }
		}
		else
		{
		    HEMAX_Logger::Instance().ShowDialog("HDA Missing", "No HDA is selected. Please select an HDA from the loaded assets list.", HEMAX_LOG_LEVEL_INFO);
		}
	    }
	}
    }
}

void
HEMAX_Plugin::ReconnectAllStrandedHdas(INode* StartingNode)
{
    if (!StartingNode)
    {
	return;
    }

    // Check for geometry HDAs
    std::string HdaStampName(HEMAX_MAX_HOUDINI_STAMP_NAME);

    if (DoesCustomAttributeExist(StartingNode->GetCustAttribContainer(),
                                 HEMAX_MAX_HOUDINI_STAMP_INDEX,
                                 HdaStampName))
    {
	ReengageGeometryHda(StartingNode,
                            StartingNode->GetCustAttribContainer());
    }

    // Now check for modifier HDAs
    Object* NodeObject = StartingNode->GetObjectRef();

    if (NodeObject)
    {
	if (NodeObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
	    IDerivedObject* DerivedObject = 
                dynamic_cast<IDerivedObject*>(NodeObject);
	    ModifierDive(DerivedObject, StartingNode);
	}
    }

    int ChildCount = StartingNode->NumberOfChildren();

    for (int z = 0; z < ChildCount; z++)
    {
	ReconnectAllStrandedHdas(StartingNode->GetChildNode(z));
    }
}

void
HEMAX_Plugin::ModifierDive(IDerivedObject* DerivedObject, INode* StartingNode)
{
    int ModifierCount = DerivedObject->NumModifiers();

    for (int i = 0; i < ModifierCount; i++)
    {
	if (DerivedObject->GetModifier(i)->ClassID() == HEMAX_Modifier_CLASS_ID)
	{
	    HEMAX_Modifier* TheModifier = dynamic_cast<HEMAX_Modifier*>(DerivedObject->GetModifier(i));
	    ReengageModifierHda(StartingNode, TheModifier, TheModifier->GetCustAttribContainer());
	}
    }

    Object* BaseObj = DerivedObject->GetObjRef();
    if (BaseObj)
    {
	IDerivedObject* BaseObjDerivedObj = dynamic_cast<IDerivedObject*>(BaseObj);
	if (BaseObjDerivedObj)
	{
	    ModifierDive(BaseObjDerivedObj, StartingNode);
	}
    }
}

void
HEMAX_Plugin::ReengageModifierHda(INode* Node, HEMAX_Modifier* Modifier, ICustAttribContainer* CustAttribs)
{
    if (CustAttribs)
    {
	CustAttrib* AssetPathAttrib = CustAttribs->GetCustAttrib(HEMAX_HOUDINI_MODIFIER_ASSET_PATH_INDEX);

	if (AssetPathAttrib)
	{
	    std::wstring WideName(AssetPathAttrib->GetName());
	    std::string AttribName(WideName.begin(), WideName.end());
	    std::string AssetPathName(HEMAX_HOUDINI_MODIFIER_ASSET_PATH_NAME);

	    if (AttribName == AssetPathName)
	    {
		// All is good now. Begin reconnecting the modifier to the HDA
		const wchar_t* WidePath;
		AssetPathAttrib->GetParamBlock(0)->GetValue(0, 0, WidePath, FOREVER);

		std::wstring WStringPath(WidePath);
		std::string AssetPath(WStringPath.begin(), WStringPath.end());

		HEMAX_Asset* TheAsset = PluginStore->FindAsset(AssetPath);

		std::string AssetFoundLocation = AssetPath;
		bool Success = true;

		if (!TheAsset)
		{
		    AssetFoundLocation = PluginStore->LoadNewAsset(AssetPath, Success);
		}

		if (Success)
		{
		    TheAsset = PluginStore->FindAsset(AssetPath);

		    int Index = 0;

		    if (TheAsset)
		    {
			HEMAX_ModifierHda* MaxHda = nullptr;
			if (TheAsset->AssetCount == 1)
			{
			    MaxHda = new HEMAX_ModifierHda;
			    MaxHda->Recreate(*TheAsset, Index, Modifier, Node);
			}
			else if (TheAsset->AssetCount > 1)
			{
			    MaxHda = new HEMAX_ModifierHda;
			    CustAttrib* AssetLibraryIndex = CustAttribs->GetCustAttrib(HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_INDEX);
			    AssetLibraryIndex->GetParamBlock(0)->GetValue(0, 0, Index, FOREVER);
			    MaxHda->Recreate(*TheAsset, Index, Modifier, Node);
			}

			if (MaxHda)
			{
			    PluginStore->Add3dsmaxHda(Node->GetHandle(), MaxHda);
			    Modifier->RegisterPlugin(this);
			    ReloadHdaFromCustomAttributes(MaxHda);
			    UpdateEntireHda(MaxHda);
			}
		    }
                    else
                    {
                        std::string Msg = "Could not reload modifier HDA "
                            "because the required HDA <" + AssetPath + "> is "
                            "not loaded.";
                        HEMAX_Logger::Instance().AddEntry(Msg,
                            HEMAX_LOG_LEVEL_ERROR);
                    }
		}
	    }
	}
    }
}

void
HEMAX_Plugin::ReengageGeometryHda(INode* Node, ICustAttribContainer* CustAttribs)
{
    CustAttrib* AssetPathAttrib = CustAttribs->GetCustAttrib(
                                    HEMAX_MAX_HOUDINI_ASSET_PATH_INDEX);

    if (AssetPathAttrib)
    {
	std::wstring WideName(AssetPathAttrib->GetName());
	std::string AttribName(WideName.begin(), WideName.end());
	std::string AssetPathName(HEMAX_MAX_HOUDINI_ASSET_PATH_NAME);

	if (AttribName == AssetPathName)
	{
	    // All is good now. Begin reconnecting the asset.

	    const wchar_t* WidePath;
	    AssetPathAttrib->GetParamBlock(0)->GetValue(0, 0, WidePath, FOREVER);

	    std::wstring WStringPath(WidePath);
	    std::string AssetPath(WStringPath.begin(), WStringPath.end());

	    HEMAX_Asset* TheAsset = PluginStore->FindAsset(AssetPath);

	    std::string AssetFoundLocation = AssetPath;
	    bool Success = true;

	    if (!TheAsset)
	    {
		AssetFoundLocation = PluginStore->LoadNewAsset(AssetPath, Success);
	    }

	    if (Success)
	    {
		TheAsset = PluginStore->FindAsset(AssetFoundLocation);

                if (TheAsset)
                {
                    HEMAX_GeometryHda* GeometryHda = nullptr;

                    if (TheAsset->AssetCount == 1)
                    {
                        GeometryHda = new HEMAX_GeometryHda();
                        GeometryHda->Init(*TheAsset, 0);
                        ConstructReferencesForEditableCurve(GeometryHda);
                    }
                    else
                    {
                        CustAttrib* AssetLibraryIndex =
                            CustAttribs->GetCustAttrib(
                                HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_INDEX);
                        int Index;
                        AssetLibraryIndex->GetParamBlock(0)->GetValue(0, 0,
                            Index, FOREVER);

                        GeometryHda = new HEMAX_GeometryHda();
                        GeometryHda->Init(*TheAsset, Index);
                        ConstructReferencesForEditableCurve(GeometryHda);
                    }

                    if (GeometryHda)
                    {
                        PluginStore->Add3dsmaxHda(Node->GetHandle(), GeometryHda);
                        GeometryHda->SetContainerNode(Node);
                        GeometryHda->SetCustomAttributeContainer(
                            Node->GetCustAttribContainer());
                        ReloadHdaFromCustomAttributes(GeometryHda);
                        GeometryHda->CreateGeometryHdaFromContainerNode();
                        UpdateEntireHda(GeometryHda);
                    }
                }
                else
                {
                    std::string Msg = "Could not reload geometry HDA because "
                        "the required HDA <" + AssetPath + "> is not loaded.";
                    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR);
                }
	    }
	}
    }
}

void
HEMAX_Plugin::HandleModifierStackCollapse(INode* TheNode, Modifier* HDAModifier)
{
    if (HDAModifier)
    {
	HEMAX_3dsmaxHda* MaxHda = PluginStore->Find3dsmaxHda(TheNode->GetHandle(), HDAModifier);
	if (MaxHda)
	{
	    HEMAX_Modifier* MaxModifier = dynamic_cast<HEMAX_Modifier*>(HDAModifier);
	    MaxModifier->DeleteInputNode();
	    Clear3dsmaxHdaInputConnections(MaxHda);
	    PluginStore->Remove3dsmaxHda(TheNode->GetHandle(), HDAModifier);
	}
    }
}

bool
HEMAX_Plugin::StartSession()
{
    if (!HEMAX_SessionManager::GetSessionManager().StartSession())
        return false;

    PushTimelineSettings();
    PushCurrentTime(GetCOREInterface()->GetTime());

    GetCOREInterface()->SelectNode(nullptr);

    TraverseHdaLoadPaths();

    INode* RootNode = GetCOREInterface()->GetRootNode();
    ReconnectAllStrandedHdas(RootNode);

    ToolShelf->LoadToolAssets();
    PluginEvents->AssetLoaded();

    return true;
}

bool
HEMAX_Plugin::StopSession()
{
    PluginEvents->SelectionSetChanged(nullptr, true);
    DestroyAllEditableNodeReferences();
    PluginStore->EmptyOutStore();

    HEMAX_SessionManager::GetSessionManager().StopSession();
    PluginEvents->SessionChanged();

    return true;
}

ULONG
HEMAX_Plugin::DetermineInputNodeSelection(HEMAX_3dsmaxHda* Hda)
{
    int SelectedMaxNodeCount = GetCOREInterface()->GetSelNodeCount();

    if (SelectedMaxNodeCount > 0)
    {
	for (int i = 0; i < SelectedMaxNodeCount; ++i)
	{
	    ULONG SelectedNode = GetCOREInterface()->GetSelNode(i)->GetHandle();
	    HEMAX_3dsmaxHda* SelectedAsset = PluginStore->Find3dsmaxHda(SelectedNode);

	    if (!SelectedAsset || SelectedAsset != Hda)
	    {
		return SelectedNode;
	    }
	}
    }

    return 0;
}

HEMAX_3dsMaxInput*
HEMAX_Plugin::Create3dsMaxInput(ULONG InputNode)
{
    HEMAX_3dsMaxInput* NewMaxInput = new HEMAX_3dsMaxInput(GetCOREInterface()->GetINodeByHandle(InputNode));
    return NewMaxInput;
}

std::string
HEMAX_Plugin::GetNodeName(ULONG Node)
{
    INode* SceneNode = GetCOREInterface()->GetINodeByHandle(Node);

    if (Node)
    {
	std::wstring WideName(SceneNode->GetName());
	return std::string(WideName.begin(), WideName.end());
    }

    return std::string("");
}

void
HEMAX_Plugin::UpdateHdaLoadDirectory(std::string Directory)
{
    HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_HDA_LOAD_PATH,
        Directory);

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.IsSessionActive())
    {
	PluginStore->LoadAllAssetsInDirectory(Directory);
        PluginEvents->AssetLoaded();
    }
}

void
HEMAX_Plugin::UpdateHdaSearchPathDirectory(std::string Directory)
{
    HEMAX_UserPrefs::Get().SetStringSetting(HEMAX_SETTING_HDA_REPO_PATH,
        Directory);
    PluginStore->UserHdaRepository = Directory;
}

HEMAX_Store*
HEMAX_Plugin::GetPluginStore()
{
    return PluginStore;
}

void
HEMAX_Plugin::DirtyEditableNodes(ULONG ContainerNodeHandle)
{
    HEMAX_GeometryHda* MaxHda = PluginStore->FindGeometryHda(ContainerNodeHandle);

    if (MaxHda && MaxHda->Type == HEMAX_GEOMETRY_HDA)
    {
	for (int i = 0; i < MaxHda->EditableCurves.size(); i++)
	{
	    MaxHda->EditableCurves[i].Dirty = true;
	}
    }
}

void
HEMAX_Plugin::ConstructReferencesForEditableCurve(HEMAX_GeometryHda* GeoHda)
{
    if (GeoHda && GeoHda->ContainerNode)
    {
	std::vector<HEMAX_EditableCurve>& EditableCurves = GeoHda->EditableCurves;
	std::vector<HEMAX_ReferenceMaker*> CurveNodeRefs;
	for (int i = 0; i < EditableCurves.size(); ++i)
	{
	    HEMAX_ReferenceMaker* NewRef = new HEMAX_ReferenceMaker(this);
	    NewRef->AddEditableNodeToWatch(EditableCurves[i].Node);
	    CurveNodeRefs.push_back(NewRef);
	}
	EditableCurveReferences.insert({ GeoHda->ContainerNode, CurveNodeRefs });
    }
}

void
HEMAX_Plugin::RemoveReferencesForEditableCurve(INode* Node)
{
    if (Node)
    {
	auto Search = EditableCurveReferences.find({ Node });

	if (Search != EditableCurveReferences.end())
	{
	    std::vector<HEMAX_ReferenceMaker*> Refs = Search->second;

	    for (int i = 0; i < Refs.size(); ++i)
	    {
		delete Refs[i];
	    }

	    Refs.clear();
	    EditableCurveReferences.erase(Search);
	}
    }
}

void
HEMAX_Plugin::DestroyAllEditableNodeReferences()
{
    for (auto It = EditableCurveReferences.begin(); It != EditableCurveReferences.end(); ++It)
    {
	auto References = It->second;

	for (int r = 0; r < References.size(); ++r)
	{
	    delete References[r];
	}

	References.clear();
    }

    EditableCurveReferences.clear();
}

void
HEMAX_Plugin::HandleParameterInputCleared(HEMAX_Node* Node,
                                          HEMAX_Parameter Parameter,
                                          bool UpdateHda)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	Clear3dsmaxHdaParameterInput(Hda, &Parameter);

        if (UpdateHda)
        {
            Hda->Cook3dsmaxHda();
            Hda->Update3dsmaxHda();
            GetCOREInterface()->ForceCompleteRedraw();
        }
    }
}

void
HEMAX_Plugin::HandleParameterInputSelection(HEMAX_Node* Node, HEMAX_Parameter Parameter, INode* InputNodeToUse)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	HEMAX_3dsMaxInput* MaxInput = Set3dsmaxHdaParameterInput(Hda, &Parameter, InputNodeToUse);
	Hda->Cook3dsmaxHda();
	Hda->Update3dsmaxHda();
	GetCOREInterface()->ForceCompleteRedraw();
    }
}

void
HEMAX_Plugin::HandleSubnetworkInputCleared(HEMAX_Node* Node, int Subnetwork,
                                           bool UpdateHda)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	Clear3dsmaxHdaSubnetworkInput(Hda, Subnetwork);

        if (UpdateHda)
        {
            Hda->Cook3dsmaxHda();
            Hda->Update3dsmaxHda();
            GetCOREInterface()->ForceCompleteRedraw();
        }
    }
}

void
HEMAX_Plugin::HandleSubnetworkInputSelection(HEMAX_Node* Node, int Subnetwork, INode* InputNodeToUse)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	Set3dsmaxHdaSubnetworkInput(Hda, Subnetwork, InputNodeToUse);
	Hda->Cook3dsmaxHda();
	Hda->Update3dsmaxHda();
	GetCOREInterface()->ForceCompleteRedraw();
    }
}

void
HEMAX_Plugin::Clear3dsmaxHdaParameterInput(HEMAX_3dsmaxHda* Hda, HEMAX_Parameter* Parameter)
{
    if (Hda)
    {
	HEMAX_InputInstance* OldInput = Hda->FindParameterInput(Parameter->Info.id);
	ULONG OldInputHandle = -1;

	if (OldInput)
	{
	    OldInputHandle = OldInput->MaxInput->Get3dsMaxNodeHandle();
	    delete OldInput->MergeNode;
	    OldInput->MergeNode = nullptr;
	}

	Hda->ClearParameterInputNode(Parameter->Info.id);
	Parameter->UpdateInputNode(-1);
	Hda->UpdateParameterInputNode(Parameter->Info.id);
	Hda->UpdateInputNodeCustomAttribute(*Parameter, nullptr);

	if (OldInput)
	{
	    OldInput->MaxInput->DecrementInputCount();
	    OldInput->MaxInput->RemoveParameterUsage(Hda->Hda.MainNode, Parameter->GetName());

	    if (OldInput->MaxInput->GetInputCount() <= 0)
	    {
		PluginStore->Delete3dsMaxInput(OldInputHandle);
	    }
	}
    }
}

HEMAX_3dsMaxInput*
HEMAX_Plugin::Set3dsmaxHdaParameterInput(HEMAX_3dsmaxHda* Hda, HEMAX_Parameter* Parameter, INode* InputNodeToUse)
{
    HEMAX_3dsMaxInput* MaxInput = nullptr;

    if (Hda)
    {
	HEMAX_InputInstance* OldInput = Hda->FindParameterInput(Parameter->Info.id);

	if (OldInput)
	{
	    Clear3dsmaxHdaParameterInput(Hda, Parameter);
	    delete OldInput->MergeNode;
	    OldInput->MergeNode = nullptr;
	}

	ULONG SelectedInputNode = 0;

	if (InputNodeToUse)
	{
	    SelectedInputNode = InputNodeToUse->GetHandle();
	}
	else
	{
	    SelectedInputNode = DetermineInputNodeSelection(Hda);
	}

	if (SelectedInputNode != 0)
	{
	    MaxInput = PluginStore->Find3dsMaxInput(SelectedInputNode);

	    if (!MaxInput)
	    {
		MaxInput = Create3dsMaxInput(SelectedInputNode);
		PluginStore->Add3dsMaxInput(MaxInput);
	    }

	    if (MaxInput)
	    {
		HEMAX_InputMerge* Merge;

		if (Hda->Type == HEMAX_MODIFIER_HDA)
		{
		    HEMAX_ModifierHda* ModifierHda = static_cast<HEMAX_ModifierHda*>(Hda);
		    INode* HoudiniAssetNode = ModifierHda->ContainerNode;
		    INode* SelectedInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput->Get3dsMaxNodeHandle());
		    Merge = new HEMAX_InputMerge(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(HoudiniAssetNode, SelectedInputNode));
		}
		else
		{
		    Merge = new HEMAX_InputMerge(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(SelectedInputNode)));
		}

		Merge->MergeInput(*MaxInput->GetInputNode());
		HEMAX_InputInstance* InputInstance = new HEMAX_InputInstance(MaxInput, Merge);

		Hda->SetParameterInput(Parameter->Info.id, InputInstance);
		Hda->UpdateParameterInputNode(Parameter->Info.id);
		Hda->UpdateInputNodeCustomAttribute(*Parameter, MaxInput->GetInputNode());
		MaxInput->IncrementInputCount();
		MaxInput->AddParameterUsage(Hda->Hda.MainNode, Parameter->GetName());
	    }
	}
    }

    return MaxInput;
}

void
HEMAX_Plugin::Clear3dsmaxHdaSubnetworkInput(HEMAX_3dsmaxHda* Hda, int Subnetwork)
{
    if (Hda)
    {
	HEMAX_InputInstance* OldInput = Hda->FindSubnetworkInput(Subnetwork);

	if (OldInput)
	{
	    ULONG OldInputHandle = -1;

	    if (OldInput->MaxInput)
	    {
		OldInputHandle = OldInput->MaxInput->Get3dsMaxNodeHandle();
	    }

	    Hda->ClearSubnetworkInput(Subnetwork);
	    Hda->UpdateSubnetworkInput(Subnetwork);
	    Hda->UpdateSubnetworkCustomAttribute(Subnetwork, nullptr);

	    if (OldInput)
	    {
		delete OldInput->MergeNode;

		OldInput->MaxInput->DecrementInputCount();
		OldInput->MaxInput->RemoveSubnetworkUsage(Hda->Hda.MainNode, Subnetwork);

		if (OldInput->MaxInput->GetInputCount() <= 0)
		{
		    PluginStore->Delete3dsMaxInput(OldInputHandle);
		}
	    }
	}
    }
}

HEMAX_3dsMaxInput*
HEMAX_Plugin::Set3dsmaxHdaSubnetworkInput(HEMAX_3dsmaxHda* Hda, int Subnetwork, INode* InputNodeToUse)
{
    HEMAX_3dsMaxInput* MaxInput = nullptr;

    if (Hda)
    {
	ULONG SelectedInputNode = 0;

	if (InputNodeToUse)
	{
	    SelectedInputNode = InputNodeToUse->GetHandle();
	}
	else
	{
	    SelectedInputNode = DetermineInputNodeSelection(Hda);
	}

	if (SelectedInputNode != 0)
	{
	    MaxInput = PluginStore->Find3dsMaxInput(SelectedInputNode);

	    if (!MaxInput)
	    {
		MaxInput = Create3dsMaxInput(SelectedInputNode);
		PluginStore->Add3dsMaxInput(MaxInput);
	    }

	    if (MaxInput)
	    {
		HEMAX_InputMerge* Merge;

		if (Hda->Type == HEMAX_MODIFIER_HDA)
		{
		    HEMAX_ModifierHda* ModifierHda = static_cast<HEMAX_ModifierHda*>(Hda);
		    INode* HoudiniAssetNode = ModifierHda->ContainerNode;
		    INode* SelectedInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput->Get3dsMaxNodeHandle());
		    Merge = new HEMAX_InputMerge(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(HoudiniAssetNode, SelectedInputNode));
		}
		else
		{
		    Merge = new HEMAX_InputMerge(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(SelectedInputNode)));
		}

		Merge->MergeInput(*MaxInput->GetInputNode());
		HEMAX_InputInstance* InputInstance = new HEMAX_InputInstance(MaxInput, Merge);

		Hda->SetSubnetworkInput(Subnetwork, InputInstance);
		Hda->UpdateSubnetworkInput(Subnetwork);
		Hda->UpdateSubnetworkCustomAttribute(Subnetwork, MaxInput->GetInputNode());

		MaxInput->IncrementInputCount();
		MaxInput->AddSubnetworkUsage(Hda->Hda.MainNode, Subnetwork);
	    }
	}
    }

    return MaxInput;
}

void
HEMAX_Plugin::RefreshInputConnections(HEMAX_3dsmaxHda* Hda)
{
    for (int i = 0; i < Hda->SubnetworkNodeInputs.size(); i++)
    {
	if (Hda->SubnetworkNodeInputs[i])
	{
	    Hda->SubnetworkNodeInputs[i]->RefreshConnection();
	}
    }
    for (auto It = Hda->InputNodeMap.begin(); It != Hda->InputNodeMap.end(); It++)
    {
	if (It->second)
	{
	    It->second->RefreshConnection();
	}
    }
}

void
HEMAX_Plugin::UpdateDirtiedInputNodes(HEMAX_3dsmaxHda* Hda)
{
    for (int i = 0; i < Hda->Hda.MainNode.Info.inputCount; i++)
    {
	HEMAX_InputInstance* MaxInput = Hda->SubnetworkNodeInputs[i];

	if (MaxInput)
	{
	    if (MaxInput->MaxInput && MaxInput->MaxInput->IsDirty())
	    {
		MaxInput->MaxInput->UpdateInputNode();
		delete MaxInput->MergeNode;

		if (Hda->Type == HEMAX_MODIFIER_HDA)
		{
		    HEMAX_ModifierHda* ModifierHda = static_cast<HEMAX_ModifierHda*>(Hda);
		    INode* HoudiniAssetNode = ModifierHda->ContainerNode;
		    INode* SelectedInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput->MaxInput->Get3dsMaxNodeHandle());
		    MaxInput->MergeNode = new HEMAX_InputMerge(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(HoudiniAssetNode, SelectedInputNode));
		}
		else
		{
		    MaxInput->MergeNode = new HEMAX_InputMerge(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(MaxInput->MaxInput->Get3dsMaxNodeHandle())));
		}

		MaxInput->MergeNode->MergeInput(*MaxInput->MaxInput->GetInputNode());

		Hda->SetSubnetworkInput(i, MaxInput);
		Hda->UpdateSubnetworkInput(i);
		Hda->UpdateSubnetworkCustomAttribute(i, MaxInput->MaxInput->GetInputNode());

	    }
	    else
	    {
		// If it's not dirty, the Transform still needs to be updated because 3ds Max does not send
		// NotifyRefChanged messages for keyframed transforms
		if (Hda->Type == HEMAX_MODIFIER_HDA)
		{
		    HEMAX_ModifierHda* ModifierHda = static_cast<HEMAX_ModifierHda*>(Hda);
		    MaxInput->MergeNode->PushMaxTransform(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(ModifierHda->ContainerNode, GetCOREInterface()->GetINodeByHandle(MaxInput->MaxInput->Get3dsMaxNodeHandle())));
		}
		else
		{
		    MaxInput->MergeNode->PushMaxTransform(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(MaxInput->MaxInput->Get3dsMaxNodeHandle())));
		}
	    }
	}
    }

    for (auto It = Hda->InputNodeMap.begin(); It != Hda->InputNodeMap.end(); It++)
    {
	if (It->second)
	{
	    HEMAX_InputInstance* ParameterInput = It->second;
	    if (ParameterInput->MaxInput && ParameterInput->MaxInput->IsDirty())
	    {
		ParameterInput->MaxInput->UpdateInputNode();
		delete ParameterInput->MergeNode;

		if (Hda->Type == HEMAX_MODIFIER_HDA)
		{
		    HEMAX_ModifierHda* ModifierHda = static_cast<HEMAX_ModifierHda*>(Hda);
		    INode* HoudiniAssetNode = ModifierHda->ContainerNode;
		    INode* SelectedInputNode = GetCOREInterface()->GetINodeByHandle(ParameterInput->MaxInput->Get3dsMaxNodeHandle());
		    ParameterInput->MergeNode = new HEMAX_InputMerge(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(HoudiniAssetNode, SelectedInputNode));
		}
		else
		{
		    ParameterInput->MergeNode = new HEMAX_InputMerge(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(ParameterInput->MaxInput->Get3dsMaxNodeHandle())));
		}

		ParameterInput->MergeNode->MergeInput(*(ParameterInput->MaxInput->GetInputNode()));

		Hda->SetParameterInput(It->first, ParameterInput);
		Hda->UpdateParameterInputNode(It->first);
		Hda->UpdateInputNodeCustomAttribute(*(Hda->Hda.MainNode.GetParameter(It->first)), ParameterInput->MaxInput->GetInputNode());
	    }
	    else
	    {
		// If it's not dirty, the Transform still needs to be updated because 3ds Max does not send
		// NotifyRefChanged messages for keyframed transforms
		if (Hda->Type == HEMAX_MODIFIER_HDA)
		{
		    HEMAX_ModifierHda* ModifierHda = static_cast<HEMAX_ModifierHda*>(Hda);
		    ParameterInput->MergeNode->PushMaxTransform(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(ModifierHda->ContainerNode, GetCOREInterface()->GetINodeByHandle(ParameterInput->MaxInput->Get3dsMaxNodeHandle())));
		}
		else
		{
		    ParameterInput->MergeNode->PushMaxTransform(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(ParameterInput->MaxInput->Get3dsMaxNodeHandle())));
		}
	    }
	}
    }
}

void
HEMAX_Plugin::Handle3dsMaxInputSourceDeletion(HEMAX_3dsMaxInput* MaxInput)
{
    if (MaxInput)
    {
	std::vector<HEMAX_InputUsage>& Usages = MaxInput->GetInputUsages();

	for (int i = (int)Usages.size() - 1; i >= 0; i--)
	{
	    if (Usages[i].Subnetwork == -1)
	    {
		// Parameter input
		HEMAX_3dsmaxHda* MaxHda = PluginStore->Find3dsmaxHda(Usages[i].Node);

		if (MaxHda)
		{
		    HEMAX_Parameter* Parameter = MaxHda->Hda.MainNode.GetParameter(Usages[i].Parameter);
		    HandleParameterInputCleared(&Usages[i].Node, *Parameter);
		}
	    }
	    else
	    {
		// Subnetwork input
		HandleSubnetworkInputCleared(&Usages[i].Node, Usages[i].Subnetwork);
	    }
	}
    }
}

void
HEMAX_Plugin::Clear3dsmaxHdaInputConnections(HEMAX_3dsmaxHda* Hda,
                                             bool UpdateHda)
{
    for (int i = 0; i < Hda->Hda.MainNode.Info.inputCount; i++)
    {
	if (Hda->SubnetworkNodeInputs[i])
	{
	    HandleSubnetworkInputCleared(&Hda->Hda.MainNode, i, UpdateHda);
	}
    }

    std::vector<HEMAX_Parameter> ParameterInputs = Hda->GetAllParametersWithInputs();

    for (int i = 0; i < ParameterInputs.size(); i++)
    {
	if (ParameterInputs[i].Type != HEMAX_PARAM_INVALID)
	{
	    HandleParameterInputCleared(&Hda->Hda.MainNode, ParameterInputs[i],
                                        UpdateHda);
	}
    }
}

void
HEMAX_Plugin::HandleRecookRequest(HEMAX_Node* Node)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	UpdateEntireHda(Hda);
    }
}

void
HEMAX_Plugin::ReloadAssetDefinition(HEMAX_Node *Node)
{
    HEMAX_3dsmaxHda *Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	HEMAX_Asset *Asset = PluginStore->FindAsset(Hda->Hda.HdaAsset.Path);
	if (Asset)
	{
	    Asset->UpdateAssetDefinition();

	    std::vector<HEMAX_3dsmaxHda*> HdasToReload = PluginStore->FindAllHdasUsingAssetDefinition(Asset);

	    for (int i = 0; i < HdasToReload.size(); i++)
	    {
		if (HdasToReload[i]->Type == HEMAX_GEOMETRY_HDA)
		{
		    int AssetIndex = HdasToReload[i]->Hda.HdaAssetIndex;

		    HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(HdasToReload[i]);

		    HEMAX_GeometryHda* CreatedAsset =
                        new HEMAX_GeometryHda();
		    CreatedAsset->Create(*Asset, AssetIndex);
		    ConstructReferencesForEditableCurve(CreatedAsset);
		    PluginStore->Add3dsmaxHda(CreatedAsset->ContainerNode->GetHandle(), CreatedAsset);

		    // Reconnect the subnetwork inputs

		    for (int s = 0; s < GeometryHda->SubnetworkNodeInputs.size(); s++)
		    {
			if (s < CreatedAsset->Hda.MainNode.Info.inputCount)
			{
                            if (!GeometryHda->SubnetworkNodeInputs[s])
                                continue;

			    ULONG InputINodeHandle = GeometryHda->SubnetworkNodeInputs[s]->MaxInput->Get3dsMaxNodeHandle();
			    INode* InputINode = GetCOREInterface()->GetINodeByHandle(InputINodeHandle);
			    HandleSubnetworkInputSelection(&(CreatedAsset->Hda.MainNode), s, InputINode);
			}
		    }

		    // Reconnect any operator path parameter inputs

		    for (auto InputMapIt = GeometryHda->InputNodeMap.begin(); InputMapIt != GeometryHda->InputNodeMap.end(); InputMapIt++)
		    {
			HEMAX_ParameterId ParmId = InputMapIt->first;
			HEMAX_InputInstance* InputInstance = InputMapIt->second;

			HEMAX_Parameter* OldParm = GeometryHda->Hda.MainNode.GetParameter(ParmId);
			if (OldParm)
			{
			    std::string ParmName = OldParm->GetName();  
			    HEMAX_Parameter* NewParm = CreatedAsset->Hda.MainNode.GetParameter(ParmName);
			    if (NewParm)
			    {
				ULONG InputINodeHandle = InputInstance->MaxInput->Get3dsMaxNodeHandle();
				INode* InputINode = GetCOREInterface()->GetINodeByHandle(InputINodeHandle);
				HandleParameterInputSelection(&(CreatedAsset->Hda.MainNode), *NewParm, InputINode);
			    }
			}
		    }

		    CreatedAsset->CopyAllParameterValues(*GeometryHda);
		    UpdateEntireHda(CreatedAsset);

		    Clear3dsmaxHdaInputConnections(HdasToReload[i]);

		    std::wstring ContainerName = GeometryHda->ContainerNode->GetName();

		    PluginStore->Delete3dsmaxHda(GeometryHda->ContainerNode->GetHandle());

		    CreatedAsset->ContainerNode->SetName(ContainerName.c_str());

		    GetCOREInterface()->ForceCompleteRedraw();
		}
		else if (HdasToReload[i]->Type == HEMAX_MODIFIER_HDA)
		{
		    HEMAX_ModifierHda* ModifierHda = static_cast<HEMAX_ModifierHda*>(HdasToReload[i]);
		    INode* Node = ModifierHda->ContainerNode;
		    int AssetIndex = HdasToReload[i]->Hda.HdaAssetIndex;
		    int ModifierIndex, DerivedObjIndex;

		    if (GetCOREInterface7()->FindModifier(*(ModifierHda->ContainerNode), *(ModifierHda->DisplayGeometry), ModifierIndex, DerivedObjIndex))
		    {
			ManualModifierAddInProgress = true;

			HEMAX_Modifier* ModifierPlugin = (HEMAX_Modifier*)GetCOREInterface()->CreateInstance(OSM_CLASS_ID, HEMAX_Modifier_CLASS_ID);
			GetCOREInterface7()->AddModifier(*Node, *ModifierPlugin, ModifierIndex);

			HEMAX_ModifierHda* NewModifierHda = new HEMAX_ModifierHda;

			if (NewModifierHda->Create(*Asset, AssetIndex, ModifierPlugin, Node))
			{
			    ModifierPlugin->RegisterPlugin(this);
			    PluginStore->Add3dsmaxHda(Node->GetHandle(), NewModifierHda);
			    NewModifierHda->CopyAllParameterValues(*ModifierHda);
			    UpdateEntireHda(NewModifierHda);

			    std::wstring ModName = ModifierHda->DisplayGeometry->GetName();
			    NewModifierHda->DisplayGeometry->SetName(ModName.c_str());
			}
			else
			{
			    GetCOREInterface7()->DeleteModifier((*Node), ModifierIndex);
			    delete ModifierPlugin;
			}

			ManualModifierAddInProgress = false;

			GetCOREInterface7()->DeleteModifier(*(ModifierHda->ContainerNode), ModifierIndex+1);
		    }		    
		}
	    }
	}
        else
        {
            std::string Msg = "Could not reload the asset definition of HDA <" +
                Hda->Hda.HdaAsset.Path + "> because the HDA is not currently "
                "loaded.";
            HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR);
        }
    }
}

void
HEMAX_Plugin::HandleParameterIntValuesUpdate(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<int> IntValues)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	Parameter.UpdateIntVals(IntValues);
	Hda->UpdateIntCustomAttribute(Parameter, IntValues);

	if (Node->AutoRecookOnParameterUpdate)
	{
	    UpdateEntireHda(Hda);
	}
    }
}

void
HEMAX_Plugin::HandleParameterFloatValuesUpdate(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<float> FloatValues)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	Parameter.UpdateFloatVals(FloatValues);
	Hda->UpdateFloatCustomAttribute(Parameter, FloatValues);

	if (Node->AutoRecookOnParameterUpdate)
	{
	    UpdateEntireHda(Hda);
	}
    }
}

void
HEMAX_Plugin::HandleParameterStringValuesUpdate(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<std::string> StringValues)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	Parameter.UpdateStringVals(StringValues);
	Hda->UpdateStringCustomAttribute(Parameter, StringValues);

	if (Node->AutoRecookOnParameterUpdate)
	{
	    UpdateEntireHda(Hda);
	}
    }
}

void
HEMAX_Plugin::HandleMultiParameterUpdate(HEMAX_Node* Node, HEMAX_Parameter Parameter, HEMAX_MultiParameterChangeInfo ChangeInfo)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
	Hda->UpdateMultiParameterList(Parameter);
	UpdateEntireHda(Hda);
    }
}

void
HEMAX_Plugin::UpdateEntireHda(HEMAX_3dsmaxHda* Hda, bool ForceRedraw)
{
    if (Hda->Type == HEMAX_GEOMETRY_HDA)
    {
	HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(Hda);
	for (int i = 0; i < GeometryHda->EditableCurves.size(); i++)
	{
	    if (GeometryHda->EditableCurves[i].Dirty)
	    {
		GeometryHda->PushEditableNodeChanges(GeometryHda->EditableCurves[i]);
	    }
	}
    }

    RefreshInputConnections(Hda);
    UpdateDirtiedInputNodes(Hda);
    Hda->Cook3dsmaxHda();
    Hda->Update3dsmaxHda();
    Hda->UpdateAllCustomAttributes();

    if (ForceRedraw)
    {
	GetCOREInterface()->ForceCompleteRedraw();

	if (Hda->Type == HEMAX_MODIFIER_HDA)
	{
	    HEMAX_ModifierHda* ModifierHda = static_cast<HEMAX_ModifierHda*>(Hda);
	    if (ModifierHda->ContainerNode && ModifierHda->DisplayGeometry)
	    {
		if (ModifierHda->DisplayGeometry->CheckIfTopOfStack(ModifierHda->ContainerNode))
		{
		    ModifierHda->DisplayGeometry->ApplyMaterialsToNode();
		    ModifierHda->DisplayGeometry->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
		}
	    }
	}
    }
}

void
HEMAX_Plugin::ReloadHdaFromCustomAttributes(HEMAX_3dsmaxHda* MaxHda)
{
    std::vector<HEMAX_SubnetworkInputMapping> Unconnected = MaxHda->ReloadSubnetworkInputsFromCustomAttributes();
    for (int s = 0; s < Unconnected.size(); s++)
    {
	if (Unconnected[s].Node)
	{
	    Set3dsmaxHdaSubnetworkInput(MaxHda, Unconnected[s].Subnetwork, Unconnected[s].Node);
	}
    }

    std::vector<HEMAX_ParameterInputMapping> UnconnectedInputs = MaxHda->ReloadParametersFromCustomAttributes();
    for (int s = 0; s < UnconnectedInputs.size(); s++)
    {
	if (UnconnectedInputs[s].Node)
	{
	    Set3dsmaxHdaParameterInput(MaxHda,
				       MaxHda->Hda.MainNode.GetParameter(UnconnectedInputs[s].ParameterName),
				       UnconnectedInputs[s].Node);
	}
    }

    MaxHda->Hda.MainNode.Cook();
    MaxHda->Hda.Update();
}

bool
HEMAX_Plugin::RemoveAsset(std::string AssetPath)
{
    if (PluginStore->RemoveAsset(AssetPath))
    {
        PluginEvents->AssetRemoved();
	return true;
    }
    else
    {
	return false;
    }
}

HEMAX_Shelf*
HEMAX_Plugin::GetToolShelf()
{
    return ToolShelf;
}

INode*
HEMAX_Plugin::CloneGeometryHda(HEMAX_GeometryHda* MaxHda)
{
    INode* CloneContainerNode = nullptr;
    HEMAX_GeometryHda* Clone = CreateGeometryHDA(MaxHda->Hda.HdaAsset.Path,
                                                 MaxHda->Hda.HdaAssetIndex);
    if (Clone)
    {
        // Start with all inputs
	for (int i = 0; i < MaxHda->SubnetworkNodeInputs.size(); i++)
	{
	    if (MaxHda->SubnetworkNodeInputs[i])
	    {
		HEMAX_3dsMaxInput* MaxInput =
                    MaxHda->SubnetworkNodeInputs[i]->MaxInput;
		ULONG MaxInputNodeHandle =
                    MaxInput->GetInputNode()->GetMaxNodeHandle();
		Set3dsmaxHdaSubnetworkInput(
                    Clone,
                    i,
                    GetCOREInterface()->GetINodeByHandle(MaxInputNodeHandle));
	    }
	}

	for (auto InputIt = MaxHda->InputNodeMap.begin();
             InputIt != MaxHda->InputNodeMap.end();
             InputIt++)
	{
	    if (InputIt->second)
	    {
		HEMAX_3dsMaxInput* MaxInput = InputIt->second->MaxInput;
		ULONG MaxInputNodeHandle =
                    MaxInput->GetInputNode()->GetMaxNodeHandle();
		HEMAX_Parameter* SourceParameter =
                    MaxHda->Hda.MainNode.GetParameter(InputIt->first);

		if (SourceParameter)
		{
		    std::string SourceParmName = SourceParameter->GetName();

		    HEMAX_Parameter* CloneParameter =
                        Clone->Hda.MainNode.GetParameter(SourceParmName);
		    if (CloneParameter
                        && CloneParameter->Type == HEMAX_PARAM_NODE)
		    {
			Set3dsmaxHdaParameterInput(
                            Clone,
                            CloneParameter,
                            GetCOREInterface()->GetINodeByHandle(
                                                        MaxInputNodeHandle));
		    }
		}
	    }
	}

	Clone->CopyAllParameterValues(*MaxHda);
	UpdateEntireHda(Clone);

        CloneContainerNode = Clone->ContainerNode;

        INode* SourceContainerNode = MaxHda->ContainerNode;
        TimeValue CurTime = GetCOREInterface()->GetTime();
        CloneContainerNode->SetNodeTM(CurTime,
                                      SourceContainerNode->GetNodeTM(CurTime));

        PluginEvents->SelectionSetChanged(nullptr);
        PluginEvents->SelectionSetChanged(Clone);
    }

    return CloneContainerNode;
}

Modifier*
HEMAX_Plugin::CloneModifierHda(HEMAX_3dsmaxHda* MaxHda, INode* MaxNode)
{
    ManualModifierAddInProgress = true;

    HEMAX_ModifierHda* Clone = CreateModifierHDA(MaxNode, MaxHda->Hda.HdaAsset.Path, MaxHda->Hda.HdaAssetIndex);
    if (Clone)
    {
	// Start with all inputs
	for (int i = 0; i < MaxHda->SubnetworkNodeInputs.size(); i++)
	{
	    if (MaxHda->SubnetworkNodeInputs[i])
	    {
		HEMAX_3dsMaxInput* MaxInput = MaxHda->SubnetworkNodeInputs[i]->MaxInput;
		ULONG MaxInputNodeHandle = MaxInput->GetInputNode()->GetMaxNodeHandle();
		Set3dsmaxHdaSubnetworkInput(Clone, i, GetCOREInterface()->GetINodeByHandle(MaxInputNodeHandle));
	    }
	}

	for (auto InputIt = MaxHda->InputNodeMap.begin(); InputIt != MaxHda->InputNodeMap.end(); InputIt++)
	{
	    if (InputIt->second)
	    {
		HEMAX_3dsMaxInput* MaxInput = InputIt->second->MaxInput;
		ULONG MaxInputNodeHandle = MaxInput->GetInputNode()->GetMaxNodeHandle();
		HEMAX_Parameter* SourceParameter = MaxHda->Hda.MainNode.GetParameter(InputIt->first);
		std::string SourceParmName = SourceParameter->GetName();

		HEMAX_Parameter* CloneParameter = Clone->Hda.MainNode.GetParameter(SourceParmName);
		if (CloneParameter->Type == HEMAX_PARAM_NODE)
		{
		    Set3dsmaxHdaParameterInput(Clone, CloneParameter, GetCOREInterface()->GetINodeByHandle(MaxInputNodeHandle));
		}
	    }
	}

	Clone->CopyAllParameterValues(*MaxHda);
	UpdateEntireHda(Clone);
        PluginEvents->SelectionSetChanged(Clone);

	return Clone->DisplayGeometry;
    }

    ManualModifierAddInProgress = false;

    return nullptr;
}

void
HEMAX_Plugin::TraverseHdaLoadPaths()
{
    std::string AutoLoadHdaDir;
    HEMAX_UserPrefs::Get().GetStringSetting(HEMAX_SETTING_HDA_LOAD_PATH,
        AutoLoadHdaDir);
    PluginStore->LoadAllAssetsInDirectory(AutoLoadHdaDir);

    std::string HdaLoadPath = HEMAX_Utilities::GetEnvVar(HEMAX_ENV_HDA_PATH);

    if (!HdaLoadPath.empty())
    {
	PluginStore->LoadAllAssetsInDirectory(HdaLoadPath);
    }
}

void
HEMAX_Plugin::InitializeLogPrintLevels()
{
    bool Setting;
    HEMAX_UserPrefs& Prefs = HEMAX_UserPrefs::Get();

    Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_ERRORS, Setting);
    HEMAX_Logger::Instance().ConfigurePrintLevels(HEMAX_LOG_LEVEL_ERROR,
        Setting);

    Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_WARNINGS, Setting);
    HEMAX_Logger::Instance().ConfigurePrintLevels(HEMAX_LOG_LEVEL_WARN,
        Setting);

    Prefs.GetBoolSetting(HEMAX_SETTING_DEBUG_PRINT_INFO, Setting);
    HEMAX_Logger::Instance().ConfigurePrintLevels(HEMAX_LOG_LEVEL_INFO,
        Setting);
}
