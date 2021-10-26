#include "HEMAX_Modifier.h"

#include "HEMAX_Events.h"
#include "HEMAX_Logger.h"
#include "HEMAX_ModifierHda.h"
#include "HEMAX_Input.h"
#include "HEMAX_Input_NURBS.h"
#include "HEMAX_Input_Spline.h"
#include "HEMAX_Plugin.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Types.h"

#include <linshape.h>
#include <mnmesh.h>
#include <modstack.h>
#include <polyobj.h>
#include <surf_api.h>
#include <triobj.h>

ClassDesc2*
GetHEMAX_ModifierClassDesc() {
    static HEMAX_ModifierClassDesc ModifierClassDesc;
    return &ModifierClassDesc;
}

HEMAX_Modifier::HEMAX_Modifier()
    : InputNode(nullptr), HasBeenCreated(false),
    MaxNode(nullptr), ThePlugin(nullptr)
{
    Hda = nullptr;
    Collapsing = false;
    IsUndone = false;
    Dragging = false;
    Collapsed = false;
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
    if (ThePlugin && Collapsed)
    {
        ReverseCollapse();
    }
    else if (ThePlugin)
    {
        HEMAX_3dsmaxHda* MaxHda = ThePlugin->GetPluginStore()->Find3dsmaxHda(
                                                                Hda->MainNode);
        ThePlugin->GetEventHub()->SelectionSetChanged(MaxHda);
    }
}

void
HEMAX_Modifier::EndEditParams(IObjParam *IP, ULONG Flags, Animatable *Prev)
{
    if (ThePlugin)
    {
        ThePlugin->GetEventHub()->SelectionSetChanged(nullptr);
    }
}

void
HEMAX_Modifier::SetAssetName(std::string Name)
{
    AssetName = Name;
    WideAssetName = std::wstring(AssetName.begin(), AssetName.end());
}

void
HEMAX_Modifier::SetHda(HEMAX_Hda* Hda)
{
    this->Hda = Hda;
    Asset = Hda->HdaAsset;
    AssetIndex = Hda->HdaAssetIndex;
}

HEMAX_Hda*
HEMAX_Modifier::GetHda()
{
    return Hda;
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
    if (Collapsed)
    {
        ReverseCollapse();
    }

    if (HasBeenCreated &&
        HEMAX_SessionManager::GetSessionManager().IsSessionActive() &&
        !Collapsed)
    {
	Object* MaxObject = os->obj;

	PolyObject* MaxPolyObject = nullptr;
	LinearShape* MaxLinearShape = nullptr;

	if (InputNode)
	{
	    delete InputNode;
            InputNode = nullptr;
	}

	if (MaxObject->CanConvertToType(Class_ID(LINEARSHAPE_CLASS_ID, 0)))
	{
	    MaxLinearShape = (LinearShape*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), Class_ID(LINEARSHAPE_CLASS_ID, 0));
	    MaxPolyObject = (PolyObject*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), polyObjectClassID);

            if (HEMAX_Utilities::IsOnlyClosedSplines(MaxLinearShape) ||
                HEMAX_Utilities::IsOnlyOpenSplines(MaxLinearShape))
            {
                ULONG NodeHandle = -1;
                if (MaxNode)
                    NodeHandle = MaxNode->GetHandle();

                InputNode = new HEMAX_Input_Spline(HEMAX_INPUT_SUBNETWORK,
                    MaxLinearShape, NodeHandle);
            }
            else
            {
                HEMAX_Logger::Instance().AddEntry("A shape must contain "
                    "exclusively closed splines or exclusively open splines to "
                    "be sent to Houdini", HEMAX_LOG_LEVEL_WARN);
            }
	}
	else if (MaxObject->CanConvertToType(EDITABLE_CVCURVE_CLASS_ID))
	{
            ULONG NodeHandle = -1;
            if (MaxNode)
                NodeHandle = MaxNode->GetHandle();
	    InputNode = new HEMAX_Input_NURBS(HEMAX_INPUT_SUBNETWORK, MaxObject,
                NodeHandle);
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
	else
	{
	    HEMAX_Logger::Instance().AddEntry("The object type that this " \
		    "Houdini Digital Asset is applied to is not supported.",
		    HEMAX_LOG_LEVEL_WARN);
	    return;
	}

	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

	if (InputNode)
	{
	    Hda->MainNode.ConnectInputNode(InputNode->GetInputNodeId(), 0);
	}

	if (Hda->MainNode.AutoRecookOnParameterUpdate)
	{
	    Hda->MainNode.Cook();
	}

	if (Hda->MainNode.Type == HEMAX_NODE_SOP)
	{
	    HEMAX_DisplayGeoNode& DisplayNode = Hda->TopLevelSopNode;
	    if (DisplayNode.Info.type != HAPI_GEOTYPE_INVALID)
	    {
		if (DisplayNode.Info.partCount == 1)
		{
		    DisplayNode.Parts[0].Update(DisplayNode.Info.nodeId, 0);

		    HEMAX_Mesh *TheMesh = DisplayNode.Parts[0].BuildMesh();

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
		    }
		    else
		    {
			std::string Msg = "HEMAX_Modifier: updated modifier " \
				"HDA Parameters and attempted to rebuild the " \
				"mesh, but it was either empty or invalid.";
			HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);
		    }
		}
		else
		{
		    std::string Msg = "Modifier HDAs do not support display geometry " \
			    "with more than 1 part.";
		    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
		    return;
		}
	    }
	    else
	    {
		HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_Modifier: could not get display geo for " + AssetName + " modifier."),
			HEMAX_LOG_LEVEL_ERROR);
		return;
	    }
	}
	else
	{
	    HEMAX_Logger::Instance().ShowDialog(std::string("Modifier HDA"),
		    std::string("HEMAX_Modifier: HDA used to instantiate modifier " + AssetName +
			" is not a SOP level HDA and therefore will have no effect."),
		    HEMAX_LOG_LEVEL_WARN);
	}
    }
}

void
HEMAX_Modifier::ApplyMaterialsToNode()
{
    if (Hda)
    {
	HEMAX_DisplayGeoNode& DisplayNode = Hda->TopLevelSopNode;
	if (DisplayNode.Parts.size() == 0)
	{
	    HEMAX_Mesh* TheMesh = DisplayNode.Parts[0].GetMesh();
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
    }
}

RefResult
HEMAX_Modifier::NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget,
	PartID& partID, RefMessage message, BOOL propagate)
{
    return REF_SUCCEED;
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
    Collapsing = true; 
}

void
HEMAX_Modifier::NotifyPostCollapse(INode* TheNode, Object* Obj, IDerivedObject* DerivedObject, int Index)
{
    if (ThePlugin)
    {
	ThePlugin->HandleModifierStackCollapse(TheNode, this);
    }

    Collapsing = false;
    Collapsed = true;
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

bool
HEMAX_Modifier::IsCollapsing()
{
    return Collapsing;
}

bool
HEMAX_Modifier::IsBeingDragged()
{
    return Dragging;
}

void
HEMAX_Modifier::SetIsBeingDragged(bool IsDragging)
{
    Dragging = IsDragging;
}

void
HEMAX_Modifier::ReverseCollapse()
{
    if (!ThePlugin)
        return;

    HEMAX_ModifierHda* ModifierHda = new HEMAX_ModifierHda;
    ModifierHda->Recreate(Asset, AssetIndex, this, MaxNode);
    ThePlugin->GetPluginStore()->Add3dsmaxHda(
                        MaxNode->GetHandle(), ModifierHda); 
    ThePlugin->ReloadHdaFromCustomAttributes(ModifierHda);
    ThePlugin->GetEventHub()->SelectionSetChanged(ModifierHda);
    Collapsed = false;
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

#ifdef HEMAX_VERSION_2022
const TCHAR*
HEMAX_ModifierClassDesc::NonLocalizedClassName()
{
    return _T(HEMAX_MODIFIER_STACK_PLUGIN_NAME);
}
#endif

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
