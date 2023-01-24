#pragma once

#pragma warning(push, 0)
#include <dummy.h>
#include <inode.h>
#include <linshape.h>
#pragma warning(pop)

#include "HEMAX_3dsmaxHda.h"
#include "HEMAX_CustomAttributeContainer.h"
#include "HEMAX_Hda.h"
#include "HEMAX_GeometryPlugin.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

class HEMAX_GeometryHda : public HEMAX_3dsmaxHda
{
    private:
        struct PackedPrimInfo
        {
            HAPI_NodeId Node;
            HAPI_PartId InstancingPart;
            int InstanceNum;
            HAPI_PartId InstancedPartId;
        };
    
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
        std::unordered_map<INode*, PackedPrimInfo> PackedPrimClones;
        std::unordered_map<INode*, INode*> PackedPrimCloneToSourceMap;

	std::vector<HEMAX_EditableCurve> EditableCurves;

    private:

	bool IsINodeADisplayGeometry(INode* Node);

	bool IsINodeAnEditableNode(INode* Node);

	HAPI_PartId GetPartIdFromCustomAttributes(INode* Node);

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
                INode* PluginNode, HAPI_PartId Part);

	void UpdateInstances(HEMAX_Hda& Hda);

	void StampInstanceNode(INode* Node);

	void StampPackedPrimitiveNode(INode* Node);

	void StampEditableNode(INode* Node,
                               std::string EditableNodeName,
                               HAPI_PartId PartNum);

	std::string GetEditableNodeName(INode* Node);

	int GetEditablePartNumber(INode* Node);

	void ClearInstances();

	void ClearPackedPrimNodes();

        void AssignMaterials(HEMAX_Hda& Hda,
                             HAPI_NodeId DisplayNodeId,
                             HEMAX_Part& Part,
                             HEMAX_GeometryPlugin* GeoPlugin);

	void ApplySceneMtlToGeometryPlugin(HEMAX_GeometryPlugin* GeoPlugin);

        void UpdateMaterials(HEMAX_Hda& Hda,
                             HAPI_NodeId DisplayNodeId,
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

