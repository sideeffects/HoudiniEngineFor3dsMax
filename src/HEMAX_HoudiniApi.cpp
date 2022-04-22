/*
 * Copyright (c) <2022> Side Effects Software Inc. *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * COMMENTS:
 *      This file is generated. Do not modify directly.
 */

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Logger.h"


#ifdef GetGeoInfo
#undef GetGeoInfo
#endif


HEMAX_HoudiniApi::AddAttributeFuncPtr
HEMAX_HoudiniApi::AddAttributeImpl = &HEMAX_HoudiniApi::AddAttributeEmptyStub;

HEMAX_HoudiniApi::AddGroupFuncPtr
HEMAX_HoudiniApi::AddGroupImpl = &HEMAX_HoudiniApi::AddGroupEmptyStub;

HEMAX_HoudiniApi::BindCustomImplementationFuncPtr
HEMAX_HoudiniApi::BindCustomImplementationImpl = &HEMAX_HoudiniApi::BindCustomImplementationEmptyStub;

HEMAX_HoudiniApi::CancelPDGCookFuncPtr
HEMAX_HoudiniApi::CancelPDGCookImpl = &HEMAX_HoudiniApi::CancelPDGCookEmptyStub;

HEMAX_HoudiniApi::CheckForSpecificErrorsFuncPtr
HEMAX_HoudiniApi::CheckForSpecificErrorsImpl = &HEMAX_HoudiniApi::CheckForSpecificErrorsEmptyStub;

HEMAX_HoudiniApi::CleanupFuncPtr
HEMAX_HoudiniApi::CleanupImpl = &HEMAX_HoudiniApi::CleanupEmptyStub;

HEMAX_HoudiniApi::ClearConnectionErrorFuncPtr
HEMAX_HoudiniApi::ClearConnectionErrorImpl = &HEMAX_HoudiniApi::ClearConnectionErrorEmptyStub;

HEMAX_HoudiniApi::CloseSessionFuncPtr
HEMAX_HoudiniApi::CloseSessionImpl = &HEMAX_HoudiniApi::CloseSessionEmptyStub;

HEMAX_HoudiniApi::CommitGeoFuncPtr
HEMAX_HoudiniApi::CommitGeoImpl = &HEMAX_HoudiniApi::CommitGeoEmptyStub;

HEMAX_HoudiniApi::CommitWorkItemsFuncPtr
HEMAX_HoudiniApi::CommitWorkItemsImpl = &HEMAX_HoudiniApi::CommitWorkItemsEmptyStub;

HEMAX_HoudiniApi::CommitWorkitemsFuncPtr
HEMAX_HoudiniApi::CommitWorkitemsImpl = &HEMAX_HoudiniApi::CommitWorkitemsEmptyStub;

HEMAX_HoudiniApi::ComposeChildNodeListFuncPtr
HEMAX_HoudiniApi::ComposeChildNodeListImpl = &HEMAX_HoudiniApi::ComposeChildNodeListEmptyStub;

HEMAX_HoudiniApi::ComposeNodeCookResultFuncPtr
HEMAX_HoudiniApi::ComposeNodeCookResultImpl = &HEMAX_HoudiniApi::ComposeNodeCookResultEmptyStub;

HEMAX_HoudiniApi::ComposeObjectListFuncPtr
HEMAX_HoudiniApi::ComposeObjectListImpl = &HEMAX_HoudiniApi::ComposeObjectListEmptyStub;

HEMAX_HoudiniApi::ConnectNodeInputFuncPtr
HEMAX_HoudiniApi::ConnectNodeInputImpl = &HEMAX_HoudiniApi::ConnectNodeInputEmptyStub;

HEMAX_HoudiniApi::ConvertMatrixToEulerFuncPtr
HEMAX_HoudiniApi::ConvertMatrixToEulerImpl = &HEMAX_HoudiniApi::ConvertMatrixToEulerEmptyStub;

HEMAX_HoudiniApi::ConvertMatrixToQuatFuncPtr
HEMAX_HoudiniApi::ConvertMatrixToQuatImpl = &HEMAX_HoudiniApi::ConvertMatrixToQuatEmptyStub;

HEMAX_HoudiniApi::ConvertTransformFuncPtr
HEMAX_HoudiniApi::ConvertTransformImpl = &HEMAX_HoudiniApi::ConvertTransformEmptyStub;

HEMAX_HoudiniApi::ConvertTransformEulerToMatrixFuncPtr
HEMAX_HoudiniApi::ConvertTransformEulerToMatrixImpl = &HEMAX_HoudiniApi::ConvertTransformEulerToMatrixEmptyStub;

HEMAX_HoudiniApi::ConvertTransformQuatToMatrixFuncPtr
HEMAX_HoudiniApi::ConvertTransformQuatToMatrixImpl = &HEMAX_HoudiniApi::ConvertTransformQuatToMatrixEmptyStub;

HEMAX_HoudiniApi::CookNodeFuncPtr
HEMAX_HoudiniApi::CookNodeImpl = &HEMAX_HoudiniApi::CookNodeEmptyStub;

HEMAX_HoudiniApi::CookPDGFuncPtr
HEMAX_HoudiniApi::CookPDGImpl = &HEMAX_HoudiniApi::CookPDGEmptyStub;

HEMAX_HoudiniApi::CookPDGAllOutputsFuncPtr
HEMAX_HoudiniApi::CookPDGAllOutputsImpl = &HEMAX_HoudiniApi::CookPDGAllOutputsEmptyStub;

HEMAX_HoudiniApi::CreateCustomSessionFuncPtr
HEMAX_HoudiniApi::CreateCustomSessionImpl = &HEMAX_HoudiniApi::CreateCustomSessionEmptyStub;

HEMAX_HoudiniApi::CreateHeightFieldInputFuncPtr
HEMAX_HoudiniApi::CreateHeightFieldInputImpl = &HEMAX_HoudiniApi::CreateHeightFieldInputEmptyStub;

HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNodeFuncPtr
HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNodeImpl = &HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNodeEmptyStub;

HEMAX_HoudiniApi::CreateInProcessSessionFuncPtr
HEMAX_HoudiniApi::CreateInProcessSessionImpl = &HEMAX_HoudiniApi::CreateInProcessSessionEmptyStub;

HEMAX_HoudiniApi::CreateInputCurveNodeFuncPtr
HEMAX_HoudiniApi::CreateInputCurveNodeImpl = &HEMAX_HoudiniApi::CreateInputCurveNodeEmptyStub;

HEMAX_HoudiniApi::CreateInputNodeFuncPtr
HEMAX_HoudiniApi::CreateInputNodeImpl = &HEMAX_HoudiniApi::CreateInputNodeEmptyStub;

HEMAX_HoudiniApi::CreateNodeFuncPtr
HEMAX_HoudiniApi::CreateNodeImpl = &HEMAX_HoudiniApi::CreateNodeEmptyStub;

HEMAX_HoudiniApi::CreateThriftNamedPipeSessionFuncPtr
HEMAX_HoudiniApi::CreateThriftNamedPipeSessionImpl = &HEMAX_HoudiniApi::CreateThriftNamedPipeSessionEmptyStub;

HEMAX_HoudiniApi::CreateThriftSocketSessionFuncPtr
HEMAX_HoudiniApi::CreateThriftSocketSessionImpl = &HEMAX_HoudiniApi::CreateThriftSocketSessionEmptyStub;

HEMAX_HoudiniApi::CreateWorkItemFuncPtr
HEMAX_HoudiniApi::CreateWorkItemImpl = &HEMAX_HoudiniApi::CreateWorkItemEmptyStub;

HEMAX_HoudiniApi::CreateWorkitemFuncPtr
HEMAX_HoudiniApi::CreateWorkitemImpl = &HEMAX_HoudiniApi::CreateWorkitemEmptyStub;

HEMAX_HoudiniApi::DeleteAttributeFuncPtr
HEMAX_HoudiniApi::DeleteAttributeImpl = &HEMAX_HoudiniApi::DeleteAttributeEmptyStub;

HEMAX_HoudiniApi::DeleteGroupFuncPtr
HEMAX_HoudiniApi::DeleteGroupImpl = &HEMAX_HoudiniApi::DeleteGroupEmptyStub;

HEMAX_HoudiniApi::DeleteNodeFuncPtr
HEMAX_HoudiniApi::DeleteNodeImpl = &HEMAX_HoudiniApi::DeleteNodeEmptyStub;

HEMAX_HoudiniApi::DirtyPDGNodeFuncPtr
HEMAX_HoudiniApi::DirtyPDGNodeImpl = &HEMAX_HoudiniApi::DirtyPDGNodeEmptyStub;

HEMAX_HoudiniApi::DisconnectNodeInputFuncPtr
HEMAX_HoudiniApi::DisconnectNodeInputImpl = &HEMAX_HoudiniApi::DisconnectNodeInputEmptyStub;

HEMAX_HoudiniApi::DisconnectNodeOutputsAtFuncPtr
HEMAX_HoudiniApi::DisconnectNodeOutputsAtImpl = &HEMAX_HoudiniApi::DisconnectNodeOutputsAtEmptyStub;

HEMAX_HoudiniApi::ExtractImageToFileFuncPtr
HEMAX_HoudiniApi::ExtractImageToFileImpl = &HEMAX_HoudiniApi::ExtractImageToFileEmptyStub;

HEMAX_HoudiniApi::ExtractImageToMemoryFuncPtr
HEMAX_HoudiniApi::ExtractImageToMemoryImpl = &HEMAX_HoudiniApi::ExtractImageToMemoryEmptyStub;

HEMAX_HoudiniApi::GetActiveCacheCountFuncPtr
HEMAX_HoudiniApi::GetActiveCacheCountImpl = &HEMAX_HoudiniApi::GetActiveCacheCountEmptyStub;

HEMAX_HoudiniApi::GetActiveCacheNamesFuncPtr
HEMAX_HoudiniApi::GetActiveCacheNamesImpl = &HEMAX_HoudiniApi::GetActiveCacheNamesEmptyStub;

HEMAX_HoudiniApi::GetAssetDefinitionParmCountsFuncPtr
HEMAX_HoudiniApi::GetAssetDefinitionParmCountsImpl = &HEMAX_HoudiniApi::GetAssetDefinitionParmCountsEmptyStub;

HEMAX_HoudiniApi::GetAssetDefinitionParmInfosFuncPtr
HEMAX_HoudiniApi::GetAssetDefinitionParmInfosImpl = &HEMAX_HoudiniApi::GetAssetDefinitionParmInfosEmptyStub;

HEMAX_HoudiniApi::GetAssetDefinitionParmValuesFuncPtr
HEMAX_HoudiniApi::GetAssetDefinitionParmValuesImpl = &HEMAX_HoudiniApi::GetAssetDefinitionParmValuesEmptyStub;

HEMAX_HoudiniApi::GetAssetInfoFuncPtr
HEMAX_HoudiniApi::GetAssetInfoImpl = &HEMAX_HoudiniApi::GetAssetInfoEmptyStub;

HEMAX_HoudiniApi::GetAttributeFloat64ArrayDataFuncPtr
HEMAX_HoudiniApi::GetAttributeFloat64ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeFloat64ArrayDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeFloat64DataFuncPtr
HEMAX_HoudiniApi::GetAttributeFloat64DataImpl = &HEMAX_HoudiniApi::GetAttributeFloat64DataEmptyStub;

HEMAX_HoudiniApi::GetAttributeFloatArrayDataFuncPtr
HEMAX_HoudiniApi::GetAttributeFloatArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeFloatArrayDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeFloatDataFuncPtr
HEMAX_HoudiniApi::GetAttributeFloatDataImpl = &HEMAX_HoudiniApi::GetAttributeFloatDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeInfoFuncPtr
HEMAX_HoudiniApi::GetAttributeInfoImpl = &HEMAX_HoudiniApi::GetAttributeInfoEmptyStub;

HEMAX_HoudiniApi::GetAttributeInt16ArrayDataFuncPtr
HEMAX_HoudiniApi::GetAttributeInt16ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeInt16ArrayDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeInt16DataFuncPtr
HEMAX_HoudiniApi::GetAttributeInt16DataImpl = &HEMAX_HoudiniApi::GetAttributeInt16DataEmptyStub;

HEMAX_HoudiniApi::GetAttributeInt64ArrayDataFuncPtr
HEMAX_HoudiniApi::GetAttributeInt64ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeInt64ArrayDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeInt64DataFuncPtr
HEMAX_HoudiniApi::GetAttributeInt64DataImpl = &HEMAX_HoudiniApi::GetAttributeInt64DataEmptyStub;

HEMAX_HoudiniApi::GetAttributeInt8ArrayDataFuncPtr
HEMAX_HoudiniApi::GetAttributeInt8ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeInt8ArrayDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeInt8DataFuncPtr
HEMAX_HoudiniApi::GetAttributeInt8DataImpl = &HEMAX_HoudiniApi::GetAttributeInt8DataEmptyStub;

HEMAX_HoudiniApi::GetAttributeIntArrayDataFuncPtr
HEMAX_HoudiniApi::GetAttributeIntArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeIntArrayDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeIntDataFuncPtr
HEMAX_HoudiniApi::GetAttributeIntDataImpl = &HEMAX_HoudiniApi::GetAttributeIntDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeNamesFuncPtr
HEMAX_HoudiniApi::GetAttributeNamesImpl = &HEMAX_HoudiniApi::GetAttributeNamesEmptyStub;

HEMAX_HoudiniApi::GetAttributeStringArrayDataFuncPtr
HEMAX_HoudiniApi::GetAttributeStringArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeStringArrayDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeStringDataFuncPtr
HEMAX_HoudiniApi::GetAttributeStringDataImpl = &HEMAX_HoudiniApi::GetAttributeStringDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeUInt8ArrayDataFuncPtr
HEMAX_HoudiniApi::GetAttributeUInt8ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeUInt8ArrayDataEmptyStub;

HEMAX_HoudiniApi::GetAttributeUInt8DataFuncPtr
HEMAX_HoudiniApi::GetAttributeUInt8DataImpl = &HEMAX_HoudiniApi::GetAttributeUInt8DataEmptyStub;

HEMAX_HoudiniApi::GetAvailableAssetCountFuncPtr
HEMAX_HoudiniApi::GetAvailableAssetCountImpl = &HEMAX_HoudiniApi::GetAvailableAssetCountEmptyStub;

HEMAX_HoudiniApi::GetAvailableAssetsFuncPtr
HEMAX_HoudiniApi::GetAvailableAssetsImpl = &HEMAX_HoudiniApi::GetAvailableAssetsEmptyStub;

HEMAX_HoudiniApi::GetBoxInfoFuncPtr
HEMAX_HoudiniApi::GetBoxInfoImpl = &HEMAX_HoudiniApi::GetBoxInfoEmptyStub;

HEMAX_HoudiniApi::GetCachePropertyFuncPtr
HEMAX_HoudiniApi::GetCachePropertyImpl = &HEMAX_HoudiniApi::GetCachePropertyEmptyStub;

HEMAX_HoudiniApi::GetComposedChildNodeListFuncPtr
HEMAX_HoudiniApi::GetComposedChildNodeListImpl = &HEMAX_HoudiniApi::GetComposedChildNodeListEmptyStub;

HEMAX_HoudiniApi::GetComposedNodeCookResultFuncPtr
HEMAX_HoudiniApi::GetComposedNodeCookResultImpl = &HEMAX_HoudiniApi::GetComposedNodeCookResultEmptyStub;

HEMAX_HoudiniApi::GetComposedObjectListFuncPtr
HEMAX_HoudiniApi::GetComposedObjectListImpl = &HEMAX_HoudiniApi::GetComposedObjectListEmptyStub;

HEMAX_HoudiniApi::GetComposedObjectTransformsFuncPtr
HEMAX_HoudiniApi::GetComposedObjectTransformsImpl = &HEMAX_HoudiniApi::GetComposedObjectTransformsEmptyStub;

HEMAX_HoudiniApi::GetCompositorOptionsFuncPtr
HEMAX_HoudiniApi::GetCompositorOptionsImpl = &HEMAX_HoudiniApi::GetCompositorOptionsEmptyStub;

HEMAX_HoudiniApi::GetConnectionErrorFuncPtr
HEMAX_HoudiniApi::GetConnectionErrorImpl = &HEMAX_HoudiniApi::GetConnectionErrorEmptyStub;

HEMAX_HoudiniApi::GetConnectionErrorLengthFuncPtr
HEMAX_HoudiniApi::GetConnectionErrorLengthImpl = &HEMAX_HoudiniApi::GetConnectionErrorLengthEmptyStub;

HEMAX_HoudiniApi::GetCookingCurrentCountFuncPtr
HEMAX_HoudiniApi::GetCookingCurrentCountImpl = &HEMAX_HoudiniApi::GetCookingCurrentCountEmptyStub;

HEMAX_HoudiniApi::GetCookingTotalCountFuncPtr
HEMAX_HoudiniApi::GetCookingTotalCountImpl = &HEMAX_HoudiniApi::GetCookingTotalCountEmptyStub;

HEMAX_HoudiniApi::GetCurveCountsFuncPtr
HEMAX_HoudiniApi::GetCurveCountsImpl = &HEMAX_HoudiniApi::GetCurveCountsEmptyStub;

HEMAX_HoudiniApi::GetCurveInfoFuncPtr
HEMAX_HoudiniApi::GetCurveInfoImpl = &HEMAX_HoudiniApi::GetCurveInfoEmptyStub;

HEMAX_HoudiniApi::GetCurveKnotsFuncPtr
HEMAX_HoudiniApi::GetCurveKnotsImpl = &HEMAX_HoudiniApi::GetCurveKnotsEmptyStub;

HEMAX_HoudiniApi::GetCurveOrdersFuncPtr
HEMAX_HoudiniApi::GetCurveOrdersImpl = &HEMAX_HoudiniApi::GetCurveOrdersEmptyStub;

HEMAX_HoudiniApi::GetDisplayGeoInfoFuncPtr
HEMAX_HoudiniApi::GetDisplayGeoInfoImpl = &HEMAX_HoudiniApi::GetDisplayGeoInfoEmptyStub;

HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroupFuncPtr
HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroupImpl = &HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroupEmptyStub;

HEMAX_HoudiniApi::GetEnvIntFuncPtr
HEMAX_HoudiniApi::GetEnvIntImpl = &HEMAX_HoudiniApi::GetEnvIntEmptyStub;

HEMAX_HoudiniApi::GetFaceCountsFuncPtr
HEMAX_HoudiniApi::GetFaceCountsImpl = &HEMAX_HoudiniApi::GetFaceCountsEmptyStub;

HEMAX_HoudiniApi::GetFirstVolumeTileFuncPtr
HEMAX_HoudiniApi::GetFirstVolumeTileImpl = &HEMAX_HoudiniApi::GetFirstVolumeTileEmptyStub;

HEMAX_HoudiniApi::GetGeoInfoFuncPtr
HEMAX_HoudiniApi::GetGeoInfoImpl = &HEMAX_HoudiniApi::GetGeoInfoEmptyStub;

HEMAX_HoudiniApi::GetGeoSizeFuncPtr
HEMAX_HoudiniApi::GetGeoSizeImpl = &HEMAX_HoudiniApi::GetGeoSizeEmptyStub;

HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePartFuncPtr
HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePartImpl = &HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePartEmptyStub;

HEMAX_HoudiniApi::GetGroupMembershipFuncPtr
HEMAX_HoudiniApi::GetGroupMembershipImpl = &HEMAX_HoudiniApi::GetGroupMembershipEmptyStub;

HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePartFuncPtr
HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePartImpl = &HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePartEmptyStub;

HEMAX_HoudiniApi::GetGroupNamesFuncPtr
HEMAX_HoudiniApi::GetGroupNamesImpl = &HEMAX_HoudiniApi::GetGroupNamesEmptyStub;

HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePartFuncPtr
HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePartImpl = &HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePartEmptyStub;

HEMAX_HoudiniApi::GetHIPFileNodeCountFuncPtr
HEMAX_HoudiniApi::GetHIPFileNodeCountImpl = &HEMAX_HoudiniApi::GetHIPFileNodeCountEmptyStub;

HEMAX_HoudiniApi::GetHIPFileNodeIdsFuncPtr
HEMAX_HoudiniApi::GetHIPFileNodeIdsImpl = &HEMAX_HoudiniApi::GetHIPFileNodeIdsEmptyStub;

HEMAX_HoudiniApi::GetHandleBindingInfoFuncPtr
HEMAX_HoudiniApi::GetHandleBindingInfoImpl = &HEMAX_HoudiniApi::GetHandleBindingInfoEmptyStub;

HEMAX_HoudiniApi::GetHandleInfoFuncPtr
HEMAX_HoudiniApi::GetHandleInfoImpl = &HEMAX_HoudiniApi::GetHandleInfoEmptyStub;

HEMAX_HoudiniApi::GetHeightFieldDataFuncPtr
HEMAX_HoudiniApi::GetHeightFieldDataImpl = &HEMAX_HoudiniApi::GetHeightFieldDataEmptyStub;

HEMAX_HoudiniApi::GetImageFilePathFuncPtr
HEMAX_HoudiniApi::GetImageFilePathImpl = &HEMAX_HoudiniApi::GetImageFilePathEmptyStub;

HEMAX_HoudiniApi::GetImageInfoFuncPtr
HEMAX_HoudiniApi::GetImageInfoImpl = &HEMAX_HoudiniApi::GetImageInfoEmptyStub;

HEMAX_HoudiniApi::GetImageMemoryBufferFuncPtr
HEMAX_HoudiniApi::GetImageMemoryBufferImpl = &HEMAX_HoudiniApi::GetImageMemoryBufferEmptyStub;

HEMAX_HoudiniApi::GetImagePlaneCountFuncPtr
HEMAX_HoudiniApi::GetImagePlaneCountImpl = &HEMAX_HoudiniApi::GetImagePlaneCountEmptyStub;

HEMAX_HoudiniApi::GetImagePlanesFuncPtr
HEMAX_HoudiniApi::GetImagePlanesImpl = &HEMAX_HoudiniApi::GetImagePlanesEmptyStub;

HEMAX_HoudiniApi::GetInputCurveInfoFuncPtr
HEMAX_HoudiniApi::GetInputCurveInfoImpl = &HEMAX_HoudiniApi::GetInputCurveInfoEmptyStub;

HEMAX_HoudiniApi::GetInstanceTransformsOnPartFuncPtr
HEMAX_HoudiniApi::GetInstanceTransformsOnPartImpl = &HEMAX_HoudiniApi::GetInstanceTransformsOnPartEmptyStub;

HEMAX_HoudiniApi::GetInstancedObjectIdsFuncPtr
HEMAX_HoudiniApi::GetInstancedObjectIdsImpl = &HEMAX_HoudiniApi::GetInstancedObjectIdsEmptyStub;

HEMAX_HoudiniApi::GetInstancedPartIdsFuncPtr
HEMAX_HoudiniApi::GetInstancedPartIdsImpl = &HEMAX_HoudiniApi::GetInstancedPartIdsEmptyStub;

HEMAX_HoudiniApi::GetInstancerPartTransformsFuncPtr
HEMAX_HoudiniApi::GetInstancerPartTransformsImpl = &HEMAX_HoudiniApi::GetInstancerPartTransformsEmptyStub;

HEMAX_HoudiniApi::GetManagerNodeIdFuncPtr
HEMAX_HoudiniApi::GetManagerNodeIdImpl = &HEMAX_HoudiniApi::GetManagerNodeIdEmptyStub;

HEMAX_HoudiniApi::GetMaterialInfoFuncPtr
HEMAX_HoudiniApi::GetMaterialInfoImpl = &HEMAX_HoudiniApi::GetMaterialInfoEmptyStub;

HEMAX_HoudiniApi::GetMaterialNodeIdsOnFacesFuncPtr
HEMAX_HoudiniApi::GetMaterialNodeIdsOnFacesImpl = &HEMAX_HoudiniApi::GetMaterialNodeIdsOnFacesEmptyStub;

HEMAX_HoudiniApi::GetNextVolumeTileFuncPtr
HEMAX_HoudiniApi::GetNextVolumeTileImpl = &HEMAX_HoudiniApi::GetNextVolumeTileEmptyStub;

HEMAX_HoudiniApi::GetNodeFromPathFuncPtr
HEMAX_HoudiniApi::GetNodeFromPathImpl = &HEMAX_HoudiniApi::GetNodeFromPathEmptyStub;

HEMAX_HoudiniApi::GetNodeInfoFuncPtr
HEMAX_HoudiniApi::GetNodeInfoImpl = &HEMAX_HoudiniApi::GetNodeInfoEmptyStub;

HEMAX_HoudiniApi::GetNodeInputNameFuncPtr
HEMAX_HoudiniApi::GetNodeInputNameImpl = &HEMAX_HoudiniApi::GetNodeInputNameEmptyStub;

HEMAX_HoudiniApi::GetNodeOutputNameFuncPtr
HEMAX_HoudiniApi::GetNodeOutputNameImpl = &HEMAX_HoudiniApi::GetNodeOutputNameEmptyStub;

HEMAX_HoudiniApi::GetNodePathFuncPtr
HEMAX_HoudiniApi::GetNodePathImpl = &HEMAX_HoudiniApi::GetNodePathEmptyStub;

HEMAX_HoudiniApi::GetNumWorkItemsFuncPtr
HEMAX_HoudiniApi::GetNumWorkItemsImpl = &HEMAX_HoudiniApi::GetNumWorkItemsEmptyStub;

HEMAX_HoudiniApi::GetNumWorkitemsFuncPtr
HEMAX_HoudiniApi::GetNumWorkitemsImpl = &HEMAX_HoudiniApi::GetNumWorkitemsEmptyStub;

HEMAX_HoudiniApi::GetObjectInfoFuncPtr
HEMAX_HoudiniApi::GetObjectInfoImpl = &HEMAX_HoudiniApi::GetObjectInfoEmptyStub;

HEMAX_HoudiniApi::GetObjectTransformFuncPtr
HEMAX_HoudiniApi::GetObjectTransformImpl = &HEMAX_HoudiniApi::GetObjectTransformEmptyStub;

HEMAX_HoudiniApi::GetOutputGeoCountFuncPtr
HEMAX_HoudiniApi::GetOutputGeoCountImpl = &HEMAX_HoudiniApi::GetOutputGeoCountEmptyStub;

HEMAX_HoudiniApi::GetOutputGeoInfosFuncPtr
HEMAX_HoudiniApi::GetOutputGeoInfosImpl = &HEMAX_HoudiniApi::GetOutputGeoInfosEmptyStub;

HEMAX_HoudiniApi::GetOutputNodeIdFuncPtr
HEMAX_HoudiniApi::GetOutputNodeIdImpl = &HEMAX_HoudiniApi::GetOutputNodeIdEmptyStub;

HEMAX_HoudiniApi::GetPDGEventsFuncPtr
HEMAX_HoudiniApi::GetPDGEventsImpl = &HEMAX_HoudiniApi::GetPDGEventsEmptyStub;

HEMAX_HoudiniApi::GetPDGGraphContextIdFuncPtr
HEMAX_HoudiniApi::GetPDGGraphContextIdImpl = &HEMAX_HoudiniApi::GetPDGGraphContextIdEmptyStub;

HEMAX_HoudiniApi::GetPDGGraphContextsFuncPtr
HEMAX_HoudiniApi::GetPDGGraphContextsImpl = &HEMAX_HoudiniApi::GetPDGGraphContextsEmptyStub;

HEMAX_HoudiniApi::GetPDGGraphContextsCountFuncPtr
HEMAX_HoudiniApi::GetPDGGraphContextsCountImpl = &HEMAX_HoudiniApi::GetPDGGraphContextsCountEmptyStub;

HEMAX_HoudiniApi::GetPDGStateFuncPtr
HEMAX_HoudiniApi::GetPDGStateImpl = &HEMAX_HoudiniApi::GetPDGStateEmptyStub;

HEMAX_HoudiniApi::GetParametersFuncPtr
HEMAX_HoudiniApi::GetParametersImpl = &HEMAX_HoudiniApi::GetParametersEmptyStub;

HEMAX_HoudiniApi::GetParmChoiceListsFuncPtr
HEMAX_HoudiniApi::GetParmChoiceListsImpl = &HEMAX_HoudiniApi::GetParmChoiceListsEmptyStub;

HEMAX_HoudiniApi::GetParmExpressionFuncPtr
HEMAX_HoudiniApi::GetParmExpressionImpl = &HEMAX_HoudiniApi::GetParmExpressionEmptyStub;

HEMAX_HoudiniApi::GetParmFileFuncPtr
HEMAX_HoudiniApi::GetParmFileImpl = &HEMAX_HoudiniApi::GetParmFileEmptyStub;

HEMAX_HoudiniApi::GetParmFloatValueFuncPtr
HEMAX_HoudiniApi::GetParmFloatValueImpl = &HEMAX_HoudiniApi::GetParmFloatValueEmptyStub;

HEMAX_HoudiniApi::GetParmFloatValuesFuncPtr
HEMAX_HoudiniApi::GetParmFloatValuesImpl = &HEMAX_HoudiniApi::GetParmFloatValuesEmptyStub;

HEMAX_HoudiniApi::GetParmIdFromNameFuncPtr
HEMAX_HoudiniApi::GetParmIdFromNameImpl = &HEMAX_HoudiniApi::GetParmIdFromNameEmptyStub;

HEMAX_HoudiniApi::GetParmInfoFuncPtr
HEMAX_HoudiniApi::GetParmInfoImpl = &HEMAX_HoudiniApi::GetParmInfoEmptyStub;

HEMAX_HoudiniApi::GetParmInfoFromNameFuncPtr
HEMAX_HoudiniApi::GetParmInfoFromNameImpl = &HEMAX_HoudiniApi::GetParmInfoFromNameEmptyStub;

HEMAX_HoudiniApi::GetParmIntValueFuncPtr
HEMAX_HoudiniApi::GetParmIntValueImpl = &HEMAX_HoudiniApi::GetParmIntValueEmptyStub;

HEMAX_HoudiniApi::GetParmIntValuesFuncPtr
HEMAX_HoudiniApi::GetParmIntValuesImpl = &HEMAX_HoudiniApi::GetParmIntValuesEmptyStub;

HEMAX_HoudiniApi::GetParmNodeValueFuncPtr
HEMAX_HoudiniApi::GetParmNodeValueImpl = &HEMAX_HoudiniApi::GetParmNodeValueEmptyStub;

HEMAX_HoudiniApi::GetParmStringValueFuncPtr
HEMAX_HoudiniApi::GetParmStringValueImpl = &HEMAX_HoudiniApi::GetParmStringValueEmptyStub;

HEMAX_HoudiniApi::GetParmStringValuesFuncPtr
HEMAX_HoudiniApi::GetParmStringValuesImpl = &HEMAX_HoudiniApi::GetParmStringValuesEmptyStub;

HEMAX_HoudiniApi::GetParmTagNameFuncPtr
HEMAX_HoudiniApi::GetParmTagNameImpl = &HEMAX_HoudiniApi::GetParmTagNameEmptyStub;

HEMAX_HoudiniApi::GetParmTagValueFuncPtr
HEMAX_HoudiniApi::GetParmTagValueImpl = &HEMAX_HoudiniApi::GetParmTagValueEmptyStub;

HEMAX_HoudiniApi::GetParmWithTagFuncPtr
HEMAX_HoudiniApi::GetParmWithTagImpl = &HEMAX_HoudiniApi::GetParmWithTagEmptyStub;

HEMAX_HoudiniApi::GetPartInfoFuncPtr
HEMAX_HoudiniApi::GetPartInfoImpl = &HEMAX_HoudiniApi::GetPartInfoEmptyStub;

HEMAX_HoudiniApi::GetPresetFuncPtr
HEMAX_HoudiniApi::GetPresetImpl = &HEMAX_HoudiniApi::GetPresetEmptyStub;

HEMAX_HoudiniApi::GetPresetBufLengthFuncPtr
HEMAX_HoudiniApi::GetPresetBufLengthImpl = &HEMAX_HoudiniApi::GetPresetBufLengthEmptyStub;

HEMAX_HoudiniApi::GetServerEnvIntFuncPtr
HEMAX_HoudiniApi::GetServerEnvIntImpl = &HEMAX_HoudiniApi::GetServerEnvIntEmptyStub;

HEMAX_HoudiniApi::GetServerEnvStringFuncPtr
HEMAX_HoudiniApi::GetServerEnvStringImpl = &HEMAX_HoudiniApi::GetServerEnvStringEmptyStub;

HEMAX_HoudiniApi::GetServerEnvVarCountFuncPtr
HEMAX_HoudiniApi::GetServerEnvVarCountImpl = &HEMAX_HoudiniApi::GetServerEnvVarCountEmptyStub;

HEMAX_HoudiniApi::GetServerEnvVarListFuncPtr
HEMAX_HoudiniApi::GetServerEnvVarListImpl = &HEMAX_HoudiniApi::GetServerEnvVarListEmptyStub;

HEMAX_HoudiniApi::GetSessionEnvIntFuncPtr
HEMAX_HoudiniApi::GetSessionEnvIntImpl = &HEMAX_HoudiniApi::GetSessionEnvIntEmptyStub;

HEMAX_HoudiniApi::GetSessionSyncInfoFuncPtr
HEMAX_HoudiniApi::GetSessionSyncInfoImpl = &HEMAX_HoudiniApi::GetSessionSyncInfoEmptyStub;

HEMAX_HoudiniApi::GetSphereInfoFuncPtr
HEMAX_HoudiniApi::GetSphereInfoImpl = &HEMAX_HoudiniApi::GetSphereInfoEmptyStub;

HEMAX_HoudiniApi::GetStatusFuncPtr
HEMAX_HoudiniApi::GetStatusImpl = &HEMAX_HoudiniApi::GetStatusEmptyStub;

HEMAX_HoudiniApi::GetStatusStringFuncPtr
HEMAX_HoudiniApi::GetStatusStringImpl = &HEMAX_HoudiniApi::GetStatusStringEmptyStub;

HEMAX_HoudiniApi::GetStatusStringBufLengthFuncPtr
HEMAX_HoudiniApi::GetStatusStringBufLengthImpl = &HEMAX_HoudiniApi::GetStatusStringBufLengthEmptyStub;

HEMAX_HoudiniApi::GetStringFuncPtr
HEMAX_HoudiniApi::GetStringImpl = &HEMAX_HoudiniApi::GetStringEmptyStub;

HEMAX_HoudiniApi::GetStringBatchFuncPtr
HEMAX_HoudiniApi::GetStringBatchImpl = &HEMAX_HoudiniApi::GetStringBatchEmptyStub;

HEMAX_HoudiniApi::GetStringBatchSizeFuncPtr
HEMAX_HoudiniApi::GetStringBatchSizeImpl = &HEMAX_HoudiniApi::GetStringBatchSizeEmptyStub;

HEMAX_HoudiniApi::GetStringBufLengthFuncPtr
HEMAX_HoudiniApi::GetStringBufLengthImpl = &HEMAX_HoudiniApi::GetStringBufLengthEmptyStub;

HEMAX_HoudiniApi::GetSupportedImageFileFormatCountFuncPtr
HEMAX_HoudiniApi::GetSupportedImageFileFormatCountImpl = &HEMAX_HoudiniApi::GetSupportedImageFileFormatCountEmptyStub;

HEMAX_HoudiniApi::GetSupportedImageFileFormatsFuncPtr
HEMAX_HoudiniApi::GetSupportedImageFileFormatsImpl = &HEMAX_HoudiniApi::GetSupportedImageFileFormatsEmptyStub;

HEMAX_HoudiniApi::GetTimeFuncPtr
HEMAX_HoudiniApi::GetTimeImpl = &HEMAX_HoudiniApi::GetTimeEmptyStub;

HEMAX_HoudiniApi::GetTimelineOptionsFuncPtr
HEMAX_HoudiniApi::GetTimelineOptionsImpl = &HEMAX_HoudiniApi::GetTimelineOptionsEmptyStub;

HEMAX_HoudiniApi::GetTotalCookCountFuncPtr
HEMAX_HoudiniApi::GetTotalCookCountImpl = &HEMAX_HoudiniApi::GetTotalCookCountEmptyStub;

HEMAX_HoudiniApi::GetUseHoudiniTimeFuncPtr
HEMAX_HoudiniApi::GetUseHoudiniTimeImpl = &HEMAX_HoudiniApi::GetUseHoudiniTimeEmptyStub;

HEMAX_HoudiniApi::GetVertexListFuncPtr
HEMAX_HoudiniApi::GetVertexListImpl = &HEMAX_HoudiniApi::GetVertexListEmptyStub;

HEMAX_HoudiniApi::GetViewportFuncPtr
HEMAX_HoudiniApi::GetViewportImpl = &HEMAX_HoudiniApi::GetViewportEmptyStub;

HEMAX_HoudiniApi::GetVolumeBoundsFuncPtr
HEMAX_HoudiniApi::GetVolumeBoundsImpl = &HEMAX_HoudiniApi::GetVolumeBoundsEmptyStub;

HEMAX_HoudiniApi::GetVolumeInfoFuncPtr
HEMAX_HoudiniApi::GetVolumeInfoImpl = &HEMAX_HoudiniApi::GetVolumeInfoEmptyStub;

HEMAX_HoudiniApi::GetVolumeTileFloatDataFuncPtr
HEMAX_HoudiniApi::GetVolumeTileFloatDataImpl = &HEMAX_HoudiniApi::GetVolumeTileFloatDataEmptyStub;

HEMAX_HoudiniApi::GetVolumeTileIntDataFuncPtr
HEMAX_HoudiniApi::GetVolumeTileIntDataImpl = &HEMAX_HoudiniApi::GetVolumeTileIntDataEmptyStub;

HEMAX_HoudiniApi::GetVolumeVisualInfoFuncPtr
HEMAX_HoudiniApi::GetVolumeVisualInfoImpl = &HEMAX_HoudiniApi::GetVolumeVisualInfoEmptyStub;

HEMAX_HoudiniApi::GetVolumeVoxelFloatDataFuncPtr
HEMAX_HoudiniApi::GetVolumeVoxelFloatDataImpl = &HEMAX_HoudiniApi::GetVolumeVoxelFloatDataEmptyStub;

HEMAX_HoudiniApi::GetVolumeVoxelIntDataFuncPtr
HEMAX_HoudiniApi::GetVolumeVoxelIntDataImpl = &HEMAX_HoudiniApi::GetVolumeVoxelIntDataEmptyStub;

HEMAX_HoudiniApi::GetWorkItemAttributeSizeFuncPtr
HEMAX_HoudiniApi::GetWorkItemAttributeSizeImpl = &HEMAX_HoudiniApi::GetWorkItemAttributeSizeEmptyStub;

HEMAX_HoudiniApi::GetWorkItemFloatAttributeFuncPtr
HEMAX_HoudiniApi::GetWorkItemFloatAttributeImpl = &HEMAX_HoudiniApi::GetWorkItemFloatAttributeEmptyStub;

HEMAX_HoudiniApi::GetWorkItemInfoFuncPtr
HEMAX_HoudiniApi::GetWorkItemInfoImpl = &HEMAX_HoudiniApi::GetWorkItemInfoEmptyStub;

HEMAX_HoudiniApi::GetWorkItemIntAttributeFuncPtr
HEMAX_HoudiniApi::GetWorkItemIntAttributeImpl = &HEMAX_HoudiniApi::GetWorkItemIntAttributeEmptyStub;

HEMAX_HoudiniApi::GetWorkItemOutputFilesFuncPtr
HEMAX_HoudiniApi::GetWorkItemOutputFilesImpl = &HEMAX_HoudiniApi::GetWorkItemOutputFilesEmptyStub;

HEMAX_HoudiniApi::GetWorkItemStringAttributeFuncPtr
HEMAX_HoudiniApi::GetWorkItemStringAttributeImpl = &HEMAX_HoudiniApi::GetWorkItemStringAttributeEmptyStub;

HEMAX_HoudiniApi::GetWorkItemsFuncPtr
HEMAX_HoudiniApi::GetWorkItemsImpl = &HEMAX_HoudiniApi::GetWorkItemsEmptyStub;

HEMAX_HoudiniApi::GetWorkitemDataLengthFuncPtr
HEMAX_HoudiniApi::GetWorkitemDataLengthImpl = &HEMAX_HoudiniApi::GetWorkitemDataLengthEmptyStub;

HEMAX_HoudiniApi::GetWorkitemFloatDataFuncPtr
HEMAX_HoudiniApi::GetWorkitemFloatDataImpl = &HEMAX_HoudiniApi::GetWorkitemFloatDataEmptyStub;

HEMAX_HoudiniApi::GetWorkitemInfoFuncPtr
HEMAX_HoudiniApi::GetWorkitemInfoImpl = &HEMAX_HoudiniApi::GetWorkitemInfoEmptyStub;

HEMAX_HoudiniApi::GetWorkitemIntDataFuncPtr
HEMAX_HoudiniApi::GetWorkitemIntDataImpl = &HEMAX_HoudiniApi::GetWorkitemIntDataEmptyStub;

HEMAX_HoudiniApi::GetWorkitemResultInfoFuncPtr
HEMAX_HoudiniApi::GetWorkitemResultInfoImpl = &HEMAX_HoudiniApi::GetWorkitemResultInfoEmptyStub;

HEMAX_HoudiniApi::GetWorkitemStringDataFuncPtr
HEMAX_HoudiniApi::GetWorkitemStringDataImpl = &HEMAX_HoudiniApi::GetWorkitemStringDataEmptyStub;

HEMAX_HoudiniApi::GetWorkitemsFuncPtr
HEMAX_HoudiniApi::GetWorkitemsImpl = &HEMAX_HoudiniApi::GetWorkitemsEmptyStub;

HEMAX_HoudiniApi::InitializeFuncPtr
HEMAX_HoudiniApi::InitializeImpl = &HEMAX_HoudiniApi::InitializeEmptyStub;

HEMAX_HoudiniApi::InsertMultiparmInstanceFuncPtr
HEMAX_HoudiniApi::InsertMultiparmInstanceImpl = &HEMAX_HoudiniApi::InsertMultiparmInstanceEmptyStub;

HEMAX_HoudiniApi::InterruptFuncPtr
HEMAX_HoudiniApi::InterruptImpl = &HEMAX_HoudiniApi::InterruptEmptyStub;

HEMAX_HoudiniApi::IsInitializedFuncPtr
HEMAX_HoudiniApi::IsInitializedImpl = &HEMAX_HoudiniApi::IsInitializedEmptyStub;

HEMAX_HoudiniApi::IsNodeValidFuncPtr
HEMAX_HoudiniApi::IsNodeValidImpl = &HEMAX_HoudiniApi::IsNodeValidEmptyStub;

HEMAX_HoudiniApi::IsSessionValidFuncPtr
HEMAX_HoudiniApi::IsSessionValidImpl = &HEMAX_HoudiniApi::IsSessionValidEmptyStub;

HEMAX_HoudiniApi::LoadAssetLibraryFromFileFuncPtr
HEMAX_HoudiniApi::LoadAssetLibraryFromFileImpl = &HEMAX_HoudiniApi::LoadAssetLibraryFromFileEmptyStub;

HEMAX_HoudiniApi::LoadAssetLibraryFromMemoryFuncPtr
HEMAX_HoudiniApi::LoadAssetLibraryFromMemoryImpl = &HEMAX_HoudiniApi::LoadAssetLibraryFromMemoryEmptyStub;

HEMAX_HoudiniApi::LoadGeoFromFileFuncPtr
HEMAX_HoudiniApi::LoadGeoFromFileImpl = &HEMAX_HoudiniApi::LoadGeoFromFileEmptyStub;

HEMAX_HoudiniApi::LoadGeoFromMemoryFuncPtr
HEMAX_HoudiniApi::LoadGeoFromMemoryImpl = &HEMAX_HoudiniApi::LoadGeoFromMemoryEmptyStub;

HEMAX_HoudiniApi::LoadHIPFileFuncPtr
HEMAX_HoudiniApi::LoadHIPFileImpl = &HEMAX_HoudiniApi::LoadHIPFileEmptyStub;

HEMAX_HoudiniApi::LoadNodeFromFileFuncPtr
HEMAX_HoudiniApi::LoadNodeFromFileImpl = &HEMAX_HoudiniApi::LoadNodeFromFileEmptyStub;

HEMAX_HoudiniApi::MergeHIPFileFuncPtr
HEMAX_HoudiniApi::MergeHIPFileImpl = &HEMAX_HoudiniApi::MergeHIPFileEmptyStub;

HEMAX_HoudiniApi::ParmHasExpressionFuncPtr
HEMAX_HoudiniApi::ParmHasExpressionImpl = &HEMAX_HoudiniApi::ParmHasExpressionEmptyStub;

HEMAX_HoudiniApi::ParmHasTagFuncPtr
HEMAX_HoudiniApi::ParmHasTagImpl = &HEMAX_HoudiniApi::ParmHasTagEmptyStub;

HEMAX_HoudiniApi::PausePDGCookFuncPtr
HEMAX_HoudiniApi::PausePDGCookImpl = &HEMAX_HoudiniApi::PausePDGCookEmptyStub;

HEMAX_HoudiniApi::PythonThreadInterpreterLockFuncPtr
HEMAX_HoudiniApi::PythonThreadInterpreterLockImpl = &HEMAX_HoudiniApi::PythonThreadInterpreterLockEmptyStub;

HEMAX_HoudiniApi::QueryNodeInputFuncPtr
HEMAX_HoudiniApi::QueryNodeInputImpl = &HEMAX_HoudiniApi::QueryNodeInputEmptyStub;

HEMAX_HoudiniApi::QueryNodeOutputConnectedCountFuncPtr
HEMAX_HoudiniApi::QueryNodeOutputConnectedCountImpl = &HEMAX_HoudiniApi::QueryNodeOutputConnectedCountEmptyStub;

HEMAX_HoudiniApi::QueryNodeOutputConnectedNodesFuncPtr
HEMAX_HoudiniApi::QueryNodeOutputConnectedNodesImpl = &HEMAX_HoudiniApi::QueryNodeOutputConnectedNodesEmptyStub;

HEMAX_HoudiniApi::RemoveCustomStringFuncPtr
HEMAX_HoudiniApi::RemoveCustomStringImpl = &HEMAX_HoudiniApi::RemoveCustomStringEmptyStub;

HEMAX_HoudiniApi::RemoveMultiparmInstanceFuncPtr
HEMAX_HoudiniApi::RemoveMultiparmInstanceImpl = &HEMAX_HoudiniApi::RemoveMultiparmInstanceEmptyStub;

HEMAX_HoudiniApi::RemoveParmExpressionFuncPtr
HEMAX_HoudiniApi::RemoveParmExpressionImpl = &HEMAX_HoudiniApi::RemoveParmExpressionEmptyStub;

HEMAX_HoudiniApi::RenameNodeFuncPtr
HEMAX_HoudiniApi::RenameNodeImpl = &HEMAX_HoudiniApi::RenameNodeEmptyStub;

HEMAX_HoudiniApi::RenderCOPToImageFuncPtr
HEMAX_HoudiniApi::RenderCOPToImageImpl = &HEMAX_HoudiniApi::RenderCOPToImageEmptyStub;

HEMAX_HoudiniApi::RenderTextureToImageFuncPtr
HEMAX_HoudiniApi::RenderTextureToImageImpl = &HEMAX_HoudiniApi::RenderTextureToImageEmptyStub;

HEMAX_HoudiniApi::ResetSimulationFuncPtr
HEMAX_HoudiniApi::ResetSimulationImpl = &HEMAX_HoudiniApi::ResetSimulationEmptyStub;

HEMAX_HoudiniApi::RevertGeoFuncPtr
HEMAX_HoudiniApi::RevertGeoImpl = &HEMAX_HoudiniApi::RevertGeoEmptyStub;

HEMAX_HoudiniApi::RevertParmToDefaultFuncPtr
HEMAX_HoudiniApi::RevertParmToDefaultImpl = &HEMAX_HoudiniApi::RevertParmToDefaultEmptyStub;

HEMAX_HoudiniApi::RevertParmToDefaultsFuncPtr
HEMAX_HoudiniApi::RevertParmToDefaultsImpl = &HEMAX_HoudiniApi::RevertParmToDefaultsEmptyStub;

HEMAX_HoudiniApi::SaveGeoToFileFuncPtr
HEMAX_HoudiniApi::SaveGeoToFileImpl = &HEMAX_HoudiniApi::SaveGeoToFileEmptyStub;

HEMAX_HoudiniApi::SaveGeoToMemoryFuncPtr
HEMAX_HoudiniApi::SaveGeoToMemoryImpl = &HEMAX_HoudiniApi::SaveGeoToMemoryEmptyStub;

HEMAX_HoudiniApi::SaveHIPFileFuncPtr
HEMAX_HoudiniApi::SaveHIPFileImpl = &HEMAX_HoudiniApi::SaveHIPFileEmptyStub;

HEMAX_HoudiniApi::SaveNodeToFileFuncPtr
HEMAX_HoudiniApi::SaveNodeToFileImpl = &HEMAX_HoudiniApi::SaveNodeToFileEmptyStub;

HEMAX_HoudiniApi::SetAnimCurveFuncPtr
HEMAX_HoudiniApi::SetAnimCurveImpl = &HEMAX_HoudiniApi::SetAnimCurveEmptyStub;

HEMAX_HoudiniApi::SetAttributeFloat64ArrayDataFuncPtr
HEMAX_HoudiniApi::SetAttributeFloat64ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeFloat64ArrayDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeFloat64DataFuncPtr
HEMAX_HoudiniApi::SetAttributeFloat64DataImpl = &HEMAX_HoudiniApi::SetAttributeFloat64DataEmptyStub;

HEMAX_HoudiniApi::SetAttributeFloatArrayDataFuncPtr
HEMAX_HoudiniApi::SetAttributeFloatArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeFloatArrayDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeFloatDataFuncPtr
HEMAX_HoudiniApi::SetAttributeFloatDataImpl = &HEMAX_HoudiniApi::SetAttributeFloatDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeInt16ArrayDataFuncPtr
HEMAX_HoudiniApi::SetAttributeInt16ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeInt16ArrayDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeInt16DataFuncPtr
HEMAX_HoudiniApi::SetAttributeInt16DataImpl = &HEMAX_HoudiniApi::SetAttributeInt16DataEmptyStub;

HEMAX_HoudiniApi::SetAttributeInt64ArrayDataFuncPtr
HEMAX_HoudiniApi::SetAttributeInt64ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeInt64ArrayDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeInt64DataFuncPtr
HEMAX_HoudiniApi::SetAttributeInt64DataImpl = &HEMAX_HoudiniApi::SetAttributeInt64DataEmptyStub;

HEMAX_HoudiniApi::SetAttributeInt8ArrayDataFuncPtr
HEMAX_HoudiniApi::SetAttributeInt8ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeInt8ArrayDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeInt8DataFuncPtr
HEMAX_HoudiniApi::SetAttributeInt8DataImpl = &HEMAX_HoudiniApi::SetAttributeInt8DataEmptyStub;

HEMAX_HoudiniApi::SetAttributeIntArrayDataFuncPtr
HEMAX_HoudiniApi::SetAttributeIntArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeIntArrayDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeIntDataFuncPtr
HEMAX_HoudiniApi::SetAttributeIntDataImpl = &HEMAX_HoudiniApi::SetAttributeIntDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeStringArrayDataFuncPtr
HEMAX_HoudiniApi::SetAttributeStringArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeStringArrayDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeStringDataFuncPtr
HEMAX_HoudiniApi::SetAttributeStringDataImpl = &HEMAX_HoudiniApi::SetAttributeStringDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeUInt8ArrayDataFuncPtr
HEMAX_HoudiniApi::SetAttributeUInt8ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeUInt8ArrayDataEmptyStub;

HEMAX_HoudiniApi::SetAttributeUInt8DataFuncPtr
HEMAX_HoudiniApi::SetAttributeUInt8DataImpl = &HEMAX_HoudiniApi::SetAttributeUInt8DataEmptyStub;

HEMAX_HoudiniApi::SetCachePropertyFuncPtr
HEMAX_HoudiniApi::SetCachePropertyImpl = &HEMAX_HoudiniApi::SetCachePropertyEmptyStub;

HEMAX_HoudiniApi::SetCompositorOptionsFuncPtr
HEMAX_HoudiniApi::SetCompositorOptionsImpl = &HEMAX_HoudiniApi::SetCompositorOptionsEmptyStub;

HEMAX_HoudiniApi::SetCurveCountsFuncPtr
HEMAX_HoudiniApi::SetCurveCountsImpl = &HEMAX_HoudiniApi::SetCurveCountsEmptyStub;

HEMAX_HoudiniApi::SetCurveInfoFuncPtr
HEMAX_HoudiniApi::SetCurveInfoImpl = &HEMAX_HoudiniApi::SetCurveInfoEmptyStub;

HEMAX_HoudiniApi::SetCurveKnotsFuncPtr
HEMAX_HoudiniApi::SetCurveKnotsImpl = &HEMAX_HoudiniApi::SetCurveKnotsEmptyStub;

HEMAX_HoudiniApi::SetCurveOrdersFuncPtr
HEMAX_HoudiniApi::SetCurveOrdersImpl = &HEMAX_HoudiniApi::SetCurveOrdersEmptyStub;

HEMAX_HoudiniApi::SetCustomStringFuncPtr
HEMAX_HoudiniApi::SetCustomStringImpl = &HEMAX_HoudiniApi::SetCustomStringEmptyStub;

HEMAX_HoudiniApi::SetFaceCountsFuncPtr
HEMAX_HoudiniApi::SetFaceCountsImpl = &HEMAX_HoudiniApi::SetFaceCountsEmptyStub;

HEMAX_HoudiniApi::SetGroupMembershipFuncPtr
HEMAX_HoudiniApi::SetGroupMembershipImpl = &HEMAX_HoudiniApi::SetGroupMembershipEmptyStub;

HEMAX_HoudiniApi::SetHeightFieldDataFuncPtr
HEMAX_HoudiniApi::SetHeightFieldDataImpl = &HEMAX_HoudiniApi::SetHeightFieldDataEmptyStub;

HEMAX_HoudiniApi::SetImageInfoFuncPtr
HEMAX_HoudiniApi::SetImageInfoImpl = &HEMAX_HoudiniApi::SetImageInfoEmptyStub;

HEMAX_HoudiniApi::SetInputCurveInfoFuncPtr
HEMAX_HoudiniApi::SetInputCurveInfoImpl = &HEMAX_HoudiniApi::SetInputCurveInfoEmptyStub;

HEMAX_HoudiniApi::SetInputCurvePositionsFuncPtr
HEMAX_HoudiniApi::SetInputCurvePositionsImpl = &HEMAX_HoudiniApi::SetInputCurvePositionsEmptyStub;

HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScalesFuncPtr
HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScalesImpl = &HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScalesEmptyStub;

HEMAX_HoudiniApi::SetNodeDisplayFuncPtr
HEMAX_HoudiniApi::SetNodeDisplayImpl = &HEMAX_HoudiniApi::SetNodeDisplayEmptyStub;

HEMAX_HoudiniApi::SetObjectTransformFuncPtr
HEMAX_HoudiniApi::SetObjectTransformImpl = &HEMAX_HoudiniApi::SetObjectTransformEmptyStub;

HEMAX_HoudiniApi::SetParmExpressionFuncPtr
HEMAX_HoudiniApi::SetParmExpressionImpl = &HEMAX_HoudiniApi::SetParmExpressionEmptyStub;

HEMAX_HoudiniApi::SetParmFloatValueFuncPtr
HEMAX_HoudiniApi::SetParmFloatValueImpl = &HEMAX_HoudiniApi::SetParmFloatValueEmptyStub;

HEMAX_HoudiniApi::SetParmFloatValuesFuncPtr
HEMAX_HoudiniApi::SetParmFloatValuesImpl = &HEMAX_HoudiniApi::SetParmFloatValuesEmptyStub;

HEMAX_HoudiniApi::SetParmIntValueFuncPtr
HEMAX_HoudiniApi::SetParmIntValueImpl = &HEMAX_HoudiniApi::SetParmIntValueEmptyStub;

HEMAX_HoudiniApi::SetParmIntValuesFuncPtr
HEMAX_HoudiniApi::SetParmIntValuesImpl = &HEMAX_HoudiniApi::SetParmIntValuesEmptyStub;

HEMAX_HoudiniApi::SetParmNodeValueFuncPtr
HEMAX_HoudiniApi::SetParmNodeValueImpl = &HEMAX_HoudiniApi::SetParmNodeValueEmptyStub;

HEMAX_HoudiniApi::SetParmStringValueFuncPtr
HEMAX_HoudiniApi::SetParmStringValueImpl = &HEMAX_HoudiniApi::SetParmStringValueEmptyStub;

HEMAX_HoudiniApi::SetPartInfoFuncPtr
HEMAX_HoudiniApi::SetPartInfoImpl = &HEMAX_HoudiniApi::SetPartInfoEmptyStub;

HEMAX_HoudiniApi::SetPresetFuncPtr
HEMAX_HoudiniApi::SetPresetImpl = &HEMAX_HoudiniApi::SetPresetEmptyStub;

HEMAX_HoudiniApi::SetServerEnvIntFuncPtr
HEMAX_HoudiniApi::SetServerEnvIntImpl = &HEMAX_HoudiniApi::SetServerEnvIntEmptyStub;

HEMAX_HoudiniApi::SetServerEnvStringFuncPtr
HEMAX_HoudiniApi::SetServerEnvStringImpl = &HEMAX_HoudiniApi::SetServerEnvStringEmptyStub;

HEMAX_HoudiniApi::SetSessionSyncFuncPtr
HEMAX_HoudiniApi::SetSessionSyncImpl = &HEMAX_HoudiniApi::SetSessionSyncEmptyStub;

HEMAX_HoudiniApi::SetSessionSyncInfoFuncPtr
HEMAX_HoudiniApi::SetSessionSyncInfoImpl = &HEMAX_HoudiniApi::SetSessionSyncInfoEmptyStub;

HEMAX_HoudiniApi::SetTimeFuncPtr
HEMAX_HoudiniApi::SetTimeImpl = &HEMAX_HoudiniApi::SetTimeEmptyStub;

HEMAX_HoudiniApi::SetTimelineOptionsFuncPtr
HEMAX_HoudiniApi::SetTimelineOptionsImpl = &HEMAX_HoudiniApi::SetTimelineOptionsEmptyStub;

HEMAX_HoudiniApi::SetTransformAnimCurveFuncPtr
HEMAX_HoudiniApi::SetTransformAnimCurveImpl = &HEMAX_HoudiniApi::SetTransformAnimCurveEmptyStub;

HEMAX_HoudiniApi::SetUseHoudiniTimeFuncPtr
HEMAX_HoudiniApi::SetUseHoudiniTimeImpl = &HEMAX_HoudiniApi::SetUseHoudiniTimeEmptyStub;

HEMAX_HoudiniApi::SetVertexListFuncPtr
HEMAX_HoudiniApi::SetVertexListImpl = &HEMAX_HoudiniApi::SetVertexListEmptyStub;

HEMAX_HoudiniApi::SetViewportFuncPtr
HEMAX_HoudiniApi::SetViewportImpl = &HEMAX_HoudiniApi::SetViewportEmptyStub;

HEMAX_HoudiniApi::SetVolumeInfoFuncPtr
HEMAX_HoudiniApi::SetVolumeInfoImpl = &HEMAX_HoudiniApi::SetVolumeInfoEmptyStub;

HEMAX_HoudiniApi::SetVolumeTileFloatDataFuncPtr
HEMAX_HoudiniApi::SetVolumeTileFloatDataImpl = &HEMAX_HoudiniApi::SetVolumeTileFloatDataEmptyStub;

HEMAX_HoudiniApi::SetVolumeTileIntDataFuncPtr
HEMAX_HoudiniApi::SetVolumeTileIntDataImpl = &HEMAX_HoudiniApi::SetVolumeTileIntDataEmptyStub;

HEMAX_HoudiniApi::SetVolumeVoxelFloatDataFuncPtr
HEMAX_HoudiniApi::SetVolumeVoxelFloatDataImpl = &HEMAX_HoudiniApi::SetVolumeVoxelFloatDataEmptyStub;

HEMAX_HoudiniApi::SetVolumeVoxelIntDataFuncPtr
HEMAX_HoudiniApi::SetVolumeVoxelIntDataImpl = &HEMAX_HoudiniApi::SetVolumeVoxelIntDataEmptyStub;

HEMAX_HoudiniApi::SetWorkItemFloatAttributeFuncPtr
HEMAX_HoudiniApi::SetWorkItemFloatAttributeImpl = &HEMAX_HoudiniApi::SetWorkItemFloatAttributeEmptyStub;

HEMAX_HoudiniApi::SetWorkItemIntAttributeFuncPtr
HEMAX_HoudiniApi::SetWorkItemIntAttributeImpl = &HEMAX_HoudiniApi::SetWorkItemIntAttributeEmptyStub;

HEMAX_HoudiniApi::SetWorkItemStringAttributeFuncPtr
HEMAX_HoudiniApi::SetWorkItemStringAttributeImpl = &HEMAX_HoudiniApi::SetWorkItemStringAttributeEmptyStub;

HEMAX_HoudiniApi::SetWorkitemFloatDataFuncPtr
HEMAX_HoudiniApi::SetWorkitemFloatDataImpl = &HEMAX_HoudiniApi::SetWorkitemFloatDataEmptyStub;

HEMAX_HoudiniApi::SetWorkitemIntDataFuncPtr
HEMAX_HoudiniApi::SetWorkitemIntDataImpl = &HEMAX_HoudiniApi::SetWorkitemIntDataEmptyStub;

HEMAX_HoudiniApi::SetWorkitemStringDataFuncPtr
HEMAX_HoudiniApi::SetWorkitemStringDataImpl = &HEMAX_HoudiniApi::SetWorkitemStringDataEmptyStub;

HEMAX_HoudiniApi::ShutdownFuncPtr
HEMAX_HoudiniApi::ShutdownImpl = &HEMAX_HoudiniApi::ShutdownEmptyStub;

HEMAX_HoudiniApi::StartThriftNamedPipeServerFuncPtr
HEMAX_HoudiniApi::StartThriftNamedPipeServerImpl = &HEMAX_HoudiniApi::StartThriftNamedPipeServerEmptyStub;

HEMAX_HoudiniApi::StartThriftSocketServerFuncPtr
HEMAX_HoudiniApi::StartThriftSocketServerImpl = &HEMAX_HoudiniApi::StartThriftSocketServerEmptyStub;


void
HEMAX_HoudiniApi::InitializeHAPI(void* LibraryHandle)
{
    if(!LibraryHandle) return;

    HEMAX_HoudiniApi::AddAttributeImpl = (AddAttributeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_AddAttribute");
    HEMAX_HoudiniApi::AddGroupImpl = (AddGroupFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_AddGroup");
    HEMAX_HoudiniApi::BindCustomImplementationImpl = (BindCustomImplementationFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_BindCustomImplementation");
    HEMAX_HoudiniApi::CancelPDGCookImpl = (CancelPDGCookFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CancelPDGCook");
    HEMAX_HoudiniApi::CheckForSpecificErrorsImpl = (CheckForSpecificErrorsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CheckForSpecificErrors");
    HEMAX_HoudiniApi::CleanupImpl = (CleanupFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_Cleanup");
    HEMAX_HoudiniApi::ClearConnectionErrorImpl = (ClearConnectionErrorFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ClearConnectionError");
    HEMAX_HoudiniApi::CloseSessionImpl = (CloseSessionFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CloseSession");
    HEMAX_HoudiniApi::CommitGeoImpl = (CommitGeoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CommitGeo");
    HEMAX_HoudiniApi::CommitWorkItemsImpl = (CommitWorkItemsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CommitWorkItems");
    HEMAX_HoudiniApi::CommitWorkitemsImpl = (CommitWorkitemsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CommitWorkitems");
    HEMAX_HoudiniApi::ComposeChildNodeListImpl = (ComposeChildNodeListFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ComposeChildNodeList");
    HEMAX_HoudiniApi::ComposeNodeCookResultImpl = (ComposeNodeCookResultFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ComposeNodeCookResult");
    HEMAX_HoudiniApi::ComposeObjectListImpl = (ComposeObjectListFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ComposeObjectList");
    HEMAX_HoudiniApi::ConnectNodeInputImpl = (ConnectNodeInputFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ConnectNodeInput");
    HEMAX_HoudiniApi::ConvertMatrixToEulerImpl = (ConvertMatrixToEulerFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ConvertMatrixToEuler");
    HEMAX_HoudiniApi::ConvertMatrixToQuatImpl = (ConvertMatrixToQuatFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ConvertMatrixToQuat");
    HEMAX_HoudiniApi::ConvertTransformImpl = (ConvertTransformFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ConvertTransform");
    HEMAX_HoudiniApi::ConvertTransformEulerToMatrixImpl = (ConvertTransformEulerToMatrixFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ConvertTransformEulerToMatrix");
    HEMAX_HoudiniApi::ConvertTransformQuatToMatrixImpl = (ConvertTransformQuatToMatrixFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ConvertTransformQuatToMatrix");
    HEMAX_HoudiniApi::CookNodeImpl = (CookNodeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CookNode");
    HEMAX_HoudiniApi::CookPDGImpl = (CookPDGFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CookPDG");
    HEMAX_HoudiniApi::CookPDGAllOutputsImpl = (CookPDGAllOutputsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CookPDGAllOutputs");
    HEMAX_HoudiniApi::CreateCustomSessionImpl = (CreateCustomSessionFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateCustomSession");
    HEMAX_HoudiniApi::CreateHeightFieldInputImpl = (CreateHeightFieldInputFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateHeightFieldInput");
    HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNodeImpl = (CreateHeightfieldInputVolumeNodeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateHeightfieldInputVolumeNode");
    HEMAX_HoudiniApi::CreateInProcessSessionImpl = (CreateInProcessSessionFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateInProcessSession");
    HEMAX_HoudiniApi::CreateInputCurveNodeImpl = (CreateInputCurveNodeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateInputCurveNode");
    HEMAX_HoudiniApi::CreateInputNodeImpl = (CreateInputNodeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateInputNode");
    HEMAX_HoudiniApi::CreateNodeImpl = (CreateNodeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateNode");
    HEMAX_HoudiniApi::CreateThriftNamedPipeSessionImpl = (CreateThriftNamedPipeSessionFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateThriftNamedPipeSession");
    HEMAX_HoudiniApi::CreateThriftSocketSessionImpl = (CreateThriftSocketSessionFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateThriftSocketSession");
    HEMAX_HoudiniApi::CreateWorkItemImpl = (CreateWorkItemFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateWorkItem");
    HEMAX_HoudiniApi::CreateWorkitemImpl = (CreateWorkitemFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_CreateWorkitem");
    HEMAX_HoudiniApi::DeleteAttributeImpl = (DeleteAttributeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_DeleteAttribute");
    HEMAX_HoudiniApi::DeleteGroupImpl = (DeleteGroupFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_DeleteGroup");
    HEMAX_HoudiniApi::DeleteNodeImpl = (DeleteNodeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_DeleteNode");
    HEMAX_HoudiniApi::DirtyPDGNodeImpl = (DirtyPDGNodeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_DirtyPDGNode");
    HEMAX_HoudiniApi::DisconnectNodeInputImpl = (DisconnectNodeInputFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_DisconnectNodeInput");
    HEMAX_HoudiniApi::DisconnectNodeOutputsAtImpl = (DisconnectNodeOutputsAtFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_DisconnectNodeOutputsAt");
    HEMAX_HoudiniApi::ExtractImageToFileImpl = (ExtractImageToFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ExtractImageToFile");
    HEMAX_HoudiniApi::ExtractImageToMemoryImpl = (ExtractImageToMemoryFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ExtractImageToMemory");
    HEMAX_HoudiniApi::GetActiveCacheCountImpl = (GetActiveCacheCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetActiveCacheCount");
    HEMAX_HoudiniApi::GetActiveCacheNamesImpl = (GetActiveCacheNamesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetActiveCacheNames");
    HEMAX_HoudiniApi::GetAssetDefinitionParmCountsImpl = (GetAssetDefinitionParmCountsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAssetDefinitionParmCounts");
    HEMAX_HoudiniApi::GetAssetDefinitionParmInfosImpl = (GetAssetDefinitionParmInfosFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAssetDefinitionParmInfos");
    HEMAX_HoudiniApi::GetAssetDefinitionParmValuesImpl = (GetAssetDefinitionParmValuesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAssetDefinitionParmValues");
    HEMAX_HoudiniApi::GetAssetInfoImpl = (GetAssetInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAssetInfo");
    HEMAX_HoudiniApi::GetAttributeFloat64ArrayDataImpl = (GetAttributeFloat64ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeFloat64ArrayData");
    HEMAX_HoudiniApi::GetAttributeFloat64DataImpl = (GetAttributeFloat64DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeFloat64Data");
    HEMAX_HoudiniApi::GetAttributeFloatArrayDataImpl = (GetAttributeFloatArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeFloatArrayData");
    HEMAX_HoudiniApi::GetAttributeFloatDataImpl = (GetAttributeFloatDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeFloatData");
    HEMAX_HoudiniApi::GetAttributeInfoImpl = (GetAttributeInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeInfo");
    HEMAX_HoudiniApi::GetAttributeInt16ArrayDataImpl = (GetAttributeInt16ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeInt16ArrayData");
    HEMAX_HoudiniApi::GetAttributeInt16DataImpl = (GetAttributeInt16DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeInt16Data");
    HEMAX_HoudiniApi::GetAttributeInt64ArrayDataImpl = (GetAttributeInt64ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeInt64ArrayData");
    HEMAX_HoudiniApi::GetAttributeInt64DataImpl = (GetAttributeInt64DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeInt64Data");
    HEMAX_HoudiniApi::GetAttributeInt8ArrayDataImpl = (GetAttributeInt8ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeInt8ArrayData");
    HEMAX_HoudiniApi::GetAttributeInt8DataImpl = (GetAttributeInt8DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeInt8Data");
    HEMAX_HoudiniApi::GetAttributeIntArrayDataImpl = (GetAttributeIntArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeIntArrayData");
    HEMAX_HoudiniApi::GetAttributeIntDataImpl = (GetAttributeIntDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeIntData");
    HEMAX_HoudiniApi::GetAttributeNamesImpl = (GetAttributeNamesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeNames");
    HEMAX_HoudiniApi::GetAttributeStringArrayDataImpl = (GetAttributeStringArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeStringArrayData");
    HEMAX_HoudiniApi::GetAttributeStringDataImpl = (GetAttributeStringDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeStringData");
    HEMAX_HoudiniApi::GetAttributeUInt8ArrayDataImpl = (GetAttributeUInt8ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeUInt8ArrayData");
    HEMAX_HoudiniApi::GetAttributeUInt8DataImpl = (GetAttributeUInt8DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAttributeUInt8Data");
    HEMAX_HoudiniApi::GetAvailableAssetCountImpl = (GetAvailableAssetCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAvailableAssetCount");
    HEMAX_HoudiniApi::GetAvailableAssetsImpl = (GetAvailableAssetsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetAvailableAssets");
    HEMAX_HoudiniApi::GetBoxInfoImpl = (GetBoxInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetBoxInfo");
    HEMAX_HoudiniApi::GetCachePropertyImpl = (GetCachePropertyFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetCacheProperty");
    HEMAX_HoudiniApi::GetComposedChildNodeListImpl = (GetComposedChildNodeListFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetComposedChildNodeList");
    HEMAX_HoudiniApi::GetComposedNodeCookResultImpl = (GetComposedNodeCookResultFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetComposedNodeCookResult");
    HEMAX_HoudiniApi::GetComposedObjectListImpl = (GetComposedObjectListFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetComposedObjectList");
    HEMAX_HoudiniApi::GetComposedObjectTransformsImpl = (GetComposedObjectTransformsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetComposedObjectTransforms");
    HEMAX_HoudiniApi::GetCompositorOptionsImpl = (GetCompositorOptionsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetCompositorOptions");
    HEMAX_HoudiniApi::GetConnectionErrorImpl = (GetConnectionErrorFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetConnectionError");
    HEMAX_HoudiniApi::GetConnectionErrorLengthImpl = (GetConnectionErrorLengthFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetConnectionErrorLength");
    HEMAX_HoudiniApi::GetCookingCurrentCountImpl = (GetCookingCurrentCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetCookingCurrentCount");
    HEMAX_HoudiniApi::GetCookingTotalCountImpl = (GetCookingTotalCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetCookingTotalCount");
    HEMAX_HoudiniApi::GetCurveCountsImpl = (GetCurveCountsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetCurveCounts");
    HEMAX_HoudiniApi::GetCurveInfoImpl = (GetCurveInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetCurveInfo");
    HEMAX_HoudiniApi::GetCurveKnotsImpl = (GetCurveKnotsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetCurveKnots");
    HEMAX_HoudiniApi::GetCurveOrdersImpl = (GetCurveOrdersFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetCurveOrders");
    HEMAX_HoudiniApi::GetDisplayGeoInfoImpl = (GetDisplayGeoInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetDisplayGeoInfo");
    HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroupImpl = (GetEdgeCountOfEdgeGroupFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetEdgeCountOfEdgeGroup");
    HEMAX_HoudiniApi::GetEnvIntImpl = (GetEnvIntFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetEnvInt");
    HEMAX_HoudiniApi::GetFaceCountsImpl = (GetFaceCountsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetFaceCounts");
    HEMAX_HoudiniApi::GetFirstVolumeTileImpl = (GetFirstVolumeTileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetFirstVolumeTile");
    HEMAX_HoudiniApi::GetGeoInfoImpl = (GetGeoInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetGeoInfo");
    HEMAX_HoudiniApi::GetGeoSizeImpl = (GetGeoSizeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetGeoSize");
    HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePartImpl = (GetGroupCountOnPackedInstancePartFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetGroupCountOnPackedInstancePart");
    HEMAX_HoudiniApi::GetGroupMembershipImpl = (GetGroupMembershipFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetGroupMembership");
    HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePartImpl = (GetGroupMembershipOnPackedInstancePartFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetGroupMembershipOnPackedInstancePart");
    HEMAX_HoudiniApi::GetGroupNamesImpl = (GetGroupNamesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetGroupNames");
    HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePartImpl = (GetGroupNamesOnPackedInstancePartFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetGroupNamesOnPackedInstancePart");
    HEMAX_HoudiniApi::GetHIPFileNodeCountImpl = (GetHIPFileNodeCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetHIPFileNodeCount");
    HEMAX_HoudiniApi::GetHIPFileNodeIdsImpl = (GetHIPFileNodeIdsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetHIPFileNodeIds");
    HEMAX_HoudiniApi::GetHandleBindingInfoImpl = (GetHandleBindingInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetHandleBindingInfo");
    HEMAX_HoudiniApi::GetHandleInfoImpl = (GetHandleInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetHandleInfo");
    HEMAX_HoudiniApi::GetHeightFieldDataImpl = (GetHeightFieldDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetHeightFieldData");
    HEMAX_HoudiniApi::GetImageFilePathImpl = (GetImageFilePathFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetImageFilePath");
    HEMAX_HoudiniApi::GetImageInfoImpl = (GetImageInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetImageInfo");
    HEMAX_HoudiniApi::GetImageMemoryBufferImpl = (GetImageMemoryBufferFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetImageMemoryBuffer");
    HEMAX_HoudiniApi::GetImagePlaneCountImpl = (GetImagePlaneCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetImagePlaneCount");
    HEMAX_HoudiniApi::GetImagePlanesImpl = (GetImagePlanesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetImagePlanes");
    HEMAX_HoudiniApi::GetInputCurveInfoImpl = (GetInputCurveInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetInputCurveInfo");
    HEMAX_HoudiniApi::GetInstanceTransformsOnPartImpl = (GetInstanceTransformsOnPartFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetInstanceTransformsOnPart");
    HEMAX_HoudiniApi::GetInstancedObjectIdsImpl = (GetInstancedObjectIdsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetInstancedObjectIds");
    HEMAX_HoudiniApi::GetInstancedPartIdsImpl = (GetInstancedPartIdsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetInstancedPartIds");
    HEMAX_HoudiniApi::GetInstancerPartTransformsImpl = (GetInstancerPartTransformsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetInstancerPartTransforms");
    HEMAX_HoudiniApi::GetManagerNodeIdImpl = (GetManagerNodeIdFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetManagerNodeId");
    HEMAX_HoudiniApi::GetMaterialInfoImpl = (GetMaterialInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetMaterialInfo");
    HEMAX_HoudiniApi::GetMaterialNodeIdsOnFacesImpl = (GetMaterialNodeIdsOnFacesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetMaterialNodeIdsOnFaces");
    HEMAX_HoudiniApi::GetNextVolumeTileImpl = (GetNextVolumeTileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetNextVolumeTile");
    HEMAX_HoudiniApi::GetNodeFromPathImpl = (GetNodeFromPathFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetNodeFromPath");
    HEMAX_HoudiniApi::GetNodeInfoImpl = (GetNodeInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetNodeInfo");
    HEMAX_HoudiniApi::GetNodeInputNameImpl = (GetNodeInputNameFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetNodeInputName");
    HEMAX_HoudiniApi::GetNodeOutputNameImpl = (GetNodeOutputNameFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetNodeOutputName");
    HEMAX_HoudiniApi::GetNodePathImpl = (GetNodePathFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetNodePath");
    HEMAX_HoudiniApi::GetNumWorkItemsImpl = (GetNumWorkItemsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetNumWorkItems");
    HEMAX_HoudiniApi::GetNumWorkitemsImpl = (GetNumWorkitemsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetNumWorkitems");
    HEMAX_HoudiniApi::GetObjectInfoImpl = (GetObjectInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetObjectInfo");
    HEMAX_HoudiniApi::GetObjectTransformImpl = (GetObjectTransformFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetObjectTransform");
    HEMAX_HoudiniApi::GetOutputGeoCountImpl = (GetOutputGeoCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetOutputGeoCount");
    HEMAX_HoudiniApi::GetOutputGeoInfosImpl = (GetOutputGeoInfosFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetOutputGeoInfos");
    HEMAX_HoudiniApi::GetOutputNodeIdImpl = (GetOutputNodeIdFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetOutputNodeId");
    HEMAX_HoudiniApi::GetPDGEventsImpl = (GetPDGEventsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetPDGEvents");
    HEMAX_HoudiniApi::GetPDGGraphContextIdImpl = (GetPDGGraphContextIdFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetPDGGraphContextId");
    HEMAX_HoudiniApi::GetPDGGraphContextsImpl = (GetPDGGraphContextsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetPDGGraphContexts");
    HEMAX_HoudiniApi::GetPDGGraphContextsCountImpl = (GetPDGGraphContextsCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetPDGGraphContextsCount");
    HEMAX_HoudiniApi::GetPDGStateImpl = (GetPDGStateFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetPDGState");
    HEMAX_HoudiniApi::GetParametersImpl = (GetParametersFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParameters");
    HEMAX_HoudiniApi::GetParmChoiceListsImpl = (GetParmChoiceListsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmChoiceLists");
    HEMAX_HoudiniApi::GetParmExpressionImpl = (GetParmExpressionFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmExpression");
    HEMAX_HoudiniApi::GetParmFileImpl = (GetParmFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmFile");
    HEMAX_HoudiniApi::GetParmFloatValueImpl = (GetParmFloatValueFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmFloatValue");
    HEMAX_HoudiniApi::GetParmFloatValuesImpl = (GetParmFloatValuesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmFloatValues");
    HEMAX_HoudiniApi::GetParmIdFromNameImpl = (GetParmIdFromNameFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmIdFromName");
    HEMAX_HoudiniApi::GetParmInfoImpl = (GetParmInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmInfo");
    HEMAX_HoudiniApi::GetParmInfoFromNameImpl = (GetParmInfoFromNameFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmInfoFromName");
    HEMAX_HoudiniApi::GetParmIntValueImpl = (GetParmIntValueFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmIntValue");
    HEMAX_HoudiniApi::GetParmIntValuesImpl = (GetParmIntValuesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmIntValues");
    HEMAX_HoudiniApi::GetParmNodeValueImpl = (GetParmNodeValueFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmNodeValue");
    HEMAX_HoudiniApi::GetParmStringValueImpl = (GetParmStringValueFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmStringValue");
    HEMAX_HoudiniApi::GetParmStringValuesImpl = (GetParmStringValuesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmStringValues");
    HEMAX_HoudiniApi::GetParmTagNameImpl = (GetParmTagNameFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmTagName");
    HEMAX_HoudiniApi::GetParmTagValueImpl = (GetParmTagValueFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmTagValue");
    HEMAX_HoudiniApi::GetParmWithTagImpl = (GetParmWithTagFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetParmWithTag");
    HEMAX_HoudiniApi::GetPartInfoImpl = (GetPartInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetPartInfo");
    HEMAX_HoudiniApi::GetPresetImpl = (GetPresetFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetPreset");
    HEMAX_HoudiniApi::GetPresetBufLengthImpl = (GetPresetBufLengthFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetPresetBufLength");
    HEMAX_HoudiniApi::GetServerEnvIntImpl = (GetServerEnvIntFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetServerEnvInt");
    HEMAX_HoudiniApi::GetServerEnvStringImpl = (GetServerEnvStringFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetServerEnvString");
    HEMAX_HoudiniApi::GetServerEnvVarCountImpl = (GetServerEnvVarCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetServerEnvVarCount");
    HEMAX_HoudiniApi::GetServerEnvVarListImpl = (GetServerEnvVarListFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetServerEnvVarList");
    HEMAX_HoudiniApi::GetSessionEnvIntImpl = (GetSessionEnvIntFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetSessionEnvInt");
    HEMAX_HoudiniApi::GetSessionSyncInfoImpl = (GetSessionSyncInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetSessionSyncInfo");
    HEMAX_HoudiniApi::GetSphereInfoImpl = (GetSphereInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetSphereInfo");
    HEMAX_HoudiniApi::GetStatusImpl = (GetStatusFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetStatus");
    HEMAX_HoudiniApi::GetStatusStringImpl = (GetStatusStringFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetStatusString");
    HEMAX_HoudiniApi::GetStatusStringBufLengthImpl = (GetStatusStringBufLengthFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetStatusStringBufLength");
    HEMAX_HoudiniApi::GetStringImpl = (GetStringFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetString");
    HEMAX_HoudiniApi::GetStringBatchImpl = (GetStringBatchFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetStringBatch");
    HEMAX_HoudiniApi::GetStringBatchSizeImpl = (GetStringBatchSizeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetStringBatchSize");
    HEMAX_HoudiniApi::GetStringBufLengthImpl = (GetStringBufLengthFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetStringBufLength");
    HEMAX_HoudiniApi::GetSupportedImageFileFormatCountImpl = (GetSupportedImageFileFormatCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetSupportedImageFileFormatCount");
    HEMAX_HoudiniApi::GetSupportedImageFileFormatsImpl = (GetSupportedImageFileFormatsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetSupportedImageFileFormats");
    HEMAX_HoudiniApi::GetTimeImpl = (GetTimeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetTime");
    HEMAX_HoudiniApi::GetTimelineOptionsImpl = (GetTimelineOptionsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetTimelineOptions");
    HEMAX_HoudiniApi::GetTotalCookCountImpl = (GetTotalCookCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetTotalCookCount");
    HEMAX_HoudiniApi::GetUseHoudiniTimeImpl = (GetUseHoudiniTimeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetUseHoudiniTime");
    HEMAX_HoudiniApi::GetVertexListImpl = (GetVertexListFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetVertexList");
    HEMAX_HoudiniApi::GetViewportImpl = (GetViewportFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetViewport");
    HEMAX_HoudiniApi::GetVolumeBoundsImpl = (GetVolumeBoundsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetVolumeBounds");
    HEMAX_HoudiniApi::GetVolumeInfoImpl = (GetVolumeInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetVolumeInfo");
    HEMAX_HoudiniApi::GetVolumeTileFloatDataImpl = (GetVolumeTileFloatDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetVolumeTileFloatData");
    HEMAX_HoudiniApi::GetVolumeTileIntDataImpl = (GetVolumeTileIntDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetVolumeTileIntData");
    HEMAX_HoudiniApi::GetVolumeVisualInfoImpl = (GetVolumeVisualInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetVolumeVisualInfo");
    HEMAX_HoudiniApi::GetVolumeVoxelFloatDataImpl = (GetVolumeVoxelFloatDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetVolumeVoxelFloatData");
    HEMAX_HoudiniApi::GetVolumeVoxelIntDataImpl = (GetVolumeVoxelIntDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetVolumeVoxelIntData");
    HEMAX_HoudiniApi::GetWorkItemAttributeSizeImpl = (GetWorkItemAttributeSizeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkItemAttributeSize");
    HEMAX_HoudiniApi::GetWorkItemFloatAttributeImpl = (GetWorkItemFloatAttributeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkItemFloatAttribute");
    HEMAX_HoudiniApi::GetWorkItemInfoImpl = (GetWorkItemInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkItemInfo");
    HEMAX_HoudiniApi::GetWorkItemIntAttributeImpl = (GetWorkItemIntAttributeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkItemIntAttribute");
    HEMAX_HoudiniApi::GetWorkItemOutputFilesImpl = (GetWorkItemOutputFilesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkItemOutputFiles");
    HEMAX_HoudiniApi::GetWorkItemStringAttributeImpl = (GetWorkItemStringAttributeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkItemStringAttribute");
    HEMAX_HoudiniApi::GetWorkItemsImpl = (GetWorkItemsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkItems");
    HEMAX_HoudiniApi::GetWorkitemDataLengthImpl = (GetWorkitemDataLengthFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkitemDataLength");
    HEMAX_HoudiniApi::GetWorkitemFloatDataImpl = (GetWorkitemFloatDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkitemFloatData");
    HEMAX_HoudiniApi::GetWorkitemInfoImpl = (GetWorkitemInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkitemInfo");
    HEMAX_HoudiniApi::GetWorkitemIntDataImpl = (GetWorkitemIntDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkitemIntData");
    HEMAX_HoudiniApi::GetWorkitemResultInfoImpl = (GetWorkitemResultInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkitemResultInfo");
    HEMAX_HoudiniApi::GetWorkitemStringDataImpl = (GetWorkitemStringDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkitemStringData");
    HEMAX_HoudiniApi::GetWorkitemsImpl = (GetWorkitemsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_GetWorkitems");
    HEMAX_HoudiniApi::InitializeImpl = (InitializeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_Initialize");
    HEMAX_HoudiniApi::InsertMultiparmInstanceImpl = (InsertMultiparmInstanceFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_InsertMultiparmInstance");
    HEMAX_HoudiniApi::InterruptImpl = (InterruptFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_Interrupt");
    HEMAX_HoudiniApi::IsInitializedImpl = (IsInitializedFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_IsInitialized");
    HEMAX_HoudiniApi::IsNodeValidImpl = (IsNodeValidFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_IsNodeValid");
    HEMAX_HoudiniApi::IsSessionValidImpl = (IsSessionValidFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_IsSessionValid");
    HEMAX_HoudiniApi::LoadAssetLibraryFromFileImpl = (LoadAssetLibraryFromFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_LoadAssetLibraryFromFile");
    HEMAX_HoudiniApi::LoadAssetLibraryFromMemoryImpl = (LoadAssetLibraryFromMemoryFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_LoadAssetLibraryFromMemory");
    HEMAX_HoudiniApi::LoadGeoFromFileImpl = (LoadGeoFromFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_LoadGeoFromFile");
    HEMAX_HoudiniApi::LoadGeoFromMemoryImpl = (LoadGeoFromMemoryFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_LoadGeoFromMemory");
    HEMAX_HoudiniApi::LoadHIPFileImpl = (LoadHIPFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_LoadHIPFile");
    HEMAX_HoudiniApi::LoadNodeFromFileImpl = (LoadNodeFromFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_LoadNodeFromFile");
    HEMAX_HoudiniApi::MergeHIPFileImpl = (MergeHIPFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_MergeHIPFile");
    HEMAX_HoudiniApi::ParmHasExpressionImpl = (ParmHasExpressionFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ParmHasExpression");
    HEMAX_HoudiniApi::ParmHasTagImpl = (ParmHasTagFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ParmHasTag");
    HEMAX_HoudiniApi::PausePDGCookImpl = (PausePDGCookFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_PausePDGCook");
    HEMAX_HoudiniApi::PythonThreadInterpreterLockImpl = (PythonThreadInterpreterLockFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_PythonThreadInterpreterLock");
    HEMAX_HoudiniApi::QueryNodeInputImpl = (QueryNodeInputFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_QueryNodeInput");
    HEMAX_HoudiniApi::QueryNodeOutputConnectedCountImpl = (QueryNodeOutputConnectedCountFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_QueryNodeOutputConnectedCount");
    HEMAX_HoudiniApi::QueryNodeOutputConnectedNodesImpl = (QueryNodeOutputConnectedNodesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_QueryNodeOutputConnectedNodes");
    HEMAX_HoudiniApi::RemoveCustomStringImpl = (RemoveCustomStringFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_RemoveCustomString");
    HEMAX_HoudiniApi::RemoveMultiparmInstanceImpl = (RemoveMultiparmInstanceFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_RemoveMultiparmInstance");
    HEMAX_HoudiniApi::RemoveParmExpressionImpl = (RemoveParmExpressionFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_RemoveParmExpression");
    HEMAX_HoudiniApi::RenameNodeImpl = (RenameNodeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_RenameNode");
    HEMAX_HoudiniApi::RenderCOPToImageImpl = (RenderCOPToImageFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_RenderCOPToImage");
    HEMAX_HoudiniApi::RenderTextureToImageImpl = (RenderTextureToImageFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_RenderTextureToImage");
    HEMAX_HoudiniApi::ResetSimulationImpl = (ResetSimulationFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_ResetSimulation");
    HEMAX_HoudiniApi::RevertGeoImpl = (RevertGeoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_RevertGeo");
    HEMAX_HoudiniApi::RevertParmToDefaultImpl = (RevertParmToDefaultFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_RevertParmToDefault");
    HEMAX_HoudiniApi::RevertParmToDefaultsImpl = (RevertParmToDefaultsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_RevertParmToDefaults");
    HEMAX_HoudiniApi::SaveGeoToFileImpl = (SaveGeoToFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SaveGeoToFile");
    HEMAX_HoudiniApi::SaveGeoToMemoryImpl = (SaveGeoToMemoryFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SaveGeoToMemory");
    HEMAX_HoudiniApi::SaveHIPFileImpl = (SaveHIPFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SaveHIPFile");
    HEMAX_HoudiniApi::SaveNodeToFileImpl = (SaveNodeToFileFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SaveNodeToFile");
    HEMAX_HoudiniApi::SetAnimCurveImpl = (SetAnimCurveFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAnimCurve");
    HEMAX_HoudiniApi::SetAttributeFloat64ArrayDataImpl = (SetAttributeFloat64ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeFloat64ArrayData");
    HEMAX_HoudiniApi::SetAttributeFloat64DataImpl = (SetAttributeFloat64DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeFloat64Data");
    HEMAX_HoudiniApi::SetAttributeFloatArrayDataImpl = (SetAttributeFloatArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeFloatArrayData");
    HEMAX_HoudiniApi::SetAttributeFloatDataImpl = (SetAttributeFloatDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeFloatData");
    HEMAX_HoudiniApi::SetAttributeInt16ArrayDataImpl = (SetAttributeInt16ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeInt16ArrayData");
    HEMAX_HoudiniApi::SetAttributeInt16DataImpl = (SetAttributeInt16DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeInt16Data");
    HEMAX_HoudiniApi::SetAttributeInt64ArrayDataImpl = (SetAttributeInt64ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeInt64ArrayData");
    HEMAX_HoudiniApi::SetAttributeInt64DataImpl = (SetAttributeInt64DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeInt64Data");
    HEMAX_HoudiniApi::SetAttributeInt8ArrayDataImpl = (SetAttributeInt8ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeInt8ArrayData");
    HEMAX_HoudiniApi::SetAttributeInt8DataImpl = (SetAttributeInt8DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeInt8Data");
    HEMAX_HoudiniApi::SetAttributeIntArrayDataImpl = (SetAttributeIntArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeIntArrayData");
    HEMAX_HoudiniApi::SetAttributeIntDataImpl = (SetAttributeIntDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeIntData");
    HEMAX_HoudiniApi::SetAttributeStringArrayDataImpl = (SetAttributeStringArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeStringArrayData");
    HEMAX_HoudiniApi::SetAttributeStringDataImpl = (SetAttributeStringDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeStringData");
    HEMAX_HoudiniApi::SetAttributeUInt8ArrayDataImpl = (SetAttributeUInt8ArrayDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeUInt8ArrayData");
    HEMAX_HoudiniApi::SetAttributeUInt8DataImpl = (SetAttributeUInt8DataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetAttributeUInt8Data");
    HEMAX_HoudiniApi::SetCachePropertyImpl = (SetCachePropertyFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetCacheProperty");
    HEMAX_HoudiniApi::SetCompositorOptionsImpl = (SetCompositorOptionsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetCompositorOptions");
    HEMAX_HoudiniApi::SetCurveCountsImpl = (SetCurveCountsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetCurveCounts");
    HEMAX_HoudiniApi::SetCurveInfoImpl = (SetCurveInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetCurveInfo");
    HEMAX_HoudiniApi::SetCurveKnotsImpl = (SetCurveKnotsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetCurveKnots");
    HEMAX_HoudiniApi::SetCurveOrdersImpl = (SetCurveOrdersFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetCurveOrders");
    HEMAX_HoudiniApi::SetCustomStringImpl = (SetCustomStringFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetCustomString");
    HEMAX_HoudiniApi::SetFaceCountsImpl = (SetFaceCountsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetFaceCounts");
    HEMAX_HoudiniApi::SetGroupMembershipImpl = (SetGroupMembershipFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetGroupMembership");
    HEMAX_HoudiniApi::SetHeightFieldDataImpl = (SetHeightFieldDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetHeightFieldData");
    HEMAX_HoudiniApi::SetImageInfoImpl = (SetImageInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetImageInfo");
    HEMAX_HoudiniApi::SetInputCurveInfoImpl = (SetInputCurveInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetInputCurveInfo");
    HEMAX_HoudiniApi::SetInputCurvePositionsImpl = (SetInputCurvePositionsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetInputCurvePositions");
    HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScalesImpl = (SetInputCurvePositionsRotationsScalesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetInputCurvePositionsRotationsScales");
    HEMAX_HoudiniApi::SetNodeDisplayImpl = (SetNodeDisplayFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetNodeDisplay");
    HEMAX_HoudiniApi::SetObjectTransformImpl = (SetObjectTransformFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetObjectTransform");
    HEMAX_HoudiniApi::SetParmExpressionImpl = (SetParmExpressionFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetParmExpression");
    HEMAX_HoudiniApi::SetParmFloatValueImpl = (SetParmFloatValueFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetParmFloatValue");
    HEMAX_HoudiniApi::SetParmFloatValuesImpl = (SetParmFloatValuesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetParmFloatValues");
    HEMAX_HoudiniApi::SetParmIntValueImpl = (SetParmIntValueFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetParmIntValue");
    HEMAX_HoudiniApi::SetParmIntValuesImpl = (SetParmIntValuesFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetParmIntValues");
    HEMAX_HoudiniApi::SetParmNodeValueImpl = (SetParmNodeValueFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetParmNodeValue");
    HEMAX_HoudiniApi::SetParmStringValueImpl = (SetParmStringValueFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetParmStringValue");
    HEMAX_HoudiniApi::SetPartInfoImpl = (SetPartInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetPartInfo");
    HEMAX_HoudiniApi::SetPresetImpl = (SetPresetFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetPreset");
    HEMAX_HoudiniApi::SetServerEnvIntImpl = (SetServerEnvIntFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetServerEnvInt");
    HEMAX_HoudiniApi::SetServerEnvStringImpl = (SetServerEnvStringFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetServerEnvString");
    HEMAX_HoudiniApi::SetSessionSyncImpl = (SetSessionSyncFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetSessionSync");
    HEMAX_HoudiniApi::SetSessionSyncInfoImpl = (SetSessionSyncInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetSessionSyncInfo");
    HEMAX_HoudiniApi::SetTimeImpl = (SetTimeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetTime");
    HEMAX_HoudiniApi::SetTimelineOptionsImpl = (SetTimelineOptionsFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetTimelineOptions");
    HEMAX_HoudiniApi::SetTransformAnimCurveImpl = (SetTransformAnimCurveFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetTransformAnimCurve");
    HEMAX_HoudiniApi::SetUseHoudiniTimeImpl = (SetUseHoudiniTimeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetUseHoudiniTime");
    HEMAX_HoudiniApi::SetVertexListImpl = (SetVertexListFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetVertexList");
    HEMAX_HoudiniApi::SetViewportImpl = (SetViewportFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetViewport");
    HEMAX_HoudiniApi::SetVolumeInfoImpl = (SetVolumeInfoFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetVolumeInfo");
    HEMAX_HoudiniApi::SetVolumeTileFloatDataImpl = (SetVolumeTileFloatDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetVolumeTileFloatData");
    HEMAX_HoudiniApi::SetVolumeTileIntDataImpl = (SetVolumeTileIntDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetVolumeTileIntData");
    HEMAX_HoudiniApi::SetVolumeVoxelFloatDataImpl = (SetVolumeVoxelFloatDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetVolumeVoxelFloatData");
    HEMAX_HoudiniApi::SetVolumeVoxelIntDataImpl = (SetVolumeVoxelIntDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetVolumeVoxelIntData");
    HEMAX_HoudiniApi::SetWorkItemFloatAttributeImpl = (SetWorkItemFloatAttributeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetWorkItemFloatAttribute");
    HEMAX_HoudiniApi::SetWorkItemIntAttributeImpl = (SetWorkItemIntAttributeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetWorkItemIntAttribute");
    HEMAX_HoudiniApi::SetWorkItemStringAttributeImpl = (SetWorkItemStringAttributeFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetWorkItemStringAttribute");
    HEMAX_HoudiniApi::SetWorkitemFloatDataImpl = (SetWorkitemFloatDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetWorkitemFloatData");
    HEMAX_HoudiniApi::SetWorkitemIntDataImpl = (SetWorkitemIntDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetWorkitemIntData");
    HEMAX_HoudiniApi::SetWorkitemStringDataImpl = (SetWorkitemStringDataFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_SetWorkitemStringData");
    HEMAX_HoudiniApi::ShutdownImpl = (ShutdownFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_Shutdown");
    HEMAX_HoudiniApi::StartThriftNamedPipeServerImpl = (StartThriftNamedPipeServerFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_StartThriftNamedPipeServer");
    HEMAX_HoudiniApi::StartThriftSocketServerImpl = (StartThriftSocketServerFuncPtr) HEMAX_Platform::GetDllExport(LibraryHandle, "HAPI_StartThriftSocketServer");
}


void
HEMAX_HoudiniApi::FinalizeHAPI()
{
    HEMAX_HoudiniApi::AddAttributeImpl = &HEMAX_HoudiniApi::AddAttributeEmptyStub;
    HEMAX_HoudiniApi::AddGroupImpl = &HEMAX_HoudiniApi::AddGroupEmptyStub;
    HEMAX_HoudiniApi::BindCustomImplementationImpl = &HEMAX_HoudiniApi::BindCustomImplementationEmptyStub;
    HEMAX_HoudiniApi::CancelPDGCookImpl = &HEMAX_HoudiniApi::CancelPDGCookEmptyStub;
    HEMAX_HoudiniApi::CheckForSpecificErrorsImpl = &HEMAX_HoudiniApi::CheckForSpecificErrorsEmptyStub;
    HEMAX_HoudiniApi::CleanupImpl = &HEMAX_HoudiniApi::CleanupEmptyStub;
    HEMAX_HoudiniApi::ClearConnectionErrorImpl = &HEMAX_HoudiniApi::ClearConnectionErrorEmptyStub;
    HEMAX_HoudiniApi::CloseSessionImpl = &HEMAX_HoudiniApi::CloseSessionEmptyStub;
    HEMAX_HoudiniApi::CommitGeoImpl = &HEMAX_HoudiniApi::CommitGeoEmptyStub;
    HEMAX_HoudiniApi::CommitWorkItemsImpl = &HEMAX_HoudiniApi::CommitWorkItemsEmptyStub;
    HEMAX_HoudiniApi::CommitWorkitemsImpl = &HEMAX_HoudiniApi::CommitWorkitemsEmptyStub;
    HEMAX_HoudiniApi::ComposeChildNodeListImpl = &HEMAX_HoudiniApi::ComposeChildNodeListEmptyStub;
    HEMAX_HoudiniApi::ComposeNodeCookResultImpl = &HEMAX_HoudiniApi::ComposeNodeCookResultEmptyStub;
    HEMAX_HoudiniApi::ComposeObjectListImpl = &HEMAX_HoudiniApi::ComposeObjectListEmptyStub;
    HEMAX_HoudiniApi::ConnectNodeInputImpl = &HEMAX_HoudiniApi::ConnectNodeInputEmptyStub;
    HEMAX_HoudiniApi::ConvertMatrixToEulerImpl = &HEMAX_HoudiniApi::ConvertMatrixToEulerEmptyStub;
    HEMAX_HoudiniApi::ConvertMatrixToQuatImpl = &HEMAX_HoudiniApi::ConvertMatrixToQuatEmptyStub;
    HEMAX_HoudiniApi::ConvertTransformImpl = &HEMAX_HoudiniApi::ConvertTransformEmptyStub;
    HEMAX_HoudiniApi::ConvertTransformEulerToMatrixImpl = &HEMAX_HoudiniApi::ConvertTransformEulerToMatrixEmptyStub;
    HEMAX_HoudiniApi::ConvertTransformQuatToMatrixImpl = &HEMAX_HoudiniApi::ConvertTransformQuatToMatrixEmptyStub;
    HEMAX_HoudiniApi::CookNodeImpl = &HEMAX_HoudiniApi::CookNodeEmptyStub;
    HEMAX_HoudiniApi::CookPDGImpl = &HEMAX_HoudiniApi::CookPDGEmptyStub;
    HEMAX_HoudiniApi::CookPDGAllOutputsImpl = &HEMAX_HoudiniApi::CookPDGAllOutputsEmptyStub;
    HEMAX_HoudiniApi::CreateCustomSessionImpl = &HEMAX_HoudiniApi::CreateCustomSessionEmptyStub;
    HEMAX_HoudiniApi::CreateHeightFieldInputImpl = &HEMAX_HoudiniApi::CreateHeightFieldInputEmptyStub;
    HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNodeImpl = &HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNodeEmptyStub;
    HEMAX_HoudiniApi::CreateInProcessSessionImpl = &HEMAX_HoudiniApi::CreateInProcessSessionEmptyStub;
    HEMAX_HoudiniApi::CreateInputCurveNodeImpl = &HEMAX_HoudiniApi::CreateInputCurveNodeEmptyStub;
    HEMAX_HoudiniApi::CreateInputNodeImpl = &HEMAX_HoudiniApi::CreateInputNodeEmptyStub;
    HEMAX_HoudiniApi::CreateNodeImpl = &HEMAX_HoudiniApi::CreateNodeEmptyStub;
    HEMAX_HoudiniApi::CreateThriftNamedPipeSessionImpl = &HEMAX_HoudiniApi::CreateThriftNamedPipeSessionEmptyStub;
    HEMAX_HoudiniApi::CreateThriftSocketSessionImpl = &HEMAX_HoudiniApi::CreateThriftSocketSessionEmptyStub;
    HEMAX_HoudiniApi::CreateWorkItemImpl = &HEMAX_HoudiniApi::CreateWorkItemEmptyStub;
    HEMAX_HoudiniApi::CreateWorkitemImpl = &HEMAX_HoudiniApi::CreateWorkitemEmptyStub;
    HEMAX_HoudiniApi::DeleteAttributeImpl = &HEMAX_HoudiniApi::DeleteAttributeEmptyStub;
    HEMAX_HoudiniApi::DeleteGroupImpl = &HEMAX_HoudiniApi::DeleteGroupEmptyStub;
    HEMAX_HoudiniApi::DeleteNodeImpl = &HEMAX_HoudiniApi::DeleteNodeEmptyStub;
    HEMAX_HoudiniApi::DirtyPDGNodeImpl = &HEMAX_HoudiniApi::DirtyPDGNodeEmptyStub;
    HEMAX_HoudiniApi::DisconnectNodeInputImpl = &HEMAX_HoudiniApi::DisconnectNodeInputEmptyStub;
    HEMAX_HoudiniApi::DisconnectNodeOutputsAtImpl = &HEMAX_HoudiniApi::DisconnectNodeOutputsAtEmptyStub;
    HEMAX_HoudiniApi::ExtractImageToFileImpl = &HEMAX_HoudiniApi::ExtractImageToFileEmptyStub;
    HEMAX_HoudiniApi::ExtractImageToMemoryImpl = &HEMAX_HoudiniApi::ExtractImageToMemoryEmptyStub;
    HEMAX_HoudiniApi::GetActiveCacheCountImpl = &HEMAX_HoudiniApi::GetActiveCacheCountEmptyStub;
    HEMAX_HoudiniApi::GetActiveCacheNamesImpl = &HEMAX_HoudiniApi::GetActiveCacheNamesEmptyStub;
    HEMAX_HoudiniApi::GetAssetDefinitionParmCountsImpl = &HEMAX_HoudiniApi::GetAssetDefinitionParmCountsEmptyStub;
    HEMAX_HoudiniApi::GetAssetDefinitionParmInfosImpl = &HEMAX_HoudiniApi::GetAssetDefinitionParmInfosEmptyStub;
    HEMAX_HoudiniApi::GetAssetDefinitionParmValuesImpl = &HEMAX_HoudiniApi::GetAssetDefinitionParmValuesEmptyStub;
    HEMAX_HoudiniApi::GetAssetInfoImpl = &HEMAX_HoudiniApi::GetAssetInfoEmptyStub;
    HEMAX_HoudiniApi::GetAttributeFloat64ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeFloat64ArrayDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeFloat64DataImpl = &HEMAX_HoudiniApi::GetAttributeFloat64DataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeFloatArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeFloatArrayDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeFloatDataImpl = &HEMAX_HoudiniApi::GetAttributeFloatDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeInfoImpl = &HEMAX_HoudiniApi::GetAttributeInfoEmptyStub;
    HEMAX_HoudiniApi::GetAttributeInt16ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeInt16ArrayDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeInt16DataImpl = &HEMAX_HoudiniApi::GetAttributeInt16DataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeInt64ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeInt64ArrayDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeInt64DataImpl = &HEMAX_HoudiniApi::GetAttributeInt64DataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeInt8ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeInt8ArrayDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeInt8DataImpl = &HEMAX_HoudiniApi::GetAttributeInt8DataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeIntArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeIntArrayDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeIntDataImpl = &HEMAX_HoudiniApi::GetAttributeIntDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeNamesImpl = &HEMAX_HoudiniApi::GetAttributeNamesEmptyStub;
    HEMAX_HoudiniApi::GetAttributeStringArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeStringArrayDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeStringDataImpl = &HEMAX_HoudiniApi::GetAttributeStringDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeUInt8ArrayDataImpl = &HEMAX_HoudiniApi::GetAttributeUInt8ArrayDataEmptyStub;
    HEMAX_HoudiniApi::GetAttributeUInt8DataImpl = &HEMAX_HoudiniApi::GetAttributeUInt8DataEmptyStub;
    HEMAX_HoudiniApi::GetAvailableAssetCountImpl = &HEMAX_HoudiniApi::GetAvailableAssetCountEmptyStub;
    HEMAX_HoudiniApi::GetAvailableAssetsImpl = &HEMAX_HoudiniApi::GetAvailableAssetsEmptyStub;
    HEMAX_HoudiniApi::GetBoxInfoImpl = &HEMAX_HoudiniApi::GetBoxInfoEmptyStub;
    HEMAX_HoudiniApi::GetCachePropertyImpl = &HEMAX_HoudiniApi::GetCachePropertyEmptyStub;
    HEMAX_HoudiniApi::GetComposedChildNodeListImpl = &HEMAX_HoudiniApi::GetComposedChildNodeListEmptyStub;
    HEMAX_HoudiniApi::GetComposedNodeCookResultImpl = &HEMAX_HoudiniApi::GetComposedNodeCookResultEmptyStub;
    HEMAX_HoudiniApi::GetComposedObjectListImpl = &HEMAX_HoudiniApi::GetComposedObjectListEmptyStub;
    HEMAX_HoudiniApi::GetComposedObjectTransformsImpl = &HEMAX_HoudiniApi::GetComposedObjectTransformsEmptyStub;
    HEMAX_HoudiniApi::GetCompositorOptionsImpl = &HEMAX_HoudiniApi::GetCompositorOptionsEmptyStub;
    HEMAX_HoudiniApi::GetConnectionErrorImpl = &HEMAX_HoudiniApi::GetConnectionErrorEmptyStub;
    HEMAX_HoudiniApi::GetConnectionErrorLengthImpl = &HEMAX_HoudiniApi::GetConnectionErrorLengthEmptyStub;
    HEMAX_HoudiniApi::GetCookingCurrentCountImpl = &HEMAX_HoudiniApi::GetCookingCurrentCountEmptyStub;
    HEMAX_HoudiniApi::GetCookingTotalCountImpl = &HEMAX_HoudiniApi::GetCookingTotalCountEmptyStub;
    HEMAX_HoudiniApi::GetCurveCountsImpl = &HEMAX_HoudiniApi::GetCurveCountsEmptyStub;
    HEMAX_HoudiniApi::GetCurveInfoImpl = &HEMAX_HoudiniApi::GetCurveInfoEmptyStub;
    HEMAX_HoudiniApi::GetCurveKnotsImpl = &HEMAX_HoudiniApi::GetCurveKnotsEmptyStub;
    HEMAX_HoudiniApi::GetCurveOrdersImpl = &HEMAX_HoudiniApi::GetCurveOrdersEmptyStub;
    HEMAX_HoudiniApi::GetDisplayGeoInfoImpl = &HEMAX_HoudiniApi::GetDisplayGeoInfoEmptyStub;
    HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroupImpl = &HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroupEmptyStub;
    HEMAX_HoudiniApi::GetEnvIntImpl = &HEMAX_HoudiniApi::GetEnvIntEmptyStub;
    HEMAX_HoudiniApi::GetFaceCountsImpl = &HEMAX_HoudiniApi::GetFaceCountsEmptyStub;
    HEMAX_HoudiniApi::GetFirstVolumeTileImpl = &HEMAX_HoudiniApi::GetFirstVolumeTileEmptyStub;
    HEMAX_HoudiniApi::GetGeoInfoImpl = &HEMAX_HoudiniApi::GetGeoInfoEmptyStub;
    HEMAX_HoudiniApi::GetGeoSizeImpl = &HEMAX_HoudiniApi::GetGeoSizeEmptyStub;
    HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePartImpl = &HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePartEmptyStub;
    HEMAX_HoudiniApi::GetGroupMembershipImpl = &HEMAX_HoudiniApi::GetGroupMembershipEmptyStub;
    HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePartImpl = &HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePartEmptyStub;
    HEMAX_HoudiniApi::GetGroupNamesImpl = &HEMAX_HoudiniApi::GetGroupNamesEmptyStub;
    HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePartImpl = &HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePartEmptyStub;
    HEMAX_HoudiniApi::GetHIPFileNodeCountImpl = &HEMAX_HoudiniApi::GetHIPFileNodeCountEmptyStub;
    HEMAX_HoudiniApi::GetHIPFileNodeIdsImpl = &HEMAX_HoudiniApi::GetHIPFileNodeIdsEmptyStub;
    HEMAX_HoudiniApi::GetHandleBindingInfoImpl = &HEMAX_HoudiniApi::GetHandleBindingInfoEmptyStub;
    HEMAX_HoudiniApi::GetHandleInfoImpl = &HEMAX_HoudiniApi::GetHandleInfoEmptyStub;
    HEMAX_HoudiniApi::GetHeightFieldDataImpl = &HEMAX_HoudiniApi::GetHeightFieldDataEmptyStub;
    HEMAX_HoudiniApi::GetImageFilePathImpl = &HEMAX_HoudiniApi::GetImageFilePathEmptyStub;
    HEMAX_HoudiniApi::GetImageInfoImpl = &HEMAX_HoudiniApi::GetImageInfoEmptyStub;
    HEMAX_HoudiniApi::GetImageMemoryBufferImpl = &HEMAX_HoudiniApi::GetImageMemoryBufferEmptyStub;
    HEMAX_HoudiniApi::GetImagePlaneCountImpl = &HEMAX_HoudiniApi::GetImagePlaneCountEmptyStub;
    HEMAX_HoudiniApi::GetImagePlanesImpl = &HEMAX_HoudiniApi::GetImagePlanesEmptyStub;
    HEMAX_HoudiniApi::GetInputCurveInfoImpl = &HEMAX_HoudiniApi::GetInputCurveInfoEmptyStub;
    HEMAX_HoudiniApi::GetInstanceTransformsOnPartImpl = &HEMAX_HoudiniApi::GetInstanceTransformsOnPartEmptyStub;
    HEMAX_HoudiniApi::GetInstancedObjectIdsImpl = &HEMAX_HoudiniApi::GetInstancedObjectIdsEmptyStub;
    HEMAX_HoudiniApi::GetInstancedPartIdsImpl = &HEMAX_HoudiniApi::GetInstancedPartIdsEmptyStub;
    HEMAX_HoudiniApi::GetInstancerPartTransformsImpl = &HEMAX_HoudiniApi::GetInstancerPartTransformsEmptyStub;
    HEMAX_HoudiniApi::GetManagerNodeIdImpl = &HEMAX_HoudiniApi::GetManagerNodeIdEmptyStub;
    HEMAX_HoudiniApi::GetMaterialInfoImpl = &HEMAX_HoudiniApi::GetMaterialInfoEmptyStub;
    HEMAX_HoudiniApi::GetMaterialNodeIdsOnFacesImpl = &HEMAX_HoudiniApi::GetMaterialNodeIdsOnFacesEmptyStub;
    HEMAX_HoudiniApi::GetNextVolumeTileImpl = &HEMAX_HoudiniApi::GetNextVolumeTileEmptyStub;
    HEMAX_HoudiniApi::GetNodeFromPathImpl = &HEMAX_HoudiniApi::GetNodeFromPathEmptyStub;
    HEMAX_HoudiniApi::GetNodeInfoImpl = &HEMAX_HoudiniApi::GetNodeInfoEmptyStub;
    HEMAX_HoudiniApi::GetNodeInputNameImpl = &HEMAX_HoudiniApi::GetNodeInputNameEmptyStub;
    HEMAX_HoudiniApi::GetNodeOutputNameImpl = &HEMAX_HoudiniApi::GetNodeOutputNameEmptyStub;
    HEMAX_HoudiniApi::GetNodePathImpl = &HEMAX_HoudiniApi::GetNodePathEmptyStub;
    HEMAX_HoudiniApi::GetNumWorkItemsImpl = &HEMAX_HoudiniApi::GetNumWorkItemsEmptyStub;
    HEMAX_HoudiniApi::GetNumWorkitemsImpl = &HEMAX_HoudiniApi::GetNumWorkitemsEmptyStub;
    HEMAX_HoudiniApi::GetObjectInfoImpl = &HEMAX_HoudiniApi::GetObjectInfoEmptyStub;
    HEMAX_HoudiniApi::GetObjectTransformImpl = &HEMAX_HoudiniApi::GetObjectTransformEmptyStub;
    HEMAX_HoudiniApi::GetOutputGeoCountImpl = &HEMAX_HoudiniApi::GetOutputGeoCountEmptyStub;
    HEMAX_HoudiniApi::GetOutputGeoInfosImpl = &HEMAX_HoudiniApi::GetOutputGeoInfosEmptyStub;
    HEMAX_HoudiniApi::GetOutputNodeIdImpl = &HEMAX_HoudiniApi::GetOutputNodeIdEmptyStub;
    HEMAX_HoudiniApi::GetPDGEventsImpl = &HEMAX_HoudiniApi::GetPDGEventsEmptyStub;
    HEMAX_HoudiniApi::GetPDGGraphContextIdImpl = &HEMAX_HoudiniApi::GetPDGGraphContextIdEmptyStub;
    HEMAX_HoudiniApi::GetPDGGraphContextsImpl = &HEMAX_HoudiniApi::GetPDGGraphContextsEmptyStub;
    HEMAX_HoudiniApi::GetPDGGraphContextsCountImpl = &HEMAX_HoudiniApi::GetPDGGraphContextsCountEmptyStub;
    HEMAX_HoudiniApi::GetPDGStateImpl = &HEMAX_HoudiniApi::GetPDGStateEmptyStub;
    HEMAX_HoudiniApi::GetParametersImpl = &HEMAX_HoudiniApi::GetParametersEmptyStub;
    HEMAX_HoudiniApi::GetParmChoiceListsImpl = &HEMAX_HoudiniApi::GetParmChoiceListsEmptyStub;
    HEMAX_HoudiniApi::GetParmExpressionImpl = &HEMAX_HoudiniApi::GetParmExpressionEmptyStub;
    HEMAX_HoudiniApi::GetParmFileImpl = &HEMAX_HoudiniApi::GetParmFileEmptyStub;
    HEMAX_HoudiniApi::GetParmFloatValueImpl = &HEMAX_HoudiniApi::GetParmFloatValueEmptyStub;
    HEMAX_HoudiniApi::GetParmFloatValuesImpl = &HEMAX_HoudiniApi::GetParmFloatValuesEmptyStub;
    HEMAX_HoudiniApi::GetParmIdFromNameImpl = &HEMAX_HoudiniApi::GetParmIdFromNameEmptyStub;
    HEMAX_HoudiniApi::GetParmInfoImpl = &HEMAX_HoudiniApi::GetParmInfoEmptyStub;
    HEMAX_HoudiniApi::GetParmInfoFromNameImpl = &HEMAX_HoudiniApi::GetParmInfoFromNameEmptyStub;
    HEMAX_HoudiniApi::GetParmIntValueImpl = &HEMAX_HoudiniApi::GetParmIntValueEmptyStub;
    HEMAX_HoudiniApi::GetParmIntValuesImpl = &HEMAX_HoudiniApi::GetParmIntValuesEmptyStub;
    HEMAX_HoudiniApi::GetParmNodeValueImpl = &HEMAX_HoudiniApi::GetParmNodeValueEmptyStub;
    HEMAX_HoudiniApi::GetParmStringValueImpl = &HEMAX_HoudiniApi::GetParmStringValueEmptyStub;
    HEMAX_HoudiniApi::GetParmStringValuesImpl = &HEMAX_HoudiniApi::GetParmStringValuesEmptyStub;
    HEMAX_HoudiniApi::GetParmTagNameImpl = &HEMAX_HoudiniApi::GetParmTagNameEmptyStub;
    HEMAX_HoudiniApi::GetParmTagValueImpl = &HEMAX_HoudiniApi::GetParmTagValueEmptyStub;
    HEMAX_HoudiniApi::GetParmWithTagImpl = &HEMAX_HoudiniApi::GetParmWithTagEmptyStub;
    HEMAX_HoudiniApi::GetPartInfoImpl = &HEMAX_HoudiniApi::GetPartInfoEmptyStub;
    HEMAX_HoudiniApi::GetPresetImpl = &HEMAX_HoudiniApi::GetPresetEmptyStub;
    HEMAX_HoudiniApi::GetPresetBufLengthImpl = &HEMAX_HoudiniApi::GetPresetBufLengthEmptyStub;
    HEMAX_HoudiniApi::GetServerEnvIntImpl = &HEMAX_HoudiniApi::GetServerEnvIntEmptyStub;
    HEMAX_HoudiniApi::GetServerEnvStringImpl = &HEMAX_HoudiniApi::GetServerEnvStringEmptyStub;
    HEMAX_HoudiniApi::GetServerEnvVarCountImpl = &HEMAX_HoudiniApi::GetServerEnvVarCountEmptyStub;
    HEMAX_HoudiniApi::GetServerEnvVarListImpl = &HEMAX_HoudiniApi::GetServerEnvVarListEmptyStub;
    HEMAX_HoudiniApi::GetSessionEnvIntImpl = &HEMAX_HoudiniApi::GetSessionEnvIntEmptyStub;
    HEMAX_HoudiniApi::GetSessionSyncInfoImpl = &HEMAX_HoudiniApi::GetSessionSyncInfoEmptyStub;
    HEMAX_HoudiniApi::GetSphereInfoImpl = &HEMAX_HoudiniApi::GetSphereInfoEmptyStub;
    HEMAX_HoudiniApi::GetStatusImpl = &HEMAX_HoudiniApi::GetStatusEmptyStub;
    HEMAX_HoudiniApi::GetStatusStringImpl = &HEMAX_HoudiniApi::GetStatusStringEmptyStub;
    HEMAX_HoudiniApi::GetStatusStringBufLengthImpl = &HEMAX_HoudiniApi::GetStatusStringBufLengthEmptyStub;
    HEMAX_HoudiniApi::GetStringImpl = &HEMAX_HoudiniApi::GetStringEmptyStub;
    HEMAX_HoudiniApi::GetStringBatchImpl = &HEMAX_HoudiniApi::GetStringBatchEmptyStub;
    HEMAX_HoudiniApi::GetStringBatchSizeImpl = &HEMAX_HoudiniApi::GetStringBatchSizeEmptyStub;
    HEMAX_HoudiniApi::GetStringBufLengthImpl = &HEMAX_HoudiniApi::GetStringBufLengthEmptyStub;
    HEMAX_HoudiniApi::GetSupportedImageFileFormatCountImpl = &HEMAX_HoudiniApi::GetSupportedImageFileFormatCountEmptyStub;
    HEMAX_HoudiniApi::GetSupportedImageFileFormatsImpl = &HEMAX_HoudiniApi::GetSupportedImageFileFormatsEmptyStub;
    HEMAX_HoudiniApi::GetTimeImpl = &HEMAX_HoudiniApi::GetTimeEmptyStub;
    HEMAX_HoudiniApi::GetTimelineOptionsImpl = &HEMAX_HoudiniApi::GetTimelineOptionsEmptyStub;
    HEMAX_HoudiniApi::GetTotalCookCountImpl = &HEMAX_HoudiniApi::GetTotalCookCountEmptyStub;
    HEMAX_HoudiniApi::GetUseHoudiniTimeImpl = &HEMAX_HoudiniApi::GetUseHoudiniTimeEmptyStub;
    HEMAX_HoudiniApi::GetVertexListImpl = &HEMAX_HoudiniApi::GetVertexListEmptyStub;
    HEMAX_HoudiniApi::GetViewportImpl = &HEMAX_HoudiniApi::GetViewportEmptyStub;
    HEMAX_HoudiniApi::GetVolumeBoundsImpl = &HEMAX_HoudiniApi::GetVolumeBoundsEmptyStub;
    HEMAX_HoudiniApi::GetVolumeInfoImpl = &HEMAX_HoudiniApi::GetVolumeInfoEmptyStub;
    HEMAX_HoudiniApi::GetVolumeTileFloatDataImpl = &HEMAX_HoudiniApi::GetVolumeTileFloatDataEmptyStub;
    HEMAX_HoudiniApi::GetVolumeTileIntDataImpl = &HEMAX_HoudiniApi::GetVolumeTileIntDataEmptyStub;
    HEMAX_HoudiniApi::GetVolumeVisualInfoImpl = &HEMAX_HoudiniApi::GetVolumeVisualInfoEmptyStub;
    HEMAX_HoudiniApi::GetVolumeVoxelFloatDataImpl = &HEMAX_HoudiniApi::GetVolumeVoxelFloatDataEmptyStub;
    HEMAX_HoudiniApi::GetVolumeVoxelIntDataImpl = &HEMAX_HoudiniApi::GetVolumeVoxelIntDataEmptyStub;
    HEMAX_HoudiniApi::GetWorkItemAttributeSizeImpl = &HEMAX_HoudiniApi::GetWorkItemAttributeSizeEmptyStub;
    HEMAX_HoudiniApi::GetWorkItemFloatAttributeImpl = &HEMAX_HoudiniApi::GetWorkItemFloatAttributeEmptyStub;
    HEMAX_HoudiniApi::GetWorkItemInfoImpl = &HEMAX_HoudiniApi::GetWorkItemInfoEmptyStub;
    HEMAX_HoudiniApi::GetWorkItemIntAttributeImpl = &HEMAX_HoudiniApi::GetWorkItemIntAttributeEmptyStub;
    HEMAX_HoudiniApi::GetWorkItemOutputFilesImpl = &HEMAX_HoudiniApi::GetWorkItemOutputFilesEmptyStub;
    HEMAX_HoudiniApi::GetWorkItemStringAttributeImpl = &HEMAX_HoudiniApi::GetWorkItemStringAttributeEmptyStub;
    HEMAX_HoudiniApi::GetWorkItemsImpl = &HEMAX_HoudiniApi::GetWorkItemsEmptyStub;
    HEMAX_HoudiniApi::GetWorkitemDataLengthImpl = &HEMAX_HoudiniApi::GetWorkitemDataLengthEmptyStub;
    HEMAX_HoudiniApi::GetWorkitemFloatDataImpl = &HEMAX_HoudiniApi::GetWorkitemFloatDataEmptyStub;
    HEMAX_HoudiniApi::GetWorkitemInfoImpl = &HEMAX_HoudiniApi::GetWorkitemInfoEmptyStub;
    HEMAX_HoudiniApi::GetWorkitemIntDataImpl = &HEMAX_HoudiniApi::GetWorkitemIntDataEmptyStub;
    HEMAX_HoudiniApi::GetWorkitemResultInfoImpl = &HEMAX_HoudiniApi::GetWorkitemResultInfoEmptyStub;
    HEMAX_HoudiniApi::GetWorkitemStringDataImpl = &HEMAX_HoudiniApi::GetWorkitemStringDataEmptyStub;
    HEMAX_HoudiniApi::GetWorkitemsImpl = &HEMAX_HoudiniApi::GetWorkitemsEmptyStub;
    HEMAX_HoudiniApi::InitializeImpl = &HEMAX_HoudiniApi::InitializeEmptyStub;
    HEMAX_HoudiniApi::InsertMultiparmInstanceImpl = &HEMAX_HoudiniApi::InsertMultiparmInstanceEmptyStub;
    HEMAX_HoudiniApi::InterruptImpl = &HEMAX_HoudiniApi::InterruptEmptyStub;
    HEMAX_HoudiniApi::IsInitializedImpl = &HEMAX_HoudiniApi::IsInitializedEmptyStub;
    HEMAX_HoudiniApi::IsNodeValidImpl = &HEMAX_HoudiniApi::IsNodeValidEmptyStub;
    HEMAX_HoudiniApi::IsSessionValidImpl = &HEMAX_HoudiniApi::IsSessionValidEmptyStub;
    HEMAX_HoudiniApi::LoadAssetLibraryFromFileImpl = &HEMAX_HoudiniApi::LoadAssetLibraryFromFileEmptyStub;
    HEMAX_HoudiniApi::LoadAssetLibraryFromMemoryImpl = &HEMAX_HoudiniApi::LoadAssetLibraryFromMemoryEmptyStub;
    HEMAX_HoudiniApi::LoadGeoFromFileImpl = &HEMAX_HoudiniApi::LoadGeoFromFileEmptyStub;
    HEMAX_HoudiniApi::LoadGeoFromMemoryImpl = &HEMAX_HoudiniApi::LoadGeoFromMemoryEmptyStub;
    HEMAX_HoudiniApi::LoadHIPFileImpl = &HEMAX_HoudiniApi::LoadHIPFileEmptyStub;
    HEMAX_HoudiniApi::LoadNodeFromFileImpl = &HEMAX_HoudiniApi::LoadNodeFromFileEmptyStub;
    HEMAX_HoudiniApi::MergeHIPFileImpl = &HEMAX_HoudiniApi::MergeHIPFileEmptyStub;
    HEMAX_HoudiniApi::ParmHasExpressionImpl = &HEMAX_HoudiniApi::ParmHasExpressionEmptyStub;
    HEMAX_HoudiniApi::ParmHasTagImpl = &HEMAX_HoudiniApi::ParmHasTagEmptyStub;
    HEMAX_HoudiniApi::PausePDGCookImpl = &HEMAX_HoudiniApi::PausePDGCookEmptyStub;
    HEMAX_HoudiniApi::PythonThreadInterpreterLockImpl = &HEMAX_HoudiniApi::PythonThreadInterpreterLockEmptyStub;
    HEMAX_HoudiniApi::QueryNodeInputImpl = &HEMAX_HoudiniApi::QueryNodeInputEmptyStub;
    HEMAX_HoudiniApi::QueryNodeOutputConnectedCountImpl = &HEMAX_HoudiniApi::QueryNodeOutputConnectedCountEmptyStub;
    HEMAX_HoudiniApi::QueryNodeOutputConnectedNodesImpl = &HEMAX_HoudiniApi::QueryNodeOutputConnectedNodesEmptyStub;
    HEMAX_HoudiniApi::RemoveCustomStringImpl = &HEMAX_HoudiniApi::RemoveCustomStringEmptyStub;
    HEMAX_HoudiniApi::RemoveMultiparmInstanceImpl = &HEMAX_HoudiniApi::RemoveMultiparmInstanceEmptyStub;
    HEMAX_HoudiniApi::RemoveParmExpressionImpl = &HEMAX_HoudiniApi::RemoveParmExpressionEmptyStub;
    HEMAX_HoudiniApi::RenameNodeImpl = &HEMAX_HoudiniApi::RenameNodeEmptyStub;
    HEMAX_HoudiniApi::RenderCOPToImageImpl = &HEMAX_HoudiniApi::RenderCOPToImageEmptyStub;
    HEMAX_HoudiniApi::RenderTextureToImageImpl = &HEMAX_HoudiniApi::RenderTextureToImageEmptyStub;
    HEMAX_HoudiniApi::ResetSimulationImpl = &HEMAX_HoudiniApi::ResetSimulationEmptyStub;
    HEMAX_HoudiniApi::RevertGeoImpl = &HEMAX_HoudiniApi::RevertGeoEmptyStub;
    HEMAX_HoudiniApi::RevertParmToDefaultImpl = &HEMAX_HoudiniApi::RevertParmToDefaultEmptyStub;
    HEMAX_HoudiniApi::RevertParmToDefaultsImpl = &HEMAX_HoudiniApi::RevertParmToDefaultsEmptyStub;
    HEMAX_HoudiniApi::SaveGeoToFileImpl = &HEMAX_HoudiniApi::SaveGeoToFileEmptyStub;
    HEMAX_HoudiniApi::SaveGeoToMemoryImpl = &HEMAX_HoudiniApi::SaveGeoToMemoryEmptyStub;
    HEMAX_HoudiniApi::SaveHIPFileImpl = &HEMAX_HoudiniApi::SaveHIPFileEmptyStub;
    HEMAX_HoudiniApi::SaveNodeToFileImpl = &HEMAX_HoudiniApi::SaveNodeToFileEmptyStub;
    HEMAX_HoudiniApi::SetAnimCurveImpl = &HEMAX_HoudiniApi::SetAnimCurveEmptyStub;
    HEMAX_HoudiniApi::SetAttributeFloat64ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeFloat64ArrayDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeFloat64DataImpl = &HEMAX_HoudiniApi::SetAttributeFloat64DataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeFloatArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeFloatArrayDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeFloatDataImpl = &HEMAX_HoudiniApi::SetAttributeFloatDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeInt16ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeInt16ArrayDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeInt16DataImpl = &HEMAX_HoudiniApi::SetAttributeInt16DataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeInt64ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeInt64ArrayDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeInt64DataImpl = &HEMAX_HoudiniApi::SetAttributeInt64DataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeInt8ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeInt8ArrayDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeInt8DataImpl = &HEMAX_HoudiniApi::SetAttributeInt8DataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeIntArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeIntArrayDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeIntDataImpl = &HEMAX_HoudiniApi::SetAttributeIntDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeStringArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeStringArrayDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeStringDataImpl = &HEMAX_HoudiniApi::SetAttributeStringDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeUInt8ArrayDataImpl = &HEMAX_HoudiniApi::SetAttributeUInt8ArrayDataEmptyStub;
    HEMAX_HoudiniApi::SetAttributeUInt8DataImpl = &HEMAX_HoudiniApi::SetAttributeUInt8DataEmptyStub;
    HEMAX_HoudiniApi::SetCachePropertyImpl = &HEMAX_HoudiniApi::SetCachePropertyEmptyStub;
    HEMAX_HoudiniApi::SetCompositorOptionsImpl = &HEMAX_HoudiniApi::SetCompositorOptionsEmptyStub;
    HEMAX_HoudiniApi::SetCurveCountsImpl = &HEMAX_HoudiniApi::SetCurveCountsEmptyStub;
    HEMAX_HoudiniApi::SetCurveInfoImpl = &HEMAX_HoudiniApi::SetCurveInfoEmptyStub;
    HEMAX_HoudiniApi::SetCurveKnotsImpl = &HEMAX_HoudiniApi::SetCurveKnotsEmptyStub;
    HEMAX_HoudiniApi::SetCurveOrdersImpl = &HEMAX_HoudiniApi::SetCurveOrdersEmptyStub;
    HEMAX_HoudiniApi::SetCustomStringImpl = &HEMAX_HoudiniApi::SetCustomStringEmptyStub;
    HEMAX_HoudiniApi::SetFaceCountsImpl = &HEMAX_HoudiniApi::SetFaceCountsEmptyStub;
    HEMAX_HoudiniApi::SetGroupMembershipImpl = &HEMAX_HoudiniApi::SetGroupMembershipEmptyStub;
    HEMAX_HoudiniApi::SetHeightFieldDataImpl = &HEMAX_HoudiniApi::SetHeightFieldDataEmptyStub;
    HEMAX_HoudiniApi::SetImageInfoImpl = &HEMAX_HoudiniApi::SetImageInfoEmptyStub;
    HEMAX_HoudiniApi::SetInputCurveInfoImpl = &HEMAX_HoudiniApi::SetInputCurveInfoEmptyStub;
    HEMAX_HoudiniApi::SetInputCurvePositionsImpl = &HEMAX_HoudiniApi::SetInputCurvePositionsEmptyStub;
    HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScalesImpl = &HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScalesEmptyStub;
    HEMAX_HoudiniApi::SetNodeDisplayImpl = &HEMAX_HoudiniApi::SetNodeDisplayEmptyStub;
    HEMAX_HoudiniApi::SetObjectTransformImpl = &HEMAX_HoudiniApi::SetObjectTransformEmptyStub;
    HEMAX_HoudiniApi::SetParmExpressionImpl = &HEMAX_HoudiniApi::SetParmExpressionEmptyStub;
    HEMAX_HoudiniApi::SetParmFloatValueImpl = &HEMAX_HoudiniApi::SetParmFloatValueEmptyStub;
    HEMAX_HoudiniApi::SetParmFloatValuesImpl = &HEMAX_HoudiniApi::SetParmFloatValuesEmptyStub;
    HEMAX_HoudiniApi::SetParmIntValueImpl = &HEMAX_HoudiniApi::SetParmIntValueEmptyStub;
    HEMAX_HoudiniApi::SetParmIntValuesImpl = &HEMAX_HoudiniApi::SetParmIntValuesEmptyStub;
    HEMAX_HoudiniApi::SetParmNodeValueImpl = &HEMAX_HoudiniApi::SetParmNodeValueEmptyStub;
    HEMAX_HoudiniApi::SetParmStringValueImpl = &HEMAX_HoudiniApi::SetParmStringValueEmptyStub;
    HEMAX_HoudiniApi::SetPartInfoImpl = &HEMAX_HoudiniApi::SetPartInfoEmptyStub;
    HEMAX_HoudiniApi::SetPresetImpl = &HEMAX_HoudiniApi::SetPresetEmptyStub;
    HEMAX_HoudiniApi::SetServerEnvIntImpl = &HEMAX_HoudiniApi::SetServerEnvIntEmptyStub;
    HEMAX_HoudiniApi::SetServerEnvStringImpl = &HEMAX_HoudiniApi::SetServerEnvStringEmptyStub;
    HEMAX_HoudiniApi::SetSessionSyncImpl = &HEMAX_HoudiniApi::SetSessionSyncEmptyStub;
    HEMAX_HoudiniApi::SetSessionSyncInfoImpl = &HEMAX_HoudiniApi::SetSessionSyncInfoEmptyStub;
    HEMAX_HoudiniApi::SetTimeImpl = &HEMAX_HoudiniApi::SetTimeEmptyStub;
    HEMAX_HoudiniApi::SetTimelineOptionsImpl = &HEMAX_HoudiniApi::SetTimelineOptionsEmptyStub;
    HEMAX_HoudiniApi::SetTransformAnimCurveImpl = &HEMAX_HoudiniApi::SetTransformAnimCurveEmptyStub;
    HEMAX_HoudiniApi::SetUseHoudiniTimeImpl = &HEMAX_HoudiniApi::SetUseHoudiniTimeEmptyStub;
    HEMAX_HoudiniApi::SetVertexListImpl = &HEMAX_HoudiniApi::SetVertexListEmptyStub;
    HEMAX_HoudiniApi::SetViewportImpl = &HEMAX_HoudiniApi::SetViewportEmptyStub;
    HEMAX_HoudiniApi::SetVolumeInfoImpl = &HEMAX_HoudiniApi::SetVolumeInfoEmptyStub;
    HEMAX_HoudiniApi::SetVolumeTileFloatDataImpl = &HEMAX_HoudiniApi::SetVolumeTileFloatDataEmptyStub;
    HEMAX_HoudiniApi::SetVolumeTileIntDataImpl = &HEMAX_HoudiniApi::SetVolumeTileIntDataEmptyStub;
    HEMAX_HoudiniApi::SetVolumeVoxelFloatDataImpl = &HEMAX_HoudiniApi::SetVolumeVoxelFloatDataEmptyStub;
    HEMAX_HoudiniApi::SetVolumeVoxelIntDataImpl = &HEMAX_HoudiniApi::SetVolumeVoxelIntDataEmptyStub;
    HEMAX_HoudiniApi::SetWorkItemFloatAttributeImpl = &HEMAX_HoudiniApi::SetWorkItemFloatAttributeEmptyStub;
    HEMAX_HoudiniApi::SetWorkItemIntAttributeImpl = &HEMAX_HoudiniApi::SetWorkItemIntAttributeEmptyStub;
    HEMAX_HoudiniApi::SetWorkItemStringAttributeImpl = &HEMAX_HoudiniApi::SetWorkItemStringAttributeEmptyStub;
    HEMAX_HoudiniApi::SetWorkitemFloatDataImpl = &HEMAX_HoudiniApi::SetWorkitemFloatDataEmptyStub;
    HEMAX_HoudiniApi::SetWorkitemIntDataImpl = &HEMAX_HoudiniApi::SetWorkitemIntDataEmptyStub;
    HEMAX_HoudiniApi::SetWorkitemStringDataImpl = &HEMAX_HoudiniApi::SetWorkitemStringDataEmptyStub;
    HEMAX_HoudiniApi::ShutdownImpl = &HEMAX_HoudiniApi::ShutdownEmptyStub;
    HEMAX_HoudiniApi::StartThriftNamedPipeServerImpl = &HEMAX_HoudiniApi::StartThriftNamedPipeServerEmptyStub;
    HEMAX_HoudiniApi::StartThriftSocketServerImpl = &HEMAX_HoudiniApi::StartThriftSocketServerEmptyStub;
}


bool
HEMAX_HoudiniApi::IsHAPIInitialized()
{
    return ( HEMAX_HoudiniApi::IsInitializedImpl != &HEMAX_HoudiniApi::IsInitializedEmptyStub );
}
bool
HEMAX_HoudiniApi::HandleHAPIResult(const HAPI_Session* session, HAPI_Result Result)
{
    if (Result != HAPI_RESULT_SUCCESS)
    {
        if (!session)
            return false;

        int StrBufLen;
        HEMAX_HoudiniApi::GetStatusStringBufLength(session,
            HAPI_STATUS_CALL_RESULT, HAPI_STATUSVERBOSITY_ERRORS, &StrBufLen);
        if (StrBufLen > 0)
        {
            char* StatusString = new char[StrBufLen];
            HEMAX_HoudiniApi::GetStatusString(session, HAPI_STATUS_CALL_RESULT,
                StatusString, StrBufLen);
            HEMAX_Logger::Instance().AddEntry(StatusString, HEMAX_LOG_LEVEL_ERROR);
            delete [] StatusString;
            return false;
        }
    }

    return true;
}


bool
HEMAX_HoudiniApi::AddAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::AddAttributeImpl(session, node_id, part_id, name, attr_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::AddGroup(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::AddGroupImpl(session, node_id, part_id, group_type, group_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::BindCustomImplementation(HAPI_SessionType session_type, const char * dll_path)
{
    HAPI_Result Result = HEMAX_HoudiniApi::BindCustomImplementationImpl(session_type, dll_path);
    return HandleHAPIResult(nullptr, Result);
}


bool
HEMAX_HoudiniApi::CancelPDGCook(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CancelPDGCookImpl(session, graph_context_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CheckForSpecificErrors(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ErrorCodeBits errors_to_look_for, HAPI_ErrorCodeBits * errors_found)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CheckForSpecificErrorsImpl(session, node_id, errors_to_look_for, errors_found);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::Cleanup(const HAPI_Session * session)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CleanupImpl(session);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ClearConnectionError()
{
    HAPI_Result Result = HEMAX_HoudiniApi::ClearConnectionErrorImpl();
    return HandleHAPIResult(nullptr, Result);
}


bool
HEMAX_HoudiniApi::CloseSession(const HAPI_Session * session)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CloseSessionImpl(session);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CommitGeo(const HAPI_Session * session, HAPI_NodeId node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CommitGeoImpl(session, node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CommitWorkItems(const HAPI_Session * session, HAPI_NodeId node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CommitWorkItemsImpl(session, node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CommitWorkitems(const HAPI_Session * session, HAPI_NodeId node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CommitWorkitemsImpl(session, node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ComposeChildNodeList(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_NodeTypeBits node_type_filter, HAPI_NodeFlagsBits node_flags_filter, HAPI_Bool recursive, int * count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ComposeChildNodeListImpl(session, parent_node_id, node_type_filter, node_flags_filter, recursive, count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ComposeNodeCookResult(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_StatusVerbosity verbosity, int * buffer_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ComposeNodeCookResultImpl(session, node_id, verbosity, buffer_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ComposeObjectList(const HAPI_Session * session, HAPI_NodeId parent_node_id, const char * categories, int * object_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ComposeObjectListImpl(session, parent_node_id, categories, object_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ConnectNodeInput(const HAPI_Session * session, HAPI_NodeId node_id, int input_index, HAPI_NodeId node_id_to_connect, int output_index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ConnectNodeInputImpl(session, node_id, input_index, node_id_to_connect, output_index);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ConvertMatrixToEuler(const HAPI_Session * session, const float * matrix, HAPI_RSTOrder rst_order, HAPI_XYZOrder rot_order, HAPI_TransformEuler * transform_out)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ConvertMatrixToEulerImpl(session, matrix, rst_order, rot_order, transform_out);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ConvertMatrixToQuat(const HAPI_Session * session, const float * matrix, HAPI_RSTOrder rst_order, HAPI_Transform * transform_out)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ConvertMatrixToQuatImpl(session, matrix, rst_order, transform_out);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ConvertTransform(const HAPI_Session * session, const HAPI_TransformEuler * transform_in, HAPI_RSTOrder rst_order, HAPI_XYZOrder rot_order, HAPI_TransformEuler * transform_out)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ConvertTransformImpl(session, transform_in, rst_order, rot_order, transform_out);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ConvertTransformEulerToMatrix(const HAPI_Session * session, const HAPI_TransformEuler * transform, float * matrix)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ConvertTransformEulerToMatrixImpl(session, transform, matrix);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ConvertTransformQuatToMatrix(const HAPI_Session * session, const HAPI_Transform * transform, float * matrix)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ConvertTransformQuatToMatrixImpl(session, transform, matrix);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CookNode(const HAPI_Session * session, HAPI_NodeId node_id, const HAPI_CookOptions * cook_options)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CookNodeImpl(session, node_id, cook_options);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CookPDG(const HAPI_Session * session, HAPI_NodeId cook_node_id, int generate_only, int blocking)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CookPDGImpl(session, cook_node_id, generate_only, blocking);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CookPDGAllOutputs(const HAPI_Session* session, HAPI_NodeId cook_node_id, int generate_only, int blocking)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CookPDGAllOutputsImpl(session, cook_node_id, generate_only, blocking);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateCustomSession(HAPI_SessionType session_type, void * session_info, HAPI_Session * session)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateCustomSessionImpl(session_type, session_info, session);
    return HandleHAPIResult(nullptr, Result);
}


bool
HEMAX_HoudiniApi::CreateHeightFieldInput(const HAPI_Session * session, HAPI_NodeId parent_node_id, const char * name, int xsize, int ysize, float voxelsize, HAPI_HeightFieldSampling sampling, HAPI_NodeId * heightfield_node_id, HAPI_NodeId * height_node_id, HAPI_NodeId * mask_node_id, HAPI_NodeId * merge_node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateHeightFieldInputImpl(session, parent_node_id, name, xsize, ysize, voxelsize, sampling, heightfield_node_id, height_node_id, mask_node_id, merge_node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNode(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_NodeId * new_node_id, const char * name, int xsize, int ysize, float voxelsize)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNodeImpl(session, parent_node_id, new_node_id, name, xsize, ysize, voxelsize);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateInProcessSession(HAPI_Session * session)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateInProcessSessionImpl(session);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateInputCurveNode(const HAPI_Session * session, HAPI_NodeId * node_id, const char * name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateInputCurveNodeImpl(session, node_id, name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateInputNode(const HAPI_Session * session, HAPI_NodeId * node_id, const char * name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateInputNodeImpl(session, node_id, name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateNode(const HAPI_Session * session, HAPI_NodeId parent_node_id, const char * operator_name, const char * node_label, HAPI_Bool cook_on_creation, HAPI_NodeId * new_node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateNodeImpl(session, parent_node_id, operator_name, node_label, cook_on_creation, new_node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateThriftNamedPipeSession(HAPI_Session * session, const char * pipe_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftNamedPipeSessionImpl(session, pipe_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateThriftSocketSession(HAPI_Session * session, const char * host_name, int port)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateThriftSocketSessionImpl(session, host_name, port);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateWorkItem(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId * work_item_id, const char * name, int index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateWorkItemImpl(session, node_id, work_item_id, name, index);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::CreateWorkitem(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId * workitem_id, const char * name, int index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::CreateWorkitemImpl(session, node_id, workitem_id, name, index);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::DeleteAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::DeleteAttributeImpl(session, node_id, part_id, name, attr_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::DeleteGroup(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::DeleteGroupImpl(session, node_id, part_id, group_type, group_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::DeleteNode(const HAPI_Session * session, HAPI_NodeId node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::DeleteNodeImpl(session, node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::DirtyPDGNode(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Bool clean_results)
{
    HAPI_Result Result = HEMAX_HoudiniApi::DirtyPDGNodeImpl(session, node_id, clean_results);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::DisconnectNodeInput(const HAPI_Session * session, HAPI_NodeId node_id, int input_index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::DisconnectNodeInputImpl(session, node_id, input_index);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::DisconnectNodeOutputsAt(const HAPI_Session * session, HAPI_NodeId node_id, int output_index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::DisconnectNodeOutputsAtImpl(session, node_id, output_index);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ExtractImageToFile(const HAPI_Session * session, HAPI_NodeId material_node_id, const char * image_file_format_name, const char * image_planes, const char * destination_folder_path, const char * destination_file_name, int * destination_file_path)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ExtractImageToFileImpl(session, material_node_id, image_file_format_name, image_planes, destination_folder_path, destination_file_name, destination_file_path);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ExtractImageToMemory(const HAPI_Session * session, HAPI_NodeId material_node_id, const char * image_file_format_name, const char * image_planes, int * buffer_size)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ExtractImageToMemoryImpl(session, material_node_id, image_file_format_name, image_planes, buffer_size);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetActiveCacheCount(const HAPI_Session * session, int * active_cache_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetActiveCacheCountImpl(session, active_cache_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetActiveCacheNames(const HAPI_Session * session, HAPI_StringHandle * cache_names_array, int active_cache_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetActiveCacheNamesImpl(session, cache_names_array, active_cache_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAssetDefinitionParmCounts(const HAPI_Session * session, HAPI_AssetLibraryId library_id, const char * asset_name, int * parm_count, int * int_value_count, int * float_value_count, int * string_value_count, int * choice_value_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAssetDefinitionParmCountsImpl(session, library_id, asset_name, parm_count, int_value_count, float_value_count, string_value_count, choice_value_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAssetDefinitionParmInfos(const HAPI_Session * session, HAPI_AssetLibraryId library_id, const char * asset_name, HAPI_ParmInfo * parm_infos_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAssetDefinitionParmInfosImpl(session, library_id, asset_name, parm_infos_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAssetDefinitionParmValues(const HAPI_Session * session, HAPI_AssetLibraryId library_id, const char * asset_name, int * int_values_array, int int_start, int int_length, float * float_values_array, int float_start, int float_length, HAPI_Bool string_evaluate, HAPI_StringHandle * string_values_array, int string_start, int string_length, HAPI_ParmChoiceInfo * choice_values_array, int choice_start, int choice_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAssetDefinitionParmValuesImpl(session, library_id, asset_name, int_values_array, int_start, int_length, float_values_array, float_start, float_length, string_evaluate, string_values_array, string_start, string_length, choice_values_array, choice_start, choice_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAssetInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_AssetInfo * asset_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAssetInfoImpl(session, node_id, asset_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeFloat64ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, double * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeFloat64ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeFloat64Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, double * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeFloat64DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeFloatArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, float * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeFloatArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, float * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeFloatDataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeOwner owner, HAPI_AttributeInfo * attr_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeInfoImpl(session, node_id, part_id, name, owner, attr_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt16ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_Int16 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeInt16ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt16Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_Int16 * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeInt16DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt64ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_Int64 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeInt64ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt64Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_Int64 * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeInt64DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt8ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_Int8 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeInt8ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt8Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_Int8 * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeInt8DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeIntArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeIntArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, int * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeIntDataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeNames(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_AttributeOwner owner, HAPI_StringHandle * attribute_names_array, int count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeNamesImpl(session, node_id, part_id, owner, attribute_names_array, count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeStringArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_StringHandle * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeStringArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeStringData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_StringHandle * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeStringDataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeUInt8ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_UInt8 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeUInt8ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAttributeUInt8Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_UInt8 * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAttributeUInt8DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAvailableAssetCount(const HAPI_Session * session, HAPI_AssetLibraryId library_id, int * asset_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAvailableAssetCountImpl(session, library_id, asset_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetAvailableAssets(const HAPI_Session * session, HAPI_AssetLibraryId library_id, HAPI_StringHandle * asset_names_array, int asset_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetAvailableAssetsImpl(session, library_id, asset_names_array, asset_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetBoxInfo(const HAPI_Session * session, HAPI_NodeId geo_node_id, HAPI_PartId part_id, HAPI_BoxInfo * box_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetBoxInfoImpl(session, geo_node_id, part_id, box_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetCacheProperty(const HAPI_Session * session, const char * cache_name, HAPI_CacheProperty cache_property, int * property_value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCachePropertyImpl(session, cache_name, cache_property, property_value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetComposedChildNodeList(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_NodeId * child_node_ids_array, int count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetComposedChildNodeListImpl(session, parent_node_id, child_node_ids_array, count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetComposedNodeCookResult(const HAPI_Session * session, char * string_value, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetComposedNodeCookResultImpl(session, string_value, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetComposedObjectList(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_ObjectInfo * object_infos_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetComposedObjectListImpl(session, parent_node_id, object_infos_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetComposedObjectTransforms(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_RSTOrder rst_order, HAPI_Transform * transform_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetComposedObjectTransformsImpl(session, parent_node_id, rst_order, transform_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetCompositorOptions(const HAPI_Session * session, HAPI_CompositorOptions * compositor_options)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCompositorOptionsImpl(session, compositor_options);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetConnectionError(char * string_value, int length, HAPI_Bool clear)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetConnectionErrorImpl(string_value, length, clear);
    return HandleHAPIResult(nullptr, Result);
}


bool
HEMAX_HoudiniApi::GetConnectionErrorLength(int * buffer_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetConnectionErrorLengthImpl(buffer_length);
    return HandleHAPIResult(nullptr, Result);
}


bool
HEMAX_HoudiniApi::GetCookingCurrentCount(const HAPI_Session * session, int * count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCookingCurrentCountImpl(session, count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetCookingTotalCount(const HAPI_Session * session, int * count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCookingTotalCountImpl(session, count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetCurveCounts(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * counts_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCurveCountsImpl(session, node_id, part_id, counts_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetCurveInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_CurveInfo * info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCurveInfoImpl(session, node_id, part_id, info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetCurveKnots(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float * knots_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCurveKnotsImpl(session, node_id, part_id, knots_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetCurveOrders(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * orders_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetCurveOrdersImpl(session, node_id, part_id, orders_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetDisplayGeoInfo(const HAPI_Session * session, HAPI_NodeId object_node_id, HAPI_GeoInfo * geo_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetDisplayGeoInfoImpl(session, object_node_id, geo_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroup(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * group_name, int * edge_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroupImpl(session, node_id, part_id, group_name, edge_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetEnvInt(HAPI_EnvIntType int_type, int * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetEnvIntImpl(int_type, value);
    return HandleHAPIResult(nullptr, Result);
}


bool
HEMAX_HoudiniApi::GetFaceCounts(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * face_counts_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetFaceCountsImpl(session, node_id, part_id, face_counts_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetFirstVolumeTile(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeTileInfo * tile)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetFirstVolumeTileImpl(session, node_id, part_id, tile);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetGeoInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_GeoInfo * geo_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetGeoInfoImpl(session, node_id, geo_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetGeoSize(const HAPI_Session * session, HAPI_NodeId node_id, const char * format, int * size)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetGeoSizeImpl(session, node_id, format, size);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePart(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * pointGroupCount, int * primitiveGroupCount)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePartImpl(session, node_id, part_id, pointGroupCount, primitiveGroupCount);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetGroupMembership(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, HAPI_Bool * membership_array_all_equal, int * membership_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetGroupMembershipImpl(session, node_id, part_id, group_type, group_name, membership_array_all_equal, membership_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePart(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, HAPI_Bool * membership_array_all_equal, int * membership_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePartImpl(session, node_id, part_id, group_type, group_name, membership_array_all_equal, membership_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetGroupNames(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_GroupType group_type, HAPI_StringHandle * group_names_array, int group_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetGroupNamesImpl(session, node_id, group_type, group_names_array, group_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePart(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, HAPI_StringHandle * group_names_array, int group_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePartImpl(session, node_id, part_id, group_type, group_names_array, group_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetHIPFileNodeCount(const HAPI_Session * session, HAPI_HIPFileId id, int * count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetHIPFileNodeCountImpl(session, id, count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetHIPFileNodeIds(const HAPI_Session * session, HAPI_HIPFileId id, HAPI_NodeId * node_ids, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetHIPFileNodeIdsImpl(session, id, node_ids, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetHandleBindingInfo(const HAPI_Session * session, HAPI_NodeId node_id, int handle_index, HAPI_HandleBindingInfo * handle_binding_infos_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetHandleBindingInfoImpl(session, node_id, handle_index, handle_binding_infos_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetHandleInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_HandleInfo * handle_infos_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetHandleInfoImpl(session, node_id, handle_infos_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetHeightFieldData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float * values_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetHeightFieldDataImpl(session, node_id, part_id, values_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetImageFilePath(const HAPI_Session * session, HAPI_NodeId material_node_id, const char * image_file_format_name, const char * image_planes, const char * destination_folder_path, const char * destination_file_name, HAPI_ParmId texture_parm_id, int * destination_file_path)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetImageFilePathImpl(session, material_node_id, image_file_format_name, image_planes, destination_folder_path, destination_file_name, texture_parm_id, destination_file_path);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetImageInfo(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_ImageInfo * image_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetImageInfoImpl(session, material_node_id, image_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetImageMemoryBuffer(const HAPI_Session * session, HAPI_NodeId material_node_id, char * buffer, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetImageMemoryBufferImpl(session, material_node_id, buffer, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetImagePlaneCount(const HAPI_Session * session, HAPI_NodeId material_node_id, int * image_plane_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetImagePlaneCountImpl(session, material_node_id, image_plane_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetImagePlanes(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_StringHandle * image_planes_array, int image_plane_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetImagePlanesImpl(session, material_node_id, image_planes_array, image_plane_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetInputCurveInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_InputCurveInfo * info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetInputCurveInfoImpl(session, node_id, part_id, info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetInstanceTransformsOnPart(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_RSTOrder rst_order, HAPI_Transform * transforms_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetInstanceTransformsOnPartImpl(session, node_id, part_id, rst_order, transforms_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetInstancedObjectIds(const HAPI_Session * session, HAPI_NodeId object_node_id, HAPI_NodeId * instanced_node_id_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetInstancedObjectIdsImpl(session, object_node_id, instanced_node_id_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetInstancedPartIds(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_PartId * instanced_parts_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetInstancedPartIdsImpl(session, node_id, part_id, instanced_parts_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetInstancerPartTransforms(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_RSTOrder rst_order, HAPI_Transform * transforms_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetInstancerPartTransformsImpl(session, node_id, part_id, rst_order, transforms_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetManagerNodeId(const HAPI_Session * session, HAPI_NodeType node_type, HAPI_NodeId * node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetManagerNodeIdImpl(session, node_type, node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetMaterialInfo(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_MaterialInfo * material_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetMaterialInfoImpl(session, material_node_id, material_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetMaterialNodeIdsOnFaces(const HAPI_Session * session, HAPI_NodeId geometry_node_id, HAPI_PartId part_id, HAPI_Bool * are_all_the_same, HAPI_NodeId * material_ids_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetMaterialNodeIdsOnFacesImpl(session, geometry_node_id, part_id, are_all_the_same, material_ids_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetNextVolumeTile(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeTileInfo * tile)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetNextVolumeTileImpl(session, node_id, part_id, tile);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetNodeFromPath(const HAPI_Session * session, const HAPI_NodeId parent_node_id, const char * path, HAPI_NodeId * node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetNodeFromPathImpl(session, parent_node_id, path, node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetNodeInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeInfo * node_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetNodeInfoImpl(session, node_id, node_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetNodeInputName(const HAPI_Session * session, HAPI_NodeId node_id, int input_idx, HAPI_StringHandle * name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetNodeInputNameImpl(session, node_id, input_idx, name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetNodeOutputName(const HAPI_Session * session, HAPI_NodeId node_id, int output_idx, HAPI_StringHandle * name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetNodeOutputNameImpl(session, node_id, output_idx, name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetNodePath(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeId relative_to_node_id, HAPI_StringHandle * path)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetNodePathImpl(session, node_id, relative_to_node_id, path);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetNumWorkItems(const HAPI_Session * session, HAPI_NodeId node_id, int * num)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetNumWorkItemsImpl(session, node_id, num);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetNumWorkitems(const HAPI_Session * session, HAPI_NodeId node_id, int * num)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetNumWorkitemsImpl(session, node_id, num);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetObjectInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ObjectInfo * object_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetObjectInfoImpl(session, node_id, object_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetObjectTransform(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeId relative_to_node_id, HAPI_RSTOrder rst_order, HAPI_Transform * transform)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetObjectTransformImpl(session, node_id, relative_to_node_id, rst_order, transform);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetOutputGeoCount(const HAPI_Session* session, HAPI_NodeId node_id, int* count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetOutputGeoCountImpl(session, node_id, count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetOutputGeoInfos(const HAPI_Session* session, HAPI_NodeId node_id, HAPI_GeoInfo* geo_infos_array, int count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetOutputGeoInfosImpl(session, node_id, geo_infos_array, count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetOutputNodeId(const HAPI_Session * session, HAPI_NodeId node_id, int output, HAPI_NodeId * output_node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetOutputNodeIdImpl(session, node_id, output, output_node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetPDGEvents(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_PDG_EventInfo * event_array, int length, int * event_count, int * remaining_events)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetPDGEventsImpl(session, graph_context_id, event_array, length, event_count, remaining_events);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetPDGGraphContextId(const HAPI_Session * session, HAPI_NodeId top_node_id, HAPI_PDG_GraphContextId * context_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetPDGGraphContextIdImpl(session, top_node_id, context_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetPDGGraphContexts(const HAPI_Session * session, HAPI_StringHandle * context_names_array, HAPI_PDG_GraphContextId * context_id_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetPDGGraphContextsImpl(session, context_names_array, context_id_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetPDGGraphContextsCount(const HAPI_Session* session, int* num_contexts)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetPDGGraphContextsCountImpl(session, num_contexts);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetPDGState(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, int * pdg_state)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetPDGStateImpl(session, graph_context_id, pdg_state);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParameters(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmInfo * parm_infos_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParametersImpl(session, node_id, parm_infos_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmChoiceLists(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmChoiceInfo * parm_choices_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmChoiceListsImpl(session, node_id, parm_choices_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmExpression(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_StringHandle * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmExpressionImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmFile(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, const char * destination_directory, const char * destination_file_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmFileImpl(session, node_id, parm_name, destination_directory, destination_file_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmFloatValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, float * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmFloatValueImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmFloatValues(const HAPI_Session * session, HAPI_NodeId node_id, float * values_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmFloatValuesImpl(session, node_id, values_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmIdFromName(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_ParmId * parm_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmIdFromNameImpl(session, node_id, parm_name, parm_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, HAPI_ParmInfo * parm_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmInfoImpl(session, node_id, parm_id, parm_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmInfoFromName(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_ParmInfo * parm_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmInfoFromNameImpl(session, node_id, parm_name, parm_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmIntValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, int * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmIntValueImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmIntValues(const HAPI_Session * session, HAPI_NodeId node_id, int * values_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmIntValuesImpl(session, node_id, values_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmNodeValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_NodeId * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmNodeValueImpl(session, node_id, parm_name, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmStringValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_Bool evaluate, HAPI_StringHandle * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmStringValueImpl(session, node_id, parm_name, index, evaluate, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmStringValues(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Bool evaluate, HAPI_StringHandle * values_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmStringValuesImpl(session, node_id, evaluate, values_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmTagName(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int tag_index, HAPI_StringHandle * tag_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmTagNameImpl(session, node_id, parm_id, tag_index, tag_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmTagValue(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, const char * tag_name, HAPI_StringHandle * tag_value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmTagValueImpl(session, node_id, parm_id, tag_name, tag_value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetParmWithTag(const HAPI_Session * session, HAPI_NodeId node_id, const char * tag_name, HAPI_ParmId * parm_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetParmWithTagImpl(session, node_id, tag_name, parm_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetPartInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_PartInfo * part_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetPartInfoImpl(session, node_id, part_id, part_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetPreset(const HAPI_Session * session, HAPI_NodeId node_id, char * buffer, int buffer_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetPresetImpl(session, node_id, buffer, buffer_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetPresetBufLength(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PresetType preset_type, const char * preset_name, int * buffer_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetPresetBufLengthImpl(session, node_id, preset_type, preset_name, buffer_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetServerEnvInt(const HAPI_Session * session, const char * variable_name, int * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetServerEnvIntImpl(session, variable_name, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetServerEnvString(const HAPI_Session * session, const char * variable_name, HAPI_StringHandle * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetServerEnvStringImpl(session, variable_name, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetServerEnvVarCount(const HAPI_Session * session, int * env_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetServerEnvVarCountImpl(session, env_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetServerEnvVarList(const HAPI_Session * session, HAPI_StringHandle * values_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetServerEnvVarListImpl(session, values_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetSessionEnvInt(const HAPI_Session * session, HAPI_SessionEnvIntType int_type, int * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetSessionEnvIntImpl(session, int_type, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetSessionSyncInfo(const HAPI_Session * session, HAPI_SessionSyncInfo * session_sync_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetSessionSyncInfoImpl(session, session_sync_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetSphereInfo(const HAPI_Session * session, HAPI_NodeId geo_node_id, HAPI_PartId part_id, HAPI_SphereInfo * sphere_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetSphereInfoImpl(session, geo_node_id, part_id, sphere_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetStatus(const HAPI_Session * session, HAPI_StatusType status_type, int * status)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetStatusImpl(session, status_type, status);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetStatusString(const HAPI_Session * session, HAPI_StatusType status_type, char * string_value, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetStatusStringImpl(session, status_type, string_value, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetStatusStringBufLength(const HAPI_Session * session, HAPI_StatusType status_type, HAPI_StatusVerbosity verbosity, int * buffer_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetStatusStringBufLengthImpl(session, status_type, verbosity, buffer_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetString(const HAPI_Session * session, HAPI_StringHandle string_handle, char * string_value, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetStringImpl(session, string_handle, string_value, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetStringBatch(const HAPI_Session * session, char * char_buffer, int char_array_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetStringBatchImpl(session, char_buffer, char_array_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetStringBatchSize(const HAPI_Session * session, const int * string_handle_array, int string_handle_count, int * string_buffer_size)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetStringBatchSizeImpl(session, string_handle_array, string_handle_count, string_buffer_size);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetStringBufLength(const HAPI_Session * session, HAPI_StringHandle string_handle, int * buffer_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetStringBufLengthImpl(session, string_handle, buffer_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetSupportedImageFileFormatCount(const HAPI_Session * session, int * file_format_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetSupportedImageFileFormatCountImpl(session, file_format_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetSupportedImageFileFormats(const HAPI_Session * session, HAPI_ImageFileFormat * formats_array, int file_format_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetSupportedImageFileFormatsImpl(session, formats_array, file_format_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetTime(const HAPI_Session * session, float * time)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetTimeImpl(session, time);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetTimelineOptions(const HAPI_Session * session, HAPI_TimelineOptions * timeline_options)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetTimelineOptionsImpl(session, timeline_options);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetTotalCookCount(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeTypeBits node_type_filter, HAPI_NodeFlagsBits node_flags_filter, HAPI_Bool recursive, int * count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetTotalCookCountImpl(session, node_id, node_type_filter, node_flags_filter, recursive, count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetUseHoudiniTime(const HAPI_Session * session, HAPI_Bool * enabled)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetUseHoudiniTimeImpl(session, enabled);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetVertexList(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * vertex_list_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetVertexListImpl(session, node_id, part_id, vertex_list_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetViewport(const HAPI_Session * session, HAPI_Viewport * viewport)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetViewportImpl(session, viewport);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetVolumeBounds(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float * x_min, float * y_min, float * z_min, float * x_max, float * y_max, float * z_max, float * x_center, float * y_center, float * z_center)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetVolumeBoundsImpl(session, node_id, part_id, x_min, y_min, z_min, x_max, y_max, z_max, x_center, y_center, z_center);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetVolumeInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeInfo * volume_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetVolumeInfoImpl(session, node_id, part_id, volume_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetVolumeTileFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float fill_value, const HAPI_VolumeTileInfo * tile, float * values_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetVolumeTileFloatDataImpl(session, node_id, part_id, fill_value, tile, values_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetVolumeTileIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int fill_value, const HAPI_VolumeTileInfo * tile, int * values_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetVolumeTileIntDataImpl(session, node_id, part_id, fill_value, tile, values_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetVolumeVisualInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeVisualInfo * visual_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetVolumeVisualInfoImpl(session, node_id, part_id, visual_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetVolumeVoxelFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, float * values_array, int value_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetVolumeVoxelFloatDataImpl(session, node_id, part_id, x_index, y_index, z_index, values_array, value_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetVolumeVoxelIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, int * values_array, int value_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetVolumeVoxelIntDataImpl(session, node_id, part_id, x_index, y_index, z_index, values_array, value_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkItemAttributeSize(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, int * length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkItemAttributeSizeImpl(session, node_id, work_item_id, attribute_name, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkItemFloatAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, float * data_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkItemFloatAttributeImpl(session, node_id, work_item_id, attribute_name, data_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkItemInfo(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_PDG_WorkItemId work_item_id, HAPI_PDG_WorkItemInfo * work_item_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkItemInfoImpl(session, graph_context_id, work_item_id, work_item_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkItemIntAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, int * data_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkItemIntAttributeImpl(session, node_id, work_item_id, attribute_name, data_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkItemOutputFiles(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, HAPI_PDG_WorkItemOutputFile * resultinfo_array, int resultinfo_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkItemOutputFilesImpl(session, node_id, work_item_id, resultinfo_array, resultinfo_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkItemStringAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, HAPI_StringHandle * data_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkItemStringAttributeImpl(session, node_id, work_item_id, attribute_name, data_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkItems(const HAPI_Session * session, HAPI_NodeId node_id, int * work_item_ids_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkItemsImpl(session, node_id, work_item_ids_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkitemDataLength(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, int * length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkitemDataLengthImpl(session, node_id, workitem_id, data_name, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkitemFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, float * data_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkitemFloatDataImpl(session, node_id, workitem_id, data_name, data_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkitemInfo(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_PDG_WorkItemId workitem_id, HAPI_PDG_WorkItemInfo * workitem_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkitemInfoImpl(session, graph_context_id, workitem_id, workitem_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkitemIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, int * data_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkitemIntDataImpl(session, node_id, workitem_id, data_name, data_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkitemResultInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, HAPI_PDG_WorkItemOutputFile * resultinfo_array, int resultinfo_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkitemResultInfoImpl(session, node_id, workitem_id, resultinfo_array, resultinfo_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkitemStringData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, HAPI_StringHandle * data_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkitemStringDataImpl(session, node_id, workitem_id, data_name, data_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::GetWorkitems(const HAPI_Session * session, HAPI_NodeId node_id, int * workitem_ids_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::GetWorkitemsImpl(session, node_id, workitem_ids_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::Initialize(const HAPI_Session * session, const HAPI_CookOptions * cook_options, HAPI_Bool use_cooking_thread, int cooking_thread_stack_size, const char * houdini_environment_files, const char * otl_search_path, const char * dso_search_path, const char * image_dso_search_path, const char * audio_dso_search_path)
{
    HAPI_Result Result = HEMAX_HoudiniApi::InitializeImpl(session, cook_options, use_cooking_thread, cooking_thread_stack_size, houdini_environment_files, otl_search_path, dso_search_path, image_dso_search_path, audio_dso_search_path);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::InsertMultiparmInstance(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int instance_position)
{
    HAPI_Result Result = HEMAX_HoudiniApi::InsertMultiparmInstanceImpl(session, node_id, parm_id, instance_position);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::Interrupt(const HAPI_Session * session)
{
    HAPI_Result Result = HEMAX_HoudiniApi::InterruptImpl(session);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::IsInitialized(const HAPI_Session * session)
{
    HAPI_Result Result = HEMAX_HoudiniApi::IsInitializedImpl(session);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::IsNodeValid(const HAPI_Session * session, HAPI_NodeId node_id, int unique_node_id, HAPI_Bool * answer)
{
    HAPI_Result Result = HEMAX_HoudiniApi::IsNodeValidImpl(session, node_id, unique_node_id, answer);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::IsSessionValid(const HAPI_Session * session)
{
    HAPI_Result Result = HEMAX_HoudiniApi::IsSessionValidImpl(session);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::LoadAssetLibraryFromFile(const HAPI_Session * session, const char * file_path, HAPI_Bool allow_overwrite, HAPI_AssetLibraryId * library_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::LoadAssetLibraryFromFileImpl(session, file_path, allow_overwrite, library_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::LoadAssetLibraryFromMemory(const HAPI_Session * session, const char * library_buffer, int library_buffer_length, HAPI_Bool allow_overwrite, HAPI_AssetLibraryId * library_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::LoadAssetLibraryFromMemoryImpl(session, library_buffer, library_buffer_length, allow_overwrite, library_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::LoadGeoFromFile(const HAPI_Session * session, HAPI_NodeId node_id, const char * file_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::LoadGeoFromFileImpl(session, node_id, file_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::LoadGeoFromMemory(const HAPI_Session * session, HAPI_NodeId node_id, const char * format, const char * buffer, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::LoadGeoFromMemoryImpl(session, node_id, format, buffer, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::LoadHIPFile(const HAPI_Session * session, const char * file_name, HAPI_Bool cook_on_load)
{
    HAPI_Result Result = HEMAX_HoudiniApi::LoadHIPFileImpl(session, file_name, cook_on_load);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::LoadNodeFromFile(const HAPI_Session * session, const char * file_name, HAPI_NodeId parent_node_id, const char * node_label, HAPI_Bool cook_on_load, HAPI_NodeId * new_node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::LoadNodeFromFileImpl(session, file_name, parent_node_id, node_label, cook_on_load, new_node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::MergeHIPFile(const HAPI_Session * session, const char * file_name, HAPI_Bool cook_on_load, HAPI_HIPFileId * file_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::MergeHIPFileImpl(session, file_name, cook_on_load, file_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ParmHasExpression(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_Bool * has_expression)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ParmHasExpressionImpl(session, node_id, parm_name, index, has_expression);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ParmHasTag(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, const char * tag_name, HAPI_Bool * has_tag)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ParmHasTagImpl(session, node_id, parm_id, tag_name, has_tag);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::PausePDGCook(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::PausePDGCookImpl(session, graph_context_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::PythonThreadInterpreterLock(const HAPI_Session * session, HAPI_Bool locked)
{
    HAPI_Result Result = HEMAX_HoudiniApi::PythonThreadInterpreterLockImpl(session, locked);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::QueryNodeInput(const HAPI_Session * session, HAPI_NodeId node_to_query, int input_index, HAPI_NodeId * connected_node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::QueryNodeInputImpl(session, node_to_query, input_index, connected_node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::QueryNodeOutputConnectedCount(const HAPI_Session * session, HAPI_NodeId node_id, int output_idx, HAPI_Bool into_subnets, HAPI_Bool through_dots, int * connected_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::QueryNodeOutputConnectedCountImpl(session, node_id, output_idx, into_subnets, through_dots, connected_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::QueryNodeOutputConnectedNodes(const HAPI_Session * session, HAPI_NodeId node_id, int output_idx, HAPI_Bool into_subnets, HAPI_Bool through_dots, HAPI_NodeId * connected_node_ids_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::QueryNodeOutputConnectedNodesImpl(session, node_id, output_idx, into_subnets, through_dots, connected_node_ids_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::RemoveCustomString(const HAPI_Session * session, const HAPI_StringHandle string_handle)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RemoveCustomStringImpl(session, string_handle);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::RemoveMultiparmInstance(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int instance_position)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RemoveMultiparmInstanceImpl(session, node_id, parm_id, instance_position);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::RemoveParmExpression(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RemoveParmExpressionImpl(session, node_id, parm_id, index);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::RenameNode(const HAPI_Session * session, HAPI_NodeId node_id, const char * new_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RenameNodeImpl(session, node_id, new_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::RenderCOPToImage(const HAPI_Session * session, HAPI_NodeId cop_node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RenderCOPToImageImpl(session, cop_node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::RenderTextureToImage(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_ParmId parm_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RenderTextureToImageImpl(session, material_node_id, parm_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::ResetSimulation(const HAPI_Session * session, HAPI_NodeId node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ResetSimulationImpl(session, node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::RevertGeo(const HAPI_Session * session, HAPI_NodeId node_id)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RevertGeoImpl(session, node_id);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::RevertParmToDefault(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RevertParmToDefaultImpl(session, node_id, parm_name, index);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::RevertParmToDefaults(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::RevertParmToDefaultsImpl(session, node_id, parm_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SaveGeoToFile(const HAPI_Session * session, HAPI_NodeId node_id, const char * file_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SaveGeoToFileImpl(session, node_id, file_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SaveGeoToMemory(const HAPI_Session * session, HAPI_NodeId node_id, char * buffer, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SaveGeoToMemoryImpl(session, node_id, buffer, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SaveHIPFile(const HAPI_Session * session, const char * file_path, HAPI_Bool lock_nodes)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SaveHIPFileImpl(session, file_path, lock_nodes);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SaveNodeToFile(const HAPI_Session * session, HAPI_NodeId node_id, const char * file_name)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SaveNodeToFileImpl(session, node_id, file_name);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAnimCurve(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int parm_index, const HAPI_Keyframe * curve_keyframes_array, int keyframe_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAnimCurveImpl(session, node_id, parm_id, parm_index, curve_keyframes_array, keyframe_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeFloat64ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const double * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeFloat64ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeFloat64Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const double * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeFloat64DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeFloatArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const float * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeFloatArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const float * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeFloatDataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt16ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int16 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeInt16ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt16Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int16 * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeInt16DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt64ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int64 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeInt64ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt64Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int64 * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeInt64DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt8ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int8 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeInt8ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt8Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int8 * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeInt8DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeIntArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const int * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeIntArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const int * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeIntDataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeStringArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const char ** data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeStringArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeStringData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const char ** data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeStringDataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeUInt8ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_UInt8 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeUInt8ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetAttributeUInt8Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_UInt8 * data_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetAttributeUInt8DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetCacheProperty(const HAPI_Session * session, const char * cache_name, HAPI_CacheProperty cache_property, int property_value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCachePropertyImpl(session, cache_name, cache_property, property_value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetCompositorOptions(const HAPI_Session * session, const HAPI_CompositorOptions * compositor_options)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCompositorOptionsImpl(session, compositor_options);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetCurveCounts(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * counts_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCurveCountsImpl(session, node_id, part_id, counts_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetCurveInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_CurveInfo * info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCurveInfoImpl(session, node_id, part_id, info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetCurveKnots(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const float * knots_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCurveKnotsImpl(session, node_id, part_id, knots_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetCurveOrders(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * orders_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCurveOrdersImpl(session, node_id, part_id, orders_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetCustomString(const HAPI_Session * session, const char * string_value, HAPI_StringHandle * handle_value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetCustomStringImpl(session, string_value, handle_value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetFaceCounts(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * face_counts_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetFaceCountsImpl(session, node_id, part_id, face_counts_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetGroupMembership(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, const int * membership_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetGroupMembershipImpl(session, node_id, part_id, group_type, group_name, membership_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetHeightFieldData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const float * values_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetHeightFieldDataImpl(session, node_id, part_id, name, values_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetImageInfo(const HAPI_Session * session, HAPI_NodeId material_node_id, const HAPI_ImageInfo * image_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetImageInfoImpl(session, material_node_id, image_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetInputCurveInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_InputCurveInfo * info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetInputCurveInfoImpl(session, node_id, part_id, info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetInputCurvePositions(const HAPI_Session* session, HAPI_NodeId node_id, HAPI_PartId part_id, const float* positions_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetInputCurvePositionsImpl(session, node_id, part_id, positions_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScales(const HAPI_Session* session, HAPI_NodeId node_id, HAPI_PartId part_id, const float* positions_array, int positions_start, int positions_length, const float* rotations_array, int rotations_start, int rotations_length, const float * scales_array, int scales_start, int scales_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScalesImpl(session, node_id, part_id, positions_array, positions_start, positions_length, rotations_array, rotations_start, rotations_length, scales_array, scales_start, scales_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetNodeDisplay(const HAPI_Session * session, HAPI_NodeId node_id, int onOff)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetNodeDisplayImpl(session, node_id, onOff);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetObjectTransform(const HAPI_Session * session, HAPI_NodeId node_id, const HAPI_TransformEuler * trans)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetObjectTransformImpl(session, node_id, trans);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetParmExpression(const HAPI_Session * session, HAPI_NodeId node_id, const char * value, HAPI_ParmId parm_id, int index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmExpressionImpl(session, node_id, value, parm_id, index);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetParmFloatValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, float value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmFloatValueImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetParmFloatValues(const HAPI_Session * session, HAPI_NodeId node_id, const float * values_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmFloatValuesImpl(session, node_id, values_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetParmIntValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, int value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmIntValueImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetParmIntValues(const HAPI_Session * session, HAPI_NodeId node_id, const int * values_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmIntValuesImpl(session, node_id, values_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetParmNodeValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_NodeId value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmNodeValueImpl(session, node_id, parm_name, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetParmStringValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * value, HAPI_ParmId parm_id, int index)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetParmStringValueImpl(session, node_id, value, parm_id, index);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetPartInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_PartInfo * part_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetPartInfoImpl(session, node_id, part_id, part_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetPreset(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PresetType preset_type, const char * preset_name, const char * buffer, int buffer_length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetPresetImpl(session, node_id, preset_type, preset_name, buffer, buffer_length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetServerEnvInt(const HAPI_Session * session, const char * variable_name, int value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetServerEnvIntImpl(session, variable_name, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetServerEnvString(const HAPI_Session * session, const char * variable_name, const char * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetServerEnvStringImpl(session, variable_name, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetSessionSync(const HAPI_Session * session, HAPI_Bool enable)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetSessionSyncImpl(session, enable);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetSessionSyncInfo(const HAPI_Session * session, const HAPI_SessionSyncInfo * session_sync_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetSessionSyncInfoImpl(session, session_sync_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetTime(const HAPI_Session * session, float time)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetTimeImpl(session, time);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetTimelineOptions(const HAPI_Session * session, const HAPI_TimelineOptions * timeline_options)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetTimelineOptionsImpl(session, timeline_options);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetTransformAnimCurve(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_TransformComponent trans_comp, const HAPI_Keyframe * curve_keyframes_array, int keyframe_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetTransformAnimCurveImpl(session, node_id, trans_comp, curve_keyframes_array, keyframe_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetUseHoudiniTime(const HAPI_Session * session, HAPI_Bool enabled)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetUseHoudiniTimeImpl(session, enabled);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetVertexList(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * vertex_list_array, int start, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetVertexListImpl(session, node_id, part_id, vertex_list_array, start, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetViewport(const HAPI_Session * session, const HAPI_Viewport * viewport)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetViewportImpl(session, viewport);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetVolumeInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_VolumeInfo * volume_info)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetVolumeInfoImpl(session, node_id, part_id, volume_info);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetVolumeTileFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_VolumeTileInfo * tile, const float * values_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetVolumeTileFloatDataImpl(session, node_id, part_id, tile, values_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetVolumeTileIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_VolumeTileInfo * tile, const int * values_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetVolumeTileIntDataImpl(session, node_id, part_id, tile, values_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetVolumeVoxelFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, const float * values_array, int value_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetVolumeVoxelFloatDataImpl(session, node_id, part_id, x_index, y_index, z_index, values_array, value_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetVolumeVoxelIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, const int * values_array, int value_count)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetVolumeVoxelIntDataImpl(session, node_id, part_id, x_index, y_index, z_index, values_array, value_count);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetWorkItemFloatAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, const float * values_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetWorkItemFloatAttributeImpl(session, node_id, work_item_id, attribute_name, values_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetWorkItemIntAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, const int * values_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetWorkItemIntAttributeImpl(session, node_id, work_item_id, attribute_name, values_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetWorkItemStringAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, int data_index, const char * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetWorkItemStringAttributeImpl(session, node_id, work_item_id, attribute_name, data_index, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetWorkitemFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, const float * values_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetWorkitemFloatDataImpl(session, node_id, workitem_id, data_name, values_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetWorkitemIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, const int * values_array, int length)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetWorkitemIntDataImpl(session, node_id, workitem_id, data_name, values_array, length);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::SetWorkitemStringData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, int data_index, const char * value)
{
    HAPI_Result Result = HEMAX_HoudiniApi::SetWorkitemStringDataImpl(session, node_id, workitem_id, data_name, data_index, value);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::Shutdown(const HAPI_Session * session)
{
    HAPI_Result Result = HEMAX_HoudiniApi::ShutdownImpl(session);
    return HandleHAPIResult(session, Result);
}


bool
HEMAX_HoudiniApi::StartThriftNamedPipeServer(const HAPI_ThriftServerOptions * options, const char * pipe_name, HAPI_ProcessId * process_id, const char * log_file)
{
    HAPI_Result Result = HEMAX_HoudiniApi::StartThriftNamedPipeServerImpl(options, pipe_name, process_id, log_file);
    return HandleHAPIResult(nullptr, Result);
}


bool
HEMAX_HoudiniApi::StartThriftSocketServer(const HAPI_ThriftServerOptions * options, int port, HAPI_ProcessId * process_id, const char * log_file)
{
    HAPI_Result Result = HEMAX_HoudiniApi::StartThriftSocketServerImpl(options, port, process_id, log_file);
    return HandleHAPIResult(nullptr, Result);
}


bool
HEMAX_HoudiniApi::AddAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::AddAttributeImpl(session, node_id, part_id, name, attr_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::AddGroup(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::AddGroupImpl(session, node_id, part_id, group_type, group_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::BindCustomImplementation(HAPI_SessionType session_type, const char * dll_path, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::BindCustomImplementationImpl(session_type, dll_path);
    return HandleHAPIResult(nullptr, result);
}


bool
HEMAX_HoudiniApi::CancelPDGCook(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CancelPDGCookImpl(session, graph_context_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CheckForSpecificErrors(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ErrorCodeBits errors_to_look_for, HAPI_ErrorCodeBits * errors_found, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CheckForSpecificErrorsImpl(session, node_id, errors_to_look_for, errors_found);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::Cleanup(const HAPI_Session * session, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CleanupImpl(session);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ClearConnectionError(HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ClearConnectionErrorImpl();
    return HandleHAPIResult(nullptr, result);
}


bool
HEMAX_HoudiniApi::CloseSession(const HAPI_Session * session, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CloseSessionImpl(session);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CommitGeo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CommitGeoImpl(session, node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CommitWorkItems(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CommitWorkItemsImpl(session, node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CommitWorkitems(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CommitWorkitemsImpl(session, node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ComposeChildNodeList(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_NodeTypeBits node_type_filter, HAPI_NodeFlagsBits node_flags_filter, HAPI_Bool recursive, int * count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ComposeChildNodeListImpl(session, parent_node_id, node_type_filter, node_flags_filter, recursive, count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ComposeNodeCookResult(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_StatusVerbosity verbosity, int * buffer_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ComposeNodeCookResultImpl(session, node_id, verbosity, buffer_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ComposeObjectList(const HAPI_Session * session, HAPI_NodeId parent_node_id, const char * categories, int * object_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ComposeObjectListImpl(session, parent_node_id, categories, object_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ConnectNodeInput(const HAPI_Session * session, HAPI_NodeId node_id, int input_index, HAPI_NodeId node_id_to_connect, int output_index, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ConnectNodeInputImpl(session, node_id, input_index, node_id_to_connect, output_index);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ConvertMatrixToEuler(const HAPI_Session * session, const float * matrix, HAPI_RSTOrder rst_order, HAPI_XYZOrder rot_order, HAPI_TransformEuler * transform_out, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ConvertMatrixToEulerImpl(session, matrix, rst_order, rot_order, transform_out);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ConvertMatrixToQuat(const HAPI_Session * session, const float * matrix, HAPI_RSTOrder rst_order, HAPI_Transform * transform_out, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ConvertMatrixToQuatImpl(session, matrix, rst_order, transform_out);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ConvertTransform(const HAPI_Session * session, const HAPI_TransformEuler * transform_in, HAPI_RSTOrder rst_order, HAPI_XYZOrder rot_order, HAPI_TransformEuler * transform_out, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ConvertTransformImpl(session, transform_in, rst_order, rot_order, transform_out);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ConvertTransformEulerToMatrix(const HAPI_Session * session, const HAPI_TransformEuler * transform, float * matrix, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ConvertTransformEulerToMatrixImpl(session, transform, matrix);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ConvertTransformQuatToMatrix(const HAPI_Session * session, const HAPI_Transform * transform, float * matrix, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ConvertTransformQuatToMatrixImpl(session, transform, matrix);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CookNode(const HAPI_Session * session, HAPI_NodeId node_id, const HAPI_CookOptions * cook_options, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CookNodeImpl(session, node_id, cook_options);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CookPDG(const HAPI_Session * session, HAPI_NodeId cook_node_id, int generate_only, int blocking, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CookPDGImpl(session, cook_node_id, generate_only, blocking);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CookPDGAllOutputs(const HAPI_Session* session, HAPI_NodeId cook_node_id, int generate_only, int blocking, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CookPDGAllOutputsImpl(session, cook_node_id, generate_only, blocking);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateCustomSession(HAPI_SessionType session_type, void * session_info, HAPI_Session * session, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateCustomSessionImpl(session_type, session_info, session);
    return HandleHAPIResult(nullptr, result);
}


bool
HEMAX_HoudiniApi::CreateHeightFieldInput(const HAPI_Session * session, HAPI_NodeId parent_node_id, const char * name, int xsize, int ysize, float voxelsize, HAPI_HeightFieldSampling sampling, HAPI_NodeId * heightfield_node_id, HAPI_NodeId * height_node_id, HAPI_NodeId * mask_node_id, HAPI_NodeId * merge_node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateHeightFieldInputImpl(session, parent_node_id, name, xsize, ysize, voxelsize, sampling, heightfield_node_id, height_node_id, mask_node_id, merge_node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNode(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_NodeId * new_node_id, const char * name, int xsize, int ysize, float voxelsize, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNodeImpl(session, parent_node_id, new_node_id, name, xsize, ysize, voxelsize);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateInProcessSession(HAPI_Session * session, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateInProcessSessionImpl(session);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateInputCurveNode(const HAPI_Session * session, HAPI_NodeId * node_id, const char * name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateInputCurveNodeImpl(session, node_id, name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateInputNode(const HAPI_Session * session, HAPI_NodeId * node_id, const char * name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateInputNodeImpl(session, node_id, name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateNode(const HAPI_Session * session, HAPI_NodeId parent_node_id, const char * operator_name, const char * node_label, HAPI_Bool cook_on_creation, HAPI_NodeId * new_node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateNodeImpl(session, parent_node_id, operator_name, node_label, cook_on_creation, new_node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateThriftNamedPipeSession(HAPI_Session * session, const char * pipe_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateThriftNamedPipeSessionImpl(session, pipe_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateThriftSocketSession(HAPI_Session * session, const char * host_name, int port, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateThriftSocketSessionImpl(session, host_name, port);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateWorkItem(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId * work_item_id, const char * name, int index, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateWorkItemImpl(session, node_id, work_item_id, name, index);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::CreateWorkitem(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId * workitem_id, const char * name, int index, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::CreateWorkitemImpl(session, node_id, workitem_id, name, index);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::DeleteAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::DeleteAttributeImpl(session, node_id, part_id, name, attr_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::DeleteGroup(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::DeleteGroupImpl(session, node_id, part_id, group_type, group_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::DeleteNode(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::DeleteNodeImpl(session, node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::DirtyPDGNode(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Bool clean_results, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::DirtyPDGNodeImpl(session, node_id, clean_results);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::DisconnectNodeInput(const HAPI_Session * session, HAPI_NodeId node_id, int input_index, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::DisconnectNodeInputImpl(session, node_id, input_index);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::DisconnectNodeOutputsAt(const HAPI_Session * session, HAPI_NodeId node_id, int output_index, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::DisconnectNodeOutputsAtImpl(session, node_id, output_index);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ExtractImageToFile(const HAPI_Session * session, HAPI_NodeId material_node_id, const char * image_file_format_name, const char * image_planes, const char * destination_folder_path, const char * destination_file_name, int * destination_file_path, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ExtractImageToFileImpl(session, material_node_id, image_file_format_name, image_planes, destination_folder_path, destination_file_name, destination_file_path);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ExtractImageToMemory(const HAPI_Session * session, HAPI_NodeId material_node_id, const char * image_file_format_name, const char * image_planes, int * buffer_size, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ExtractImageToMemoryImpl(session, material_node_id, image_file_format_name, image_planes, buffer_size);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetActiveCacheCount(const HAPI_Session * session, int * active_cache_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetActiveCacheCountImpl(session, active_cache_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetActiveCacheNames(const HAPI_Session * session, HAPI_StringHandle * cache_names_array, int active_cache_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetActiveCacheNamesImpl(session, cache_names_array, active_cache_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAssetDefinitionParmCounts(const HAPI_Session * session, HAPI_AssetLibraryId library_id, const char * asset_name, int * parm_count, int * int_value_count, int * float_value_count, int * string_value_count, int * choice_value_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAssetDefinitionParmCountsImpl(session, library_id, asset_name, parm_count, int_value_count, float_value_count, string_value_count, choice_value_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAssetDefinitionParmInfos(const HAPI_Session * session, HAPI_AssetLibraryId library_id, const char * asset_name, HAPI_ParmInfo * parm_infos_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAssetDefinitionParmInfosImpl(session, library_id, asset_name, parm_infos_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAssetDefinitionParmValues(const HAPI_Session * session, HAPI_AssetLibraryId library_id, const char * asset_name, int * int_values_array, int int_start, int int_length, float * float_values_array, int float_start, int float_length, HAPI_Bool string_evaluate, HAPI_StringHandle * string_values_array, int string_start, int string_length, HAPI_ParmChoiceInfo * choice_values_array, int choice_start, int choice_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAssetDefinitionParmValuesImpl(session, library_id, asset_name, int_values_array, int_start, int_length, float_values_array, float_start, float_length, string_evaluate, string_values_array, string_start, string_length, choice_values_array, choice_start, choice_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAssetInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_AssetInfo * asset_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAssetInfoImpl(session, node_id, asset_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeFloat64ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, double * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeFloat64ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeFloat64Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, double * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeFloat64DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeFloatArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, float * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeFloatArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, float * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeFloatDataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeOwner owner, HAPI_AttributeInfo * attr_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeInfoImpl(session, node_id, part_id, name, owner, attr_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt16ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_Int16 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeInt16ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt16Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_Int16 * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeInt16DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt64ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_Int64 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeInt64ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt64Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_Int64 * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeInt64DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt8ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_Int8 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeInt8ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeInt8Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_Int8 * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeInt8DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeIntArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeIntArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, int * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeIntDataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeNames(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_AttributeOwner owner, HAPI_StringHandle * attribute_names_array, int count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeNamesImpl(session, node_id, part_id, owner, attribute_names_array, count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeStringArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_StringHandle * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeStringArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeStringData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_StringHandle * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeStringDataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeUInt8ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_UInt8 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeUInt8ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAttributeUInt8Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_UInt8 * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAttributeUInt8DataImpl(session, node_id, part_id, name, attr_info, stride, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAvailableAssetCount(const HAPI_Session * session, HAPI_AssetLibraryId library_id, int * asset_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAvailableAssetCountImpl(session, library_id, asset_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetAvailableAssets(const HAPI_Session * session, HAPI_AssetLibraryId library_id, HAPI_StringHandle * asset_names_array, int asset_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetAvailableAssetsImpl(session, library_id, asset_names_array, asset_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetBoxInfo(const HAPI_Session * session, HAPI_NodeId geo_node_id, HAPI_PartId part_id, HAPI_BoxInfo * box_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetBoxInfoImpl(session, geo_node_id, part_id, box_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetCacheProperty(const HAPI_Session * session, const char * cache_name, HAPI_CacheProperty cache_property, int * property_value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetCachePropertyImpl(session, cache_name, cache_property, property_value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetComposedChildNodeList(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_NodeId * child_node_ids_array, int count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetComposedChildNodeListImpl(session, parent_node_id, child_node_ids_array, count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetComposedNodeCookResult(const HAPI_Session * session, char * string_value, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetComposedNodeCookResultImpl(session, string_value, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetComposedObjectList(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_ObjectInfo * object_infos_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetComposedObjectListImpl(session, parent_node_id, object_infos_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetComposedObjectTransforms(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_RSTOrder rst_order, HAPI_Transform * transform_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetComposedObjectTransformsImpl(session, parent_node_id, rst_order, transform_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetCompositorOptions(const HAPI_Session * session, HAPI_CompositorOptions * compositor_options, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetCompositorOptionsImpl(session, compositor_options);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetConnectionError(char * string_value, int length, HAPI_Bool clear, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetConnectionErrorImpl(string_value, length, clear);
    return HandleHAPIResult(nullptr, result);
}


bool
HEMAX_HoudiniApi::GetConnectionErrorLength(int * buffer_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetConnectionErrorLengthImpl(buffer_length);
    return HandleHAPIResult(nullptr, result);
}


bool
HEMAX_HoudiniApi::GetCookingCurrentCount(const HAPI_Session * session, int * count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetCookingCurrentCountImpl(session, count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetCookingTotalCount(const HAPI_Session * session, int * count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetCookingTotalCountImpl(session, count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetCurveCounts(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * counts_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetCurveCountsImpl(session, node_id, part_id, counts_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetCurveInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_CurveInfo * info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetCurveInfoImpl(session, node_id, part_id, info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetCurveKnots(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float * knots_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetCurveKnotsImpl(session, node_id, part_id, knots_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetCurveOrders(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * orders_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetCurveOrdersImpl(session, node_id, part_id, orders_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetDisplayGeoInfo(const HAPI_Session * session, HAPI_NodeId object_node_id, HAPI_GeoInfo * geo_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetDisplayGeoInfoImpl(session, object_node_id, geo_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroup(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * group_name, int * edge_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroupImpl(session, node_id, part_id, group_name, edge_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetEnvInt(HAPI_EnvIntType int_type, int * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetEnvIntImpl(int_type, value);
    return HandleHAPIResult(nullptr, result);
}


bool
HEMAX_HoudiniApi::GetFaceCounts(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * face_counts_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetFaceCountsImpl(session, node_id, part_id, face_counts_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetFirstVolumeTile(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeTileInfo * tile, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetFirstVolumeTileImpl(session, node_id, part_id, tile);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetGeoInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_GeoInfo * geo_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetGeoInfoImpl(session, node_id, geo_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetGeoSize(const HAPI_Session * session, HAPI_NodeId node_id, const char * format, int * size, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetGeoSizeImpl(session, node_id, format, size);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePart(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * pointGroupCount, int * primitiveGroupCount, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePartImpl(session, node_id, part_id, pointGroupCount, primitiveGroupCount);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetGroupMembership(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, HAPI_Bool * membership_array_all_equal, int * membership_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetGroupMembershipImpl(session, node_id, part_id, group_type, group_name, membership_array_all_equal, membership_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePart(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, HAPI_Bool * membership_array_all_equal, int * membership_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePartImpl(session, node_id, part_id, group_type, group_name, membership_array_all_equal, membership_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetGroupNames(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_GroupType group_type, HAPI_StringHandle * group_names_array, int group_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetGroupNamesImpl(session, node_id, group_type, group_names_array, group_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePart(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, HAPI_StringHandle * group_names_array, int group_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePartImpl(session, node_id, part_id, group_type, group_names_array, group_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetHIPFileNodeCount(const HAPI_Session * session, HAPI_HIPFileId id, int * count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetHIPFileNodeCountImpl(session, id, count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetHIPFileNodeIds(const HAPI_Session * session, HAPI_HIPFileId id, HAPI_NodeId * node_ids, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetHIPFileNodeIdsImpl(session, id, node_ids, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetHandleBindingInfo(const HAPI_Session * session, HAPI_NodeId node_id, int handle_index, HAPI_HandleBindingInfo * handle_binding_infos_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetHandleBindingInfoImpl(session, node_id, handle_index, handle_binding_infos_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetHandleInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_HandleInfo * handle_infos_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetHandleInfoImpl(session, node_id, handle_infos_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetHeightFieldData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float * values_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetHeightFieldDataImpl(session, node_id, part_id, values_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetImageFilePath(const HAPI_Session * session, HAPI_NodeId material_node_id, const char * image_file_format_name, const char * image_planes, const char * destination_folder_path, const char * destination_file_name, HAPI_ParmId texture_parm_id, int * destination_file_path, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetImageFilePathImpl(session, material_node_id, image_file_format_name, image_planes, destination_folder_path, destination_file_name, texture_parm_id, destination_file_path);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetImageInfo(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_ImageInfo * image_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetImageInfoImpl(session, material_node_id, image_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetImageMemoryBuffer(const HAPI_Session * session, HAPI_NodeId material_node_id, char * buffer, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetImageMemoryBufferImpl(session, material_node_id, buffer, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetImagePlaneCount(const HAPI_Session * session, HAPI_NodeId material_node_id, int * image_plane_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetImagePlaneCountImpl(session, material_node_id, image_plane_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetImagePlanes(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_StringHandle * image_planes_array, int image_plane_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetImagePlanesImpl(session, material_node_id, image_planes_array, image_plane_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetInputCurveInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_InputCurveInfo * info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetInputCurveInfoImpl(session, node_id, part_id, info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetInstanceTransformsOnPart(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_RSTOrder rst_order, HAPI_Transform * transforms_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetInstanceTransformsOnPartImpl(session, node_id, part_id, rst_order, transforms_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetInstancedObjectIds(const HAPI_Session * session, HAPI_NodeId object_node_id, HAPI_NodeId * instanced_node_id_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetInstancedObjectIdsImpl(session, object_node_id, instanced_node_id_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetInstancedPartIds(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_PartId * instanced_parts_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetInstancedPartIdsImpl(session, node_id, part_id, instanced_parts_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetInstancerPartTransforms(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_RSTOrder rst_order, HAPI_Transform * transforms_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetInstancerPartTransformsImpl(session, node_id, part_id, rst_order, transforms_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetManagerNodeId(const HAPI_Session * session, HAPI_NodeType node_type, HAPI_NodeId * node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetManagerNodeIdImpl(session, node_type, node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetMaterialInfo(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_MaterialInfo * material_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetMaterialInfoImpl(session, material_node_id, material_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetMaterialNodeIdsOnFaces(const HAPI_Session * session, HAPI_NodeId geometry_node_id, HAPI_PartId part_id, HAPI_Bool * are_all_the_same, HAPI_NodeId * material_ids_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetMaterialNodeIdsOnFacesImpl(session, geometry_node_id, part_id, are_all_the_same, material_ids_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetNextVolumeTile(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeTileInfo * tile, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetNextVolumeTileImpl(session, node_id, part_id, tile);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetNodeFromPath(const HAPI_Session * session, const HAPI_NodeId parent_node_id, const char * path, HAPI_NodeId * node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetNodeFromPathImpl(session, parent_node_id, path, node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetNodeInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeInfo * node_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetNodeInfoImpl(session, node_id, node_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetNodeInputName(const HAPI_Session * session, HAPI_NodeId node_id, int input_idx, HAPI_StringHandle * name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetNodeInputNameImpl(session, node_id, input_idx, name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetNodeOutputName(const HAPI_Session * session, HAPI_NodeId node_id, int output_idx, HAPI_StringHandle * name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetNodeOutputNameImpl(session, node_id, output_idx, name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetNodePath(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeId relative_to_node_id, HAPI_StringHandle * path, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetNodePathImpl(session, node_id, relative_to_node_id, path);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetNumWorkItems(const HAPI_Session * session, HAPI_NodeId node_id, int * num, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetNumWorkItemsImpl(session, node_id, num);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetNumWorkitems(const HAPI_Session * session, HAPI_NodeId node_id, int * num, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetNumWorkitemsImpl(session, node_id, num);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetObjectInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ObjectInfo * object_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetObjectInfoImpl(session, node_id, object_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetObjectTransform(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeId relative_to_node_id, HAPI_RSTOrder rst_order, HAPI_Transform * transform, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetObjectTransformImpl(session, node_id, relative_to_node_id, rst_order, transform);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetOutputGeoCount(const HAPI_Session* session, HAPI_NodeId node_id, int* count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetOutputGeoCountImpl(session, node_id, count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetOutputGeoInfos(const HAPI_Session* session, HAPI_NodeId node_id, HAPI_GeoInfo* geo_infos_array, int count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetOutputGeoInfosImpl(session, node_id, geo_infos_array, count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetOutputNodeId(const HAPI_Session * session, HAPI_NodeId node_id, int output, HAPI_NodeId * output_node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetOutputNodeIdImpl(session, node_id, output, output_node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetPDGEvents(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_PDG_EventInfo * event_array, int length, int * event_count, int * remaining_events, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetPDGEventsImpl(session, graph_context_id, event_array, length, event_count, remaining_events);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetPDGGraphContextId(const HAPI_Session * session, HAPI_NodeId top_node_id, HAPI_PDG_GraphContextId * context_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetPDGGraphContextIdImpl(session, top_node_id, context_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetPDGGraphContexts(const HAPI_Session * session, HAPI_StringHandle * context_names_array, HAPI_PDG_GraphContextId * context_id_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetPDGGraphContextsImpl(session, context_names_array, context_id_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetPDGGraphContextsCount(const HAPI_Session* session, int* num_contexts, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetPDGGraphContextsCountImpl(session, num_contexts);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetPDGState(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, int * pdg_state, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetPDGStateImpl(session, graph_context_id, pdg_state);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParameters(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmInfo * parm_infos_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParametersImpl(session, node_id, parm_infos_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmChoiceLists(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmChoiceInfo * parm_choices_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmChoiceListsImpl(session, node_id, parm_choices_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmExpression(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_StringHandle * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmExpressionImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmFile(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, const char * destination_directory, const char * destination_file_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmFileImpl(session, node_id, parm_name, destination_directory, destination_file_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmFloatValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, float * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmFloatValueImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmFloatValues(const HAPI_Session * session, HAPI_NodeId node_id, float * values_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmFloatValuesImpl(session, node_id, values_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmIdFromName(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_ParmId * parm_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmIdFromNameImpl(session, node_id, parm_name, parm_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, HAPI_ParmInfo * parm_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmInfoImpl(session, node_id, parm_id, parm_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmInfoFromName(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_ParmInfo * parm_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmInfoFromNameImpl(session, node_id, parm_name, parm_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmIntValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, int * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmIntValueImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmIntValues(const HAPI_Session * session, HAPI_NodeId node_id, int * values_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmIntValuesImpl(session, node_id, values_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmNodeValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_NodeId * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmNodeValueImpl(session, node_id, parm_name, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmStringValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_Bool evaluate, HAPI_StringHandle * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmStringValueImpl(session, node_id, parm_name, index, evaluate, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmStringValues(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Bool evaluate, HAPI_StringHandle * values_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmStringValuesImpl(session, node_id, evaluate, values_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmTagName(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int tag_index, HAPI_StringHandle * tag_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmTagNameImpl(session, node_id, parm_id, tag_index, tag_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmTagValue(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, const char * tag_name, HAPI_StringHandle * tag_value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmTagValueImpl(session, node_id, parm_id, tag_name, tag_value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetParmWithTag(const HAPI_Session * session, HAPI_NodeId node_id, const char * tag_name, HAPI_ParmId * parm_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetParmWithTagImpl(session, node_id, tag_name, parm_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetPartInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_PartInfo * part_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetPartInfoImpl(session, node_id, part_id, part_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetPreset(const HAPI_Session * session, HAPI_NodeId node_id, char * buffer, int buffer_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetPresetImpl(session, node_id, buffer, buffer_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetPresetBufLength(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PresetType preset_type, const char * preset_name, int * buffer_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetPresetBufLengthImpl(session, node_id, preset_type, preset_name, buffer_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetServerEnvInt(const HAPI_Session * session, const char * variable_name, int * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetServerEnvIntImpl(session, variable_name, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetServerEnvString(const HAPI_Session * session, const char * variable_name, HAPI_StringHandle * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetServerEnvStringImpl(session, variable_name, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetServerEnvVarCount(const HAPI_Session * session, int * env_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetServerEnvVarCountImpl(session, env_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetServerEnvVarList(const HAPI_Session * session, HAPI_StringHandle * values_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetServerEnvVarListImpl(session, values_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetSessionEnvInt(const HAPI_Session * session, HAPI_SessionEnvIntType int_type, int * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetSessionEnvIntImpl(session, int_type, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetSessionSyncInfo(const HAPI_Session * session, HAPI_SessionSyncInfo * session_sync_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetSessionSyncInfoImpl(session, session_sync_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetSphereInfo(const HAPI_Session * session, HAPI_NodeId geo_node_id, HAPI_PartId part_id, HAPI_SphereInfo * sphere_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetSphereInfoImpl(session, geo_node_id, part_id, sphere_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetStatus(const HAPI_Session * session, HAPI_StatusType status_type, int * status, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetStatusImpl(session, status_type, status);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetStatusString(const HAPI_Session * session, HAPI_StatusType status_type, char * string_value, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetStatusStringImpl(session, status_type, string_value, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetStatusStringBufLength(const HAPI_Session * session, HAPI_StatusType status_type, HAPI_StatusVerbosity verbosity, int * buffer_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetStatusStringBufLengthImpl(session, status_type, verbosity, buffer_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetString(const HAPI_Session * session, HAPI_StringHandle string_handle, char * string_value, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetStringImpl(session, string_handle, string_value, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetStringBatch(const HAPI_Session * session, char * char_buffer, int char_array_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetStringBatchImpl(session, char_buffer, char_array_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetStringBatchSize(const HAPI_Session * session, const int * string_handle_array, int string_handle_count, int * string_buffer_size, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetStringBatchSizeImpl(session, string_handle_array, string_handle_count, string_buffer_size);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetStringBufLength(const HAPI_Session * session, HAPI_StringHandle string_handle, int * buffer_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetStringBufLengthImpl(session, string_handle, buffer_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetSupportedImageFileFormatCount(const HAPI_Session * session, int * file_format_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetSupportedImageFileFormatCountImpl(session, file_format_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetSupportedImageFileFormats(const HAPI_Session * session, HAPI_ImageFileFormat * formats_array, int file_format_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetSupportedImageFileFormatsImpl(session, formats_array, file_format_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetTime(const HAPI_Session * session, float * time, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetTimeImpl(session, time);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetTimelineOptions(const HAPI_Session * session, HAPI_TimelineOptions * timeline_options, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetTimelineOptionsImpl(session, timeline_options);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetTotalCookCount(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeTypeBits node_type_filter, HAPI_NodeFlagsBits node_flags_filter, HAPI_Bool recursive, int * count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetTotalCookCountImpl(session, node_id, node_type_filter, node_flags_filter, recursive, count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetUseHoudiniTime(const HAPI_Session * session, HAPI_Bool * enabled, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetUseHoudiniTimeImpl(session, enabled);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetVertexList(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * vertex_list_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetVertexListImpl(session, node_id, part_id, vertex_list_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetViewport(const HAPI_Session * session, HAPI_Viewport * viewport, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetViewportImpl(session, viewport);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetVolumeBounds(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float * x_min, float * y_min, float * z_min, float * x_max, float * y_max, float * z_max, float * x_center, float * y_center, float * z_center, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetVolumeBoundsImpl(session, node_id, part_id, x_min, y_min, z_min, x_max, y_max, z_max, x_center, y_center, z_center);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetVolumeInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeInfo * volume_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetVolumeInfoImpl(session, node_id, part_id, volume_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetVolumeTileFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float fill_value, const HAPI_VolumeTileInfo * tile, float * values_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetVolumeTileFloatDataImpl(session, node_id, part_id, fill_value, tile, values_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetVolumeTileIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int fill_value, const HAPI_VolumeTileInfo * tile, int * values_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetVolumeTileIntDataImpl(session, node_id, part_id, fill_value, tile, values_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetVolumeVisualInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeVisualInfo * visual_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetVolumeVisualInfoImpl(session, node_id, part_id, visual_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetVolumeVoxelFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, float * values_array, int value_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetVolumeVoxelFloatDataImpl(session, node_id, part_id, x_index, y_index, z_index, values_array, value_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetVolumeVoxelIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, int * values_array, int value_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetVolumeVoxelIntDataImpl(session, node_id, part_id, x_index, y_index, z_index, values_array, value_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkItemAttributeSize(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, int * length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkItemAttributeSizeImpl(session, node_id, work_item_id, attribute_name, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkItemFloatAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, float * data_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkItemFloatAttributeImpl(session, node_id, work_item_id, attribute_name, data_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkItemInfo(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_PDG_WorkItemId work_item_id, HAPI_PDG_WorkItemInfo * work_item_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkItemInfoImpl(session, graph_context_id, work_item_id, work_item_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkItemIntAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, int * data_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkItemIntAttributeImpl(session, node_id, work_item_id, attribute_name, data_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkItemOutputFiles(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, HAPI_PDG_WorkItemOutputFile * resultinfo_array, int resultinfo_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkItemOutputFilesImpl(session, node_id, work_item_id, resultinfo_array, resultinfo_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkItemStringAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, HAPI_StringHandle * data_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkItemStringAttributeImpl(session, node_id, work_item_id, attribute_name, data_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkItems(const HAPI_Session * session, HAPI_NodeId node_id, int * work_item_ids_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkItemsImpl(session, node_id, work_item_ids_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkitemDataLength(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, int * length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkitemDataLengthImpl(session, node_id, workitem_id, data_name, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkitemFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, float * data_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkitemFloatDataImpl(session, node_id, workitem_id, data_name, data_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkitemInfo(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_PDG_WorkItemId workitem_id, HAPI_PDG_WorkItemInfo * workitem_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkitemInfoImpl(session, graph_context_id, workitem_id, workitem_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkitemIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, int * data_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkitemIntDataImpl(session, node_id, workitem_id, data_name, data_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkitemResultInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, HAPI_PDG_WorkItemOutputFile * resultinfo_array, int resultinfo_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkitemResultInfoImpl(session, node_id, workitem_id, resultinfo_array, resultinfo_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkitemStringData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, HAPI_StringHandle * data_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkitemStringDataImpl(session, node_id, workitem_id, data_name, data_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::GetWorkitems(const HAPI_Session * session, HAPI_NodeId node_id, int * workitem_ids_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::GetWorkitemsImpl(session, node_id, workitem_ids_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::Initialize(const HAPI_Session * session, const HAPI_CookOptions * cook_options, HAPI_Bool use_cooking_thread, int cooking_thread_stack_size, const char * houdini_environment_files, const char * otl_search_path, const char * dso_search_path, const char * image_dso_search_path, const char * audio_dso_search_path, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::InitializeImpl(session, cook_options, use_cooking_thread, cooking_thread_stack_size, houdini_environment_files, otl_search_path, dso_search_path, image_dso_search_path, audio_dso_search_path);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::InsertMultiparmInstance(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int instance_position, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::InsertMultiparmInstanceImpl(session, node_id, parm_id, instance_position);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::Interrupt(const HAPI_Session * session, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::InterruptImpl(session);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::IsInitialized(const HAPI_Session * session, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::IsInitializedImpl(session);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::IsNodeValid(const HAPI_Session * session, HAPI_NodeId node_id, int unique_node_id, HAPI_Bool * answer, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::IsNodeValidImpl(session, node_id, unique_node_id, answer);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::IsSessionValid(const HAPI_Session * session, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::IsSessionValidImpl(session);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::LoadAssetLibraryFromFile(const HAPI_Session * session, const char * file_path, HAPI_Bool allow_overwrite, HAPI_AssetLibraryId * library_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::LoadAssetLibraryFromFileImpl(session, file_path, allow_overwrite, library_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::LoadAssetLibraryFromMemory(const HAPI_Session * session, const char * library_buffer, int library_buffer_length, HAPI_Bool allow_overwrite, HAPI_AssetLibraryId * library_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::LoadAssetLibraryFromMemoryImpl(session, library_buffer, library_buffer_length, allow_overwrite, library_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::LoadGeoFromFile(const HAPI_Session * session, HAPI_NodeId node_id, const char * file_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::LoadGeoFromFileImpl(session, node_id, file_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::LoadGeoFromMemory(const HAPI_Session * session, HAPI_NodeId node_id, const char * format, const char * buffer, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::LoadGeoFromMemoryImpl(session, node_id, format, buffer, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::LoadHIPFile(const HAPI_Session * session, const char * file_name, HAPI_Bool cook_on_load, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::LoadHIPFileImpl(session, file_name, cook_on_load);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::LoadNodeFromFile(const HAPI_Session * session, const char * file_name, HAPI_NodeId parent_node_id, const char * node_label, HAPI_Bool cook_on_load, HAPI_NodeId * new_node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::LoadNodeFromFileImpl(session, file_name, parent_node_id, node_label, cook_on_load, new_node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::MergeHIPFile(const HAPI_Session * session, const char * file_name, HAPI_Bool cook_on_load, HAPI_HIPFileId * file_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::MergeHIPFileImpl(session, file_name, cook_on_load, file_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ParmHasExpression(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_Bool * has_expression, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ParmHasExpressionImpl(session, node_id, parm_name, index, has_expression);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ParmHasTag(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, const char * tag_name, HAPI_Bool * has_tag, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ParmHasTagImpl(session, node_id, parm_id, tag_name, has_tag);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::PausePDGCook(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::PausePDGCookImpl(session, graph_context_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::PythonThreadInterpreterLock(const HAPI_Session * session, HAPI_Bool locked, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::PythonThreadInterpreterLockImpl(session, locked);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::QueryNodeInput(const HAPI_Session * session, HAPI_NodeId node_to_query, int input_index, HAPI_NodeId * connected_node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::QueryNodeInputImpl(session, node_to_query, input_index, connected_node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::QueryNodeOutputConnectedCount(const HAPI_Session * session, HAPI_NodeId node_id, int output_idx, HAPI_Bool into_subnets, HAPI_Bool through_dots, int * connected_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::QueryNodeOutputConnectedCountImpl(session, node_id, output_idx, into_subnets, through_dots, connected_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::QueryNodeOutputConnectedNodes(const HAPI_Session * session, HAPI_NodeId node_id, int output_idx, HAPI_Bool into_subnets, HAPI_Bool through_dots, HAPI_NodeId * connected_node_ids_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::QueryNodeOutputConnectedNodesImpl(session, node_id, output_idx, into_subnets, through_dots, connected_node_ids_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::RemoveCustomString(const HAPI_Session * session, const HAPI_StringHandle string_handle, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::RemoveCustomStringImpl(session, string_handle);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::RemoveMultiparmInstance(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int instance_position, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::RemoveMultiparmInstanceImpl(session, node_id, parm_id, instance_position);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::RemoveParmExpression(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int index, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::RemoveParmExpressionImpl(session, node_id, parm_id, index);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::RenameNode(const HAPI_Session * session, HAPI_NodeId node_id, const char * new_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::RenameNodeImpl(session, node_id, new_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::RenderCOPToImage(const HAPI_Session * session, HAPI_NodeId cop_node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::RenderCOPToImageImpl(session, cop_node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::RenderTextureToImage(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_ParmId parm_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::RenderTextureToImageImpl(session, material_node_id, parm_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::ResetSimulation(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ResetSimulationImpl(session, node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::RevertGeo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::RevertGeoImpl(session, node_id);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::RevertParmToDefault(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::RevertParmToDefaultImpl(session, node_id, parm_name, index);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::RevertParmToDefaults(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::RevertParmToDefaultsImpl(session, node_id, parm_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SaveGeoToFile(const HAPI_Session * session, HAPI_NodeId node_id, const char * file_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SaveGeoToFileImpl(session, node_id, file_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SaveGeoToMemory(const HAPI_Session * session, HAPI_NodeId node_id, char * buffer, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SaveGeoToMemoryImpl(session, node_id, buffer, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SaveHIPFile(const HAPI_Session * session, const char * file_path, HAPI_Bool lock_nodes, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SaveHIPFileImpl(session, file_path, lock_nodes);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SaveNodeToFile(const HAPI_Session * session, HAPI_NodeId node_id, const char * file_name, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SaveNodeToFileImpl(session, node_id, file_name);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAnimCurve(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int parm_index, const HAPI_Keyframe * curve_keyframes_array, int keyframe_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAnimCurveImpl(session, node_id, parm_id, parm_index, curve_keyframes_array, keyframe_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeFloat64ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const double * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeFloat64ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeFloat64Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const double * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeFloat64DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeFloatArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const float * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeFloatArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const float * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeFloatDataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt16ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int16 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeInt16ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt16Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int16 * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeInt16DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt64ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int64 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeInt64ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt64Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int64 * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeInt64DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt8ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int8 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeInt8ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeInt8Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int8 * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeInt8DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeIntArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const int * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeIntArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const int * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeIntDataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeStringArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const char ** data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeStringArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeStringData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const char ** data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeStringDataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeUInt8ArrayData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_UInt8 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeUInt8ArrayDataImpl(session, node_id, part_id, name, attr_info, data_fixed_array, data_fixed_length, sizes_fixed_array, start, sizes_fixed_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetAttributeUInt8Data(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_UInt8 * data_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetAttributeUInt8DataImpl(session, node_id, part_id, name, attr_info, data_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetCacheProperty(const HAPI_Session * session, const char * cache_name, HAPI_CacheProperty cache_property, int property_value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetCachePropertyImpl(session, cache_name, cache_property, property_value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetCompositorOptions(const HAPI_Session * session, const HAPI_CompositorOptions * compositor_options, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetCompositorOptionsImpl(session, compositor_options);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetCurveCounts(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * counts_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetCurveCountsImpl(session, node_id, part_id, counts_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetCurveInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_CurveInfo * info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetCurveInfoImpl(session, node_id, part_id, info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetCurveKnots(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const float * knots_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetCurveKnotsImpl(session, node_id, part_id, knots_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetCurveOrders(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * orders_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetCurveOrdersImpl(session, node_id, part_id, orders_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetCustomString(const HAPI_Session * session, const char * string_value, HAPI_StringHandle * handle_value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetCustomStringImpl(session, string_value, handle_value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetFaceCounts(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * face_counts_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetFaceCountsImpl(session, node_id, part_id, face_counts_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetGroupMembership(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, const int * membership_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetGroupMembershipImpl(session, node_id, part_id, group_type, group_name, membership_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetHeightFieldData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const float * values_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetHeightFieldDataImpl(session, node_id, part_id, name, values_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetImageInfo(const HAPI_Session * session, HAPI_NodeId material_node_id, const HAPI_ImageInfo * image_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetImageInfoImpl(session, material_node_id, image_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetInputCurveInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_InputCurveInfo * info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetInputCurveInfoImpl(session, node_id, part_id, info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetInputCurvePositions(const HAPI_Session* session, HAPI_NodeId node_id, HAPI_PartId part_id, const float* positions_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetInputCurvePositionsImpl(session, node_id, part_id, positions_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScales(const HAPI_Session* session, HAPI_NodeId node_id, HAPI_PartId part_id, const float* positions_array, int positions_start, int positions_length, const float* rotations_array, int rotations_start, int rotations_length, const float * scales_array, int scales_start, int scales_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScalesImpl(session, node_id, part_id, positions_array, positions_start, positions_length, rotations_array, rotations_start, rotations_length, scales_array, scales_start, scales_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetNodeDisplay(const HAPI_Session * session, HAPI_NodeId node_id, int onOff, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetNodeDisplayImpl(session, node_id, onOff);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetObjectTransform(const HAPI_Session * session, HAPI_NodeId node_id, const HAPI_TransformEuler * trans, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetObjectTransformImpl(session, node_id, trans);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetParmExpression(const HAPI_Session * session, HAPI_NodeId node_id, const char * value, HAPI_ParmId parm_id, int index, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetParmExpressionImpl(session, node_id, value, parm_id, index);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetParmFloatValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, float value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetParmFloatValueImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetParmFloatValues(const HAPI_Session * session, HAPI_NodeId node_id, const float * values_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetParmFloatValuesImpl(session, node_id, values_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetParmIntValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, int value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetParmIntValueImpl(session, node_id, parm_name, index, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetParmIntValues(const HAPI_Session * session, HAPI_NodeId node_id, const int * values_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetParmIntValuesImpl(session, node_id, values_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetParmNodeValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_NodeId value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetParmNodeValueImpl(session, node_id, parm_name, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetParmStringValue(const HAPI_Session * session, HAPI_NodeId node_id, const char * value, HAPI_ParmId parm_id, int index, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetParmStringValueImpl(session, node_id, value, parm_id, index);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetPartInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_PartInfo * part_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetPartInfoImpl(session, node_id, part_id, part_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetPreset(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PresetType preset_type, const char * preset_name, const char * buffer, int buffer_length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetPresetImpl(session, node_id, preset_type, preset_name, buffer, buffer_length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetServerEnvInt(const HAPI_Session * session, const char * variable_name, int value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetServerEnvIntImpl(session, variable_name, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetServerEnvString(const HAPI_Session * session, const char * variable_name, const char * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetServerEnvStringImpl(session, variable_name, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetSessionSync(const HAPI_Session * session, HAPI_Bool enable, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetSessionSyncImpl(session, enable);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetSessionSyncInfo(const HAPI_Session * session, const HAPI_SessionSyncInfo * session_sync_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetSessionSyncInfoImpl(session, session_sync_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetTime(const HAPI_Session * session, float time, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetTimeImpl(session, time);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetTimelineOptions(const HAPI_Session * session, const HAPI_TimelineOptions * timeline_options, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetTimelineOptionsImpl(session, timeline_options);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetTransformAnimCurve(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_TransformComponent trans_comp, const HAPI_Keyframe * curve_keyframes_array, int keyframe_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetTransformAnimCurveImpl(session, node_id, trans_comp, curve_keyframes_array, keyframe_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetUseHoudiniTime(const HAPI_Session * session, HAPI_Bool enabled, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetUseHoudiniTimeImpl(session, enabled);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetVertexList(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * vertex_list_array, int start, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetVertexListImpl(session, node_id, part_id, vertex_list_array, start, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetViewport(const HAPI_Session * session, const HAPI_Viewport * viewport, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetViewportImpl(session, viewport);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetVolumeInfo(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_VolumeInfo * volume_info, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetVolumeInfoImpl(session, node_id, part_id, volume_info);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetVolumeTileFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_VolumeTileInfo * tile, const float * values_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetVolumeTileFloatDataImpl(session, node_id, part_id, tile, values_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetVolumeTileIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_VolumeTileInfo * tile, const int * values_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetVolumeTileIntDataImpl(session, node_id, part_id, tile, values_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetVolumeVoxelFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, const float * values_array, int value_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetVolumeVoxelFloatDataImpl(session, node_id, part_id, x_index, y_index, z_index, values_array, value_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetVolumeVoxelIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, const int * values_array, int value_count, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetVolumeVoxelIntDataImpl(session, node_id, part_id, x_index, y_index, z_index, values_array, value_count);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetWorkItemFloatAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, const float * values_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetWorkItemFloatAttributeImpl(session, node_id, work_item_id, attribute_name, values_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetWorkItemIntAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, const int * values_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetWorkItemIntAttributeImpl(session, node_id, work_item_id, attribute_name, values_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetWorkItemStringAttribute(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, int data_index, const char * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetWorkItemStringAttributeImpl(session, node_id, work_item_id, attribute_name, data_index, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetWorkitemFloatData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, const float * values_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetWorkitemFloatDataImpl(session, node_id, workitem_id, data_name, values_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetWorkitemIntData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, const int * values_array, int length, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetWorkitemIntDataImpl(session, node_id, workitem_id, data_name, values_array, length);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::SetWorkitemStringData(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, int data_index, const char * value, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::SetWorkitemStringDataImpl(session, node_id, workitem_id, data_name, data_index, value);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::Shutdown(const HAPI_Session * session, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::ShutdownImpl(session);
    return HandleHAPIResult(session, result);
}


bool
HEMAX_HoudiniApi::StartThriftNamedPipeServer(const HAPI_ThriftServerOptions * options, const char * pipe_name, HAPI_ProcessId * process_id, const char * log_file, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::StartThriftNamedPipeServerImpl(options, pipe_name, process_id, log_file);
    return HandleHAPIResult(nullptr, result);
}


bool
HEMAX_HoudiniApi::StartThriftSocketServer(const HAPI_ThriftServerOptions * options, int port, HAPI_ProcessId * process_id, const char * log_file, HAPI_Result& result)
{
    result = HEMAX_HoudiniApi::StartThriftSocketServerImpl(options, port, process_id, log_file);
    return HandleHAPIResult(nullptr, result);
}


HAPI_Result
HEMAX_HoudiniApi::AddAttributeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::AddGroupEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::BindCustomImplementationEmptyStub(HAPI_SessionType session_type, const char * dll_path)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CancelPDGCookEmptyStub(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CheckForSpecificErrorsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ErrorCodeBits errors_to_look_for, HAPI_ErrorCodeBits * errors_found)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CleanupEmptyStub(const HAPI_Session * session)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ClearConnectionErrorEmptyStub()
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CloseSessionEmptyStub(const HAPI_Session * session)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CommitGeoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CommitWorkItemsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CommitWorkitemsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ComposeChildNodeListEmptyStub(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_NodeTypeBits node_type_filter, HAPI_NodeFlagsBits node_flags_filter, HAPI_Bool recursive, int * count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ComposeNodeCookResultEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_StatusVerbosity verbosity, int * buffer_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ComposeObjectListEmptyStub(const HAPI_Session * session, HAPI_NodeId parent_node_id, const char * categories, int * object_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ConnectNodeInputEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int input_index, HAPI_NodeId node_id_to_connect, int output_index)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ConvertMatrixToEulerEmptyStub(const HAPI_Session * session, const float * matrix, HAPI_RSTOrder rst_order, HAPI_XYZOrder rot_order, HAPI_TransformEuler * transform_out)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ConvertMatrixToQuatEmptyStub(const HAPI_Session * session, const float * matrix, HAPI_RSTOrder rst_order, HAPI_Transform * transform_out)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ConvertTransformEmptyStub(const HAPI_Session * session, const HAPI_TransformEuler * transform_in, HAPI_RSTOrder rst_order, HAPI_XYZOrder rot_order, HAPI_TransformEuler * transform_out)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ConvertTransformEulerToMatrixEmptyStub(const HAPI_Session * session, const HAPI_TransformEuler * transform, float * matrix)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ConvertTransformQuatToMatrixEmptyStub(const HAPI_Session * session, const HAPI_Transform * transform, float * matrix)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CookNodeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const HAPI_CookOptions * cook_options)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CookPDGEmptyStub(const HAPI_Session * session, HAPI_NodeId cook_node_id, int generate_only, int blocking)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CookPDGAllOutputsEmptyStub(const HAPI_Session* session, HAPI_NodeId cook_node_id, int generate_only, int blocking)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateCustomSessionEmptyStub(HAPI_SessionType session_type, void * session_info, HAPI_Session * session)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateHeightFieldInputEmptyStub(const HAPI_Session * session, HAPI_NodeId parent_node_id, const char * name, int xsize, int ysize, float voxelsize, HAPI_HeightFieldSampling sampling, HAPI_NodeId * heightfield_node_id, HAPI_NodeId * height_node_id, HAPI_NodeId * mask_node_id, HAPI_NodeId * merge_node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateHeightfieldInputVolumeNodeEmptyStub(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_NodeId * new_node_id, const char * name, int xsize, int ysize, float voxelsize)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateInProcessSessionEmptyStub(HAPI_Session * session)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateInputCurveNodeEmptyStub(const HAPI_Session * session, HAPI_NodeId * node_id, const char * name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateInputNodeEmptyStub(const HAPI_Session * session, HAPI_NodeId * node_id, const char * name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateNodeEmptyStub(const HAPI_Session * session, HAPI_NodeId parent_node_id, const char * operator_name, const char * node_label, HAPI_Bool cook_on_creation, HAPI_NodeId * new_node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateThriftNamedPipeSessionEmptyStub(HAPI_Session * session, const char * pipe_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateThriftSocketSessionEmptyStub(HAPI_Session * session, const char * host_name, int port)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateWorkItemEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId * work_item_id, const char * name, int index)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::CreateWorkitemEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId * workitem_id, const char * name, int index)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::DeleteAttributeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::DeleteGroupEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::DeleteNodeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::DirtyPDGNodeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Bool clean_results)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::DisconnectNodeInputEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int input_index)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::DisconnectNodeOutputsAtEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int output_index)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ExtractImageToFileEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, const char * image_file_format_name, const char * image_planes, const char * destination_folder_path, const char * destination_file_name, int * destination_file_path)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ExtractImageToMemoryEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, const char * image_file_format_name, const char * image_planes, int * buffer_size)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetActiveCacheCountEmptyStub(const HAPI_Session * session, int * active_cache_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetActiveCacheNamesEmptyStub(const HAPI_Session * session, HAPI_StringHandle * cache_names_array, int active_cache_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAssetDefinitionParmCountsEmptyStub(const HAPI_Session * session, HAPI_AssetLibraryId library_id, const char * asset_name, int * parm_count, int * int_value_count, int * float_value_count, int * string_value_count, int * choice_value_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAssetDefinitionParmInfosEmptyStub(const HAPI_Session * session, HAPI_AssetLibraryId library_id, const char * asset_name, HAPI_ParmInfo * parm_infos_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAssetDefinitionParmValuesEmptyStub(const HAPI_Session * session, HAPI_AssetLibraryId library_id, const char * asset_name, int * int_values_array, int int_start, int int_length, float * float_values_array, int float_start, int float_length, HAPI_Bool string_evaluate, HAPI_StringHandle * string_values_array, int string_start, int string_length, HAPI_ParmChoiceInfo * choice_values_array, int choice_start, int choice_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAssetInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_AssetInfo * asset_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeFloat64ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, double * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeFloat64DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, double * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeFloatArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, float * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeFloatDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, float * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeOwner owner, HAPI_AttributeInfo * attr_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeInt16ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_Int16 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeInt16DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_Int16 * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeInt64ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_Int64 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeInt64DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_Int64 * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeInt8ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_Int8 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeInt8DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_Int8 * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeIntArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeIntDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, int * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeNamesEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_AttributeOwner owner, HAPI_StringHandle * attribute_names_array, int count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeStringArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_StringHandle * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeStringDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_StringHandle * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeUInt8ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, HAPI_UInt8 * data_fixed_array, int data_fixed_length, int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAttributeUInt8DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, HAPI_AttributeInfo * attr_info, int stride, HAPI_UInt8 * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAvailableAssetCountEmptyStub(const HAPI_Session * session, HAPI_AssetLibraryId library_id, int * asset_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetAvailableAssetsEmptyStub(const HAPI_Session * session, HAPI_AssetLibraryId library_id, HAPI_StringHandle * asset_names_array, int asset_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetBoxInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId geo_node_id, HAPI_PartId part_id, HAPI_BoxInfo * box_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetCachePropertyEmptyStub(const HAPI_Session * session, const char * cache_name, HAPI_CacheProperty cache_property, int * property_value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetComposedChildNodeListEmptyStub(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_NodeId * child_node_ids_array, int count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetComposedNodeCookResultEmptyStub(const HAPI_Session * session, char * string_value, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetComposedObjectListEmptyStub(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_ObjectInfo * object_infos_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetComposedObjectTransformsEmptyStub(const HAPI_Session * session, HAPI_NodeId parent_node_id, HAPI_RSTOrder rst_order, HAPI_Transform * transform_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetCompositorOptionsEmptyStub(const HAPI_Session * session, HAPI_CompositorOptions * compositor_options)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetConnectionErrorEmptyStub(char * string_value, int length, HAPI_Bool clear)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetConnectionErrorLengthEmptyStub(int * buffer_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetCookingCurrentCountEmptyStub(const HAPI_Session * session, int * count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetCookingTotalCountEmptyStub(const HAPI_Session * session, int * count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetCurveCountsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * counts_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetCurveInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_CurveInfo * info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetCurveKnotsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float * knots_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetCurveOrdersEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * orders_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetDisplayGeoInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId object_node_id, HAPI_GeoInfo * geo_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetEdgeCountOfEdgeGroupEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * group_name, int * edge_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetEnvIntEmptyStub(HAPI_EnvIntType int_type, int * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetFaceCountsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * face_counts_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetFirstVolumeTileEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeTileInfo * tile)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetGeoInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_GeoInfo * geo_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetGeoSizeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * format, int * size)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetGroupCountOnPackedInstancePartEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * pointGroupCount, int * primitiveGroupCount)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetGroupMembershipEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, HAPI_Bool * membership_array_all_equal, int * membership_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetGroupMembershipOnPackedInstancePartEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, HAPI_Bool * membership_array_all_equal, int * membership_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetGroupNamesEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_GroupType group_type, HAPI_StringHandle * group_names_array, int group_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetGroupNamesOnPackedInstancePartEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, HAPI_StringHandle * group_names_array, int group_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetHIPFileNodeCountEmptyStub(const HAPI_Session * session, HAPI_HIPFileId id, int * count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetHIPFileNodeIdsEmptyStub(const HAPI_Session * session, HAPI_HIPFileId id, HAPI_NodeId * node_ids, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetHandleBindingInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int handle_index, HAPI_HandleBindingInfo * handle_binding_infos_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetHandleInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_HandleInfo * handle_infos_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetHeightFieldDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float * values_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetImageFilePathEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, const char * image_file_format_name, const char * image_planes, const char * destination_folder_path, const char * destination_file_name, HAPI_ParmId texture_parm_id, int * destination_file_path)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetImageInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_ImageInfo * image_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetImageMemoryBufferEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, char * buffer, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetImagePlaneCountEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, int * image_plane_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetImagePlanesEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_StringHandle * image_planes_array, int image_plane_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetInputCurveInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_InputCurveInfo * info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetInstanceTransformsOnPartEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_RSTOrder rst_order, HAPI_Transform * transforms_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetInstancedObjectIdsEmptyStub(const HAPI_Session * session, HAPI_NodeId object_node_id, HAPI_NodeId * instanced_node_id_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetInstancedPartIdsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_PartId * instanced_parts_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetInstancerPartTransformsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_RSTOrder rst_order, HAPI_Transform * transforms_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetManagerNodeIdEmptyStub(const HAPI_Session * session, HAPI_NodeType node_type, HAPI_NodeId * node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetMaterialInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_MaterialInfo * material_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetMaterialNodeIdsOnFacesEmptyStub(const HAPI_Session * session, HAPI_NodeId geometry_node_id, HAPI_PartId part_id, HAPI_Bool * are_all_the_same, HAPI_NodeId * material_ids_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetNextVolumeTileEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeTileInfo * tile)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetNodeFromPathEmptyStub(const HAPI_Session * session, const HAPI_NodeId parent_node_id, const char * path, HAPI_NodeId * node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetNodeInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeInfo * node_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetNodeInputNameEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int input_idx, HAPI_StringHandle * name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetNodeOutputNameEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int output_idx, HAPI_StringHandle * name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetNodePathEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeId relative_to_node_id, HAPI_StringHandle * path)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetNumWorkItemsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int * num)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetNumWorkitemsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int * num)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetObjectInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ObjectInfo * object_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetObjectTransformEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeId relative_to_node_id, HAPI_RSTOrder rst_order, HAPI_Transform * transform)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetOutputGeoCountEmptyStub(const HAPI_Session* session, HAPI_NodeId node_id, int* count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetOutputGeoInfosEmptyStub(const HAPI_Session* session, HAPI_NodeId node_id, HAPI_GeoInfo* geo_infos_array, int count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetOutputNodeIdEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int output, HAPI_NodeId * output_node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetPDGEventsEmptyStub(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_PDG_EventInfo * event_array, int length, int * event_count, int * remaining_events)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetPDGGraphContextIdEmptyStub(const HAPI_Session * session, HAPI_NodeId top_node_id, HAPI_PDG_GraphContextId * context_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetPDGGraphContextsEmptyStub(const HAPI_Session * session, HAPI_StringHandle * context_names_array, HAPI_PDG_GraphContextId * context_id_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetPDGGraphContextsCountEmptyStub(const HAPI_Session* session, int* num_contexts)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetPDGStateEmptyStub(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, int * pdg_state)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParametersEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmInfo * parm_infos_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmChoiceListsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmChoiceInfo * parm_choices_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmExpressionEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_StringHandle * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmFileEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, const char * destination_directory, const char * destination_file_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmFloatValueEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, float * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmFloatValuesEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, float * values_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmIdFromNameEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_ParmId * parm_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, HAPI_ParmInfo * parm_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmInfoFromNameEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_ParmInfo * parm_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmIntValueEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, int * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmIntValuesEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int * values_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmNodeValueEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_NodeId * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmStringValueEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_Bool evaluate, HAPI_StringHandle * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmStringValuesEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_Bool evaluate, HAPI_StringHandle * values_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmTagNameEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int tag_index, HAPI_StringHandle * tag_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmTagValueEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, const char * tag_name, HAPI_StringHandle * tag_value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetParmWithTagEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * tag_name, HAPI_ParmId * parm_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetPartInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_PartInfo * part_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetPresetEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, char * buffer, int buffer_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetPresetBufLengthEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PresetType preset_type, const char * preset_name, int * buffer_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetServerEnvIntEmptyStub(const HAPI_Session * session, const char * variable_name, int * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetServerEnvStringEmptyStub(const HAPI_Session * session, const char * variable_name, HAPI_StringHandle * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetServerEnvVarCountEmptyStub(const HAPI_Session * session, int * env_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetServerEnvVarListEmptyStub(const HAPI_Session * session, HAPI_StringHandle * values_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetSessionEnvIntEmptyStub(const HAPI_Session * session, HAPI_SessionEnvIntType int_type, int * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetSessionSyncInfoEmptyStub(const HAPI_Session * session, HAPI_SessionSyncInfo * session_sync_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetSphereInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId geo_node_id, HAPI_PartId part_id, HAPI_SphereInfo * sphere_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetStatusEmptyStub(const HAPI_Session * session, HAPI_StatusType status_type, int * status)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetStatusStringEmptyStub(const HAPI_Session * session, HAPI_StatusType status_type, char * string_value, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetStatusStringBufLengthEmptyStub(const HAPI_Session * session, HAPI_StatusType status_type, HAPI_StatusVerbosity verbosity, int * buffer_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetStringEmptyStub(const HAPI_Session * session, HAPI_StringHandle string_handle, char * string_value, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetStringBatchEmptyStub(const HAPI_Session * session, char * char_buffer, int char_array_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetStringBatchSizeEmptyStub(const HAPI_Session * session, const int * string_handle_array, int string_handle_count, int * string_buffer_size)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetStringBufLengthEmptyStub(const HAPI_Session * session, HAPI_StringHandle string_handle, int * buffer_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetSupportedImageFileFormatCountEmptyStub(const HAPI_Session * session, int * file_format_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetSupportedImageFileFormatsEmptyStub(const HAPI_Session * session, HAPI_ImageFileFormat * formats_array, int file_format_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetTimeEmptyStub(const HAPI_Session * session, float * time)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetTimelineOptionsEmptyStub(const HAPI_Session * session, HAPI_TimelineOptions * timeline_options)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetTotalCookCountEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_NodeTypeBits node_type_filter, HAPI_NodeFlagsBits node_flags_filter, HAPI_Bool recursive, int * count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetUseHoudiniTimeEmptyStub(const HAPI_Session * session, HAPI_Bool * enabled)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetVertexListEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int * vertex_list_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetViewportEmptyStub(const HAPI_Session * session, HAPI_Viewport * viewport)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetVolumeBoundsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float * x_min, float * y_min, float * z_min, float * x_max, float * y_max, float * z_max, float * x_center, float * y_center, float * z_center)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetVolumeInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeInfo * volume_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetVolumeTileFloatDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, float fill_value, const HAPI_VolumeTileInfo * tile, float * values_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetVolumeTileIntDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int fill_value, const HAPI_VolumeTileInfo * tile, int * values_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetVolumeVisualInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_VolumeVisualInfo * visual_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetVolumeVoxelFloatDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, float * values_array, int value_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetVolumeVoxelIntDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, int * values_array, int value_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkItemAttributeSizeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, int * length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkItemFloatAttributeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, float * data_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkItemInfoEmptyStub(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_PDG_WorkItemId work_item_id, HAPI_PDG_WorkItemInfo * work_item_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkItemIntAttributeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, int * data_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkItemOutputFilesEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, HAPI_PDG_WorkItemOutputFile * resultinfo_array, int resultinfo_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkItemStringAttributeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, HAPI_StringHandle * data_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkItemsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int * work_item_ids_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkitemDataLengthEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, int * length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkitemFloatDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, float * data_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkitemInfoEmptyStub(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id, HAPI_PDG_WorkItemId workitem_id, HAPI_PDG_WorkItemInfo * workitem_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkitemIntDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, int * data_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkitemResultInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, HAPI_PDG_WorkItemOutputFile * resultinfo_array, int resultinfo_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkitemStringDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, HAPI_StringHandle * data_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::GetWorkitemsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int * workitem_ids_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::InitializeEmptyStub(const HAPI_Session * session, const HAPI_CookOptions * cook_options, HAPI_Bool use_cooking_thread, int cooking_thread_stack_size, const char * houdini_environment_files, const char * otl_search_path, const char * dso_search_path, const char * image_dso_search_path, const char * audio_dso_search_path)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::InsertMultiparmInstanceEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int instance_position)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::InterruptEmptyStub(const HAPI_Session * session)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::IsInitializedEmptyStub(const HAPI_Session * session)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::IsNodeValidEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int unique_node_id, HAPI_Bool * answer)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::IsSessionValidEmptyStub(const HAPI_Session * session)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::LoadAssetLibraryFromFileEmptyStub(const HAPI_Session * session, const char * file_path, HAPI_Bool allow_overwrite, HAPI_AssetLibraryId * library_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::LoadAssetLibraryFromMemoryEmptyStub(const HAPI_Session * session, const char * library_buffer, int library_buffer_length, HAPI_Bool allow_overwrite, HAPI_AssetLibraryId * library_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::LoadGeoFromFileEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * file_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::LoadGeoFromMemoryEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * format, const char * buffer, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::LoadHIPFileEmptyStub(const HAPI_Session * session, const char * file_name, HAPI_Bool cook_on_load)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::LoadNodeFromFileEmptyStub(const HAPI_Session * session, const char * file_name, HAPI_NodeId parent_node_id, const char * node_label, HAPI_Bool cook_on_load, HAPI_NodeId * new_node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::MergeHIPFileEmptyStub(const HAPI_Session * session, const char * file_name, HAPI_Bool cook_on_load, HAPI_HIPFileId * file_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ParmHasExpressionEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, HAPI_Bool * has_expression)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ParmHasTagEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, const char * tag_name, HAPI_Bool * has_tag)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::PausePDGCookEmptyStub(const HAPI_Session * session, HAPI_PDG_GraphContextId graph_context_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::PythonThreadInterpreterLockEmptyStub(const HAPI_Session * session, HAPI_Bool locked)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::QueryNodeInputEmptyStub(const HAPI_Session * session, HAPI_NodeId node_to_query, int input_index, HAPI_NodeId * connected_node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::QueryNodeOutputConnectedCountEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int output_idx, HAPI_Bool into_subnets, HAPI_Bool through_dots, int * connected_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::QueryNodeOutputConnectedNodesEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int output_idx, HAPI_Bool into_subnets, HAPI_Bool through_dots, HAPI_NodeId * connected_node_ids_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::RemoveCustomStringEmptyStub(const HAPI_Session * session, const HAPI_StringHandle string_handle)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::RemoveMultiparmInstanceEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int instance_position)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::RemoveParmExpressionEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int index)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::RenameNodeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * new_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::RenderCOPToImageEmptyStub(const HAPI_Session * session, HAPI_NodeId cop_node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::RenderTextureToImageEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, HAPI_ParmId parm_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ResetSimulationEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::RevertGeoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::RevertParmToDefaultEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::RevertParmToDefaultsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SaveGeoToFileEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * file_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SaveGeoToMemoryEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, char * buffer, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SaveHIPFileEmptyStub(const HAPI_Session * session, const char * file_path, HAPI_Bool lock_nodes)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SaveNodeToFileEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * file_name)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAnimCurveEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_ParmId parm_id, int parm_index, const HAPI_Keyframe * curve_keyframes_array, int keyframe_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeFloat64ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const double * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeFloat64DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const double * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeFloatArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const float * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeFloatDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const float * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeInt16ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int16 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeInt16DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int16 * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeInt64ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int64 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeInt64DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int64 * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeInt8ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int8 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeInt8DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_Int8 * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeIntArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const int * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeIntDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const int * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeStringArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const char ** data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeStringDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const char ** data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeUInt8ArrayDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_UInt8 * data_fixed_array, int data_fixed_length, const int * sizes_fixed_array, int start, int sizes_fixed_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetAttributeUInt8DataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const HAPI_AttributeInfo * attr_info, const HAPI_UInt8 * data_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetCachePropertyEmptyStub(const HAPI_Session * session, const char * cache_name, HAPI_CacheProperty cache_property, int property_value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetCompositorOptionsEmptyStub(const HAPI_Session * session, const HAPI_CompositorOptions * compositor_options)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetCurveCountsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * counts_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetCurveInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_CurveInfo * info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetCurveKnotsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const float * knots_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetCurveOrdersEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * orders_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetCustomStringEmptyStub(const HAPI_Session * session, const char * string_value, HAPI_StringHandle * handle_value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetFaceCountsEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * face_counts_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetGroupMembershipEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, HAPI_GroupType group_type, const char * group_name, const int * membership_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetHeightFieldDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const char * name, const float * values_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetImageInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId material_node_id, const HAPI_ImageInfo * image_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetInputCurveInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_InputCurveInfo * info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetInputCurvePositionsEmptyStub(const HAPI_Session* session, HAPI_NodeId node_id, HAPI_PartId part_id, const float* positions_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetInputCurvePositionsRotationsScalesEmptyStub(const HAPI_Session* session, HAPI_NodeId node_id, HAPI_PartId part_id, const float* positions_array, int positions_start, int positions_length, const float* rotations_array, int rotations_start, int rotations_length, const float * scales_array, int scales_start, int scales_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetNodeDisplayEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, int onOff)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetObjectTransformEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const HAPI_TransformEuler * trans)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetParmExpressionEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * value, HAPI_ParmId parm_id, int index)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetParmFloatValueEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, float value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetParmFloatValuesEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const float * values_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetParmIntValueEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, int index, int value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetParmIntValuesEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const int * values_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetParmNodeValueEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * parm_name, HAPI_NodeId value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetParmStringValueEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, const char * value, HAPI_ParmId parm_id, int index)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetPartInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_PartInfo * part_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetPresetEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PresetType preset_type, const char * preset_name, const char * buffer, int buffer_length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetServerEnvIntEmptyStub(const HAPI_Session * session, const char * variable_name, int value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetServerEnvStringEmptyStub(const HAPI_Session * session, const char * variable_name, const char * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetSessionSyncEmptyStub(const HAPI_Session * session, HAPI_Bool enable)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetSessionSyncInfoEmptyStub(const HAPI_Session * session, const HAPI_SessionSyncInfo * session_sync_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetTimeEmptyStub(const HAPI_Session * session, float time)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetTimelineOptionsEmptyStub(const HAPI_Session * session, const HAPI_TimelineOptions * timeline_options)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetTransformAnimCurveEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_TransformComponent trans_comp, const HAPI_Keyframe * curve_keyframes_array, int keyframe_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetUseHoudiniTimeEmptyStub(const HAPI_Session * session, HAPI_Bool enabled)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetVertexListEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const int * vertex_list_array, int start, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetViewportEmptyStub(const HAPI_Session * session, const HAPI_Viewport * viewport)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetVolumeInfoEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_VolumeInfo * volume_info)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetVolumeTileFloatDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_VolumeTileInfo * tile, const float * values_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetVolumeTileIntDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, const HAPI_VolumeTileInfo * tile, const int * values_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetVolumeVoxelFloatDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, const float * values_array, int value_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetVolumeVoxelIntDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PartId part_id, int x_index, int y_index, int z_index, const int * values_array, int value_count)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetWorkItemFloatAttributeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, const float * values_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetWorkItemIntAttributeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, const int * values_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetWorkItemStringAttributeEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId work_item_id, const char * attribute_name, int data_index, const char * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetWorkitemFloatDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, const float * values_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetWorkitemIntDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, const int * values_array, int length)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::SetWorkitemStringDataEmptyStub(const HAPI_Session * session, HAPI_NodeId node_id, HAPI_PDG_WorkItemId workitem_id, const char * data_name, int data_index, const char * value)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::ShutdownEmptyStub(const HAPI_Session * session)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::StartThriftNamedPipeServerEmptyStub(const HAPI_ThriftServerOptions * options, const char * pipe_name, HAPI_ProcessId * process_id, const char * log_file)
{
    return HAPI_RESULT_FAILURE;
}


HAPI_Result
HEMAX_HoudiniApi::StartThriftSocketServerEmptyStub(const HAPI_ThriftServerOptions * options, int port, HAPI_ProcessId * process_id, const char * log_file)
{
    return HAPI_RESULT_FAILURE;
}


#define GetGeoInfo GetGeoInfoW