#include "HEMAX_MaxScriptInterface.h"

#include <maxscript/maxscript.h>
#include <maxscript/macros/define_instantiation_functions.h>

#include "HEMAX_Plugin.h"

HEMAX_Plugin* HEMAX_MaxScriptInterface::PluginInstance;

def_visible_primitive(CreateGeometryHda, "CreateGeometryHda");
def_visible_primitive(CreateModifierHda, "CreateModifierHda");
def_visible_primitive(CookHda, "CookHda");
def_visible_primitive(SetGeometryHdaParameter, "SetGeometryHdaParameter");
def_visible_primitive(SetModifierHdaParameter, "SetModifierHdaParameter");

Value*
CreateGeometryHda_cf(Value** ArgList, int Count)
{
	if (Count == 1)
	{
		std::wstring WAssetPath(ArgList[0]->to_string());
		std::string AssetPath(WAssetPath.begin(), WAssetPath.end());

		HEMAX_MaxScriptInterface::CreateGeometryHda(AssetPath, 0);
	}
	else if (Count == 2)
	{
		std::wstring WAssetPath(ArgList[0]->to_string());
		std::string AssetPath(WAssetPath.begin(), WAssetPath.end());

		int AssetIndex = ArgList[1]->to_int();
		HEMAX_MaxScriptInterface::CreateGeometryHda(AssetPath, AssetIndex);
	}

	return &ok;
}

Value*
CreateModifierHda_cf(Value** ArgList, int Count)
{
	if (Count == 2)
	{
		INode* Node = ArgList[0]->to_node();
		std::wstring WAssetPath(ArgList[1]->to_string());
		std::string AssetPath(WAssetPath.begin(), WAssetPath.end());

		HEMAX_MaxScriptInterface::CreateModifierHda(Node, AssetPath, 0);
	}
	else if (Count == 3)
	{
		INode* Node = ArgList[0]->to_node();
		std::wstring WAssetPath(ArgList[1]->to_string());
		std::string AssetPath(WAssetPath.begin(), WAssetPath.end());

		int AssetIndex = ArgList[2]->to_int();
		HEMAX_MaxScriptInterface::CreateModifierHda(Node, AssetPath, AssetIndex);
	}

	return &ok;
}

Value*
CookHda_cf(Value** ArgList, int Count)
{
	if (Count == 1)
	{
		INode* Node = ArgList[0]->to_node();
		HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->Find3dsmaxHda(Node->GetHandle());
		if (Hda)
		{
			HEMAX_MaxScriptInterface::PluginInstance->HandleRecookRequest(&(Hda->Hda.MainNode));
		}
	}
	else if (Count == 2)
	{
		INode* Node = ArgList[0]->to_node();
		Modifier* Modifier = ArgList[1]->to_modifier();
		HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->Find3dsmaxHda(Node->GetHandle(), Modifier);
		if (Hda)
		{
			HEMAX_MaxScriptInterface::PluginInstance->HandleRecookRequest(&(Hda->Hda.MainNode));
		}
	}

	return &ok;
}

Value*
SetGeometryHdaParameter_cf(Value** ArgList, int Count)
{
	if (Count >= 3)
	{
		INode* Node = ArgList[0]->to_node();
		std::wstring WParamName(ArgList[1]->to_string());
		std::string ParamName(WParamName.begin(), WParamName.end());

		HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->Find3dsmaxHda(Node->GetHandle());
		if (Hda)
		{
			HEMAX_Parameter Parm = GetParameter(Hda->Hda.MainNode, ParamName);
			
			if (Parm.Type != HEMAX_PARAM_INVALID)
			{
				if (HEMAX_MaxScriptInterface::UpdateParameter(Parm, ArgList, Count, 2))
				{
					HEMAX_MaxScriptInterface::PluginInstance->HandleRecookRequest(&(Hda->Hda.MainNode));
				}
			}
		}
	}

	return &ok;
}

Value*
SetModifierHdaParameter_cf(Value** ArgList, int Count)
{
	if (Count >= 4)
	{
		INode* Node = ArgList[0]->to_node();
		Modifier* Modifier = ArgList[1]->to_modifier();
		std::wstring WParamName(ArgList[2]->to_string());
		std::string ParamName(WParamName.begin(), WParamName.end());

		HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->Find3dsmaxHda(Node->GetHandle(), Modifier);
		if (Hda)
		{
			HEMAX_Parameter Parm = GetParameter(Hda->Hda.MainNode, ParamName);

			if (Parm.Type != HEMAX_PARAM_INVALID)
			{
				if (HEMAX_MaxScriptInterface::UpdateParameter(Parm, ArgList, Count, 3))
				{
					HEMAX_MaxScriptInterface::PluginInstance->HandleRecookRequest(&(Hda->Hda.MainNode));
				}
			}
		}
	}

	return &ok;
}

void
HEMAX_MaxScriptInterface::CreateGeometryHda(std::string AssetPath, int AssetIndex)
{
	HEMAX_MaxScriptInterface::PluginInstance->CreateGeometryHDA(AssetPath, AssetIndex);
}

void
HEMAX_MaxScriptInterface::CreateModifierHda(INode* Node, std::string AssetPath, int AssetIndex)
{
	HEMAX_MaxScriptInterface::PluginInstance->CreateModifierHDA(Node, AssetPath, AssetIndex);
}

bool
HEMAX_MaxScriptInterface::UpdateParameter(HEMAX_Parameter Parm, Value** ArgList, int Count, int StartIndex)
{
	bool Success = true;

	HEMAX_MaxScriptArgType ArgType = HEMAX_MaxScriptInterface::ResolveParameterTypeToMaxScriptType(Parm.Type);

	switch (ArgType)
	{
	case HEMAX_MS_ARG_INT_TYPE:
	{
		std::vector<int> IntVals;
		for (int i = StartIndex; i < Count; i++)
		{
			IntVals.push_back(ArgList[i]->to_int());
		}
		UpdateParameterIntValues(Parm, IntVals);
	} break;
	case HEMAX_MS_ARG_FLOAT_TYPE:
	{
		std::vector<float> FloatVals;
		for (int i = StartIndex; i < Count; i++)
		{
			FloatVals.push_back(ArgList[i]->to_float());
		}
		UpdateParameterFloatValues(Parm, FloatVals);
	} break;
	case HEMAX_MS_ARG_STRING_TYPE:
	{
		std::vector<std::string> StringVals;
		for (int i = StartIndex; i < Count; i++)
		{
			std::wstring WVal(ArgList[i]->to_string());
			StringVals.push_back(std::string(WVal.begin(), WVal.end()));
		}
		UpdateParameterStringValues(Parm, StringVals);
	} break;
	default:
	{
		Success = false;
	} break;
	}

	return Success;
}

HEMAX_MaxScriptArgType
HEMAX_MaxScriptInterface::ResolveParameterTypeToMaxScriptType(HEMAX_ParameterType Type)
{
	switch (Type)
	{
		case HEMAX_PARAM_INTEGER:
		case HEMAX_PARAM_TOGGLE:
		{
			return HEMAX_MS_ARG_INT_TYPE;
		} break;
		case HEMAX_PARAM_STRING:
		case HEMAX_PARAM_PATH_FILE:
        case HEMAX_PARAM_PATH_FILE_DIR:
		case HEMAX_PARAM_PATH_FILE_GEO:
		case HEMAX_PARAM_PATH_FILE_IMAGE:
		{
			return HEMAX_MS_ARG_STRING_TYPE;
		} break;
		case HEMAX_PARAM_FLOAT:
		{
			return HEMAX_MS_ARG_FLOAT_TYPE;
		} break;
		default:
		{
			return HEMAX_MS_ARG_INVALID_TYPE;
		} break;
	}
}