#include "HEMAX_CustomAttributeContainer.h"

#include "resource.h"
#include "icustattribcontainer.h"
#include "simpobj.h"
#include "HEMAX_3dsmaxHda.h"

extern HINSTANCE hInstance;

/////////////////////////////////////////////
////////////// STATIC ///////////////////////
/////////////////////////////////////////////

bool
DoesCustomAttributeExist(ICustAttribContainer* CustomAttributeContainer, int CustomAttributeIndex, std::string CustomAttributeName)
{
    if (CustomAttributeContainer)
    {
	if (CustomAttributeContainer->GetNumCustAttribs() > CustomAttributeIndex)
	{
	    CustAttrib* Attrib = CustomAttributeContainer->GetCustAttrib(CustomAttributeIndex);

	    if (Attrib)
	    {
		std::wstring WideName(Attrib->GetName());
		std::string AttribName(WideName.begin(), WideName.end());

		if (AttribName == CustomAttributeName)
		{
		    return true;
		}
	    }
	}

    }

    return false;
}

CustAttrib*
GetCustomAttributeByName(ICustAttribContainer* CustomAttributeContainer, std::string ParameterName)
{
    if (CustomAttributeContainer)
    {
	for (int i = 0; i < CustomAttributeContainer->GetNumCustAttribs(); ++i)
	{
	    CustAttrib* Attrib = CustomAttributeContainer->GetCustAttrib(i);

	    if (Attrib)
	    {
		std::wstring WideName(Attrib->GetName());
		std::string AttribName(WideName.begin(), WideName.end());

		if (AttribName == ParameterName)
		{
		    return Attrib;
		}
	    }
	}
    }

    return nullptr;
}

HEMAX_StringParameterAttrib*
GetStringParameterAttrib(ICustAttribContainer* CustomAttributeContainer, std::string ParameterName)
{
    if (!CustomAttributeContainer)
    {
	return nullptr;
    }

    CustAttrib* Attrib = GetCustomAttributeByName(CustomAttributeContainer, ParameterName);

    if (Attrib)
    {
	HEMAX_ParameterAttrib* ParmAttrib = dynamic_cast<HEMAX_ParameterAttrib*>(Attrib);
	if (ParmAttrib)
	{
	    ParamType2 ParamType = ParmAttrib->PBlock->GetParameterType(0);
	    if (ParamType == TYPE_STRING)
	    {
		HEMAX_StringParameterAttrib* StringAttrib = dynamic_cast<HEMAX_StringParameterAttrib*>(Attrib);
		return StringAttrib;
	    }
	}
    }

    return nullptr;
}

HEMAX_IntegerParameterAttrib*
GetIntParameterAttrib(ICustAttribContainer* CustomAttributeContainer, std::string ParameterName)
{
    if (!CustomAttributeContainer)
    {
	return nullptr;
    }

    CustAttrib* Attrib = GetCustomAttributeByName(CustomAttributeContainer, ParameterName);

    if (Attrib)
    {
	HEMAX_ParameterAttrib* ParmAttrib = dynamic_cast<HEMAX_ParameterAttrib*>(Attrib);
	if (ParmAttrib)
	{
	    ParamType2 ParamType = ParmAttrib->PBlock->GetParameterType(0);
	    if (ParamType == TYPE_INT)
	    {
		HEMAX_IntegerParameterAttrib* IntegerAttrib = dynamic_cast<HEMAX_IntegerParameterAttrib*>(Attrib);
		return IntegerAttrib;
	    }
	}
    }

    return nullptr;
}

bool
CheckForCustomAttributeOnNode(INode* Node, std::string CustomAttributeName)
{
    ICustAttribContainer* Container = Node->GetCustAttribContainer();

    if (Container)
    {
	if (GetCustomAttributeByName(Container, CustomAttributeName))
	{
	    return true;
	}
    }

    return false;
}

ClassDesc2*
GetHEMAX_IntegerParameterAttrib_ClassDesc() {
    static HEMAX_IntegerParameterAttrib_ClassDesc IntegerParameterAttribDesc;
    return &IntegerParameterAttribDesc;
}

ClassDesc2*
GetHEMAX_StringParameterAttrib_ClassDesc() {
    static HEMAX_StringParameterAttrib_ClassDesc StringParameterAttribDesc;
    return &StringParameterAttribDesc;
}

ClassDesc2*
GetHEMAX_FloatParameterAttrib_ClassDesc() {
    static HEMAX_FloatParameterAttrib_ClassDesc FloatParameterAttribDesc;
    return &FloatParameterAttribDesc;
}

ClassDesc2*
GetHEMAX_ToggleParameterAttrib_ClassDesc()
{
    static HEMAX_ToggleParameterAttrib_ClassDesc ToggleParameterAttribDesc;
    return &ToggleParameterAttribDesc;
}

ClassDesc2*
GetHEMAX_NodeParameterAttrib_ClassDesc()
{
    static HEMAX_NodeParameterAttrib_ClassDesc NodeParameterAttribDesc;
    return &NodeParameterAttribDesc;
}

ClassDesc2*
GetHEMAX_MultiParameterAttrib_ClassDesc()
{
    static HEMAX_MultiParameterAttrib_ClassDesc MultiParameterAttribDesc;
    return &MultiParameterAttribDesc;
}

/////////////////////////////////////////////
////////////// COMMON ///////////////////////
/////////////////////////////////////////////

std::string
HEMAX_ParameterAttrib::GetParameterName()
{
    return ParameterName;
}

void
HEMAX_ParameterAttrib::SetParameterName(std::string Name)
{
    ParameterName = Name;
    WideParameterName = std::wstring(ParameterName.begin(), ParameterName.end());
}

#ifdef HEMAX_VERSION_2022
const TCHAR*
HEMAX_ParameterAttrib::GetName(bool Localized)
{
    return WideParameterName.c_str();
}
#else
const TCHAR*
HEMAX_ParameterAttrib::GetName()
{
    return WideParameterName.c_str();
}
#endif

SClass_ID
HEMAX_ParameterAttrib::SuperClassID()
{
    return CUST_ATTRIB_CLASS_ID;
}

int
HEMAX_ParameterAttrib::NumRefs()
{
    return 1;
}

int
HEMAX_ParameterAttrib::NumSubs()
{
    return 1;
}

Animatable*
HEMAX_ParameterAttrib::SubAnim(int i)
{
    return PBlock;
}

RefTargetHandle
HEMAX_ParameterAttrib::GetReference(int i)
{
    if (i == 0)
    {
	return PBlock;
    }
    else
    {
	return nullptr;
    }
}

int
HEMAX_ParameterAttrib::NumParamBlocks()
{
    return 1;
}

IParamBlock2*
HEMAX_ParameterAttrib::GetParamBlock(int i)
{
    return PBlock;
}

IParamBlock2*
HEMAX_ParameterAttrib::GetParamBlockByID(BlockID Id)
{
    return (PBlock->ID() == Id) ? PBlock : nullptr;
}

void
HEMAX_ParameterAttrib::SetReference(int i, RefTargetHandle RTarget)
{
    if (i == 0)
    {
	PBlock = (IParamBlock2*)RTarget;
    }
}

IOResult
HEMAX_ParameterAttrib::Save(ISave* Save)
{
    Save->BeginChunk(HEMAX_PARAMETER_ATTRIB_NAME_CHUNK);
    Save->WriteCString(ParameterName.c_str());
    Save->EndChunk();

    return IO_OK;
}

IOResult
HEMAX_ParameterAttrib::Load(ILoad* Load)
{
    IOResult LoadResult;

    char** StringBuf = new char*;

    while (IO_OK == (LoadResult = Load->OpenChunk()))
    {
	switch (Load->CurChunkID())
	{
	    case HEMAX_PARAMETER_ATTRIB_NAME_CHUNK:
		{
		    LoadResult = Load->ReadCStringChunk(StringBuf);

		    if (LoadResult == IO_OK)
		    {
			ParameterName = *StringBuf;
		    }

		    Load->CloseChunk();
		    break;
		}
	    default:
		{
		    break;
		}
	}

	if (LoadResult != IO_OK)
	{
	    return LoadResult;
	}
    }

    delete StringBuf;

    WideParameterName = std::wstring(ParameterName.begin(), ParameterName.end());

    return IO_OK;
}

/////////////////////////////////////////////
////////////// INTEGER //////////////////////
/////////////////////////////////////////////

ParamBlockDesc2 IntegerParameterAttrib_ParamBlock(0, _T("IntegerParameter"), IDS_HEMAX_PARAMATTRIB_VALUE, GetHEMAX_IntegerParameterAttrib_ClassDesc(), P_AUTO_CONSTRUCT, 0,
	0, _T("Value"), TYPE_INT, P_ANIMATABLE, IDS_HEMAX_PARAMATTRIB_VALUE, p_end, p_end);

HEMAX_IntegerParameterAttrib::HEMAX_IntegerParameterAttrib()
{
    PBlock = nullptr;
    GetHEMAX_IntegerParameterAttrib_ClassDesc()->MakeAutoParamBlocks(this);
}

Class_ID
HEMAX_IntegerParameterAttrib::ClassID()
{
    return HEMAX_INTEGER_PARAMETER_CLASS_ID;
}

RefResult
HEMAX_IntegerParameterAttrib::NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate)
{
    return REF_SUCCEED;
}

int
HEMAX_IntegerParameterAttrib::GetIntValue()
{
    if (PBlock)
    {
	int Value;
	PBlock->GetValue(0, GetCOREInterface()->GetTime(), Value, FOREVER);
	return Value;
    }

    return -1;
}

int
HEMAX_IntegerParameterAttrib_ClassDesc::IsPublic()
{
    return 1;
}

void*
HEMAX_IntegerParameterAttrib_ClassDesc::Create(BOOL Loading)
{
    return new HEMAX_IntegerParameterAttrib;
}

#ifdef HEMAX_VERSION_2022
const TCHAR*
HEMAX_IntegerParameterAttrib_ClassDesc::NonLocalizedClassName()
{
    return _T("HEMAX_IntegerParameterAttrib_Class");
}
#endif

const TCHAR*
HEMAX_IntegerParameterAttrib_ClassDesc::ClassName()
{
    return _T("HEMAX_IntegerParameterAttrib_Class");
}

SClass_ID
HEMAX_IntegerParameterAttrib_ClassDesc::SuperClassID()
{
    return CUST_ATTRIB_CLASS_ID;
}

Class_ID
HEMAX_IntegerParameterAttrib_ClassDesc::ClassID()
{
    return HEMAX_INTEGER_PARAMETER_CLASS_ID;
}

const TCHAR*
HEMAX_IntegerParameterAttrib_ClassDesc::Category()
{
    return _T("HEMAX_CustAttrib");
}

const TCHAR*
HEMAX_IntegerParameterAttrib_ClassDesc::InternalName()
{
    return _T("HEMAX_IntegerParameterAttrib");
}

HINSTANCE
HEMAX_IntegerParameterAttrib_ClassDesc::HInstance()
{
    return hInstance;
}

/////////////////////////////////////////////
////////////// STRING ///////////////////////
/////////////////////////////////////////////

ParamBlockDesc2 StringParameterAttrib_ParamBlock(0, _T("StringParameter"), IDS_HEMAX_PARAMATTRIB_VALUE, GetHEMAX_StringParameterAttrib_ClassDesc(), P_AUTO_CONSTRUCT, 0,
	0, _T("Value"), TYPE_STRING, P_ANIMATABLE, IDS_HEMAX_PARAMATTRIB_VALUE, p_end, p_end);

HEMAX_StringParameterAttrib::HEMAX_StringParameterAttrib()
{
    PBlock = nullptr;
    GetHEMAX_StringParameterAttrib_ClassDesc()->MakeAutoParamBlocks(this);
}

Class_ID
HEMAX_StringParameterAttrib::ClassID()
{
    return HEMAX_STRING_PARAMETER_CLASS_ID;
}

RefResult
HEMAX_StringParameterAttrib::NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate)
{
    return REF_SUCCEED;
}

void
HEMAX_StringParameterAttrib::SetStringValue(std::string Value)
{
    if (PBlock)
    {
	std::wstring WideValue(Value.begin(), Value.end());
	PBlock->SetValue(0, GetCOREInterface()->GetTime(), WideValue.c_str());
    }
}

std::string
HEMAX_StringParameterAttrib::GetStringValue()
{
    if (PBlock)
    {
	const MCHAR* Name;
	PBlock->GetValue(0, GetCOREInterface()->GetTime(), Name, FOREVER);
	std::wstring WideName(Name);
	return std::string(WideName.begin(), WideName.end());
    }

    return "";
}

int
HEMAX_StringParameterAttrib_ClassDesc::IsPublic()
{
    return 1;
}

void*
HEMAX_StringParameterAttrib_ClassDesc::Create(BOOL Loading)
{
    return new HEMAX_StringParameterAttrib;
}

#ifdef HEMAX_VERSION_2022
const TCHAR*
HEMAX_StringParameterAttrib_ClassDesc::NonLocalizedClassName()
{
    return L"HEMAX_StringParameterAttrib_Class";
}
#endif

const TCHAR*
HEMAX_StringParameterAttrib_ClassDesc::ClassName()
{
    return L"HEMAX_StringParameterAttrib_Class";
}

SClass_ID
HEMAX_StringParameterAttrib_ClassDesc::SuperClassID()
{
    return CUST_ATTRIB_CLASS_ID;
}

Class_ID
HEMAX_StringParameterAttrib_ClassDesc::ClassID()
{
    return HEMAX_STRING_PARAMETER_CLASS_ID;
}

const TCHAR*
HEMAX_StringParameterAttrib_ClassDesc::Category()
{
    return _T("HEMAX_CustAttrib");
}

const TCHAR*
HEMAX_StringParameterAttrib_ClassDesc::InternalName()
{
    return L"HEMAX_StringParameterAttrib";
}

HINSTANCE
HEMAX_StringParameterAttrib_ClassDesc::HInstance()
{
    return hInstance;
}

/////////////////////////////////////////////
////////////// FLOAT ////////////////////////
/////////////////////////////////////////////

ParamBlockDesc2 FloatParameterAttrib_ParamBlock(0, _T("FloatParameter"), IDS_HEMAX_PARAMATTRIB_VALUE, GetHEMAX_FloatParameterAttrib_ClassDesc(), P_AUTO_CONSTRUCT, 0,
	0, _T("Value"), TYPE_FLOAT, P_ANIMATABLE, IDS_HEMAX_PARAMATTRIB_VALUE, p_end, p_end);

HEMAX_FloatParameterAttrib::HEMAX_FloatParameterAttrib()
{
    PBlock = nullptr;
    GetHEMAX_FloatParameterAttrib_ClassDesc()->MakeAutoParamBlocks(this);
}

Class_ID
HEMAX_FloatParameterAttrib::ClassID()
{
    return HEMAX_FLOAT_PARAMETER_CLASS_ID;
}

RefResult
HEMAX_FloatParameterAttrib::NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate)
{
    return REF_SUCCEED;
}

int
HEMAX_FloatParameterAttrib_ClassDesc::IsPublic()
{
    return 1;
}

void*
HEMAX_FloatParameterAttrib_ClassDesc::Create(BOOL Loading)
{
    return new HEMAX_FloatParameterAttrib;
}

#ifdef HEMAX_VERSION_2022
const TCHAR*
HEMAX_FloatParameterAttrib_ClassDesc::NonLocalizedClassName()
{
    return L"HEMAX_FloatParameterAttrib_Class";
}
#endif

const TCHAR*
HEMAX_FloatParameterAttrib_ClassDesc::ClassName()
{
    return L"HEMAX_FloatParameterAttrib_Class";
}

SClass_ID
HEMAX_FloatParameterAttrib_ClassDesc::SuperClassID()
{
    return CUST_ATTRIB_CLASS_ID;
}

Class_ID
HEMAX_FloatParameterAttrib_ClassDesc::ClassID()
{
    return HEMAX_FLOAT_PARAMETER_CLASS_ID;
}

const TCHAR*
HEMAX_FloatParameterAttrib_ClassDesc::Category()
{
    return _T("HEMAX_CustAttrib");
}

const TCHAR*
HEMAX_FloatParameterAttrib_ClassDesc::InternalName()
{
    return L"HEMAX_FloatParameterAttrib";
}

HINSTANCE
HEMAX_FloatParameterAttrib_ClassDesc::HInstance()
{
    return hInstance;
}

/////////////////////////////////////////////
////////////// TOGGLE ///////////////////////
/////////////////////////////////////////////

ParamBlockDesc2 ToggleParameterAttrib_ParamBlock(0, _T("ToggleParameter"), IDS_HEMAX_PARAMATTRIB_VALUE, GetHEMAX_ToggleParameterAttrib_ClassDesc(), P_AUTO_CONSTRUCT, 0,
	0, _T("Value"), TYPE_BOOL, P_ANIMATABLE, IDS_HEMAX_PARAMATTRIB_VALUE, p_end, p_end);

HEMAX_ToggleParameterAttrib::HEMAX_ToggleParameterAttrib()
{
    PBlock = nullptr;
    GetHEMAX_ToggleParameterAttrib_ClassDesc()->MakeAutoParamBlocks(this);
}

Class_ID
HEMAX_ToggleParameterAttrib::ClassID()
{
    return HEMAX_TOGGLE_PARAMETER_CLASS_ID;
}

RefResult
HEMAX_ToggleParameterAttrib::NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate)
{
    return REF_SUCCEED;
}

int
HEMAX_ToggleParameterAttrib_ClassDesc::IsPublic()
{
    return 1;
}

void*
HEMAX_ToggleParameterAttrib_ClassDesc::Create(BOOL Loading)
{
    return new HEMAX_ToggleParameterAttrib;
}

#ifdef HEMAX_VERSION_2022
const TCHAR*
HEMAX_ToggleParameterAttrib_ClassDesc::NonLocalizedClassName()
{
    return L"HEMAX_ToggleParameterAttrib_Class";
}
#endif

const TCHAR*
HEMAX_ToggleParameterAttrib_ClassDesc::ClassName()
{
    return L"HEMAX_ToggleParameterAttrib_Class";
}

SClass_ID
HEMAX_ToggleParameterAttrib_ClassDesc::SuperClassID()
{
    return CUST_ATTRIB_CLASS_ID;
}

Class_ID
HEMAX_ToggleParameterAttrib_ClassDesc::ClassID()
{
    return HEMAX_TOGGLE_PARAMETER_CLASS_ID;
}

const TCHAR*
HEMAX_ToggleParameterAttrib_ClassDesc::Category()
{
    return _T("HEMAX_CustAttrib");
}

const TCHAR*
HEMAX_ToggleParameterAttrib_ClassDesc::InternalName()
{
    return L"HEMAX_ToggleParameterAttrib";
}

HINSTANCE
HEMAX_ToggleParameterAttrib_ClassDesc::HInstance()
{
    return hInstance;
}

/////////////////////////////////////////////
////////////// NODE /////////////////////////
/////////////////////////////////////////////

ParamBlockDesc2 NodeParameterAttrib_ParamBlock(0, _T("NodeParameter"), IDS_HEMAX_PARAMATTRIB_VALUE, GetHEMAX_NodeParameterAttrib_ClassDesc(), P_AUTO_CONSTRUCT, 0,
	0, _T("Value"), TYPE_INODE, P_SUBANIM, IDS_HEMAX_PARAMATTRIB_VALUE, p_end, p_end);

HEMAX_NodeParameterAttrib::HEMAX_NodeParameterAttrib()
{
    PBlock = nullptr;
    GetHEMAX_NodeParameterAttrib_ClassDesc()->MakeAutoParamBlocks(this);
    Owner = nullptr;
    MessagesBlocked = false;
}

void
HEMAX_NodeParameterAttrib::CreateMaxHoudiniAssetLink(INode* Hda, HEMAX_InputType InputType, int Id)
{
    Owner = Hda;
    this->InputType = InputType;

    if (InputType == HEMAX_INPUT_PARAMETER)
    {
	ParameterId = Id;
	Subnetwork = -1;
    }
    else if (InputType == HEMAX_INPUT_SUBNETWORK)
    {
	ParameterId = -1;
	Subnetwork = Id;
    }
}

void
HEMAX_NodeParameterAttrib::UpdateOwner(INode* Hda)
{
    Owner = Hda;
}

void
HEMAX_NodeParameterAttrib::SetMessagesBlocked(bool Block)
{
    MessagesBlocked = Block;
}

Class_ID
HEMAX_NodeParameterAttrib::ClassID()
{
    return HEMAX_NODE_PARAMETER_CLASS_ID;
}

RefResult
HEMAX_NodeParameterAttrib::NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate)
{
    return REF_SUCCEED;
}

void
HEMAX_NodeParameterAttrib::RefDeletedUndoRedo()
{
    Owner = nullptr;
}

int
HEMAX_NodeParameterAttrib_ClassDesc::IsPublic()
{
    return 1;
}

void*
HEMAX_NodeParameterAttrib_ClassDesc::Create(BOOL Loading)
{
    return new HEMAX_NodeParameterAttrib;
}

#ifdef HEMAX_VERSION_2022
const TCHAR*
HEMAX_NodeParameterAttrib_ClassDesc::NonLocalizedClassName()
{
    return L"HEMAX_NodeParameterAttrib_Class";
}
#endif

const TCHAR*
HEMAX_NodeParameterAttrib_ClassDesc::ClassName()
{
    return L"HEMAX_NodeParameterAttrib_Class";
}

SClass_ID
HEMAX_NodeParameterAttrib_ClassDesc::SuperClassID()
{
    return CUST_ATTRIB_CLASS_ID;
}

Class_ID
HEMAX_NodeParameterAttrib_ClassDesc::ClassID()
{
    return HEMAX_NODE_PARAMETER_CLASS_ID;
}

const TCHAR*
HEMAX_NodeParameterAttrib_ClassDesc::Category()
{
    return _T("HEMAX_CustAttrib");
}

const TCHAR*
HEMAX_NodeParameterAttrib_ClassDesc::InternalName()
{
    return L"HEMAX_NodeParameterAttrib";
}

HINSTANCE
HEMAX_NodeParameterAttrib_ClassDesc::HInstance()
{
    return hInstance;
}

/////////////////////////////////////////////
////////////// MULTI-PARAMETER //////////////
/////////////////////////////////////////////

ParamBlockDesc2 MultiParameterAttrib_ParamBlock(0, _T("MultiParameter"), IDS_HEMAX_PARAMATTRIB_VALUE, GetHEMAX_MultiParameterAttrib_ClassDesc(), P_AUTO_CONSTRUCT, 0,
	0, _T("InstanceCount"), TYPE_INT, P_INVISIBLE, IDS_HEMAX_PARAMATTRIB_VALUE, p_end,
	1, _T("InstanceLength"), TYPE_INT, P_INVISIBLE, IDS_HEMAX_PARAMATTRIB_VALUE, p_end, p_end);

HEMAX_MultiParameterAttrib::HEMAX_MultiParameterAttrib()
{
    PBlock = nullptr;
    GetHEMAX_MultiParameterAttrib_ClassDesc()->MakeAutoParamBlocks(this);
}

Class_ID
HEMAX_MultiParameterAttrib::ClassID()
{
    return HEMAX_MULTIPARAMETER_CLASS_ID;
}

RefResult
HEMAX_MultiParameterAttrib::NotifyRefChanged(const Interval& ChangeInt, RefTargetHandle hTarget, PartID& PartID, RefMessage Message, BOOL Propagate)
{
    return REF_SUCCEED;
}

int
HEMAX_MultiParameterAttrib_ClassDesc::IsPublic()
{
    return 1;
}

void*
HEMAX_MultiParameterAttrib_ClassDesc::Create(BOOL Loading)
{
    return new HEMAX_MultiParameterAttrib;
}

#ifdef HEMAX_VERSION_2022
const TCHAR*
HEMAX_MultiParameterAttrib_ClassDesc::NonLocalizedClassName()
{
    return _T("HEMAX_MultiParameterAttrib_Class");
}
#endif

const TCHAR*
HEMAX_MultiParameterAttrib_ClassDesc::ClassName()
{
    return _T("HEMAX_MultiParameterAttrib_Class");
}

SClass_ID
HEMAX_MultiParameterAttrib_ClassDesc::SuperClassID()
{
    return CUST_ATTRIB_CLASS_ID;
}

Class_ID
HEMAX_MultiParameterAttrib_ClassDesc::ClassID()
{
    return HEMAX_MULTIPARAMETER_CLASS_ID;
}

const TCHAR*
HEMAX_MultiParameterAttrib_ClassDesc::Category()
{
    return _T("HEMAX_CustAttrib");
}

const TCHAR*
HEMAX_MultiParameterAttrib_ClassDesc::InternalName()
{
    return _T("HEMAX_MultiParameterAttrib");
}

HINSTANCE
HEMAX_MultiParameterAttrib_ClassDesc::HInstance()
{
    return hInstance;
}
