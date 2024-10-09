#pragma once

#include "HEMAX_Events.h"
#include "HEMAX_Store.h"
#include "HEMAX_Types.h"

#include <HAPI_Common.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#define HEMAX_AUTO_PIPE_NAME "HEMAX_AutoPipeServer"
#define HEMAX_USE_SESSION_ENV_FLAG "USE_HENGINE_ENV_IN_HOSTS"

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

        bool                                OpenSessionSync();
        bool                                CloseSessionSync();

        bool                                IsSessionValidAndInitialized();

        HEMAX_Events&                       GetEvents() { return Events; }
        HEMAX_Store&                        GetStore() { return Store; }

        void                                SetHFSPath(const std::wstring& HFS)
                                                { HFSPath = HFS; }

        HAPI_Session                        Session;

    private:
	                                    HEMAX_SessionManager();

        bool                                CreateSocketSession();
        bool                                CreateNamedPipeSession();
        bool                                CreateSharedMemorySession();

        bool                                TryConnectSession(bool Required=true);
        bool                                ConnectSocketSession(bool Required);
        bool                                ConnectNamedPipeSession(bool Required);
        bool                                ConnectSharedMemorySession(bool Required);

        void                                InitializeSession();

        HEMAX_Events                        Events;
        HEMAX_Store                         Store;

        std::wstring                        HFSPath;

        HANDLE                              HESSProcHandle = nullptr;
};
