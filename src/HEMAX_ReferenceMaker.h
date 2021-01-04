#pragma once

#include <maxapi.h>

class HEMAX_Plugin;

class HEMAX_ReferenceMaker : public ReferenceMaker
{
    public:
	HEMAX_ReferenceMaker(HEMAX_Plugin* Plugin);
	~HEMAX_ReferenceMaker();

	RefResult NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate);

	void AddEditableNodeToWatch(INode* Node);

    private:
	HEMAX_Plugin* ThePlugin;
};
