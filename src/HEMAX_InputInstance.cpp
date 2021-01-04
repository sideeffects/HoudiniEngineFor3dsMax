#include "HEMAX_InputInstance.h"

HEMAX_InputInstance::HEMAX_InputInstance()
    : MaxInput(nullptr), MergeNode(nullptr)
{
}

HEMAX_InputInstance::HEMAX_InputInstance(HEMAX_3dsMaxInput* Input, HEMAX_InputMerge* Merge)
    : MaxInput(Input), MergeNode(Merge)
{
}

HEMAX_InputInstance::~HEMAX_InputInstance()
{
    MaxInput = nullptr;
    MergeNode = nullptr;
}

    void
HEMAX_InputInstance::RefreshConnection()
{
    if (MergeNode && MaxInput)
    {
	MergeNode->RemoveAllMergedInputs();
	MergeNode->MergeInput(*(MaxInput->GetInputNode()));
    }
}
