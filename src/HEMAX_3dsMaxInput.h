#pragma once

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <inode.h>
#include <ref.h>
#pragma warning(pop)

#include <vector>
#include <string>

#include "HEMAX_Input.h"

struct HEMAX_InputUsage
{
    HEMAX_Node OwnerNode;
    int Subnetwork = -1;
    std::string ParameterName = "";

    bool operator==(const HEMAX_InputUsage& Other)
    {
        return (this->OwnerNode.Info.id == Other.OwnerNode.Info.id &&
                this->Subnetwork == Other.Subnetwork &&
                this->ParameterName == Other.ParameterName);
    }
    
    bool IsSubnetUsage() const { return Subnetwork > -1; }
    bool IsParameterUsage() const { return !ParameterName.empty(); }
};

class HEMAX_3dsMaxInput : public ReferenceMaker
{
    public:
	HEMAX_3dsMaxInput(INode* Node);
	~HEMAX_3dsMaxInput() = default;

	RefResult NotifyRefChanged(const Interval& ChangeInt,
            RefTargetHandle HTarget, PartID& PartID, RefMessage Message,
            BOOL Propagate);

	ULONG Get3dsMaxNodeHandle() const;
	HEMAX_Input* GetInputNode();

	bool IsDirty();
	void UpdateInputNode();

        int GetUsageCount();
        void AddUsage(const HEMAX_InputUsage& Usage);
        void RemoveUsage(const HEMAX_InputUsage& Usage);

	std::vector<HEMAX_InputUsage>& GetInputUsages();

    private:
	INode* MaxNode = nullptr;
        std::unique_ptr<HEMAX_Input> InputNode;
	bool Dirty = false;

	std::vector<HEMAX_InputUsage> Usages;

	void CreateInputNode();
        void DeleteInputNode();
};
