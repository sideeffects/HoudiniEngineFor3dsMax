#pragma once

#pragma warning(push, 0)
#pragma warning(disable : 4265 4700 4715 4717 4263 4266 4390 4407)
#include <custattrib.h>
#include <iparamb2.h>
#pragma warning(pop)

#include "HEMAX_Types.h"

class HEMAX_3dsmaxHda;

#define HEMAX_INTEGER_PARAMETER_CLASS_ID Class_ID(0x499721d0, 0x78b55406)
#define HEMAX_STRING_PARAMETER_CLASS_ID Class_ID(0x692d7cf8, 0x59857abe)
#define HEMAX_FLOAT_PARAMETER_CLASS_ID Class_ID(0x1cc16359, 0x4f631900)
#define HEMAX_TOGGLE_PARAMETER_CLASS_ID Class_ID(0x733f2108, 0x7fe243e4)
#define HEMAX_NODE_PARAMETER_CLASS_ID Class_ID(0x25c53c03, 0x54841d80)
#define HEMAX_NODELIST_PARAMETER_CLASS_ID Class_ID(0x38ea47c6, 0x588a36df)
#define HEMAX_MULTIPARAMETER_CLASS_ID Class_ID(0xfab4b50, 0xb7076c)

#define HEMAX_PARAMETER_ATTRIB_NAME_LENGTH 900
#define HEMAX_PARAMETER_ATTRIB_NAME_CHUNK 1000

#define HEMAX_MAX_HOUDINI_STAMP_INDEX 0
const TSTR HEMAX_MAX_HOUDINI_STAMP_NAME(_T("MaxHoudiniAssetStamp"));
#define HEMAX_MAX_HOUDINI_ASSET_PATH_INDEX 1
const TSTR HEMAX_MAX_HOUDINI_ASSET_PATH_NAME(_T("MaxHoudiniAssetPath"));
#define HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_INDEX 2
const TSTR HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_NAME(_T("MaxHoudiniAssetLibraryNumber"));
#define HEMAX_MAX_HOUDINI_SAVE_CONVERSION_INDEX 3
const TSTR HEMAX_MAX_HOUDINI_SAVE_CONVERSION_NAME(_T("MaxHoudiniSaveConversion"));
#define HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_INDEX 4
const TSTR HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_NAME(_T("MaxHoudiniAssetPushTransformToHAPI"));
#define HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_INDEX 5
const TSTR HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_NAME(_T("MaxHoudiniAssetApplyHAPITransform"));
#define HEMAX_MAX_HOUDINI_MAX_INDEX 5

#define HEMAX_HOUDINI_MODIFIER_STAMP_INDEX 0
const TSTR HEMAX_HOUDINI_MODIFIER_STAMP_NAME(_T("MaxModifierAssetStamp"));
#define HEMAX_HOUDINI_MODIFIER_ASSET_PATH_INDEX 1
const TSTR HEMAX_HOUDINI_MODIFIER_ASSET_PATH_NAME(_T("MaxModifierAssetPath"));
#define HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_INDEX 2
const TSTR HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_NAME(_T("MaxModifierAssetLibraryNumber"));
#define HEMAX_HOUDINI_MODIFIER_MAX_INDEX 2

#define HEMAX_MAX_GEO_STAMP_INDEX 0
const TSTR HEMAX_MAX_GEO_STAMP_NAME(_T("MaxHoudiniGeoStamp"));
#define HEMAX_MAX_GEO_PART_STAMP_INDEX 1
const TSTR HEMAX_MAX_GEO_PART_STAMP_NAME(_T("MaxHoudiniGeoPartNum"));
#define HEMAX_MAX_GEO_MAX_INDEX 1

#define HEMAX_MAX_GEO_INSTANCE_STAMP_INDEX 0
const TSTR HEMAX_MAX_GEO_INSTANCE_STAMP_NAME(_T("MaxHoudiniGeoInstanceStamp"));
#define HEMAX_MAX_GEO_INSTANCE_MAX_INDEX 0

#define HEMAX_MAX_GEO_PACKED_PRIM_STAMP_INDEX 0
const TSTR HEMAX_MAX_GEO_PACKED_PRIM_STAMP_NAME(_T("MaxHoudiniGeoPackedPrimStamp"));
#define HEMAX_MAX_GEO_PACKED_PRIM_MAX_INDEX 0

#define HEMAX_EDITABLE_NODE_STAMP_INDEX 0
const TSTR HEMAX_EDITABLE_NODE_STAMP_NAME(_T("MaxHoudiniEditableNodeStamp"));
#define HEMAX_EDITABLE_NODE_NAME_INDEX 1
const TSTR HEMAX_EDITABLE_NODE_NAME_NAME(_T("MaxHoudiniEditableNodeName"));
#define HEMAX_EDITABLE_NODE_PART_NUM_INDEX 2
const TSTR HEMAX_EDITABLE_NODE_PART_NUM_NAME(_T("MaxHoudiniEditableNodePartNum"));
#define HEMAX_EDITABLE_NODE_MAX_INDEX 2

ClassDesc2* GetHEMAX_IntegerParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_StringParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_FloatParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_ToggleParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_NodeParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_NodeListParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_MultiParameterAttrib_ClassDesc();

class HEMAX_ParameterAttrib : public CustAttrib
{
    public:
	IParamBlock2* PBlock;

	const TSTR& GetParameterName() const;
	void SetParameterName(const TSTR& Name);

#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        const TCHAR* GetName(bool Localized) override;
#else
        const TCHAR* GetName() override;
#endif

	SClass_ID SuperClassID();

	int NumRefs();
	int NumSubs();
	Animatable* SubAnim(int i);

	RefTargetHandle GetReference(int i);

	int NumParamBlocks();
	IParamBlock2* GetParamBlock(int i);
	IParamBlock2* GetParamBlockByID(BlockID Id);

	IOResult Save(ISave* Save) override;
#if !defined(HEMAX_VERSION_2017) && \
    !defined(HEMAX_VERSION_2018) && \
    !defined(HEMAX_VERSION_2019) && \
    !defined(HEMAX_VERSION_2020) && \
    !defined(HEMAX_VERSION_2021) && \
    !defined(HEMAX_VERSION_2022) && \
    !defined(HEMAX_VERSION_2023)
        IOResult Save(ISave* Save, ChannelMask) override;
#endif

	IOResult Load(ILoad* Load) override;
#if !defined(HEMAX_VERSION_2017) && \
    !defined(HEMAX_VERSION_2018) && \
    !defined(HEMAX_VERSION_2019) && \
    !defined(HEMAX_VERSION_2020) && \
    !defined(HEMAX_VERSION_2021) && \
    !defined(HEMAX_VERSION_2022) && \
    !defined(HEMAX_VERSION_2023)
        IOResult Load(ILoad* Load, ChannelMask) override;
#endif

    protected:
        TSTR ParameterName;

	void SetReference(int i, RefTargetHandle RTarget);
};

class HEMAX_IntegerParameterAttrib : public HEMAX_ParameterAttrib
{
    public:
	HEMAX_IntegerParameterAttrib();

	Class_ID ClassID();

	RefResult NotifyRefChanged(const Interval& ChangeInt,
            RefTargetHandle hTarget, PartID& PartID, RefMessage Message,
            BOOL Propagate);

	int GetIntValue();
};

class HEMAX_IntegerParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        const TCHAR* NonLocalizedClassName();
#endif
	const TCHAR* ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();
	const TCHAR* InternalName();
	HINSTANCE HInstance();
};

class HEMAX_StringParameterAttrib : public HEMAX_ParameterAttrib
{
    public:

	HEMAX_StringParameterAttrib();

	Class_ID ClassID();

	RefResult NotifyRefChanged(const Interval& ChangeInt,
            RefTargetHandle hTarget, PartID& PartID, RefMessage Message,
            BOOL Propagate);

	void SetStringValue(std::string Value);
	std::string GetStringValue();

};

class HEMAX_StringParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        const TCHAR* NonLocalizedClassName();
#endif
	const TCHAR* ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();
	const TCHAR* InternalName();
	HINSTANCE HInstance();
};

class HEMAX_FloatParameterAttrib : public HEMAX_ParameterAttrib
{
    public:

	HEMAX_FloatParameterAttrib();

	Class_ID ClassID();

	RefResult NotifyRefChanged(const Interval& ChangeInt,
            RefTargetHandle hTarget, PartID& PartID, RefMessage Message,
            BOOL Propagate);

};

class HEMAX_FloatParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        const TCHAR* NonLocalizedClassName();
#endif
	const TCHAR* ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();
	const TCHAR* InternalName();
	HINSTANCE HInstance();
};

class HEMAX_ToggleParameterAttrib : public HEMAX_ParameterAttrib
{
    public:

	HEMAX_ToggleParameterAttrib();

	Class_ID ClassID();

	RefResult NotifyRefChanged(const Interval& ChangeInt,
            RefTargetHandle hTarget, PartID& PartID, RefMessage Message,
            BOOL Propagate);

};

class HEMAX_ToggleParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        const TCHAR* NonLocalizedClassName();
#endif
	const TCHAR* ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();
	const TCHAR* InternalName();
	HINSTANCE HInstance();
};

class HEMAX_NodeParameterAttrib : public HEMAX_ParameterAttrib
{
    public:

	HEMAX_NodeParameterAttrib();
	void CreateMaxHoudiniAssetLink(
            INode* Hda, HEMAX_InputType InputType, int Id);
	void UpdateOwner(INode* Hda);
	void SetMessagesBlocked(bool Block);
	Class_ID ClassID();
	RefResult NotifyRefChanged(const Interval& ChangeInt,
            RefTargetHandle hTarget, PartID& PartID, RefMessage Message,
            BOOL Propagate);

#if defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        void RefDeletedUndoRedo(RefMakerHandle oldOwner) override;
#else
	void RefDeletedUndoRedo() override;
#endif

    private:
	INode* Owner;
	HEMAX_InputType InputType;
	int ParameterId;
	int Subnetwork;
	bool MessagesBlocked;

};

class HEMAX_NodeParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        const TCHAR* NonLocalizedClassName();
#endif
	const TCHAR* ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();
	const TCHAR* InternalName();
	HINSTANCE HInstance();
};

class HEMAX_NodeListParameterAttrib : public HEMAX_ParameterAttrib
{
    public:

	HEMAX_NodeListParameterAttrib();
	void CreateMaxHoudiniAssetLink(INode* Hda, HEMAX_InputType InputType, int Id);
	void UpdateOwner(INode* Hda);
	void SetMessagesBlocked(bool Block);
	Class_ID ClassID();
	RefResult NotifyRefChanged(const Interval& ChangeInt,
            RefTargetHandle hTarget, PartID& PartID, RefMessage Message,
            BOOL Propagate);

#if defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        void RefDeletedUndoRedo(RefMakerHandle oldOwner) override;
#else
	void RefDeletedUndoRedo() override;
#endif

    private:
	INode* Owner;
	HEMAX_InputType InputType;
	int ParameterId;
	int Subnetwork;
	bool MessagesBlocked;

};

class HEMAX_NodeListParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        const TCHAR* NonLocalizedClassName();
#endif
	const TCHAR* ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();
	const TCHAR* InternalName();
	HINSTANCE HInstance();
};

class HEMAX_MultiParameterAttrib : public HEMAX_ParameterAttrib
{
    public:
	HEMAX_MultiParameterAttrib();

	Class_ID ClassID();

	RefResult NotifyRefChanged(const Interval& ChangeInt,
            RefTargetHandle hTarget, PartID& PartID, RefMessage Message,
            BOOL Propagate);

};

class HEMAX_MultiParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023) || \
    defined(HEMAX_VERSION_2024) || \
    defined(HEMAX_VERSION_2025) || \
    defined(HEMAX_VERSION_2026) || \
    defined(HEMAX_VERSION_2027)
        const TCHAR* NonLocalizedClassName();
#endif
	const TCHAR* ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();
	const TCHAR* InternalName();
	HINSTANCE HInstance();
};

bool
DoesCustomAttributeExist(ICustAttribContainer* CustomAttributeContainer,
        int CustomAttributeIndex, const TSTR& CustomAttributeName);

CustAttrib*
GetCustomAttributeByName(ICustAttribContainer* CustomAttributeContainer,
        const TSTR& ParameterName);

HEMAX_StringParameterAttrib*
GetStringParameterAttrib(ICustAttribContainer* CustomAttributeContainer,
        const TSTR& ParameterName);

HEMAX_IntegerParameterAttrib*
GetIntParameterAttrib(ICustAttribContainer* CustomAttributeContainer,
        const TSTR& ParameterName);

bool
CheckForCustomAttributeOnNode(INode* Node, const TSTR& CustomAttributeName);
