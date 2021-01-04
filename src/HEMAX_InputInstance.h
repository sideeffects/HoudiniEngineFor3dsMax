#pragma once

#include "HEMAX_3dsMaxInput.h"
#include "HEMAX_InputMerge.h"

struct HEMAX_InputInstance
{
    HEMAX_InputInstance();
    HEMAX_InputInstance(HEMAX_3dsMaxInput* Input, HEMAX_InputMerge* Merge);
    ~HEMAX_InputInstance();

    void RefreshConnection();

    HEMAX_3dsMaxInput* MaxInput;
    HEMAX_InputMerge* MergeNode;
};
