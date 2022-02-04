#include "HEMAX_Node.h"

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

    if (SM.Session->CreateNode(-1, AssetName.c_str(), nullptr, false, &Info.id))
    {
	SM.Session->GetNodeInfo(Info.id, &Info);
        Type = Info.type;

	NodeName = SM.Session->GetHAPIString(Info.nameSH);

	if (Info.inputCount > 0)
	{
	    HAPI_StringHandle Handle;
	    for (int j = 0; j < Info.inputCount; j++)
	    {
		SM.Session->GetNodeInputName(Info.id, j, &Handle);
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

	SM.Session->CookNode(Info.id);
	while (SM.Session->IsCookFinished() != COOK_FINISHED)
	{
	    // Wait
	}
	SM.Session->GetNodeInfo(Info.id, &Info);

	if (Info.parmCount > 0)
	{
	    std::vector<HAPI_ParmInfo> ParmInfos(Info.parmCount);
	    SM.Session->GetParameters(Info.id, &ParmInfos.front(), 0, Info.parmCount);

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
    HEMAX_SessionManager::GetSessionManager().Session->DeleteNode(Info.id);
    Type = HAPI_NODETYPE_NONE;
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

    SessionManager.Session->SetObjectTransform(Info.id, &HEMAX_Utilities::MaxTransformToHAPITransformEuler(Xform));
}

void
HEMAX_Node::SetParentTransform(HEMAX_MaxTransform& Xform)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetObjectTransform(Info.parentId, &HEMAX_Utilities::MaxTransformToHAPITransformEuler(Xform, HEMAX_SRT));
}

void
HEMAX_Node::ConnectInputNode(HAPI_NodeId NodeToConnect, int InputIndex)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->ConnectNodeInput(Info.id, InputIndex, NodeToConnect);
}

void
HEMAX_Node::DisconnectInputNode(int InputIndex)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->DisconnectNodeInput(Info.id, InputIndex);
}

HAPI_NodeId
HEMAX_Node::QueryNodeInput(int InputIndex)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HAPI_NodeId ConnectedNode;

    if (SessionManager.Session->QueryNodeInput(Info.id, InputIndex, &ConnectedNode))
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
