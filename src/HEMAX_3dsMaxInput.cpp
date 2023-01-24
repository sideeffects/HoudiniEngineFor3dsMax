#include "HEMAX_3dsMaxInput.h"

#pragma warning(push, 0)
#include <surf_api.h>
#include <linshape.h>
#pragma warning(pop)

#include "HEMAX_Input_Spline.h"
#include "HEMAX_Input_NURBS.h"
#include "HEMAX_Input_Geometry.h"
#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"

HEMAX_3dsMaxInput::HEMAX_3dsMaxInput(INode* Node)
    : MaxNode(Node), InputNode(nullptr), InputCount(0), Dirty(false)
{
    if (MaxNode)
    {
	CreateInputNode();
	ReplaceReference(0, Node);
    }
}

HEMAX_3dsMaxInput::~HEMAX_3dsMaxInput()
{
    DeleteInputNode();
}

RefResult
HEMAX_3dsMaxInput::NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle HTarget, PartID& PartID, RefMessage Message, BOOL Propagate)
{
    Dirty = true;
    return REF_SUCCEED;
}

ULONG
HEMAX_3dsMaxInput::Get3dsMaxNodeHandle()
{
    if (MaxNode)
    {
	return MaxNode->GetHandle();
    }
    else
    {
	return -1;
    }
}

HEMAX_Input*
HEMAX_3dsMaxInput::GetInputNode()
{
    return InputNode;
}

bool
HEMAX_3dsMaxInput::IsDirty()
{
    return Dirty;
}

void
HEMAX_3dsMaxInput::UpdateInputNode()
{
    if (Dirty)
    {
	DeleteInputNode();
	CreateInputNode();
	Dirty = false;
    }
}

int
HEMAX_3dsMaxInput::GetInputCount()
{
    return InputCount;
}

void
HEMAX_3dsMaxInput::IncrementInputCount()
{
    InputCount++;
}

void
HEMAX_3dsMaxInput::DecrementInputCount()
{
    InputCount--;
}

void
HEMAX_3dsMaxInput::AddSubnetworkUsage(HEMAX_Node Node, int Subnetwork)
{
    HEMAX_InputUsage NewUsage;
    NewUsage.Node = Node;
    NewUsage.Subnetwork = Subnetwork;
    NewUsage.Parameter = "";

    Usages.push_back(NewUsage);
}

void
HEMAX_3dsMaxInput::AddParameterUsage(HEMAX_Node Node, std::string Parameter)
{
    HEMAX_InputUsage NewUsage;
    NewUsage.Node = Node;
    NewUsage.Subnetwork = -1;
    NewUsage.Parameter = Parameter;

    Usages.push_back(NewUsage);
}

void
HEMAX_3dsMaxInput::RemoveSubnetworkUsage(HEMAX_Node Node, int Subnetwork)
{
    for (int i = 0; i < Usages.size(); i++)
    {
	if (Usages[i].Node.Info.id == Node.Info.id && Usages[i].Subnetwork == Subnetwork)
	{
	    Usages.erase(Usages.begin() + i);
	}
    }
}

void
HEMAX_3dsMaxInput::RemoveParameterUsage(HEMAX_Node Node, std::string Parameter)
{
    for (int i = 0; i < Usages.size(); i++)
    {
	if (Usages[i].Node.Info.id == Node.Info.id && Usages[i].Parameter == Parameter)
	{
	    Usages.erase(Usages.begin() + i);
	}
    }
}

std::vector<HEMAX_InputUsage>&
HEMAX_3dsMaxInput::GetInputUsages()
{
    return Usages;
}

void
HEMAX_3dsMaxInput::CreateInputNode()
{
    if (MaxNode)
    {
	ObjectState MaxObjectState = MaxNode->EvalWorldState(GetCOREInterface()->GetTime());
	Object* MaxObject = MaxObjectState.obj;

	if (!MaxObject)
	    return;

	if (MaxObject->CanConvertToType(Class_ID(LINEARSHAPE_CLASS_ID, 0)))
	{
	    // A closed linear shape will be treated like input geometry since it should just be a polygon in Houdini
	    // Otherwise, treat it as an input spline

	    LinearShape* MaxLinearShape = (LinearShape*)MaxObject->ConvertToType(GetCOREInterface()->GetTime(), Class_ID(LINEARSHAPE_CLASS_ID, 0));

            if (HEMAX_Utilities::IsOnlyClosedSplines(MaxLinearShape) ||
                HEMAX_Utilities::IsOnlyOpenSplines(MaxLinearShape))
            {
                InputNode = new HEMAX_Input_Spline(MaxNode->GetHandle());
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
	    InputNode = new HEMAX_Input_NURBS(MaxNode->GetHandle());
	}
	else
	{
	    InputNode = new HEMAX_Input_Geometry(MaxNode->GetHandle());
	}
    }
}

void
HEMAX_3dsMaxInput::DeleteInputNode()
{
    if (InputNode)
    {
	delete InputNode;
	InputNode = nullptr;
    }
}
