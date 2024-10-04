#include "HEMAX_InputInstance.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_Input.h"
#include "HEMAX_SessionManager.h"

const char* const HEMAX_INPUT_MERGE_ASSET_NAME("Sop/merge");
const char* const HEMAX_INPUT_MERGE_XFORM_PARM_NAME("xformtype");

HEMAX_InputInstance::HEMAX_InputInstance(const HEMAX_Node& Owner,
        int Subnetwork)
{
    MergeNode.Init(HEMAX_INPUT_MERGE_ASSET_NAME);
    MergeNode.Cook();
    InputUsage.OwnerNode = Owner;
    InputUsage.Subnetwork = Subnetwork;
}

HEMAX_InputInstance::HEMAX_InputInstance(const HEMAX_Node& Owner,
        const std::string& ParameterName)
{
    MergeNode.Init(HEMAX_INPUT_MERGE_ASSET_NAME);
    MergeNode.Cook();
    InputUsage.OwnerNode = Owner;
    InputUsage.ParameterName = ParameterName;
}

HEMAX_InputInstance::~HEMAX_InputInstance()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::DeleteNode(&SM.Session, MergeNode.Info.parentId);

    for (auto&& Input : MaxInputs)
    {
        Input->RemoveUsage(InputUsage);

        if (Input->GetUsageCount() <= 0)
        {
            HEMAX_SessionManager::GetSessionManager().GetStore()
                .Delete3dsMaxInput(Input->Get3dsMaxNodeHandle());
        }
    }
}

void
HEMAX_InputInstance::MergeInput(HEMAX_3dsMaxInput* Input)
{
    HEMAX_Input* InputNode = Input->GetInputNode();
    MergeNode.ConnectInputNode(InputNode->GetInputNodeId(), MergeCount);
    HAPI_NodeId ConnectedNode = MergeNode.QueryNodeInput(MergeCount);
    if (ConnectedNode != -1)
    {
	HEMAX_Node ObjMergeNode(ConnectedNode, HAPI_NODETYPE_SOP);
	ObjMergeNode.Cook();
	HEMAX_Parameter* XformParm = ObjMergeNode.GetParameter(
            HEMAX_INPUT_MERGE_XFORM_PARM_NAME);

	if (XformParm)
	{
            // Parameter Transform=Into This Object
	    std::vector<int> IntValues = {1};
	    XformParm->UpdateIntVals(IntValues);
	}
    }

    Input->AddUsage(InputUsage);
    MaxInputs.push_back(Input);
    MergeCount++;
}

void
HEMAX_InputInstance::MergeInputRelativeTo(HEMAX_3dsMaxInput* Input,
        const HEMAX_MaxTransform& MaxTransform)
{
    HAPI_Session* Session =
        &HEMAX_SessionManager::GetSessionManager().Session;

    HAPI_NodeId XformNodeId = -1;
    HEMAX_HoudiniApi::CreateNode(
        Session, MergeNode.Info.parentId, "xform", nullptr, false,
        &XformNodeId);
    HAPI_TransformEuler EulerXform =
       HEMAX_Utilities::MaxTransformToHAPITransformEuler(MaxTransform,
            HAPI_SRT); 

    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "tx", 0, EulerXform.position[0]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "ty", 1, EulerXform.position[1]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "tz", 2, EulerXform.position[2]);

    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "rx", 0, EulerXform.rotationEuler[0]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "ry", 1, EulerXform.rotationEuler[1]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "rz", 2, EulerXform.rotationEuler[2]);

    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "sx", 0, EulerXform.scale[0]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "sy", 1, EulerXform.scale[1]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "sz", 2, EulerXform.scale[2]);

    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "shear1", 0, EulerXform.shear[0]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "shear2", 1, EulerXform.shear[1]);
    HEMAX_HoudiniApi::SetParmFloatValue(Session, XformNodeId,
        "shear3", 2, EulerXform.shear[2]);

    HEMAX_HoudiniApi::ConnectNodeInput(
        Session, MergeNode.Info.id, MergeCount, XformNodeId, 0);
    HEMAX_Input* InputNode = Input->GetInputNode();
    HEMAX_HoudiniApi::ConnectNodeInput(
        Session, XformNodeId, 0, InputNode->GetInputNodeId(), 0);

    HAPI_NodeId ConnectedNode = -1;
    HEMAX_HoudiniApi::QueryNodeInput(Session, XformNodeId, 0,
            &ConnectedNode);

    HEMAX_Node ObjMergeNode(ConnectedNode, HAPI_NODETYPE_SOP);
    ObjMergeNode.Cook();
    HEMAX_Parameter* XformParm = ObjMergeNode.GetParameter(
        HEMAX_INPUT_MERGE_XFORM_PARM_NAME);

    if (XformParm)
    {
        std::vector<int> IntValues = {0};
        XformParm->UpdateIntVals(IntValues);
    }

    Input->AddUsage(InputUsage);
    MaxInputs.push_back(Input);
    MergeCount++;
}

void
HEMAX_InputInstance::RefreshConnection()
{
    for (int i = MergeCount-1; i >= 0; --i)
    {
        MaxInputs[i]->RemoveUsage(InputUsage);
        MergeNode.DisconnectInputNode(i);
    }

    MergeCount = 0;
    std::vector<HEMAX_3dsMaxInput*> MaxInputsCache = MaxInputs;
    MaxInputs.clear();

    for (auto&& Input: MaxInputsCache)
    {
        if (!Input)
            continue;

        MergeInput(Input);
    }
}

HEMAX_Node&
HEMAX_InputInstance::GetMergedInputs()
{
    return MergeNode;
}

void
HEMAX_InputInstance::RemoveInput(HEMAX_3dsMaxInput* Input)
{
    for (std::vector<HEMAX_3dsMaxInput*>::size_type i = 0; i < MaxInputs.size(); ++i)
    {
        if (Input != MaxInputs[i])
            continue;

        HAPI_NodeId InputXformNode = MergeNode.QueryNodeInput(i);

        if (InputXformNode == -1)
            continue;

        HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
        HAPI_NodeId InputMergeNode = -1;

        HEMAX_HoudiniApi::DeleteNode(&SM.Session, InputXformNode);

        MaxInputs.erase(MaxInputs.begin() + i);
        --MergeCount;

        Input->RemoveUsage(InputUsage);
        if (Input->GetUsageCount() <= 0)
            SM.GetStore().Delete3dsMaxInput(Input->Get3dsMaxNodeHandle());
    }
}
