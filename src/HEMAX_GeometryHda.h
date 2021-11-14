#pragma once

#include "dummy.h"
#include "inode.h"
#include "linshape.h"

#include "HEMAX_3dsmaxHda.h"
#include "HEMAX_CustomAttributeContainer.h"
#include "HEMAX_Hda.h"
#include "HEMAX_GeometryPlugin.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

class HEMAX_GeometryHda : public HEMAX_3dsmaxHda
{
    public:
	HEMAX_GeometryHda();

	void Init(HEMAX_Asset& Asset, int AssetIndex);

	void Create(HEMAX_Asset& Asset, int AssetIndex);
	void Create(INode* ContainerNode, HEMAX_Asset& Asset, int AssetIndex);

	void CreateNewGeometryHda(HEMAX_Hda& Hda);

	void CreateNewGeometryHda(HEMAX_Hda& Hda, INode* ContainerNode);

	void CreateGeometryHdaFromContainerNode();

	void ClearAnyInstances();

	std::vector<ULONG> GetListOfAllChildINodes();

	void Update3dsmaxHda();

	void UpdateGeometryHda();

	void PushEditableNodeChanges(HEMAX_EditableCurve EditableCurve);

	std::vector<INode*> BakeGeometryHda(bool BakeDummyObj);

	void SetPushTransformsOption(bool Option);
	bool IsPushTransformsOptionEnabled();

	void SetApplyHAPITransformOption(bool Option);
	bool ShouldApplyHAPITransform();

	void SetCustomAttributeContainer(
                ICustAttribContainer* Container) override;

	void SetContainerNode(INode* Node);

	DummyObject* Dummy;
	INode* ContainerNode;

	int MaxStampIndex;

        std::unordered_set<INode*> InstanceSources;
	std::vector<INode*> InstanceClones;
        std::unordered_map<INode*, INode*> InstanceCloneToSourceMap;

        std::unordered_set<INode*> PackedPrimSources;
	std::vector<INode*> PackedPrimClones;
        std::unordered_map<INode*, INode*> PackedPrimCloneToSourceMap;

	std::vector<HEMAX_EditableCurve> EditableCurves;

    private:

	bool IsINodeADisplayGeometry(INode* Node);

	bool IsINodeAnEditableNode(INode* Node);

	HEMAX_PartId GetPartIdFromCustomAttributes(INode* Node);

	void CreateDisplayGeometry(HEMAX_Hda& Hda,
                                   HEMAX_DisplayGeoNode& DisplayNode);

	void CreateMeshPluginPart(HEMAX_Hda& Hda,
                                  HEMAX_DisplayGeoNode& DisplayNode,
                                  HEMAX_Part& Part);
	void CreateCurvePart(HEMAX_DisplayGeoNode& DisplayNode,
                             HEMAX_Part& Part);

	void CreateInstances(HEMAX_Hda& Hda);

	void CreatePackedPrimitives(HEMAX_Part& Part,
                                    HEMAX_DisplayGeoNode& DisplayNode);

	void CreateEditableCurves(HEMAX_EditableNode& EditableNode);

	void InitGeometryCustAttribContainer();

	void GenerateBoilerplateGeometryCustomAttributes(HEMAX_Hda& Hda);

	void InitGeometryPluginCustAttribContainer(INode* PluginNode);

	void GenerateBoilerplateGeometryPluginCustomAttributes(
                INode* PluginNode, HEMAX_PartId Part);

	void UpdateInstances(HEMAX_Hda& Hda);

	void StampInstanceNode(INode* Node);

	void StampPackedPrimitiveNode(INode* Node);

	void StampEditableNode(INode* Node,
                               std::string EditableNodeName,
                               HEMAX_PartId PartNum);

	std::string GetEditableNodeName(INode* Node);

	int GetEditablePartNumber(INode* Node);

	void ClearInstances();

	void ClearPackedPrimNodes();

        void AssignMaterials(HEMAX_Hda& Hda,
                             HEMAX_NodeId DisplayNodeId,
                             HEMAX_Part& Part,
                             HEMAX_GeometryPlugin* GeoPlugin);

	void ApplySceneMtlToGeometryPlugin(HEMAX_GeometryPlugin* GeoPlugin);

        void UpdateMaterials(HEMAX_Hda& Hda,
                             HEMAX_NodeId DisplayNodeId,
                             HEMAX_Part& Part,
                             HEMAX_GeometryPlugin* GeoPlugin);

        void SetPluginNodeName(INode* Node,
                               const HEMAX_DisplayGeoNode& DisplayNode,
                               const HEMAX_Part& Part,
                               bool UseUniqueName);
        void GetInstancedPluginNodeNames(
                const HEMAX_DisplayGeoNode& DisplayNode,
                const HEMAX_Part& Part,
                std::vector<std::wstring>& NodeNames);

        std::string GetDetailAttributeOverride(const std::string& Name);

        bool IsInstanceSource(INode* Node);
        bool IsPackedPrimSource(INode* Node);
        bool IsInstance(INode* Node);
        bool IsPackedPrim(INode* Node);

        INode* GetBakedInstanceSource(
            INode* Node,
            std::unordered_map<INode*, INode*>& InstanceSourceToBakedInstanceSource);
};

