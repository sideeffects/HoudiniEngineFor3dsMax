#pragma once

#include "HEMAX_Types.h"
#include <HAPI_Common.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#define HEMAX_AUTO_PIPE_NAME "HEMAX_AutoPipeServer"
#define HEMAX_USE_SESSION_ENV_FLAG "USE_HENGINE_ENV_IN_HOSTS"

class HEMAX_Events;

class HEMAX_SessionManager
{

    public:
        
        // We don't stop the session in the destructor because we have nothing
        // to ensure a sane static destruction order. It's up to DllEntry.cpp
        // or HoudiniEngineFor3dsMax.cpp to make sure things are shutdown
        // cleanly.
                                            ~HEMAX_SessionManager() = default;

	static HEMAX_SessionManager&        GetSessionManager();

        bool                                CreateSession();
        bool                                ConnectSession();
        bool                                StopSession();
        bool                                RestartSession();

        bool                                IsSessionValidAndInitialized();

        void                                RegisterOnSessionChangedCallback(
                                                const std::function<void(void)>& CB);
        void                                RegisterOnSessionReadyCallback(
                                                const std::function<void(void)>& CB);
        void                                RegisterOnSessionStoppedCallback(
                                                const std::function<void(void)>& CB);

        HAPI_Session                        Session;

    private:
	                                    HEMAX_SessionManager() = default;

        bool                                CreateSocketSession();
        bool                                CreateNamedPipeSession();
        bool                                CreateSharedMemorySession();

        bool                                ConnectSocketSession();
        bool                                ConnectNamedPipeSession();
        bool                                ConnectSharedMemorySession();

        void                                InvokeOnSessionChangedCallbacks();
        void                                InvokeOnSessionReadyCallbacks();
        void                                InvokeOnSessionStoppedCallbacks();

        std::vector<std::function<void()> > OnSessionChangedCallbacks;
        std::vector<std::function<void()> > OnSessionReadyCallbacks;
        std::vector<std::function<void()> > OnSessionStoppedCallbacks;
};
