#pragma once

#include "HEMAX_Input.h"
#include "HEMAX_Node.h"
#include "HEMAX_Utilities.h"

#define HEMAX_INPUT_MERGE_ASSET_NAME "Sop/merge"
#define HEMAX_INPUT_MERGE_XFORM_PARAM_NAME "xformtype"

class HEMAX_InputMerge
{
    public:
        HEMAX_InputMerge();
	~HEMAX_InputMerge();

	void MergeInput(HEMAX_Input& InputNode);
        void MergeInputRelativeTo(
                HEMAX_Input& InputNode,
                const HEMAX_MaxTransform& MaxTransform);
	void RemoveAllMergedInputs();

	HEMAX_Node& GetMergedInputs();

    private:
	HEMAX_Node Node;
	int MergeCount = 0;
};
