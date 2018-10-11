#include "HEMAX_GeometryHda.h"

#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Input_NURBS.h"
#include "HEMAX_Input_Spline.h"
#include <icustattribcontainer.h>
#include <custattrib.h>

#include <list>

void
HEMAX_GeometryHda::SetPushTransformsOption(bool Option)
{
    if (ContainerNode)
    {
        ICustAttribContainer* CustAttribs = ContainerNode->GetCustAttribContainer();

        if (CustAttribs)
        {
            CustAttrib* PushTransformAttrib = CustAttribs->GetCustAttrib(HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_INDEX);

            if (PushTransformAttrib)
            {
                PushTransformAttrib->GetParamBlock(0)->SetValue(0, 0, Option);
            }
        }
    }
}

bool
HEMAX_GeometryHda::IsPushTransformsOptionEnabled()
{
    int Enabled = 0;

    if (ContainerNode)
    {
        ICustAttribContainer* CustAttribs = ContainerNode->GetCustAttribContainer();

        if (CustAttribs)
        {
            CustAttrib* PushTransformAttrib = CustAttribs->GetCustAttrib(HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_INDEX);

            if (PushTransformAttrib)
            {
                PushTransformAttrib->GetParamBlock(0)->GetValue(0, 0, Enabled, FOREVER);
            }
        }
    }

    return ((Enabled == 1) ? true : false);
}

void
HEMAX_GeometryHda::SetApplyHAPITransformOption(bool Option)
{
    if (ContainerNode)
    {
        ICustAttribContainer* CustAttribs = ContainerNode->GetCustAttribContainer();

        if (CustAttribs)
        {
            CustAttrib* ApplyTransformAttrib = CustAttribs->GetCustAttrib(HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_INDEX);

            if (ApplyTransformAttrib)
            {
                ApplyTransformAttrib->GetParamBlock(0)->SetValue(0, 0, Option);
            }
        }
    }
}

bool
HEMAX_GeometryHda::ShouldApplyHAPITransform()
{
    int Enabled = 0;

    if (ContainerNode)
    {
        ICustAttribContainer* CustAttribs = ContainerNode->GetCustAttribContainer();

        if (CustAttribs)
        {
            CustAttrib* ApplyTransformAttrib = CustAttribs->GetCustAttrib(HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_INDEX);

            if (ApplyTransformAttrib)
            {
                ApplyTransformAttrib->GetParamBlock(0)->GetValue(0, 0, Enabled, FOREVER);
            }
        }
    }

    return ((Enabled == 1) ? true : false);
}

void
CreateNewGeometryHda(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda)
{
    GeometryHda.Dummy = new DummyObject;
    GeometryHda.ContainerNode = GetCOREInterface()->CreateObjectNode(GeometryHda.Dummy);

    CreateNewGeometryHda(GeometryHda, Hda, GeometryHda.ContainerNode);
}

void
CreateNewGeometryHda(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, INode* ContainerNode)
{
    GeometryHda.ContainerNode = ContainerNode;

    if (GeometryHda.ContainerNode)
    {
        std::wstring ContainerNodeLabel = std::wstring(Hda.Name.begin(), Hda.Name.end());
        GeometryHda.ContainerNode->SetName(ContainerNodeLabel.c_str());
        InitGeometryCustAttribContainer(GeometryHda);
        GenerateBoilerplateGeometryCustomAttributes(GeometryHda, Hda);

        if (Hda.ObjLevelHda)
        {
            HEMAX_Object& TopNode = Hda.TopLevelObjectNode;

            // TODO : investigate transform behaviour
            //HEMAX_Utilities::SetINodeTransform(GeometryHda.ContainerNode, HEMAX_Utilities::HAPITransformToMaxTransform(TopNode.Transform));

            for (auto It = TopNode.SopNodes.begin(); It != TopNode.SopNodes.end(); It++)
            {
                if (It->second.Parts.size() > 0)
                {
                    CreateDisplayGeometry(GeometryHda, Hda, It->second);
                }
            }

            if (TopNode.Instances.size() > 0)
            {
                CreateInstances(GeometryHda, Hda);
            }
        }
        else if (Hda.SopLevelHda)
        {
            HEMAX_DisplayGeoNode TopNode = Hda.TopLevelSopNode;

            if (TopNode.Parts.size() > 0)
            {
                CreateDisplayGeometry(GeometryHda, Hda, TopNode);
            }
        }

        for (int e = 0; e < Hda.EditableNodes.size(); e++)
        {
            CreateEditableCurves(GeometryHda, Hda.EditableNodes[e]);
        }
    }
}

void
CreateGeometryHdaFromContainerNode(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, INode* ContainerNode)
{
    GeometryHda.Dummy = (DummyObject*)ContainerNode->GetObjectRef();
    GeometryHda.ContainerNode = ContainerNode;
    GeometryHda.CustomAttributes = ContainerNode->GetCustAttribContainer();
    GeometryHda.MaxStampIndex = HEMAX_MAX_HOUDINI_MAX_INDEX;

    for (int i = HEMAX_MAX_GEO_MAX_INDEX; i < GeometryHda.CustomAttributes->GetNumCustAttribs(); i++)
    {
        HEMAX_ParameterAttrib* CustAttrib = static_cast<HEMAX_ParameterAttrib*>(GeometryHda.CustomAttributes->GetCustAttrib(i));
        GeometryHda.CustomAttributeMap.insert({ CustAttrib->GetParameterName(), CustAttrib });
    }

    if (GeometryHda.ContainerNode)
    {
        int ChildCount = GeometryHda.ContainerNode->NumberOfChildren();
        if (Hda.ObjLevelHda)
        {
            for (int i = 0; i < ChildCount; i++)
            {
                INode* Child = GeometryHda.ContainerNode->GetChildNode(i);
                if (Child)
                {
                    if (IsINodeADisplayGeometry(Child))
                    {
                        HEMAX_GeometryPlugin* GeoPlugin = dynamic_cast<HEMAX_GeometryPlugin*>(Child->GetObjectRef());
                        GeoPlugin->MaxNode = Child;
                        GeoPlugin->IsStranded = false;
                        std::wstring NodeName = Child->GetName();

                        HEMAX_DisplayGeoNode SopNode = FindSopNodeFromName(Hda.TopLevelObjectNode, std::string(NodeName.begin(), NodeName.end()));
                        if (SopNode.NodeName != "Invalid")
                        {
                            std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*> DisplayGeometries;
                            HEMAX_PartId PartId = GetPartIdFromCustomAttributes(Child);

                            if (PartId != -1)
                            {
                                auto Lookup = GeometryHda.DisplayGeometry.find({ SopNode.DisplayGeoInfo.nodeId });
                                if (Lookup != GeometryHda.DisplayGeometry.end())
                                {
                                    Lookup->second.insert({ PartId, GeoPlugin });
                                }
                                else
                                {
                                    DisplayGeometries.insert({ PartId, GeoPlugin });
                                    GeometryHda.DisplayGeometry.insert({ SopNode.DisplayGeoInfo.nodeId, DisplayGeometries });
                                }
                            }
                        }
                    }
                    else if (IsINodeAnEditableNode(Child))
                    {
                        std::string EditableNodeName = GetEditableNodeName(Child);
                        int PartNum = GetEditablePartNumber(Child);
                        bool Success;
                        HEMAX_EditableNode EditableNode = FindEditableNodeFromName(Hda, EditableNodeName, Success);
                        if (Success)
                        {
                            HEMAX_EditableCurve EditableCurve;
                            EditableCurve.PushNodeId = EditableNode.GeoInfo.nodeId;
                            EditableCurve.PushPartId = PartNum;
                            EditableCurve.Node = Child;
                            EditableCurve.Dirty = true;
                            HEMAX_SessionManager::GetSessionManager().Session->GetCurveInfo(EditableNode.GeoInfo.nodeId, PartNum, &EditableCurve.CurveInfo);
                            GeometryHda.EditableCurves.push_back(EditableCurve);
                        }
                    }
                }
            }
        }
        else if (Hda.SopLevelHda)
        {
            for (int i = 0; i < ChildCount; i++)
            {
                INode* Child = GeometryHda.ContainerNode->GetChildNode(i);
                if (Child)
                {
                    if (IsINodeADisplayGeometry(Child))
                    {
                        // Sop level HDAs should only have one relevant child node (and this is the one that we just found)
                        HEMAX_GeometryPlugin* GeoPlugin = dynamic_cast<HEMAX_GeometryPlugin*>(Child->GetObjectRef());
                        GeoPlugin->MaxNode = Child;
                        GeoPlugin->IsStranded = false;

                        std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*> DisplayGeometries;
                        HEMAX_PartId PartId = GetPartIdFromCustomAttributes(Child);

                        if (PartId != -1)
                        {
                            auto Lookup = GeometryHda.DisplayGeometry.find({ Hda.TopLevelSopNode.DisplayGeoInfo.nodeId });
                            if (Lookup != GeometryHda.DisplayGeometry.end())
                            {
                                Lookup->second.insert({ PartId, GeoPlugin });
                            }
                            else
                            {
                                DisplayGeometries.insert({ PartId, GeoPlugin });
                                GeometryHda.DisplayGeometry.insert({ Hda.TopLevelSopNode.DisplayGeoInfo.nodeId, DisplayGeometries });
                            }
                        }
                    }
                    else if (IsINodeAnEditableNode(Child))
                    {
                        std::string EditableNodeName = GetEditableNodeName(Child);
                        int PartNum = GetEditablePartNumber(Child);
                        bool Success;
                        HEMAX_EditableNode EditableNode = FindEditableNodeFromName(Hda, EditableNodeName, Success);
                        if (Success)
                        {
                            HEMAX_EditableCurve EditableCurve;
                            EditableCurve.PushNodeId = EditableNode.GeoInfo.nodeId;
                            EditableCurve.PushPartId = PartNum;
                            EditableCurve.Node = Child;
                            EditableCurve.Dirty = true;
                            HEMAX_SessionManager::GetSessionManager().Session->GetCurveInfo(EditableNode.GeoInfo.nodeId, PartNum, &EditableCurve.CurveInfo);
                            GeometryHda.EditableCurves.push_back(EditableCurve);
                        }
                    }
                }
            }
        }
    }
}

bool
IsINodeADisplayGeometry(INode* Node)
{
    ICustAttribContainer* CustAttribs = Node->GetCustAttribContainer();
    if (CustAttribs)
    {
        if (DoesCustomAttributeExist(CustAttribs, HEMAX_MAX_GEO_STAMP_INDEX, HEMAX_MAX_GEO_STAMP_NAME))
        {
            return true;
        }
    }

    return false;
}

static bool
IsINodeAnEditableNode(INode* Node)
{
    ICustAttribContainer* CustAttribs = Node->GetCustAttribContainer();
    if (CustAttribs)
    {
        if (DoesCustomAttributeExist(CustAttribs, HEMAX_EDITABLE_NODE_STAMP_INDEX, HEMAX_EDITABLE_NODE_STAMP_NAME))
        {
            return true;
        }
    }

    return false;
}

HEMAX_PartId
GetPartIdFromCustomAttributes(INode* Node)
{
    ICustAttribContainer* CustAttribs = Node->GetCustAttribContainer();
    if (CustAttribs)
    {
        if (DoesCustomAttributeExist(CustAttribs, HEMAX_MAX_GEO_PART_STAMP_INDEX, HEMAX_MAX_GEO_PART_STAMP_NAME))
        {
            CustAttrib* PartCustAttrib = Node->GetCustAttribContainer()->GetCustAttrib(HEMAX_MAX_GEO_PART_STAMP_INDEX);
            HEMAX_ParameterAttrib* Attrib = dynamic_cast<HEMAX_ParameterAttrib*>(PartCustAttrib);
            int Value;
            Attrib->PBlock->GetValue(0, GetCOREInterface()->GetTime(), Value, FOREVER);
            return Value;
        }
    }

    return -1;
}

void
CreateDisplayGeometry(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_DisplayGeoNode& DisplayNode)
{
    std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*> DisplayGeometries;

    // First pass is to create any Mesh geometries
    for (int p = 0; p < DisplayNode.Parts.size(); p++)
    {
        if (DisplayNode.Parts[p].Info.type == HEMAX_PARTTYPE_MESH)
        {
            CreateMeshPart(GeometryHda, Hda, DisplayNode, DisplayNode.Parts[p], DisplayGeometries);
        }
    }

    if (DisplayGeometries.size() > 0)
    {
        GeometryHda.DisplayGeometry.insert({ DisplayNode.DisplayGeoInfo.nodeId, DisplayGeometries });
    }

    // Second pass is for instancing (packed primitives)
    for (int p = 0; p < DisplayNode.Parts.size(); p++)
    {
        if (DisplayNode.Parts[p].Info.type == HEMAX_PARTTYPE_INSTANCER)
        {
            CreatePackedPrimitives(GeometryHda, DisplayNode.Parts[p], DisplayGeometries);
        }
    }
}

static void
CreateMeshPart(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_DisplayGeoNode& DisplayNode, HEMAX_Part& Part, std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*>& DisplayGeometries)
{
    HEMAX_GeometryPlugin* NewPlugin = new HEMAX_GeometryPlugin();
    NewPlugin->IsStranded = false;
    INode* PluginNode = GetCOREInterface()->CreateObjectNode(NewPlugin);
    NewPlugin->MaxNode = PluginNode;

    std::wstring PluginLabel = std::wstring(DisplayNode.NodeName.begin(), DisplayNode.NodeName.end());
    PluginNode->SetName(PluginLabel.c_str());

    GeometryHda.ContainerNode->AttachChild(PluginNode);
    DisplayGeometries.insert({ Part.Info.id, NewPlugin });

    LoadMesh(&NewPlugin->Mesh, DisplayNode, Part.Info.id);
    NewPlugin->BuildMesh(GetCOREInterface()->GetTime());

    InitGeometryPluginCustAttribContainer(PluginNode);
    GenerateBoilerplateGeometryPluginCustomAttributes(PluginNode, Part.Info.id);

    ApplyMaterialsToDisplayGeometry(GeometryHda, Hda, DisplayNode.DisplayGeoInfo.nodeId, Part.Info.id, NewPlugin);

    if (!DisplayNode.IsVisible || Part.Info.isInstanced)
    {
        PluginNode->Hide(true);
    }
    else
    {
        PluginNode->Hide(false);
    }
}

void
CreateInstances(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda)
{
    HEMAX_Object& ObjectNode = Hda.TopLevelObjectNode;

    for (auto InstIt = ObjectNode.Instances.begin(); InstIt != ObjectNode.Instances.end(); InstIt++)
    {
        if (InstIt->second.InstanceAttributeExists)
        {
            for (int s = 0; s < InstIt->second.InstanceNodeIds.size(); s++)
            {
                // Different display geometries
                HEMAX_GeometryInfo DisplayGeoInfo;
                HEMAX_SessionManager::GetSessionManager().Session->GetDisplayGeoInfo(InstIt->second.InstanceNodeIds[s], &DisplayGeoInfo);
                HEMAX_NodeId ObjToInstance = DisplayGeoInfo.nodeId;

                auto ObjSearch = GeometryHda.DisplayGeometry.find(ObjToInstance);

                if (ObjSearch != GeometryHda.DisplayGeometry.end())
                {
                    std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*>& GeoPlugins = ObjSearch->second;

                    INodeTab CloneableNodeTab;

                    for (auto It = GeoPlugins.begin(); It != GeoPlugins.end(); It++)
                    {
                        INode* CloneableSource = It->second->MaxNode;

                        if (CloneableSource)
                        {
                            CloneableNodeTab.AppendNode(CloneableSource);
                        }
                    }

                    INodeTab ClonedNodeTab;
                    bool CloneResult = GetCOREInterface()->CloneNodes(CloneableNodeTab, Point3(0, 0, 0), true, NODE_INSTANCE, nullptr, &ClonedNodeTab);

                    if (CloneResult)
                    {
                        INode* ParentNode = GeometryHda.ContainerNode;
                        for (int c = 0; c < ClonedNodeTab.Count(); c++)
                        {
                            ClonedNodeTab[c]->SetNodeTM(GetCOREInterface()->GetTime(), ParentNode->GetNodeTM(GetCOREInterface()->GetTime()));
                            HEMAX_Utilities::ApplyTransformToINode(ClonedNodeTab[c], InstIt->second.InstanceTransforms[s]);
                            ClonedNodeTab[c]->Hide(false);
                            StampInstanceNode(ClonedNodeTab[c]);
                            GeometryHda.InstanceClones.push_back(ClonedNodeTab[c]);
                        }
                    }
                }
                else
                {
                    HEMAX_Logger::Instance().AddEntry("HEMAX_GeometryHda: Error->attempting to instance an object node that does not exist and should exist.", HEMAX_LOG_LEVEL_ERROR);
                }
            }
        }
        else
        {
            HEMAX_GeometryInfo DisplayGeoInfo;
            HEMAX_SessionManager::GetSessionManager().Session->GetDisplayGeoInfo(InstIt->second.InstanceeId, &DisplayGeoInfo);
            HEMAX_NodeId ObjToInstance = DisplayGeoInfo.nodeId;

            auto ObjSearch = GeometryHda.DisplayGeometry.find(ObjToInstance);
            std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*>* GeoPlugins = nullptr;

            if (ObjSearch != GeometryHda.DisplayGeometry.end())
            {
                GeoPlugins = &(ObjSearch->second);
            }

            for (int s = 0; s < InstIt->second.InstanceTransforms.size(); s++)
            {
                if (GeoPlugins)
                {
                    INodeTab CloneableNodeTab;

                    for (auto It = GeoPlugins->begin(); It != GeoPlugins->end(); It++)
                    {
                        INode* CloneableSource = It->second->MaxNode;

                        if (CloneableSource)
                        {
                            CloneableNodeTab.AppendNode(CloneableSource);
                        }
                    }

                    INodeTab ClonedNodeTab;
                    bool CloneResult = GetCOREInterface()->CloneNodes(CloneableNodeTab, Point3(0, 0, 0), true, NODE_INSTANCE, nullptr, &ClonedNodeTab);

                    if (CloneResult)
                    {
                        INode* ParentNode = GeometryHda.ContainerNode;
                        for (int c = 0; c < ClonedNodeTab.Count(); c++)
                        {
                            ClonedNodeTab[c]->SetNodeTM(GetCOREInterface()->GetTime(), ParentNode->GetNodeTM(GetCOREInterface()->GetTime()));
                            HEMAX_Utilities::ApplyTransformToINode(ClonedNodeTab[c], InstIt->second.InstanceTransforms[s]);
                            ClonedNodeTab[c]->Hide(false);
                            StampInstanceNode(ClonedNodeTab[c]);
                            GeometryHda.InstanceClones.push_back(ClonedNodeTab[c]);
                        }
                    }
                }
            }
        }
    }
}

void
CreatePackedPrimitives(HEMAX_GeometryHda& GeometryHda, HEMAX_Part& Part, std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*>& DisplayGeometries)
{
    HEMAX_PackedPrimitiveInstanceInfo& InstanceInfo = Part.PackedPrimInfo;
    for (int i = 0; i < InstanceInfo.InstanceCount; i++)
    {
        for (int t = 0; t < InstanceInfo.InstancedPartCount; t++)
        {
            HEMAX_PartId SourcePartId = InstanceInfo.InstancedPartIds[t];

            auto Search = DisplayGeometries.find(SourcePartId);
            if (Search != DisplayGeometries.end())
            {
                HEMAX_GeometryPlugin* GeoPluginSource = Search->second;
                if (GeoPluginSource)
                {
                    INode* NodeSource = GeoPluginSource->MaxNode;
                    if (NodeSource)
                    {
                        INodeTab CloneableNodeTab;
                        CloneableNodeTab.AppendNode(NodeSource);

                        INodeTab ClonedNodeTab;
                        bool CloneResult = GetCOREInterface()->CloneNodes(CloneableNodeTab, Point3(0, 0, 0), true, NODE_INSTANCE, nullptr, &ClonedNodeTab);
                        if (CloneResult)
                        {
                            INode* ClonedNode = ClonedNodeTab[0];
                            INode* ParentNode = GeometryHda.ContainerNode;
                            ClonedNode->SetNodeTM(GetCOREInterface()->GetTime(), ParentNode->GetNodeTM(GetCOREInterface()->GetTime()));
                            HEMAX_Utilities::ApplyTransformToINode(ClonedNode, InstanceInfo.InstancedTransforms[i]);
                            ClonedNode->Hide(false);
                            StampPackedPrimitiveNode(ClonedNode);
                            GeometryHda.PackedPrimClones.push_back(ClonedNode);
                        }
                    }
                }
            }
        }
    }
}

static void
CreateEditableCurves(HEMAX_GeometryHda& GeometryHda, HEMAX_EditableNode& EditableNode)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    for (int p = 0; p < EditableNode.Parts.size(); p++)
    {
        if (EditableNode.Parts[p].Info.type == HEMAX_PARTTYPE_CURVE)
        {
            HEMAX_EditableCurve EditableCurve;
            EditableCurve.Node = nullptr;
            EditableCurve.PushNodeId = EditableNode.GeoInfo.nodeId;
            EditableCurve.PushPartId = p;
            if (SM.Session->GetCurveInfo(EditableNode.GeoInfo.nodeId, EditableNode.Parts[p].Info.id, &EditableCurve.CurveInfo))
            {
                switch (EditableCurve.CurveInfo.curveType)
                {
                case (HEMAX_CURVETYPE_LINEAR):
                {
                    EditableCurve.Node = MarshallDataInto3dsMaxLinearCurve(EditableCurve, EditableNode, p);
                } break;
                case (HEMAX_CURVETYPE_NURBS):
                {
                    EditableCurve.Node = MarshallDataInto3dsMaxNURBSCVCurve(EditableCurve, EditableNode, p);
                } break;
                default:
                {
                    HEMAX_Logger::Instance().AddEntry("HEMAX_Loader::Found editable curve node but curve type is invalid or not supported", HEMAX_LOG_LEVEL_WARN);
                } break;
                }
            }

            if (EditableCurve.Node)
            {
                std::string NodeName = SM.Session->GetHAPIString(EditableNode.GeoInfo.nameSH);
                std::wstring Name(NodeName.begin(), NodeName.end());
                EditableCurve.Node->SetName(Name.c_str());

                if (GeometryHda.ContainerNode)
                {
                    StampEditableNode(EditableCurve.Node, NodeName, p);
                    GeometryHda.ContainerNode->AttachChild(EditableCurve.Node, 0);
                    GeometryHda.EditableCurves.push_back(EditableCurve);
                }
            }
        }
        else if (EditableNode.Parts[p].Info.type == HEMAX_PARTTYPE_MESH)
        {
            // In this case the only possibility is that it's a single polygon representing a linear curve.
            // TODO : editable linear curve
        }
    }
}

static void
InitGeometryCustAttribContainer(HEMAX_GeometryHda& GeometryHda)
{
    GeometryHda.CustomAttributes = GeometryHda.ContainerNode->GetCustAttribContainer();

    if (!GeometryHda.CustomAttributes)
    {
        GeometryHda.ContainerNode->AllocCustAttribContainer();
        GeometryHda.CustomAttributes = GeometryHda.ContainerNode->GetCustAttribContainer();
    }
}

static void
GenerateBoilerplateGeometryCustomAttributes(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda)
{
    // Generate HEMAX boilerplate custom attributes. These are necessary for reloading the scene
    HEMAX_StringParameterAttrib* MaxHoudiniAssetStamp = new HEMAX_StringParameterAttrib;
    MaxHoudiniAssetStamp->SetParameterName(std::string(HEMAX_MAX_HOUDINI_STAMP_NAME));
    MaxHoudiniAssetStamp->PBlock->SetValue(0, 0, L"TRUE");
    GeometryHda.CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_STAMP_INDEX, MaxHoudiniAssetStamp);

    HEMAX_StringParameterAttrib* MaxHoudiniAssetPath = new HEMAX_StringParameterAttrib;
    MaxHoudiniAssetPath->SetParameterName(std::string(HEMAX_MAX_HOUDINI_ASSET_PATH_NAME));
    std::wstring WideAssetPath(Hda.HdaAsset.Path.begin(), Hda.HdaAsset.Path.end());
    MaxHoudiniAssetPath->PBlock->SetValue(0, 0, WideAssetPath.c_str());
    GeometryHda.CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_ASSET_PATH_INDEX, MaxHoudiniAssetPath);

    HEMAX_IntegerParameterAttrib* MaxHoudiniAssetLibIndex = new HEMAX_IntegerParameterAttrib;
    MaxHoudiniAssetLibIndex->SetParameterName(std::string(HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_NAME));
    MaxHoudiniAssetLibIndex->PBlock->SetValue(0, 0, Hda.HdaAssetIndex);
    GeometryHda.CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_INDEX, MaxHoudiniAssetLibIndex);

    HEMAX_ToggleParameterAttrib* MaxHoudiniConvertOnSave = new HEMAX_ToggleParameterAttrib;
    MaxHoudiniConvertOnSave->SetParameterName(std::string(HEMAX_MAX_HOUDINI_SAVE_CONVERSION_NAME));
    MaxHoudiniConvertOnSave->PBlock->SetValue(0, 0, 0);
    GeometryHda.CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_SAVE_CONVERSION_INDEX, MaxHoudiniConvertOnSave);

    HEMAX_ToggleParameterAttrib* MaxHoudiniPushTransform = new HEMAX_ToggleParameterAttrib;
    MaxHoudiniPushTransform->SetParameterName(std::string(HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_NAME));
    MaxHoudiniPushTransform->PBlock->SetValue(0, 0, 0);
    GeometryHda.CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_INDEX, MaxHoudiniPushTransform);

    HEMAX_ToggleParameterAttrib* MaxHoudiniApplyHAPITransform = new HEMAX_ToggleParameterAttrib;
    MaxHoudiniApplyHAPITransform->SetParameterName(std::string(HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_NAME));
    MaxHoudiniApplyHAPITransform->PBlock->SetValue(0, 0, 0);
    GeometryHda.CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_INDEX, MaxHoudiniApplyHAPITransform);

    // Make custom attributes for the sub-network inputs
    for (int s = 0; s < Hda.MainNode.Info.inputCount; ++s)
    {
        HEMAX_NodeParameterAttrib* InputCustAttrib = new HEMAX_NodeParameterAttrib;
        InputCustAttrib->SetParameterName("subnetwork_" + std::to_string(s));
        InputCustAttrib->CreateMaxHoudiniAssetLink(GeometryHda.ContainerNode, HEMAX_INPUT_SUBNETWORK, s);
        GeometryHda.CustomAttributes->AppendCustAttrib(InputCustAttrib);
        GeometryHda.CustomAttributeMap.insert({ "subnetwork_" + std::to_string(s), InputCustAttrib });
    }

    GeometryHda.MaxStampIndex = HEMAX_MAX_HOUDINI_MAX_INDEX;
}

static void
InitGeometryPluginCustAttribContainer(INode* PluginNode)
{
    ICustAttribContainer* Container = PluginNode->GetCustAttribContainer();

    if (!Container)
    {
        PluginNode->AllocCustAttribContainer();
    }
}

static void
GenerateBoilerplateGeometryPluginCustomAttributes(INode* PluginNode, HEMAX_PartId Part)
{
    HEMAX_StringParameterAttrib* MaxGeoStamp = new HEMAX_StringParameterAttrib;
    MaxGeoStamp->SetParameterName(std::string(HEMAX_MAX_GEO_STAMP_NAME));
    MaxGeoStamp->PBlock->SetValue(0, GetCOREInterface()->GetTime(), L"TRUE");

    HEMAX_IntegerParameterAttrib* PartStamp = new HEMAX_IntegerParameterAttrib;
    PartStamp->SetParameterName(std::string(HEMAX_MAX_GEO_PART_STAMP_NAME));
    PartStamp->PBlock->SetValue(0, GetCOREInterface()->GetTime(), Part);

    if (PluginNode->GetCustAttribContainer())
    {
        PluginNode->GetCustAttribContainer()->InsertCustAttrib(HEMAX_MAX_GEO_STAMP_INDEX, MaxGeoStamp);
        PluginNode->GetCustAttribContainer()->InsertCustAttrib(HEMAX_MAX_GEO_PART_STAMP_INDEX, PartStamp);
    }
}

std::vector<ULONG>
GetListOfAllChildINodes(HEMAX_GeometryHda& Hda)
{
    std::vector<ULONG> Nodes;

    for (auto It = Hda.DisplayGeometry.begin(); It != Hda.DisplayGeometry.end(); It++)
    {
        for (int i = 0; i < It->second.size(); i++)
        {
            if (It->second[i])
            {
                Nodes.push_back(It->second[i]->MaxNode->GetHandle());
            }
        }
    }

    for (int j = 0; j < Hda.EditableCurves.size(); j++)
    {
        if (Hda.EditableCurves[j].Node)
        {
            Nodes.push_back(Hda.EditableCurves[j].Node->GetHandle());
        }
    }

    return Nodes;
}

void
UpdateGeometryHda(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda)
{
    // Clear any packed primitive nodes
    ClearPackedPrimNodes(GeometryHda);

    if (GeometryHda.ContainerNode)
    {
        if (Hda.ObjLevelHda)
        {
            std::list<HEMAX_NodeId> NodeList;
            for (auto NodeEntry : GeometryHda.DisplayGeometry)
            {
                NodeList.push_back(NodeEntry.first);
            }

            HEMAX_Object& TopNode = Hda.TopLevelObjectNode;

            // TODO : investigate transform behaviour
            // HEMAX_Utilities::SetINodeTransform(GeometryHda.ContainerNode, HEMAX_Utilities::HAPITransformToMaxTransform(TopNode.Transform));

            for (auto SopIter = TopNode.SopNodes.begin(); SopIter != TopNode.SopNodes.end(); SopIter++)
            {
                NodeList.remove(SopIter->second.DisplayGeoInfo.nodeId);

                if (SopIter->second.Parts.size() > 0)
                {
                    auto Search = GeometryHda.DisplayGeometry.find(SopIter->second.DisplayGeoInfo.nodeId);

                    if (Search != GeometryHda.DisplayGeometry.end())
                    {
                        std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*>& Plugins = Search->second;

                        // First pass for Mesh parts
                        for (int p = 0; p < SopIter->second.Parts.size(); p++)
                        {
                            auto PluginIt = Plugins.find(SopIter->second.Parts[p].Info.id);
                            if (PluginIt != Plugins.end())
                            {
                                if (SopIter->second.Parts[p].Info.type == HEMAX_PARTTYPE_MESH)
                                {
                                    delete PluginIt->second->Mesh;
                                    LoadMesh(&PluginIt->second->Mesh, SopIter->second, p);
                                    PluginIt->second->BuildMesh(GetCOREInterface()->GetTime());
                                    PluginIt->second->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
                                    UpdateChangedMaterials(GeometryHda, Hda, Search->first, p, PluginIt->second);
                                }
                            }
                            else
                            {
                                // New part
                                if (SopIter->second.Parts[p].Info.type == HEMAX_PARTTYPE_MESH)
                                {
                                    CreateMeshPart(GeometryHda, Hda, SopIter->second, SopIter->second.Parts[p], Plugins);
                                }
                            }
                        }

                        // Second pass for packed prims
                        for (int p = 0; p < SopIter->second.Parts.size(); p++)
                        {
                            if (SopIter->second.Parts[p].Info.type == HEMAX_PARTTYPE_INSTANCER)
                            {
                                CreatePackedPrimitives(GeometryHda, SopIter->second.Parts[p], Plugins);
                            }
                        }
                    }
                    else
                    {
                        CreateDisplayGeometry(GeometryHda, Hda, SopIter->second);
                    }
                }
            }

            if (TopNode.Instances.size() > 0)
            {
                UpdateInstances(GeometryHda, Hda);
            }

            for (auto UnseenNode : NodeList)
            {
                auto Search = GeometryHda.DisplayGeometry.find(UnseenNode);
                if (Search != GeometryHda.DisplayGeometry.end())
                {
                    for (auto PluginIt = Search->second.begin(); PluginIt != Search->second.end(); PluginIt++)
                    {
                        GetCOREInterface()->DeleteNode(PluginIt->second->MaxNode);
                        PluginIt->second = nullptr;
                    }
                    Search->second.clear();
                    GeometryHda.DisplayGeometry.erase(UnseenNode);
                }

                auto MatSearch = GeometryHda.MaterialMap.find(UnseenNode);
                if (MatSearch != GeometryHda.MaterialMap.end())
                {
                    for (auto MtlIt = MatSearch->second.begin(); MtlIt != MatSearch->second.end(); MtlIt++)
                    {
                        for (int m = 0; m < MtlIt->second.size(); m++)
                        {
                            delete MtlIt->second[m];
                        }
                        MtlIt->second.clear();
                    }
                    MatSearch->second.clear();
                    GeometryHda.MaterialMap.erase(UnseenNode);
                }
            }
        }
        else if (Hda.SopLevelHda)
        {
            HEMAX_DisplayGeoNode TopNode = Hda.TopLevelSopNode;

            if (TopNode.Parts.size() > 0)
            {
                auto Search = GeometryHda.DisplayGeometry.find(TopNode.DisplayGeoInfo.nodeId);

                if (Search != GeometryHda.DisplayGeometry.end())
                {
                    std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*>& Plugins = Search->second;

                    std::unordered_map<HEMAX_PartId, HEMAX_GeometryPlugin*> UnseenPlugins = Plugins;

                    // First pass for Mesh parts
                    for (int p = 0; p < TopNode.Parts.size(); p++)
                    {
                        auto PluginIt = Plugins.find(TopNode.Parts[p].Info.id);
                        if (PluginIt != Plugins.end())
                        {
                            if (TopNode.Parts[p].Info.type == HEMAX_PARTTYPE_MESH)
                            {
                                delete PluginIt->second->Mesh;
                                LoadMesh(&PluginIt->second->Mesh, TopNode, p);
                                PluginIt->second->BuildMesh(GetCOREInterface()->GetTime());
                                PluginIt->second->ForceNotify(Interval(GetCOREInterface()->GetTime(), GetCOREInterface()->GetTime()));
                                UpdateChangedMaterials(GeometryHda, Hda, TopNode.DisplayGeoInfo.nodeId, p, PluginIt->second);
                                UnseenPlugins.erase(TopNode.Parts[p].Info.id);
                            }
                        }
                        else
                        {
                            // New part
                            if (TopNode.Parts[p].Info.type == HEMAX_PARTTYPE_MESH)
                            {
                                CreateMeshPart(GeometryHda, Hda, TopNode, TopNode.Parts[p], Plugins);
                            }
                        }
                    }

                    // Second pass for packed prims
                    for (int p = 0; p < TopNode.Parts.size(); p++)
                    {
                        if (TopNode.Parts[p].Info.type == HEMAX_PARTTYPE_INSTANCER)
                        {
                            CreatePackedPrimitives(GeometryHda, TopNode.Parts[p], Plugins);
                        }
                    }

                    // Remove any 'unseen' plugins... this means that they are no longer in use
                    for (auto PluginIt = UnseenPlugins.begin(); PluginIt != UnseenPlugins.end(); PluginIt++)
                    {
                        auto PluginSearch = Plugins.find(PluginIt->first);
                        if (PluginSearch != Plugins.end())
                        {
                            GetCOREInterface()->DeleteNode(PluginSearch->second->MaxNode);
                            PluginSearch->second = nullptr;
                            Plugins.erase(PluginIt->first);
                        }
                    }
                }
                else
                {
                    CreateDisplayGeometry(GeometryHda, Hda, TopNode);
                }
            }
        }
    }
}

void
UpdateInstances(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda)
{
    ClearInstances(GeometryHda);

    int ChildCount = GeometryHda.ContainerNode->NumberOfChildren();

    for (int c = ChildCount - 1; c >= 0; c--)
    {
        INode* Child = GeometryHda.ContainerNode->GetChildNode(c);
        if (Child)
        {
            ICustAttribContainer* CustAttribs = Child->GetCustAttribContainer();
            if (CustAttribs)
            {
                if (DoesCustomAttributeExist(CustAttribs, HEMAX_MAX_GEO_INSTANCE_STAMP_INDEX, HEMAX_MAX_GEO_INSTANCE_STAMP_NAME))
                {
                    GetCOREInterface()->DeleteNode(Child);
                }
            }
        }
    }

    CreateInstances(GeometryHda, Hda);
}

void
PushEditableNodeChanges(HEMAX_GeometryHda& GeometryHda, HEMAX_EditableCurve EditableCurve)
{
    for (int i = 0; i < GeometryHda.EditableCurves.size(); i++)
    {
        if (GeometryHda.EditableCurves[i].Node && (GeometryHda.EditableCurves[i].Node == EditableCurve.Node))
        {
            switch (EditableCurve.CurveInfo.curveType)
            {
            case (HEMAX_CURVETYPE_LINEAR):
            {
                HEMAX_Node Node(GeometryHda.EditableCurves[i].PushNodeId, HEMAX_NODE_SOP);
                HEMAX_Input_Spline LinearInput(&Node, GeometryHda.EditableCurves[i].Node->GetHandle());
                GeometryHda.EditableCurves[i].Dirty = false;
            } break;
            case (HEMAX_CURVETYPE_NURBS):
            {
                HEMAX_Node Node(GeometryHda.EditableCurves[i].PushNodeId, HEMAX_NODE_SOP);
                HEMAX_Input_NURBS NURBSInput(&Node, GeometryHda.EditableCurves[i].Node->GetHandle());
                GeometryHda.EditableCurves[i].Dirty = false;
            } break;
            default:
            {

            } break;
            }
        }
    }
}

void
StampInstanceNode(INode* Node)
{
    ICustAttribContainer* CustAttribs = Node->GetCustAttribContainer();

    if (!CustAttribs)
    {
        Node->AllocCustAttribContainer();
        CustAttribs = Node->GetCustAttribContainer();
    }

    HEMAX_StringParameterAttrib* InstanceStamp = new HEMAX_StringParameterAttrib;
    InstanceStamp->SetParameterName(std::string(HEMAX_MAX_GEO_INSTANCE_STAMP_NAME));
    InstanceStamp->PBlock->SetValue(0, GetCOREInterface()->GetTime(), L"TRUE");
    CustAttribs->InsertCustAttrib(HEMAX_MAX_GEO_INSTANCE_STAMP_INDEX, InstanceStamp);
}

void
StampPackedPrimitiveNode(INode* Node)
{
    ICustAttribContainer* CustAttribs = Node->GetCustAttribContainer();

    if (!CustAttribs)
    {
        Node->AllocCustAttribContainer();
        CustAttribs = Node->GetCustAttribContainer();
    }

    HEMAX_StringParameterAttrib* PackedPrimStamp = new HEMAX_StringParameterAttrib;
    PackedPrimStamp->SetParameterName(std::string(HEMAX_MAX_GEO_PACKED_PRIM_STAMP_NAME));
    PackedPrimStamp->PBlock->SetValue(0, GetCOREInterface()->GetTime(), L"TRUE");
    CustAttribs->InsertCustAttrib(HEMAX_MAX_GEO_PACKED_PRIM_STAMP_INDEX, PackedPrimStamp);
}

static void
StampEditableNode(INode* Node, std::string EditableNodeName, HEMAX_PartId PartNum)
{
    ICustAttribContainer* CustAttribs = Node->GetCustAttribContainer();

    if (!CustAttribs)
    {
        Node->AllocCustAttribContainer();
        CustAttribs = Node->GetCustAttribContainer();
    }

    HEMAX_StringParameterAttrib* EditableStamp = new HEMAX_StringParameterAttrib;
    EditableStamp->SetParameterName(std::string(HEMAX_EDITABLE_NODE_STAMP_NAME));
    EditableStamp->PBlock->SetValue(0, GetCOREInterface()->GetTime(), L"TRUE");
    CustAttribs->InsertCustAttrib(HEMAX_EDITABLE_NODE_STAMP_INDEX, EditableStamp);

    HEMAX_StringParameterAttrib* NameStamp = new HEMAX_StringParameterAttrib;
    NameStamp->SetParameterName(std::string(HEMAX_EDITABLE_NODE_NAME_NAME));
    std::wstring WideName(EditableNodeName.begin(), EditableNodeName.end());
    NameStamp->PBlock->SetValue(0, GetCOREInterface()->GetTime(), WideName.c_str());
    CustAttribs->InsertCustAttrib(HEMAX_EDITABLE_NODE_NAME_INDEX, NameStamp);

    HEMAX_IntegerParameterAttrib* PartStamp = new HEMAX_IntegerParameterAttrib;
    PartStamp->SetParameterName(std::string(HEMAX_EDITABLE_NODE_PART_NUM_NAME));
    PartStamp->PBlock->SetValue(0, GetCOREInterface()->GetTime(), PartNum);
    CustAttribs->InsertCustAttrib(HEMAX_EDITABLE_NODE_PART_NUM_INDEX, PartStamp);
}

static std::string
GetEditableNodeName(INode* Node)
{
    ICustAttribContainer* CustAttribs = Node->GetCustAttribContainer();

    if (DoesCustomAttributeExist(CustAttribs, HEMAX_EDITABLE_NODE_NAME_INDEX, HEMAX_EDITABLE_NODE_NAME_NAME))
    {
        HEMAX_StringParameterAttrib* NameAttrib = dynamic_cast<HEMAX_StringParameterAttrib*>(CustAttribs->GetCustAttrib(HEMAX_EDITABLE_NODE_NAME_INDEX));
        const MCHAR* Name;
        NameAttrib->PBlock->GetValue(0, GetCOREInterface()->GetTime(), Name, FOREVER);
        std::wstring WideName(Name);
        return std::string(WideName.begin(), WideName.end());
    }
    else
    {
        return "";
    }
}

static int
GetEditablePartNumber(INode* Node)
{
    ICustAttribContainer* CustAttribs = Node->GetCustAttribContainer();

    if (DoesCustomAttributeExist(CustAttribs, HEMAX_EDITABLE_NODE_PART_NUM_INDEX, HEMAX_EDITABLE_NODE_PART_NUM_NAME))
    {
        HEMAX_IntegerParameterAttrib* PartAttrib = dynamic_cast<HEMAX_IntegerParameterAttrib*>(CustAttribs->GetCustAttrib(HEMAX_EDITABLE_NODE_PART_NUM_INDEX));
        int PartNum;
        PartAttrib->PBlock->GetValue(0, GetCOREInterface()->GetTime(), PartNum, FOREVER);
        return PartNum;
    }
    else
    {
        return -1;
    }
}

void
ClearInstances(HEMAX_GeometryHda& GeometryHda)
{
    for (int i = 0; i < GeometryHda.InstanceClones.size(); i++)
    {
        if (GeometryHda.InstanceClones[i])
        {
            GetCOREInterface()->DeleteNode(GeometryHda.InstanceClones[i]);
        }
    }
    GeometryHda.InstanceClones.clear();
}

void
ClearPackedPrimNodes(HEMAX_GeometryHda& GeometryHda)
{
    for (int p = 0; p < GeometryHda.PackedPrimClones.size(); p++)
    {
        if (GeometryHda.PackedPrimClones[p])
        {
            GetCOREInterface()->DeleteNode(GeometryHda.PackedPrimClones[p]);
            GeometryHda.PackedPrimClones[p] = nullptr;
        }
    }
    GeometryHda.PackedPrimClones.clear();

    int ChildCount = GeometryHda.ContainerNode->NumberOfChildren();

    for (int c = ChildCount - 1; c >= 0; c--)
    {
        INode* Child = GeometryHda.ContainerNode->GetChildNode(c);
        if (Child)
        {
            ICustAttribContainer* CustAttribs = Child->GetCustAttribContainer();
            if (CustAttribs)
            {
                if (DoesCustomAttributeExist(CustAttribs, HEMAX_MAX_GEO_PACKED_PRIM_STAMP_INDEX, HEMAX_MAX_GEO_PACKED_PRIM_STAMP_NAME))
                {
                    GetCOREInterface()->DeleteNode(Child);
                }
            }
        }
    }
}

void
ApplyMaterialsToDisplayGeometry(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_NodeId DisplayNodeId, HEMAX_PartId Part, HEMAX_GeometryPlugin* GeoPlugin)
{
    if (GeoPlugin && GeoPlugin->Mesh)
    {
        HEMAX_Mesh* Mesh = GeoPlugin->Mesh;

        if (Mesh->MaterialPath == "")
        {
            HEMAX_NodeId* MatNodeIds = Mesh->GetMaterialIdsArray();

            if (MatNodeIds)
            {
                if (Mesh->AreMaterialIdsSame)
                {
                    HEMAX_NodeId MaterialNodeId = MatNodeIds[0];
                    if (MaterialNodeId != -1)
                    {
                        StdMat2* NewMaterial = CreateMaterialForPart(GeometryHda, Hda, MaterialNodeId, DisplayNodeId, Part);
                        if (NewMaterial)
                        {
                            GeoPlugin->MaxNode->SetMtl(NewMaterial);
                        }
                    }
                }
                else
                {
                    HEMAX_NodeId* FaceMatIds = Mesh->GetMaterialIdsArray();
                    std::unordered_map<HEMAX_NodeId, HEMAX_MaterialMapping> MultiMap;

                    MultiMtl* MultiMaterial = NewDefaultMultiMtl();
                    MultiMaterial->SetNumSubMtls(Mesh->GetNumMaterials());

                    for (int f = 0; f < Mesh->GetFaceCount(); f++)
                    {
                        HEMAX_NodeId MatNodeId = FaceMatIds[f];
                        auto Search = MultiMap.find(MatNodeId);

                        if (Search == MultiMap.end())
                        {
                            StdMat2* Material = CreateMaterialForPart(GeometryHda, Hda, MatNodeId, DisplayNodeId, Part);
                            if (Material)
                            {
                                HEMAX_MaterialMapping MapEntry;
                                MapEntry.Material = Material;
                                MapEntry.SubNumber = (int)MultiMap.size();
                                MultiMap.insert({ MatNodeId, MapEntry });

                                MultiMaterial->SetSubMtl(MapEntry.SubNumber, Material);
                                GeoPlugin->SetFaceMaterialId(f, MapEntry.SubNumber);
                            }
                        }
                        else
                        {
                            auto Search = MultiMap.find(MatNodeId);
                            if (Search != MultiMap.end())
                            {
                                GeoPlugin->SetFaceMaterialId(f, Search->second.SubNumber);
                            }
                        }
                    }

                    auto NodeSearch = GeometryHda.MaterialMap.find(DisplayNodeId);
                    if (NodeSearch != GeometryHda.MaterialMap.end())
                    {
                        auto PartMats = NodeSearch->second.find(Part);
                        if (PartMats != NodeSearch->second.end())
                        {
                            PartMats->second.push_back(MultiMaterial);
                        }
                    }

                    GeoPlugin->MaxNode->SetMtl(MultiMaterial);
                }
            }
        }
        else
        {
            ApplySceneMtlToGeometryPlugin(GeoPlugin, Mesh);
        }
    }
}

static void
ApplySceneMtlToGeometryPlugin(HEMAX_GeometryPlugin* GeoPlugin, HEMAX_Mesh* Mesh)
{
    // First look up the material in the scene
    std::wstring WideMatName(Mesh->MaterialPath.begin(), Mesh->MaterialPath.end());
    MtlBaseLib* SceneMatLib = GetCOREInterface()->GetSceneMtls();
    int MatNum = SceneMatLib->FindMtlByName(WStr(WideMatName.c_str()));

    if (MatNum > -1)
    {
        MtlBase* Mat = (*SceneMatLib)[MatNum];
        Mtl* MtlMat = (Mtl*)Mat;
        GeoPlugin->MaxNode->SetMtl(MtlMat);
    }
    else
    {
        // This means the material wasn't found in the scene,
        // and we will have to try to load it from file
        for (int s = (int)WideMatName.size() - 1; s >= 0; s--)
        {
            if (WideMatName[s] == L':')
            {
                int SplitIndex = s;
                std::wstring MatLibPath = WideMatName.substr(0, SplitIndex);
                std::wstring MatName = WideMatName.substr(SplitIndex + 1, WideMatName.size() - SplitIndex - 1);

                MtlBaseLib CustomMatLib;
                int Result = GetCOREInterface()->LoadMaterialLib(MatLibPath.c_str(), &CustomMatLib);

                if (Result)
                {
                    int CustomMatNum = CustomMatLib.FindMtlByName(WStr(MatName.c_str()));
                    if (CustomMatNum > -1)
                    {
                        MtlBase* Mat = CustomMatLib[CustomMatNum];
                        Mtl* MtlMat = (Mtl*)Mat;
                        GeoPlugin->MaxNode->SetMtl(MtlMat);
                    }
                }
                break;
            }
        }
    }
}

StdMat2*
CreateMaterialForPart(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_NodeId MatNodeId, HEMAX_NodeId DisplayNodeId, HEMAX_PartId Part)
{
    StdMat2* Material = nullptr;

    auto MatNode = Hda.ShopNodes.find(MatNodeId);

    if (MatNode == Hda.ShopNodes.end())
    {
        HEMAX_MaterialNode CustomMaterial(MatNodeId);
        Hda.ShopNodes.insert({ MatNodeId, CustomMaterial });
    }

    MatNode = Hda.ShopNodes.find(MatNodeId);

    if (MatNode != Hda.ShopNodes.end())
    {
        HEMAX_MaterialNode& Mat = MatNode->second;
        Mat.GetMaterialInformation();
        if (Mat.HasTexture)
        {
            if (Mat.RenderTextureToMemory())
            {
                Material = Mat.Build3dsmaxMaterialFromNode();
                if (Material)
                {
                    auto NodeSearch = GeometryHda.MaterialMap.find(DisplayNodeId);
                    if (NodeSearch != GeometryHda.MaterialMap.end())
                    {
                        auto PartMats = NodeSearch->second.find(Part);
                        if (PartMats != NodeSearch->second.end())
                        {
                            PartMats->second.push_back(Material);
                        }
                    }
                    else
                    {
                        std::vector<Mtl*> MaterialList;
                        std::unordered_map<HEMAX_PartId, std::vector<Mtl*>> PartMtlEntry;
                        MaterialList.push_back(Material);
                        PartMtlEntry.insert({ Part, MaterialList });
                        GeometryHda.MaterialMap.insert({ DisplayNodeId, PartMtlEntry });
                    }
                }
            }
        }
        else
        {
            Material = Mat.Build3dsmaxMaterialFromNode();
            if (Material)
            {
                auto NodeSearch = GeometryHda.MaterialMap.find(DisplayNodeId);
                if (NodeSearch != GeometryHda.MaterialMap.end())
                {
                    auto PartMats = NodeSearch->second.find(Part);
                    if (PartMats != NodeSearch->second.end())
                    {
                        PartMats->second.push_back(Material);
                    }
                }
                else
                {
                    std::vector<Mtl*> MaterialList;
                    std::unordered_map<HEMAX_PartId, std::vector<Mtl*>> PartMtlEntry;
                    MaterialList.push_back(Material);
                    PartMtlEntry.insert({ Part, MaterialList });
                    GeometryHda.MaterialMap.insert({ DisplayNodeId, PartMtlEntry });
                }
            }
        }
    }

    return Material;
}

void
UpdateChangedMaterials(HEMAX_GeometryHda& GeometryHda, HEMAX_Hda& Hda, HEMAX_NodeId DisplayNodeId, HEMAX_PartId Part, HEMAX_GeometryPlugin* GeoPlugin)
{
    if (GeoPlugin && GeoPlugin->Mesh)
    {
        HEMAX_Mesh* Mesh = GeoPlugin->Mesh;

        if (Mesh->MaterialPath == "")
        {
            HEMAX_NodeId* MatNodeIds = Mesh->GetMaterialIdsArray();

            if (MatNodeIds)
            {
                if (Mesh->AreMaterialIdsSame)
                {
                    auto MatNode = Hda.ShopNodes.find(MatNodeIds[0]);
                    if (MatNode != Hda.ShopNodes.end())
                    {
                        HEMAX_MaterialNode& Mat = MatNode->second;
                        if (Mat.CheckHasChanged())
                        {
                            Mat.GetMaterialInformation();

                            auto MatSearch = GeometryHda.MaterialMap.find(DisplayNodeId);
                            if (MatSearch != GeometryHda.MaterialMap.end())
                            {
                                auto EntrySearch = MatSearch->second.find(Part);
                                if (EntrySearch != MatSearch->second.end())
                                {
                                    std::vector<Mtl*> Mats = EntrySearch->second;
                                    for (int i = 0; i < Mats.size(); i++)
                                    {
                                        if (Mats[i])
                                        {
                                            Mats[i]->DeleteThis();
                                        }
                                    }
                                    GeometryHda.MaterialMap.erase(DisplayNodeId);
                                }
                            }

                            ApplyMaterialsToDisplayGeometry(GeometryHda, Hda, DisplayNodeId, Part, GeoPlugin);
                            return;
                        }
                    }
                }
                else
                {
                    HEMAX_NodeId* FaceMatIds = GeoPlugin->Mesh->GetMaterialIdsArray();
                    for (int f = 0; f < GeoPlugin->Mesh->GetFaceCount(); f++)
                    {
                        HEMAX_NodeId MatNodeId = FaceMatIds[f];
                        auto MatNode = Hda.ShopNodes.find(MatNodeId);
                        if (MatNode != Hda.ShopNodes.end())
                        {
                            HEMAX_MaterialNode& Mat = MatNode->second;
                            if (Mat.CheckHasChanged())
                            {
                                Mat.GetMaterialInformation();

                                auto MatSearch = GeometryHda.MaterialMap.find(DisplayNodeId);
                                if (MatSearch != GeometryHda.MaterialMap.end())
                                {
                                    auto EntrySearch = MatSearch->second.find(Part);
                                    if (EntrySearch != MatSearch->second.end())
                                    {
                                        std::vector<Mtl*> Mats = EntrySearch->second;
                                        for (int i = 0; i < Mats.size(); i++)
                                        {
                                            if (Mats[i])
                                            {
                                                Mats[i]->DeleteThis();
                                            }
                                        }
                                        GeometryHda.MaterialMap.erase(DisplayNodeId);
                                    }
                                }

                                ApplyMaterialsToDisplayGeometry(GeometryHda, Hda, DisplayNodeId, Part, GeoPlugin);
                                return;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            ApplySceneMtlToGeometryPlugin(GeoPlugin, Mesh);
        }
    }
}

void
BakeGeometryHda(HEMAX_GeometryHda& GeometryHda)
{
    INodeTab NodesToClone;
    NodesToClone.AppendNode(GeometryHda.ContainerNode);

    INodeTab ClonedNodeTab;
    bool CloneResult = GetCOREInterface()->CloneNodes(NodesToClone, Point3(0, 0, 0), false, NODE_COPY, nullptr, &ClonedNodeTab);

    if (CloneResult)
    {
        if (ClonedNodeTab.Count() > 0)
        {
            INode* Parent = ClonedNodeTab[0];

            int ChildCount = GeometryHda.ContainerNode->NumberOfChildren();

            for (int j = 0; j < ChildCount; j++)
            {
                INode* ChildNode = GeometryHda.ContainerNode->GetChildNode(j);
                if (ChildNode)
                {
                    if (!ChildNode->IsHidden())
                    {
                        if (!CheckForCustomAttributeOnNode(ChildNode, HEMAX_EDITABLE_NODE_STAMP_NAME))
                        {
                            Object* ChildNodeObject = ChildNode->GetObjectRef();
                            PolyObject* CopiedChildNodeObject = (PolyObject*)ChildNodeObject->ConvertToType(GetCOREInterface()->GetTime(), polyObjectClassID);
                            Object* CollapsedCopyObject = CopiedChildNodeObject->CollapseObject();

                            if ((void*)ChildNodeObject != (void*)CopiedChildNodeObject && (void*)CopiedChildNodeObject != (void*)CollapsedCopyObject)
                            {
                                delete CopiedChildNodeObject;
                            }

                            if (ChildNodeObject)
                            {
                                INode* CopyNode = GetCOREInterface()->CreateObjectNode(CollapsedCopyObject);
                                if (CopyNode)
                                {
                                    CopyNode->SetName(ChildNode->GetName());
                                    CopyNode->SetMtl(ChildNode->GetMtl());
                                    CopyNode->SetNodeTM(GetCOREInterface()->GetTime(), ChildNode->GetNodeTM(GetCOREInterface()->GetTime()));
                                    Parent->AttachChild(CopyNode);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}