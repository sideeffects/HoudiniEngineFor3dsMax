#pragma once

#include <istdplug.h>
#include <iparamb2.h>
#include <maxtypes.h>
#include <simpmod.h>

#include "HEMAX_Node.h"
#include "UI/HEMAX_UI.h"
#include "HEMAX_Plugin.h"
#include "HEMAX_Input_Geometry.h"

#define HEMAX_Modifier_CLASS_ID Class_ID(0x53e0677, 0x51fb0852)

class HEMAX_UI;
class HEMAX_Plugin;

class HEMAX_Modifier : public Modifier
{
public:
    INode* MaxNode;

    HEMAX_Node OutputNode;

    HEMAX_Modifier();
    ~HEMAX_Modifier();
    void DeleteThis();

    void CleanupHAPIComponents();
    void ClearHAPIReferences();
    void DeleteInputNode();

    void RegisterPlugin(HEMAX_Plugin* Plugin);
    void BeginEditParams(IObjParam  *IP, ULONG Flags, Animatable *Prev);
    void EndEditParams(IObjParam *IP, ULONG Flags, Animatable *Prev);

    void SetAssetName(std::string Name);

    virtual Class_ID ClassID() override;

    void ModifyObject(TimeValue t, ModContext& mc, ObjectState* os, INode* node) override;

    void ApplyMaterialsToNode();

    Class_ID InputType();
    ChannelMask ChannelsUsed();
    ChannelMask ChannelsChanged();

    CreateMouseCallBack* GetCreateMouseCallBack() { return nullptr; }

    RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget,
        PartID& partID, RefMessage message, BOOL propagate);

    const TCHAR* GetObjectName();

    void MarkCreated();
    void MarkUndone();

    void NotifyPreCollapse(INode* TheNode, IDerivedObject* DerivedObject, int Index) override;

    ReferenceTarget* Clone(RemapDir& Remap) override;
    void BaseClone(ReferenceTarget* From, ReferenceTarget* To, RemapDir& Remap) override;

    bool CheckIfTopOfStack(INode* ContainerNode);

private:

    HEMAX_Mesh* TheMesh;

    HEMAX_Input* InputNode;

    bool HasBeenCreated;
    HEMAX_Plugin* ThePlugin;

    bool IsUndone;

    std::string AssetName;
    std::wstring WideAssetName;
};

class HEMAX_ModifierClassDesc : public ClassDesc2
{
public:
    int IsPublic();
    void* Create(BOOL Loading = false);
    const TCHAR* ClassName();
    SClass_ID SuperClassID();
    Class_ID ClassID();
    const TCHAR* Category();
};
