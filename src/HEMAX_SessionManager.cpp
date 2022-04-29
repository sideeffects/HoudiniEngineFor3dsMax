#include "HEMAX_SessionManager.h"

#include "HEMAX_Events.h"
#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"

const char* const HAPI_CLIENT_NAME_ENV_VAR = "HAPI_CLIENT_NAME";
const char* const HAPI_CLIENT_NAME_ENV_VAL = "3dsmax";

HEMAX_SessionManager&
HEMAX_SessionManager::GetSessionManager()
{
    static HEMAX_SessionManager SessionManager;
    return SessionManager;
}

HEMAX_SessionManager::HEMAX_SessionManager()
    : SessionType( HEMAX_THRIFT_PIPE )
{
    IsActiveSession = false;
    IsSessionInitialized = false;
    AutoSession = false;
    InitializeSession();
}

HEMAX_SessionManager::~HEMAX_SessionManager()
{
    if (IsActiveSession)
    {
	StopSession();
	IsActiveSession = false;
    }

    delete Session;
}

bool
HEMAX_SessionManager::StartSession()
{
    HEMAX_HoudiniApi::ClearConnectionError();
    if (Session->CreateSession())
    {
	if (Session->InitializeHAPI(
			HoudiniEnvFiles.c_str(),
			OtlSearchPath.c_str(),
			DsoSearchPath.c_str(),
			ImageDsoSearchPath.c_str(),
			AudioDsoSearchPath.c_str()
		    )
	    )
	{
	    IsActiveSession = true;
	    IsSessionInitialized = true;

	    auto EnvMap = GetEnvMap();

	    if (DoesUseHEngineFlagExist(EnvMap))
	    {
		CopyHEngineEnv(EnvMap);
	    }

            HEMAX_HoudiniApi::SetServerEnvString(Session,
                HAPI_CLIENT_NAME_ENV_VAR, HAPI_CLIENT_NAME_ENV_VAL);
	}

        PluginEvents->SessionChanged();
    }
    else
    {
        int ErrLen = 0;
        HEMAX_HoudiniApi::GetConnectionErrorLength(&ErrLen);

        if (ErrLen > 0)
        {
            char* ErrMsg = new char[ErrLen];
            HEMAX_HoudiniApi::GetConnectionError(ErrMsg, ErrLen, false);
            HEMAX_Logger::Instance().AddEntry(ErrMsg, HEMAX_LOG_LEVEL_ERROR);
            delete [] ErrMsg;
        }
    }

    return IsActiveSession;
}

void
HEMAX_SessionManager::StopSession()
{
    if (IsSessionInitialized && IsActiveSession)
    {
        HEMAX_HoudiniApi::Cleanup(Session);
	IsSessionInitialized = false;
    }

    if (HEMAX_HoudiniApi::CloseSession(Session))
    {
	IsActiveSession = false;
    }

    AutoSession = false;
}

bool
HEMAX_SessionManager::IsSessionActive()
{
    return IsActiveSession;
}

void
HEMAX_SessionManager::SetEventHub(HEMAX_Events* _PluginEvents)
{
    PluginEvents = _PluginEvents;
}

HEMAX_SessionType
HEMAX_SessionManager::GetSessionType()
{
    return SessionType;
}

void
HEMAX_SessionManager::SetSessionType( HEMAX_SessionType SType )
{
    SessionType = SType;
    InitializeSession();
    PluginEvents->SessionChanged();
}

bool
HEMAX_SessionManager::IsAutoSession()
{
    return AutoSession;
}

void
HEMAX_SessionManager::InitializeSession()
{
    if ( Session )
    {
	delete Session;
	Session = nullptr;
    }

    switch ( SessionType )
    {
	case HEMAX_IN_PROCESS:
	    {
		Session = new HEMAX_HAPISession;
		break;
	    }
	case HEMAX_THRIFT_SOCKET:
	    {
		Session = new HEMAX_HAPIThriftSocketSession;
		break;
	    }
	case HEMAX_THRIFT_PIPE:
	    {
		Session = new HEMAX_HAPIThriftPipeSession;
		break;
	    }
	default:
	    break;
    }
}

std::unordered_map<std::string, std::string>
HEMAX_SessionManager::GetEnvMap()
{
    std::unordered_map<std::string, std::string> EnvMap;

    int EnvVarCount;
    if (HEMAX_HoudiniApi::GetServerEnvVarCount(Session, &EnvVarCount))
    {
	HAPI_StringHandle* EnvSH = new HAPI_StringHandle[EnvVarCount];
	if (HEMAX_HoudiniApi::GetServerEnvVarList(Session, EnvSH, 0, EnvVarCount))
	{
	    std::vector<std::string> EnvList(EnvVarCount);
	    for (int v = 0; v < EnvVarCount; v++)
	    {
		EnvList[v] = Session->GetHAPIString(EnvSH[v]);
		for (auto searchIt = EnvList[v].begin(); searchIt != EnvList[v].end(); searchIt++)
		{
		    if ((*searchIt) == '=')
		    {
			auto ValIt = searchIt + 1;
			std::string Val = "";

			if (ValIt != EnvList[v].end())
			{
			    Val = std::string((searchIt + 1), EnvList[v].end());
			}

			EnvMap.insert({ std::string(EnvList[v].begin(), searchIt), Val });
		    }
		}
	    }
	}
    }

    return EnvMap;
}

bool
HEMAX_SessionManager::DoesUseHEngineFlagExist(std::unordered_map<std::string, std::string>& EnvMap)
{
    auto EnvSearch = EnvMap.find(HEMAX_USE_SESSION_ENV_FLAG);
    if (EnvSearch != EnvMap.end())
    {
	return true;
    }
    else
    {
	return false;
    }
}

void
HEMAX_SessionManager::CopyHEngineEnv(std::unordered_map<std::string, std::string>& EnvMap)
{
    for (auto EnvIt = EnvMap.begin(); EnvIt != EnvMap.end(); EnvIt++)
    {
	HEMAX_Utilities::SetEnvVar(EnvIt->first, EnvIt->second);
    }
}

void
HEMAX_SessionManager::SetThriftNamedPipeSessionName(std::string PipeName)
{
    if (SessionType == HEMAX_THRIFT_PIPE)
    {
	((HEMAX_HAPIThriftPipeSession*)Session)->SetPipeName(PipeName);
    }
}

void
HEMAX_SessionManager::SetThriftSocketHostName(std::string HostName)
{
    if (SessionType == HEMAX_THRIFT_SOCKET)
    {
	((HEMAX_HAPIThriftSocketSession*)Session)->SetHostName(HostName);
    }
}

void
HEMAX_SessionManager::SetThriftSocketPortNumber(int Port)
{
    if (SessionType == HEMAX_THRIFT_SOCKET)
    {
	((HEMAX_HAPIThriftSocketSession*)Session)->SetPortNumber(Port);
    }
}

void
HEMAX_SessionManager::StartThriftNamedPipeThinClient()
{
    if (SessionType == HEMAX_THRIFT_PIPE)
    {
	SetThriftNamedPipeSessionName(HEMAX_AUTO_PIPE_NAME);
	HEMAX_HAPIThriftPipeSession* TheSession =
            (HEMAX_HAPIThriftPipeSession*)Session;

	HAPI_ThriftServerOptions ServerOptions;
	ServerOptions.autoClose = true;
	ServerOptions.timeoutMs = 20000.0f;
        ServerOptions.verbosity = HAPI_STATUSVERBOSITY_ALL;
	HAPI_ProcessId ServerProcessId;

        HEMAX_HoudiniApi::ClearConnectionError();
        HEMAX_HoudiniApi::StartThriftNamedPipeServer(&ServerOptions,
            HEMAX_AUTO_PIPE_NAME, &ServerProcessId, nullptr);

        int ErrLen = 0;
        HEMAX_HoudiniApi::GetConnectionErrorLength(&ErrLen);

        if (ErrLen > 0)
        {
            char* ErrMsg = new char[ErrLen];
            HEMAX_HoudiniApi::GetConnectionError(ErrMsg, ErrLen, false);
            HEMAX_Logger::Instance().AddEntry(ErrMsg, HEMAX_LOG_LEVEL_ERROR);
            delete [] ErrMsg;

            std::string PathVar = HEMAX_Utilities::GetEnvVar("PATH");
            std::string DebugInfo = ("PATH=" + PathVar);
            HEMAX_Logger::Instance().AddEntry(DebugInfo, HEMAX_LOG_LEVEL_ERROR);
        } 

	AutoSession = true;
    }
}

void
HEMAX_SessionManager::SetHoudiniEnvFiles(std::string Files)
{
    HoudiniEnvFiles = Files;
}

void
HEMAX_SessionManager::SetOtlSearchPath(std::string Path)
{
    OtlSearchPath = Path;
}

void
HEMAX_SessionManager::SetDsoSearchPath(std::string Path)
{
    DsoSearchPath = Path;
}

void
HEMAX_SessionManager::SetImageDsoSearchPath(std::string Path)
{
    ImageDsoSearchPath = Path;
}

void
HEMAX_SessionManager::SetAudioDsoSearchPath(std::string Path)
{
    AudioDsoSearchPath = Path;
}
