#pragma once

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <maxapi.h>
#pragma warning(pop)

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
