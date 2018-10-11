#include "HEMAX_HAPISession.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Node.h"

HEMAX_HAPISession::HEMAX_HAPISession() : SeparateCookingThread( true ),
        CookingThreadStackSize( -1 )
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
HEMAX_HAPISession::HandleStatusResult( HAPI_Result Result )
{
    if ( Result != HAPI_RESULT_SUCCESS )
    {
        return false;
    }

    return true;
}

HEMAX_CookResult
HEMAX_HAPISession::IsCookFinished()
{
    int Status;
    HAPI_Result Result = HEMAX_HoudiniApi::GetStatus( this, HAPI_STATUS_COOK_STATE, &Status );

    if ( !HandleStatusResult( Result ) )
    {
        return COOK_FAILED;
    }
    else
    {
        if ( Status <= HAPI_STATE_MAX_READY_STATE )
        {
            return COOK_FINISHED;
        }
        else
        {
            return COOK_NOT_FINISHED;
        }
    }
}

bool
HEMAX_HAPISession::CreateSession()
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateInProcessSession( this );

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::InitializeHAPI(const char* HoudiniEnvFiles, const char * otlSearchPath, const char* dsoSearchPath, const char* imageDsoSearchPath, const char* audioDsoSearchPath)
{
    CookOptions.curveRefineLOD = 8.0f;
    CookOptions.clearErrorsAndWarnings = false;
    CookOptions.maxVerticesPerPrimitive = -1;
    CookOptions.splitGeosByGroup = false;
    CookOptions.refineCurveToLinear = false;
    CookOptions.handleBoxPartTypes = false;
    CookOptions.handleSpherePartTypes = false;
    CookOptions.splitPointsByVertexAttributes = false;
    CookOptions.packedPrimInstancingMode = HAPI_PACKEDPRIM_INSTANCING_MODE_FLAT;

    HAPI_Result Result = HEMAX_HoudiniApi::Initialize( this, &CookOptions, SeparateCookingThread, CookingThreadStackSize,
                             HoudiniEnvFiles, otlSearchPath, dsoSearchPath, imageDsoSearchPath, audioDsoSearchPath );

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::IsSessionValid()
{
    return HandleStatusResult(HEMAX_HoudiniApi::IsSessionValid(this));
}

bool
HEMAX_HAPISession::Cleanup()
{
    HAPI_Result Result = HEMAX_HoudiniApi::Cleanup(this);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::CloseSession()
{
    HAPI_Result Close_Result = HEMAX_HoudiniApi::CloseSession( this );

    return ( HandleStatusResult( Close_Result ) );
}

bool
HEMAX_HAPIThriftSocketSession::CreateSession()
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftSocketSession( this, HostName.c_str(), Port );

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPIThriftPipeSession::CreateSession()
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftNamedPipeSession( this, PipeName.c_str() );

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::LoadHoudiniDigitalAsset( const char* FilePath, bool AllowOverwrite,
    HEMAX_AssetId* AssetLibId, HEMAX_AssetLoadStatus* LoadStatus )
{
    HAPI_Result Result = HEMAX_HoudiniApi::LoadAssetLibraryFromFile( this, FilePath, AllowOverwrite, AssetLibId );

    if (LoadStatus)
    {
        if (Result == HAPI_RESULT_ASSET_DEF_ALREADY_LOADED)
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
    }

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetAvailableAssetCount( HEMAX_AssetId AssetLibId, int* AssetCount )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAvailableAssetCount( this, AssetLibId, AssetCount );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetAvailableAssets( HEMAX_AssetId AssetLibId, HEMAX_StringHandle* AssetNames, int AssetCount )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAvailableAssets( this, AssetLibId, AssetNames, AssetCount );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetStringBufferLength( HEMAX_StringHandle StringHandle, int* StringBufferLength )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetStringBufLength( this, StringHandle, StringBufferLength );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetString( HEMAX_StringHandle StringHandle, char* StringValue, int StringBufferLength )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetString( this, StringHandle, StringValue, StringBufferLength );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::CreateNode( HEMAX_NodeId ParentNode, const char* OperatorName,
    const char* NodeLabel, HEMAX_Bool CookOnCreation,
    HEMAX_NodeId* NewNodeId )
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateNode( this, ParentNode, OperatorName, NodeLabel,
        CookOnCreation, NewNodeId );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::DeleteNode(HEMAX_NodeId NodeId)
{
    HAPI_Result Result = HEMAX_HoudiniApi::DeleteNode( this, NodeId );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetAssetInfo( HEMAX_NodeId Node, HEMAX_AssetInfo* AssetInfo )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAssetInfo( this, Node, AssetInfo );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::CookNode( HEMAX_NodeId Node)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CookNode( this, Node, &CookOptions );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetNodeInfo( HEMAX_NodeId Node, HEMAX_NodeInfo* NodeInfo )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetNodeInfo( this, Node, NodeInfo );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetObjectInfo( HEMAX_NodeId Node, HEMAX_ObjectInfo* ObjectInfo )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetObjectInfo( this, Node, ObjectInfo );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetGeometryInfo(HEMAX_NodeId Node, HEMAX_GeometryInfo* GeoInfo)
{
#ifdef GetGeoInfo
#undef GetGeoInfo
#endif
    HAPI_Result Result = HEMAX_HoudiniApi::GetGeoInfo( this, Node, GeoInfo );
#define GetGeoInfo GetGeoInfoW

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetDisplayGeoInfo( HEMAX_NodeId Node, HEMAX_GeometryInfo* GeoInfo )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetDisplayGeoInfo( this, Node, GeoInfo );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetPartInfo( HEMAX_NodeId Node, HEMAX_PartId PartId, HEMAX_PartInfo* PartInfo )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetPartInfo( this, Node, PartId, PartInfo );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetFaceCounts( HEMAX_NodeId Node, HEMAX_PartId PartId, int* FaceCountsArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetFaceCounts( this, Node, PartId, FaceCountsArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetVertexList( HEMAX_NodeId Node, HEMAX_PartId PartId, int* VertexListArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetVertexList( this, Node, PartId, VertexListArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetAttributeNames( HEMAX_NodeId Node, HEMAX_PartId PartId, HEMAX_AttributeOwner Owner, HEMAX_StringHandle* AttributeNamesArray, int Count )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeNames( this, Node, PartId, HAPI_AttributeOwner(Owner), AttributeNamesArray, Count );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetAttributeInfo( HEMAX_NodeId Node, HEMAX_PartId PartId, const char* Name, HEMAX_AttributeOwner Owner, HEMAX_AttributeInfo* AttributeInfo)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeInfo( this, Node, PartId, Name, HAPI_AttributeOwner(Owner), AttributeInfo );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetAttributeIntData( HEMAX_NodeId Node, HEMAX_PartId PartId, const char* Name, HEMAX_AttributeInfo* AttributeInfo,
                                        int Stride, int* IntDataArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeIntData( this, Node, PartId, Name, AttributeInfo, Stride, IntDataArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetAttributeFloatData( HEMAX_NodeId Node, HEMAX_PartId PartId, const char* Name, HEMAX_AttributeInfo* AttributeInfo,
                                          int Stride, float* FloatDataArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeFloatData( this, Node, PartId, Name, AttributeInfo, Stride, FloatDataArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetAttributeStringData( HEMAX_NodeId Node, HEMAX_PartId PartId, const char* Name, HEMAX_AttributeInfo* AttributeInfo,
                                           HEMAX_StringHandle* StringHandleDataArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeStringData( this, Node, PartId, Name, AttributeInfo, StringHandleDataArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetParameters( HEMAX_NodeId Node, HEMAX_ParameterInfo* ParmInfosArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParameters( this, Node, ParmInfosArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetParameterIntValues( HEMAX_NodeId Node, int* ValuesArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmIntValues( this, Node, ValuesArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetParameterFloatValues( HEMAX_NodeId Node, float* ValuesArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmFloatValues( this, Node, ValuesArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetParameterStringValues( HEMAX_NodeId Node, bool Evaluate, HEMAX_StringHandle* ValuesArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmStringValues( this, Node, Evaluate, ValuesArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetParameterChoiceLists( HEMAX_NodeId Node, HEMAX_ParameterChoiceInfo* ParmChoicesArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmChoiceLists( this, Node, ParmChoicesArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::SetParameterIntValues( HEMAX_NodeId Node, int* ValuesArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmIntValues( this, Node, ValuesArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::SetParameterFloatValues( HEMAX_NodeId Node, float* ValuesArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmFloatValues( this, Node, ValuesArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::SetParameterStringValue( HEMAX_NodeId Node, const char* StringValue, HEMAX_ParameterId ParmId, int Index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmStringValue( this, Node, StringValue, ParmId, Index );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::InsertMultiParameterInstance( HEMAX_NodeId Node, HEMAX_ParameterId Parameter, int Position )
{
    HAPI_Result Result = HEMAX_HoudiniApi::InsertMultiparmInstance( this, Node, Parameter, Position );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::RemoveMultiParameterInstance( HEMAX_NodeId Node, HEMAX_ParameterId Parameter, int Position )
{
    HAPI_Result Result = HEMAX_HoudiniApi::RemoveMultiparmInstance( this, Node, Parameter, Position );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetComposedObjectTransforms( HEMAX_NodeId Node, HAPI_RSTOrder RSTOrder, HAPI_Transform* TransformArray, int Start, int Length )
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetComposedObjectTransforms( this, Node, RSTOrder, TransformArray, Start, Length );
    
    return HandleStatusResult( Result );
}

std::string
HEMAX_HAPISession::GetHAPIString( HEMAX_StringHandle Handle )
{
    int StringBufferLength;
    this->GetStringBufferLength( Handle, &StringBufferLength );

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
HEMAX_HAPISession::ComposeObjectList(HEMAX_NodeId Node, const char* Categories, int* ObjectCount)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ComposeObjectList( this, Node, Categories, ObjectCount );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetComposedObjectList(HEMAX_NodeId Node, HEMAX_ObjectInfo* ObjectInfosArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetComposedObjectList( this, Node, ObjectInfosArray, Start, Length );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::ComposeChildNodeList(HEMAX_NodeId Node, HAPI_NodeTypeBits NodeTypeFilter, HAPI_NodeFlagsBits NodeFlagFilter, HEMAX_Bool Recursive, int* Count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ComposeChildNodeList( this, Node, NodeTypeFilter, NodeFlagFilter, Recursive, Count );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetComposedChildNodeList(HEMAX_NodeId Node, HEMAX_NodeId* ChildNodeIdsArray, int Count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetComposedChildNodeList( this, Node, ChildNodeIdsArray, Count );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetObjectTransform(HEMAX_NodeId Node, HEMAX_NodeId RelativeToNodeId, HAPI_RSTOrder RSTOrder, HAPI_Transform* Transform)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetObjectTransform( this, Node, RelativeToNodeId, RSTOrder, Transform );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::GetParameterNodeValue(HEMAX_NodeId Node, const char* ParameterName, HEMAX_NodeId* Value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmNodeValue( this, Node, ParameterName, Value );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::SetParameterNodeValue(HEMAX_NodeId Node, const char* ParameterName, HEMAX_NodeId Value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmNodeValue( this, Node, ParameterName, Value );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::CreateInputNode(HEMAX_NodeId* Node, const char* NodeName)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateInputNode( this, Node, NodeName );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::SetObjectTransform(HEMAX_NodeId Node, const HAPI_TransformEuler* Transform)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetObjectTransform( this, Node, Transform );

    return HandleStatusResult( Result );
}

bool
HEMAX_HAPISession::SaveHIPFile(const char* FilePath, HEMAX_Bool LockNodes)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SaveHIPFile(this, FilePath, LockNodes);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::CommitGeometry(HEMAX_NodeId Node)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CommitGeo(this, Node);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetPartInfo(HEMAX_NodeId Node, HEMAX_PartId PartId, const HEMAX_PartInfo* PartInfo)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetPartInfo(this, Node, PartId, PartInfo);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::AddAttribute(HEMAX_NodeId Node, HEMAX_PartId PartId, const char* Name, const HEMAX_AttributeInfo* AttributeInfo)
{
    HAPI_Result Result = HEMAX_HoudiniApi::AddAttribute(this, Node, PartId, Name, AttributeInfo);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetAttributeFloatData(HEMAX_NodeId Node, HEMAX_PartId Part, const char* Name, const HEMAX_AttributeInfo* AttributeInfo, const float* DataArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeFloatData(this, Node, Part, Name, AttributeInfo, DataArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetAttributeIntData(HEMAX_NodeId Node, HEMAX_PartId Part, const char* Name, const HEMAX_AttributeInfo* AttributeInfo, const int* DataArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeIntData(this, Node, Part, Name, AttributeInfo, DataArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetAttributeStringData(HEMAX_NodeId Node, HEMAX_PartId Part, const char* Name, const HEMAX_AttributeInfo* AttributeInfo, const char** DataArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeStringData(this, Node, Part, Name, AttributeInfo, DataArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetVertexList(HEMAX_NodeId Node, HEMAX_PartId Part, const int* VertexListArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetVertexList(this, Node, Part, VertexListArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetFaceCounts(HEMAX_NodeId Node, HEMAX_PartId Part, const int* FaceCountsArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetFaceCounts(this, Node, Part, FaceCountsArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetInstanceTransforms(HEMAX_NodeId Node, HAPI_RSTOrder RSTOrder, HAPI_Transform* TransformsArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetInstanceTransforms(this, Node, RSTOrder, TransformsArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::StartThriftNamedPipeServer(const HAPI_ThriftServerOptions* Options, const char* PipeName, HAPI_ProcessId* ProcessId)
{
    HAPI_Result Result = HEMAX_HoudiniApi::StartThriftNamedPipeServer(Options, PipeName, ProcessId);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::StartThriftSocketServer(const HAPI_ThriftServerOptions* Options, int Port, HAPI_ProcessId* ProcessId)
{
    HAPI_Result Result = HEMAX_HoudiniApi::StartThriftSocketServer(Options, Port, ProcessId);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::ConnectNodeInput(HEMAX_NodeId Node, int InputIndex, HEMAX_NodeId NodeToConnect, int OutputIndex)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ConnectNodeInput(this, Node, InputIndex, NodeToConnect, OutputIndex);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::DisconnectNodeInput(HEMAX_NodeId Node, int InputIndex)
{
    HAPI_Result Result = HEMAX_HoudiniApi::DisconnectNodeInput(this, Node, InputIndex);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetMaterialNodeIdsOnFaces(HEMAX_NodeId GeometryNode, HEMAX_PartId Part, bool* AreAllTheSame, HEMAX_NodeId* MaterialIdsArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetMaterialNodeIdsOnFaces(this, GeometryNode, Part, AreAllTheSame, MaterialIdsArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetMaterialInfo(HEMAX_NodeId MaterialNode, HEMAX_MaterialInfo* MaterialInfo)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetMaterialInfo(this, MaterialNode, MaterialInfo);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::RenderTextureToImage(HEMAX_NodeId MaterialNode, HEMAX_ParameterId TextureMapFilePath)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RenderTextureToImage(this, MaterialNode, TextureMapFilePath);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetImageInfo(HEMAX_NodeId MaterialNode, HEMAX_ImageInfo* ImageInfo)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetImageInfo(this, MaterialNode, ImageInfo);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetImageInfo(HEMAX_NodeId MaterialNode, const HEMAX_ImageInfo* ImageInfo)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetImageInfo(this, MaterialNode, ImageInfo);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::ExtractImageToFile(HEMAX_NodeId MaterialNode, const char* ImageFileFormatName, const char* ImagePlanes, const char* DestinationFolderPath,
                                        const char* DestinationFileName, int* DestinationFilePath)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ExtractImageToFile(this, MaterialNode, ImageFileFormatName, ImagePlanes, DestinationFolderPath, DestinationFileName, DestinationFilePath);
    
    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::ExtractImageToMemory(HEMAX_NodeId MaterialNode, const char* ImageFileFormatName, const char* ImagePlanes, int* BufferSize)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ExtractImageToMemory(this, MaterialNode, ImageFileFormatName, ImagePlanes, BufferSize);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetImageMemoryBuffer(HEMAX_NodeId MaterialNode, char* Buffer, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetImageMemoryBuffer(this, MaterialNode, Buffer, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetInstancedObjectIds(HEMAX_NodeId InstancerNodeId, HEMAX_NodeId* InstancedNodeIdArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetInstancedObjectIds(this, InstancerNodeId, InstancedNodeIdArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetInstancedPartIds(HEMAX_NodeId NodeId, HEMAX_PartId PartId, HEMAX_PartId* PartIdArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetInstancedPartIds(this, NodeId, PartId, PartIdArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetInstancerPartTransforms(HEMAX_NodeId NodeId, HEMAX_PartId PartId, HAPI_RSTOrder RSTOrder, HAPI_Transform* TransformArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetInstancerPartTransforms(this, NodeId, PartId, RSTOrder, TransformArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetCurveInfo(HEMAX_NodeId NodeId, HEMAX_PartId PartId, HEMAX_CurveInfo* CurveInfo)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCurveInfo(this, NodeId, PartId, CurveInfo);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetCurveCounts(HEMAX_NodeId NodeId, HEMAX_PartId PartId, int* CountsArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCurveCounts(this, NodeId, PartId, CountsArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetCurveOrders(HEMAX_NodeId NodeId, HEMAX_PartId PartId, int* OrdersArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCurveOrders(this, NodeId, PartId, OrdersArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetCurveKnots(HEMAX_NodeId NodeId, HEMAX_PartId PartId, float* KnotsArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCurveKnots(this, NodeId, PartId, KnotsArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetCurveInfo(HEMAX_NodeId NodeId, HEMAX_PartId PartId, const HAPI_CurveInfo* CurveInfo)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCurveInfo(this, NodeId, PartId, CurveInfo);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetCurveOrders(HEMAX_NodeId NodeId, HEMAX_PartId PartId, const int* OrdersArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCurveOrders(this, NodeId, PartId, OrdersArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetCurveCounts(HEMAX_NodeId NodeId, HEMAX_PartId PartId, const int* CountsArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCurveCounts(this, NodeId, PartId, CountsArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetCurveKnots(HEMAX_NodeId NodeId, HEMAX_PartId PartId, const float* KnotsArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCurveKnots(this, NodeId, PartId, KnotsArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::QueryNodeInput(HEMAX_NodeId NodeToQuery, int InputIndex, HEMAX_NodeId* ConnectedNodeId)
{
    HAPI_Result Result = HEMAX_HoudiniApi::QueryNodeInput(this, NodeToQuery, InputIndex, ConnectedNodeId);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetTime(float* Time)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetTime(this, Time);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetTime(float Time)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetTime(this, Time);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetTimelineOptions(HEMAX_TimelineOptions* TimelineOptions)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetTimelineOptions(this, TimelineOptions);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::SetTimelineOptions(const HEMAX_TimelineOptions* TimelineOptions)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetTimelineOptions(this, TimelineOptions);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetServerEnvVarCount(int * Count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetServerEnvVarCount(this, Count);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetServerEnvVarList(HAPI_StringHandle* HandleArray, int Start, int Length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetServerEnvVarList(this, HandleArray, Start, Length);

    return HandleStatusResult(Result);
}

bool
HEMAX_HAPISession::GetServerEnvString(const char * VarName, HAPI_StringHandle * Value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetServerEnvString(this, VarName, Value);

    return HandleStatusResult(Result);
}