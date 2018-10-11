#pragma once

#include "HEMAX_HAPISession.h"
#include "HEMAX_Node.h"
#include "HEMAX_Types.h"
#include "HEMAX_Store.h"

#include <string>

#define HEMAX_AUTO_PIPE_NAME "HEMAX_AutoPipeServer"

class HEMAX_SessionManager
{
public:
    static HEMAX_SessionManager& GetSessionManager();
    
    bool StartSession(const char* HoudiniEnvFiles, const char* OtlSearchPath, const char* DsoSearchPath, const char* ImageDsoSearchPath, const char* AudioDsoSearchPath);
    void StopSession();
    bool IsSessionActive();

    HEMAX_SessionType GetSessionType();
    void SetSessionType( HEMAX_SessionType SType );

    std::vector<HEMAX_ObjectInfo> GetAllChildObjectNodes(HEMAX_Node* Node);
    std::vector<HEMAX_HAPITransform> GetAllObjectTransforms(HEMAX_Node* Node);
    HEMAX_HAPITransform GetObjectTransform(HEMAX_Node* Node);

    std::vector<HEMAX_MaxTransform> GetInstanceTransforms(HEMAX_NodeId InstancerNode);
    std::vector<HEMAX_MaxTransform> GetInstancedPartTransforms(HEMAX_NodeId NodeId, HEMAX_PartId PartId, int InstancedPartCount);
    std::vector<std::string> GetResolvedStringsFromStringAttribute(HEMAX_NodeId Node, HEMAX_PartId PartId, const char* Name, HEMAX_AttributeInfo* AttributeInfo);
    
    void LoadAllAssetsInDirectory(std::string Directory, HEMAX_Store* AssetStore);

    HEMAX_HAPISession* Session;
    
    ~HEMAX_SessionManager();

    void SetThriftNamedPipeSessionName(std::string PipeName);
    void SetThriftSocketHostName(std::string HostName);
    void SetThriftSocketPortNumber(int Port);

    void StartThriftNamedPipeThinClient();
    
private:
    HEMAX_SessionManager();
   
    HEMAX_SessionType SessionType;

    void InitializeSession();

    bool IsActiveSession;
    bool IsSessionInitialized;
};
