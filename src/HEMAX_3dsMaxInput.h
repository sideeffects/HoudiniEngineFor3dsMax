#pragma once

#include "inode.h"
#include "ref.h"
#include <vector>
#include <string>
#include "HEMAX_Input.h"

struct HEMAX_InputUsage
{
    HEMAX_Node Node;
    int Subnetwork;
    std::string Parameter;
};

class HEMAX_3dsMaxInput : public ReferenceMaker
{
public:
    HEMAX_3dsMaxInput(INode* Node);
    ~HEMAX_3dsMaxInput();

    RefResult NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle HTarget, PartID& PartID, RefMessage Message, BOOL Propagate);

    ULONG Get3dsMaxNodeHandle();
    HEMAX_Input* GetInputNode();

    bool IsDirty();
    void UpdateInputNode();

    int GetInputCount();
    void IncrementInputCount();
    void DecrementInputCount();

    void AddSubnetworkUsage(HEMAX_Node Node, int Subnetwork);
    void AddParameterUsage(HEMAX_Node Node, std::string Parameter);

    void RemoveSubnetworkUsage(HEMAX_Node Node, int Subnetwork);
    void RemoveParameterUsage(HEMAX_Node Node, std::string Parameter);

    std::vector<HEMAX_InputUsage>& GetInputUsages();

private:
    INode* MaxNode;
    HEMAX_Input* InputNode;
    int InputCount;
    bool Dirty;

    std::vector<HEMAX_InputUsage> Usages;

    void CreateInputNode();
    void DeleteInputNode();
};