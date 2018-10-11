#include "HEMAX_SessionManager.h"

HEMAX_SessionManager&
HEMAX_SessionManager::GetSessionManager()
{
    static HEMAX_SessionManager SessionManager;
    return SessionManager;
}

HEMAX_SessionManager::HEMAX_SessionManager()
    : SessionType( HEMAX_IN_PROCESS ), IsActiveSession(false), IsSessionInitialized(false)
{
    InitializeSession();
}

HEMAX_SessionManager::~HEMAX_SessionManager()
{
    StopSession();
    delete Session;
    IsActiveSession = false;
}

bool
HEMAX_SessionManager::StartSession(const char* HoudiniEnvFiles, const char* OtlSearchPath, const char* DsoSearchPath, const char* ImageDsoSearchPath, const char* AudioDsoSearchPath)
{
    if (Session->CreateSession())
    {
        if (IsSessionInitialized)
        {
            Session->Cleanup();
            IsSessionInitialized = false;
        }

        if (Session->InitializeHAPI(HoudiniEnvFiles, OtlSearchPath, DsoSearchPath, ImageDsoSearchPath, AudioDsoSearchPath))
        {
            IsActiveSession = true;
            IsSessionInitialized = true;

            int EnvVarCount;
            if (Session->GetServerEnvVarCount(&EnvVarCount))
            {
                HEMAX_StringHandle* EnvSH = new HEMAX_StringHandle[EnvVarCount];
                if (Session->GetServerEnvVarList(EnvSH, 0, EnvVarCount))
                {
                    std::unordered_map < std::string, std::string> EnvMap;

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

                                EnvMap.insert({std::string(EnvList[v].begin(), searchIt), Val});
                            }
                        }
                    }
                }
            }
        }
    }

    return IsActiveSession;
}

void
HEMAX_SessionManager::StopSession()
{
    if (Session->CloseSession())
    {
        IsActiveSession = false;
    }
}

bool
HEMAX_SessionManager::IsSessionActive()
{
    return IsActiveSession;
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

std::vector<HEMAX_ObjectInfo>
HEMAX_SessionManager::GetAllChildObjectNodes(HEMAX_Node* Node)
{
    int ObjectNodeCount;
    Session->ComposeObjectList(Node->Info.id, nullptr, &ObjectNodeCount);

    std::vector<HEMAX_ObjectInfo> ObjectInfos(ObjectNodeCount);

    Session->GetComposedObjectList(Node->Info.id, &ObjectInfos.front(), 0, ObjectNodeCount);

    return ObjectInfos;
}

std::vector<HEMAX_HAPITransform>
HEMAX_SessionManager::GetAllObjectTransforms(HEMAX_Node* Node)
{
    int ObjectNodeCount;
    Session->ComposeObjectList(Node->Info.id, nullptr, &ObjectNodeCount);

    std::vector<HEMAX_HAPITransform> TransformObjects(ObjectNodeCount);

    Session->GetComposedObjectTransforms(Node->Info.id, HAPI_RSTORDER_DEFAULT, &TransformObjects.front(), 0, ObjectNodeCount);

    return TransformObjects;
}

HEMAX_HAPITransform
HEMAX_SessionManager::GetObjectTransform(HEMAX_Node* Node)
{
    int ObjectNodeCount;
    Session->ComposeObjectList(Node->Info.id, nullptr, &ObjectNodeCount);

    HEMAX_HAPITransform Transform;

    Session->GetObjectTransform(Node->Info.id, -1, HAPI_RSTORDER_DEFAULT, &Transform);

    return Transform;
}

void
HEMAX_SessionManager::LoadAllAssetsInDirectory(std::string Directory, HEMAX_Store* AssetStore)
{
    if (!Directory.empty())
    {
        DWORD DirResult = GetFileAttributesA(Directory.c_str());

        if (DirResult != INVALID_FILE_ATTRIBUTES && (DirResult & FILE_ATTRIBUTE_DIRECTORY))
        {
            std::string PathHDA = Directory + "\\*.hda";
            std::string PathOTL = Directory + "\\*.otl";

            WIN32_FIND_DATAA FileData;
            HANDLE ResultHandle = FindFirstFileA(PathHDA.c_str(), &FileData);

            if (ResultHandle != INVALID_HANDLE_VALUE)
            {
                std::string FilePath(Directory + "\\" + FileData.cFileName);
                bool Success;
                AssetStore->LoadNewAsset(FilePath, Success);

                while (FindNextFileA(ResultHandle, &FileData))
                {
                    std::string FilePath(Directory + "\\" + FileData.cFileName);
                    AssetStore->LoadNewAsset(FilePath, Success);
                }
            }

            ResultHandle = FindFirstFileA(PathOTL.c_str(), &FileData);

            if (ResultHandle != INVALID_HANDLE_VALUE)
            {
                std::string FilePath(Directory + "\\" + FileData.cFileName);
                bool Success;
                AssetStore->LoadNewAsset(FilePath, Success);

                while (FindNextFileA(ResultHandle, &FileData))
                {
                    std::string FilePath(Directory + "\\" + FileData.cFileName);
                    AssetStore->LoadNewAsset(FilePath, Success);
                }
            }
        }
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
        HEMAX_HAPIThriftPipeSession* TheSession = (HEMAX_HAPIThriftPipeSession*)Session;
        HAPI_ThriftServerOptions ServerOptions;
        ServerOptions.autoClose = true;
        ServerOptions.timeoutMs = 5000.0f;
        HAPI_ProcessId ServerProcessId;
        TheSession->StartThriftNamedPipeServer(&ServerOptions, HEMAX_AUTO_PIPE_NAME, &ServerProcessId);
    }
}

std::vector<HEMAX_MaxTransform>
HEMAX_SessionManager::GetInstanceTransforms(HEMAX_NodeId InstancerNode)
{
    HEMAX_GeometryInfo GeoInfo;
    Session->GetDisplayGeoInfo(InstancerNode, &GeoInfo);

    HEMAX_PartInfo InstancerPointInfo;
    Session->GetPartInfo(GeoInfo.nodeId, 0, &InstancerPointInfo);

    HAPI_Transform* InstanceTransforms = new HAPI_Transform[InstancerPointInfo.pointCount];

    Session->GetInstanceTransforms(GeoInfo.nodeId, HAPI_RSTORDER_DEFAULT, InstanceTransforms, 0, InstancerPointInfo.pointCount);

    std::vector<HEMAX_MaxTransform> Transforms;

    for (int p = 0; p < InstancerPointInfo.pointCount; ++p)
    {
        Transforms.push_back(HEMAX_Utilities::HAPITransformToMaxTransform(InstanceTransforms[p]));
    }

    delete[] InstanceTransforms;

    return Transforms;
}

std::vector<HEMAX_MaxTransform>
HEMAX_SessionManager::GetInstancedPartTransforms(HEMAX_NodeId NodeId, HEMAX_PartId PartId, int InstancedPartCount)
{
    HAPI_Transform* HAPITransforms = new HAPI_Transform[InstancedPartCount];

    Session->GetInstancerPartTransforms(NodeId, PartId, HAPI_RSTORDER_DEFAULT, HAPITransforms, 0, InstancedPartCount);

    std::vector<HEMAX_MaxTransform> MaxTransforms;

    for (int t = 0; t < InstancedPartCount; ++t)
    {
        MaxTransforms.push_back(HEMAX_Utilities::HAPITransformToMaxTransform(HAPITransforms[t]));
    }

    delete[] HAPITransforms;

    return MaxTransforms;
}

std::vector<std::string>
HEMAX_SessionManager::GetResolvedStringsFromStringAttribute(HEMAX_NodeId Node, HEMAX_PartId PartId, const char* Name, HEMAX_AttributeInfo* AttributeInfo)
{
    HEMAX_StringHandle* StringHandles = new HEMAX_StringHandle[AttributeInfo->count];
    Session->GetAttributeStringData(Node, PartId, Name, AttributeInfo, StringHandles, 0, AttributeInfo->count);

    std::vector<std::string> ResolvedStrings;

    for (int s = 0; s < AttributeInfo->count; ++s)
    {
        ResolvedStrings.push_back(Session->GetHAPIString(StringHandles[s]));
    }

    delete[] StringHandles;

    return ResolvedStrings;
}
