#pragma once

#include "HEMAX_3dsMaxInput.h"
#include "HEMAX_Node.h"
#include "HEMAX_Utilities.h"

#include <vector>

class HEMAX_InputInstance
{
public:
    HEMAX_InputInstance(const HEMAX_Node& Owner, int Subnetwork);
    HEMAX_InputInstance(const HEMAX_Node& Owner,
            const std::string& ParameterName);
    ~HEMAX_InputInstance();

    void MergeInput(HEMAX_3dsMaxInput* Input);
    void MergeInputRelativeTo(HEMAX_3dsMaxInput* Input,
                              const HEMAX_MaxTransform& MaxTransform);

    void RefreshConnection();

    HEMAX_Node& GetMergedInputs();

    void RemoveInput(HEMAX_3dsMaxInput* Input);

    const std::vector<HEMAX_3dsMaxInput*>& GetMaxInputs() const
        { return MaxInputs; }

    int GetMergedInputCount() const { return MergeCount; }

private:
    std::vector<HEMAX_3dsMaxInput*> MaxInputs;
    HEMAX_InputUsage InputUsage;
    HEMAX_Node MergeNode;
    int MergeCount = 0;
};
