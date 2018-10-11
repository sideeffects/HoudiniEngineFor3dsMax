#pragma once

#include "HEMAX_Input.h"
#include "HEMAX_Node.h"
#include "HEMAX_Utilities.h"

#define HEMAX_INPUT_MERGE_ASSET_NAME "Sop/merge"
#define HEMAX_INPUT_MERGE_XFORM_PARAM_NAME "xformtype"

class HEMAX_InputMerge
{
public:
    HEMAX_InputMerge(HEMAX_MaxTransform Transform);
    ~HEMAX_InputMerge();

    void PushMaxTransform(HEMAX_MaxTransform NewTransform);

    void RemoveAllMergedInputs();
    void MergeInput(HEMAX_Input& InputNode);

    HEMAX_Node& GetMergedInputs();

private:
    HEMAX_Node Node;
    HEMAX_MaxTransform Transform;

    int MergeCount;
};