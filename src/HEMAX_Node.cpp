#include "HEMAX_Node.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_UserPrefs.h"

constexpr const char* DoubleCookParmName = "hda_force_double_cook";

HEMAX_Node::HEMAX_Node()
{
    Info.id = -1;
    Type = HAPI_NODETYPE_NONE;
    
    InitNodeOptions();
}

HEMAX_Node::HEMAX_Node(HAPI_NodeId NodeId, HAPI_NodeType NodeType)
{
    Info.id = NodeId;
    Type = NodeType;

    InitNodeOptions();
}

void
HEMAX_Node::Init(const std::string& Asset)
{
    AssetName = Asset;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (HEMAX_HoudiniApi::CreateNode(SM.Session, -1, AssetName.c_str(), nullptr,
            false, &Info.id))
    {
        HEMAX_HoudiniApi::GetNodeInfo(SM.Session, Info.id, &Info);
        Type = Info.type;

	NodeName = SM.Session->GetHAPIString(Info.nameSH);

	if (Info.inputCount > 0)
	{
	    HAPI_StringHandle Handle;
	    for (int j = 0; j < Info.inputCount; j++)
	    {
                HEMAX_HoudiniApi::GetNodeInputName(SM.Session, Info.id, j,
                    &Handle);
		InputLabels.push_back(SM.Session->GetHAPIString(Handle));	
	    }
	}
    }
}

void
HEMAX_Node::Cook()
{
    if (Type != HAPI_NODETYPE_NONE)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

        HEMAX_HoudiniApi::CookNode(SM.Session, Info.id, SM.Session->GetCookOptions());
	while (SM.Session->IsCookFinished() != COOK_FINISHED)
	{
	    // Wait
	}
        HEMAX_HoudiniApi::GetNodeInfo(SM.Session, Info.id, &Info);

	if (Info.parmCount > 0)
	{
	    std::vector<HAPI_ParmInfo> ParmInfos(Info.parmCount);
            HEMAX_HoudiniApi::GetParameters(SM.Session, Info.id,
                &ParmInfos.front(), 0, Info.parmCount);

	    Parameters.clear();

	    for (int i = 0; i < Info.parmCount; i++)
	    {
                Parameters.emplace_back(Info.id, ParmInfos[i]);
	    }
	}
    }
    else
    {
	HEMAX_Logger::Instance().AddEntry("Cannot cook an invalid node.",
				    HEMAX_LOG_LEVEL_WARN);	
    }
}

void
HEMAX_Node::Delete()
{
    HEMAX_HoudiniApi::DeleteNode(HEMAX_SessionManager::GetSessionManager().Session,
        Info.id);
    Type = HAPI_NODETYPE_NONE;
}

void
HEMAX_Node::UpdateInfo()
{
    HEMAX_HoudiniApi::GetNodeInfo(HEMAX_SessionManager::GetSessionManager().Session,
        Info.id, &Info);
}

std::vector<HEMAX_Parameter>&
HEMAX_Node::GetParameters()
{
    return Parameters;
}

HEMAX_Parameter*
HEMAX_Node::GetParameter(HAPI_ParmId ParmId)
{
    for (int i = 0; i < Parameters.size(); i++)
    {
	if (Parameters[i].Info.id == ParmId)
	{
	    return &Parameters[i];
	}
    }

    return nullptr;
}

HEMAX_Parameter*
HEMAX_Node::GetParameter(std::string ParmName)
{
    for (int i = 0; i < Parameters.size(); i++)
    {
	if (Parameters[i].GetName() == ParmName)
	{
	    return &Parameters[i];
	}
    }

    return nullptr;
}

std::vector<HEMAX_Parameter>
HEMAX_Node::GetAllMultiParameterInstances(int MultiId)
{
    std::vector<HEMAX_Parameter> Instances;

    HEMAX_Parameter* MultiParmList = GetParameter(MultiId);

    if (MultiParmList)
    {
	for (int i = 0; i < Parameters.size(); i++)
	{
	    if (Parameters[i].Info.isChildOfMultiParm &&
		Parameters[i].Info.parentId == MultiId)
	    {
		Instances.push_back(Parameters[i]);
	    }
	}
    }

    return Instances;
}

void
HEMAX_Node::SetTransform(HEMAX_MaxTransform& Xform)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::SetObjectTransform(SessionManager.Session, Info.id,
        &HEMAX_Utilities::MaxTransformToHAPITransformEuler(Xform));
}

void
HEMAX_Node::SetParentTransform(HEMAX_MaxTransform& Xform)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::SetObjectTransform(SessionManager.Session, Info.parentId,
        &HEMAX_Utilities::MaxTransformToHAPITransformEuler(Xform, HAPI_SRT));
}

void
HEMAX_Node::ConnectInputNode(HAPI_NodeId NodeToConnect, int InputIndex)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::ConnectNodeInput(SessionManager.Session, Info.id,
        InputIndex, NodeToConnect, 0);
}

void
HEMAX_Node::DisconnectInputNode(int InputIndex)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HEMAX_HoudiniApi::DisconnectNodeInput(SessionManager.Session, Info.id,
        InputIndex);
}

HAPI_NodeId
HEMAX_Node::QueryNodeInput(int InputIndex)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_NodeId ConnectedNode;

    if (HEMAX_HoudiniApi::QueryNodeInput(SM.Session, Info.id, InputIndex,
            &ConnectedNode))
    {
	return ConnectedNode;
    }
    else
    {
	return -1;
    }
}

bool
HEMAX_Node::ShouldCookTwice()
{
    for (int i = 0; i < Parameters.size(); i++)
    {
	if (Parameters[i].GetName() == DoubleCookParmName)
	{
	    std::vector<int> IntVals = Parameters[i].GetIntVals();
	   return (bool)(IntVals[0]); 
	}
    }

    return false;
}

void
HEMAX_Node::InitNodeOptions()
{
    AutoRecookOnParameterUpdate = true;
    RealtimeRecookEnabled = false;
    AutoRecookOnInputChange = false;

    bool Option;

    if (HEMAX_UserPrefs::Get().GetBoolSetting(
            HEMAX_SETTING_NODE_OPTION_AUTORECOOK, Option))
    {
        AutoRecookOnParameterUpdate = Option;
    }

    if (HEMAX_UserPrefs::Get().GetBoolSetting(
            HEMAX_SETTING_NODE_OPTION_SLIDERCOOK, Option))
    {
        RealtimeRecookEnabled = Option;
    }
}
