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
                                     HAPI_AssetLibraryId* AssetLibId,
                                     HEMAX_AssetLoadStatus* LoadStatus=nullptr);

	bool GetAvailableAssetCount(HAPI_AssetLibraryId AssetLibId,
                                    int* AssetCount);

	bool GetAvailableAssets(HAPI_AssetLibraryId AssetLibId,
                                HAPI_StringHandle* AssetNames,
                                int AssetCount);

	bool GetStringBufferLength(HAPI_StringHandle StringHandle,
                                   int* StringBufferLength);

	bool GetString(HAPI_StringHandle StringHandle,
                       char* StringValue,
                       int StringBufferLength);

	bool CreateNode(HAPI_NodeId ParentNode,
                        const char* OperatorName,
		        const char* NodeLabel,
                        HAPI_Bool CookOnCreation,
                        HAPI_NodeId* NewNodeId );

	bool DeleteNode(HAPI_NodeId NodeId);

	bool GetAssetInfo(HAPI_NodeId Node,
                          HAPI_AssetInfo* AssetInfo);

	bool CookNode(HAPI_NodeId Node);

	bool HandleStatusResult(HAPI_Result Result);

	HEMAX_CookResult IsCookFinished();

	bool GetNodeInfo(HAPI_NodeId Node,
                         HAPI_NodeInfo* NodeInfo);

	bool GetNodeInputName(HAPI_NodeId Node,
                              int InputIndex,
                              HAPI_StringHandle* nameSH);

	bool GetObjectInfo(HAPI_NodeId Node,
                           HAPI_ObjectInfo* ObjectInfo);

	bool GetGeometryInfo(HAPI_NodeId Node,
                             HAPI_GeoInfo* GeoInfo);

	bool GetDisplayGeoInfo(HAPI_NodeId Node,
                               HAPI_GeoInfo* GeoInfo);

	bool GetPartInfo(HAPI_NodeId Node,
                         HAPI_PartId PartId,
                         HAPI_PartInfo* PartInfo);

        bool GetEdgeCountOfEdgeGroup(HAPI_NodeId Node,
                                     HAPI_PartId PartId,
                                     const char* GroupName,
                                     int* EdgeCount);
    
	bool GetFaceCounts(HAPI_NodeId Node,
                           HAPI_PartId PartId,
                           int* FaceCountsArray,
                           int Start,
                           int Length);

	bool GetVertexList(HAPI_NodeId Node,
                           HAPI_PartId PartId,
                           int* VertexListArray,
                           int Start,
                           int Length);

	bool GetAttributeNames(HAPI_NodeId Node,
                               HAPI_PartId PartId,
                               HAPI_AttributeOwner Owner,
                               HAPI_StringHandle* AttributeNamesArray,
                               int Count);

	bool GetAttributeInfo(HAPI_NodeId Node,
                              HAPI_PartId PartId,
                              const char* Name,
                              HAPI_AttributeOwner Owner,
                              HAPI_AttributeInfo* AttributeInfo);

	bool GetAttributeIntData(HAPI_NodeId Node,
                                 HAPI_PartId PartId,
                                 const char* Name,
                                 HAPI_AttributeInfo* AttributeInfo,
		                 int Stride,
                                 int* IntDataArray,
                                 int Start,
                                 int Length);

	bool GetAttributeFloatData(HAPI_NodeId Node,
                                   HAPI_PartId PartId,
                                   const char* Name,
                                   HAPI_AttributeInfo* AttributeInfo,
		                   int Stride,
                                   float* FloatDataArray,
                                   int Start,
                                   int Length);

	bool GetAttributeStringData(HAPI_NodeId Node,
                                    HAPI_PartId PartId,
                                    const char* Name,
                                    HAPI_AttributeInfo* AttributeInfo,
		                    HAPI_StringHandle* StringHandleDataArray,
                                    int Start,
                                    int Length);

	bool GetParameters(HAPI_NodeId Node,
                           HAPI_ParmInfo* ParmInfosArray,
                           int Start,
                           int Length);

        bool GetParameterInfo(HAPI_NodeId Node,
                              HAPI_ParmId ParmId,
                              HAPI_ParmInfo* ParmInfo);

	bool GetParameterIntValues(HAPI_NodeId Node,
                                   int* ValuesArray,
                                   int Start,
                                   int Length);

	bool GetParameterFloatValues(HAPI_NodeId Node,
                                     float* ValuesArray,
                                     int Start,
                                     int Length);

	bool GetParameterStringValues(HAPI_NodeId Node,
                                      bool Evaluate,
                                      HAPI_StringHandle* ValuesArray,
                                      int Start,
                                      int Length);

	bool GetParameterChoiceLists(
                                HAPI_NodeId Node,
                                HAPI_ParmChoiceInfo* ParmChoicesArray,
                                int Start,
                                int Length);

        bool GetParameterIdFromName(HAPI_NodeId Node,
                                    const char* ParmName,
                                    HAPI_ParmId* ParmId);

	bool SetParameterIntValues(HAPI_NodeId Node,
                                   int* ValuesArray,
                                   int Start,
                                   int Length);

	bool SetParameterFloatValues(HAPI_NodeId Node,
                                     float* ValuesArray,
                                     int Start,
                                     int Length);

	bool SetParameterStringValue(HAPI_NodeId Node,
                                     const char* StringValue,
                                     HAPI_ParmId ParmId,
                                     int Index);

	bool InsertMultiParameterInstance(HAPI_NodeId Node,
                                          HAPI_ParmId Parameter,
                                          int Position);

	bool RemoveMultiParameterInstance(HAPI_NodeId Node,
                                          HAPI_ParmId Parameter,
                                          int Position);

	bool ComposeObjectList(HAPI_NodeId Node,
                               const char* Categories,
                               int* ObjectCount);

	bool GetComposedObjectList(HAPI_NodeId Node,
                                   HAPI_ObjectInfo* ObjectInfosArray,
                                   int Start,
                                   int Length);

	bool ComposeChildNodeList(HAPI_NodeId Node,
                                  HAPI_NodeTypeBits NodeTypeFilter,
                                  HAPI_NodeFlagsBits NodeFlagFilter,
                                  HAPI_Bool Recursive,
                                  int* Count);

	bool GetComposedChildNodeList(HAPI_NodeId Node,
                                      HAPI_NodeId* ChildNodeIdsArray,
                                      int Count);

	bool GetComposedObjectTransforms(HAPI_NodeId Node,
                                         HAPI_RSTOrder RSTOrder,
                                         HAPI_Transform* TransformArray,
                                         int Start,
                                         int Length);

	bool GetObjectTransform(HAPI_NodeId Node,
                                HAPI_NodeId RelativeToNodeId,
                                HAPI_RSTOrder RSTOrder,
                                HAPI_Transform* Transform);

	bool GetParameterNodeValue(HAPI_NodeId Node,
                                   const char* ParameterName,
                                   HAPI_NodeId* Value);

	bool SetParameterNodeValue(HAPI_NodeId Node,
                                   const char* ParameterName,
                                   HAPI_NodeId Value);

	bool CreateInputNode(HAPI_NodeId* Node,
                             const char* NodeName);

	bool SetObjectTransform(HAPI_NodeId Node,
                                const HAPI_TransformEuler* Transform);

	bool SaveHIPFile(const char* FilePath,
                         HAPI_Bool LockNodes);

	bool CommitGeometry(HAPI_NodeId Node);

	bool SetPartInfo(HAPI_NodeId Node,
                         HAPI_PartId PartId,
                         const HAPI_PartInfo* PartInfo);

	bool AddAttribute(HAPI_NodeId Node,
                          HAPI_PartId PartId,
                          const char* Name,
                          const HAPI_AttributeInfo* AttributeInfo);

	bool SetAttributeFloatData(HAPI_NodeId Node,
                                   HAPI_PartId Part,
                                   const char* Name,
                                   const HAPI_AttributeInfo* AttributeInfo,
                                   const float* DataArray,
                                   int Start,
                                   int Length);

	bool SetAttributeIntData(HAPI_NodeId Node,
                                 HAPI_PartId Part,
                                 const char* Name,
                                 const HAPI_AttributeInfo* AttributeInfo,
                                 const int* DataArray,
                                 int Start,
                                 int Length);

	bool SetAttributeStringData(HAPI_NodeId Node,
                                    HAPI_PartId Part,
                                    const char* Name,
                                    const HAPI_AttributeInfo* AttributeInfo,
                                    const char** DataArray,
                                    int Start,
                                    int Length);

	bool SetVertexList(HAPI_NodeId Node,
                           HAPI_PartId Part,
                           const int* VertexListArray,
                           int Start,
                           int Length);

	bool SetFaceCounts(HAPI_NodeId Node,
                           HAPI_PartId Part,
                           const int* FaceCountsArray,
                           int Start,
                           int Length);

	bool GetInstanceTransformsOnPart(HAPI_NodeId Node,
                                    HAPI_PartId Part,
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

	bool ConnectNodeInput(HAPI_NodeId Node,
                              int InputIndex,
                              HAPI_NodeId NodeToConnect,
                              int OutputIndex = 0);

	bool DisconnectNodeInput(HAPI_NodeId Node,
                                 int InputIndex);

	bool GetMaterialNodeIdsOnFaces(HAPI_NodeId GeometryNode,
                                       HAPI_PartId Part,
                                       bool* AreAllTheSame,
                                       HAPI_NodeId* MaterialIdsArray,
                                       int Start,
                                       int Length);

	bool GetMaterialInfo(HAPI_NodeId MaterialNode,
                             HAPI_MaterialInfo* MaterialInfo);

	bool RenderTextureToImage(HAPI_NodeId MaterialNode,
                                  HAPI_ParmId TextureMapFilePath);

	bool GetImageInfo(HAPI_NodeId MaterialNode,
                          HAPI_ImageInfo* ImageInfo);

	bool SetImageInfo(HAPI_NodeId MaterialNode,
                          const HAPI_ImageInfo* ImageInfo);

	bool ExtractImageToFile(HAPI_NodeId MaterialNode,
                                const char* ImageFileFormatName,
                                const char* ImagePlanes,
                                const char* DestinationFolderPath,
		                const char* DestinationFileName,
                                int* DestinationFilePath);

	bool ExtractImageToMemory(HAPI_NodeId MaterialNode,
                                  const char* ImageFileFormatName,
                                  const char* ImagePlanes,
                                  int* BufferSize);

	bool GetImageMemoryBuffer(HAPI_NodeId MaterialNode,
                                  char* Buffer,
                                  int Length);

	bool GetInstancedObjectIds(HAPI_NodeId InstancerNodeId,
                                   HAPI_NodeId* InstancedNodeIdArray,
                                   int Start,
                                   int Length);

	bool GetInstancedPartIds(HAPI_NodeId NodeId,
                                 HAPI_PartId PartId,
                                 HAPI_PartId* PartIdArray,
                                 int Start,
                                 int Length);

	bool GetInstancerPartTransforms(HAPI_NodeId NodeId,
                                        HAPI_PartId PartId,
                                        HAPI_RSTOrder RSTOrder,
                                        HAPI_Transform* TransformArray,
                                        int Start,
                                        int Length);

	bool GetCurveInfo(HAPI_NodeId NodeId,
                          HAPI_PartId PartId,
                          HAPI_CurveInfo* CurveInfo);

	bool GetCurveCounts(HAPI_NodeId NodeId,
                            HAPI_PartId PartId,
                            int* CountsArray,
                            int Start,
                            int Length);

	bool GetCurveOrders(HAPI_NodeId NodeId,
                            HAPI_PartId PartId,
                            int* OrdersArray,
                            int Start,
                            int Length);

	bool GetCurveKnots(HAPI_NodeId NodeId,
                           HAPI_PartId PartId,
                           float* KnotsArray,
                           int Start,
                           int Length);

	bool SetCurveInfo(HAPI_NodeId NodeId,
                          HAPI_PartId PartId,
                          const HAPI_CurveInfo* CurveInfo);

	bool SetCurveOrders(HAPI_NodeId NodeId,
                            HAPI_PartId PartId,
                            const int* OrdersArray,
                            int Start,
                            int Length);

	bool SetCurveCounts(HAPI_NodeId NodeId,
                            HAPI_PartId PartId,
                            const int* CountsArray,
                            int Start,
                            int Length);

	bool SetCurveKnots(HAPI_NodeId NodeId,
                           HAPI_PartId PartId,
                           const float* KnotsArray,
                           int Start,
                           int Length);

	bool QueryNodeInput(HAPI_NodeId NodeToQuery,
                            int InputIndex,
                            HAPI_NodeId* ConnectedNodeId);

	bool GetTime(float* Time);

	bool SetTime(float Time);

	bool GetTimelineOptions(HAPI_TimelineOptions* TimelineOptions);

	bool SetTimelineOptions(const HAPI_TimelineOptions* TimelineOptions);

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

        bool GetGroupNames(HAPI_NodeId NodeId, HAPI_GroupType GroupType,
            HAPI_StringHandle* GroupNameHandles, int Count);

        bool GetGroupMembership(HAPI_NodeId NodeId, HAPI_PartId PartId,
            HAPI_GroupType GroupType, const char* GroupName,
            HAPI_Bool* MembershipAllEqualArray, int* MembershipArray, int Start,
            int Length);

        bool AddGroup(HAPI_NodeId NodeId,
                      HAPI_PartId PartId,
                      HAPI_GroupType GroupType,
                      const char* GroupName);

        bool SetGroupMembership(HAPI_NodeId NodeId,
                                HAPI_PartId PartId,
                                HAPI_GroupType GroupType,
                                const char* GroupName,
                                const int* MembershipArray,
                                int Start,
                                int Length);

        bool ClearConnectionError();

        bool GetConnectionError(char* StringValue,
                                int Length,
                                bool Clear = false);

        bool GetConnectionErrorLength(int* BufferLength);

	std::string GetHAPIString(HAPI_StringHandle Handle);

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
