#include "HEMAX_Parameter.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"

HEMAX_Parameter::HEMAX_Parameter(const HAPI_NodeId& NodeId, const HAPI_ParmInfo& ParameterInfo)
{
    Node = NodeId;
    Info = ParameterInfo;
    Type = Info.type;
}

HEMAX_Parameter&
HEMAX_Parameter::operator=(const HEMAX_Parameter& Other)
{
    Node = Other.Node;
    Type = Other.Type;
    Info = Other.Info;

    Name = Other.Name;
    Label = Other.Label;
    Help = Other.Help;

    return *this;
}

HEMAX_Parameter::HEMAX_Parameter(const HEMAX_Parameter& Other)
{
    Node = Other.Node;
    Type = Other.Type;
    Info = Other.Info;

    Name = Other.Name;
    Label = Other.Label;
    Help = Other.Help;
}

std::string&
HEMAX_Parameter::GetName()
{
    if (Name == "")
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
	Name = SM.Session->GetHAPIString(Info.nameSH);
    }

    return Name;
}

std::string&
HEMAX_Parameter::GetLabel()
{
    if (Label == "")
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
	Label = SM.Session->GetHAPIString(Info.labelSH);
    }

    return Label;
}

std::string&
HEMAX_Parameter::GetHelp()
{
    if (Help == "" && Info.helpSH != 0)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
	Help = SM.Session->GetHAPIString(Info.helpSH);	
    }

    return Help;
}

std::vector<int>
HEMAX_Parameter::GetIntVals() const
{
    // Do type checking here

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    std::vector<int> IntVals(Info.size);

    HEMAX_HoudiniApi::GetParmIntValues(SM.Session, Node, &IntVals.front(),
	Info.intValuesIndex, Info.size);

    return IntVals;
}

std::vector<float>
HEMAX_Parameter::GetFloatVals() const
{
    // Do type checking here

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    std::vector<float> FloatVals(Info.size);

    HEMAX_HoudiniApi::GetParmFloatValues(SM.Session, Node,
        &FloatVals.front(), Info.floatValuesIndex, Info.size);

    return FloatVals;
}

std::vector<std::string>
HEMAX_Parameter::GetStringVals() const
{
    // Do type checking here
    
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    std::vector<HAPI_StringHandle> Handles(Info.size);
    std::vector<std::string> StringVals(Info.size);

    HEMAX_HoudiniApi::GetParmStringValues(SM.Session, Node, true,
        &Handles.front(), Info.stringValuesIndex, Info.size); 

    for (int i = 0; i < Info.size; i++)
    {
	StringVals[i] = SM.Session->GetHAPIString(Handles[i]);
    }

    return StringVals;
}

std::vector<HEMAX_ParmChoice>
HEMAX_Parameter::GetIntParameterChoiceLists()
{
    // Do type checking here
    
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    std::vector<HAPI_ParmChoiceInfo> Choices(Info.choiceCount);
    std::vector<HEMAX_ParmChoice> ParmChoices(Info.choiceCount);

    HEMAX_HoudiniApi::GetParmChoiceLists(SM.Session, Node, &Choices.front(),
        Info.choiceIndex, Info.choiceCount);

    for (int i = 0; i < Info.choiceCount; i++)
    {
	ParmChoices[i].ChoiceLabel = SM.Session->GetHAPIString(Choices[i].labelSH);
	ParmChoices[i].ChoiceValue = SM.Session->GetHAPIString(Choices[i].valueSH);
    }

    return ParmChoices;
}

std::vector<HEMAX_ParmChoice>
HEMAX_Parameter::GetStringParameterChoiceLists()
{
    // Do some type checking here?
    
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    std::vector<HAPI_ParmChoiceInfo> Choices(Info.choiceCount);
    std::vector<HEMAX_ParmChoice> ParmChoices(Info.choiceCount);

    HEMAX_HoudiniApi::GetParmChoiceLists(SM.Session, Node, &Choices.front(),
        Info.choiceIndex, Info.choiceCount);

    for (int i = 0; i < Info.choiceCount; i++)
    {
	ParmChoices[i].ChoiceLabel = SM.Session->GetHAPIString(Choices[i].labelSH);
	ParmChoices[i].ChoiceValue = SM.Session->GetHAPIString(Choices[i].valueSH);
    }

    return ParmChoices;
}

HAPI_NodeId
HEMAX_Parameter::GetInputNodeId()
{
    if (Type == HAPI_PARMTYPE_NODE)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

	HAPI_NodeId InputNodeId;
	if (HEMAX_HoudiniApi::GetParmNodeValue(SM.Session, Node,
                Name.c_str(), &InputNodeId))
	{
	    return InputNodeId;
	}
    }

    return -1;
}

std::string
HEMAX_Parameter::GetInputNodeName()
{
    std::string InputNodeName = "";

    HAPI_NodeId InputNodeId = GetInputNodeId();

    if (InputNodeId > -1)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
	HAPI_NodeInfo InputNodeInfo;
        if (HEMAX_HoudiniApi::GetNodeInfo(SM.Session, InputNodeId, &InputNodeInfo))
	{
	    InputNodeName = SM.Session->GetHAPIString(InputNodeInfo.nameSH);
	}
    }

    return InputNodeName;
}

void
HEMAX_Parameter::UpdateIntVals(std::vector<int>& Vals)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::SetParmIntValues(SM.Session, Node, &Vals.front(),
        Info.intValuesIndex, Info.size);
}

void
HEMAX_Parameter::UpdateFloatVals(std::vector<float>& Vals)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::SetParmFloatValues(SM.Session, Node, &Vals.front(),
        Info.floatValuesIndex, Info.size);
}

void
HEMAX_Parameter::UpdateStringVals(std::vector<std::string>& Vals)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    for (int i = 0; i < Vals.size(); i++)
    {
        HEMAX_HoudiniApi::SetParmStringValue(SM.Session, Node, Vals[i].c_str(),
	    Info.id, i);
    }
}

void
HEMAX_Parameter::UpdateInputNode(HAPI_NodeId InputNode)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::SetParmNodeValue(SM.Session, Node, Name.c_str(),
        InputNode);
}

bool
HEMAX_Parameter::IsChoiceList()
{
    return (Info.choiceCount > 0);
}

bool
HEMAX_Parameter::IsRootLevel()
{
    return (Info.parentId < 0);
}

bool
HEMAX_Parameter::HasUIConstraints()
{
    return (Info.hasUIMin || Info.hasUIMax);
}

bool
HEMAX_Parameter::IsMultiParameter()
{
    return Type == HAPI_PARMTYPE_MULTIPARMLIST;    
}

int
HEMAX_Parameter::GetInstancePosition(int InstanceStartOffset)
{
    return Info.instanceNum - InstanceStartOffset;
}

int
HEMAX_Parameter::GetInstanceStartOffset()
{
    return Info.instanceStartOffset;
}

void
HEMAX_Parameter::InsertInstance(int Position)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::InsertMultiparmInstance(SM.Session, Node, Info.id,
		    Position + GetInstanceStartOffset());
}

void
HEMAX_Parameter::RemoveInstance(int Position)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HEMAX_HoudiniApi::RemoveMultiparmInstance(SM.Session, Node, Info.id,
		    Position + GetInstanceStartOffset());
}

void
HEMAX_Parameter::CopyValuesFrom(const HEMAX_Parameter& Other)
{
    switch (Other.Type)
    {
	case HAPI_PARMTYPE_INT:
	case HAPI_PARMTYPE_TOGGLE:
	{
	    std::vector<int> Values = Other.GetIntVals();
	    UpdateIntVals(Values);
	} break;
	case HAPI_PARMTYPE_FLOAT:
	case HAPI_PARMTYPE_COLOR:
	{
	    std::vector<float> Values = Other.GetFloatVals();
	    UpdateFloatVals(Values);
	} break;
	case HAPI_PARMTYPE_STRING:
	case HAPI_PARMTYPE_PATH_FILE:
	case HAPI_PARMTYPE_PATH_FILE_DIR:
	case HAPI_PARMTYPE_PATH_FILE_GEO:
	case HAPI_PARMTYPE_PATH_FILE_IMAGE:
	{
	    std::vector<std::string> Values = Other.GetStringVals();
	    UpdateStringVals(Values);
	} break;
	case HAPI_PARMTYPE_MULTIPARMLIST:
	{
	    int InstCount = Info.instanceCount;
	    int OtherInstCount = Other.Info.instanceCount;

	    if (OtherInstCount > InstCount)
	    {
		while (InstCount != OtherInstCount)
		{
		    InsertInstance(InstCount++);
		}
	    }
	    else if (InstCount > OtherInstCount)
	    {
		while (InstCount != OtherInstCount)
		{
		    RemoveInstance(InstCount - 1);
		    InstCount--;
		}
	    }

	} break;
	default:
	{
	    // Nothing to do
	} break;
    }

}

