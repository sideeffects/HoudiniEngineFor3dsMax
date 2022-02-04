#include "HEMAX_GeometryHda.h"

#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Input_NURBS.h"
#include "HEMAX_Input_Spline.h"
#include "HEMAX_UserPrefs.h"
#include "HEMAX_Utilities.h"

#include <icustattribcontainer.h>
#include <ilayer.h>
#include <ilayermanager.h>
#include <custattrib.h>

#include <algorithm>
#include <list>

HEMAX_GeometryHda::HEMAX_GeometryHda()
{
    Dummy = nullptr;
    ContainerNode = nullptr;
    CustomAttributes = nullptr;
    Type = HEMAX_GEOMETRY_HDA;
}

void
HEMAX_GeometryHda::Init(HEMAX_Asset& Asset, int AssetIndex)
{
    Hda.Init(Asset, AssetIndex);

    if (Hda.MainNode.Type != HAPI_NODETYPE_NONE)
    {
        InitializeSubnetworks(); 
    }
}

void
HEMAX_GeometryHda::Create(HEMAX_Asset& Asset, int AssetIndex)
{
    Init(Asset, AssetIndex);

    if (Hda.MainNode.Type != HAPI_NODETYPE_NONE)
    {
        CreateNewGeometryHda(Hda);
        InitializeParameterCustomAttributes();
    }
}

void
HEMAX_GeometryHda::Create(INode* ContainerNode, HEMAX_Asset& Asset, int AssetIndex)
{
    Init(Asset, AssetIndex);
    CreateNewGeometryHda(Hda, ContainerNode);
    InitializeParameterCustomAttributes();
}

void
HEMAX_GeometryHda::CreateNewGeometryHda(HEMAX_Hda& Hda)
{
    Dummy = new DummyObject;
    ContainerNode = GetCOREInterface()->CreateObjectNode(Dummy);

    CreateNewGeometryHda(Hda, ContainerNode);
}

void
HEMAX_GeometryHda::CreateNewGeometryHda(HEMAX_Hda& Hda, INode* ContainerNode)
{
    this->ContainerNode = ContainerNode;

    if (ContainerNode)
    {
	std::wstring ContainerNodeLabel = std::wstring(Hda.Name.begin(), Hda.Name.end());
	ContainerNode->SetName(ContainerNodeLabel.c_str());
	InitGeometryCustAttribContainer();
	GenerateBoilerplateGeometryCustomAttributes(Hda);

	if (Hda.HdaType == OBJ_LEVEL_HDA)
	{
	    HEMAX_Object& TopNode = Hda.TopLevelObjectNode;

	    for (auto It = TopNode.SopNodes.begin(); It != TopNode.SopNodes.end(); It++)
	    {
		if (It->second.Parts.size() > 0)
		{
		    CreateDisplayGeometry(Hda, It->second);
		}
	    }

	    if (TopNode.Instances.size() > 0)
	    {
		CreateInstances(Hda);
	    }
	}
	else if (Hda.HdaType == SOP_LEVEL_HDA)
	{
	    HEMAX_DisplayGeoNode& TopNode = Hda.TopLevelSopNode;

	    if (TopNode.Parts.size() > 0)
	    {
		CreateDisplayGeometry(Hda, TopNode);
	    }
	}

	for (int e = 0; e < Hda.EditableNodes.size(); e++)
	{
	    CreateEditableCurves(Hda.EditableNodes[e]);
	}
    }
}

void
HEMAX_GeometryHda::CreateGeometryHdaFromContainerNode()
{
    if (ContainerNode)
    {
	Dummy = (DummyObject*)ContainerNode->GetObjectRef();

	int ChildCount = ContainerNode->NumberOfChildren();
	if (Hda.HdaType == OBJ_LEVEL_HDA)
	{
	    for (int i = 0; i < ChildCount; i++)
	    {
		INode* Child = ContainerNode->GetChildNode(i);
		if (Child)
		{
		    if (IsINodeADisplayGeometry(Child))
		    {
			HEMAX_Part* Part = nullptr;
                        std::string NodeName =
                            HEMAX_Utilities::WideStringToStringUnsafe(
                                Child->GetName());
                        HEMAX_DisplayGeoNode* SopNode =
                            Hda.TopLevelObjectNode.FindSopNodeFromName(NodeName);
			if (SopNode)
			{
			    HAPI_PartId PartId = GetPartIdFromCustomAttributes(Child);
			    if (PartId != -1)
			    {
				Part = SopNode->GetPart(PartId);
			    }
			}

			if (Part)
			{
			    HEMAX_GeometryPlugin* GeoPlugin = dynamic_cast<HEMAX_GeometryPlugin*>(Child->GetObjectRef());
			    LinearShape* Shape = dynamic_cast<LinearShape*>(Child->GetObjectRef());

			    if (GeoPlugin)
			    {
				GeoPlugin->MaxNode = Child;
				GeoPlugin->IsStranded = false;

				Part->SetMeshPlugin(GeoPlugin);
			    }
			    else if (Shape)
			    {
				Part->SetCurvePlugin(Shape);
			    }
			    else if (Child->GetObjectRef()->CanConvertToType(EDITABLE_SURF_CLASS_ID))
			    {
				Part->SetCurvePlugin(Child);
			    }
			}
		    }
		    else if (IsINodeAnEditableNode(Child))
		    {
			std::string EditableNodeName = GetEditableNodeName(Child);
			int PartNum = GetEditablePartNumber(Child);
			bool Success;
			HEMAX_EditableNode EditableNode = Hda.FindEditableNodeFromName(EditableNodeName, Success);
			if (Success)
			{
			    HEMAX_EditableCurve EditableCurve;
			    EditableCurve.PushNodeId = EditableNode.GeoInfo.nodeId;
			    EditableCurve.PushPartId = PartNum;
			    EditableCurve.Node = Child;
			    EditableCurve.Dirty = true;
			    HEMAX_SessionManager::GetSessionManager().Session->GetCurveInfo(EditableNode.GeoInfo.nodeId, PartNum, &EditableCurve.CurveInfo);
			    EditableCurves.push_back(EditableCurve);
			}
		    }
		}
	    }
	}
	else if (Hda.HdaType == SOP_LEVEL_HDA)
	{
	    for (int i = 0; i < ChildCount; i++)
	    {
		INode* Child = ContainerNode->GetChildNode(i);
		if (Child)
		{
		    if (IsINodeADisplayGeometry(Child))
		    {
			HAPI_PartId PartId = GetPartIdFromCustomAttributes(Child);
			HEMAX_Part* Part = Hda.TopLevelSopNode.GetPart(PartId);

			// Sop level HDAs should only have one relevant child node (and this is the one that we just found)
			if (Part)
			{
			    HEMAX_GeometryPlugin* GeoPlugin = dynamic_cast<HEMAX_GeometryPlugin*>(Child->GetObjectRef());
			    LinearShape* Shape = dynamic_cast<LinearShape*>(Child->GetObjectRef());

			    if (GeoPlugin)
			    {
				GeoPlugin->MaxNode = Child;
				GeoPlugin->IsStranded = false;
				
				Part->SetMeshPlugin(GeoPlugin);
			    }
			    else if (Shape)
			    {
				Part->SetCurvePlugin(Shape);		
			    }
			    else if (Child->GetObjectRef()->CanConvertToType(EDITABLE_SURF_CLASS_ID))
			    {
				Part->SetCurvePlugin(Child);
			    }
			}
		    }
		    else if (IsINodeAnEditableNode(Child))
		    {
			std::string EditableNodeName = GetEditableNodeName(Child);
			int PartNum = GetEditablePartNumber(Child);
			bool Success;
			HEMAX_EditableNode EditableNode = Hda.FindEditableNodeFromName(EditableNodeName, Success);
			if (Success)
			{
			    HEMAX_EditableCurve EditableCurve;
			    EditableCurve.PushNodeId = EditableNode.GeoInfo.nodeId;
			    EditableCurve.PushPartId = PartNum;
			    EditableCurve.Node = Child;
			    EditableCurve.Dirty = true;
			    HEMAX_SessionManager::GetSessionManager().Session->GetCurveInfo(EditableNode.GeoInfo.nodeId, PartNum, &EditableCurve.CurveInfo);
			    EditableCurves.push_back(EditableCurve);
			}
		    }
		}
	    }
	}
    }
}

void
HEMAX_GeometryHda::ClearAnyInstances()
{
    ClearInstances();
    ClearPackedPrimNodes();
}

std::vector<ULONG>
HEMAX_GeometryHda::GetListOfAllChildINodes()
{
    std::vector<ULONG> Nodes;

    std::vector<HEMAX_DisplayGeoNode*> DisplayNodes = Hda.AllDisplayNodes();

    for (int i = 0; i < DisplayNodes.size(); i++)
    {
	for (int p = 0; p < DisplayNodes[i]->Parts.size(); p++)
	{
	    INode* MaxNode = DisplayNodes[i]->Parts[p].GetINodeOf3dsmaxObject();
	    if (MaxNode)
	    {
		Nodes.push_back(MaxNode->GetHandle());
	    }
	}
    }

    for (int j = 0; j < EditableCurves.size(); j++)
    {
	if (EditableCurves[j].Node)
	{
	    Nodes.push_back(EditableCurves[j].Node->GetHandle());
	}
    }

    return Nodes;
}

void
HEMAX_GeometryHda::Update3dsmaxHda()
{
    Hda.Update();
    UpdateGeometryHda();
}

void
HEMAX_GeometryHda::UpdateGeometryHda()
{
    // Clear any packed primitive nodes
    ClearPackedPrimNodes();

    if (ContainerNode)
    {
	if (Hda.HdaType == OBJ_LEVEL_HDA)
	{
	    std::vector<HEMAX_DisplayGeoNode*> DisplayNodes = Hda.AllDisplayNodes();

	    for (int i = 0; i < DisplayNodes.size(); i++)
	    {
		for (int p = 0; p < DisplayNodes[i]->Parts.size(); p++)
		{
		    HEMAX_Part& Part = DisplayNodes[i]->Parts[p];

		    if (Part.Info.type == HEMAX_PARTTYPE_MESH)
		    {
			HEMAX_GeometryPlugin* GeoPlugin = Part.GetMeshPlugin();
			if (GeoPlugin)
			{
			    Part.Build3dsmaxObject();
                            
                            if (GeoPlugin->Mesh &&
                                GeoPlugin->Mesh->MaterialPath != "")
                            {
                                ApplySceneMtlToGeometryPlugin(GeoPlugin);
                            }
                            else
                            {
                                UpdateMaterials(Hda,
                                                DisplayNodes[i]->Info.nodeId,
                                                Part,
                                                GeoPlugin);
                            }

                            bool UseUniqueName = false;
                            HEMAX_UserPrefs::Get().GetBoolSetting(
                                        HEMAX_SETTING_NODE_NAMES_UNIQUE,
                                        UseUniqueName);
                            SetPluginNodeName(GeoPlugin->MaxNode,
                                              *DisplayNodes[i],
                                              Part,
                                              UseUniqueName);
			}
			else
			{
			    CreateMeshPluginPart(Hda, *(DisplayNodes[i]), DisplayNodes[i]->Parts[p]);
			}
		    }
		    else if (Part.Info.type == HEMAX_PARTTYPE_CURVE)
		    {
			CreateCurvePart(*(DisplayNodes[i]), DisplayNodes[i]->Parts[p]);
		    }
		    else if (Part.Info.type == HEMAX_PARTTYPE_INVALID)
		    {
			INode* PartINode = nullptr;

			HEMAX_PartType PreviousPartType = Part.GetPartType();
			if (PreviousPartType == HEMAX_PARTTYPE_MESH)
			{
			    HEMAX_GeometryPlugin* GeomPlugin = Part.GetMeshPlugin();
			    if (GeomPlugin)
			    {
				PartINode = GeomPlugin->MaxNode;
			    }
			}
			else if (PreviousPartType == HEMAX_PARTTYPE_CURVE)
			{
			    HEMAX_CurvePlugin* CurvePlugin = Part.GetCurvePlugin();
			    if (CurvePlugin)
			    {
				PartINode = CurvePlugin->GetINode();
			    }
			}

			if (PartINode)
			{
			    GetCOREInterface()->DeleteNode(PartINode);
			    Part.ClearAnyGeometryRefs();
			}
		    }

		    Part.UpdatePartType();
		}

		for (int p = 0; p < DisplayNodes[i]->Parts.size(); p++)
		{
		    if (DisplayNodes[i]->Parts[p].Info.type == HEMAX_PARTTYPE_INSTANCER)
		    {
			CreatePackedPrimitives(DisplayNodes[i]->Parts[p], *(DisplayNodes[i])); 
		    }
		}
	    }

	    HEMAX_Object& TopNode = Hda.TopLevelObjectNode;

	    if (TopNode.Instances.size() > 0)
	    {
		UpdateInstances(Hda);
	    }
	}
	else if (Hda.HdaType == SOP_LEVEL_HDA)
	{
	    HEMAX_DisplayGeoNode& TopNode = Hda.TopLevelSopNode;

	    for (int p = 0; p < TopNode.Parts.size(); p++)
	    {
		if (TopNode.Parts[p].Info.type == HEMAX_PARTTYPE_MESH)
		{
		    HEMAX_GeometryPlugin* GeoPlugin = TopNode.Parts[p].GetMeshPlugin();

		    if (GeoPlugin)
		    {
			TopNode.Parts[p].Build3dsmaxObject();

                        if (GeoPlugin->Mesh &&
                            GeoPlugin->Mesh->MaterialPath != "")
                        {
                            ApplySceneMtlToGeometryPlugin(GeoPlugin);
                        }
                        else
                        {
                            UpdateMaterials(Hda,
                                            TopNode.Info.nodeId,
                                            TopNode.Parts[p],
                                            GeoPlugin);
                        }
		    }		    
		    else
		    {
			CreateMeshPluginPart(Hda, TopNode, TopNode.Parts[p]);	
		    }
		}
		else if (TopNode.Parts[p].Info.type == HEMAX_PARTTYPE_CURVE)
		{
		    CreateCurvePart(TopNode, TopNode.Parts[p]);
		}
		else if (TopNode.Parts[p].Info.type == HEMAX_PARTTYPE_INVALID)
		{
		    INode* PartINode = nullptr;

		    HEMAX_PartType PreviousPartType = TopNode.Parts[p].GetPartType();
		    if (PreviousPartType == HEMAX_PARTTYPE_MESH)
		    {
			HEMAX_GeometryPlugin* GeomPlugin = TopNode.Parts[p].GetMeshPlugin();
			if (GeomPlugin)
			{
			    PartINode = GeomPlugin->MaxNode;
			}
		    }
		    else if (PreviousPartType == HEMAX_PARTTYPE_CURVE)
		    {
			HEMAX_CurvePlugin* CurvePlugin = TopNode.Parts[p].GetCurvePlugin();
			if (CurvePlugin)
			{
			    PartINode = CurvePlugin->GetINode();
			}
		    }

		    if (PartINode)
		    {
			GetCOREInterface()->DeleteNode(PartINode);
			TopNode.Parts[p].ClearAnyGeometryRefs();
		    }
		}

		TopNode.Parts[p].UpdatePartType();
	    }

	    for (int p = 0; p < TopNode.Parts.size(); p++)
	    {
		if (TopNode.Parts[p].Info.type == HEMAX_PARTTYPE_INSTANCER)
		{
		    CreatePackedPrimitives(TopNode.Parts[p], TopNode);
		}
	    }
	}
    }
}

void
HEMAX_GeometryHda::PushEditableNodeChanges(HEMAX_EditableCurve EditableCurve)
{
    for (int i = 0; i < EditableCurves.size(); i++)
    {
	if (EditableCurves[i].Node && (EditableCurves[i].Node == EditableCurve.Node))
	{
	    switch (EditableCurve.CurveInfo.curveType)
	    {
		case (HEMAX_CURVETYPE_LINEAR):
		{
		    HEMAX_Node Node(EditableCurves[i].PushNodeId, HAPI_NODETYPE_SOP);
		    HEMAX_Input_Spline LinearInput(&Node, EditableCurves[i].Node->GetHandle());
		    EditableCurves[i].Dirty = false;
		} break;
		case (HEMAX_CURVETYPE_NURBS):
		{
		    HEMAX_Node Node(EditableCurves[i].PushNodeId, HAPI_NODETYPE_SOP);
		    HEMAX_Input_NURBS NURBSInput(&Node, EditableCurves[i].Node->GetHandle());
		    EditableCurves[i].Dirty = false;
		} break;
		default:
		{

		} break;
	    }
	}
    }
}

std::vector<INode*>
HEMAX_GeometryHda::BakeGeometryHda(bool BakeDummyObj)
{
    std::vector<INode*> BakeResults;
    INode* BakedParent = nullptr;

    if (BakeDummyObj)
    {
        INodeTab NodesToClone;
        NodesToClone.AppendNode(ContainerNode);

        INodeTab ClonedNodeTab;
        bool CloneResult = GetCOREInterface()->CloneNodes(NodesToClone,
                                                          Point3(0, 0, 0),
                                                          false,
                                                          NODE_COPY,
                                                          nullptr,
                                                          &ClonedNodeTab);

        if (CloneResult)
        {
            BakedParent = ClonedNodeTab[0];
            BakeResults.push_back(BakedParent);

            std::string ContainerNodeBakedName = GetDetailAttributeOverride(
                HEMAX_BAKE_PARENT_NODE_NAME_ATTRIBUTE);
            if (!ContainerNodeBakedName.empty())
            {
                std::wstring WContainerNodeBakedName(
                    ContainerNodeBakedName.begin(),
                    ContainerNodeBakedName.end());
                BakedParent->SetName(WContainerNodeBakedName.c_str());
            }
        }
    }

    bool CreateLayer = false;
    HEMAX_UserPrefs::Get().GetBoolSetting(HEMAX_SETTING_BAKE_CREATE_LAYER,
        CreateLayer);
    ILayer* Layer = nullptr;

    if (CreateLayer)
    {
        // Check for attribute override
        std::string LayerName = GetDetailAttributeOverride(
            HEMAX_BAKE_LAYER_NAME_ATTRIBUTE);
        if (LayerName.empty())
        {
            if (BakeDummyObj && BakedParent)
            {
                LayerName = HEMAX_Utilities::WideStringToStringUnsafe(
                                                BakedParent->GetName());
            }
            else
            {
                LayerName = HEMAX_Utilities::WideStringToStringUnsafe(
                                                ContainerNode->GetName());
            }
        }

        ILayerManager* LM = GetCOREInterface13()->GetLayerManager();
        std::wstring WLayerName(LayerName.begin(), LayerName.end());
        
        Layer = LM->GetLayer(WLayerName.c_str());

        if (!Layer)
        {
            Layer = LM->CreateLayer();
            Layer->SetName(WLayerName.c_str());
        }
    }

    int ChildCount = ContainerNode->NumberOfChildren();
    std::vector<INode*> SkippedNodes;

    for (int j = 0; j < ChildCount; j++)
    {
        INode* ChildNode = ContainerNode->GetChildNode(j);
        if (ChildNode)
        {
            bool IsEditableNode = CheckForCustomAttributeOnNode(
                                        ChildNode,
                                        HEMAX_EDITABLE_NODE_STAMP_NAME);

            bool ShouldSkip = ChildNode->IsHidden()
                || IsEditableNode
                || IsInstanceSource(ChildNode)
                || IsPackedPrimSource(ChildNode)
                || IsInstance(ChildNode)
                || IsPackedPrim(ChildNode);
                                    
            if (ShouldSkip)
            {
                SkippedNodes.push_back(ChildNode);
                continue;
            }

            Object* ChildNodeObj = ChildNode->GetObjectRef();
            Object* CopiedChildNodeObj = nullptr;

            HEMAX_GeometryPlugin* Geometry =
                dynamic_cast<HEMAX_GeometryPlugin*>(ChildNodeObj);

            TimeValue CurrentTime = GetCOREInterface()->GetTime();

            INode* BakedNode = nullptr;

            if (Geometry)
            {
                CopiedChildNodeObj = ChildNodeObj->ConvertToType(
                                                        CurrentTime,
                                                        polyObjectClassID);

                Object* CollapsedChildNodeObj =
                    CopiedChildNodeObj->CollapseObject();

                if ((void*)ChildNodeObj != (void*)CopiedChildNodeObj &&
                    (void*)CopiedChildNodeObj != (void*)CollapsedChildNodeObj)
                {
                    delete CopiedChildNodeObj;
                }

                if (CollapsedChildNodeObj)
                {
                    BakedNode = GetCOREInterface()->CreateObjectNode(
                                                        CollapsedChildNodeObj);
                }
            }
            else
            {
                INodeTab NodeToBake;
                NodeToBake.AppendNode(ChildNode);
                INodeTab BakedNodeTab;

                bool CloneResult = GetCOREInterface()->CloneNodes(
                                        NodeToBake,
                                        Point3(0, 0, 0),
                                        false,
                                        NODE_COPY,
                                        nullptr,
                                        &BakedNodeTab);

                if (CloneResult)
                {
                    BakedNode = BakedNodeTab[0];
                }
            }

            if (BakedNode)
            {
                BakedNode->SetName(ChildNode->GetName());
                BakedNode->SetMtl(ChildNode->GetMtl());
                BakedNode->SetNodeTM(CurrentTime,
                                     ChildNode->GetNodeTM(CurrentTime));
                BakeResults.push_back(BakedNode);

                if (BakedParent && BakeDummyObj)
                {
                    BakedParent->AttachChild(BakedNode);
                }
                else
                {
                    GetCOREInterface()->GetRootNode()->AttachChild(BakedNode);
                }
            }
        }
    }

    std::unordered_map<INode*, INode*> InstanceSourceToBakedInstanceSource;

    // Do one pass to create the instance sources (since they might not be in
    // proper order)
    for (int c = 0; c < SkippedNodes.size(); ++c)
    {
        if (IsInstanceSource(SkippedNodes[c])
            || IsPackedPrimSource(SkippedNodes[c]))
        {
            Object* SourceObj = SkippedNodes[c]->GetObjectRef();
            Object* CopiedObj = nullptr;

            HEMAX_GeometryPlugin* Geometry =
                dynamic_cast<HEMAX_GeometryPlugin*>(SourceObj);

            if (Geometry)
            {
                INode* BakedNode = nullptr;
                TimeValue CurrentTime = GetCOREInterface()->GetTime(); 

                CopiedObj = SourceObj->ConvertToType(CurrentTime,
                    polyObjectClassID);
                Object* CollapsedObj = CopiedObj->CollapseObject();
                if ((void*)SourceObj != (void*)CopiedObj &&
                    (void*)CopiedObj != (void*)CollapsedObj)
                {
                    delete CopiedObj;
                    CopiedObj = nullptr;
                }

                if (CollapsedObj)
                {
                    INode* BakedNode =
                        GetCOREInterface()->CreateObjectNode(CollapsedObj);
                    BakedNode->SetName(SkippedNodes[c]->GetName());
                    BakedNode->SetMtl(SkippedNodes[c]->GetMtl());
                    BakedNode->SetNodeTM(CurrentTime,
                        SkippedNodes[c]->GetNodeTM(CurrentTime));
                    BakeResults.push_back(BakedNode);

                    if (BakedParent && BakeDummyObj)
                        BakedParent->AttachChild(BakedNode);
                    else
                        GetCOREInterface()->GetRootNode()->AttachChild(BakedNode);

                    BakedNode->Hide(true);

                    InstanceSourceToBakedInstanceSource.insert({
                        SkippedNodes[c], BakedNode});
                }
            }
        }
    }

    // Do another pass to create the actual instances
    for (int c = 0; c < SkippedNodes.size(); ++c)
    {
        if (IsInstance(SkippedNodes[c])
            || IsPackedPrim(SkippedNodes[c]))
        {
            INode* BakedSource = GetBakedInstanceSource(SkippedNodes[c],
                InstanceSourceToBakedInstanceSource);
            if (BakedSource)
            {
                INodeTab NodeToBake;
                NodeToBake.AppendNode(BakedSource);
                INodeTab BakedNodeTab;

                bool Success = GetCOREInterface()->CloneNodes(
                    NodeToBake,
                    Point3(0, 0, 0),
                    false,
                    NODE_INSTANCE,
                    nullptr,
                    &BakedNodeTab);

                if (Success)
                {
                    INode* BakedInstance = BakedNodeTab[0];
                    BakedInstance->SetName(SkippedNodes[c]->GetName());
                    BakedInstance->SetMtl(SkippedNodes[c]->GetMtl());
                    TimeValue CurrentTime = GetCOREInterface()->GetTime();
                    BakedInstance->SetNodeTM(
                        CurrentTime,
                        SkippedNodes[c]->GetNodeTM(CurrentTime));
                    BakeResults.push_back(BakedInstance);

                    if (BakedParent && BakeDummyObj)
                        BakedParent->AttachChild(BakedInstance);
                    else
                        GetCOREInterface()->GetRootNode()->AttachChild(BakedInstance);

                    BakedInstance->Hide(false);
                }
            }
        }
    }

    if (Layer)
    {
        for (int i = 0; i < BakeResults.size(); i++)
        {
            Layer->AddToLayer(BakeResults[i]);
        }
    }

    return BakeResults;
}

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
HEMAX_GeometryHda::SetCustomAttributeContainer(ICustAttribContainer* Container)
{
    CustomAttributes = Container;

    MaxStampIndex = HEMAX_MAX_HOUDINI_MAX_INDEX;

    for (int i = HEMAX_MAX_GEO_MAX_INDEX; i < CustomAttributes->GetNumCustAttribs(); i++)
    {
        HEMAX_ParameterAttrib* CustAttrib =
            dynamic_cast<HEMAX_ParameterAttrib*>(CustomAttributes->GetCustAttrib(i));

        if (CustAttrib)
        {
            CustomAttributeMap.insert(
                { CustAttrib->GetParameterName(), CustAttrib });
        }
    }
}

void
HEMAX_GeometryHda::SetContainerNode(INode* Node)
{
    ContainerNode = Node;
}

bool
HEMAX_GeometryHda::IsINodeADisplayGeometry(INode* Node)
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

bool
HEMAX_GeometryHda::IsINodeAnEditableNode(INode* Node)
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

HAPI_PartId
HEMAX_GeometryHda::GetPartIdFromCustomAttributes(INode* Node)
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
HEMAX_GeometryHda::CreateDisplayGeometry(HEMAX_Hda& Hda, HEMAX_DisplayGeoNode& DisplayNode)
{
    // First pass is to create any Mesh geometries
    for (int p = 0; p < DisplayNode.Parts.size(); p++)
    {
	if (DisplayNode.Parts[p].Info.type == HEMAX_PARTTYPE_MESH)
	{
	    CreateMeshPluginPart(Hda, DisplayNode, DisplayNode.Parts[p]);
	}
	else if (DisplayNode.Parts[p].Info.type == HEMAX_PARTTYPE_CURVE)
	{
	    CreateCurvePart(DisplayNode, DisplayNode.Parts[p]);
	}
    }

    // Second pass is for instancing (packed primitives)
    for (int p = 0; p < DisplayNode.Parts.size(); p++)
    {
	if (DisplayNode.Parts[p].Info.type == HEMAX_PARTTYPE_INSTANCER)
	{
	    CreatePackedPrimitives(DisplayNode.Parts[p], DisplayNode);
	}
    }
}

void
HEMAX_GeometryHda::CreateMeshPluginPart(HEMAX_Hda& Hda, HEMAX_DisplayGeoNode& DisplayNode, HEMAX_Part& Part)
{
    HEMAX_GeometryPlugin* NewPlugin = new HEMAX_GeometryPlugin(
                                        Hda.HasTimeDependentNodes());
    NewPlugin->IsStranded = false;
    INode* PluginNode = GetCOREInterface()->CreateObjectNode(NewPlugin);
    NewPlugin->MaxNode = PluginNode;
    
    bool UseUniqueName = false;
    HEMAX_UserPrefs::Get().GetBoolSetting(HEMAX_SETTING_NODE_NAMES_UNIQUE,
        UseUniqueName);
    SetPluginNodeName(PluginNode, DisplayNode, Part, UseUniqueName);

    ContainerNode->AttachChild(PluginNode);

    Part.SetMeshPlugin(NewPlugin);
    Part.Build3dsmaxObject();

    InitGeometryPluginCustAttribContainer(PluginNode);
    GenerateBoilerplateGeometryPluginCustomAttributes(PluginNode, Part.Info.id);

    if (NewPlugin->Mesh && NewPlugin->Mesh->MaterialPath != "")
    {
        ApplySceneMtlToGeometryPlugin(NewPlugin); 
    }
    else
    {
        AssignMaterials(Hda, DisplayNode.Info.nodeId, Part, NewPlugin);
    }

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
HEMAX_GeometryHda::CreateCurvePart(HEMAX_DisplayGeoNode& DisplayNode, HEMAX_Part& Part)
{
    if (Part.Build3dsmaxObject())
    {
	INode* CurveNode = Part.GetINodeOf3dsmaxObject();
	ContainerNode->AttachChild(CurveNode);

	std::wstring WCurveLabel = std::wstring(DisplayNode.Name.begin(),
						DisplayNode.Name.end());

        bool UseUniqueName = false;
        HEMAX_UserPrefs::Get().GetBoolSetting(HEMAX_SETTING_NODE_NAMES_UNIQUE,
            UseUniqueName);
        SetPluginNodeName(CurveNode, DisplayNode, Part, UseUniqueName);

	InitGeometryPluginCustAttribContainer(CurveNode);
	GenerateBoilerplateGeometryPluginCustomAttributes(CurveNode,
							  Part.Info.id);

	if (!DisplayNode.IsVisible || Part.Info.isInstanced)
	{
	    CurveNode->Hide(true);
	}
	else
	{
	    CurveNode->Hide(false);
	}
    }
}

void
HEMAX_GeometryHda::CreateInstances(HEMAX_Hda& Hda)
{
    HEMAX_Object& ObjectNode = Hda.TopLevelObjectNode;

    bool UseInstanceSourceName = true;
    HEMAX_UserPrefs::Get().GetBoolSetting(
        HEMAX_SETTING_NODE_INSTANCE_NAME_ORIGINAL, UseInstanceSourceName);

    for (auto InstIt = ObjectNode.Instances.begin(); InstIt != ObjectNode.Instances.end(); InstIt++)
    {
	if (InstIt->second.HasMultipleInstancees)
	{
	    for (int s = 0; s < InstIt->second.InstanceNodeIds.size(); s++)
	    {
		// Different display geometries
		HAPI_GeoInfo DisplayGeoInfo;
		HEMAX_SessionManager::GetSessionManager().Session->GetDisplayGeoInfo(
                        InstIt->second.InstanceNodeIds[s], &DisplayGeoInfo);
		HAPI_NodeId ObjToInstance = InstIt->second.InstanceNodeIds[s];

		auto ObjSearch = ObjectNode.SopNodes.find(ObjToInstance);

		if (ObjSearch != ObjectNode.SopNodes.end())
		{
		    INodeTab CloneableNodeTab;

		    for (int i = 0; i < ObjSearch->second.Parts.size(); i++)
		    {
			INode* CloneableSource =
                            ObjSearch->second.Parts[i].GetINodeOf3dsmaxObject();

			if (CloneableSource)
			{
                            InstanceSources.insert(CloneableSource);
			    CloneableNodeTab.AppendNode(CloneableSource);
			}
		    }

		    INodeTab ClonedNodeTab, ClonedNodeSourceTab;
		    bool CloneResult = GetCOREInterface()->CloneNodes(
                        CloneableNodeTab, Point3(0, 0, 0), true, NODE_INSTANCE,
                        &ClonedNodeSourceTab, &ClonedNodeTab);

		    if (CloneResult)
		    {
			INode* ParentNode = ContainerNode;
			for (int c = 0; c < ClonedNodeTab.Count(); c++)
			{
			    ClonedNodeTab[c]->SetNodeTM(
                                GetCOREInterface()->GetTime(),
                                ParentNode->GetNodeTM(GetCOREInterface()->GetTime()));
			    HEMAX_Utilities::ApplyTransformToINode(ClonedNodeTab[c],
                                InstIt->second.InstanceTransforms[s]);
			    ClonedNodeTab[c]->Hide(false);
			    StampInstanceNode(ClonedNodeTab[c]);
			    InstanceClones.push_back(ClonedNodeTab[c]);
                            InstanceCloneToSourceMap.insert(
                                {ClonedNodeTab[c], ClonedNodeSourceTab[c]});

                            if (UseInstanceSourceName)
                            {
                                ClonedNodeTab[c]->SetName(
                                    CloneableNodeTab[c]->GetName());
                            }
			}
		    }
		}
	    }
	}
	else
	{
	    auto ObjSearch = ObjectNode.SopNodes.find(InstIt->second.InstanceeId);
	    if (ObjSearch != ObjectNode.SopNodes.end())
	    {
		for (int s = 0; s < InstIt->second.InstanceTransforms.size(); s++)
		{
		    INodeTab CloneableNodeTab;

		    for (int i = 0; i < ObjSearch->second.Parts.size(); i++)
		    {
			INode* CloneableSource =
                            ObjSearch->second.Parts[i].GetINodeOf3dsmaxObject();

			if (CloneableSource)
			{
                            InstanceSources.insert(CloneableSource);
			    CloneableNodeTab.AppendNode(CloneableSource);
			}
		    }

		    INodeTab ClonedNodeTab, ClonedNodeSourceTab;
		    bool CloneResult = GetCOREInterface()->CloneNodes(
                        CloneableNodeTab,
		        Point3(0, 0, 0),
			true,
			NODE_INSTANCE,
			&ClonedNodeSourceTab,
		        &ClonedNodeTab);

		    if (CloneResult)
		    {
			INode* ParentNode = ContainerNode;
			for (int c = 0; c < ClonedNodeTab.Count(); c++)
			{
			    TimeValue CurrentTime = GetCOREInterface()->GetTime();
			    ClonedNodeTab[c]->SetNodeTM(CurrentTime,
							ParentNode->GetNodeTM(CurrentTime));
			    HEMAX_Utilities::ApplyTransformToINode(ClonedNodeTab[c],
								   InstIt->second.InstanceTransforms[s]);
			    ClonedNodeTab[c]->Hide(false);
			    StampInstanceNode(ClonedNodeTab[c]);
			    InstanceClones.push_back(ClonedNodeTab[c]);
                            InstanceCloneToSourceMap.insert(
                                {ClonedNodeTab[c], ClonedNodeSourceTab[c]});

                            if (UseInstanceSourceName)
                            {
                                ClonedNodeTab[c]->SetName(
                                    CloneableNodeTab[c]->GetName());
                            }
			}
		    }
		}
	    }
	}
    }
}

void
HEMAX_GeometryHda::CreatePackedPrimitives(HEMAX_Part& Part, HEMAX_DisplayGeoNode& DisplayNode)
{
    HEMAX_PackedPrimitiveInstanceInfo& InstanceInfo = Part.PackedPrimInfo;

    std::vector<std::wstring> NodeNames;
    GetInstancedPluginNodeNames(DisplayNode, Part, NodeNames);

    bool UseInstanceSourceName = true;
    HEMAX_UserPrefs::Get().GetBoolSetting(
        HEMAX_SETTING_NODE_INSTANCE_NAME_ORIGINAL, UseInstanceSourceName);

    for (int i = 0; i < InstanceInfo.InstanceCount; i++)
    {
	for (int t = 0; t < InstanceInfo.InstancedPartCount; t++)
	{
	    HAPI_PartId SourcePartId = InstanceInfo.InstancedPartIds[t];
	    HEMAX_Part* InstancedPart = DisplayNode.GetPart(SourcePartId);

	    INode* SourceNode = InstancedPart->GetINodeOf3dsmaxObject();
	    if (SourceNode)
	    {
                // Single name for all instances
                if (NodeNames.size() == 1)
                {
                    SourceNode->SetName(NodeNames[0].c_str());
                }

		INodeTab CloneableNodeTab;
		CloneableNodeTab.AppendNode(SourceNode);
                PackedPrimSources.insert(SourceNode);
		
		INodeTab ClonedNodeTab, ClonedNodeSourceTab;
		bool CloneResult = GetCOREInterface()->CloneNodes(
                    CloneableNodeTab,
		    Point3(0, 0, 0),
		    true,
		    NODE_INSTANCE,
		    &ClonedNodeSourceTab,
		    &ClonedNodeTab);

		if (CloneResult)
		{
		    INode* ClonedNode = ClonedNodeTab[0];
                    INode* ClonedNodeSource = ClonedNodeSourceTab[0];
		    INode* ParentNode = ContainerNode;
		    TimeValue CurTime = GetCOREInterface()->GetTime();
		    ClonedNode->SetNodeTM(CurTime, ParentNode->GetNodeTM(CurTime));
		    HEMAX_Utilities::ApplyTransformToINode(
                            ClonedNode,
			    InstanceInfo.InstancedTransforms[i]);
		    ClonedNode->Hide(false);
		    StampPackedPrimitiveNode(ClonedNode);
		    PackedPrimClones.push_back(ClonedNode);
                    PackedPrimCloneToSourceMap.insert(
                        {ClonedNode, ClonedNodeSource});

                    // Node name specified by primitive attribute
                    if (NodeNames.size() > 1 && i < NodeNames.size())
                    {
                        ClonedNode->SetName(NodeNames[i].c_str());
                    }
                    else if (UseInstanceSourceName)
                    {
                        ClonedNode->SetName(SourceNode->GetName());
                    }
		}
	    }
	}
    }
}

void
HEMAX_GeometryHda::CreateEditableCurves(HEMAX_EditableNode& EditableNode)
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

		if (ContainerNode)
		{
		    StampEditableNode(EditableCurve.Node, NodeName, p);
		    ContainerNode->AttachChild(EditableCurve.Node, 0);
		    EditableCurves.push_back(EditableCurve);
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

void
HEMAX_GeometryHda::InitGeometryCustAttribContainer()
{
    CustomAttributes = ContainerNode->GetCustAttribContainer();

    if (!CustomAttributes)
    {
	ContainerNode->AllocCustAttribContainer();
	CustomAttributes = ContainerNode->GetCustAttribContainer();
    }
}

void
HEMAX_GeometryHda::GenerateBoilerplateGeometryCustomAttributes(HEMAX_Hda& Hda)
{
    // Generate HEMAX boilerplate custom attributes. These are necessary for reloading the scene
    HEMAX_StringParameterAttrib* MaxHoudiniAssetStamp = new HEMAX_StringParameterAttrib;
    MaxHoudiniAssetStamp->SetParameterName(std::string(HEMAX_MAX_HOUDINI_STAMP_NAME));
    MaxHoudiniAssetStamp->PBlock->SetValue(0, 0, L"TRUE");
    CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_STAMP_INDEX, MaxHoudiniAssetStamp);

    HEMAX_StringParameterAttrib* MaxHoudiniAssetPath = new HEMAX_StringParameterAttrib;
    MaxHoudiniAssetPath->SetParameterName(std::string(HEMAX_MAX_HOUDINI_ASSET_PATH_NAME));
    std::wstring WideAssetPath(Hda.HdaAsset.Path.begin(), Hda.HdaAsset.Path.end());
    MaxHoudiniAssetPath->PBlock->SetValue(0, 0, WideAssetPath.c_str());
    CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_ASSET_PATH_INDEX, MaxHoudiniAssetPath);

    HEMAX_IntegerParameterAttrib* MaxHoudiniAssetLibIndex = new HEMAX_IntegerParameterAttrib;
    MaxHoudiniAssetLibIndex->SetParameterName(std::string(HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_NAME));
    MaxHoudiniAssetLibIndex->PBlock->SetValue(0, 0, Hda.HdaAssetIndex);
    CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_INDEX, MaxHoudiniAssetLibIndex);

    HEMAX_ToggleParameterAttrib* MaxHoudiniConvertOnSave = new HEMAX_ToggleParameterAttrib;
    MaxHoudiniConvertOnSave->SetParameterName(std::string(HEMAX_MAX_HOUDINI_SAVE_CONVERSION_NAME));
    MaxHoudiniConvertOnSave->PBlock->SetValue(0, 0, 0);
    CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_SAVE_CONVERSION_INDEX, MaxHoudiniConvertOnSave);

    HEMAX_ToggleParameterAttrib* MaxHoudiniPushTransform = new HEMAX_ToggleParameterAttrib;
    MaxHoudiniPushTransform->SetParameterName(std::string(HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_NAME));
    MaxHoudiniPushTransform->PBlock->SetValue(0, 0, 0);
    CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_INDEX, MaxHoudiniPushTransform);

    HEMAX_ToggleParameterAttrib* MaxHoudiniApplyHAPITransform = new HEMAX_ToggleParameterAttrib;
    MaxHoudiniApplyHAPITransform->SetParameterName(std::string(HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_NAME));
    MaxHoudiniApplyHAPITransform->PBlock->SetValue(0, 0, 0);
    CustomAttributes->InsertCustAttrib(HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_INDEX, MaxHoudiniApplyHAPITransform);

    // Make custom attributes for the sub-network inputs
    for (int s = 0; s < Hda.MainNode.Info.inputCount; ++s)
    {
	HEMAX_NodeParameterAttrib* InputCustAttrib = new HEMAX_NodeParameterAttrib;
	InputCustAttrib->SetParameterName("subnetwork_" + std::to_string(s));
	InputCustAttrib->CreateMaxHoudiniAssetLink(ContainerNode, HEMAX_INPUT_SUBNETWORK, s);
	CustomAttributes->AppendCustAttrib(InputCustAttrib);
	CustomAttributeMap.insert({ "subnetwork_" + std::to_string(s), InputCustAttrib });
    }

    MaxStampIndex = HEMAX_MAX_HOUDINI_MAX_INDEX;
}

void
HEMAX_GeometryHda::InitGeometryPluginCustAttribContainer(INode* PluginNode)
{
    ICustAttribContainer* Container = PluginNode->GetCustAttribContainer();

    if (!Container)
    {
	PluginNode->AllocCustAttribContainer();
    }
}

void
HEMAX_GeometryHda::GenerateBoilerplateGeometryPluginCustomAttributes(INode* PluginNode, HAPI_PartId Part)
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

void
HEMAX_GeometryHda::UpdateInstances(HEMAX_Hda& Hda)
{
    ClearInstances();

    int ChildCount = ContainerNode->NumberOfChildren();

    for (int c = ChildCount - 1; c >= 0; c--)
    {
	INode* Child = ContainerNode->GetChildNode(c);
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

    CreateInstances(Hda);
}

void
HEMAX_GeometryHda::StampInstanceNode(INode* Node)
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
HEMAX_GeometryHda::StampPackedPrimitiveNode(INode* Node)
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

void
HEMAX_GeometryHda::StampEditableNode(INode* Node, std::string EditableNodeName, HAPI_PartId PartNum)
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

std::string
HEMAX_GeometryHda::GetEditableNodeName(INode* Node)
{
    ICustAttribContainer* CustAttribs = Node->GetCustAttribContainer();

    if (DoesCustomAttributeExist(CustAttribs, HEMAX_EDITABLE_NODE_NAME_INDEX, HEMAX_EDITABLE_NODE_NAME_NAME))
    {
	HEMAX_StringParameterAttrib* NameAttrib = dynamic_cast<HEMAX_StringParameterAttrib*>(CustAttribs->GetCustAttrib(HEMAX_EDITABLE_NODE_NAME_INDEX));
	const MCHAR* Name;
	NameAttrib->PBlock->GetValue(0, GetCOREInterface()->GetTime(), Name, FOREVER);
	return HEMAX_Utilities::WideStringToStringUnsafe(Name);
    }
    else
    {
	return "";
    }
}

int
HEMAX_GeometryHda::GetEditablePartNumber(INode* Node)
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
HEMAX_GeometryHda::ClearInstances()
{
    for (int i = 0; i < InstanceClones.size(); i++)
    {
	if (InstanceClones[i])
	{
	    GetCOREInterface()->DeleteNode(InstanceClones[i]);
	}
    }
    InstanceClones.clear();
    InstanceSources.clear();
    InstanceCloneToSourceMap.clear();
}

void
HEMAX_GeometryHda::ClearPackedPrimNodes()
{
    for (int p = 0; p < PackedPrimClones.size(); p++)
    {
	if (PackedPrimClones[p])
	{
	    GetCOREInterface()->DeleteNode(PackedPrimClones[p]);
	    PackedPrimClones[p] = nullptr;
	}
    }
    PackedPrimClones.clear();
    PackedPrimSources.clear();
    PackedPrimCloneToSourceMap.clear();

    int ChildCount = ContainerNode->NumberOfChildren();

    for (int c = ChildCount - 1; c >= 0; c--)
    {
	INode* Child = ContainerNode->GetChildNode(c);
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
HEMAX_GeometryHda::AssignMaterials(HEMAX_Hda& Hda,
                                   HAPI_NodeId DisplayNodeId,
                                   HEMAX_Part& Part,
                                   HEMAX_GeometryPlugin* GeoPlugin)
{
    if (!(GeoPlugin && GeoPlugin->Mesh))
       return; 

    HEMAX_Mesh* Mesh = GeoPlugin->Mesh;
    HAPI_NodeId* MatNodeIds = Mesh->GetMaterialIdsArray();

    if (!MatNodeIds)
        return;

    if (Mesh->AreMaterialIdsSame)
    {
        HAPI_NodeId MaterialNodeId = MatNodeIds[0];
        
        if (MaterialNodeId == -1)
            return;

        auto ShopNode = Hda.ShopNodes.find(MaterialNodeId);

        if (ShopNode == Hda.ShopNodes.end())
        {
            std::string Msg = std::string("No material node with id: ") + 
                std::to_string(int(MaterialNodeId)) + " was found.";
            HEMAX_Logger::Instance().AddEntry(Msg,
                                              HEMAX_LOG_LEVEL_ERROR);
         
            return;       
        }

        HEMAX_MaterialNode& Mat = ShopNode->second;

        if (Mat.IsDirty())
        {
            Mat.Build3dsmaxMaterial();
        }

        StdMat2* Material = Mat.Get3dsmaxMaterial();

        if (!Material)
            return;

        GeoPlugin->MaxNode->SetMtl(Material);
    }
    else
    {
        HAPI_NodeId* FaceMatIds = Mesh->GetMaterialIdsArray();
        std::unordered_map<HAPI_NodeId, HEMAX_MaterialMapping> MultiMap;

        MultiMtl* MultiMaterial = NewDefaultMultiMtl(); 
        MultiMaterial->SetNumSubMtls(Mesh->GetNumMaterials());

        for (int f = 0; f < Mesh->GetFaceCount(); f++)
        {
            HAPI_NodeId MatNodeId = FaceMatIds[f];
            auto Search = MultiMap.find(MatNodeId);

            if (Search == MultiMap.end())
            {
                StdMat2* Material = nullptr;

                if (MatNodeId != -1)
                {
                    auto ShopNode = Hda.ShopNodes.find(MatNodeId);

                    if (ShopNode != Hda.ShopNodes.end())
                    {
                        HEMAX_MaterialNode& Mat = ShopNode->second;

                        if (Mat.IsDirty())
                        {
                            Mat.Build3dsmaxMaterial();
                        }

                        Material = Mat.Get3dsmaxMaterial();
                    }
                    else
                    {
                        std::string Msg = std::string("No material node with "
                                "id: ") + std::to_string(int(MatNodeId)) +
                                " was found.";
                        HEMAX_Logger::Instance().AddEntry(
                                                    Msg,
                                                    HEMAX_LOG_LEVEL_ERROR);
                    }
                }

                if (!Material)
                {
                    Material = NewDefaultStdMat();
                }

                HEMAX_MaterialMapping MapEntry;
                MapEntry.Material = Material;
                MapEntry.SubNumber = (int)MultiMap.size();
                MultiMap.insert({MatNodeId, MapEntry});

                MultiMaterial->SetSubMtl(MapEntry.SubNumber, Material);
                GeoPlugin->SetFaceMaterialId(f, MapEntry.SubNumber);
            }
            else
            {
                GeoPlugin->SetFaceMaterialId(f, Search->second.SubNumber);
            }
        }

        GeoPlugin->MaxNode->SetMtl(MultiMaterial);
    }
}

void
HEMAX_GeometryHda::ApplySceneMtlToGeometryPlugin(
                                        HEMAX_GeometryPlugin* GeoPlugin)
{
    HEMAX_Mesh* Mesh = GeoPlugin->Mesh;

    // First look up the material in the scene
    std::wstring WideMatName(Mesh->MaterialPath.begin(),
                             Mesh->MaterialPath.end());
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
		std::wstring MatName = WideMatName.substr(
                                        SplitIndex + 1,
                                        WideMatName.size() - SplitIndex - 1);

		MtlBaseLib CustomMatLib;
		int Result = GetCOREInterface()->LoadMaterialLib(
                                        MatLibPath.c_str(),
                                        &CustomMatLib);

		if (Result)
		{
		    int CustomMatNum = CustomMatLib.FindMtlByName(
                                                        WStr(MatName.c_str()));
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

void
HEMAX_GeometryHda::UpdateMaterials(HEMAX_Hda& Hda,
                                   HAPI_NodeId DisplayNodeId,
                                   HEMAX_Part& Part,
                                   HEMAX_GeometryPlugin* GeoPlugin)
{
    if (!(GeoPlugin && GeoPlugin->Mesh))
        return;

    HEMAX_Mesh* Mesh = GeoPlugin->Mesh;
    HAPI_NodeId* MatNodeIds = Mesh->GetMaterialIdsArray();

    if (!MatNodeIds)
        return;

    if (Mesh->AreMaterialIdsSame)
    {
        HAPI_NodeId MaterialNodeId = MatNodeIds[0];

        if (MaterialNodeId == -1)
            return;

        auto ShopNode = Hda.ShopNodes.find(MaterialNodeId);

        if (ShopNode == Hda.ShopNodes.end())
        {
            std::string Msg = std::string("No material node with id: ") + 
                std::to_string(int(MaterialNodeId)) + " was found.";
            HEMAX_Logger::Instance().AddEntry(Msg,
                                              HEMAX_LOG_LEVEL_ERROR);
         
            return;       
        }

        HEMAX_MaterialNode& MatNode = ShopNode->second;

        if (MatNode.IsDirty())
        {
            MatNode.Build3dsmaxMaterial();
        }

        StdMat2* Material = MatNode.Get3dsmaxMaterial();

        if (!Material)
            return;

        GeoPlugin->MaxNode->SetMtl(Material);
    }
    else
    {
        HAPI_NodeId* FaceMatIds = Mesh->GetMaterialIdsArray();
        std::unordered_map<HAPI_NodeId, HEMAX_MaterialMapping> MultiMap;

        Mtl* PluginMat = GeoPlugin->MaxNode->GetMtl();
        MultiMtl* MultiMaterial = dynamic_cast<MultiMtl*>(PluginMat);
        MultiMtl* PreviousMultiMat = nullptr;

        if (MultiMaterial)
        {
            PreviousMultiMat = MultiMaterial;
        }

        MultiMaterial = NewDefaultMultiMtl();
        MultiMaterial->SetNumSubMtls(Mesh->GetNumMaterials());

        for (int f = 0; f < Mesh->GetFaceCount(); f++)
        {
            HAPI_NodeId MatNodeId = FaceMatIds[f];
            auto Search = MultiMap.find(MatNodeId);
 
            if (Search == MultiMap.end())
            {
                    StdMat2* Material = nullptr;

                    if (MatNodeId != -1)
                    {
                        auto ShopNode = Hda.ShopNodes.find(MatNodeId);

                        if (ShopNode != Hda.ShopNodes.end())
                        {
                            HEMAX_MaterialNode& Mat = ShopNode->second;

                            if (Mat.IsDirty())
                            {
                                Mat.Build3dsmaxMaterial(); 
                            }

                            Material = Mat.Get3dsmaxMaterial();
                        }
                        else
                        {
                            std::string Msg = std::string(
                                    "No material node with id: ") +
                                    std::to_string(int(MatNodeId)) +
                                    " was found.";
                            HEMAX_Logger::Instance().AddEntry(
                                                    Msg,
                                                    HEMAX_LOG_LEVEL_ERROR);
                        }
                    } 

                    if (!Material)
                    {
                        Material = NewDefaultStdMat();
                    }

                    HEMAX_MaterialMapping MapEntry;
                    MapEntry.Material = Material;
                    MapEntry.SubNumber = (int)MultiMap.size();
                    MultiMap.insert({MatNodeId, MapEntry});

                    MultiMaterial->SetSubMtl(MapEntry.SubNumber, Material);
                    GeoPlugin->SetFaceMaterialId(f, MapEntry.SubNumber);
            }
            else
            {
                GeoPlugin->SetFaceMaterialId(f, Search->second.SubNumber);
            }
        }

        GeoPlugin->MaxNode->SetMtl(MultiMaterial);

        if (PreviousMultiMat)
        {
            PreviousMultiMat->MaybeAutoDelete();
        }
    }
}

void
HEMAX_GeometryHda::SetPluginNodeName(INode* Node,
                                     const HEMAX_DisplayGeoNode& DisplayNode,
                                     const HEMAX_Part& Part,
                                     bool UseUniqueName)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_AttributeInfo AttrInfo_Detail;
    SM.Session->GetAttributeInfo(DisplayNode.Info.nodeId,
                                 Part.Info.id,
                                 HEMAX_MAX_NODE_NAME_OUTPUT,
                                 HEMAX_ATTRIBUTEOWNER_DETAIL,
                                 &AttrInfo_Detail);

    std::wstring PluginLabel;

    if (AttrInfo_Detail.exists)
    {
        HAPI_StringHandle NameAttrSH;
        SM.Session->GetAttributeStringData(DisplayNode.Info.nodeId,
                                           Part.Info.id,
                                           HEMAX_MAX_NODE_NAME_OUTPUT,
                                           &AttrInfo_Detail,
                                           &NameAttrSH,
                                           0,
                                           1);
        std::string Label = SM.Session->GetHAPIString(NameAttrSH);
        PluginLabel = std::wstring(Label.begin(), Label.end());
    }
    else
    {
        PluginLabel = std::wstring(DisplayNode.Name.begin(),
                                   DisplayNode.Name.end());
    }

    if (UseUniqueName)
    {
        PluginLabel = L"_" + PluginLabel;
        PluginLabel = ContainerNode->GetName() + PluginLabel;
    }

    Node->SetName(PluginLabel.c_str());
}

void
HEMAX_GeometryHda::GetInstancedPluginNodeNames(
    const HEMAX_DisplayGeoNode& DisplayNode, const HEMAX_Part& Part,
    std::vector<std::wstring>& NodeNames)
{
    NodeNames.clear();

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_AttributeInfo NameDetailInfo, NamePrimInfo;
    SM.Session->GetAttributeInfo(DisplayNode.Info.nodeId, Part.Info.id,
                    HEMAX_MAX_NODE_NAME_OUTPUT, HEMAX_ATTRIBUTEOWNER_DETAIL,
                    &NameDetailInfo);
    SM.Session->GetAttributeInfo(DisplayNode.Info.nodeId, Part.Info.id,
                    HEMAX_MAX_NODE_NAME_OUTPUT, HEMAX_ATTRIBUTEOWNER_PRIM,
                    &NamePrimInfo);

    std::wstring PluginLabel;

    if (NamePrimInfo.exists)
    {
        std::vector<HAPI_StringHandle> NameAttrSHArray(NamePrimInfo.count);

        SM.Session->GetAttributeStringData(DisplayNode.Info.nodeId,
            Part.Info.id, HEMAX_MAX_NODE_NAME_OUTPUT, &NamePrimInfo,
            &NameAttrSHArray.front(), 0, NamePrimInfo.count);

        for (int i = 0; i < NameAttrSHArray.size(); i++)
        {
            std::string Label = SM.Session->GetHAPIString(NameAttrSHArray[i]);
            NodeNames.push_back(std::wstring(Label.begin(), Label.end()));
        }
    }
    else if (NameDetailInfo.exists)
    {
        HAPI_StringHandle NameAttrSH;
        SM.Session->GetAttributeStringData(DisplayNode.Info.nodeId,
            Part.Info.id, HEMAX_MAX_NODE_NAME_OUTPUT, &NameDetailInfo,
            &NameAttrSH, 0, 1);
        std::string Label = SM.Session->GetHAPIString(NameAttrSH);
        NodeNames.push_back(std::wstring(Label.begin(), Label.end()));
    }
    else
    {
        PluginLabel = std::wstring(DisplayNode.Name.begin(),
                                   DisplayNode.Name.end());

        NodeNames.push_back(PluginLabel);
    }

    bool UseUniqueName = false;
    HEMAX_UserPrefs::Get().GetBoolSetting(HEMAX_SETTING_NODE_NAMES_UNIQUE,
        UseUniqueName);

    if (UseUniqueName)
    {
        for (int i = 0; i < NodeNames.size(); i++)
        {
            NodeNames[i] = L"_" + NodeNames[i];
            NodeNames[i] = ContainerNode->GetName() + NodeNames[i];
        }
    }
}

std::string
HEMAX_GeometryHda::GetDetailAttributeOverride(const std::string& Name)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    std::string OverrideValue;

    if (Hda.HdaType == OBJ_LEVEL_HDA)
    {
        std::vector<HEMAX_DisplayGeoNode*> DisplayNodes = Hda.AllDisplayNodes();
        bool FoundAttr = false;

        for (int i = 0; i < DisplayNodes.size(); i++)
        {
            for (int p = 0; p < DisplayNodes[i]->Parts.size(); p++)
            {
                HAPI_AttributeInfo AttrInfo;
                SM.Session->GetAttributeInfo(DisplayNodes[i]->Info.nodeId,
                    DisplayNodes[i]->Parts[p].Info.id, Name.c_str(),
                    HEMAX_ATTRIBUTEOWNER_DETAIL, &AttrInfo);

                if (AttrInfo.exists)
                {
                    HAPI_StringHandle ValSH;
                    SM.Session->GetAttributeStringData(DisplayNodes[i]->Info.nodeId,
                        DisplayNodes[i]->Parts[p].Info.id, Name.c_str(),
                        &AttrInfo, &ValSH, 0, 1);
                    std::string AttrVal = SM.Session->GetHAPIString(ValSH);

                    if (FoundAttr && AttrVal != OverrideValue)
                    {
                        HEMAX_Logger::Instance().AddEntry(
                            std::string("Detail attribute override ") + Name +
                            " was found on multiple parts and had different values."
                            " The first value will be used.", HEMAX_LOG_LEVEL_WARN);
                        return AttrVal;
                    }
                    else
                    {
                        FoundAttr = true;
                        OverrideValue = AttrVal;
                    }
                }
            }
        }
    }
    else if (Hda.HdaType == SOP_LEVEL_HDA)
    {
        bool FoundAttr = false;
        HEMAX_DisplayGeoNode& TopNode = Hda.TopLevelSopNode;
        for (int p = 0; p < TopNode.Parts.size(); p++)
        {
            HAPI_AttributeInfo AttrInfo;
            SM.Session->GetAttributeInfo(TopNode.Info.nodeId,
                TopNode.Parts[p].Info.id, Name.c_str(),
                HEMAX_ATTRIBUTEOWNER_DETAIL, &AttrInfo);

            if (AttrInfo.exists)
            {
                HAPI_StringHandle ValSH;
                SM.Session->GetAttributeStringData(TopNode.Info.nodeId,
                    TopNode.Parts[p].Info.id, Name.c_str(), &AttrInfo, &ValSH, 
                    0, 1);
                std::string AttrVal = SM.Session->GetHAPIString(ValSH);

                if (FoundAttr && AttrVal != OverrideValue)
                {
                    HEMAX_Logger::Instance().AddEntry(
                        std::string("Detail attribute override ") + Name +
                        " was found on multiple parts and had different values."
                        " The first value will be used.", HEMAX_LOG_LEVEL_WARN);
                    return AttrVal;
                }
                else
                {
                    FoundAttr = true;
                    OverrideValue = AttrVal;
                }
            }
        }
    }

    return OverrideValue;
}

bool
HEMAX_GeometryHda::IsInstanceSource(INode* Node)
{
    return InstanceSources.find(Node) != InstanceSources.end();
}

bool
HEMAX_GeometryHda::IsPackedPrimSource(INode* Node)
{
    return PackedPrimSources.find(Node) != PackedPrimSources.end();
}

bool
HEMAX_GeometryHda::IsInstance(INode* Node)
{
    auto It = std::find(InstanceClones.begin(), InstanceClones.end(), Node);
    return It != InstanceClones.end();
}

bool
HEMAX_GeometryHda::IsPackedPrim(INode* Node)
{
    auto It = std::find(PackedPrimClones.begin(), PackedPrimClones.end(), Node);
    return It != PackedPrimClones.end();
}

INode*
HEMAX_GeometryHda::GetBakedInstanceSource(INode* Node,
    std::unordered_map<INode*, INode*>& InstanceSourceToBakedInstanceSource)
{
    if (IsInstance(Node))
    {
        auto Search = InstanceCloneToSourceMap.find(Node);
        if (Search != InstanceCloneToSourceMap.end())
        {
            INode* SourceNode = Search->second;
            Search = InstanceSourceToBakedInstanceSource.find(SourceNode);
            if (Search != InstanceSourceToBakedInstanceSource.end())
            {
                return Search->second;
            }
        }
    }
    else if (IsPackedPrim(Node))
    {
        auto Search = PackedPrimCloneToSourceMap.find(Node);
        if (Search != PackedPrimCloneToSourceMap.end())
        {
            INode* SourceNode = Search->second;
            Search = InstanceSourceToBakedInstanceSource.find(SourceNode);
            if (Search != InstanceSourceToBakedInstanceSource.end())
            {
                return Search->second;
            } 
        }
    }

    return nullptr;
}
