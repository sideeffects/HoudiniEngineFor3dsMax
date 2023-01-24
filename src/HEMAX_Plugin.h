#pragma once

#include "HEMAX_Mesh.h"
#include "HEMAX_Store.h"
#include "HEMAX_Asset.h"
#include "HEMAX_Input.h"
#include "HEMAX_ModifierAsset.h"
#include "HEMAX_Shelf.h"

#pragma warning(push, 0)
#include <notify.h>
#include <maxapi.h>
#pragma warning(pop)

#include <unordered_map>
#include <string>

extern std::string HEMAX_CurrentAssetSelection;

class HEMAX_Events;
class HEMAX_ModifierAsset;
class HEMAX_ReferenceMaker;
class HEMAX_3dsmaxHda;

void MAXSelectionSetChangedCallback(void* PluginPointer, NotifyInfo* Info);
void FilePreOpenHandler(void* PluginPointer, NotifyInfo* Info);
void FilePostOpenHandler(void* PluginPointer, NotifyInfo* Info);
void PreNewAllHandler(void* PluginPointer, NotifyInfo* Info);
void NodePreDeleteHandler(void* PluginPointer, NotifyInfo* Info);
void ModifierPostAddHandler(void* PluginPointer, NotifyInfo* Info);
void ModifierPreDeleteHandler(void* PluginPointer, NotifyInfo* Info);
void NodeCreatedHandler(void* PluginPointer, NotifyInfo* Info);
void RenderPreEvalHandler(void* PluginPointer, NotifyInfo* Info);

typedef struct
{
    INode* node;
    Modifier* mod;
    ModContext* mc;
} HEMAX_ModifierEvent;

typedef struct
{
    INodeTab* OriginalNodes;
    INodeTab* ClonedNodes;
    CloneType Type;
} HEMAX_CloneEvent;

class HEMAX_Plugin : public TimeChangeCallback
{
    public:
	HEMAX_Plugin(Interface* Interface, HMODULE LibHAPIL);
	~HEMAX_Plugin();

        void Init(std::string HapiToolsDir);

        void SetEventHub(HEMAX_Events* _PluginEvents);
        HEMAX_Events* GetEventHub();

	void TimeChanged(TimeValue Time) override;
	void SyncEntireScene();

	void LoadNewAsset(std::string Path);
	void CreateGeometryHDA(std::string Path);
	HEMAX_GeometryHda* CreateGeometryHDA(std::string Path, int AssetIndex);
	HEMAX_ModifierHda* CreateModifierHDA(INode* Node, std::string Path, int AssetIndex);
	void CreateModifierHDAs(std::string Path);
	INode* CloneHda(HEMAX_3dsmaxHda* MaxHda);
	Modifier* CopyHdaToNode(HEMAX_3dsmaxHda* MaxHda, INode* Node);

	void SelectionSetChangeHandler();
	void HandleSceneFileOpening();
	void HandleSceneFilePostOpen();
	void HandlePreNewAll();
	void HandleNodePreDelete(Tab<INode*>* NodesBeingDeleted);
	void HandleModifierPostAdd(HEMAX_ModifierEvent* ModEvent);
	void HandleModifierPreDelete(HEMAX_ModifierEvent* ModEvent);
	void HandleNodeCreated(INode* AddedNode);
	void HandleRenderPreEval(TimeValue* Time);

	bool StartSession();
	bool StopSession();

	ULONG DetermineInputNodeSelection(HEMAX_3dsmaxHda* Hda);

	HEMAX_3dsMaxInput* Create3dsMaxInput(ULONG InputNode);

	std::string GetNodeName(ULONG Node);

        void UpdateHdaLoadDirectory(std::string Directory);
        void UpdateHdaSearchPathDirectory(std::string Directory);

	HEMAX_Store* GetPluginStore();

	void HandleModifierStackCollapse(INode* TheNode, Modifier* HDAModifier);

	void DirtyEditableNodes(ULONG ContainerNodeHandle);

	void HandleParameterInputCleared(HEMAX_Node* Node,
                                         HEMAX_Parameter Parameter,
                                         bool UpdateHda = true);
	void HandleParameterInputSelection(HEMAX_Node* Node, HEMAX_Parameter Parameter, INode* InputNodeToUse = nullptr);
	void HandleSubnetworkInputCleared(HEMAX_Node* Node, int Subnetwork,
                                          bool UpdateHda = true);
	void HandleSubnetworkInputSelection(HEMAX_Node* Node, int Subnetwork, INode* InputNodeToUse = nullptr);

	void HandleRecookRequest(HEMAX_Node* Node);
	void ReloadAssetDefinition(HEMAX_Node* Node);
	void HandleParameterIntValuesUpdate(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<int> IntValues);
	void HandleParameterFloatValuesUpdate(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<float> FloatValues);
	void HandleParameterStringValuesUpdate(HEMAX_Node* Node, HEMAX_Parameter Parameter, std::vector<std::string> StringValues);
	void HandleMultiParameterUpdate(HEMAX_Node* Node, HEMAX_Parameter Parameter, HEMAX_MultiParameterChangeInfo ChangeInfo);

	bool RemoveAsset(std::string AssetPath);

	HEMAX_Shelf* GetToolShelf();

	void ReloadHdaFromCustomAttributes(HEMAX_3dsmaxHda* MaxHda);

	void Clear3dsmaxHdaInputConnections(HEMAX_3dsmaxHda* Hda,
                                            bool UpdateHda = true);

    private:

	bool ManualModifierAddInProgress;

	Interface* MaxInterface;
	HEMAX_Store* PluginStore;
	HEMAX_Shelf* ToolShelf;

        HEMAX_Events* PluginEvents;

	void ReconnectAllStrandedHdas(INode* StartingNode);
	void ModifierDive(IDerivedObject* DerivedObject, INode* StartingNode);

	void ReengageModifierHda(INode* Node, HEMAX_Modifier* Modifier, ICustAttribContainer* CustAttribs);
	void ReengageGeometryHda(INode* Node, ICustAttribContainer* CustAttribs);

	std::unordered_map<INode*, std::vector<HEMAX_ReferenceMaker*>> EditableCurveReferences;

	void ConstructReferencesForEditableCurve(HEMAX_GeometryHda* GeoHda);
	void RemoveReferencesForEditableCurve(INode* Node);

	void DestroyAllEditableNodeReferences();

	void Clear3dsmaxHdaParameterInput(HEMAX_3dsmaxHda* Hda,
                                          HEMAX_Parameter* Parameter);
	HEMAX_3dsMaxInput* Set3dsmaxHdaParameterInput(HEMAX_3dsmaxHda* Hda, HEMAX_Parameter* Parameter, INode* InputNodeToUse);

	void Clear3dsmaxHdaSubnetworkInput(HEMAX_3dsmaxHda* Hda, int Subnetwork);
	HEMAX_3dsMaxInput* Set3dsmaxHdaSubnetworkInput(HEMAX_3dsmaxHda* Hda, int Subnetwork, INode* InputNodeToUse);

	void RefreshInputConnections(HEMAX_3dsmaxHda* Hda);
	void UpdateDirtiedInputNodes(HEMAX_3dsmaxHda* Hda);

	void Handle3dsMaxInputSourceDeletion(HEMAX_3dsMaxInput* MaxInput);

	void UpdateEntireHda(HEMAX_3dsmaxHda* Hda, bool ForceRedraw = true);

	INode* CloneGeometryHda(HEMAX_GeometryHda* MaxHda);
	Modifier* CloneModifierHda(HEMAX_3dsmaxHda* MaxHda, INode* MaxNode);

	void TraverseHdaLoadPaths();

	void InitializeLogPrintLevels();
};
