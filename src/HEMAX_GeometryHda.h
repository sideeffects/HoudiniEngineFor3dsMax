#pragma once

#include "HEMAX_Hda.h"
#include "inode.h"
#include "dummy.h"
#include "HEMAX_GeometryPlugin.h"
#include "HEMAX_CustomAttributeContainer.h"
#include <unordered_map>
#include <vector>

struct HEMAX_GeometryHda
{
    DummyObject* Dummy;
    INode* ContainerNode;

    ICustAttribContainer* CustomAttributes;
    std::unordered_map<std::string, HEMAX_ParameterAttrib*> CustomAttributeMap;
    int MaxStampIndex;

    std::unordered_map<HEMAX_NodeId, std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*>> DisplayGeometry;
    std::unordered_map<HEMAX_NodeId, std::unordered_map<HEMAX_PartId, std::vector<Mtl*>>> MaterialMap;

    std::vector<INode*> InstanceClones;
    std::vector<INode*> PackedPrimClones;

    std::vector<HEMAX_EditableCurve> EditableCurves;

    void SetPushTransformsOption(bool Option);
    bool IsPushTransformsOptionEnabled();

    void SetApplyHAPITransformOption(bool Option);
    bool ShouldApplyHAPITransform();
};

void
CreateNewGeometryHda(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda);

void
CreateNewGeometryHda(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, INode* ContainerNode);

void
CreateGeometryHdaFromContainerNode(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, INode* ContainerNode);

static bool
IsINodeADisplayGeometry(INode* Node);

static bool
IsINodeAnEditableNode(INode* Node);

static HEMAX_PartId
GetPartIdFromCustomAttributes(INode* Node);

static void
CreateDisplayGeometry(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_DisplayGeoNode& DisplayNode);

static void
CreateMeshPart(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_DisplayGeoNode& DisplayNode, HEMAX_Part& Part, std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*>& DisplayGeometries);

static void
CreateInstances(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda);

static void
CreatePackedPrimitives(HEMAX_GeometryHda& GeometryHda, HEMAX_Part& Part, std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*>& DisplayGeometries);

static void
CreateEditableCurves(HEMAX_GeometryHda& GeometryHda, HEMAX_EditableNode& EditableNode);

static void
InitGeometryCustAttribContainer(HEMAX_GeometryHda& GeometryHda);

static void
GenerateBoilerplateGeometryCustomAttributes(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda);

static void
InitGeometryPluginCustAttribContainer(INode* PluginNode);

static void
GenerateBoilerplateGeometryPluginCustomAttributes(INode* PluginNode, HEMAX_PartId Part);

std::vector<ULONG>
GetListOfAllChildINodes(HEMAX_GeometryHda& Hda);

void
UpdateGeometryHda(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda);

static void
UpdateInstances(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda);

void
PushEditableNodeChanges(HEMAX_GeometryHda& GeometryHda, HEMAX_EditableCurve EditableCurve);

static void
StampInstanceNode(INode* Node);

static void
StampPackedPrimitiveNode(INode* Node);

static void
StampEditableNode(INode* Node, std::string EditableNodeName, HEMAX_PartId PartNum);

static std::string
GetEditableNodeName(INode* Node);

static int
GetEditablePartNumber(INode* Node);

static void
ClearInstances(HEMAX_GeometryHda& GeometryHda);

static void
ClearPackedPrimNodes(HEMAX_GeometryHda& GeometryHda);

static void
ApplyMaterialsToDisplayGeometry(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_NodeId DisplayNodeId, HEMAX_PartId Part, HEMAX_GeometryPlugin* GeoPlugin);

static void
ApplySceneMtlToGeometryPlugin(HEMAX_GeometryPlugin* GeoPlugin, HEMAX_Mesh* Mesh);

static StdMat2*
CreateMaterialForPart(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_NodeId MatNodeId, HEMAX_NodeId DisplayNodeId, HEMAX_PartId Part);

static void
UpdateChangedMaterials(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_NodeId DisplayNodeId, HEMAX_PartId Part, HEMAX_GeometryPlugin* GeoPlugin);

void
BakeGeometryHda(HEMAX_GeometryHda& GeometryHda);