#pragma once

#include "hold.h"
#include "inode.h"
#include "simpmod.h"
#include <vector>

#include "HEMAX_Store.h"
#include "HEMAX_Plugin.h"

const wchar_t* const HEMAX_UNDO_CREATE_GENERIC = L"Create";
const wchar_t* const HEMAX_UNDO_GEO_HDA_CREATE = L"Create HDA Object";
const wchar_t* const HEMAX_UNDO_GEO_DELETE_NODE = L"Delete HDA Geometry Node";
const wchar_t* const HEMAX_UNDO_GEO_INSTANCE_DELETE = L"Delete HDA Geometry Instance Node";
const wchar_t* const HEMAX_SELECT_HDA = L"Select HDA";
const wchar_t* const HEMAX_HDA_UPDATE_INT_PARAM = L"Change HDA Integer Parameter";
const wchar_t* const HEMAX_HDA_UPDATE_FLOAT_PARAM = L"Change HDA Float Parameter";
const wchar_t* const HEMAX_HDA_UPDATE_STRING_PARAM = L"Change HDA String Parameter";
const wchar_t* const HEMAX_HDA_UPDATE_INPUT_PARAM = L"Change HDA Input Parameter";
const wchar_t* const HEMAX_HDA_UPDATE_MULTI_PARAM = L"Change HDA Multi-Parameter";
const wchar_t* const HEMAX_HDA_UPDATE_SUBNETWORK_INPUT = L"Change Subnetwork Input";
const wchar_t* const HEMAX_MOD_HDA_CREATE = L"Add Modifier";
const wchar_t* const HEMAX_UNDO_GEO_INSTANCE_SOURCE_DELETE = L"Delete HDA Geometry Instance Source";

class HEMAX_ParameterRestore : public RestoreObj
{
public:
    HEMAX_ParameterRestore(INode* TheNode, std::string ParameterName);
    HEMAX_ParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId Param);
    ~HEMAX_ParameterRestore();

    virtual void Restore(int IsUndo) = 0;
    virtual void Redo() = 0;
    virtual int Size();

    void EndHold() override;
    TSTR Description() override;

protected:
    INode* Node;
    std::string Name;

    HEMAX_NodeId ParamNodeId;
    HEMAX_ParameterId ParamId;
};

class HEMAX_IntParameterRestore : public HEMAX_ParameterRestore
{
public:

    HEMAX_IntParameterRestore(INode* TheNode, std::string ParameterName, std::vector<int> OldIntValues, std::vector<int> NewIntValues);
    HEMAX_IntParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<int>OldIntValues, std::vector<int> NewIntValues);

    void Restore(int IsUndo) override;
    void Redo() override;
    int Size() override;

private:

    std::vector<int> OldParamValues;
    std::vector<int> NewParamValues;

};

class HEMAX_FloatParameterRestore : public HEMAX_ParameterRestore
{
public:

    HEMAX_FloatParameterRestore(INode* TheNode, std::string ParameterName, std::vector<float> OldFloatValues, std::vector<float> NewFloatValues);
    HEMAX_FloatParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<float> OldIntValues, std::vector<float> NewFloatValues);

    void Restore(int IsUndo) override;
    void Redo() override;
    int Size() override;

private:

    std::vector<float> OldParamValues;
    std::vector<float> NewParamValues;

};

class HEMAX_StringParameterRestore : public HEMAX_ParameterRestore
{
public:

    HEMAX_StringParameterRestore(INode* TheNode, std::string ParameterName, std::vector<std::string> OldStringValues, std::vector<std::string> NewStringValues);
    HEMAX_StringParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<std::string> OldStringValues, std::vector<std::string> NewStringValues);

    void Restore(int IsUndo) override;
    void Redo() override;
    int Size() override;

private:

    std::vector<std::string> OldParamValues;
    std::vector<std::string> NewParamValues;

};

class HEMAX_MultiParameterRestore : public HEMAX_ParameterRestore
{
public:

    HEMAX_MultiParameterRestore(INode* TheNode, std::string ParameterName, HEMAX_MultiParameterChangeInfo ChangeInfo);
    HEMAX_MultiParameterRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, HEMAX_MultiParameterChangeInfo ChangeInfo);

    void Restore(int IsUndo) override;
    void Redo() override;
    int Size() override;

private:

    HEMAX_MultiParameterChangeInfo MultiParamChangeInfo;
};

class HEMAX_InputRestore : public HEMAX_ParameterRestore
{
public:
    HEMAX_InputRestore(INode* TheNode, std::string ParameterName, INode* OldInput, INode* NewInput);
    HEMAX_InputRestore(INode* TheNode, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, INode* OldInput, INode* NewInput);

    void Restore(int IsUndo) override;
    void Redo() override;
    int Size() override;

private:

    INode* OldInputNode;
    INode* NewInputNode;
};

class HEMAX_SubnetworkRestore : public HEMAX_ParameterRestore
{
public:
    HEMAX_SubnetworkRestore(INode* TheNode, int Subnetwork, INode* OldInput, INode* NewInput);
    HEMAX_SubnetworkRestore(INode* TheNode, HEMAX_NodeId NodeId, int Subnetwork, INode* OldInput, INode* NewInput);

    void Restore(int IsUndo) override;
    void Redo() override;
    int Size() override;

private:

    int SubnetworkIndex;
    INode* OldInputNode;
    INode* NewInputNode;
};

class HEMAX_Undo
{
public:
    static HEMAX_Undo& Instance();
    ~HEMAX_Undo();

    void ConnectPlugin(HEMAX_Plugin* Plugin);
    void ConnectPluginStore(HEMAX_Store* Store);
    void ConnectPluginUserInterface(HEMAX_UI* UserInterface);

    void UpdateIntParameterValues(INode* Node, std::string ParameterName, std::vector<int>& IntValues);
    void UpdateFloatParameterValues(INode* Node, std::string ParameterName, std::vector<float>& FloatValues);
    void UpdateStringParameterValues(INode* Node, std::string ParameterName, std::vector<std::string>& StringValues);
    void UpdateInputParameterValue(INode* Node, std::string ParameterName, INode* InputNode);
    void UpdateSubnetworkInputValue(INode* Node, int Subnetwork, INode* InputNode);

    void UndoMultiParameterChange(INode* Node, std::string ParameterName, HEMAX_MultiParameterChangeInfo ChangeInfo);
    void RedoMultiParameterChange(INode* Node, std::string ParameterName, HEMAX_MultiParameterChangeInfo ChangeInfo);

    void UpdateIntParameterValues(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<int>& IntValues);
    void UpdateFloatParameterValues(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<float>& FloatValues);
    void UpdateStringParameterValues(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, std::vector<std::string>& StringValues);
    void UpdateInputParameterValue(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, INode* InputNode);
    void UpdateSubnetworkInputValue(INode* Node, HEMAX_NodeId NodeId, int Subnetwork, INode* InputNode);

    void UndoMultiParameterChange(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, HEMAX_MultiParameterChangeInfo ChangeInfo);
    void RedoMultiParameterChange(INode* Node, HEMAX_NodeId NodeId, HEMAX_ParameterId ParameterId, HEMAX_MultiParameterChangeInfo ChangeInfo);

private:
    HEMAX_Undo();

    HEMAX_Plugin* ThePlugin;
    HEMAX_Store* PluginStore;
    HEMAX_UI* PluginUserInterface;
};
