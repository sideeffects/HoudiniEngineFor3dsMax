#include "HEMAX_SessionManager.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"
#include "HEMAX_Platform.h"
#include "HEMAX_Time.h"
#include "HEMAX_UserPrefs.h"
#include "HEMAX_Utilities.h"

#include <dbgprint.h>
#include <sstream>
#include <string>

#include <processthreadsapi.h>
#include <StopWatch.h>

// TODO: session refactor
// We need to set these in HARS
const char* const HAPI_CLIENT_NAME_ENV_VAR = "HAPI_CLIENT_NAME";
const char* const HAPI_CLIENT_NAME_ENV_VAL = "3dsmax";

HEMAX_SessionManager::HEMAX_SessionManager()
    : Store(Events)
{

}

HEMAX_SessionManager&
HEMAX_SessionManager::GetSessionManager()
{
    static HEMAX_SessionManager SessionManager;
    return SessionManager;
}

bool
HEMAX_SessionManager::CreateSession()
{
    HEMAX_Logger::Instance().AddEntry("Creating session...",
        HEMAX_LOG_LEVEL_INFO);

    if (IsSessionValidAndInitialized())
    {
        HEMAX_Logger::Instance().AddEntry("Cannot create a session because "
            "there is already an active session.", HEMAX_LOG_LEVEL_WARN);
        return false;
    }

    int SessionTypeVal;
    if (!HEMAX_UserPrefs::Get().GetIntSetting(HEMAX_SETTING_SESSION_TYPE,
            SessionTypeVal))
    {
        HEMAX_Logger::Instance().AddEntry("Could not create a session because "
            "the session type could not be determined.", HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HEMAX_SessionTypePref SessionType =
        static_cast<HEMAX_SessionTypePref>(SessionTypeVal);

    bool Result = false;

    if (SessionType == HEMAX_SessionTypePref::Socket)
    {
        Result = CreateSocketSession();
    }
    else if (SessionType == HEMAX_SessionTypePref::NamedPipe)
    {
        Result = CreateNamedPipeSession();
    }
    else if (SessionType == HEMAX_SessionTypePref::SharedMemory)
    {
        Result = CreateSharedMemorySession();
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry("Failed to create a session because "
            "an invalid session type was provided.", HEMAX_LOG_LEVEL_ERROR);
    }

    if (!Result)
        return false;

    InitializeSession();
   
    Events.EmitEvent(HEMAX_EventType::SessionChanged, nullptr);

    return true;
}

bool
HEMAX_SessionManager::ConnectSession()
{
    HEMAX_Logger::Instance().AddEntry("Connecting to session...",
        HEMAX_LOG_LEVEL_INFO);

    return TryConnectSession();
}

bool
HEMAX_SessionManager::StopSession()
{
    Events.EmitEvent(HEMAX_EventType::PreSessionStopped, nullptr);

    HEMAX_Logger::Instance().AddEntry("Stopping session...",
        HEMAX_LOG_LEVEL_INFO);

    if (HEMAX_HoudiniApi::IsSessionValid(&Session) != HAPI_RESULT_SUCCESS)
    {
        HEMAX_Logger::Instance().AddEntry("No active session found",
            HEMAX_LOG_LEVEL_INFO);
        return true;
    }

    if (HEMAX_HoudiniApi::IsInitialized(&Session) == HAPI_RESULT_SUCCESS)
        HEMAX_HoudiniApi::Cleanup(&Session);

    HEMAX_HoudiniApi::CloseSession(&Session);

    HEMAX_Logger::Instance().AddEntry("Session Stopped", HEMAX_LOG_LEVEL_INFO);

    Events.EmitEvent(HEMAX_EventType::SessionStopped, nullptr);
    Events.EmitEvent(HEMAX_EventType::SessionChanged, nullptr);

    return true;
}

bool
HEMAX_SessionManager::RestartSession()
{
    HEMAX_Logger::Instance().AddEntry("Restarting session...",
        HEMAX_LOG_LEVEL_INFO);

    if (!StopSession())
        return false;

    bool Success = CreateSession();

    Events.EmitEvent(HEMAX_EventType::SessionChanged, nullptr);

    return Success;
}

bool
HEMAX_SessionManager::OpenSessionSync()
{
    HEMAX_Logger::Instance().AddEntry("Opening Houdini SessionSync...",
            HEMAX_LOG_LEVEL_INFO);

    if (IsSessionValidAndInitialized())
    {
        bool Result = StopSession();

        if (!Result)
            return false;
    }

    int SessionTypeVal;
    if (!HEMAX_UserPrefs::Get().GetIntSetting(HEMAX_SETTING_SESSION_TYPE,
            SessionTypeVal))
    {
        HEMAX_Logger::Instance().AddEntry("Could not create a session because "
            "the session type could not be determined.", HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    std::stringstream Args;
    Args << "-hess=";

    HEMAX_SessionTypePref SessionType =
        static_cast<HEMAX_SessionTypePref>(SessionTypeVal);

    if (SessionType == HEMAX_SessionTypePref::Socket)
    {
        int Port;

        bool PortResult = HEMAX_UserPrefs::Get().GetIntSetting(
            HEMAX_SETTING_SESSION_PORT, Port);

        if (!PortResult)
            return false;

        Args << "port:" << std::to_string(Port);
    }
    else if (SessionType == HEMAX_SessionTypePref::NamedPipe)
    {
        std::string PipeName;

        bool PipeNameResult = HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_PIPE_NAME, PipeName);

        if (!PipeNameResult)
            return false;

        Args << "pipe:" << PipeName;
    }
    else if (SessionType == HEMAX_SessionTypePref::SharedMemory)
    {
        std::string SharedMemoryName, SharedMemoryBufferType;
        int SharedMemoryBufferSize;

        bool SharedMemoryNameResult = HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_NAME, SharedMemoryName);
        bool SharedMemoryBufferTypeResult = HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_TYPE, SharedMemoryBufferType);
        bool SharedMemoryBufferSizeResult = HEMAX_UserPrefs::Get().GetIntSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_SIZE, SharedMemoryBufferSize);

        if (!SharedMemoryNameResult || !SharedMemoryBufferTypeResult || !SharedMemoryBufferSizeResult)
            return false;

        Args << "shared:";

        if (SharedMemoryBufferType == HEMAX_SharedMemoryBufferType_Ring)
        {
            Args << "ring:";
        }
        else if (SharedMemoryBufferType == HEMAX_SharedMemoryBufferType_FixedLength)
        {
            Args << "fixed:";
        }
        else
        {
            std::stringstream ErrMsg;
            ErrMsg << "Cannot open Houdini Engine SessionSync because an "
                << "invalid shared memory buffer type was provided ("
                << SharedMemoryBufferType << ").";
            HEMAX_Logger::Instance().AddEntry(ErrMsg.str(),
                HEMAX_LOG_LEVEL_ERROR);
            return false;
        }

        Args << std::to_string(SharedMemoryBufferSize) << ":" << SharedMemoryName;
    }
    else
    {
        return false;
    }

    std::wstring HessArgs = HEMAX_Utilities::GetWideString(Args.str());

    if (!HEMAX_Platform::IsProcessRunning(HESSProcHandle))
    {
        STARTUPINFOW StartupInfo;
        PROCESS_INFORMATION ProcessInfo;

        ZeroMemory(&StartupInfo, sizeof(StartupInfo));
        StartupInfo.cb = sizeof(StartupInfo);
        ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));

        std::wstring HoudiniExecutable = HFSPath + L"/bin/houdini.exe ";
        HoudiniExecutable += HessArgs;

        bool CreateProcessResult = CreateProcessW(
                NULL,
                HoudiniExecutable.data(),
                NULL,
                NULL,
                false,
                0,
                NULL,
                NULL,
                &StartupInfo,
                &ProcessInfo);

        if (!CreateProcessResult)
        {
            DWORD ErrorCode = GetLastError();
            LPWSTR ErrorText = nullptr;

            FormatMessageW(
                    FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER
                    |FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    ErrorCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPWSTR)&ErrorText,
                    0,
                    NULL);

            std::string ErrMsg = "Could not open Houdini Engine SessionSync "
                "because Houdini could not be started: ";

            if (!ErrorText)
            {
                ErrMsg += "unknown reason.";
            }
            else
            {
                std::wstring WinErrMsgW(ErrorText);
                std::string WinErrMsg = HEMAX_Utilities::GetUtf8String(WinErrMsgW);

                ErrMsg += WinErrMsg;

                LocalFree(ErrorText);
                ErrorText = nullptr;
            }

            HEMAX_Logger::Instance().AddEntry(ErrMsg, HEMAX_LOG_LEVEL_ERROR);
            return false;
        }

        HESSProcHandle = ProcessInfo.hProcess;
    }

    MaxSDK::Util::StopWatch Timer;
    double TimeoutMS = 180*60*1000; // 3 minute timeout
    Timer.Start();

    HEMAX_Logger::Instance().AddEntry("Connecting to SessionSync...",
        HEMAX_LOG_LEVEL_INFO);

    HEMAX_Logger::Instance().SetHAPILoggingEnabled(false);
    while (!TryConnectSession(false))
    {
        DWORD ExitCode;
        if (GetExitCodeProcess(HESSProcHandle, &ExitCode) &&
            ExitCode != STILL_ACTIVE)
        {
            HEMAX_Logger::Instance().SetHAPILoggingEnabled(true);
            HESSProcHandle = nullptr;

            switch (ExitCode)
            {
                case 3:
                    HEMAX_Logger::Instance().AddEntry("Failed to start Houdini "
                        "SessionSync. No licenses were available.",
                        HEMAX_LOG_LEVEL_ERROR);
                    return false;
                    break;
                default:
                    HEMAX_Logger::Instance().AddEntry("Failed to start Houdini "
                        "SessionSync. Unknown reason.",
                        HEMAX_LOG_LEVEL_ERROR);
                    return false;
                    break;
            }
        }

        if (Timer.GetElapsedTime() >= TimeoutMS)
        {
            if (HEMAX_Platform::IsProcessRunning(HESSProcHandle))
                TerminateProcess(HESSProcHandle, 0);

            HEMAX_Logger::Instance().SetHAPILoggingEnabled(true);
            HESSProcHandle = nullptr;

            HEMAX_Logger::Instance().AddEntry("Timed out waiting for Houdini "
                "to start.", HEMAX_LOG_LEVEL_ERROR);

            return false;
        }
    }
    HEMAX_Logger::Instance().SetHAPILoggingEnabled(true);

    return true;
}

bool
HEMAX_SessionManager::CloseSessionSync()
{
    HEMAX_Logger::Instance().AddEntry("Closing Houdini SessionSync...",
            HEMAX_LOG_LEVEL_INFO);

    if (IsSessionValidAndInitialized())
       StopSession(); 

    if (HEMAX_Platform::IsProcessRunning(HESSProcHandle))
        TerminateProcess(HESSProcHandle, 0);

    HESSProcHandle = nullptr;

    HEMAX_Logger::Instance().AddEntry("Houdini SessionSync stopped.",
            HEMAX_LOG_LEVEL_INFO);

    return true;
}

bool
HEMAX_SessionManager::IsSessionValidAndInitialized()
{
    return (HEMAX_HoudiniApi::IsSessionValid(&Session) == HAPI_RESULT_SUCCESS &&
            HEMAX_HoudiniApi::IsInitialized(&Session) == HAPI_RESULT_SUCCESS);
}

bool
HEMAX_SessionManager::CreateSocketSession()
{
    std::string Host;
    if (!HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_HOST_NAME, Host))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to create a socket session "
            "because the hostname could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    int Port;
    if (!HEMAX_UserPrefs::Get().GetIntSetting(
            HEMAX_SETTING_SESSION_PORT, Port))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to create a socket session "
            "because the port number could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    } 

    HEMAX_HoudiniApi::ClearConnectionError();

    HAPI_ThriftServerOptions ServerOptions =
        HEMAX_HoudiniApi::ThriftServerOptions_Create();
    HAPI_ProcessId ProcessId;
    HAPI_Result Result = HEMAX_HoudiniApi::StartThriftSocketServer(
            &ServerOptions, Port, &ProcessId, nullptr);

    if (Result == HAPI_RESULT_SUCCESS)
    {
        std::stringstream SStream;
        SStream << "Thrift Socket Server started on " << Host
            << ":" << std::to_string(Port);
        std::string Msg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);
    }
    else
    {
        std::stringstream SStream;
        SStream << "Failed to start a Thrift socket server on " <<
            Host << ":" << std::to_string(Port);

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HEMAX_HoudiniApi::ClearConnectionError();

    HAPI_SessionInfo SessionInfo = HEMAX_HoudiniApi::SessionInfo_Create();
    Result = HEMAX_HoudiniApi::CreateThriftSocketSession(&Session,
        Host.c_str(), Port, &SessionInfo);

    if (Result != HAPI_RESULT_SUCCESS)
    {
        std::stringstream SStream;
        SStream << "Failed to create a Thrift socket session on " <<
            Host << ":" << std::to_string(Port);

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    std::stringstream SStream;
    SStream << "Successfully created a socket session on " << Host << ":"
        << std::to_string(Port);
    std::string Msg = SStream.str();
    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);
    
    return true;
}

bool
HEMAX_SessionManager::CreateNamedPipeSession()
{
    std::string PipeName;
    if (!HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_PIPE_NAME, PipeName))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to create a named pipe "
            "session because the pipe name could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HEMAX_HoudiniApi::ClearConnectionError();

    HAPI_ThriftServerOptions ServerOptions =
        HEMAX_HoudiniApi::ThriftServerOptions_Create();
    HAPI_ProcessId ProcessId;
    HAPI_Result Result = HEMAX_HoudiniApi::StartThriftNamedPipeServer(
            &ServerOptions, PipeName.c_str(), &ProcessId, nullptr);

    if (Result == HAPI_RESULT_SUCCESS)
    {
        std::stringstream SStream;
        SStream << "Thrift named pipe server started with name " << PipeName;
        std::string Msg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);
    }
    else
    {
        std::stringstream SStream;
        SStream << "Failed to start a Thrift named pipe server with name "
            << PipeName;

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HEMAX_HoudiniApi::ClearConnectionError();

    HAPI_SessionInfo SessionInfo = HEMAX_HoudiniApi::SessionInfo_Create();
    Result = HEMAX_HoudiniApi::CreateThriftNamedPipeSession(&Session,
        PipeName.c_str(), &SessionInfo);

    if (Result != HAPI_RESULT_SUCCESS)
    {
        std::stringstream SStream;
        SStream << "Failed to create a Thrift named pipe session with name "
            << PipeName;

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    std::stringstream SStream;
    SStream << "Successfully created a named pipe session with name " << PipeName;
    std::string Msg = SStream.str();
    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);

    return true;
}

bool
HEMAX_SessionManager::CreateSharedMemorySession()
{
    std::string SharedMemoryName;
    if (!HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_NAME, SharedMemoryName))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to create a shared memory "
            "session because the name could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    int BufferSize;
    if (!HEMAX_UserPrefs::Get().GetIntSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_SIZE, BufferSize))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to create a shared memory "
            "session because the buffer size could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    std::string BufferType;
    if (!HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_TYPE, BufferType))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to create a shared memory "
            "session because the buffer type could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HAPI_ThriftServerOptions ServerOptions =
        HEMAX_HoudiniApi::ThriftServerOptions_Create();

    if (BufferType == "Fixed")
    {
        ServerOptions.sharedMemoryBufferType = HAPI_THRIFT_SHARED_MEMORY_FIXED_LENGTH_BUFFER;
    }
    else if (BufferType == "Ring")
    {
        ServerOptions.sharedMemoryBufferType = HAPI_THRIFT_SHARED_MEMORY_RING_BUFFER;
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry("Failed to create a shared memory "
            "session because an invalid buffer type was provided.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    ServerOptions.sharedMemoryBufferSize = BufferSize;

    HEMAX_HoudiniApi::ClearConnectionError();
    HAPI_ProcessId ProcessId;
    HAPI_Result Result = HEMAX_HoudiniApi::StartThriftSharedMemoryServer(
        &ServerOptions, SharedMemoryName.c_str(), &ProcessId, nullptr);

    if (Result == HAPI_RESULT_SUCCESS)
    {
        std::stringstream SStream;
        SStream << "Thrift shared memory server started with name "
            << SharedMemoryName << " (" << std::to_string(BufferSize) << " MB "
            << BufferType << " Buffer)";
        std::string Msg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);
    }
    else
    {
        std::stringstream SStream;
        SStream << "Failed to start a Thrift shared memory server with name "
            << SharedMemoryName << " (" << std::to_string(BufferSize) << " MB "
            << BufferType << " Buffer)";

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HEMAX_HoudiniApi::ClearConnectionError();

    HAPI_SessionInfo SessionInfo = HEMAX_HoudiniApi::SessionInfo_Create();
    SessionInfo.sharedMemoryBufferType = ServerOptions.sharedMemoryBufferType;
    SessionInfo.sharedMemoryBufferSize = ServerOptions.sharedMemoryBufferSize;
    Result = HEMAX_HoudiniApi::CreateThriftSharedMemorySession(&Session,
        SharedMemoryName.c_str(), &SessionInfo);

    if (Result != HAPI_RESULT_SUCCESS)
    {
        std::stringstream SStream;
        SStream << "Failed to create a Thrift shared memory session with name "
            << SharedMemoryName << " (" << std::to_string(BufferSize) << " MB "
            << BufferType << " Buffer)";

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    std::stringstream SStream;
    SStream << "Successfully created a shared memory session with name "
        << SharedMemoryName << " (" << std::to_string(BufferSize) << " MB "
        << BufferType << " Buffer)";
    std::string Msg = SStream.str();
    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);

    return true;
}

bool
HEMAX_SessionManager::TryConnectSession(bool Required)
{
    if (IsSessionValidAndInitialized())
    {
        HEMAX_Logger::Instance().AddEntry("Cannot connect to a session because "
            "there is already an active session.", HEMAX_LOG_LEVEL_WARN);
        return false;
    }

    int SessionTypeVal;
    if (!HEMAX_UserPrefs::Get().GetIntSetting(HEMAX_SETTING_SESSION_TYPE,
            SessionTypeVal))
    {
        HEMAX_Logger::Instance().AddEntry("Could not connect to a session "
            "because the session type could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HEMAX_SessionTypePref SessionType =
        static_cast<HEMAX_SessionTypePref>(SessionTypeVal);

    bool Result = false;

    if (SessionType == HEMAX_SessionTypePref::Socket)
    {
        Result = ConnectSocketSession(Required);
    }
    else if (SessionType == HEMAX_SessionTypePref::NamedPipe)
    {
        Result = ConnectNamedPipeSession(Required);
    }
    else if (SessionType == HEMAX_SessionTypePref::SharedMemory)
    {
        Result = ConnectSharedMemorySession(Required);
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry("Failed to connect to a session "
            "because an invalid session type was provided.",
            HEMAX_LOG_LEVEL_ERROR);
    }

    if (!Result)
        return false;

    if (HEMAX_HoudiniApi::IsInitialized(&Session) != HAPI_RESULT_SUCCESS)
    {
        InitializeSession();
    }

    HEMAX_Logger::Instance().AddEntry("Finished connecting to session",
        HEMAX_LOG_LEVEL_INFO);

    Events.EmitEvent(HEMAX_EventType::SessionChanged, nullptr);

    return true;
}

bool
HEMAX_SessionManager::ConnectSocketSession(bool Required)
{
    std::string Host;
    if (!HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_HOST_NAME, Host))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to connect to a socket "
            "session because the hostname could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    int Port;
    if (!HEMAX_UserPrefs::Get().GetIntSetting(
            HEMAX_SETTING_SESSION_PORT, Port))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to connect to a socket "
            "session because the port number could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    } 

    HEMAX_HoudiniApi::ClearConnectionError();

    HAPI_SessionInfo SessionInfo = HEMAX_HoudiniApi::SessionInfo_Create();
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftSocketSession(&Session,
        Host.c_str(), Port, &SessionInfo);

    if (Result != HAPI_RESULT_SUCCESS)
    {
        // If the connection is not required, we don't need to log an error.
        if (!Required)
            return false;

        std::stringstream SStream;
        SStream << "Failed to connect to a Thrift socket session on " <<
            Host << ":" << std::to_string(Port);

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    std::stringstream SStream;
    SStream << "Successfully connected to a socket session on " << Host << ":"
        << std::to_string(Port);
    std::string Msg = SStream.str();
    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);

    return true;
}

bool
HEMAX_SessionManager::ConnectNamedPipeSession(bool Required)
{
    std::string PipeName;
    if (!HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_PIPE_NAME, PipeName))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to connect to a named pipe "
            "session because the pipe name could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HEMAX_HoudiniApi::ClearConnectionError();

    HAPI_SessionInfo SessionInfo = HEMAX_HoudiniApi::SessionInfo_Create();
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftNamedPipeSession(&Session,
        PipeName.c_str(), &SessionInfo);

    if (Result != HAPI_RESULT_SUCCESS)
    {
        // If the connection is not required, we don't need to log an error.
        if (!Required)
            return false;

        std::stringstream SStream;
        SStream << "Failed to connect to a Thrift named pipe session with name "
            << PipeName;

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    std::stringstream SStream;
    SStream << "Successfully connected to a named pipe session with name "
        << PipeName;
    std::string Msg = SStream.str();
    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);

    return true;
}

bool
HEMAX_SessionManager::ConnectSharedMemorySession(bool Required)
{
    std::string SharedMemoryName;
    if (!HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_NAME, SharedMemoryName))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to connect to a shared "
            "memory session because the name could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    int BufferSize;
    if (!HEMAX_UserPrefs::Get().GetIntSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_SIZE, BufferSize))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to connect to a shared "
            "memory session because the buffer size could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    std::string BufferType;
    if (!HEMAX_UserPrefs::Get().GetStringSetting(
            HEMAX_SETTING_SESSION_SHARED_MEMORY_BUFFER_TYPE, BufferType))
    {
        HEMAX_Logger::Instance().AddEntry("Failed to connect to a shared "
            "memory session because the buffer type could not be determined.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HAPI_ThriftSharedMemoryBufferType SharedMemoryBufferType;

    if (BufferType == "Fixed")
    {
        SharedMemoryBufferType = HAPI_THRIFT_SHARED_MEMORY_FIXED_LENGTH_BUFFER;
    }
    else if (BufferType == "Ring")
    {
        SharedMemoryBufferType = HAPI_THRIFT_SHARED_MEMORY_RING_BUFFER;
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry("Failed to connect to a shared "
            "memory session because an invalid buffer type was provided.",
            HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    HEMAX_HoudiniApi::ClearConnectionError();

    HAPI_SessionInfo SessionInfo = HEMAX_HoudiniApi::SessionInfo_Create();
    SessionInfo.sharedMemoryBufferType = SharedMemoryBufferType;
    SessionInfo.sharedMemoryBufferSize = BufferSize;
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftSharedMemorySession(
            &Session, SharedMemoryName.c_str(), &SessionInfo);

    if (Result != HAPI_RESULT_SUCCESS)
    {
        // If the connection is not required, we don't need to log an error.
        if (!Required)
            return false;

        std::stringstream SStream;
        SStream << "Failed to connect to a Thrift shared memory session with "
            "name " << SharedMemoryName << " (" << std::to_string(BufferSize)
            << " MB " << BufferType << " Buffer)";

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
        return false;
    }

    std::stringstream SStream;
    SStream << "Successfully connected to a shared memory session with name "
        << SharedMemoryName << " (" << std::to_string(BufferSize) << " MB "
        << BufferType << " Buffer)";
    std::string Msg = SStream.str();
    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);

    return true;
}

void
HEMAX_SessionManager::InitializeSession()
{
    std::string HoudiniEnv, OtlSearch, DsoSearch, ImageDsoSearch, AudioDsoSearch;

    HEMAX_UserPrefs::Get().GetStringSetting(HEMAX_SETTING_SESSION_ENV_FILES,
        HoudiniEnv);
    HEMAX_UserPrefs::Get().GetStringSetting(HEMAX_SETTING_SESSION_OTL_SEARCH,
        OtlSearch);
    HEMAX_UserPrefs::Get().GetStringSetting(HEMAX_SETTING_SESSION_DSO_SEARCH,
        DsoSearch);
    HEMAX_UserPrefs::Get().GetStringSetting(
        HEMAX_SETTING_SESSION_IMAGE_DSO_SEARCH, ImageDsoSearch);
    HEMAX_UserPrefs::Get().GetStringSetting(
        HEMAX_SETTING_SESSION_AUDIO_DSO_SEARCH, AudioDsoSearch);

    HAPI_CookOptions CookOptions = HEMAX_HoudiniApi::CookOptions_Create();
    CookOptions.packedPrimInstancingMode = HAPI_PACKEDPRIM_INSTANCING_MODE_FLAT;
    HAPI_Result InitResult = HEMAX_HoudiniApi::Initialize(&Session,
        &CookOptions, false, -1, HoudiniEnv.c_str(), OtlSearch.c_str(),
        DsoSearch.c_str(), ImageDsoSearch.c_str(), AudioDsoSearch.c_str());

    if (InitResult == HAPI_RESULT_SUCCESS)
    {
        HEMAX_Logger::Instance().AddEntry(
            "Session initalized and ready to use.", HEMAX_LOG_LEVEL_INFO);

        HEMAX_Time::PushTimelineSettings();
        HEMAX_Time::PushCurrentTime(GetCOREInterface()->GetTime());

        HEMAX_HoudiniApi::SetServerEnvString(&Session,
            HAPI_CLIENT_NAME_ENV_VAR, HAPI_CLIENT_NAME_ENV_VAL);

        Events.EmitEvent(HEMAX_EventType::SessionReady, nullptr);
    }
}
