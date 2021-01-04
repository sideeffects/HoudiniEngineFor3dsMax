#include "HEMAX_MaxScriptInterface.h"

#include <maxscript/maxwrapper/mxsobjects.h>
#include <maxscript/maxscript.h>
#include <maxscript/macros/define_instantiation_functions.h>

#include "HEMAX_3dsmaxHda.h"
#include "HEMAX_Events.h"
#include "HEMAX_GeometryHda.h"
#include "HEMAX_Logger.h"
#include "HEMAX_ModifierHda.h"
#include "HEMAX_Plugin.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_UserPrefs.h"
#include "UI/HEMAX_UI.h"

HEMAX_Plugin* HEMAX_MaxScriptInterface::PluginInstance = nullptr;
HEMAX_UI* HEMAX_MaxScriptInterface::PluginUserInterface = nullptr;

def_visible_primitive(CreateSession, "CreateSession");
def_visible_primitive(CloseSession, "CloseSession");
def_visible_primitive(IsSessionActive, "IsSessionActive");
def_visible_primitive(IsHdaLoaded, "IsHdaLoaded");
def_visible_primitive(LoadHda, "LoadHda");
def_visible_primitive(CreateGeometryHda, "CreateGeometryHda");
def_visible_primitive(CreateModifierHda, "CreateModifierHda");
def_visible_primitive(CookHda, "CookHda");
def_visible_primitive(SetGeometryHdaParameter, "SetGeometryHdaParameter");
def_visible_primitive(SetModifierHdaParameter, "SetModifierHdaParameter");
def_visible_primitive(BakeHda, "BakeHda");
def_visible_primitive(CopyModifierHda, "CopyModifierHda");
def_visible_primitive(CloneObjectHda, "CloneObjectHda");
def_visible_primitive(GetListOfGeometryHdaParameters, "GetListOfGeometryHdaParameters");
def_visible_primitive(GetListOfModifierHdaParameters, "GetListOfModifierHdaParameters");
def_visible_primitive(SetGeometryHdaInput, "SetGeometryHdaInput");
def_visible_primitive(SetModifierHdaInput, "SetModifierHdaInput");
def_visible_primitive(SetGeometryHdaOpParmInput, "SetGeometryHdaOpParmInput");
def_visible_primitive(SetModifierHdaOpParmInput, "SetModifierHdaOpParmInput");
def_visible_primitive(SetGeometryHdaAutoRecookingEnabled, "SetGeometryHdaAutoRecookingEnabled");
def_visible_primitive(SetModifierHdaAutoRecookingEnabled, "SetModifierHdaAutoRecookingEnabled");
def_visible_primitive(SetHdaSelectionLocked, "SetHdaSelectionLocked");

Value*
CreateSession_cf(Value** ArgList, int Count)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.IsSessionActive())
    {
	std::string Msg = "Cannot start a session: there is already an active session";
	HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
	return &false_value;
    }

    SM.StartThriftNamedPipeThinClient();
    
    if (!HEMAX_MaxScriptInterface::PluginInstance->StartSession())
    {
	return &false_value;
    }

    HEMAX_MaxScriptInterface::PluginInstance->GetEventHub()->SessionChanged();

    return &true_value;
}

Value*
CloseSession_cf(Value** ArgList, int Count)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.IsSessionActive())
    {
	HEMAX_MaxScriptInterface::PluginInstance->StopSession();
        HEMAX_MaxScriptInterface::PluginInstance->GetEventHub()->SessionChanged();
    }

    return &true_value;
}

Value*
IsSessionActive_cf(Value** ArgList, int Count)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.IsSessionActive())
    {
        return &true_value;
    }
    else
    {
        return &false_value;
    }
}

Value*
IsHdaLoaded_cf(Value** ArgList, int Count)
{
    check_arg_count(IsHdaLoaded, 1, Count);
    type_check(ArgList[0], String, _M("IsHdaLoaded filepath"));

    std::wstring WHdaPath(ArgList[0]->to_string());
    std::string HdaPath = std::string(WHdaPath.begin(), WHdaPath.end()); 

    HEMAX_Plugin* Plugin = HEMAX_MaxScriptInterface::PluginInstance;
    HEMAX_Store* Store = Plugin->GetPluginStore();

    if (Store->IsAssetLoaded(HdaPath))
    {
	return &true_value;
    }
    else
    {
	return &false_value;
    }
}

Value*
LoadHda_cf(Value** ArgList, int Count)
{
    check_arg_count(LoadHda, 1, Count);
    type_check(ArgList[0], String, _M("LoadHda filepath"));

    std::wstring WHdaPath(ArgList[0]->to_string());
    std::string HdaPath = std::string(WHdaPath.begin(), WHdaPath.end());

    HEMAX_Plugin* Plugin = HEMAX_MaxScriptInterface::PluginInstance;
    HEMAX_Store* Store = Plugin->GetPluginStore();

    bool Success;
    Store->LoadNewAsset(HdaPath, Success);

    if (Success)
    {
        Plugin->GetEventHub()->AssetLoaded();
	return &true_value;
    }
    else
    {
	return &false_value;
    }
}

Value*
CreateGeometryHda_cf(Value** ArgList, int Count)
{
    HEMAX_3dsmaxHda* Hda = nullptr;

    if (Count == 1)
    {
	std::wstring WAssetPath(ArgList[0]->to_string());
	std::string AssetPath(WAssetPath.begin(), WAssetPath.end());

	Hda = HEMAX_MaxScriptInterface::CreateGeometryHda(AssetPath, 0);
    }
    else if (Count == 2)
    {
	std::wstring WAssetPath(ArgList[0]->to_string());
	std::string AssetPath(WAssetPath.begin(), WAssetPath.end());

	int AssetIndex = ArgList[1]->to_int();
	Hda = HEMAX_MaxScriptInterface::CreateGeometryHda(AssetPath, AssetIndex);
    }

    if (Hda)
    {
	HEMAX_GeometryHda* GeomHda = static_cast<HEMAX_GeometryHda*>(Hda);
	return MAXNode::intern(GeomHda->ContainerNode);
    }
    else
    {
	return &undefined;
    }
}

Value*
CreateModifierHda_cf(Value** ArgList, int Count)
{
    HEMAX_3dsmaxHda* Hda = nullptr;

    if (Count == 2)
    {
	INode* Node = ArgList[0]->to_node();
	std::wstring WAssetPath(ArgList[1]->to_string());
	std::string AssetPath(WAssetPath.begin(), WAssetPath.end());

	Hda = HEMAX_MaxScriptInterface::CreateModifierHda(Node, AssetPath, 0);
    }
    else if (Count == 3)
    {
	INode* Node = ArgList[0]->to_node();
	std::wstring WAssetPath(ArgList[1]->to_string());
	std::string AssetPath(WAssetPath.begin(), WAssetPath.end());

	int AssetIndex = ArgList[2]->to_int();
	Hda = HEMAX_MaxScriptInterface::CreateModifierHda(Node, AssetPath, AssetIndex);
    }
    if (Hda)
    {
	HEMAX_ModifierHda* ModHda = static_cast<HEMAX_ModifierHda*>(Hda);
	return MAXModifier::intern(ModHda->DisplayGeometry);
    }
    else
    {
	return &undefined;
    }
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
	else
	{
	    std::string Msg = "Could not find the specified geometry HDA";
	    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN); 
	    return &false_value;
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
	else
	{
	    std::string Msg = "Could not find the specified modifier HDA";
	    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
	    return &false_value;
	}
    }

    return &true_value;
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
	    HEMAX_Parameter* Parm = Hda->Hda.MainNode.GetParameter(ParamName);

	    if (Parm)
	    {
		if (HEMAX_MaxScriptInterface::UpdateParameter(*Hda, *Parm, ArgList, Count, 2))
		{
		    HEMAX_MaxScriptInterface::PluginInstance->HandleRecookRequest(&(Hda->Hda.MainNode));
		    return &true_value;
		}
	    }
	}
    }

    return &false_value;
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
	    HEMAX_Parameter* Parm = Hda->Hda.MainNode.GetParameter(ParamName);

	    if (Parm)
	    {
		if (HEMAX_MaxScriptInterface::UpdateParameter(*Hda, *Parm, ArgList, Count, 3))
		{
		    HEMAX_MaxScriptInterface::PluginInstance->HandleRecookRequest(&(Hda->Hda.MainNode));
		    return &true_value;
		}
	    }
	}
    }

    return &false_value;
}

Value*
BakeHda_cf(Value** ArgList, int Count)
{
    if (Count == 1)
    {
	INode* Node = ArgList[0]->to_node();
	HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->Find3dsmaxHda(Node->GetHandle());
	if (Hda && Hda->Type == HEMAX_GEOMETRY_HDA)
	{
	    HEMAX_GeometryHda* GeometryHda = static_cast<HEMAX_GeometryHda*>(Hda);
            
            HEMAX_UserPrefs* Prefs = HEMAX_MaxScriptInterface::PluginInstance->
                                        GetUserPrefs();

            bool BakeDummyObj;
            Prefs->GetBoolSetting(HEMAX_SETTING_BAKE_DUMMY_OBJECT,
                                  BakeDummyObj);

	    std::vector<INode*> BakeResults = GeometryHda->BakeGeometryHda(
                                                            BakeDummyObj);

	    if (BakeResults.size() > 1)
	    {
		Array* NodeList = new Array(int(BakeResults.size()) - 1);

		for (int i = 1; i < BakeResults.size(); i++)
		{
		    NodeList->append(MAXNode::intern(BakeResults[i]));
		}

		return NodeList;
	    }
	}
    }

    return &undefined;
}

Value*
CopyModifierHda_cf(Value** ArgList, int Count)
{
    if (Count == 3)
    {
	INode* FromNode = ArgList[0]->to_node();
	Modifier* SourceMod = ArgList[1]->to_modifier();
	INode* ToNode = ArgList[2]->to_node();

	HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->Find3dsmaxHda(FromNode->GetHandle(), SourceMod);
	if (Hda)
	{
	    Modifier* Clone = HEMAX_MaxScriptInterface::PluginInstance->CopyHdaToNode(Hda, ToNode);
	    if (Clone)
	    {
		return MAXModifier::intern(Clone);
	    }
	}
    }

    return &undefined;
}

Value*
CloneObjectHda_cf(Value** ArgList, int Count)
{
    if (Count == 1)
    {
	INode* Node = ArgList[0]->to_node();
	HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->Find3dsmaxHda(Node->GetHandle());
	if (Hda)
	{
	    INode* Clone = HEMAX_MaxScriptInterface::PluginInstance->CloneHda(Hda);
	    if (Clone)
	    {
		return MAXNode::intern(Clone);
	    }
	}
    }

    return &undefined;
}

Value*
GetListOfGeometryHdaParameters_cf(Value** ArgList, int Count)
{
    check_arg_count(GetListOfGeometryHdaParameters, 1, Count);
    
    INode* Node = ArgList[0]->to_node();
    if (Node)
    {
	HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->Find3dsmaxHda(Node->GetHandle());

	if (Hda)
	{
	    std::vector<HEMAX_Parameter>& Parameters = Hda->Hda.MainNode.GetParameters();

	    Array* Parms = new Array((int)Parameters.size());
	    for (int i = 0; i < Parameters.size(); i++)
	    {
		std::string Name = Parameters[i].GetName();
		std::wstring WName = std::wstring(Name.begin(), Name.end());
		Parms->append(Name::intern(WName.c_str()));
	    }

	    return Parms;
	}	    
    }
    
    return &undefined;
}

Value*
GetListOfModifierHdaParameters_cf(Value** ArgList, int Count)
{
    check_arg_count(GetListOfModifierHdaParameters, 2, Count);

    INode* Node = ArgList[0]->to_node();
    Modifier* Mod = ArgList[1]->to_modifier();

    if (Node && Mod)
    {
	HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->Find3dsmaxHda(Node->GetHandle(), Mod);
	if (Hda)
	{
	    std::vector<HEMAX_Parameter>& Parameters = Hda->Hda.MainNode.GetParameters();
	    
	    Array* Parms = new Array((int)Parameters.size());
	    for (int i = 0; i < Parameters.size(); i++)
	    {
		std::string Name = Parameters[i].GetName();
		std::wstring WName = std::wstring(Name.begin(), Name.end());
		Parms->append(Name::intern(WName.c_str()));
	    }

	    return Parms;
	}
    }

    return &undefined;
}

Value*
SetGeometryHdaInput_cf(Value** ArgList, int Count)
{
    check_arg_count(SetGeometryHdaInput, 3, Count);

    INode* Node = ArgList[0]->to_node();
    int Subnetwork = ArgList[1]->to_int();
    INode* InputNode = ArgList[2]->to_node();

    if (Node && InputNode)
    {
	HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->
				    Find3dsmaxHda(Node->GetHandle());
	if (Hda)
	{
	    int InputCount = Hda->Hda.MainNode.Info.inputCount; 
	    if (Subnetwork < InputCount)
	    {
		HEMAX_MaxScriptInterface::PluginInstance->
		    HandleSubnetworkInputSelection(&Hda->Hda.MainNode,
						   Subnetwork,
						   InputNode);

                HEMAX_MaxScriptInterface::PluginInstance->
                    GetEventHub()->InputChanged();

		return &true_value;
	    }
	    else
	    {
		HEMAX_Logger::Instance().AddEntry("Invalid subnetwork input specified.",
					    HEMAX_LOG_LEVEL_ERROR);
	    }
	}
    }

    return &false_value;
}

Value*
SetModifierHdaInput_cf(Value** ArgList, int Count)
{
    check_arg_count(SetModifierHdaInput, 4, Count);

    INode* Node = ArgList[0]->to_node();
    Modifier* Mod = ArgList[1]->to_modifier();
    int Subnetwork = ArgList[2]->to_int();
    INode* InputNode = ArgList[3]->to_node();

    if (Node && Mod && InputNode)
    {
	HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->GetPluginStore()->
				    Find3dsmaxHda(Node->GetHandle(), Mod);	    

	if (Hda)
	{
	    int InputCount = Hda->Hda.MainNode.Info.inputCount;
	    if (Subnetwork == 0)
	    {
		HEMAX_Logger::Instance().AddEntry("The first subnetwork input is reserved " \
					    "for the modifier's input.", HEMAX_LOG_LEVEL_ERROR);
	    }
	    else if (Subnetwork > 0 && Subnetwork < InputCount)
	    {
		HEMAX_MaxScriptInterface::PluginInstance->
		    HandleSubnetworkInputSelection(&Hda->Hda.MainNode,
						   Subnetwork,
						   InputNode);		   

                HEMAX_MaxScriptInterface::PluginInstance->
                    GetEventHub()->InputChanged();

		return &true_value;
	    }
	    else
	    {
		HEMAX_Logger::Instance().AddEntry("Invalid subnetwork input specified.",
					    HEMAX_LOG_LEVEL_ERROR);
	    }
	}
    }

    return &false_value;
}

Value*
SetGeometryHdaOpParmInput_cf(Value** ArgList, int Count)
{
    check_arg_count(SetGeometryHdaOpParmInput, 3, Count);

    INode* Node = ArgList[0]->to_node();
    std::wstring WParamName(ArgList[1]->to_string());
    std::string ParamName(WParamName.begin(), WParamName.end());
    INode* InputNode = ArgList[2]->to_node();

    if (Node && InputNode)
    {
	HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->
				GetPluginStore()->Find3dsmaxHda(Node->GetHandle());

	if (Hda)
	{
	    HEMAX_Parameter* Parm = Hda->Hda.MainNode.GetParameter(ParamName);
	    if (Parm)
	    {
		HEMAX_MaxScriptInterface::PluginInstance->
		    HandleParameterInputSelection(&Hda->Hda.MainNode,
						  *Parm,
						  InputNode);		  

                HEMAX_MaxScriptInterface::PluginInstance->
                    GetEventHub()->ParameterChanged();

		return &true_value;
	    }
	}
    }

    return &false_value;
}

Value*
SetModifierHdaOpParmInput_cf(Value** ArgList, int Count)
{
    check_arg_count(SetModifierHdaOpParmInput, 4, Count);

    INode* Node = ArgList[0]->to_node();
    Modifier* Mod = ArgList[1]->to_modifier();
    std::wstring WParamName(ArgList[2]->to_string());
    std::string ParamName(WParamName.begin(), WParamName.end());
    INode* InputNode = ArgList[3]->to_node();

    if (Node && Mod && InputNode)
    {
	HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->
				GetPluginStore()->Find3dsmaxHda(Node->GetHandle(), Mod);

	if (Hda)
	{
	    HEMAX_Parameter* Parm = Hda->Hda.MainNode.GetParameter(ParamName);
	    if (Parm)
	    {
		HEMAX_MaxScriptInterface::PluginInstance->
		    HandleParameterInputSelection(&Hda->Hda.MainNode,
						  *Parm,
						  InputNode);

                HEMAX_MaxScriptInterface::PluginInstance->
                    GetEventHub()->ParameterChanged();

		return &true_value;
	    }
	}
    }

    return &false_value;
}

Value*
SetGeometryHdaAutoRecookingEnabled_cf(Value** ArgList, int Count)
{
    check_arg_count(SetGeometryHdaAutoRecookingEnabled, 2, Count);
    INode* Node = ArgList[0]->to_node();
    bool Enabled = ArgList[1]->to_bool();

    if (Node)
    {
        HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->
            GetPluginStore()->Find3dsmaxHda(Node->GetHandle());

        if (Hda)
        {
            Hda->Hda.MainNode.AutoRecookOnParameterUpdate = Enabled;
            HEMAX_MaxScriptInterface::PluginInstance->
                GetEventHub()->NodeChanged();
            return &true_value;
        }
    }

    return &false_value;
}

Value*
SetModifierHdaAutoRecookingEnabled_cf(Value** ArgList, int Count)
{
    check_arg_count(SetModifierHdaAutoRecookingEnabled, 3, Count);
    INode* Node = ArgList[0]->to_node();
    Modifier* Mod = ArgList[1]->to_modifier();
    bool Enabled = ArgList[2]->to_bool();

    if (Node && Mod)
    {
        HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->
            GetPluginStore()->Find3dsmaxHda(Node->GetHandle(), Mod);

        if (Hda)
        {
            Hda->Hda.MainNode.AutoRecookOnParameterUpdate = Enabled;
            HEMAX_MaxScriptInterface::PluginInstance->
                GetEventHub()->NodeChanged();
            return &true_value;
        }
    }

    return &false_value;
}

Value*
SetHdaSelectionLocked_cf(Value** ArgList, int Count)
{
    check_arg_count(SetHdaSelectionLocked, 1, Count);
    bool Locked = ArgList[0]->to_bool();

    if (Locked &&
        HEMAX_MaxScriptInterface::PluginUserInterface->GetCurrentHdaSelection())
    {
        HEMAX_MaxScriptInterface::PluginUserInterface->SetSelectionLocked(true);
    }
    else if (!Locked)
    {
        HEMAX_MaxScriptInterface::PluginUserInterface->SetSelectionLocked(false);
    }

    return &true_value;
}

HEMAX_3dsmaxHda*
HEMAX_MaxScriptInterface::CreateGeometryHda(std::string AssetPath, int AssetIndex)
{
    HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->CreateGeometryHDA(AssetPath, AssetIndex);

    return Hda;
}

HEMAX_3dsmaxHda*
HEMAX_MaxScriptInterface::CreateModifierHda(INode* Node, std::string AssetPath, int AssetIndex)
{
    HEMAX_3dsmaxHda* Hda = HEMAX_MaxScriptInterface::PluginInstance->CreateModifierHDA(Node, AssetPath, AssetIndex);

    return Hda;
}

bool
HEMAX_MaxScriptInterface::UpdateParameter(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter Parm, Value** ArgList, int Count, int StartIndex)
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
	    Parm.UpdateIntVals(IntVals);
	} break;
	case HEMAX_MS_ARG_FLOAT_TYPE:
	{
	    std::vector<float> FloatVals;
	    for (int i = StartIndex; i < Count; i++)
	    {
		FloatVals.push_back(ArgList[i]->to_float());
	    }
	    Parm.UpdateFloatVals(FloatVals);
	} break;
	case HEMAX_MS_ARG_STRING_TYPE:
	{
	    std::vector<std::string> StringVals;
	    for (int i = StartIndex; i < Count; i++)
	    {
		std::wstring WVal(ArgList[i]->to_string());
		StringVals.push_back(std::string(WVal.begin(), WVal.end()));
	    }
	    Parm.UpdateStringVals(StringVals);
	} break;
	case HEMAX_MS_ARG_NODE_TYPE:
	{
	    HEMAX_MaxScriptInterface::PluginInstance->HandleParameterInputSelection(&MaxHda.Hda.MainNode, Parm, ArgList[StartIndex]->to_node());
	}
	default:
	{
	    Success = false;
	} break;
    }

    if (Success)
    {
        HEMAX_MaxScriptInterface::PluginInstance->
            GetEventHub()->ParameterChanged();
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
	case HEMAX_PARAM_MULTIPARMLIST:
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
	case HEMAX_PARAM_NODE:
	{
	    return HEMAX_MS_ARG_NODE_TYPE;
	} break;
	default:
	{
	    return HEMAX_MS_ARG_INVALID_TYPE;
	} break;
    }
}
