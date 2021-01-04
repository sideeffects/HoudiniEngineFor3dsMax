#include "HEMAX_ReferenceMaker.h"

#include "HEMAX_Plugin.h"

HEMAX_ReferenceMaker::HEMAX_ReferenceMaker(HEMAX_Plugin* Plugin)
    : ThePlugin(Plugin)
{}

HEMAX_ReferenceMaker::~HEMAX_ReferenceMaker() {}

RefResult
HEMAX_ReferenceMaker::NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate)
{
    if (Message == REFMSG_SHAPE_END_CHANGE)
    {
	INode* TheNode = (INode*)hTarget;

	if (TheNode)
	{
	    INode* HoudiniAssetNode = TheNode->GetParentNode();

	    if (HoudiniAssetNode)
	    {
		ThePlugin->DirtyEditableNodes(HoudiniAssetNode->GetHandle());
	    }
	}
    }
    else if (Message == REFMSG_CHANGE)
    {
	INode* TheNode = (INode*)hTarget;

	if (TheNode)
	{
	    INode* HoudiniAssetNode = TheNode->GetParentNode();

	    if (HoudiniAssetNode)
	    {
		ThePlugin->DirtyEditableNodes(HoudiniAssetNode->GetHandle());
	    }
	}
    }

    return REF_SUCCEED;
}

void
HEMAX_ReferenceMaker::AddEditableNodeToWatch(INode* Node)
{
    if (Node)
    {
	ReplaceReference(0, Node);
    }
}
