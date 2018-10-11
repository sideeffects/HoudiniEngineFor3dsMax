#include "HEMAX_Plugin.h"

#include "HEMAX_SessionManager.h"

#include "icustattribcontainer.h"
#include "custattrib.h"
#include "modstack.h"
#include "HEMAX_CustomAttributeContainer.h"
#include "UI/HEMAX_AssetSelection.h"
#include "UI/HEMAX_HDASelectionDialog.h"
#include "HEMAX_Logger.h"
#include "HEMAX_Undo.h"
#include "HEMAX_HoudiniApi.h"
#include "HEMAX_ReferenceMaker.h"
#include "HEMAX_GeometryMouseCallback.h"
#include "HEMAX_3dsMaxInput.h"
#include "HEMAX_Time.h"
#include "HEMAX_Hda.h"
#include "HEMAX_MaxScriptInterface.h"

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
UndoHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandleUndo(static_cast<MCHAR*>(Info->callParam));
}

void
RedoHandler(void* PluginPointer, NotifyInfo* Info)
{
    HEMAX_Plugin* PluginRegistrar = static_cast<HEMAX_Plugin*>(PluginPointer);
    PluginRegistrar->HandleRedo(static_cast<MCHAR*>(Info->callParam));
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

HEMAX_Plugin::HEMAX_Plugin(Interface* Interface, HEMAX_UI* UserInterface, HMODULE LibHAPIL, std::string HapiToolsDir)
    : MaxInterface(Interface), PluginUserInterface(UserInterface), ManualModifierAddInProgress(false)
{
    if (LibHAPIL)
    {
        HEMAX_HoudiniApi::InitializeHAPI(LibHAPIL);

        if (HEMAX_HoudiniApi::IsHAPIInitialized())
        {
            PluginStore = new HEMAX_Store;
            UserPreferences = new HEMAX_UserPrefs;
            ToolShelf = new HEMAX_Shelf(PluginStore);

            PluginUserInterface->AttachPlugin(this);

            if (GetAutoStartWindowOption())
            {
                PluginUserInterface->ShowHEMAXWindow();
            }
            else
            {
                PluginUserInterface->UnshowHEMAXWindow();
            }

            PluginUserInterface->TriggerOutOfProcessSession();

            PluginStore->UserHdaRepository = GetHdaRepoDirectory();

            ToolShelf->LoadShelvesFromJson(UserPreferences->GetPluginConfigFolder());
            ToolShelf->AddShelfDirectory(HapiToolsDir, "Default", true);

            RegisterNotification((NOTIFYPROC)MAXSelectionSetChangedCallback, (void*)this, NOTIFY_SELECTIONSET_CHANGED);
            RegisterNotification((NOTIFYPROC)FilePreOpenHandler, (void*)this, NOTIFY_FILE_PRE_OPEN);
            RegisterNotification((NOTIFYPROC)FilePostOpenHandler, (void*)this, NOTIFY_FILE_POST_OPEN);
            RegisterNotification((NOTIFYPROC)PreNewAllHandler, (void*)this, NOTIFY_SYSTEM_PRE_NEW);
            RegisterNotification((NOTIFYPROC)NodePreDeleteHandler, (void*)this, NOTIFY_SEL_NODES_PRE_DELETE);
            RegisterNotification((NOTIFYPROC)ModifierPostAddHandler, (void*)this, NOTIFY_POST_MODIFIER_ADDED);
            RegisterNotification((NOTIFYPROC)ModifierPreDeleteHandler, (void*)this, NOTIFY_PRE_MODIFIER_DELETED);
            RegisterNotification((NOTIFYPROC)UndoHandler, (void*)this, NOTIFY_SCENE_UNDO);
            RegisterNotification((NOTIFYPROC)RedoHandler, (void*)this, NOTIFY_SCENE_REDO);            
            RegisterNotification((NOTIFYPROC)NodeCreatedHandler, (void*)this, NOTIFY_NODE_CREATED);
            RegisterNotification((NOTIFYPROC)RenderPreEvalHandler, (void*)this, NOTIFY_RENDER_PREEVAL);

            GetCOREInterface()->RegisterTimeChangeCallback(this);

			HEMAX_MaxScriptInterface::PluginInstance = this;

            PluginUserInterface->Update();
        }
        else
        {
            HEMAX_Logger::Instance().AddEntry("HEMAX_Plugin: libHAPIL.dll was loaded, but the function addresses could not be determined.",
                HEMAX_LOG_LEVEL_ERROR);
        }
    }
}

HEMAX_Plugin::~HEMAX_Plugin()
{
    ToolShelf->SaveShelfToJson(UserPreferences->GetPluginConfigFolder());

    delete PluginStore;
    delete UserPreferences;
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
        UpdateEntireHda(Hdas[i], false);
    }
}

void
HEMAX_Plugin::SelectionSetChangeHandler()
{
    HEMAX_3dsmaxHda* AssetToSelect = nullptr;
    int SelectedMaxNodeCount = GetCOREInterface()->GetSelNodeCount();

    if (SelectedMaxNodeCount == 1)
    {
        bool HEMAXNodeFound = false;

        ULONG SelectedNode = GetCOREInterface()->GetSelNode(0)->GetHandle();
        AssetToSelect = PluginStore->Find3dsmaxHda(SelectedNode);
        
        if (!AssetToSelect)
        {
            bool AutoSelectRootNode;
            UserPreferences->GetBoolSetting(HEMAX_SETTING_GRAB_ROOT, AutoSelectRootNode);
            if (AutoSelectRootNode)
            {
                AssetToSelect = PluginStore->Find3dsmaxHdaViaChildGeometry(SelectedNode);

                if (AssetToSelect)
                {
                    ULONG AssetNode = AssetToSelect->GeometryHda.ContainerNode->GetHandle();
                    INode* SelectedMaxAssetNode = GetCOREInterface()->GetINodeByHandle(AssetNode);

                    if (SelectedMaxAssetNode)
                    {
                        theHold.Resume();
                        theHold.Begin();
                        GetCOREInterface()->SelectNode(SelectedMaxAssetNode, 0);
                        theHold.Accept(HEMAX_SELECT_HDA);
                        theHold.Suspend();
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
            AssetToSelect = PluginStore->Find3dsmaxHda(SelectedNode);

            if (AssetToSelect)
            {
                break;
            }
        }
    }
    
    PluginUserInterface->ChangeHdaSelection(AssetToSelect);
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
                std::vector<std::string> Paths = PluginStore->GetListOfLoadedAssets();
                PluginUserInterface->UpdateLoadedAssetLibrariesList(&Paths);
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
}

HEMAX_3dsmaxHda*
HEMAX_Plugin::CreateGeometryHDA(std::string Path, int AssetIndex)
{
    HEMAX_3dsmaxHda* CreatedAsset = PluginStore->CreateGeometryHda(Path, AssetIndex);

    if (CreatedAsset)
    {
        ConstructReferencesForEditableCurve(CreatedAsset);
        GetCOREInterface()->ForceCompleteRedraw();
        GetCOREInterface()->SelectNode(CreatedAsset->GeometryHda.ContainerNode);
    }

    return CreatedAsset;
}

HEMAX_3dsmaxHda*
HEMAX_Plugin::CreateModifierHDA(INode* Node, std::string Path, int AssetIndex)
{
	ManualModifierAddInProgress = true;
    HEMAX_3dsmaxHda* ModifierHda = nullptr;

    ModifierHda = PluginStore->CreateModifierHda(Node, Path, AssetIndex);

    if (ModifierHda)
    {
        ModifierHda->ModifierHda.DisplayGeometry->RegisterPlugin(this);
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
                HEMAX_3dsmaxHda* ModifierHda = CreateModifierHDA(SelectedNodes[i], Path, 0);
                if (ModifierHda)
                {
                    PluginUserInterface->ChangeHdaSelection(ModifierHda);
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

    ManualModifierAddInProgress = false;
}

void
HEMAX_Plugin::CloneHda(HEMAX_3dsmaxHda* MaxHda)
{
    if (MaxHda)
    {
        if (MaxHda->Type == HEMAX_GEOMETRY_HDA)
        {
            CloneGeometryHda(MaxHda);
        }
        else if (MaxHda->Type == HEMAX_MODIFIER_HDA)
        {
            CloneModifierHda(MaxHda, MaxHda->ModifierHda.ContainerNode);
        }
    }
}

void
HEMAX_Plugin::CopyHdaToNode(HEMAX_3dsmaxHda* MaxHda, INode* Node)
{
    if (MaxHda && Node)
    {
        GetCOREInterface()->SelectNode(Node);
        CloneModifierHda(MaxHda, Node);
    }
}

void
HEMAX_Plugin::HandleSceneFileOpening()
{
    PluginUserInterface->ChangeHdaSelection(nullptr, true);
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

        auto LoadedAssetList = PluginStore->GetListOfLoadedAssets();
        PluginUserInterface->UpdateLoadedAssetLibrariesList(&LoadedAssetList);
    }
}

void
HEMAX_Plugin::HandlePreNewAll()
{
    PluginUserInterface->ChangeHdaSelection(nullptr, true);
    DestroyAllEditableNodeReferences();
    PluginStore->EmptyOutStore();
    
    if (HEMAX_SessionManager::GetSessionManager().IsSessionActive())
    {
        TraverseHdaLoadPaths();
        ToolShelf->LoadToolAssets();
        PluginUserInterface->UpdateLoadedAssetLibrariesList(&PluginStore->GetListOfLoadedAssets());
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
            if (PluginUserInterface->GetCurrentHdaSelection() == Hda)
            {
                PluginUserInterface->ChangeHdaSelection(nullptr, true);
            }
            Clear3dsmaxHdaInputConnections(Hda);
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
                        HEMAX_3dsmaxHda* ModifierHda = new HEMAX_3dsmaxHda;

                        if (CreateModifier3dsmaxHda(*ModifierHda, *TheAsset, 0, ModifierPlugin, ModEvent->node))
                        {
                            ModifierPlugin->RegisterPlugin(this);
                            PluginStore->Add3dsmaxHda(ModEvent->node->GetHandle(), ModifierHda);
                            GetCOREInterface()->ForceCompleteRedraw();
                            ModifierHda->ModifierHda.DisplayGeometry->ApplyMaterialsToNode();
                            ModifierHda->ModifierHda.DisplayGeometry->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
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
                                    HEMAX_3dsmaxHda* ModifierHda = new HEMAX_3dsmaxHda;

                                    if (CreateModifier3dsmaxHda(*ModifierHda, *TheAsset, s, ModifierPlugin, ModEvent->node))
                                    {
                                        ModifierPlugin->RegisterPlugin(this);
                                        PluginStore->Add3dsmaxHda(ModEvent->node->GetHandle(), ModifierHda);
                                        GetCOREInterface()->ForceCompleteRedraw();
                                        ModifierHda->ModifierHda.DisplayGeometry->ApplyMaterialsToNode();
                                        ModifierHda->ModifierHda.DisplayGeometry->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
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
                    HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_Plugin: attempting to create a modifier plugin with asset <" + SelectedAssetPath + "> but it does not exist"), HEMAX_LOG_LEVEL_WARN);
                    GetCOREInterface7()->DeleteModifier(*(ModEvent->node), *(ModEvent->mod));
                }
            }
            else
            {
                GetCOREInterface7()->DeleteModifier(*(ModEvent->node), *(ModEvent->mod));
            }
        }
    }
}

void
HEMAX_Plugin::HandleModifierPreDelete(HEMAX_ModifierEvent* ModEvent)
{
    HEMAX_3dsmaxHda* ModifierHda = PluginStore->Find3dsmaxHda(ModEvent->node->GetHandle(), ModEvent->mod);

    if (ModifierHda)
    {
        if (ModifierHda == PluginUserInterface->GetCurrentHdaSelection())
        {
            PluginUserInterface->ChangeHdaSelection(nullptr, true);
        }

        Clear3dsmaxHdaInputConnections(ModifierHda);
        ModifierHda->ModifierHda.DisplayGeometry->CleanupHAPIComponents();
        PluginStore->Delete3dsmaxHda(ModEvent->node->GetHandle(), ModEvent->mod);
    }
}

void
HEMAX_Plugin::HandleUndo(std::wstring UndoEventName)
{
    /*
    if (UndoEventName == std::wstring(HEMAX_UNDO_GEO_HDA_CREATE) || UndoEventName == std::wstring(HEMAX_UNDO_CREATE_GENERIC))
    {
        HEMAX_MaxHoudiniAsset* UndoneAsset = PluginStore->FindUndoneGeometryHDA();

        if (UndoneAsset)
        {
            ((HEMAX_GeometryAsset*)UndoneAsset)->MarkUndone();
            PluginStore->ClearMaxHoudiniAsset(UndoneAsset->GetMaxNodeHandle(), false, nullptr);

            PluginUserInterface->UpdateHDASelection(nullptr);
        }
    }
    else if (UndoEventName == std::wstring(HEMAX_MOD_HDA_CREATE))
    {
        HEMAX_Modifier* UndoneModifier = dynamic_cast<HEMAX_Modifier*>(PluginStore->FindUndoneModifierHDA());

        if (UndoneModifier)
        {
            HEMAX_ModifierAsset* ModifierAsset = PluginStore->FindModifierAssetByHEMAXNode(UndoneModifier->GetNode());

            if (ModifierAsset)
            {
                UndoneModifier->MarkUndone();
                ModifierAsset->RemoveModifierFromStack(UndoneModifier->GetNode()->Info.id);

                PluginUserInterface->SelectHDAModifier(nullptr);

                if (ModifierAsset->GetNumberOfHDAModifiers() <= 0)
                {
                    PluginStore->RemoveModifierAsset(ModifierAsset->GetMaxNode()->GetHandle());
                }
            }
        }
    }
    */
}

void
HEMAX_Plugin::HandleRedo(std::wstring RedoEventName)
{
    /*
    if (RedoEventName == std::wstring(HEMAX_UNDO_GEO_HDA_CREATE) || RedoEventName == std::wstring(HEMAX_UNDO_CREATE_GENERIC))
    {
        INode* Stranded = SearchForStrandedGeometryHDA(GetCOREInterface()->GetRootNode());

        if (Stranded)
        {
            RedoCreateHDAObject(Stranded);
        }
        else
        {
            HEMAX_Logger::Instance().AddEntry("HEMAX_Plugin: while performing redo of an HDA object, could not reconnect to HAPI.", HEMAX_LOG_LEVEL_ERROR);
        }
    }
    */
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
                std::string AssetPath = PluginUserInterface->GetCurrentAssetSelection();
                HEMAX_Asset* CurrentAsset = PluginStore->FindAsset(AssetPath);

                if (CurrentAsset)
                {
                    if (CurrentAsset->AssetCount == 1)
                    {
                        HEMAX_3dsmaxHda* GeometryHda = new HEMAX_3dsmaxHda;
                        AddedNode->SetObjectRef(nullptr);
                        GeometryHda->GeometryHda.Dummy = new DummyObject;
                        AddedNode->SetObjectRef(GeometryHda->GeometryHda.Dummy);

                        CreateGeometry3dsmaxHda(*GeometryHda, AddedNode, *CurrentAsset, 0);
                        ConstructReferencesForEditableCurve(GeometryHda);
                        PluginStore->Add3dsmaxHda(GeometryHda->GeometryHda.ContainerNode->GetHandle(), GeometryHda);
                        GetCOREInterface()->ForceCompleteRedraw();
                        GetCOREInterface()->SelectNode(GetCOREInterface()->GetINodeByHandle(GeometryHda->GeometryHda.ContainerNode->GetHandle()));

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
                                    HEMAX_3dsmaxHda* GeometryHda = new HEMAX_3dsmaxHda;
                                    AddedNode->SetObjectRef(nullptr);
                                    GeometryHda->GeometryHda.Dummy = new DummyObject;
                                    AddedNode->SetObjectRef(GeometryHda->GeometryHda.Dummy);

                                    CreateGeometry3dsmaxHda(*GeometryHda, AddedNode, *CurrentAsset, 0);
                                    ConstructReferencesForEditableCurve(GeometryHda);
                                    PluginStore->Add3dsmaxHda(GeometryHda->GeometryHda.ContainerNode->GetHandle(), GeometryHda);
                                    GetCOREInterface()->ForceCompleteRedraw();
                                    GetCOREInterface()->SelectNode(GetCOREInterface()->GetINodeByHandle(GeometryHda->GeometryHda.ContainerNode->GetHandle()));

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
HEMAX_Plugin::RedoCreateHDAObject(INode* MHANode)
{
    /*
    ICustAttribContainer* CustomAttributes = MHANode->GetCustAttribContainer();

    if (CustomAttributes)
    {
        CustAttrib* AssetPathAttrib = CustomAttributes->GetCustAttrib(HEMAX_MAX_HOUDINI_ASSET_PATH_INDEX);

        if (AssetPathAttrib)
        {
            std::wstring WideName(AssetPathAttrib->GetName());
            std::string AttribName(WideName.begin(), WideName.end());
            std::string AssetPathName(HEMAX_MAX_HOUDINI_ASSET_PATH_NAME);

            if (AttribName == AssetPathName)
            {
                const wchar_t* WidePath;
                AssetPathAttrib->GetParamBlock(0)->GetValue(0, 0, WidePath, FOREVER);

                std::wstring WStringPath(WidePath);
                std::string AssetPath(WStringPath.begin(), WStringPath.end());

                HEMAX_Asset* TheAsset = PluginStore->FindAsset(AssetPath);

                if (!TheAsset)
                {
                    PluginStore->LoadNewAsset(AssetPath);
                    std::vector<std::string> LoadedAssetsList = PluginStore->GetListOfLoadedAssets();
                    PluginUserInterface->UpdateLoadedAssetLibrariesList(&LoadedAssetsList);
                }

                TheAsset = PluginStore->FindAsset(AssetPath);

                if (TheAsset)
                {
                    //HEMAX_MaxHoudiniAsset* RecreatedAsset = nullptr;

                    if (TheAsset->AssetCount == 1)
                    {
                        //RecreatedAsset = RedoGeometryAsset(TheAsset, 0, MHANode->GetHandle());
                    }
                    else
                    {
                        CustAttrib* AssetLibraryIndex = CustomAttributes->GetCustAttrib(HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_INDEX);

                        int Index;
                        AssetLibraryIndex->GetParamBlock(0)->GetValue(0, 0, Index, FOREVER);

                        if (AssetLibraryIndex)
                        {
                            //RecreatedAsset = RedoGeometryAsset(TheAsset, Index, MHANode->GetHandle());
                        }
                        else
                        {
                            HEMAX_Logger::Instance().AddEntry("HEMAX_Plugin: could not find the asset library number custom attribute when performing redo on Create HDA object."
                                " Therefore, the HDA cannot be reconnected to HAPI and will remain static.", HEMAX_LOG_LEVEL_WARN);
                        }
                    }

                    if (RecreatedAsset)
                    {
                        //PluginStore->AddNewMaxHoudiniAsset(MHANode->GetHandle(), RecreatedAsset);
                    }
                    
                }
            }
        }
        else
        {
            HEMAX_Logger::Instance().AddEntry("HEMAX_Plugin: could not find the asset path custom attribute when performing redo on Create HDA object."
                " Therefore, the HDA cannot be reconnected to HAPI and will remain static.", HEMAX_LOG_LEVEL_WARN);
        }
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry("HEMAX_Plugin: could not find a custom attribute container for 3ds Max Node when performing redo on Create HDA object."
            " Therefore, the HDA cannot be reconnected to HAPI and will remain static.", HEMAX_LOG_LEVEL_WARN);
    }
    */
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

    if (DoesCustomAttributeExist(StartingNode->GetCustAttribContainer(), HEMAX_MAX_HOUDINI_STAMP_INDEX, HdaStampName))
    {
        ReengageGeometryHda(StartingNode, StartingNode->GetCustAttribContainer());
    }

    // Now check for modifier HDAs
    Object* NodeObject = StartingNode->GetObjectRef();

    if (NodeObject)
    {
        if (NodeObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
        {
            IDerivedObject* DerivedObject = dynamic_cast<IDerivedObject*>(NodeObject);
            int ModifierCount = DerivedObject->NumModifiers();

            for (int i = 0; i < ModifierCount; i++)
            {
                if (DerivedObject->GetModifier(i)->ClassID() == HEMAX_Modifier_CLASS_ID)
                {
                    HEMAX_Modifier* TheModifier = dynamic_cast<HEMAX_Modifier*>(DerivedObject->GetModifier(i));
                    ReengageModifierHda(StartingNode, TheModifier, TheModifier->GetCustAttribContainer());
                }
            }
        }
    }

    int ChildCount = StartingNode->NumberOfChildren();

    for (int z = 0; z < ChildCount; z++)
    {
        ReconnectAllStrandedHdas(StartingNode->GetChildNode(z));
    }
}

bool
HEMAX_Plugin::ReconnectThisModifierHDA(INode* Node, Modifier* Mod)
{
    return false;
    // TODO figure out what to do here
    //return ReconnectHDAModifier(Node, Mod);
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
                        HEMAX_3dsmaxHda* MaxHda = nullptr;
                        if (TheAsset->AssetCount == 1)
                        {
                            MaxHda = new HEMAX_3dsmaxHda;
                            RecreateModifier3dsmaxHda(*MaxHda, *TheAsset, Index, Modifier, Node);
                        }
                        else if (TheAsset->AssetCount > 1)
                        {
                            MaxHda = new HEMAX_3dsmaxHda;
                            CustAttrib* AssetLibraryIndex = CustAttribs->GetCustAttrib(HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_INDEX);
                            AssetLibraryIndex->GetParamBlock(0)->GetValue(0, 0, Index, FOREVER);
                            RecreateModifier3dsmaxHda(*MaxHda, *TheAsset, Index, Modifier, Node);
                        }

                        if (MaxHda)
                        {
                            PluginStore->Add3dsmaxHda(Node->GetHandle(), MaxHda);
                            Modifier->RegisterPlugin(this);
                            ReloadHdaFromCustomAttributes(MaxHda);
                            UpdateEntireHda(MaxHda);
                        }
                    }
                }
            }
        }
    }
}

void
HEMAX_Plugin::ReengageGeometryHda(INode* Node, ICustAttribContainer* CustAttribs)
{
    CustAttrib* AssetPathAttrib = CustAttribs->GetCustAttrib(HEMAX_MAX_HOUDINI_ASSET_PATH_INDEX);

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

                HEMAX_3dsmaxHda* MaxHda = nullptr;

                if (TheAsset->AssetCount == 1)
                {
                    MaxHda = new HEMAX_3dsmaxHda;
                    RecreateGeometry3dsmaxHda(*MaxHda, *TheAsset, 0, Node);
                    ConstructReferencesForEditableCurve(MaxHda);
                }
                else
                {
                    CustAttrib* AssetLibraryIndex = CustAttribs->GetCustAttrib(HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_INDEX);
                    int Index;
                    AssetLibraryIndex->GetParamBlock(0)->GetValue(0, 0, Index, FOREVER);

                    MaxHda = new HEMAX_3dsmaxHda;
                    RecreateGeometry3dsmaxHda(*MaxHda, *TheAsset, Index, Node);
                    ConstructReferencesForEditableCurve(MaxHda);
                }

                if (MaxHda)
                {
                    PluginStore->Add3dsmaxHda(Node->GetHandle(), MaxHda);
                    ReloadHdaFromCustomAttributes(MaxHda);
                    UpdateEntireHda(MaxHda);
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

void
HEMAX_Plugin::CurrentSessionStopped()
{
    DestroyAllEditableNodeReferences();
    PluginStore->EmptyOutStore();
    std::vector<std::string> Paths = PluginStore->GetListOfLoadedAssets();
    PluginUserInterface->UpdateLoadedAssetLibrariesList(&Paths);
    PluginUserInterface->ChangeHdaSelection(nullptr, true);

    HEMAX_SessionManager::GetSessionManager().StopSession();
}

void
HEMAX_Plugin::NewSessionStarted()
{
    PushTimelineSettings();
    PushCurrentTime(GetCOREInterface()->GetTime());

    GetCOREInterface()->SelectNode(nullptr);

    TraverseHdaLoadPaths();

    INode* RootNode = GetCOREInterface()->GetRootNode();
    ReconnectAllStrandedHdas(RootNode);

    ToolShelf->LoadToolAssets();

    PluginUserInterface->UpdateLoadedAssetLibrariesList(&PluginStore->GetListOfLoadedAssets());
}

ULONG
HEMAX_Plugin::DetermineInputNodeSelection()
{
    int SelectedMaxNodeCount = GetCOREInterface()->GetSelNodeCount();

    if (SelectedMaxNodeCount > 0)
    {
        for (int i = 0; i < SelectedMaxNodeCount; ++i)
        {
            ULONG SelectedNode = GetCOREInterface()->GetSelNode(i)->GetHandle();
            HEMAX_3dsmaxHda* SelectedAsset = PluginStore->Find3dsmaxHda(SelectedNode);

            if (!SelectedAsset || SelectedAsset != PluginUserInterface->GetCurrentHdaSelection())
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
HEMAX_Plugin::SetAutoSelectRootMHANode(bool Enabled)
{
    UserPreferences->SetBoolSetting(HEMAX_SETTING_GRAB_ROOT, Enabled);
}

bool
HEMAX_Plugin::GetAutoSelectRootMHANode()
{
    bool Option;
    UserPreferences->GetBoolSetting(HEMAX_SETTING_GRAB_ROOT, Option);

    return Option;
}

void
HEMAX_Plugin::SetAutoStartWindowOption(bool Enabled)
{
    UserPreferences->SetBoolSetting(HEMAX_SETTING_AUTO_START_WINDOW, Enabled);
}

bool
HEMAX_Plugin::GetAutoStartWindowOption()
{
    bool Option;
    UserPreferences->GetBoolSetting(HEMAX_SETTING_AUTO_START_WINDOW, Option);

    return Option;
}

void
HEMAX_Plugin::SetAutoLoadHDADirectory(std::string Directory)
{
    UserPreferences->SetStringSetting(HEMAX_SETTING_HDA_LOAD_PATH, Directory);

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.IsSessionActive())
    {
        SM.LoadAllAssetsInDirectory(Directory, PluginStore);
        std::vector<std::string> LoadedAsset = PluginStore->GetListOfLoadedAssets();
        PluginUserInterface->UpdateLoadedAssetLibrariesList(&LoadedAsset);
    }
}

std::string
HEMAX_Plugin::GetAutoLoadHDADirectory()
{
    std::string Directory;
    UserPreferences->GetStringSetting(HEMAX_SETTING_HDA_LOAD_PATH, Directory);

    return Directory;
}

void
HEMAX_Plugin::SetHdaRepoDirectory(std::string Directory)
{
    UserPreferences->SetStringSetting(HEMAX_SETTING_HDA_REPO_PATH, Directory);
    PluginStore->UserHdaRepository = Directory;
}

std::string
HEMAX_Plugin::GetHdaRepoDirectory()
{
    std::string Directory;
    UserPreferences->GetStringSetting(HEMAX_SETTING_HDA_REPO_PATH, Directory);

    return Directory;
}

HEMAX_Store*
HEMAX_Plugin::GetPluginStore()
{
    return PluginStore;
}

void
HEMAX_Plugin::DirtyEditableNodes(ULONG ContainerNodeHandle)
{
    HEMAX_3dsmaxHda* MaxHda = PluginStore->Find3dsmaxHda(ContainerNodeHandle);

    if (MaxHda && MaxHda->Type == HEMAX_GEOMETRY_HDA)
    {
        for (int i = 0; i < MaxHda->GeometryHda.EditableCurves.size(); i++)
        {
            MaxHda->GeometryHda.EditableCurves[i].Dirty = true;
        }
    }
}

void
HEMAX_Plugin::ConstructReferencesForEditableCurve(HEMAX_3dsmaxHda* GeoHda)
{
    if (GeoHda && (GeoHda->Type == HEMAX_GEOMETRY_HDA) && GeoHda->GeometryHda.ContainerNode)
    {
        std::vector<HEMAX_EditableCurve>& EditableCurves = GeoHda->GeometryHda.EditableCurves;
        std::vector<HEMAX_ReferenceMaker*> CurveNodeRefs;
        for (int i = 0; i < EditableCurves.size(); ++i)
        {
            HEMAX_ReferenceMaker* NewRef = new HEMAX_ReferenceMaker(this);
            NewRef->AddEditableNodeToWatch(EditableCurves[i].Node);
            CurveNodeRefs.push_back(NewRef);
        }
        EditableCurveReferences.insert({ GeoHda->GeometryHda.ContainerNode, CurveNodeRefs });
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

HEMAX_UserPrefs*
HEMAX_Plugin::GetUserPrefs()
{
    return UserPreferences;
}

void
HEMAX_Plugin::HandleParameterInputCleared(HEMAX_Node* Node, HEMAX_Parameter Parameter)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
        Clear3dsmaxHdaParameterInput(Hda, &Parameter);
        Cook3dsmaxHda(*Hda);
        Update3dsmaxHda(*Hda);
        GetCOREInterface()->ForceCompleteRedraw();
    }
}

void
HEMAX_Plugin::HandleParameterInputSelection(HEMAX_Node* Node, HEMAX_Parameter Parameter, INode* InputNodeToUse)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
        HEMAX_3dsMaxInput* MaxInput = Set3dsmaxHdaParameterInput(Hda, &Parameter, InputNodeToUse);
        Cook3dsmaxHda(*Hda);
        Update3dsmaxHda(*Hda);
        GetCOREInterface()->ForceCompleteRedraw();
    }
}

void
HEMAX_Plugin::HandleSubnetworkInputCleared(HEMAX_Node* Node, int Subnetwork)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
        Clear3dsmaxHdaSubnetworkInput(Hda, Subnetwork);
        Cook3dsmaxHda(*Hda);
        Update3dsmaxHda(*Hda);
        GetCOREInterface()->ForceCompleteRedraw();
    }
}

void
HEMAX_Plugin::HandleSubnetworkInputSelection(HEMAX_Node* Node, int Subnetwork, INode* InputNodeToUse)
{
        HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

        if (Hda)
        {
            Set3dsmaxHdaSubnetworkInput(Hda, Subnetwork, InputNodeToUse);
            Cook3dsmaxHda(*Hda);
            Update3dsmaxHda(*Hda);
            GetCOREInterface()->ForceCompleteRedraw();
        }
}

void
HEMAX_Plugin::Clear3dsmaxHdaParameterInput(HEMAX_3dsmaxHda* Hda, HEMAX_Parameter* Parameter)
{
    if (Hda)
    {
        HEMAX_InputInstance* OldInput = FindParameterInput(*Hda, Parameter->Info.id);
        ULONG OldInputHandle = -1;

        if (OldInput)
        {
            OldInputHandle = OldInput->MaxInput->Get3dsMaxNodeHandle();
            delete OldInput->MergeNode;
            OldInput->MergeNode = nullptr;
        }

        ClearParameterInputNode(*Hda, Parameter->Info.id);
        UpdateParameterInputNode(*Parameter, -1);
        UpdateParameterInputNode(*Hda, Parameter->Info.id);
        UpdateInputNodeCustomAttribute(*Hda, *Parameter, nullptr);

        if (OldInput)
        {
            OldInput->MaxInput->DecrementInputCount();
            OldInput->MaxInput->RemoveParameterUsage(Hda->Hda.MainNode, GetParameterName(*Parameter));

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
        HEMAX_InputInstance* OldInput = FindParameterInput(*Hda, Parameter->Info.id);

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
            SelectedInputNode = DetermineInputNodeSelection();
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
                    INode* HoudiniAssetNode = Hda->ModifierHda.ContainerNode;
                    INode* SelectedInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput->Get3dsMaxNodeHandle());
                    Merge = new HEMAX_InputMerge(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(HoudiniAssetNode, SelectedInputNode));
                }
                else
                {
                    Merge = new HEMAX_InputMerge(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(SelectedInputNode)));
                }

                Merge->MergeInput(*MaxInput->GetInputNode());
                HEMAX_InputInstance* InputInstance = new HEMAX_InputInstance(MaxInput, Merge);

                SetParameterInput(*Hda, Parameter->Info.id, InputInstance);
                UpdateParameterInputNode(*Hda, Parameter->Info.id);
                UpdateInputNodeCustomAttribute(*Hda, *Parameter, MaxInput->GetInputNode());
                MaxInput->IncrementInputCount();
                MaxInput->AddParameterUsage(Hda->Hda.MainNode, GetParameterName(*Parameter));
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
        HEMAX_InputInstance* OldInput = FindSubnetworkInput(*Hda, Subnetwork);

        if (OldInput)
        {
            ULONG OldInputHandle = -1;

            if (OldInput->MaxInput)
            {
                OldInputHandle = OldInput->MaxInput->Get3dsMaxNodeHandle();
            }

            ClearSubnetworkInput(*Hda, Subnetwork);
            UpdateSubnetworkInput(*Hda, Subnetwork);
            UpdateSubnetworkCustomAttribute(*Hda, Subnetwork, nullptr);

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
            SelectedInputNode = DetermineInputNodeSelection();
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
                    INode* HoudiniAssetNode = Hda->ModifierHda.ContainerNode;
                    INode* SelectedInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput->Get3dsMaxNodeHandle());
                    Merge = new HEMAX_InputMerge(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(HoudiniAssetNode, SelectedInputNode));
                }
                else
                {
                    Merge = new HEMAX_InputMerge(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(SelectedInputNode)));
                }

                Merge->MergeInput(*MaxInput->GetInputNode());
                HEMAX_InputInstance* InputInstance = new HEMAX_InputInstance(MaxInput, Merge);

                SetSubnetworkInput(*Hda, Subnetwork, InputInstance);
                UpdateSubnetworkInput(*Hda, Subnetwork);
                UpdateSubnetworkCustomAttribute(*Hda, Subnetwork, MaxInput->GetInputNode());
                
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
                    INode* HoudiniAssetNode = Hda->ModifierHda.ContainerNode;
                    INode* SelectedInputNode = GetCOREInterface()->GetINodeByHandle(MaxInput->MaxInput->Get3dsMaxNodeHandle());
                    MaxInput->MergeNode = new HEMAX_InputMerge(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(HoudiniAssetNode, SelectedInputNode));
                }
                else
                {
                    MaxInput->MergeNode = new HEMAX_InputMerge(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(MaxInput->MaxInput->Get3dsMaxNodeHandle())));
                }

                MaxInput->MergeNode->MergeInput(*MaxInput->MaxInput->GetInputNode());

                SetSubnetworkInput(*Hda, i, MaxInput);
                UpdateSubnetworkInput(*Hda, i);
                UpdateSubnetworkCustomAttribute(*Hda, i, MaxInput->MaxInput->GetInputNode());
                
            }
            else
            {
                // If it's not dirty, the Transform still needs to be updated because 3ds Max does not send
                // NotifyRefChanged messages for keyframed transforms
                if (Hda->Type == HEMAX_MODIFIER_HDA)
                {
                    MaxInput->MergeNode->PushMaxTransform(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(Hda->ModifierHda.ContainerNode, GetCOREInterface()->GetINodeByHandle(MaxInput->MaxInput->Get3dsMaxNodeHandle())));
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
                    INode* HoudiniAssetNode = Hda->ModifierHda.ContainerNode;
                    INode* SelectedInputNode = GetCOREInterface()->GetINodeByHandle(ParameterInput->MaxInput->Get3dsMaxNodeHandle());
                    ParameterInput->MergeNode = new HEMAX_InputMerge(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(HoudiniAssetNode, SelectedInputNode));
                }
                else
                {
                    ParameterInput->MergeNode = new HEMAX_InputMerge(HEMAX_Utilities::BuildMaxTransformFromINode(GetCOREInterface()->GetINodeByHandle(ParameterInput->MaxInput->Get3dsMaxNodeHandle())));
                }

                ParameterInput->MergeNode->MergeInput(*(ParameterInput->MaxInput->GetInputNode()));

                SetParameterInput(*Hda, It->first, ParameterInput);
                UpdateParameterInputNode(*Hda, It->first);
                UpdateInputNodeCustomAttribute(*Hda, GetParameter(Hda->Hda.MainNode, It->first), ParameterInput->MaxInput->GetInputNode());
            }
            else
            {
                // If it's not dirty, the Transform still needs to be updated because 3ds Max does not send
                // NotifyRefChanged messages for keyframed transforms
                if (Hda->Type == HEMAX_MODIFIER_HDA)
                {
                    ParameterInput->MergeNode->PushMaxTransform(HEMAX_Utilities::CalculateRelativeTransformBetweenINodes(Hda->ModifierHda.ContainerNode, GetCOREInterface()->GetINodeByHandle(ParameterInput->MaxInput->Get3dsMaxNodeHandle())));
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
                    HEMAX_Parameter Parameter = GetParameter(MaxHda->Hda.MainNode, Usages[i].Parameter);
                    HandleParameterInputCleared(&Usages[i].Node, Parameter);
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
HEMAX_Plugin::Clear3dsmaxHdaInputConnections(HEMAX_3dsmaxHda* Hda)
{
    for (int i = 0; i < Hda->Hda.MainNode.Info.inputCount; i++)
    {
        if (Hda->SubnetworkNodeInputs[i])
        {
            HandleSubnetworkInputCleared(&Hda->Hda.MainNode, i);
        }
    }

    std::vector<HEMAX_Parameter> ParameterInputs = GetAllParametersWithInputs(*Hda);

    for (int i = 0; i < ParameterInputs.size(); i++)
    {
        if (ParameterInputs[i].Type != HEMAX_PARAM_INVALID)
        {
            HandleParameterInputCleared(&Hda->Hda.MainNode, ParameterInputs[i]);
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
	        UpdateAssetDefinition(*Asset, Hda->Hda.HdaAsset.Path);

	        std::vector<HEMAX_3dsmaxHda*> HdasToReload = PluginStore->FindAllHdasUsingAssetDefinition(Asset);

	        for (int i = 0; i < HdasToReload.size(); i++)
	        {
		        if (HdasToReload[i]->Type == HEMAX_GEOMETRY_HDA)
		        {
		            int AssetIndex = HdasToReload[i]->Hda.HdaAssetIndex;

		            Clear3dsmaxHdaInputConnections(HdasToReload[i]);
		            PluginStore->Delete3dsmaxHda(HdasToReload[i]->GeometryHda.ContainerNode->GetHandle());

		            HEMAX_3dsmaxHda* CreatedAsset = new HEMAX_3dsmaxHda;
		            CreateGeometry3dsmaxHda(*CreatedAsset, *Asset, AssetIndex);
		            ConstructReferencesForEditableCurve(CreatedAsset);
		            PluginStore->Add3dsmaxHda(CreatedAsset->GeometryHda.ContainerNode->GetHandle(), CreatedAsset);
		            GetCOREInterface()->ForceCompleteRedraw();
		        }
		        else if (HdasToReload[i]->Type == HEMAX_MODIFIER_HDA)
		        {
		            INode* Node = HdasToReload[i]->ModifierHda.ContainerNode;
		            int AssetIndex = HdasToReload[i]->Hda.HdaAssetIndex;
		            int ModifierIndex, DerivedObjIndex;
		    
		            if (GetCOREInterface7()->FindModifier(*(HdasToReload[i]->ModifierHda.ContainerNode), *(HdasToReload[i]->ModifierHda.DisplayGeometry), ModifierIndex, DerivedObjIndex))
		            {
			            Clear3dsmaxHdaInputConnections(HdasToReload[i]);
			            HdasToReload[i]->ModifierHda.DisplayGeometry->CleanupHAPIComponents();
			            PluginStore->Remove3dsmaxHda(HdasToReload[i]->ModifierHda.ContainerNode->GetHandle(), HdasToReload[i]->ModifierHda.DisplayGeometry);
			            GetCOREInterface7()->DeleteModifier(*HdasToReload[i]->ModifierHda.ContainerNode, ModifierIndex);

			            ManualModifierAddInProgress = true;

			            HEMAX_Modifier* ModifierPlugin = (HEMAX_Modifier*)GetCOREInterface()->CreateInstance(OSM_CLASS_ID, HEMAX_Modifier_CLASS_ID);
			            GetCOREInterface7()->AddModifier(*Node, *ModifierPlugin, ModifierIndex);

			            HEMAX_3dsmaxHda* ModifierHda = new HEMAX_3dsmaxHda;

			            if (CreateModifier3dsmaxHda(*ModifierHda, *Asset, AssetIndex, ModifierPlugin, Node))
			            {
			                ModifierPlugin->RegisterPlugin(this);
			                PluginStore->Add3dsmaxHda(Node->GetHandle(), ModifierHda);
			                UpdateEntireHda(ModifierHda);
			            }
			            else
			            {
			                GetCOREInterface7()->DeleteModifier((*Node), ModifierIndex);
			                delete ModifierPlugin;
			            }

			            ManualModifierAddInProgress = false;
		            }		    
		        }
	        }
	    }
    }
}

void
HEMAX_Plugin::HandleParameterIntValuesUpdate(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<int> IntValues)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(*Node);

    if (Hda)
    {
        UpdateParameterIntValues(Parameter, IntValues);
        UpdateIntCustomAttribute(*Hda, Parameter, IntValues);

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
        UpdateParameterFloatValues(Parameter, FloatValues);
        UpdateFloatCustomAttribute(*Hda, Parameter, FloatValues);
        
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
        UpdateParameterStringValues(Parameter, StringValues);
        UpdateStringCustomAttribute(*Hda, Parameter, StringValues);

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
        UpdateMultiParameterList(*Hda, Parameter);
        UpdateEntireHda(Hda);
    }
}

void
HEMAX_Plugin::RequestParameterViewerClearSelection()
{
    PluginUserInterface->ChangeHdaSelection(nullptr);
}

void
HEMAX_Plugin::RequestParameterViewerSetSelection(HEMAX_Node Node)
{
    HEMAX_3dsmaxHda* Hda = PluginStore->Find3dsmaxHda(Node);

    if (Hda)
    {
        PluginUserInterface->ChangeHdaSelection(Hda);
    }
}

void
HEMAX_Plugin::UpdateEntireHda(HEMAX_3dsmaxHda* Hda, bool ForceRedraw)
{
    if (Hda->Type == HEMAX_GEOMETRY_HDA)
    {
        for (int i = 0; i < Hda->GeometryHda.EditableCurves.size(); i++)
        {
            if (Hda->GeometryHda.EditableCurves[i].Dirty)
            {
                PushEditableNodeChanges(Hda->GeometryHda, Hda->GeometryHda.EditableCurves[i]);
            }
        }
    }

    RefreshInputConnections(Hda);
    UpdateDirtiedInputNodes(Hda);
    Cook3dsmaxHda(*Hda);
    Update3dsmaxHda(*Hda);
    UpdateAllCustomAttributes(*Hda);
    
    if (ForceRedraw)
    {
        GetCOREInterface()->ForceCompleteRedraw();

        if (Hda->Type == HEMAX_MODIFIER_HDA)
        {
            if (Hda->ModifierHda.ContainerNode && Hda->ModifierHda.DisplayGeometry)
            {
                if (Hda->ModifierHda.DisplayGeometry->CheckIfTopOfStack(Hda->ModifierHda.ContainerNode))
                {
                    Hda->ModifierHda.DisplayGeometry->ApplyMaterialsToNode();
                    Hda->ModifierHda.DisplayGeometry->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
                }
            }
        }
    }
}

void
HEMAX_Plugin::ReloadHdaFromCustomAttributes(HEMAX_3dsmaxHda* MaxHda)
{
    std::vector<HEMAX_SubnetworkInputMapping> Unconnected = ReloadSubnetworkInputsFromCustomAttributes(*MaxHda);
    for (int s = 0; s < Unconnected.size(); s++)
    {
        if (Unconnected[s].Node)
        {
            HandleSubnetworkInputSelection(&MaxHda->Hda.MainNode, Unconnected[s].Subnetwork, Unconnected[s].Node);
        }
    }

    std::vector<HEMAX_ParameterInputMapping> UnconnectedInputs = ReloadParametersFromCustomAttributes(*MaxHda);
    for (int s = 0; s < UnconnectedInputs.size(); s++)
    {
        if (UnconnectedInputs[s].Node)
        {
            HandleParameterInputSelection(&MaxHda->Hda.MainNode, GetParameter(MaxHda->Hda.MainNode, UnconnectedInputs[s].ParameterName), UnconnectedInputs[s].Node);
        }
    }
}

bool
HEMAX_Plugin::RemoveAsset(std::string AssetPath)
{
    if (PluginStore->RemoveAsset(AssetPath))
    {
        std::vector<std::string> Paths = PluginStore->GetListOfLoadedAssets();
        PluginUserInterface->UpdateLoadedAssetLibrariesList(&Paths);
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

void
HEMAX_Plugin::CloneGeometryHda(HEMAX_3dsmaxHda* MaxHda)
{
    HEMAX_3dsmaxHda* Clone = CreateGeometryHDA(MaxHda->Hda.HdaAsset.Path, MaxHda->Hda.HdaAssetIndex);
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
                HEMAX_Parameter SourceParameter = GetParameter(MaxHda->Hda.MainNode, InputIt->first);
                std::string SourceParmName = GetParameterName(SourceParameter);

                HEMAX_Parameter CloneParameter = GetParameter(Clone->Hda.MainNode, SourceParmName);
                if (CloneParameter.Type == HEMAX_PARAM_NODE)
                {
                    Set3dsmaxHdaParameterInput(Clone, &CloneParameter, GetCOREInterface()->GetINodeByHandle(MaxInputNodeHandle));
                }
            }
        }

        CopyAllParameterValues((*Clone), (*MaxHda));
        UpdateEntireHda(Clone);
        PluginUserInterface->ChangeHdaSelection(nullptr);
        PluginUserInterface->ChangeHdaSelection(Clone);
    }
}

void
HEMAX_Plugin::CloneModifierHda(HEMAX_3dsmaxHda* MaxHda, INode* MaxNode)
{
    ManualModifierAddInProgress = true;

    HEMAX_3dsmaxHda* Clone = CreateModifierHDA(MaxNode, MaxHda->Hda.HdaAsset.Path, MaxHda->Hda.HdaAssetIndex);
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
                HEMAX_Parameter SourceParameter = GetParameter(MaxHda->Hda.MainNode, InputIt->first);
                std::string SourceParmName = GetParameterName(SourceParameter);

                HEMAX_Parameter CloneParameter = GetParameter(Clone->Hda.MainNode, SourceParmName);
                if (CloneParameter.Type == HEMAX_PARAM_NODE)
                {
                    Set3dsmaxHdaParameterInput(Clone, &CloneParameter, GetCOREInterface()->GetINodeByHandle(MaxInputNodeHandle));
                }
            }
        }

        CopyAllParameterValues((*Clone), (*MaxHda));
        UpdateEntireHda(Clone);
        PluginUserInterface->ChangeHdaSelection(Clone);
    }

    ManualModifierAddInProgress = false;
}

void
HEMAX_Plugin::TraverseHdaLoadPaths()
{
    HEMAX_SessionManager::GetSessionManager().LoadAllAssetsInDirectory(GetAutoLoadHDADirectory(), PluginStore);

    std::string HdaLoadPath = HEMAX_Utilities::GetEnvVar(HEMAX_ENV_HDA_PATH);

    if (!HdaLoadPath.empty())
    {
        HEMAX_SessionManager::GetSessionManager().LoadAllAssetsInDirectory(HdaLoadPath, PluginStore);
    }
}
