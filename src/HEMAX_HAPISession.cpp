#include "HEMAX_HAPISession.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Node.h"
#include "HEMAX_Logger.h"

HEMAX_HAPISession::HEMAX_HAPISession()
    : SeparateCookingThread(false)
    , CookingThreadStackSize( -1 )
{
}

HEMAX_HAPISession::~HEMAX_HAPISession() {}

HEMAX_HAPIThriftPipeSession::HEMAX_HAPIThriftPipeSession()
    : HEMAX_HAPISession()
{
}

HEMAX_HAPIThriftPipeSession::~HEMAX_HAPIThriftPipeSession() {}

HEMAX_HAPIThriftSocketSession::HEMAX_HAPIThriftSocketSession()
    : HEMAX_HAPISession()
{
}

HEMAX_HAPIThriftSocketSession::~HEMAX_HAPIThriftSocketSession() {}

HEMAX_CookResult
HEMAX_HAPISession::IsCookFinished()
{
    int Status;
    bool Result = HEMAX_HoudiniApi::GetStatus(this, HAPI_STATUS_COOK_STATE,
        &Status);

    if (!Result)
        return COOK_FAILED;
    else if (Status <= HAPI_STATE_MAX_READY_STATE)
        return COOK_FINISHED; 
    else
        return COOK_NOT_FINISHED;
}

bool
HEMAX_HAPISession::CreateSession()
{
    return HEMAX_HoudiniApi::CreateInProcessSession(this);
}

bool
HEMAX_HAPISession::InitializeHAPI(const char* HoudiniEnvFiles,
                                  const char* otlSearchPath,
                                  const char* dsoSearchPath,
                                  const char* imageDsoSearchPath,
                                  const char* audioDsoSearchPath)
{
    CookOptions.curveRefineLOD = 8.0f;
    CookOptions.clearErrorsAndWarnings = false;
    CookOptions.maxVerticesPerPrimitive = -1;
    CookOptions.splitGeosByGroup = false;
    CookOptions.splitGeosByAttribute = false;
    CookOptions.splitAttrSH = 0;
    CookOptions.refineCurveToLinear = false;
    CookOptions.handleBoxPartTypes = false;
    CookOptions.handleSpherePartTypes = false;
    CookOptions.splitPointsByVertexAttributes = false;
    CookOptions.packedPrimInstancingMode = HAPI_PACKEDPRIM_INSTANCING_MODE_FLAT;
    CookOptions.cacheMeshTopology = false;

    bool Result = HEMAX_HoudiniApi::Initialize(this,
                                               &CookOptions,
                                               SeparateCookingThread,
                                               CookingThreadStackSize,
	                                       HoudiniEnvFiles,
                                               otlSearchPath,
                                               dsoSearchPath,
                                               imageDsoSearchPath,
                                               audioDsoSearchPath);

    return Result;
}

bool
HEMAX_HAPIThriftSocketSession::CreateSession()
{
    bool Result = HEMAX_HoudiniApi::CreateThriftSocketSession(this,
        HostName.c_str(), Port);

    if (!Result)
    {
	std::string Msg = "Could not create session on host: " + HostName +
	    " / Port: " + std::to_string(Port);
	HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR);
    }

    return Result;
}

bool
HEMAX_HAPIThriftPipeSession::CreateSession()
{
    bool Result = HEMAX_HoudiniApi::CreateThriftNamedPipeSession(this,
        PipeName.c_str());

    if (!Result)
    {
	std::string Msg = "Could not create session with pipe name: " + PipeName;
	HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR);
    }

    return Result;
}

bool
HEMAX_HAPISession::LoadHoudiniDigitalAsset(const char* FilePath,
                                           bool AllowOverwrite,
	                                   HAPI_AssetLibraryId* AssetLibId,
                                           HEMAX_AssetLoadStatus* LoadStatus)
{
    HAPI_Result Result;
     
    bool Success = HEMAX_HoudiniApi::LoadAssetLibraryFromFile(this, FilePath,
        AllowOverwrite, AssetLibId, Result);

    if (Result == HAPI_RESULT_FAILURE)
    {
	(*LoadStatus) = HEMAX_ASSET_INVALID;
    }
    else if (Result == HAPI_RESULT_ASSET_DEF_ALREADY_LOADED)
    {
	(*LoadStatus) = HEMAX_ASSET_ALREADY_LOADED;
    }
    else if (Result == HAPI_RESULT_CANT_LOADFILE)
    {
	(*LoadStatus) = HEMAX_ASSET_NOT_FOUND;
    }
    else if (Result == HAPI_RESULT_ASSET_INVALID)
    {
	(*LoadStatus) = HEMAX_ASSET_INVALID;
    }
    else
    {
	(*LoadStatus) = HEMAX_ASSET_NO_STATUS;
    }

    return Success;
}

void
HEMAX_HAPIThriftSocketSession::SetHostName(std::string HName)
{
    HostName = HName;
}

void
HEMAX_HAPIThriftSocketSession::SetPortNumber(int PortNumber)
{
    Port = PortNumber;
}

void
HEMAX_HAPIThriftPipeSession::SetPipeName(std::string Name)
{
    PipeName = Name;
}

std::string
HEMAX_HAPISession::GetHAPIString(HAPI_StringHandle Handle)
{
    int StringBufferLength;
    HEMAX_HoudiniApi::GetStringBufLength(this, Handle, &StringBufferLength);

    if (StringBufferLength > 0)
    {
	char* StringResult = new char[StringBufferLength];
        HEMAX_HoudiniApi::GetString(this, Handle, StringResult,
            StringBufferLength);

	std::string ReturnString(StringResult);

	delete[] StringResult;

	return ReturnString;
    }
    else
    {
	return std::string("");
    }
}

std::string
HEMAX_HAPISession::GetHAPIErrorStatusString()
{
    int StrBufLen;
    HEMAX_HoudiniApi::GetStatusStringBufLength(this, HAPI_STATUS_CALL_RESULT,
        HAPI_STATUSVERBOSITY_ALL, &StrBufLen);

    if (StrBufLen > 0)
    {
	char* StringResult = new char[StrBufLen];
        HEMAX_HoudiniApi::GetStatusString(this, HAPI_STATUS_CALL_RESULT,
            StringResult, StrBufLen);

	std::string ReturnStr(StringResult);

	delete[] StringResult;

	return ReturnStr;
    }
    else
    {
	return std::string("");
    }
}
