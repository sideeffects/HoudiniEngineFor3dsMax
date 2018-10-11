#pragma once

#include "HEMAX_Parameter.h"

#include <string>
#include <inode.h>

class HEMAX_Plugin;

enum HEMAX_MaxScriptArgType
{
	HEMAX_MS_ARG_INT_TYPE,
	HEMAX_MS_ARG_FLOAT_TYPE,
	HEMAX_MS_ARG_STRING_TYPE,
	HEMAX_MS_ARG_INVALID_TYPE
};

struct HEMAX_MaxScriptInterface
{
	static HEMAX_Plugin* PluginInstance;

	static void CreateGeometryHda(std::string AssetPath, int AssetIndex);
	static void CreateModifierHda(INode* Node, std::string AssetPath, int AssetIndex);

	static bool UpdateParameter(HEMAX_Parameter Parm, Value** ArgList, int Count, int StartIndex);
	static HEMAX_MaxScriptArgType ResolveParameterTypeToMaxScriptType(HEMAX_ParameterType Type);
};