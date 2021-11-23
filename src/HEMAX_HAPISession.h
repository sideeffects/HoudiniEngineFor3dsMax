#pragma once

#include <HAPI.h>
#include <string>

#include "HEMAX_Types.h"

class HEMAX_HAPISession : public HAPI_Session
{
    public:

	HEMAX_HAPISession();

	virtual ~HEMAX_HAPISession();

	virtual bool CreateSession();

	bool InitializeHAPI(const char* HoudiniEnvFiles,
                            const char * otlSearchPath,
                            const char* dsoSearchPath,
                            const char* imageDsoSearchPath,
                            const char* audioDsoSearchPath);

	bool CloseSession();

	bool IsSessionValid();

	bool Cleanup();

	bool LoadHoudiniDigitalAsset(const char* FilePath,
                                     bool AllowOverwrite,
                                     HEMAX_AssetId* AssetLibId,
                                     HEMAX_AssetLoadStatus* LoadStatus=nullptr);

	bool GetAvailableAssetCount(HEMAX_AssetId AssetLibId,
                                    int* AssetCount);

	bool GetAvailableAssets(HEMAX_AssetId AssetLibId,
                                HEMAX_StringHandle* AssetNames,
                                int AssetCount);

	bool GetStringBufferLength(HEMAX_StringHandle StringHandle,
                                   int* StringBufferLength);

	bool GetString(HEMAX_StringHandle StringHandle,
                       char* StringValue,
                       int StringBufferLength);

	bool CreateNode(HEMAX_NodeId ParentNode,
                        const char* OperatorName,
		        const char* NodeLabel,
                        HEMAX_Bool CookOnCreation,
                        HEMAX_NodeId* NewNodeId );

	bool DeleteNode(HEMAX_NodeId NodeId);

	bool GetAssetInfo(HEMAX_NodeId Node,
                          HEMAX_AssetInfo* AssetInfo);

	bool CookNode(HEMAX_NodeId Node);

	bool HandleStatusResult(HAPI_Result Result);

	HEMAX_CookResult IsCookFinished();

	bool GetNodeInfo(HEMAX_NodeId Node,
                         HEMAX_NodeInfo* NodeInfo);

	bool GetNodeInputName(HEMAX_NodeId Node,
                              int InputIndex,
                              HEMAX_StringHandle* nameSH);

	bool GetObjectInfo(HEMAX_NodeId Node,
                           HEMAX_ObjectInfo* ObjectInfo);

	bool GetGeometryInfo(HEMAX_NodeId Node,
                             HEMAX_GeometryInfo* GeoInfo);

	bool GetDisplayGeoInfo(HEMAX_NodeId Node,
                               HEMAX_GeometryInfo* GeoInfo);

	bool GetPartInfo(HEMAX_NodeId Node,
                         HEMAX_PartId PartId,
                         HEMAX_PartInfo* PartInfo);

        bool GetEdgeCountOfEdgeGroup(HEMAX_NodeId Node,
                                     HEMAX_PartId PartId,
                                     const char* GroupName,
                                     int* EdgeCount);
    
	bool GetFaceCounts(HEMAX_NodeId Node,
                           HEMAX_PartId PartId,
                           int* FaceCountsArray,
                           int Start,
                           int Length);

	bool GetVertexList(HEMAX_NodeId Node,
                           HEMAX_PartId PartId,
                           int* VertexListArray,
                           int Start,
                           int Length);

	bool GetAttributeNames(HEMAX_NodeId Node,
                               HEMAX_PartId PartId,
                               HEMAX_AttributeOwner Owner,
                               HEMAX_StringHandle* AttributeNamesArray,
                               int Count);

	bool GetAttributeInfo(HEMAX_NodeId Node,
                              HEMAX_PartId PartId,
                              const char* Name,
                              HEMAX_AttributeOwner Owner,
                              HEMAX_AttributeInfo* AttributeInfo);

	bool GetAttributeIntData(HEMAX_NodeId Node,
                                 HEMAX_PartId PartId,
                                 const char* Name,
                                 HEMAX_AttributeInfo* AttributeInfo,
		                 int Stride,
                                 int* IntDataArray,
                                 int Start,
                                 int Length);

	bool GetAttributeFloatData(HEMAX_NodeId Node,
                                   HEMAX_PartId PartId,
                                   const char* Name,
                                   HEMAX_AttributeInfo* AttributeInfo,
		                   int Stride,
                                   float* FloatDataArray,
                                   int Start,
                                   int Length);

	bool GetAttributeStringData(HEMAX_NodeId Node,
                                    HEMAX_PartId PartId,
                                    const char* Name,
                                    HEMAX_AttributeInfo* AttributeInfo,
		                    HEMAX_StringHandle* StringHandleDataArray,
                                    int Start,
                                    int Length);

	bool GetParameters(HEMAX_NodeId Node,
                           HEMAX_ParameterInfo* ParmInfosArray,
                           int Start,
                           int Length);

        bool GetParameterInfo(HEMAX_NodeId Node,
                              HEMAX_ParameterId ParmId,
                              HEMAX_ParameterInfo* ParmInfo);

	bool GetParameterIntValues(HEMAX_NodeId Node,
                                   int* ValuesArray,
                                   int Start,
                                   int Length);

	bool GetParameterFloatValues(HEMAX_NodeId Node,
                                     float* ValuesArray,
                                     int Start,
                                     int Length);

	bool GetParameterStringValues(HEMAX_NodeId Node,
                                      bool Evaluate,
                                      HEMAX_StringHandle* ValuesArray,
                                      int Start,
                                      int Length);

	bool GetParameterChoiceLists(
                                HEMAX_NodeId Node,
                                HEMAX_ParameterChoiceInfo* ParmChoicesArray,
                                int Start,
                                int Length);

        bool GetParameterIdFromName(HEMAX_NodeId Node,
                                    const char* ParmName,
                                    HEMAX_ParameterId* ParmId);

	bool SetParameterIntValues(HEMAX_NodeId Node,
                                   int* ValuesArray,
                                   int Start,
                                   int Length);

	bool SetParameterFloatValues(HEMAX_NodeId Node,
                                     float* ValuesArray,
                                     int Start,
                                     int Length);

	bool SetParameterStringValue(HEMAX_NodeId Node,
                                     const char* StringValue,
                                     HEMAX_ParameterId ParmId,
                                     int Index);

	bool InsertMultiParameterInstance(HEMAX_NodeId Node,
                                          HEMAX_ParameterId Parameter,
                                          int Position);

	bool RemoveMultiParameterInstance(HEMAX_NodeId Node,
                                          HEMAX_ParameterId Parameter,
                                          int Position);

	bool ComposeObjectList(HEMAX_NodeId Node,
                               const char* Categories,
                               int* ObjectCount);

	bool GetComposedObjectList(HEMAX_NodeId Node,
                                   HEMAX_ObjectInfo* ObjectInfosArray,
                                   int Start,
                                   int Length);

	bool ComposeChildNodeList(HEMAX_NodeId Node,
                                  HAPI_NodeTypeBits NodeTypeFilter,
                                  HAPI_NodeFlagsBits NodeFlagFilter,
                                  HEMAX_Bool Recursive,
                                  int* Count);

	bool GetComposedChildNodeList(HEMAX_NodeId Node,
                                      HEMAX_NodeId* ChildNodeIdsArray,
                                      int Count);

	bool GetComposedObjectTransforms(HEMAX_NodeId Node,
                                         HAPI_RSTOrder RSTOrder,
                                         HAPI_Transform* TransformArray,
                                         int Start,
                                         int Length);

	bool GetObjectTransform(HEMAX_NodeId Node,
                                HEMAX_NodeId RelativeToNodeId,
                                HAPI_RSTOrder RSTOrder,
                                HAPI_Transform* Transform);

	bool GetParameterNodeValue(HEMAX_NodeId Node,
                                   const char* ParameterName,
                                   HEMAX_NodeId* Value);

	bool SetParameterNodeValue(HEMAX_NodeId Node,
                                   const char* ParameterName,
                                   HEMAX_NodeId Value);

	bool CreateInputNode(HEMAX_NodeId* Node,
                             const char* NodeName);

	bool SetObjectTransform(HEMAX_NodeId Node,
                                const HAPI_TransformEuler* Transform);

	bool SaveHIPFile(const char* FilePath,
                         HEMAX_Bool LockNodes);

	bool CommitGeometry(HEMAX_NodeId Node);

	bool SetPartInfo(HEMAX_NodeId Node,
                         HEMAX_PartId PartId,
                         const HEMAX_PartInfo* PartInfo);

	bool AddAttribute(HEMAX_NodeId Node,
                          HEMAX_PartId PartId,
                          const char* Name,
                          const HEMAX_AttributeInfo* AttributeInfo);

	bool SetAttributeFloatData(HEMAX_NodeId Node,
                                   HEMAX_PartId Part,
                                   const char* Name,
                                   const HEMAX_AttributeInfo* AttributeInfo,
                                   const float* DataArray,
                                   int Start,
                                   int Length);

	bool SetAttributeIntData(HEMAX_NodeId Node,
                                 HEMAX_PartId Part,
                                 const char* Name,
                                 const HEMAX_AttributeInfo* AttributeInfo,
                                 const int* DataArray,
                                 int Start,
                                 int Length);

	bool SetAttributeStringData(HEMAX_NodeId Node,
                                    HEMAX_PartId Part,
                                    const char* Name,
                                    const HEMAX_AttributeInfo* AttributeInfo,
                                    const char** DataArray,
                                    int Start,
                                    int Length);

	bool SetVertexList(HEMAX_NodeId Node,
                           HEMAX_PartId Part,
                           const int* VertexListArray,
                           int Start,
                           int Length);

	bool SetFaceCounts(HEMAX_NodeId Node,
                           HEMAX_PartId Part,
                           const int* FaceCountsArray,
                           int Start,
                           int Length);

	bool GetInstanceTransformsOnPart(HEMAX_NodeId Node,
                                    HEMAX_PartId Part,
                                    HAPI_RSTOrder RSTOrder,
                                    HAPI_Transform* TransformsArray,
                                    int Start,
                                    int Length);

	bool StartThriftNamedPipeServer(const HAPI_ThriftServerOptions* Options,
                                        const char* PipeName,
                                        HAPI_ProcessId* ProcessId,
                                        const char* LogFile);

	bool StartThriftSocketServer(const HAPI_ThriftServerOptions* Options,
                                     int Port,
                                     HAPI_ProcessId* ProcessId,
                                     const char* LogFile);

	bool ConnectNodeInput(HEMAX_NodeId Node,
                              int InputIndex,
                              HEMAX_NodeId NodeToConnect,
                              int OutputIndex = 0);

	bool DisconnectNodeInput(HEMAX_NodeId Node,
                                 int InputIndex);

	bool GetMaterialNodeIdsOnFaces(HEMAX_NodeId GeometryNode,
                                       HEMAX_PartId Part,
                                       bool* AreAllTheSame,
                                       HEMAX_NodeId* MaterialIdsArray,
                                       int Start,
                                       int Length);

	bool GetMaterialInfo(HEMAX_NodeId MaterialNode,
                             HEMAX_MaterialInfo* MaterialInfo);

	bool RenderTextureToImage(HEMAX_NodeId MaterialNode,
                                  HEMAX_ParameterId TextureMapFilePath);

	bool GetImageInfo(HEMAX_NodeId MaterialNode,
                          HEMAX_ImageInfo* ImageInfo);

	bool SetImageInfo(HEMAX_NodeId MaterialNode,
                          const HEMAX_ImageInfo* ImageInfo);

	bool ExtractImageToFile(HEMAX_NodeId MaterialNode,
                                const char* ImageFileFormatName,
                                const char* ImagePlanes,
                                const char* DestinationFolderPath,
		                const char* DestinationFileName,
                                int* DestinationFilePath);

	bool ExtractImageToMemory(HEMAX_NodeId MaterialNode,
                                  const char* ImageFileFormatName,
                                  const char* ImagePlanes,
                                  int* BufferSize);

	bool GetImageMemoryBuffer(HEMAX_NodeId MaterialNode,
                                  char* Buffer,
                                  int Length);

	bool GetInstancedObjectIds(HEMAX_NodeId InstancerNodeId,
                                   HEMAX_NodeId* InstancedNodeIdArray,
                                   int Start,
                                   int Length);

	bool GetInstancedPartIds(HEMAX_NodeId NodeId,
                                 HEMAX_PartId PartId,
                                 HEMAX_PartId* PartIdArray,
                                 int Start,
                                 int Length);

	bool GetInstancerPartTransforms(HEMAX_NodeId NodeId,
                                        HEMAX_PartId PartId,
                                        HAPI_RSTOrder RSTOrder,
                                        HAPI_Transform* TransformArray,
                                        int Start,
                                        int Length);

	bool GetCurveInfo(HEMAX_NodeId NodeId,
                          HEMAX_PartId PartId,
                          HEMAX_CurveInfo* CurveInfo);

	bool GetCurveCounts(HEMAX_NodeId NodeId,
                            HEMAX_PartId PartId,
                            int* CountsArray,
                            int Start,
                            int Length);

	bool GetCurveOrders(HEMAX_NodeId NodeId,
                            HEMAX_PartId PartId,
                            int* OrdersArray,
                            int Start,
                            int Length);

	bool GetCurveKnots(HEMAX_NodeId NodeId,
                           HEMAX_PartId PartId,
                           float* KnotsArray,
                           int Start,
                           int Length);

	bool SetCurveInfo(HEMAX_NodeId NodeId,
                          HEMAX_PartId PartId,
                          const HAPI_CurveInfo* CurveInfo);

	bool SetCurveOrders(HEMAX_NodeId NodeId,
                            HEMAX_PartId PartId,
                            const int* OrdersArray,
                            int Start,
                            int Length);

	bool SetCurveCounts(HEMAX_NodeId NodeId,
                            HEMAX_PartId PartId,
                            const int* CountsArray,
                            int Start,
                            int Length);

	bool SetCurveKnots(HEMAX_NodeId NodeId,
                           HEMAX_PartId PartId,
                           const float* KnotsArray,
                           int Start,
                           int Length);

	bool QueryNodeInput(HEMAX_NodeId NodeToQuery,
                            int InputIndex,
                            HEMAX_NodeId* ConnectedNodeId);

	bool GetTime(float* Time);

	bool SetTime(float Time);

	bool GetTimelineOptions(HEMAX_TimelineOptions* TimelineOptions);

	bool SetTimelineOptions(const HEMAX_TimelineOptions* TimelineOptions);

	bool GetServerEnvVarCount(int * Count);

	bool GetServerEnvVarList(HAPI_StringHandle* HandleArray,
                                 int Start,
                                 int Length);

	bool GetServerEnvString(const char* VarName,
                                HAPI_StringHandle* Value);

	void GetStatusStringBufLength(HAPI_StatusType StatusType,
                                      HAPI_StatusVerbosity Verbosity,
                                      int *BufferLength);

	void GetStatusString(HAPI_StatusType StatusType,
                             char* StringValue,
                             int Length);

	bool SetServerEnvString(const char* VarName,
                                const char* Value);

        bool GetGroupNames(HEMAX_NodeId NodeId, HAPI_GroupType GroupType,
            HAPI_StringHandle* GroupNameHandles, int Count);

        bool GetGroupMembership(HEMAX_NodeId NodeId, HAPI_PartId PartId,
            HAPI_GroupType GroupType, const char* GroupName,
            HAPI_Bool* MembershipAllEqualArray, int* MembershipArray, int Start,
            int Length);

        bool AddGroup(HEMAX_NodeId NodeId,
                      HEMAX_PartId PartId,
                      HEMAX_GroupType GroupType,
                      const char* GroupName);

        bool SetGroupMembership(HEMAX_NodeId NodeId,
                                HEMAX_PartId PartId,
                                HEMAX_GroupType GroupType,
                                const char* GroupName,
                                const int* MembershipArray,
                                int Start,
                                int Length);

        bool ClearConnectionError();

        bool GetConnectionError(char* StringValue,
                                int Length,
                                bool Clear = false);

        bool GetConnectionErrorLength(int* BufferLength);

	std::string GetHAPIString(HEMAX_StringHandle Handle);

	std::string GetHAPIErrorStatusString();

    protected:

	HAPI_CookOptions CookOptions;

	bool SeparateCookingThread;
	int CookingThreadStackSize;

};

class HEMAX_HAPIThriftSocketSession : public HEMAX_HAPISession
{
    public:

	HEMAX_HAPIThriftSocketSession();
	~HEMAX_HAPIThriftSocketSession() override;

	bool CreateSession() override;
	void SetHostName(std::string HName);
	void SetPortNumber(int PortNum);

    private:

	std::string HostName;
	int Port;
};

class HEMAX_HAPIThriftPipeSession : public HEMAX_HAPISession
{
    public:

	HEMAX_HAPIThriftPipeSession();
	~HEMAX_HAPIThriftPipeSession() override;

	bool CreateSession() override;
	void SetPipeName(std::string Name);

    private:

	std::string PipeName;
};
