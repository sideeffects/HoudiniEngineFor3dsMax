#include "HEMAX_3dsMaxInput.h"

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <surf_api.h>
#include <linshape.h>
#pragma warning(pop)

#include "HEMAX_Input_Spline.h"
#include "HEMAX_Input_NURBS.h"
#include "HEMAX_Input_Geometry.h"
#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Utilities.h"

HEMAX_3dsMaxInput::HEMAX_3dsMaxInput(INode* Node)
    : MaxNode(Node)
{
    if (MaxNode)
    {
	CreateInputNode();
	ReplaceReference(0, Node);
    }
}

RefResult
HEMAX_3dsMaxInput::NotifyRefChanged(
        const Interval& ChangeInt,
        RefTargetHandle HTarget,
        PartID& PartID,
        RefMessage Message,
        BOOL Propagate)
{
    Dirty = true;
    return REF_SUCCEED;
}

ULONG
HEMAX_3dsMaxInput::Get3dsMaxNodeHandle() const
{
    if (MaxNode)
	return MaxNode->GetHandle();
    else
        return INode::kNullHandle;
}

HEMAX_Input*
HEMAX_3dsMaxInput::GetInputNode()
{
    return InputNode.get();
}

bool
HEMAX_3dsMaxInput::IsDirty()
{
    return Dirty;
}

void
HEMAX_3dsMaxInput::UpdateInputNode()
{
    if (!Dirty)
        return;

    DeleteInputNode();
    CreateInputNode();
    Dirty = false;
}

int
HEMAX_3dsMaxInput::GetUsageCount()
{
    return Usages.size();
}

void
HEMAX_3dsMaxInput::AddUsage(const HEMAX_InputUsage& Usage)
{
    Usages.push_back(Usage);
}

void
HEMAX_3dsMaxInput::RemoveUsage(const HEMAX_InputUsage& Usage)
{
    for (int i = 0; i < Usages.size(); ++i)
    {
        if (Usages[i] == Usage)
        {
            Usages.erase(Usages.begin() + i);
            return;
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
    if (!MaxNode)
        return;

    ObjectState MaxObjectState = MaxNode->EvalWorldState(GetCOREInterface()->GetTime());
    Object* MaxObject = MaxObjectState.obj;

    if (!MaxObject)
        return;

    if (MaxObject->CanConvertToType(Class_ID(LINEARSHAPE_CLASS_ID, 0)))
    {
        // A closed linear shape will be treated like input geometry since it should just be a polygon in Houdini
        // Otherwise, treat it as an input spline

        LinearShape* MaxLinearShape =
            (LinearShape*)MaxObject->ConvertToType(
                    GetCOREInterface()->GetTime(),
                    Class_ID(LINEARSHAPE_CLASS_ID, 0));

        if (HEMAX_Utilities::IsOnlyClosedSplines(MaxLinearShape) ||
            HEMAX_Utilities::IsOnlyOpenSplines(MaxLinearShape))
        {
            InputNode.reset(new HEMAX_Input_Spline(MaxNode->GetHandle()));
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
        InputNode.reset(new HEMAX_Input_NURBS(MaxNode->GetHandle()));
    }
    else
    {
        InputNode.reset(new HEMAX_Input_Geometry(MaxNode->GetHandle()));
    }
}

void
HEMAX_3dsMaxInput::DeleteInputNode()
{
    InputNode.reset();
}
