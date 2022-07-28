#pragma once

#include <custattrib.h>
#include <iparamb2.h>
#include "HEMAX_Types.h"

class HEMAX_3dsmaxHda;

#define HEMAX_INTEGER_PARAMETER_CLASS_ID Class_ID(0x499721d0, 0x78b55406)
#define HEMAX_STRING_PARAMETER_CLASS_ID Class_ID(0x692d7cf8, 0x59857abe)
#define HEMAX_FLOAT_PARAMETER_CLASS_ID Class_ID(0x1cc16359, 0x4f631900)
#define HEMAX_TOGGLE_PARAMETER_CLASS_ID Class_ID(0x733f2108, 0x7fe243e4)
#define HEMAX_NODE_PARAMETER_CLASS_ID Class_ID(0x25c53c03, 0x54841d80)
#define HEMAX_MULTIPARAMETER_CLASS_ID Class_ID(0xfab4b50, 0xb7076c)

#define HEMAX_PARAMETER_ATTRIB_NAME_LENGTH 900
#define HEMAX_PARAMETER_ATTRIB_NAME_CHUNK 1000

#define HEMAX_MAX_HOUDINI_STAMP_INDEX 0
const char* const HEMAX_MAX_HOUDINI_STAMP_NAME = "MaxHoudiniAssetStamp";
#define HEMAX_MAX_HOUDINI_ASSET_PATH_INDEX 1
const char* const HEMAX_MAX_HOUDINI_ASSET_PATH_NAME = "MaxHoudiniAssetPath";
#define HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_INDEX 2
const char* const HEMAX_MAX_HOUDINI_ASSET_LIBRARY_NUMBER_NAME = "MaxHoudiniAssetLibraryNumber";
#define HEMAX_MAX_HOUDINI_SAVE_CONVERSION_INDEX 3
const char* const HEMAX_MAX_HOUDINI_SAVE_CONVERSION_NAME = "MaxHoudiniSaveConversion";
#define HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_INDEX 4
const char* const HEMAX_MAX_HOUDINI_PUSH_TRANSFORM_NAME = "MaxHoudiniAssetPushTransformToHAPI";
#define HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_INDEX 5
const char* const HEMAX_MAX_HOUDINI_APPLY_HAPI_TRANSFORM_NAME = "MaxHoudiniAssetApplyHAPITransform";
#define HEMAX_MAX_HOUDINI_MAX_INDEX 5

#define HEMAX_HOUDINI_MODIFIER_STAMP_INDEX 0
const char* const HEMAX_HOUDINI_MODIFIER_STAMP_NAME = "MaxModifierAssetStamp";
#define HEMAX_HOUDINI_MODIFIER_ASSET_PATH_INDEX 1
const char* const HEMAX_HOUDINI_MODIFIER_ASSET_PATH_NAME = "MaxModifierAssetPath";
#define HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_INDEX 2
const char* const HEMAX_HOUDINI_MODIFIER_ASSET_LIBRARY_NUMBER_NAME = "MaxModifierAssetLibraryNumber";
#define HEMAX_HOUDINI_MODIFIER_MAX_INDEX 2

#define HEMAX_MAX_GEO_STAMP_INDEX 0
const char* const HEMAX_MAX_GEO_STAMP_NAME = "MaxHoudiniGeoStamp";
#define HEMAX_MAX_GEO_PART_STAMP_INDEX 1
const char* const HEMAX_MAX_GEO_PART_STAMP_NAME = "MaxHoudiniGeoPartNum";
#define HEMAX_MAX_GEO_MAX_INDEX 1

#define HEMAX_MAX_GEO_INSTANCE_STAMP_INDEX 0
const char* const HEMAX_MAX_GEO_INSTANCE_STAMP_NAME = "MaxHoudiniGeoInstanceStamp";
#define HEMAX_MAX_GEO_INSTANCE_MAX_INDEX 0

#define HEMAX_MAX_GEO_PACKED_PRIM_STAMP_INDEX 0
const char* const HEMAX_MAX_GEO_PACKED_PRIM_STAMP_NAME = "MaxHoudiniGeoPackedPrimStamp";
#define HEMAX_MAX_GEO_PACKED_PRIM_MAX_INDEX 0

#define HEMAX_EDITABLE_NODE_STAMP_INDEX 0
const char* const HEMAX_EDITABLE_NODE_STAMP_NAME = "MaxHoudiniEditableNodeStamp";
#define HEMAX_EDITABLE_NODE_NAME_INDEX 1
const char* const HEMAX_EDITABLE_NODE_NAME_NAME = "MaxHoudiniEditableNodeName";
#define HEMAX_EDITABLE_NODE_PART_NUM_INDEX 2
const char* const HEMAX_EDITABLE_NODE_PART_NUM_NAME = "MaxHoudiniEditableNodePartNum";
#define HEMAX_EDITABLE_NODE_MAX_INDEX 2

ClassDesc2* GetHEMAX_IntegerParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_StringParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_FloatParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_ToggleParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_NodeParameterAttrib_ClassDesc();
ClassDesc2* GetHEMAX_MultiParameterAttrib_ClassDesc();

class HEMAX_ParameterAttrib : public CustAttrib
{
    public:
	IParamBlock2* PBlock;

	std::string GetParameterName();

	void SetParameterName(std::string Name);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
        const TCHAR* GetName(bool Localized = false) override;
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

	IOResult Save(ISave* Save);
	IOResult Load(ILoad* Load);

    protected:
	std::string ParameterName;
	std::wstring WideParameterName;

	void SetReference(int i, RefTargetHandle RTarget);
};

class HEMAX_IntegerParameterAttrib : public HEMAX_ParameterAttrib
{
    public:
	HEMAX_IntegerParameterAttrib();

	Class_ID ClassID();

	RefResult NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate);

	int GetIntValue();
};

class HEMAX_IntegerParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
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

	RefResult NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate);

	void SetStringValue(std::string Value);
	std::string GetStringValue();

};

class HEMAX_StringParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
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

	RefResult NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate);

};

class HEMAX_FloatParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
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

	RefResult NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate);

};

class HEMAX_ToggleParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
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
	void CreateMaxHoudiniAssetLink(INode* Hda, HEMAX_InputType InputType, int Id);
	void UpdateOwner(INode* Hda);
	void SetMessagesBlocked(bool Block);
	Class_ID ClassID();
	RefResult NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate);

	void RefDeletedUndoRedo() override;

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
    defined(HEMAX_VERSION_2023)
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

	RefResult NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate);

};

class HEMAX_MultiParameterAttrib_ClassDesc : public ClassDesc2
{
    public:

	int IsPublic();
	void* Create(BOOL Loading);
#if defined(HEMAX_VERSION_2022) || \
    defined(HEMAX_VERSION_2023)
        const TCHAR* NonLocalizedClassName();
#endif
	const TCHAR* ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();
	const TCHAR* InternalName();
	HINSTANCE HInstance();
};

bool DoesCustomAttributeExist(ICustAttribContainer* CustomAttributeContainer, int CustomAttributeIndex, std::string CustomAttributeName);
CustAttrib* GetCustomAttributeByName(ICustAttribContainer* CustomAttributeContainer, std::string ParameterName);
HEMAX_StringParameterAttrib* GetStringParameterAttrib(ICustAttribContainer* CustomAttributeContainer, std::string ParameterName);
HEMAX_IntegerParameterAttrib* GetIntParameterAttrib(ICustAttribContainer* CustomAttributeContainer, std::string ParameterName);
bool CheckForCustomAttributeOnNode(INode* Node, std::string CustomAttributeName);
