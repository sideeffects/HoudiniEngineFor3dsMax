#include "HEMAX_Modifier.h"
#include "HEMAX_Logger.h"
#include "HEMAX_Types.h"
#include "HEMAX_Input.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Input_Spline.h"
#include "HEMAX_Input_NURBS.h"

#include <triobj.h>
#include <mnmesh.h>
#include <polyobj.h>
#include <modstack.h>
#include <linshape.h>
#include <surf_api.h>

ClassDesc2*
GetHEMAX_ModifierClassDesc() {
    static HEMAX_ModifierClassDesc ModifierClassDesc;
    return &ModifierClassDesc;
}

HEMAX_Modifier::HEMAX_Modifier()
    : InputNode(nullptr), HasBeenCreated(false), IsUndone(false),
    MaxNode(nullptr), ThePlugin(nullptr), TheMesh(nullptr)
{
    AssetName = "";
}

HEMAX_Modifier::~HEMAX_Modifier()
{
}

void
HEMAX_Modifier::CleanupHAPIComponents()
{
    if (InputNode)
    {
        delete InputNode;
        InputNode = nullptr;
    }

    HEMAX_NodeId SopParent = OutputNode.Info.parentId;
    DeleteNode(OutputNode);
    OutputNode.Info.isValid = false;

    HEMAX_Node SopParentNode(SopParent, HEMAX_NODE_INVALID);
    DeleteNode(SopParentNode);

    HasBeenCreated = false;
}

void
HEMAX_Modifier::ClearHAPIReferences()
{
    InputNode = nullptr;
}

void
HEMAX_Modifier::DeleteInputNode()
{
    if (InputNode)
    {
        delete InputNode;
        InputNode = nullptr;
    }
}

void
HEMAX_Modifier::DeleteThis()
{
    delete this;
}

void
HEMAX_Modifier::RegisterPlugin(HEMAX_Plugin* Plugin)
{
    ThePlugin = Plugin;
}

void
HEMAX_Modifier::BeginEditParams(IObjParam  *IP, ULONG Flags, Animatable *Prev)
{
    if (IsUndone)
    {
        if (ThePlugin && ThePlugin->ReconnectThisModifierHDA(MaxNode, this))
        {
            IsUndone = false;
            HasBeenCreated = true;
        }
    }

    if (ThePlugin)
    {
        ThePlugin->RequestParameterViewerSetSelection(OutputNode);
    }
}

void
HEMAX_Modifier::EndEditParams(IObjParam *IP, ULONG Flags, Animatable *Prev)
{
    if (ThePlugin)
    {
        ThePlugin->RequestParameterViewerClearSelection();
    }
}

void
HEMAX_Modifier::SetAssetName(std::string Name)
{
    AssetName = Name;
    WideAssetName = std::wstring(AssetName.begin(), AssetName.end());
}

Class_ID
HEMAX_Modifier::ClassID()
{
    return HEMAX_Modifier_CLASS_ID;
}

Class_ID
HEMAX_Modifier::InputType()
{
    return defObjectClassID;
}

ChannelMask 
HEMAX_Modifier::ChannelsUsed()
{
    return GEOM_CHANNEL | TOPO_CHANNEL;
}

ChannelMask
HEMAX_Modifier::ChannelsChanged()
{
    // Be more specific later to avoid bad performance.
    return PART_ALL;
}

void
HEMAX_Modifier::ModifyObject(TimeValue t, ModContext& mc, ObjectState* os, INode* node)
{
    if (HasBeenCreated && HEMAX_SessionManager::GetSessionManager().IsSessionActive())
    {
        Object* MaxObject = os->obj;

        PolyObject* MaxPolyObject = nullptr;
        LinearShape* MaxLinearShape = nullptr;

        if (InputNode)
        {
            delete InputNode;
        }
        
        if (MaxObject->CanConvertToType(Class_ID(LINEARSHAPE_CLASS_ID, 0)))
        {
            MaxLinearShape = (LinearShape*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), Class_ID(LINEARSHAPE_CLASS_ID, 0));
            MaxPolyObject = (PolyObject*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), polyObjectClassID);

            if (HEMAX_Utilities::IsLinearSplineClosed(MaxLinearShape))
            {
                InputNode = new HEMAX_Input_Geometry(HEMAX_INPUT_SUBNETWORK, MaxPolyObject, MaxNode);
            }
            else
            {
                InputNode = new HEMAX_Input_Spline(HEMAX_INPUT_SUBNETWORK, MaxLinearShape);
            }
        }
        else if (MaxObject->CanConvertToType(EDITABLE_CVCURVE_CLASS_ID))
        {
            InputNode = new HEMAX_Input_NURBS(HEMAX_INPUT_SUBNETWORK, MaxObject);
            TriObject* MaxTriObject = (TriObject*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), triObjectClassID);
            MaxPolyObject = (PolyObject*)MaxTriObject->ConvertToType(GetCOREInterface()->GetTime(), polyObjectClassID);

            if ((void*)MaxTriObject != (void*)MaxPolyObject)
            {
                delete MaxTriObject;
            }
        }
        else if (MaxObject->CanConvertToType(polyObjectClassID))
        {
            MaxPolyObject = (PolyObject*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), polyObjectClassID);
            InputNode = new HEMAX_Input_Geometry(HEMAX_INPUT_SUBNETWORK, MaxPolyObject, MaxNode);
        }
        else if (MaxObject->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
        {
            TriObject* MaxTriObject = (TriObject*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), Class_ID(TRIOBJ_CLASS_ID, 0));
            InputNode = new HEMAX_Input_Geometry(HEMAX_INPUT_SUBNETWORK, MaxTriObject);
        }
        else
        {
            // Nothing to be done if we can't use the input
            return;
        }

        HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

        if (InputNode)
        {
            SM.Session->ConnectNodeInput(OutputNode.Info.id, 0, InputNode->GetInputNodeId());
        }

        if (OutputNode.AutoRecookOnParameterUpdate)
        {
            Cook(OutputNode);
        }

        if (OutputNode.Type == HEMAX_NODE_SOP)
        {
            HAPI_GeoInfo GeoInfo;
            HAPI_PartInfo PartInfo;

            HEMAX_DisplayGeoNode DisplayNode;

            if (SM.Session->GetDisplayGeoInfo(OutputNode.Info.id, &GeoInfo))
            {
                DisplayNode.DisplayGeoInfo = GeoInfo;
                if (GeoInfo.partCount == 1)
                {
                    SM.Session->GetPartInfo(GeoInfo.nodeId, 0, &PartInfo);

                    HEMAX_Part DisplayPart;
                    DisplayPart.Info = PartInfo;
                    DisplayNode.Parts.push_back(DisplayPart);

                    if (TheMesh)
                    {
                        delete TheMesh;
                        TheMesh = nullptr;
                    }

                    LoadMesh(&TheMesh, DisplayNode, PartInfo.id);

                    if (TheMesh)
                    {
                        if (MaxPolyObject)
                        {
                            TheMesh->MarshallDataInto3dsMaxMNMesh(MaxPolyObject->GetMesh());

                            if (MaxObject != MaxPolyObject)
                            {
                                os->obj = MaxPolyObject;
                            }
                        }
                        else
                        {
                            TriObject* MaxTriObject = (TriObject*)MaxObject->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
                            TheMesh->MarshallDataInto3dsMaxMesh(MaxTriObject->mesh);
                        }
                    }
                    else
                    {
                        HEMAX_Logger::Instance().AddEntry("HEMAX_Modifier: updated modifier HDA parameters and attempted to rebuild the mesh, but it was either empty or invalid.",
                            HEMAX_LOG_LEVEL_INFO);
                    }
                }
                else
                {
                    // Multiple parts
                    // Use the first mesh part for now

                    for (int p = 0; p < GeoInfo.partCount; p++)
                    {
                        SM.Session->GetPartInfo(GeoInfo.nodeId, p, &PartInfo);

                        if (PartInfo.type == HEMAX_PARTTYPE_MESH)
                        {
                            if (TheMesh)
                            {
                                delete TheMesh;
                                TheMesh = nullptr;
                            }

                            HEMAX_Part DisplayPart;
                            DisplayPart.Info = PartInfo;
                            DisplayNode.Parts.push_back(DisplayPart);

                            LoadMesh(&TheMesh, DisplayNode, PartInfo.id);

                            if (TheMesh)
                            {
                                if (MaxPolyObject)
                                {
                                    TheMesh->MarshallDataInto3dsMaxMNMesh(MaxPolyObject->GetMesh());

                                    if (MaxObject != MaxPolyObject)
                                    {
                                        os->obj = MaxPolyObject;
                                        return;
                                    }
                                }
                                else
                                {
                                    TriObject* MaxTriObject = (TriObject*)MaxObject->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
                                    TheMesh->MarshallDataInto3dsMaxMesh(MaxTriObject->mesh);
                                }
                            }
                            else
                            {
                                HEMAX_Logger::Instance().AddEntry("HEMAX_Modifier: updated modifier HDA parameters and attempted to rebuild the mesh, but it was either empty or invalid.",
                                    HEMAX_LOG_LEVEL_INFO);
                            }
                        }
                    }

                    /*
                    std::vector<HEMAX_PartInfo> PartInfos;

                    for (int p = 0; p < GeoInfo.partCount; ++p)
                    {
                        SM.Session->GetPartInfo(GeoInfo.nodeId, p, &PartInfo);
                        PartInfos.push_back(PartInfo);
                    }

                    //TODO : conglomerate multiple parts into a single mesh
                    //HEMAX_Mesh* TheMesh = MakeMesh(GeoInfo, PartInfos);
                    HEMAX_Mesh* TheMesh = nullptr;

                    if (TheMesh)
                    {
                        if (MaxPolyObject)
                        {
                            TheMesh->MarshallDataInto3dsMaxMNMesh(MaxPolyObject->GetMesh());
                            if (MaxNode)
                            {
                                // TODO
                                //TheMesh->SetNodeMaterialFromMeshMaterials(MaxNode);
                            }
                            TriObject* ConvertedPolyObj = (TriObject*)MaxPolyObject->ConvertToType(HEMAX_FRAME_ZERO, Class_ID(TRIOBJ_CLASS_ID, 0));
                            MaxTriMesh = ConvertedPolyObj->GetMesh();
                        }
                        else
                        {
                            TheMesh->MarshallDataInto3dsMaxMesh(MaxTriMesh);
                        }

                        delete TheMesh;
                    }
                    else
                    {
                        HEMAX_Logger::Instance().AddEntry("HEMAX_Modifier: updated modifier HDA parameters and attempted to rebuild the mesh, but it was either empty or invalid.",
                            HEMAX_LOG_LEVEL_INFO);
                    }
                    */
                }
            }
            else
            {
                HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_Modifier: could not get display geo for " + AssetName + " modifier."),
                    HEMAX_LOG_LEVEL_ERROR);
            }
        }
        else
        {
            HEMAX_Logger::Instance().ShowDialog(std::string("Modifier HDA"),
                std::string("HEMAX_Modifier: HDA used to instantiate modifier " + AssetName + " is not a SOP level HDA and therefore will have no effect."),
                HEMAX_LOG_LEVEL_WARN);
        }
    }
}

void
HEMAX_Modifier::ApplyMaterialsToNode()
{
    if (MaxNode && TheMesh)
    {
        if (!(TheMesh->MaterialPath.empty()))
        {
            std::wstring WideMatName(TheMesh->MaterialPath.begin(), TheMesh->MaterialPath.end());
            MtlBaseLib* SceneMatLib = GetCOREInterface()->GetSceneMtls();
            int MatNum = SceneMatLib->FindMtlByName(WStr(WideMatName.c_str()));

            if (MatNum > -1)
            {
                MtlBase* Mat = (*SceneMatLib)[MatNum];
                Mtl* MtlMat = (Mtl*)Mat;
                MaxNode->SetMtl(MtlMat);
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
                                MaxNode->SetMtl(MtlMat);
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}

RefResult
HEMAX_Modifier::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget,
    PartID& partID, RefMessage message, BOOL propagate)
{
    return REF_SUCCEED;
}

const TCHAR*
HEMAX_Modifier::GetObjectName()
{
    if (AssetName != "")
    {
        return WideAssetName.c_str();
    }
    else
    {
        return _T(HEMAX_MODIFIER_STACK_PLUGIN_NAME);
    }
}

void
HEMAX_Modifier::MarkCreated()
{
    HasBeenCreated = true;
}

void
HEMAX_Modifier::MarkUndone()
{
    IsUndone = true;
}

void
HEMAX_Modifier::NotifyPreCollapse(INode* TheNode, IDerivedObject* DerivedObject, int Index)
{
    if (ThePlugin)
    {
        ThePlugin->HandleModifierStackCollapse(TheNode, this);
    }
}

ReferenceTarget*
HEMAX_Modifier::Clone(RemapDir& Remap)
{
    ReferenceTarget* TheClone = new HEMAX_Modifier();
    BaseClone(this, TheClone, Remap);
    return TheClone;
}

void
HEMAX_Modifier::BaseClone(ReferenceTarget* From, ReferenceTarget* To, RemapDir& Remap)
{
    if (!To || !From || From == To)
    {
        return;
    }
    else
    {
        Modifier::BaseClone(From, To, Remap);
    }
}

bool
HEMAX_Modifier::CheckIfTopOfStack(INode* ContainerNode)
{
    if (ContainerNode)
    {
        Object* Obj = ContainerNode->GetObjectRef();
        if (Obj)
        {
            if (Obj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
            {
                IDerivedObject* DerivedObject = dynamic_cast<IDerivedObject*>(Obj);
                int ModifierCount = DerivedObject->NumModifiers();

                if (ModifierCount > 0)
                {
                    if (DerivedObject->GetModifier(0) == this)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }

    return false;
}

int
HEMAX_ModifierClassDesc::IsPublic()
{
    return 1;
}

void*
HEMAX_ModifierClassDesc::Create(BOOL Loading)
{
    return new HEMAX_Modifier;
}

const TCHAR*
HEMAX_ModifierClassDesc::ClassName()
{
    return _T(HEMAX_MODIFIER_STACK_PLUGIN_NAME);
}

SClass_ID
HEMAX_ModifierClassDesc::SuperClassID()
{
    return OSM_CLASS_ID;
}

Class_ID
HEMAX_ModifierClassDesc::ClassID()
{
    return HEMAX_Modifier_CLASS_ID;
}

const TCHAR*
HEMAX_ModifierClassDesc::Category()
{
    return L"Houdini Engine";
}