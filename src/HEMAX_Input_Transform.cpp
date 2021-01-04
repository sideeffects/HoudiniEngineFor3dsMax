#include "HEMAX_Input_Transform.h"
#include "HEMAX_SessionManager.h"

HEMAX_Input_Transform::HEMAX_Input_Transform(HEMAX_InputType Type, int Id, ULONG MaxNode) : HEMAX_Input(Type, Id, MaxNode)
{
    HAPI_TransformEuler HAPITransformEuler = BuildTransform();

    CreateInputNode(GetInputNodeName());
    Node->Info.id = Node->Info.parentId;
    Node->Cook();

    MarshalNodeNameDetailAttribute();

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetObjectTransform(Node->Info.id, &HAPITransformEuler);
}

HEMAX_Input_Transform::~HEMAX_Input_Transform()
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    if (SessionManager.IsSessionActive())
    {
	Node->Delete();
    }
}

void
HEMAX_Input_Transform::RebuildAfterChange()
{
    HAPI_TransformEuler HAPITransformEuler = BuildTransform();

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetObjectTransform(Node->Info.id, &HAPITransformEuler);
}

HAPI_TransformEuler
HEMAX_Input_Transform::BuildTransform()
{
    INode* InputNode = GetCOREInterface()->GetINodeByHandle(MaxNodeHandle);

    if (InputNode)
    {
	HAPI_Transform HAPITransform = HEMAX_Utilities::MaxTransformToHAPITransform(HEMAX_Utilities::BuildMaxTransformFromINode(InputNode));

	return HEMAX_Utilities::HAPITransformToHAPITransformEuler(HAPITransform);
    }

    HAPI_TransformEuler DefaultTransform;

    DefaultTransform.position[0] = 0.0f;
    DefaultTransform.position[1] = 0.0f;
    DefaultTransform.position[2] = 0.0f;

    DefaultTransform.rotationEuler[0] = 0.0f;
    DefaultTransform.rotationEuler[1] = 0.0f;
    DefaultTransform.rotationEuler[2] = 0.0f;

    DefaultTransform.scale[0] = 0.0f;
    DefaultTransform.scale[1] = 0.0f;
    DefaultTransform.scale[2] = 0.0f;

    DefaultTransform.shear[0] = 0.0f;
    DefaultTransform.shear[1] = 0.0f;
    DefaultTransform.shear[2] = 0.0f;

    DefaultTransform.rotationOrder = HAPI_XYZORDER_DEFAULT;
    DefaultTransform.rstOrder = HAPI_RSTORDER_DEFAULT;

    return DefaultTransform;
}
