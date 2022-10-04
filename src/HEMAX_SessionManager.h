#pragma once

#include "HEMAX_HAPISession.h"
#include "HEMAX_Node.h"
#include "HEMAX_Types.h"
#include "HEMAX_Store.h"

#include <string>

#define HEMAX_AUTO_PIPE_NAME "HEMAX_AutoPipeServer"
#define HEMAX_USE_SESSION_ENV_FLAG "USE_HENGINE_ENV_IN_HOSTS"

class HEMAX_Events;

class HEMAX_SessionManager
{
    public:
	static HEMAX_SessionManager& GetSessionManager();

	bool StartSession();
	void StopSession();
	bool IsSessionActive();

        void SetEventHub(HEMAX_Events* _PluginEvents);

	HEMAX_SessionType GetSessionType();
	void SetSessionType( HEMAX_SessionType SType );
	bool IsAutoSession();

	std::unordered_map<std::string, std::string> GetEnvMap();
	bool DoesUseHEngineFlagExist(std::unordered_map<std::string, std::string>& EnvMap);
	void CopyHEngineEnv(std::unordered_map<std::string, std::string>& EnvMap);

	HEMAX_HAPISession* Session;

	~HEMAX_SessionManager();

	void SetThriftNamedPipeSessionName(std::string PipeName);
	void SetThriftSocketHostName(std::string HostName);
	void SetThriftSocketPortNumber(int Port);

	void StartThriftNamedPipeThinClient();

    private:
	HEMAX_SessionManager();

	HEMAX_SessionType SessionType;

        HEMAX_Events* PluginEvents;

	void InitializeSession();

	bool IsActiveSession;
	bool IsSessionInitialized;
	bool AutoSession;
};
