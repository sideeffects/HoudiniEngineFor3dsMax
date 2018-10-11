#include "HEMAX_Undo.h"

HEMAX_ParameterRestore::HEMAX_ParameterRestore(INode* TheNode, std::string ParameterName)
    : Node(TheNode), Name(ParameterName), ParamNodeId(-1), ParamId(-1)
{
}

HEMAX_ParameterRestore::HEMAX_ParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId Param)
    : Node(TheNode), ParamNodeId(NodeId), ParamId(Param), Name("")
{
}

HEMAX_ParameterRestore::~HEMAX_ParameterRestore() {}


void
HEMAX_ParameterRestore::EndHold()
{
    Node->ClearAFlag(A_HELD);
}

int
HEMAX_ParameterRestore::Size()
{
    return (sizeof(Node) + (int)Name.size() + sizeof(ParamNodeId) + sizeof(ParamId));
}

TSTR
HEMAX_ParameterRestore::Description()
{
    return L"HDA Parameter State";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HEMAX_IntParameterRestore::HEMAX_IntParameterRestore(INode* TheNode, std::string ParameterName, std::vector<int> OldIntValues, std::vector<int> NewIntValues)
    : HEMAX_ParameterRestore(TheNode, ParameterName), OldParamValues(OldIntValues), NewParamValues(NewIntValues)
{
}

HEMAX_IntParameterRestore::HEMAX_IntParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<int> OldIntValues, std::vector<int> NewIntValues)
    : HEMAX_ParameterRestore(TheNode, NodeId, ParameterId), OldParamValues(OldIntValues), NewParamValues(NewIntValues)
{
}

void
HEMAX_IntParameterRestore::Restore(int IsUndo)
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().UpdateIntParameterValues(Node, ParamNodeId, ParamId, OldParamValues);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateIntParameterValues(Node, Name, OldParamValues);
    }
}

void
HEMAX_IntParameterRestore::Redo()
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().UpdateIntParameterValues(Node, ParamNodeId, ParamId, NewParamValues);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateIntParameterValues(Node, Name, NewParamValues);
    }
}

int
HEMAX_IntParameterRestore::Size()
{
    return (HEMAX_ParameterRestore::Size() + (int)OldParamValues.size() + (int)NewParamValues.size());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HEMAX_FloatParameterRestore::HEMAX_FloatParameterRestore(INode* TheNode, std::string ParameterName, std::vector<float> OldFloatValues, std::vector<float> NewFloatValues)
    : HEMAX_ParameterRestore(TheNode, ParameterName), OldParamValues(OldFloatValues), NewParamValues(NewFloatValues)
{
}

HEMAX_FloatParameterRestore::HEMAX_FloatParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<float> OldFloatValues, std::vector<float> NewFloatValues)
    : HEMAX_ParameterRestore(TheNode, NodeId, ParameterId), OldParamValues(OldFloatValues), NewParamValues(NewFloatValues)
{
}

void
HEMAX_FloatParameterRestore::Restore(int IsUndo)
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().UpdateFloatParameterValues(Node, ParamNodeId, ParamId, OldParamValues);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateFloatParameterValues(Node, Name, OldParamValues);
    }
}

void
HEMAX_FloatParameterRestore::Redo()
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().UpdateFloatParameterValues(Node, ParamNodeId, ParamId, NewParamValues);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateFloatParameterValues(Node, Name, NewParamValues);
    }
}

int
HEMAX_FloatParameterRestore::Size()
{
    return (HEMAX_ParameterRestore::Size() + (int)OldParamValues.size() + (int)NewParamValues.size());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HEMAX_StringParameterRestore::HEMAX_StringParameterRestore(INode* TheNode, std::string ParameterName, std::vector<std::string> OldStringValues, std::vector<std::string> NewStringValues)
    : HEMAX_ParameterRestore(TheNode, ParameterName), OldParamValues(OldStringValues), NewParamValues(NewStringValues)
{
}

HEMAX_StringParameterRestore::HEMAX_StringParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<std::string> OldStringValues, std::vector<std::string> NewStringValues)
    : HEMAX_ParameterRestore(TheNode, NodeId, ParameterId), OldParamValues(OldStringValues), NewParamValues(NewStringValues)
{
}

void
HEMAX_StringParameterRestore::Restore(int IsUndo)
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().UpdateStringParameterValues(Node, ParamNodeId, ParamId, OldParamValues);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateStringParameterValues(Node, Name, OldParamValues);
    }
}

void
HEMAX_StringParameterRestore::Redo()
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().UpdateStringParameterValues(Node, ParamNodeId, ParamId, NewParamValues);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateStringParameterValues(Node, Name, NewParamValues);
    }
}

int
HEMAX_StringParameterRestore::Size()
{
    return (HEMAX_ParameterRestore::Size() + (int)OldParamValues.size() + (int)NewParamValues.size());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HEMAX_MultiParameterRestore::HEMAX_MultiParameterRestore(INode* TheNode, std::string ParameterName, HEMAX_MultiParameterChangeInfo ChangeInfo)
    : HEMAX_ParameterRestore(TheNode, ParameterName), MultiParamChangeInfo(ChangeInfo)
{
}

HEMAX_MultiParameterRestore::HEMAX_MultiParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, HEMAX_MultiParameterChangeInfo ChangeInfo)
    : HEMAX_ParameterRestore(TheNode, NodeId, ParameterId), MultiParamChangeInfo(ChangeInfo)
{
}

void
HEMAX_MultiParameterRestore::Restore(int IsUndo)
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().UndoMultiParameterChange(Node, ParamNodeId, ParamId, MultiParamChangeInfo);
    }
    else
    {
        HEMAX_Undo::Instance().UndoMultiParameterChange(Node, Name, MultiParamChangeInfo);
    }
}

void
HEMAX_MultiParameterRestore::Redo()
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().RedoMultiParameterChange(Node, ParamNodeId, ParamId, MultiParamChangeInfo);
    }
    else
    {
        HEMAX_Undo::Instance().RedoMultiParameterChange(Node, Name, MultiParamChangeInfo);
    }
}

int
HEMAX_MultiParameterRestore::Size()
{
    return (HEMAX_ParameterRestore::Size() + sizeof(MultiParamChangeInfo));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HEMAX_InputRestore::HEMAX_InputRestore(INode* TheNode, std::string ParameterName, INode* OldInput, INode* NewInput)
    : HEMAX_ParameterRestore(TheNode, ParameterName), OldInputNode(OldInput), NewInputNode(NewInput)
{
}

HEMAX_InputRestore::HEMAX_InputRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, INode* OldInput, INode* NewInput)
    : HEMAX_ParameterRestore(TheNode, NodeId, ParameterId), OldInputNode(OldInput), NewInputNode(NewInput)
{
}

void
HEMAX_InputRestore::Restore(int IsUndo)
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().UpdateInputParameterValue(Node, ParamNodeId, ParamId, OldInputNode);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateInputParameterValue(Node, Name, OldInputNode);
    }
}

void
HEMAX_InputRestore::Redo()
{
    if (ParamNodeId > -1 && ParamId > -1)
    {
        HEMAX_Undo::Instance().UpdateInputParameterValue(Node, ParamNodeId, ParamId, NewInputNode);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateInputParameterValue(Node, Name, NewInputNode);
    }
}

int
HEMAX_InputRestore::Size()
{
    return (HEMAX_ParameterRestore::Size() + sizeof(OldInputNode) + sizeof(NewInputNode));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HEMAX_SubnetworkRestore::HEMAX_SubnetworkRestore(INode* TheNode, int Subnetwork, INode* OldInput, INode* NewInput)
    : HEMAX_ParameterRestore(TheNode, ""), SubnetworkIndex(Subnetwork), OldInputNode(OldInput), NewInputNode(NewInput)
{
}

HEMAX_SubnetworkRestore::HEMAX_SubnetworkRestore(INode* TheNode, HEMAX_NodeId NodeId, int Subnetwork, INode* OldInput, INode* NewInput)
    : HEMAX_ParameterRestore(TheNode, NodeId, -1), SubnetworkIndex(Subnetwork), OldInputNode(OldInput), NewInputNode(NewInput)
{
}

void
HEMAX_SubnetworkRestore::Restore(int IsUndo)
{
    if (ParamNodeId > -1)
    {
        HEMAX_Undo::Instance().UpdateSubnetworkInputValue(Node, ParamNodeId, SubnetworkIndex, OldInputNode);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateSubnetworkInputValue(Node, SubnetworkIndex, OldInputNode);
    }
}

void
HEMAX_SubnetworkRestore::Redo()
{
    if (ParamNodeId > -1)
    {
        HEMAX_Undo::Instance().UpdateSubnetworkInputValue(Node, ParamNodeId, SubnetworkIndex, NewInputNode);
    }
    else
    {
        HEMAX_Undo::Instance().UpdateSubnetworkInputValue(Node, SubnetworkIndex, NewInputNode);
    }
}

int
HEMAX_SubnetworkRestore::Size()
{
    return (HEMAX_ParameterRestore::Size() + sizeof(SubnetworkIndex) + sizeof(OldInputNode) + sizeof(NewInputNode));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HEMAX_Undo&
HEMAX_Undo::Instance()
{
    static HEMAX_Undo UndoInstance;
    return UndoInstance;
}

HEMAX_Undo::HEMAX_Undo()
    : PluginStore(nullptr), PluginUserInterface(nullptr)
{}

HEMAX_Undo::~HEMAX_Undo() {}

void
HEMAX_Undo::ConnectPlugin(HEMAX_Plugin* Plugin)
{
    ThePlugin = Plugin;
}

void
HEMAX_Undo::ConnectPluginStore(HEMAX_Store* Store)
{
    PluginStore = Store;
}

void
HEMAX_Undo::ConnectPluginUserInterface(HEMAX_UI* UserInterface)
{
    PluginUserInterface = UserInterface;
}

void
HEMAX_Undo::UpdateIntParameterValues(INode* Node, std::string ParameterName, std::vector<int>& IntValues)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        theHold.Resume();

        HEMAX_MaxHoudiniAsset* GeoContainer = PluginStore->GetMaxHoudiniAsset(Node->GetHandle());
        GeoContainer->UpdateNodeParameterIntValues(ParameterName, IntValues);
        
        HEMAX_Node* HDANode = GeoContainer->GetNode();

        if (HDANode)
        {
            if (HDANode->AutoRecookOnParameterUpdate)
            {
                GeoContainer->CookHDA();
                ThePlugin->CookMaxHoudiniAsset(GeoContainer->GetNode());
            }
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();

        theHold.Suspend();
    }
    */
}

void
HEMAX_Undo::UpdateFloatParameterValues(INode* Node, std::string ParameterName, std::vector<float>& FloatValues)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        theHold.Resume();

        HEMAX_MaxHoudiniAsset* GeoContainer = PluginStore->GetMaxHoudiniAsset(Node->GetHandle());
        GeoContainer->UpdateNodeParameterFloatValues(ParameterName, FloatValues);

        HEMAX_Node* HDANode = GeoContainer->GetNode();

        if (HDANode)
        {
            if (HDANode->AutoRecookOnParameterUpdate)
            {
                GeoContainer->CookHDA();
                ThePlugin->CookMaxHoudiniAsset(GeoContainer->GetNode());
            }
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();

        theHold.Suspend();
    }
    */
}

void
HEMAX_Undo::UpdateStringParameterValues(INode* Node, std::string ParameterName, std::vector<std::string>& StringValues)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        theHold.Resume();

        HEMAX_MaxHoudiniAsset* GeoContainer = PluginStore->GetMaxHoudiniAsset(Node->GetHandle());
        GeoContainer->UpdateNodeParameterStringValues(ParameterName, StringValues);

        HEMAX_Node* HDANode = GeoContainer->GetNode();

        if (HDANode)
        {
            if (HDANode->AutoRecookOnParameterUpdate)
            {
                GeoContainer->CookHDA();
                ThePlugin->CookMaxHoudiniAsset(GeoContainer->GetNode());
            }
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();

        theHold.Suspend();
    }
    */
}

void
HEMAX_Undo::UpdateInputParameterValue(INode* Node, std::string ParameterName, INode* InputNode)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        theHold.Resume();

        HEMAX_MaxHoudiniAsset* GeoContainer = PluginStore->GetMaxHoudiniAsset(Node->GetHandle());
        
        if (GeoContainer)
        {
            HEMAX_Parameter* TheParameter = GeoContainer->GetNode()->GetParameter(ParameterName);

            if (TheParameter)
            {
                if (InputNode)
                {
                    HEMAX_Input* TheInput = ThePlugin->CreateInputNode(GeoContainer, InputNode->GetHandle(), TheParameter);

                    if (TheInput)
                    {
                        GeoContainer->UpdateInputNode(TheParameter->GetParameterId());
                    }
                }
                else
                {
                    GeoContainer->ClearInputNode(TheParameter->GetParameterId());
                }

                HEMAX_Node* HDANode = GeoContainer->GetNode();

                if (HDANode)
                {
                    if (HDANode->IsAutoRecookEnabled())
                    {
                        GeoContainer->CookHDA();
                        ThePlugin->UpdateRecookedHDA(Node->GetHandle());
                    }
                }
            }
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();

        theHold.Suspend();
    }
    */
}

void
HEMAX_Undo::UpdateSubnetworkInputValue(INode* Node, int Subnetwork, INode* InputNode)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        theHold.Resume();

        HEMAX_MaxHoudiniAsset* GeoContainer = PluginStore->GetMaxHoudiniAsset(Node->GetHandle());

        if (GeoContainer)
        {
            if (InputNode)
            {
                HEMAX_Input* TheInput = ThePlugin->CreateSubnetworkInputNode(InputNode->GetHandle(), GeoContainer, Subnetwork);

                if (TheInput)
                {
                    GeoContainer->UpdateSubnetworkInput(Subnetwork, TheInput);
                }
            }
            else
            {
                GeoContainer->ClearSubnetworkInput(Subnetwork);
            }

            HEMAX_Node* HDANode = GeoContainer->GetNode();

            if (HDANode)
            {
                if (HDANode->IsAutoRecookEnabled())
                {
                    GeoContainer->CookHDA();
                    ThePlugin->UpdateRecookedHDA(Node->GetHandle());
                }
            }
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();

        theHold.Suspend();
    }
    */
}

void
HEMAX_Undo::UndoMultiParameterChange(INode* Node, std::string ParameterName, HEMAX_MultiParameterChangeInfo ChangeInfo)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        theHold.Resume();

        HEMAX_MaxHoudiniAsset* GeoContainer = PluginStore->GetMaxHoudiniAsset(Node->GetHandle());

        HEMAX_Parameter TheParameter = GetParameter(*GeoContainer->GetNode(), ParameterName);

        if (TheParameter.Type != HEMAX_PARAM_INVALID)
        {
            if (ChangeInfo.Added)
            {
                // If the number of params added/removed is greater than 1, then the action has to be occuring at the end of the multi
                // parm list.
                if (ChangeInfo.Count > 1)
                {
                    for (int c = 0; c < ChangeInfo.Count; ++c)
                    {
                        RemoveMultiParameterInstance(TheParameter, TheParameter.Info.instanceCount - 1 - c);
                    }
                }
                else
                {
                    RemoveMultiParameterInstance(TheParameter, ChangeInfo.Position);
                }
            }
            else
            {
                // If the number of params added/removed is greater than 1, then the action has to be occuring at the end of the multi
                // parm list.
                if (ChangeInfo.Count > 1)
                {
                    for (int c = 0; c < ChangeInfo.Count; ++c)
                    {
                        InsertMultiParameterInstance(TheParameter, TheParameter.Info.instanceCount + c);
                    }
                }
                else
                {
                    InsertMultiParameterInstance(TheParameter, ChangeInfo.Position);
                }
            }
        }

        GeoContainer->CookHDA();
        GeoContainer->UpdateMultiParameterList(TheParameter.Info.id);
        HEMAX_Node* HDANode = GeoContainer->GetNode();

        if (HDANode)
        {
            GeoContainer->CookHDA();
            ThePlugin->CookMaxHoudiniAsset(GeoContainer->GetNode());
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();

        theHold.Suspend();
    }
    */
}

void
HEMAX_Undo::RedoMultiParameterChange(INode* Node, std::string ParameterName, HEMAX_MultiParameterChangeInfo ChangeInfo)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        theHold.Resume();

        HEMAX_MaxHoudiniAsset* GeoContainer = PluginStore->GetMaxHoudiniAsset(Node->GetHandle());

        HEMAX_Parameter TheParameter = GetParameter(*GeoContainer->GetNode(), ParameterName);

        if (TheParameter.Type != HEMAX_PARAM_INVALID)
        {
            if (ChangeInfo.Added)
            {
                // If the number of params added/removed is greater than 1, then the action has to be occuring at the end of the multi
                // parm list.
                if (ChangeInfo.Count > 1)
                {
                    for (int c = 0; c < ChangeInfo.Count; ++c)
                    {
                        InsertMultiParameterInstance(TheParameter, TheParameter.Info.instanceCount + c);
                    }
                }
                else
                {
                    InsertMultiParameterInstance(TheParameter, ChangeInfo.Position);
                }
            }
            else
            {
                // If the number of params added/removed is greater than 1, then the action has to be occuring at the end of the multi
                // parm list.
                if (ChangeInfo.Count > 1)
                {
                    for (int c = 0; c < ChangeInfo.Count; ++c)
                    {
                        RemoveMultiParameterInstance(TheParameter, TheParameter.Info.instanceCount - 1 - c);
                    }
                }
                else
                {
                    RemoveMultiParameterInstance(TheParameter, ChangeInfo.Position);
                }
            }
        }

        GeoContainer->CookHDA();
        GeoContainer->UpdateMultiParameterList(TheParameter.Info.id);
        HEMAX_Node* HDANode = GeoContainer->GetNode();

        if (HDANode)
        {
            GeoContainer->CookHDA();
            ThePlugin->CookMaxHoudiniAsset(GeoContainer->GetNode());
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();

        theHold.Suspend();
    }
    */
}

void
HEMAX_Undo::UpdateIntParameterValues(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<int>& IntValues)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        HEMAX_ModifierAsset* ModifierContainer = PluginStore->FindModifierAsset(Node->GetHandle());
        
        if (ModifierContainer)
        {
            ModifierContainer->UpdateNodeParameterIntValues(NodeId, ParameterId, IntValues);

            HEMAX_Modifier* TheModifier = ModifierContainer->SearchModifierStackByNodeId(NodeId);

            if (TheModifier)
            {
                if (TheModifier->GetNode()->AutoRecookOnParameterUpdate)
                {
                    ModifierContainer->CookModifier(NodeId);
                }
            }
            else
            {
                ModifierContainer->CookModifier(NodeId);
            }
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();
    }
    */
}

void
HEMAX_Undo::UpdateFloatParameterValues(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<float>& FloatValues)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        HEMAX_ModifierAsset* ModifierContainer = PluginStore->FindModifierAsset(Node->GetHandle());

        if (ModifierContainer)
        {
            ModifierContainer->UpdateNodeParameterFloatValues(NodeId, ParameterId, FloatValues);
            
            HEMAX_Modifier* TheModifier = ModifierContainer->SearchModifierStackByNodeId(NodeId);

            if (TheModifier)
            {
                if (TheModifier->GetNode()->AutoRecookOnParameterUpdate)
                {
                    ModifierContainer->CookModifier(NodeId);
                }
            }
            else
            {
                ModifierContainer->CookModifier(NodeId);
            }
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();
    }
    */
}

void
HEMAX_Undo::UpdateStringParameterValues(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<std::string>& StringValues)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        HEMAX_ModifierAsset* ModifierContainer = PluginStore->FindModifierAsset(Node->GetHandle());

        if (ModifierContainer)
        {
            ModifierContainer->UpdateNodeParameterStringValues(NodeId, ParameterId, StringValues);
            
            HEMAX_Modifier* TheModifier = ModifierContainer->SearchModifierStackByNodeId(NodeId);

            if (TheModifier)
            {
                if (TheModifier->GetNode()->AutoRecookOnParameterUpdate)
                {
                    ModifierContainer->CookModifier(NodeId);
                }
            }
            else
            {
                ModifierContainer->CookModifier(NodeId);
            }
        }

        PluginUserInterface->HandleParameterUndoRedoEvent();
    }
    */
}

void
HEMAX_Undo::UpdateInputParameterValue(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, INode* InputNode)
{

}

void
HEMAX_Undo::UpdateSubnetworkInputValue(INode* Node, HEMAX_NodeId NodeId, int Subnetwork, INode* InputNode)
{

}

void
HEMAX_Undo::UndoMultiParameterChange(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, HEMAX_MultiParameterChangeInfo ChangeInfo)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        HEMAX_ModifierAsset* ModifierContainer = PluginStore->FindModifierAsset(Node->GetHandle());

        if (ModifierContainer)
        {
            HEMAX_Modifier* TheModifier = ModifierContainer->SearchModifierStackByNodeId(NodeId);

            if (TheModifier)
            {
                HEMAX_Parameter TheParameter = GetParameter(*TheModifier->GetNode(), ParameterId);

                if (TheParameter.Type != HEMAX_PARAM_INVALID)
                {
                    if (ChangeInfo.Added)
                    {
                        // If the number of params added/removed is greater than 1, then the action has to be occuring at the end of the multi
                        // parm list.
                        if (ChangeInfo.Count > 1)
                        {
                            for (int c = 0; c < ChangeInfo.Count; ++c)
                            {
                                RemoveMultiParameterInstance(TheParameter, TheParameter.Info.instanceCount - 1 - c);
                            }
                        }
                        else
                        {
                            RemoveMultiParameterInstance(TheParameter, ChangeInfo.Position);
                        }
                    }
                    else
                    {
                        // If the number of params added/removed is greater than 1, then the action has to be occuring at the end of the multi
                        // parm list.
                        if (ChangeInfo.Count > 1)
                        {
                            for (int c = 0; c < ChangeInfo.Count; ++c)
                            {
                                InsertMultiParameterInstance(TheParameter, TheParameter.Info.instanceCount + c);
                            }
                        }
                        else
                        {
                            InsertMultiParameterInstance(TheParameter, ChangeInfo.Position);
                        }
                    }
                }
            }
            ModifierContainer->CookModifier(NodeId);
            ModifierContainer->UpdateMultiParameterList(NodeId, ParameterId);
            ModifierContainer->CookModifier(NodeId);
        }
        PluginUserInterface->HandleParameterUndoRedoEvent();
    }
    */
}

void
HEMAX_Undo::RedoMultiParameterChange(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, HEMAX_MultiParameterChangeInfo ChangeInfo)
{
    /*
    if (ThePlugin && PluginStore && PluginUserInterface)
    {
        HEMAX_ModifierAsset* ModifierContainer = PluginStore->FindModifierAsset(Node->GetHandle());

        if (ModifierContainer)
        {
            HEMAX_Modifier* TheModifier = ModifierContainer->SearchModifierStackByNodeId(NodeId);

            if (TheModifier)
            {
                HEMAX_Parameter TheParameter = GetParameter(*TheModifier->GetNode(), ParameterId);

                if (TheParameter.Type != HEMAX_PARAM_INVALID)
                {
                    if (ChangeInfo.Added)
                    {
                        // If the number of params added/removed is greater than 1, then the action has to be occuring at the end of the multi
                        // parm list.
                        if (ChangeInfo.Count > 1)
                        {
                            for (int c = 0; c < ChangeInfo.Count; ++c)
                            {
                                InsertMultiParameterInstance(TheParameter, TheParameter.Info.instanceCount + c);
                            }
                        }
                        else
                        {
                            InsertMultiParameterInstance(TheParameter, ChangeInfo.Position);
                        }
                    }
                    else
                    {
                        // If the number of params added/removed is greater than 1, then the action has to be occuring at the end of the multi
                        // parm list.
                        if (ChangeInfo.Count > 1)
                        {
                            for (int c = 0; c < ChangeInfo.Count; ++c)
                            {
                                RemoveMultiParameterInstance(TheParameter, TheParameter.Info.instanceCount - 1 - c);
                            }
                        }
                        else
                        {
                            RemoveMultiParameterInstance(TheParameter, ChangeInfo.Position);
                        }
                    }
                }
            }
            ModifierContainer->CookModifier(NodeId);
            ModifierContainer->UpdateMultiParameterList(NodeId, ParameterId);
            ModifierContainer->CookModifier(NodeId);
        }
        PluginUserInterface->HandleParameterUndoRedoEvent();
    }
    */
}