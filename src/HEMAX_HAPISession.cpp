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

bool
HEMAX_HAPISession::HandleStatusResult(HAPI_Result Result)
{
    if (Result != HAPI_RESULT_SUCCESS)
    {
	std::string HAPIError = GetHAPIErrorStatusString();
	HEMAX_Logger::Instance().AddEntry(HAPIError, HEMAX_LOG_LEVEL_ERROR);
	return false;
    }

    return true;
}

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
HEMAX_HAPISession::IsSessionValid()
{
    return HEMAX_HoudiniApi::IsSessionValid(this);
}

bool
HEMAX_HAPISession::Cleanup()
{
    return HEMAX_HoudiniApi::Cleanup(this);
}

bool
HEMAX_HAPISession::CloseSession()
{
    return HEMAX_HoudiniApi::CloseSession(this);
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
    
    HEMAX_HoudiniApi::LoadAssetLibraryFromFile(this, FilePath, AllowOverwrite,
        AssetLibId, Result);

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

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetAvailableAssetCount(HAPI_AssetLibraryId AssetLibId,
                                          int* AssetCount)
{
    return HEMAX_HoudiniApi::GetAvailableAssetCount(this, AssetLibId,
        AssetCount);
}

bool
HEMAX_HAPISession::GetAvailableAssets(HAPI_AssetLibraryId AssetLibId,
                                      HAPI_StringHandle* AssetNames,
                                      int AssetCount)
{
    return HEMAX_HoudiniApi::GetAvailableAssets(this,
                                                AssetLibId,
                                                AssetNames,
                                                AssetCount);
}

bool
HEMAX_HAPISession::GetStringBufferLength(HAPI_StringHandle StringHandle,
                                         int* StringBufferLength)
{
    return HEMAX_HoudiniApi::GetStringBufLength(this,
                                                StringHandle,
                                                StringBufferLength);
}

bool
HEMAX_HAPISession::GetString(HAPI_StringHandle StringHandle,
                             char* StringValue,
                             int StringBufferLength)
{
    return HEMAX_HoudiniApi::GetString(this,
                                       StringHandle,
                                       StringValue,
                                       StringBufferLength);
}

bool
HEMAX_HAPISession::CreateNode(HAPI_NodeId ParentNode,
                              const char* OperatorName,
                              const char* NodeLabel,
                              HAPI_Bool CookOnCreation,
	                      HAPI_NodeId* NewNodeId)
{
    return HEMAX_HoudiniApi::CreateNode(this,
                                        ParentNode,
                                        OperatorName,
                                        NodeLabel,
	                                CookOnCreation,
                                        NewNodeId);
}

bool
HEMAX_HAPISession::DeleteNode(HAPI_NodeId NodeId)
{
    return HEMAX_HoudiniApi::DeleteNode(this, NodeId);
}

bool
HEMAX_HAPISession::GetAssetInfo(HAPI_NodeId Node,
                                HAPI_AssetInfo* AssetInfo)
{
    return HEMAX_HoudiniApi::GetAssetInfo(this,
                                          Node,
                                          AssetInfo);
}

bool
HEMAX_HAPISession::CookNode(HAPI_NodeId Node)
{
    return HEMAX_HoudiniApi::CookNode(this, Node, &CookOptions);
}

bool
HEMAX_HAPISession::GetNodeInfo(HAPI_NodeId Node,
                               HAPI_NodeInfo* NodeInfo)
{
    return HEMAX_HoudiniApi::GetNodeInfo(this,
                                         Node,
                                         NodeInfo);
}

bool
HEMAX_HAPISession::GetNodeInputName(HAPI_NodeId Node,
                                    int InputIndex,
                                    HAPI_StringHandle* nameSH)
{
    return HEMAX_HoudiniApi::GetNodeInputName(this,
                                              Node,
                                              InputIndex,
                                              nameSH);
}

bool
HEMAX_HAPISession::GetObjectInfo(HAPI_NodeId Node,
                                 HAPI_ObjectInfo* ObjectInfo)
{
    return HEMAX_HoudiniApi::GetObjectInfo(this,
                                           Node,
                                           ObjectInfo);
}

bool
HEMAX_HAPISession::GetGeometryInfo(HAPI_NodeId Node,
                                   HAPI_GeoInfo* GeoInfo)
{
#ifdef GetGeoInfo
#undef GetGeoInfo
#endif
    return HEMAX_HoudiniApi::GetGeoInfo(this, Node, GeoInfo);
#define GetGeoInfo GetGeoInfoW
}

bool
HEMAX_HAPISession::GetDisplayGeoInfo(HAPI_NodeId Node,
                                     HAPI_GeoInfo* GeoInfo)
{
    return HEMAX_HoudiniApi::GetDisplayGeoInfo(this,
                                               Node,
                                               GeoInfo);
}

bool
HEMAX_HAPISession::GetPartInfo(HAPI_NodeId Node,
                               HAPI_PartId PartId,
                               HAPI_PartInfo* PartInfo)
{
    return HEMAX_HoudiniApi::GetPartInfo(this,
                                         Node,
                                         PartId,
                                         PartInfo);
}

bool
HEMAX_HAPISession::GetEdgeCountOfEdgeGroup(HAPI_NodeId Node,
                                           HAPI_PartId PartId,
                                           const char* GroupName,
                                           int* EdgeCount)
{
    return HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroup(this,
                                                     Node,
                                                     PartId,
                                                     GroupName,
                                                     EdgeCount);
}

bool
HEMAX_HAPISession::GetFaceCounts(HAPI_NodeId Node,
                                 HAPI_PartId PartId,
                                 int* FaceCountsArray,
                                 int Start,
                                 int Length)
{
    return HEMAX_HoudiniApi::GetFaceCounts(this,
                                           Node,
                                           PartId,
                                           FaceCountsArray,
                                           Start,
                                           Length);
}

bool
HEMAX_HAPISession::GetVertexList(HAPI_NodeId Node,
                                 HAPI_PartId PartId,
                                 int* VertexListArray,
                                 int Start,
                                 int Length)
{
    return HEMAX_HoudiniApi::GetVertexList(this,
                                           Node,
                                           PartId,
                                           VertexListArray,
                                           Start,
                                           Length);
}

bool
HEMAX_HAPISession::GetAttributeNames(HAPI_NodeId Node,
                                     HAPI_PartId PartId,
                                     HAPI_AttributeOwner Owner,
                                     HAPI_StringHandle* AttributeNamesArray,
                                     int Count)
{
    return HEMAX_HoudiniApi::GetAttributeNames(this,
                                               Node,
                                               PartId,
                                               Owner,
                                               AttributeNamesArray,
                                               Count);
}

bool
HEMAX_HAPISession::GetAttributeInfo(HAPI_NodeId Node,
                                    HAPI_PartId PartId,
                                    const char* Name,
                                    HAPI_AttributeOwner Owner,
                                    HAPI_AttributeInfo* AttributeInfo)
{
    return HEMAX_HoudiniApi::GetAttributeInfo(
                                            this,
                                            Node,
                                            PartId,
                                            Name,
                                            HAPI_AttributeOwner(Owner),
                                            AttributeInfo);
}

bool
HEMAX_HAPISession::GetAttributeIntData(HAPI_NodeId Node,
                                       HAPI_PartId PartId,
                                       const char* Name,
                                       HAPI_AttributeInfo* AttributeInfo,
	                               int Stride,
                                       int* IntDataArray,
                                       int Start,
                                       int Length)
{
    return HEMAX_HoudiniApi::GetAttributeIntData(this,
                                                 Node,
                                                 PartId,
                                                 Name,
                                                 AttributeInfo,
                                                 Stride,
                                                 IntDataArray,
                                                 Start,
                                                 Length);
}

bool
HEMAX_HAPISession::GetAttributeFloatData(HAPI_NodeId Node,
                                         HAPI_PartId PartId,
                                         const char* Name,
                                         HAPI_AttributeInfo* AttributeInfo,
	                                 int Stride,
                                         float* FloatDataArray,
                                         int Start,
                                         int Length)
{
    return HEMAX_HoudiniApi::GetAttributeFloatData(
                                                this,
                                                Node,
                                                PartId,
                                                Name,
                                                AttributeInfo,
                                                Stride,
                                                FloatDataArray,
                                                Start,
                                                Length);
}

bool
HEMAX_HAPISession::GetAttributeStringData(
                            HAPI_NodeId Node,
                            HAPI_PartId PartId,
                            const char* Name,
                            HAPI_AttributeInfo* AttributeInfo,
	                    HAPI_StringHandle* StringHandleDataArray,
                            int Start,
                            int Length)
{
    return HEMAX_HoudiniApi::GetAttributeStringData(
                                                    this,
                                                    Node,
                                                    PartId,
                                                    Name,
                                                    AttributeInfo,
                                                    StringHandleDataArray,
                                                    Start,
                                                    Length);
}

bool
HEMAX_HAPISession::GetParameters(HAPI_NodeId Node,
                                 HAPI_ParmInfo* ParmInfosArray,
                                 int Start,
                                 int Length)
{
    return HEMAX_HoudiniApi::GetParameters(
                                                this,
                                                Node,
                                                ParmInfosArray,
                                                Start,
                                                Length);
}

bool
HEMAX_HAPISession::GetParameterInfo(HAPI_NodeId Node,
                                    HAPI_ParmId ParmId,
                                    HAPI_ParmInfo* ParmInfo)
{
    return HEMAX_HoudiniApi::GetParmInfo(this,
                                         Node,
                                         ParmId,
                                         ParmInfo);
}

bool
HEMAX_HAPISession::GetParameterIntValues(HAPI_NodeId Node,
                                         int* ValuesArray,
                                         int Start,
                                         int Length)
{
    return HEMAX_HoudiniApi::GetParmIntValues(this,
                                              Node,
                                              ValuesArray,
                                              Start,
                                              Length);
}

bool
HEMAX_HAPISession::GetParameterFloatValues(HAPI_NodeId Node,
                                           float* ValuesArray,
                                           int Start,
                                           int Length)
{
    return HEMAX_HoudiniApi::GetParmFloatValues(this,
                                                Node,
                                                ValuesArray,
                                                Start,
                                                Length);
}

bool
HEMAX_HAPISession::GetParameterStringValues(HAPI_NodeId Node,
                                            bool Evaluate,
                                            HAPI_StringHandle* ValuesArray,
                                            int Start,
                                            int Length)
{
    return HEMAX_HoudiniApi::GetParmStringValues(this,
                                                 Node,
                                                 Evaluate,
                                                 ValuesArray,
                                                 Start,
                                                 Length);
}

bool
HEMAX_HAPISession::GetParameterChoiceLists(
                            HAPI_NodeId Node,
                            HAPI_ParmChoiceInfo* ParmChoicesArray,
                            int Start,
                            int Length)
{
    return HEMAX_HoudiniApi::GetParmChoiceLists(this,
                                                Node,
                                                ParmChoicesArray,
                                                Start,
                                                Length);
}

bool
HEMAX_HAPISession::GetParameterIdFromName(HAPI_NodeId Node,
                                          const char* ParmName,
                                          HAPI_ParmId* ParmId)
{
    return HEMAX_HoudiniApi::GetParmIdFromName(this,
                                               Node,
                                               ParmName,
                                               ParmId);
}

bool
HEMAX_HAPISession::SetParameterIntValues(HAPI_NodeId Node,
                                         int* ValuesArray,
                                         int Start,
                                         int Length)
{
    return HEMAX_HoudiniApi::SetParmIntValues(
                                                this,
                                                Node,
                                                ValuesArray,
                                                Start,
                                                Length);
}

bool
HEMAX_HAPISession::SetParameterFloatValues(HAPI_NodeId Node,
                                           float* ValuesArray,
                                           int Start,
                                           int Length)
{
    return HEMAX_HoudiniApi::SetParmFloatValues(this,
                                                Node,
                                                ValuesArray,
                                                Start,
                                                Length);
}

bool
HEMAX_HAPISession::SetParameterStringValue(HAPI_NodeId Node,
                                           const char* StringValue,
                                           HAPI_ParmId ParmId,
                                           int Index)
{
    return HEMAX_HoudiniApi::SetParmStringValue(this,
                                                Node,
                                                StringValue,
                                                ParmId,
                                                Index);
}

bool
HEMAX_HAPISession::InsertMultiParameterInstance(HAPI_NodeId Node,
                                                HAPI_ParmId Parameter,
                                                int Position)
{
    return HEMAX_HoudiniApi::InsertMultiparmInstance(this,
                                                     Node,
                                                     Parameter,
                                                     Position);
}

bool
HEMAX_HAPISession::RemoveMultiParameterInstance(HAPI_NodeId Node,
                                                HAPI_ParmId Parameter,
                                                int Position)
{
    return HEMAX_HoudiniApi::RemoveMultiparmInstance(this,
                                                     Node,
                                                     Parameter,
                                                     Position);
}

bool
HEMAX_HAPISession::GetComposedObjectTransforms(HAPI_NodeId Node,
                                               HAPI_RSTOrder RSTOrder,
                                               HAPI_Transform* TransformArray,
                                               int Start,
                                               int Length)
{
    return HEMAX_HoudiniApi::GetComposedObjectTransforms(
                                                    this,
                                                    Node,
                                                    RSTOrder,
                                                    TransformArray,
                                                    Start,
                                                    Length);
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

bool
HEMAX_HAPISession::ComposeObjectList(HAPI_NodeId Node,
                                     const char* Categories,
                                     int* ObjectCount)
{
    return HEMAX_HoudiniApi::ComposeObjectList(this,
                                               Node,
                                               Categories,
                                               ObjectCount);
}

bool
HEMAX_HAPISession::GetComposedObjectList(HAPI_NodeId Node,
                                         HAPI_ObjectInfo* ObjectInfosArray,
                                         int Start,
                                         int Length)
{
    return HEMAX_HoudiniApi::GetComposedObjectList(
                                                this,
                                                Node,
                                                ObjectInfosArray,
                                                Start,
                                                Length);
}

bool
HEMAX_HAPISession::ComposeChildNodeList(HAPI_NodeId Node,
                                        HAPI_NodeTypeBits NodeTypeFilter,
                                        HAPI_NodeFlagsBits NodeFlagFilter,
                                        HAPI_Bool Recursive,
                                        int* Count)
{
    return HEMAX_HoudiniApi::ComposeChildNodeList(this,
                                                  Node,
                                                  NodeTypeFilter,
                                                  NodeFlagFilter,
                                                  Recursive,
                                                  Count );
}

bool
HEMAX_HAPISession::GetComposedChildNodeList(HAPI_NodeId Node,
                                            HAPI_NodeId* ChildNodeIdsArray,
                                            int Count)
{
    return HEMAX_HoudiniApi::GetComposedChildNodeList(
                                                this,
                                                Node,
                                                ChildNodeIdsArray,
                                                Count);
}

bool
HEMAX_HAPISession::GetObjectTransform(HAPI_NodeId Node,
                                      HAPI_NodeId RelativeToNodeId,
                                      HAPI_RSTOrder RSTOrder,
                                      HAPI_Transform* Transform)
{
    return HEMAX_HoudiniApi::GetObjectTransform(
                                                this,
                                                Node,
                                                RelativeToNodeId,
                                                RSTOrder,
                                                Transform);
}

bool
HEMAX_HAPISession::GetParameterNodeValue(HAPI_NodeId Node,
                                         const char* ParameterName,
                                         HAPI_NodeId* Value)
{
    return HEMAX_HoudiniApi::GetParmNodeValue(this,
                                              Node,
                                              ParameterName,
                                              Value);
}

bool
HEMAX_HAPISession::SetParameterNodeValue(HAPI_NodeId Node,
                                         const char* ParameterName,
                                         HAPI_NodeId Value)
{
    return HEMAX_HoudiniApi::SetParmNodeValue(this,
                                              Node,
                                              ParameterName,
                                              Value);
}

bool
HEMAX_HAPISession::CreateInputNode(HAPI_NodeId* Node,
                                   const char* NodeName)
{
    return HEMAX_HoudiniApi::CreateInputNode(this,
                                             Node,
                                             NodeName);
}

bool
HEMAX_HAPISession::SetObjectTransform(HAPI_NodeId Node,
                                      const HAPI_TransformEuler* Transform)
{
    return HEMAX_HoudiniApi::SetObjectTransform(this,
                                                Node,
                                                Transform);
}

bool
HEMAX_HAPISession::SaveHIPFile(const char* FilePath,
                               HAPI_Bool LockNodes)
{
    return HEMAX_HoudiniApi::SaveHIPFile(this,
                                         FilePath,
                                         LockNodes);
}

bool
HEMAX_HAPISession::CommitGeometry(HAPI_NodeId Node)
{
    return HEMAX_HoudiniApi::CommitGeo(this, Node);
}

bool
HEMAX_HAPISession::SetPartInfo(HAPI_NodeId Node,
                               HAPI_PartId PartId,
                               const HAPI_PartInfo* PartInfo)
{
    return HEMAX_HoudiniApi::SetPartInfo(this,
                                         Node,
                                         PartId,
                                         PartInfo);
}

bool
HEMAX_HAPISession::AddAttribute(HAPI_NodeId Node,
                                HAPI_PartId PartId,
                                const char* Name,
                                const HAPI_AttributeInfo* AttributeInfo)
{
    return HEMAX_HoudiniApi::AddAttribute(this,
                                          Node,
                                          PartId,
                                          Name,
                                          AttributeInfo);
}

bool
HEMAX_HAPISession::SetAttributeFloatData(
                            HAPI_NodeId Node,
                            HAPI_PartId Part,
                            const char* Name,
                            const HAPI_AttributeInfo* AttributeInfo,
                            const float* DataArray,
                            int Start,
                            int Length)
{
    return HEMAX_HoudiniApi::SetAttributeFloatData(
                                                        this,
                                                        Node,
                                                        Part,
                                                        Name,
                                                        AttributeInfo,
                                                        DataArray,
                                                        Start,
                                                        Length);
}

bool
HEMAX_HAPISession::SetAttributeIntData(HAPI_NodeId Node,
                                       HAPI_PartId Part,
                                       const char* Name,
                                       const HAPI_AttributeInfo* AttributeInfo,
                                       const int* DataArray,
                                       int Start,
                                       int Length)
{
    return HEMAX_HoudiniApi::SetAttributeIntData(this,
                                                 Node,
                                                 Part,
                                                 Name,
                                                 AttributeInfo,
                                                 DataArray,
                                                 Start,
                                                 Length);
}

bool
HEMAX_HAPISession::SetAttributeStringData(
                                HAPI_NodeId Node,
                                HAPI_PartId Part,
                                const char* Name,
                                const HAPI_AttributeInfo* AttributeInfo,
                                const char** DataArray,
                                int Start,
                                int Length)
{
    return HEMAX_HoudiniApi::SetAttributeStringData(this,
                                                    Node,
                                                    Part,
                                                    Name,
                                                    AttributeInfo,
                                                    DataArray,
                                                    Start,
                                                    Length);
}

bool
HEMAX_HAPISession::SetVertexList(HAPI_NodeId Node,
                                 HAPI_PartId Part,
                                 const int* VertexListArray,
                                 int Start,
                                 int Length)
{
    return HEMAX_HoudiniApi::SetVertexList(this,
                                           Node,
                                           Part,
                                           VertexListArray,
                                           Start,
                                           Length);
}

bool
HEMAX_HAPISession::SetFaceCounts(HAPI_NodeId Node,
                                 HAPI_PartId Part,
                                 const int* FaceCountsArray,
                                 int Start,
                                 int Length)
{
    return HEMAX_HoudiniApi::SetFaceCounts(this,
                                           Node,
                                           Part,
                                           FaceCountsArray,
                                           Start,
                                           Length);
}

bool
HEMAX_HAPISession::GetInstanceTransformsOnPart(HAPI_NodeId Node,
                                               HAPI_PartId Part,
                                               HAPI_RSTOrder RSTOrder,
                                               HAPI_Transform* TransformsArray,
                                               int Start,
                                               int Length)
{
    return HEMAX_HoudiniApi::GetInstanceTransformsOnPart(
                                                            this,
                                                            Node,
                                                            Part,
                                                            RSTOrder,
                                                            TransformsArray,
                                                            Start,
                                                            Length);
}

bool
HEMAX_HAPISession::StartThriftNamedPipeServer(
                            const HAPI_ThriftServerOptions* Options,
                            const char* PipeName,
                            HAPI_ProcessId* ProcessId,
                            const char* LogFile)
{
    return HEMAX_HoudiniApi::StartThriftNamedPipeServer(
                                                Options,
                                                PipeName,
                                                ProcessId,
                                                LogFile);
}

bool
HEMAX_HAPISession::StartThriftSocketServer(
                            const HAPI_ThriftServerOptions* Options,
                            int Port,
                            HAPI_ProcessId* ProcessId,
                            const char* LogFile)
{
    return HEMAX_HoudiniApi::StartThriftSocketServer(
                                                Options,
                                                Port,
                                                ProcessId,
                                                LogFile);
}

bool
HEMAX_HAPISession::ConnectNodeInput(HAPI_NodeId Node,
                                    int InputIndex,
                                    HAPI_NodeId NodeToConnect,
                                    int OutputIndex)
{
    return HEMAX_HoudiniApi::ConnectNodeInput(this,
                                              Node,
                                              InputIndex,
                                              NodeToConnect,
                                              OutputIndex);
}

bool
HEMAX_HAPISession::DisconnectNodeInput(HAPI_NodeId Node,
                                       int InputIndex)
{
    return HEMAX_HoudiniApi::DisconnectNodeInput(this,
                                                 Node,
                                                 InputIndex);
}

bool
HEMAX_HAPISession::GetMaterialNodeIdsOnFaces(HAPI_NodeId GeometryNode,
                                             HAPI_PartId Part,
                                             bool* AreAllTheSame,
                                             HAPI_NodeId* MaterialIdsArray,
                                             int Start,
                                             int Length)
{
    return HEMAX_HoudiniApi::GetMaterialNodeIdsOnFaces(
                                                            this,
                                                            GeometryNode,
                                                            Part,
                                                            AreAllTheSame,
                                                            MaterialIdsArray,
                                                            Start,
                                                            Length);
}

bool
HEMAX_HAPISession::GetMaterialInfo(HAPI_NodeId MaterialNode,
                                   HAPI_MaterialInfo* MaterialInfo)
{
    return HEMAX_HoudiniApi::GetMaterialInfo(this,
                                             MaterialNode,
                                             MaterialInfo);
}

bool
HEMAX_HAPISession::RenderTextureToImage(HAPI_NodeId MaterialNode,
                                        HAPI_ParmId TextureMapFilePath)
{
    return HEMAX_HoudiniApi::RenderTextureToImage(
                                                this,
                                                MaterialNode,
                                                TextureMapFilePath);
}

bool
HEMAX_HAPISession::GetImageInfo(HAPI_NodeId MaterialNode,
                                HAPI_ImageInfo* ImageInfo)
{
    return HEMAX_HoudiniApi::GetImageInfo(this,
                                          MaterialNode,
                                          ImageInfo);
}

bool
HEMAX_HAPISession::SetImageInfo(HAPI_NodeId MaterialNode,
                                const HAPI_ImageInfo* ImageInfo)
{
    return HEMAX_HoudiniApi::SetImageInfo(this,
                                          MaterialNode,
                                          ImageInfo);
}

bool
HEMAX_HAPISession::ExtractImageToFile(HAPI_NodeId MaterialNode,
                                      const char* ImageFileFormatName,
                                      const char* ImagePlanes,
                                      const char* DestinationFolderPath,
	                              const char* DestinationFileName,
                                      int* DestinationFilePath)
{
    return HEMAX_HoudiniApi::ExtractImageToFile(
                                            this,
                                            MaterialNode,
                                            ImageFileFormatName,
                                            ImagePlanes,
                                            DestinationFolderPath,
                                            DestinationFileName,
                                            DestinationFilePath);
}

bool
HEMAX_HAPISession::ExtractImageToMemory(HAPI_NodeId MaterialNode,
                                        const char* ImageFileFormatName,
                                        const char* ImagePlanes, int* BufferSize)
{
    return HEMAX_HoudiniApi::ExtractImageToMemory(
                                                this,
                                                MaterialNode,
                                                ImageFileFormatName,
                                                ImagePlanes,
                                                BufferSize);
}

bool
HEMAX_HAPISession::GetImageMemoryBuffer(HAPI_NodeId MaterialNode,
                                        char* Buffer,
                                        int Length)
{
    return HEMAX_HoudiniApi::GetImageMemoryBuffer(this,
                                                  MaterialNode,
                                                  Buffer,
                                                  Length);
}

bool
HEMAX_HAPISession::GetInstancedObjectIds(HAPI_NodeId InstancerNodeId,
                                         HAPI_NodeId* InstancedNodeIdArray,
                                         int Start,
                                         int Length)
{
    return HEMAX_HoudiniApi::GetInstancedObjectIds(
                                        this,
                                        InstancerNodeId,
                                        InstancedNodeIdArray,
                                        Start,
                                        Length);
}

bool
HEMAX_HAPISession::GetInstancedPartIds(HAPI_NodeId NodeId,
                                       HAPI_PartId PartId,
                                       HAPI_PartId* PartIdArray,
                                       int Start,
                                       int Length)
{
    return HEMAX_HoudiniApi::GetInstancedPartIds(this,
                                                 NodeId,
                                                 PartId,
                                                 PartIdArray,
                                                 Start,
                                                 Length);
}

bool
HEMAX_HAPISession::GetInstancerPartTransforms(HAPI_NodeId NodeId,
                                              HAPI_PartId PartId,
                                              HAPI_RSTOrder RSTOrder,
                                              HAPI_Transform* TransformArray,
                                              int Start,
                                              int Length)
{
    return HEMAX_HoudiniApi::GetInstancerPartTransforms(
                                                        this,
                                                        NodeId,
                                                        PartId,
                                                        RSTOrder,
                                                        TransformArray,
                                                        Start,
                                                        Length);
}

bool
HEMAX_HAPISession::GetCurveInfo(HAPI_NodeId NodeId,
                                HAPI_PartId PartId,
                                HAPI_CurveInfo* CurveInfo)
{
    return HEMAX_HoudiniApi::GetCurveInfo(this,
                                          NodeId,
                                          PartId,
                                          CurveInfo);
}

bool
HEMAX_HAPISession::GetCurveCounts(HAPI_NodeId NodeId,
                                  HAPI_PartId PartId,
                                  int* CountsArray,
                                  int Start,
                                  int Length)
{
    return HEMAX_HoudiniApi::GetCurveCounts(this,
                                            NodeId,
                                            PartId,
                                            CountsArray,
                                            Start,
                                            Length);
}

bool
HEMAX_HAPISession::GetCurveOrders(HAPI_NodeId NodeId,
                                  HAPI_PartId PartId,
                                  int* OrdersArray,
                                  int Start,
                                  int Length)
{
    return HEMAX_HoudiniApi::GetCurveOrders(this,
                                            NodeId,
                                            PartId,
                                            OrdersArray,
                                            Start,
                                            Length);
}

bool
HEMAX_HAPISession::GetCurveKnots(HAPI_NodeId NodeId,
                                 HAPI_PartId PartId,
                                 float* KnotsArray,
                                 int Start,
                                 int Length)
{
    return HEMAX_HoudiniApi::GetCurveKnots(this, 
                                           NodeId,
                                           PartId,
                                           KnotsArray,
                                           Start,
                                           Length);
}

bool
HEMAX_HAPISession::SetCurveInfo(HAPI_NodeId NodeId,
                                HAPI_PartId PartId,
                                const HAPI_CurveInfo* CurveInfo)
{
    return HEMAX_HoudiniApi::SetCurveInfo(this,
                                          NodeId,
                                          PartId,
                                          CurveInfo);
}

bool
HEMAX_HAPISession::SetCurveOrders(HAPI_NodeId NodeId,
                                  HAPI_PartId PartId,
                                  const int* OrdersArray,
                                  int Start,
                                  int Length)
{
    return HEMAX_HoudiniApi::SetCurveOrders(this,
                                            NodeId,
                                            PartId,
                                            OrdersArray,
                                            Start,
                                            Length);
}

bool
HEMAX_HAPISession::SetCurveCounts(HAPI_NodeId NodeId,
                                  HAPI_PartId PartId,
                                  const int* CountsArray,
                                  int Start,
                                  int Length)
{
    return HEMAX_HoudiniApi::SetCurveCounts(this,
                                            NodeId,
                                            PartId,
                                            CountsArray,
                                            Start,
                                            Length);
}

bool
HEMAX_HAPISession::SetCurveKnots(HAPI_NodeId NodeId,
                                 HAPI_PartId PartId,
                                 const float* KnotsArray,
                                 int Start,
                                 int Length)
{
    return HEMAX_HoudiniApi::SetCurveKnots(this,
                                           NodeId,
                                           PartId,
                                           KnotsArray,
                                           Start,
                                           Length);
}

bool
HEMAX_HAPISession::QueryNodeInput(HAPI_NodeId NodeToQuery,
                                  int InputIndex,
                                  HAPI_NodeId* ConnectedNodeId)
{
    return HEMAX_HoudiniApi::QueryNodeInput(this,
                                            NodeToQuery,
                                            InputIndex,
                                            ConnectedNodeId);
}

bool
HEMAX_HAPISession::GetTime(float* Time)
{
    return HEMAX_HoudiniApi::GetTime(this, Time);
}

bool
HEMAX_HAPISession::SetTime(float Time)
{
    return HEMAX_HoudiniApi::SetTime(this, Time);
}

bool
HEMAX_HAPISession::GetTimelineOptions(HAPI_TimelineOptions* TimelineOptions)
{
    return HEMAX_HoudiniApi::GetTimelineOptions(this, TimelineOptions);
}

bool
HEMAX_HAPISession::SetTimelineOptions(
                            const HAPI_TimelineOptions* TimelineOptions)
{
    return HEMAX_HoudiniApi::SetTimelineOptions(this, TimelineOptions);
}

bool
HEMAX_HAPISession::GetServerEnvVarCount(int * Count)
{
    return HEMAX_HoudiniApi::GetServerEnvVarCount(this, Count);
}

bool
HEMAX_HAPISession::GetServerEnvVarList(HAPI_StringHandle* HandleArray,
                                       int Start,
                                       int Length)
{
    return HEMAX_HoudiniApi::GetServerEnvVarList(this,
                                                 HandleArray,
                                                 Start,
                                                 Length);
}

bool
HEMAX_HAPISession::GetServerEnvString(const char* VarName,
                                      HAPI_StringHandle * Value)
{
    return HEMAX_HoudiniApi::GetServerEnvString(this,
                                                VarName,
                                                Value);
}

void
HEMAX_HAPISession::GetStatusStringBufLength(HAPI_StatusType StatusType,
                                            HAPI_StatusVerbosity Verbosity,
                                            int *BufferLength)
{
    HEMAX_HoudiniApi::GetStatusStringBufLength(this,
                                               StatusType,
                                               Verbosity,
                                               BufferLength);
}

void
HEMAX_HAPISession::GetStatusString(HAPI_StatusType StatusType,
                                   char* StringValue,
                                   int Length)
{
    HEMAX_HoudiniApi::GetStatusString(this,
                                      StatusType,
                                      StringValue,
                                      Length);
}

bool
HEMAX_HAPISession::SetServerEnvString(const char* VarName,
                                      const char* Value)
{
    return HEMAX_HoudiniApi::SetServerEnvString(this,
                                                VarName,
                                                Value);
}


bool
HEMAX_HAPISession::GetGroupNames(HAPI_NodeId NodeId, HAPI_GroupType GroupType,
    HAPI_StringHandle* GroupNameHandles, int Count)
{
    return HEMAX_HoudiniApi::GetGroupNames(this, NodeId, GroupType,
        GroupNameHandles, Count);
}

bool
HEMAX_HAPISession::GetGroupMembership(HAPI_NodeId NodeId, HAPI_PartId PartId,
    HAPI_GroupType GroupType, const char* GroupName,
    HAPI_Bool* MembershipAllEqualArray, int* MembershipArray, int Start,
    int Length)
{
    return HEMAX_HoudiniApi::GetGroupMembership(this, NodeId,
        PartId, GroupType, GroupName, MembershipAllEqualArray, MembershipArray,
        Start, Length);
}

bool
HEMAX_HAPISession::AddGroup(HAPI_NodeId NodeId,
                            HAPI_PartId PartId,
                            HAPI_GroupType GroupType,
                            const char* GroupName)
{
    return HEMAX_HoudiniApi::AddGroup(this,
                                      NodeId,
                                      PartId,
                                      GroupType,
                                      GroupName);
}

bool
HEMAX_HAPISession::SetGroupMembership(HAPI_NodeId NodeId,
                                      HAPI_PartId PartId,
                                      HAPI_GroupType GroupType,
                                      const char* GroupName,
                                      const int* MembershipArray,
                                      int Start, int Length)
{
    return HEMAX_HoudiniApi::SetGroupMembership(
                                                    this,
                                                    NodeId,
                                                    PartId,
                                                    GroupType,
                                                    GroupName,
                                                    MembershipArray,
                                                    Start,
                                                    Length);
}

bool
HEMAX_HAPISession::ClearConnectionError()
{
    return HEMAX_HoudiniApi::ClearConnectionError();
}

bool
HEMAX_HAPISession::GetConnectionError(char* StringValue,
                                      int Length,
                                      bool Clear)
{
    return HEMAX_HoudiniApi::GetConnectionError(StringValue,
                                                Length,
                                                Clear);
}

bool
HEMAX_HAPISession::GetConnectionErrorLength(int* BufferLength)
{
    return HEMAX_HoudiniApi::GetConnectionErrorLength(BufferLength);
}

std::string
HEMAX_HAPISession::GetHAPIString(HAPI_StringHandle Handle)
{
    int StringBufferLength;
    this->GetStringBufferLength(Handle, &StringBufferLength);

    if (StringBufferLength > 0)
    {
	char* StringResult = new char[StringBufferLength];
	this->GetString(Handle, StringResult, StringBufferLength);

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
    this->GetStatusStringBufLength(HAPI_STATUS_CALL_RESULT,
                                   HAPI_STATUSVERBOSITY_ALL,
                                   &StrBufLen);

    if (StrBufLen > 0)
    {
	char* StringResult = new char[StrBufLen];
	this->GetStatusString(HAPI_STATUS_CALL_RESULT, StringResult, StrBufLen);

	std::string ReturnStr(StringResult);

	delete[] StringResult;

	return ReturnStr;
    }
    else
    {
	return std::string("");
    }
}
