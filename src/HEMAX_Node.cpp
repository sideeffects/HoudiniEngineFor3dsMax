#include "HEMAX_Node.h"
#include "HEMAX_SessionManager.h"

HEMAX_Node::HEMAX_Node()
{
    Info.id = -1;
    Type = HEMAX_NODE_INVALID;

    AutoRecookOnParameterUpdate = true;
    RealtimeRecookEnabled = false;
    AutoRecookOnInputChange = false;
}

HEMAX_Node::HEMAX_Node(HEMAX_NodeId NodeId, HEMAX_NodeType NodeType)
{
    Info.id = NodeId;
    Type = NodeType;

    AutoRecookOnParameterUpdate = true;
    RealtimeRecookEnabled = false;
    AutoRecookOnInputChange = false;
}

void
Cook(HEMAX_Node& Node)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    SM.Session->CookNode(Node.Info.id);
    while (SM.Session->IsCookFinished() != COOK_FINISHED)
    {
        // Do nothing
    }
    SM.Session->GetNodeInfo(Node.Info.id, &Node.Info);
}

void
CreateNode(HEMAX_Node& Node, std::string AssetName, bool CookOnCreation)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    Node.AssetName = AssetName;

    bool Result = SM.Session->CreateNode(-1, Node.AssetName.c_str(), nullptr, CookOnCreation, &Node.Info.id);
    Result &= SM.Session->GetNodeInfo(Node.Info.id, &Node.Info);

    if (Result)
    {
        switch (Node.Info.type)
        {
        case (HAPI_NODETYPE_OBJ):
        {
            Node.Type = HEMAX_NODE_OBJ;
            break;
        }
        case (HAPI_NODETYPE_SOP):
        {
            Node.Type = HEMAX_NODE_SOP;
            break;
        }
        default:
        {
            HEMAX_NODE_NOTDEFINED;
            break;
        }
        }
    }
}

void
CreateInputNode(HEMAX_Node& Node, std::string Name)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    SM.Session->CreateInputNode(&Node.Info.id, Name.c_str());
    Cook(Node);
}

void
DeleteNode(HEMAX_Node& Node)
{
    HEMAX_SessionManager::GetSessionManager().Session->DeleteNode(Node.Info.id);
    Node.Type = HEMAX_NODE_INVALID;
}

std::string
GetNodeName(HEMAX_Node& Node)
{
    return HEMAX_SessionManager::GetSessionManager().Session->GetHAPIString(Node.Info.nameSH);
}

std::vector<HEMAX_Parameter>
GetAllParameters(HEMAX_Node& Node)
{
    HEMAX_ParameterInfo* ParmInfos = new HEMAX_ParameterInfo[Node.Info.parmCount];

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    SM.Session->GetParameters(Node.Info.id, ParmInfos, 0, Node.Info.parmCount);

    std::vector<HEMAX_Parameter> Parameters;

    for (int i = 0; i < Node.Info.parmCount; i++)
    {
        Parameters.push_back(HEMAX_Parameter(Node.Info.id, ParmInfos[i]));
    }

    delete[] ParmInfos;

    return Parameters;
}

HEMAX_Parameter
GetParameter(HEMAX_Node& Node, HEMAX_ParameterId ParameterId)
{
    HEMAX_ParameterInfo* ParmInfos = new HEMAX_ParameterInfo[Node.Info.parmCount];

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    SM.Session->GetParameters(Node.Info.id, ParmInfos, 0, Node.Info.parmCount);

    for (int i = 0; i < Node.Info.parmCount; i++)
    {
        if (ParmInfos[i].id == ParameterId)
        {
            HEMAX_Parameter TheParameter(Node.Info.id, ParmInfos[i]);
            delete[] ParmInfos;
            return TheParameter;
        }
    }

    delete[] ParmInfos;

    HEMAX_Parameter InvalidParam;
    return InvalidParam;
}

HEMAX_Parameter
GetParameter(HEMAX_Node& Node, std::string ParameterName)
{
    HEMAX_ParameterInfo* ParmInfos = new HEMAX_ParameterInfo[Node.Info.parmCount];

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    SM.Session->GetParameters(Node.Info.id, ParmInfos, 0, Node.Info.parmCount);

    for (int i = 0; i < Node.Info.parmCount; i++)
    {
        if (SM.Session->GetHAPIString(ParmInfos[i].nameSH) == ParameterName)
        {
            HEMAX_Parameter TheParameter(Node.Info.id, ParmInfos[i]);
            delete[] ParmInfos;
            return TheParameter;
        }
    }

    delete[] ParmInfos;

    HEMAX_Parameter InvalidParam;
    return InvalidParam;
}

std::vector<HEMAX_Parameter>
GetAllMultiParameterInstances(HEMAX_Node& Node, int MultiParameterListId)
{
    std::vector<HEMAX_Parameter> Instances;

    HEMAX_Parameter MultiParameterList = GetParameter(Node, MultiParameterListId);

    std::vector<HEMAX_Parameter> NodeParameters = GetAllParameters(Node);

    for (auto Iter = NodeParameters.begin(); Iter != NodeParameters.end(); ++Iter)
    {
        if ((*Iter).Info.isChildOfMultiParm && (*Iter).Info.parentId == MultiParameterListId)
        {
            Instances.push_back((*Iter));
        }
    }

    return Instances;
}

void
SetObjectTransform(HEMAX_Node& Node, HEMAX_MaxTransform& NewTransform)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetObjectTransform(Node.Info.id, &HEMAX_Utilities::MaxTransformToHAPITransformEuler(NewTransform));
}

void
SetParentTransform(HEMAX_Node& Node, HEMAX_MaxTransform& NewTransform)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetObjectTransform(Node.Info.parentId, &HEMAX_Utilities::MaxTransformToHAPITransformEuler(NewTransform, HEMAX_SRT));
}

std::vector<HEMAX_NodeId>
GetConnectedInputNodeIds(HEMAX_Node& Node)
{
    std::vector<HEMAX_NodeId> InputConnections;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    for (int i = 0; i < Node.Info.inputCount; ++i)
    {
        HEMAX_NodeId ConnectedId;
        SessionManager.Session->QueryNodeInput(Node.Info.id, i, &ConnectedId);

        InputConnections.push_back(ConnectedId);
    }

    return InputConnections;
}

std::vector<std::string>
GetConnectedNodeNames(HEMAX_Node& Node)
{
    std::vector<HEMAX_NodeId> Incoming = GetConnectedInputNodeIds(Node);

    std::vector<std::string> Names;
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    for (int i = 0; i < Incoming.size(); ++i)
    {
        if (Incoming[i] == -1)
        {
            Names.push_back("");
        }
        else
        {
            HEMAX_NodeInfo InputNodeInfo;
            SM.Session->GetNodeInfo(Incoming[i], &InputNodeInfo);

            Names.push_back(SM.Session->GetHAPIString(InputNodeInfo.nameSH));
        }
    }

    return Names;
}

void
ConnectInputNode(HEMAX_Node& Node, HEMAX_NodeId NodeToConnect, int InputIndex)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->ConnectNodeInput(Node.Info.id, InputIndex, NodeToConnect);
}

void
DisconnectInputNode(HEMAX_Node& Node, int InputIndex)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->DisconnectNodeInput(Node.Info.id, InputIndex);
}

HEMAX_NodeId
QueryNodeInput(HEMAX_Node& Node, int InputIndex)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HEMAX_NodeId ConnectedNode;

    if (SessionManager.Session->QueryNodeInput(Node.Info.id, InputIndex, &ConnectedNode))
    {
        return ConnectedNode;
    }
    else
    {
        return -1;
    }
}

void
AddNewPart(HEMAX_Node& Node, HEMAX_PartType PartType, int FaceCount, int VertexCount, int PointCount)
{
    HEMAX_PartInfo NewNodePart;

    NewNodePart.id = 0;
    NewNodePart.nameSH = 0;
    NewNodePart.type = HAPI_PartType(PartType);

    NewNodePart.faceCount = FaceCount;
    NewNodePart.vertexCount = VertexCount;
    NewNodePart.pointCount = PointCount;

    NewNodePart.isInstanced = false;
    NewNodePart.instancedPartCount = 0;
    NewNodePart.instanceCount = 0;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetPartInfo(Node.Info.id, 0, &NewNodePart);
}

HEMAX_AttributeInfo
AddNewPointAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_FLOAT;
    NewNodeAttribute.owner = HAPI_ATTROWNER_POINT;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();
    SessionManager.Session->AddAttribute(Node.Info.id, 0, AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

void
SendPointAttributeData(HEMAX_Node& Node, HEMAX_AttributeInfo AttributeInfo, float* Points, int* Vertices, int* FaceCounts,
    int FaceCount, int VertexCount, int PointCount, std::string AttributeName)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetAttributeFloatData(Node.Info.id, 0, AttributeName.c_str(), &AttributeInfo, Points, 0, PointCount);
    SessionManager.Session->SetVertexList(Node.Info.id, 0, Vertices, 0, VertexCount);
    SessionManager.Session->SetFaceCounts(Node.Info.id, 0, FaceCounts, 0, FaceCount);
}

HEMAX_AttributeInfo
AddNewVertexAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_FLOAT;
    NewNodeAttribute.owner = HAPI_ATTROWNER_VERTEX;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->AddAttribute(Node.Info.id, 0, AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

HEMAX_AttributeInfo
AddNewDetailFloatAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo Attr;
    Attr.count = Count;
    Attr.tupleSize = TupleSize;
    Attr.exists = true;
    Attr.storage = HAPI_STORAGETYPE_FLOAT;
    Attr.owner = HAPI_ATTROWNER_DETAIL;
    Attr.originalOwner = HAPI_ATTROWNER_INVALID;
    Attr.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    SM.Session->AddAttribute(Node.Info.id, 0, AttributeName.c_str(), &Attr);

    return Attr;
}

HEMAX_AttributeInfo
AddNewPrimitiveIntAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_INT;
    NewNodeAttribute.owner = HAPI_ATTROWNER_PRIM;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();
    SessionManager.Session->AddAttribute(Node.Info.id, 0, AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

HEMAX_AttributeInfo
AddNewPrimitiveStringAttribute(HEMAX_Node& Node, int Count, int TupleSize, std::string AttributeName)
{
    HEMAX_AttributeInfo NewNodeAttribute;
    NewNodeAttribute.count = Count;
    NewNodeAttribute.tupleSize = TupleSize;
    NewNodeAttribute.exists = true;
    NewNodeAttribute.storage = HAPI_STORAGETYPE_STRING;
    NewNodeAttribute.owner = HAPI_ATTROWNER_PRIM;
    NewNodeAttribute.originalOwner = HAPI_ATTROWNER_INVALID;
    NewNodeAttribute.typeInfo = HAPI_ATTRIBUTE_TYPE_NONE;

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();
    SessionManager.Session->AddAttribute(Node.Info.id, 0, AttributeName.c_str(), &NewNodeAttribute);

    return NewNodeAttribute;
}

void
SendFloatAttributeData(HEMAX_Node& Node, std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo, float* Data, int Length)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetAttributeFloatData(Node.Info.id, 0, AttributeName.c_str(), &AttributeInfo, Data, 0, Length);
}

void
SendIntAttributeData(HEMAX_Node& Node, std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo, int* Data, int Length)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetAttributeIntData(Node.Info.id, 0, AttributeName.c_str(), &AttributeInfo, Data, 0, Length);
}

void
SendStringAttributeData(HEMAX_Node& Node, std::string AttributeName, HEMAX_AttributeInfo& AttributeInfo, const char** Data, int Length)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->SetAttributeStringData(Node.Info.id, 0, AttributeName.c_str(), &AttributeInfo, Data, 0, Length);
}

void
FinalizeInputGeometry(HEMAX_Node& Node)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    SessionManager.Session->CommitGeometry(Node.Info.id);
}

std::vector<HEMAX_Node>
GetAllChildNodes(HEMAX_Node& Node)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    int ChildNodeCount;
    SM.Session->ComposeChildNodeList(Node.Info.id, HEMAX_NODE_ANY, HEMAX_NODEFLAG_ANY, true, &ChildNodeCount);

    HEMAX_NodeId* ChildNodes = new HEMAX_NodeId[ChildNodeCount];
    SM.Session->GetComposedChildNodeList(Node.Info.id, ChildNodes, ChildNodeCount);

    std::vector<HEMAX_Node> Nodes(ChildNodeCount);

    for (int i = 0; i < ChildNodeCount; ++i)
    {
        Nodes[i].Info.id = ChildNodes[i];
        Cook(Nodes[i]);
    }

    delete[] ChildNodes;

    return Nodes;
}

std::vector<HEMAX_Node>
GetImmediateChildNodes(HEMAX_Node& Node)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    int ChildNodeCount;
    SM.Session->ComposeChildNodeList(Node.Info.id, HEMAX_NODE_ANY, HEMAX_NODEFLAG_ANY, false, &ChildNodeCount);

    HEMAX_NodeId* ChildNodes = new HEMAX_NodeId[ChildNodeCount];
    SM.Session->GetComposedChildNodeList(Node.Info.id, ChildNodes, ChildNodeCount);

    std::vector<HEMAX_Node> Nodes(ChildNodeCount);

    for (int i = 0; i < ChildNodeCount; ++i)
    {
        Nodes[i].Info.id = ChildNodes[i];
        Cook(Nodes[i]);
    }

    delete[] ChildNodes;

    return Nodes;
}