#pragma once

#include "HEMAX_Parameter.h"

#include <string>

#pragma warning(push, 0)
#include <inode.h>
#pragma warning(pop)

class HEMAX_UI;
class HEMAX_Plugin;
class HEMAX_3dsmaxHda;

enum HEMAX_MaxScriptArgType
{
    HEMAX_MS_ARG_INT_TYPE,
    HEMAX_MS_ARG_FLOAT_TYPE,
    HEMAX_MS_ARG_STRING_TYPE,
    HEMAX_MS_ARG_NODE_TYPE,
    HEMAX_MS_ARG_INVALID_TYPE
};

struct HEMAX_MaxScriptInterface
{
    static HEMAX_UI*        PluginUserInterface; 
    static HEMAX_Plugin*    PluginInstance;

    static HEMAX_3dsmaxHda* CreateGeometryHda(std::string AssetPath,
                                              int AssetIndex);

    static HEMAX_3dsmaxHda* CreateModifierHda(INode* Node,
                                              std::string AssetPath,
                                              int AssetIndex);

    static bool UpdateParameter(HEMAX_3dsmaxHda& MaxHda,
                                HEMAX_Parameter Parm,
                                Value** ArgList,
                                int Count,
                                int StartIndex);

    static HEMAX_MaxScriptArgType ResolveParameterTypeToMaxScriptType(
                                        HAPI_ParmType Type);
};
