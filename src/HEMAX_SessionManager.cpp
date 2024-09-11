#include "HEMAX_SessionManager.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"
#include "HEMAX_Time.h"
#include "HEMAX_UserPrefs.h"
#include "HEMAX_Utilities.h"

#include <dbgprint.h>
#include <sstream>
#include <string>

#pragma push_macro("HAPI_DECL_RETURN")
#undef HAPI_DECL_RETURN
#define HAPI_DECL_RETURN(x) x
#define HAPI_DECL_RETURN(x) x
#include <HAPI_Helpers.h>
#pragma pop_macro("HAPI_DECL_RETURN")

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

    HAPI_CookOptions CookOptions = HAPI_CookOptions_Create();
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

        Events.EmitEvent(HEMAX_EventType::SessionReady, nullptr);
    }
    
    Events.EmitEvent(HEMAX_EventType::SessionChanged, nullptr);

    return true;
}

bool
HEMAX_SessionManager::ConnectSession()
{
    HEMAX_Logger::Instance().AddEntry("Connecting to session...",
        HEMAX_LOG_LEVEL_INFO);
   
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
        Result = ConnectSocketSession();
    }
    else if (SessionType == HEMAX_SessionTypePref::NamedPipe)
    {
        Result = ConnectNamedPipeSession();
    }
    else if (SessionType == HEMAX_SessionTypePref::SharedMemory)
    {
        Result = ConnectSharedMemorySession();
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

        HAPI_CookOptions CookOptions = HAPI_CookOptions_Create();
        CookOptions.cacheMeshTopology = true;
        CookOptions.packedPrimInstancingMode = HAPI_PACKEDPRIM_INSTANCING_MODE_FLAT;
        HAPI_Result InitResult = HEMAX_HoudiniApi::Initialize(&Session,
            &CookOptions, false, -1, HoudiniEnv.c_str(), OtlSearch.c_str(),
            DsoSearch.c_str(), ImageDsoSearch.c_str(), AudioDsoSearch.c_str());

        if (InitResult)
        {
            HEMAX_Time::PushTimelineSettings();
            HEMAX_Time::PushCurrentTime(GetCOREInterface()->GetTime());

            Events.EmitEvent(HEMAX_EventType::SessionReady, nullptr);
        }
    }

    HEMAX_Logger::Instance().AddEntry("Finished connecting to session",
        HEMAX_LOG_LEVEL_INFO);

    Events.EmitEvent(HEMAX_EventType::SessionChanged, nullptr);

    return true;
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

    HAPI_ThriftServerOptions ServerOptions = HAPI_ThriftServerOptions_Create();
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

    HAPI_SessionInfo SessionInfo = HAPI_SessionInfo_Create();
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

    HAPI_ThriftServerOptions ServerOptions = HAPI_ThriftServerOptions_Create();
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

    HAPI_SessionInfo SessionInfo = HAPI_SessionInfo_Create();
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

    HAPI_ThriftServerOptions ServerOptions = HAPI_ThriftServerOptions_Create();

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

    HAPI_SessionInfo SessionInfo = HAPI_SessionInfo_Create();
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
HEMAX_SessionManager::ConnectSocketSession()
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

    HAPI_SessionInfo SessionInfo = HAPI_SessionInfo_Create();
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftSocketSession(&Session,
        Host.c_str(), Port, &SessionInfo);

    if (Result != HAPI_RESULT_SUCCESS)
    {
        std::stringstream SStream;
        SStream << "Failed to connect to a Thrift socket session on " <<
            Host << ":" << std::to_string(Port);

        std::string ConnectionError = HEMAX_Utilities::GetConnectionError();
        if (!ConnectionError.empty())
            SStream << ". Connection Error: " << ConnectionError;

        std::string ErrorMsg = SStream.str();
        HEMAX_Logger::Instance().AddEntry(ErrorMsg, HEMAX_LOG_LEVEL_ERROR);
    }

    std::stringstream SStream;
    SStream << "Successfully connected to a socket session on " << Host << ":"
        << std::to_string(Port);
    std::string Msg = SStream.str();
    HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_INFO);

    return true;
}

bool
HEMAX_SessionManager::ConnectNamedPipeSession()
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

    HAPI_SessionInfo SessionInfo = HAPI_SessionInfo_Create();
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftNamedPipeSession(&Session,
        PipeName.c_str(), &SessionInfo);

    if (Result != HAPI_RESULT_SUCCESS)
    {
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
HEMAX_SessionManager::ConnectSharedMemorySession()
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

    HAPI_SessionInfo SessionInfo = HAPI_SessionInfo_Create();
    SessionInfo.sharedMemoryBufferType = SharedMemoryBufferType;
    SessionInfo.sharedMemoryBufferSize = BufferSize;
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftSharedMemorySession(
            &Session, SharedMemoryName.c_str(), &SessionInfo);

    if (Result != HAPI_RESULT_SUCCESS)
    {
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

bool
HEMAX_SessionManager::IsSessionValidAndInitialized()
{
    return (HEMAX_HoudiniApi::IsSessionValid(&Session) == HAPI_RESULT_SUCCESS &&
            HEMAX_HoudiniApi::IsInitialized(&Session) == HAPI_RESULT_SUCCESS);
}
