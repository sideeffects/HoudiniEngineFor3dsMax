#pragma once

#include "HEMAX_3dsMaxInput.h"
#include "HEMAX_InputMerge.h"

#include <vector>

struct HEMAX_InputInstance
{
    HEMAX_InputInstance(
            const std::vector<HEMAX_3dsMaxInput*>& Inputs,
            HEMAX_InputMerge* Merge)
        : MergeNode(Merge)
    {
        for (auto&& Input : Inputs)
            MaxInputs.push_back(Input);
    }

    ~HEMAX_InputInstance()
    {
    }

    void RefreshConnection()
    {
        if (MergeNode)
        {
            MergeNode->RemoveAllMergedInputs();

            for (auto&& Input : MaxInputs)
            {
                if (!Input)
                    continue;

                MergeNode->MergeInput(*(Input->GetInputNode()));    
            }
        }
    }

    std::vector<HEMAX_3dsMaxInput*> MaxInputs;
    HEMAX_InputMerge* MergeNode = nullptr;
};
