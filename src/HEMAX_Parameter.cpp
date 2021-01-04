#include "HEMAX_Parameter.h"

#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"

HEMAX_Parameter::HEMAX_Parameter()
{
    Node = -1;
    Type = HEMAX_PARAM_INVALID;
}

HEMAX_Parameter::HEMAX_Parameter(const HEMAX_NodeId& NodeId, const HEMAX_ParameterInfo& ParameterInfo)
{
    Node = NodeId;
    Info = ParameterInfo;

    switch (Info.type)
    {
	case (HAPI_PARMTYPE_INT): Type = HEMAX_PARAM_INTEGER; break;
	case (HAPI_PARMTYPE_STRING): Type = HEMAX_PARAM_STRING; break;
	case (HAPI_PARMTYPE_FLOAT): Type = HEMAX_PARAM_FLOAT; break;
	case (HAPI_PARMTYPE_MULTIPARMLIST): Type = HEMAX_PARAM_MULTIPARMLIST; break;
	case (HAPI_PARMTYPE_TOGGLE): Type = HEMAX_PARAM_TOGGLE; break;
	case (HAPI_PARMTYPE_BUTTON): Type = HEMAX_PARAM_BUTTON; break;
	case (HAPI_PARMTYPE_COLOR): Type = HEMAX_PARAM_COLOR; break;
	case (HAPI_PARMTYPE_PATH_FILE): Type = HEMAX_PARAM_PATH_FILE; break;
	case (HAPI_PARMTYPE_PATH_FILE_DIR): Type = HEMAX_PARAM_PATH_FILE_DIR; break;
	case (HAPI_PARMTYPE_PATH_FILE_GEO): Type = HEMAX_PARAM_PATH_FILE_GEO; break;
	case (HAPI_PARMTYPE_PATH_FILE_IMAGE): Type = HEMAX_PARAM_PATH_FILE_IMAGE; break;
	case (HAPI_PARMTYPE_NODE): Type = HEMAX_PARAM_NODE; break;
	case (HAPI_PARMTYPE_FOLDERLIST): Type = HEMAX_PARAM_FOLDERLIST; break;
	case (HAPI_PARMTYPE_FOLDERLIST_RADIO): Type = HEMAX_PARAM_FOLDERLIST_RADIO; break;
	case (HAPI_PARMTYPE_FOLDER): Type = HEMAX_PARAM_FOLDER; break;
	case (HAPI_PARMTYPE_LABEL): Type = HEMAX_PARAM_LABEL; break;
	case (HAPI_PARMTYPE_SEPARATOR): Type = HEMAX_PARAM_SEPARATOR; break;
	default: Type = HEMAX_PARAM_NOTDEFINED; break;
    }   
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

    CopyValuesFrom(Other);

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

    CopyValuesFrom(Other);
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

    SM.Session->GetParameterIntValues(Node, &IntVals.front(),
		    Info.intValuesIndex, Info.size);

    return IntVals;
}

std::vector<float>
HEMAX_Parameter::GetFloatVals() const
{
    // Do type checking here

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    std::vector<float> FloatVals(Info.size);

    SM.Session->GetParameterFloatValues(Node, &FloatVals.front(),
		    Info.floatValuesIndex, Info.size);

    return FloatVals;
}

std::vector<std::string>
HEMAX_Parameter::GetStringVals() const
{
    // Do type checking here
    
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    std::vector<HEMAX_StringHandle> Handles(Info.size);
    std::vector<std::string> StringVals(Info.size);

    SM.Session->GetParameterStringValues(Node, true, &Handles.front(),
		    Info.stringValuesIndex, Info.size); 

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

    std::vector<HEMAX_ParameterChoiceInfo> Choices(Info.choiceCount);
    std::vector<HEMAX_ParmChoice> ParmChoices(Info.choiceCount);

    SM.Session->GetParameterChoiceLists(Node, &Choices.front(),
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

    std::vector<HEMAX_ParameterChoiceInfo> Choices(Info.choiceCount);
    std::vector<HEMAX_ParmChoice> ParmChoices(Info.choiceCount);

    SM.Session->GetParameterChoiceLists(Node, &Choices.front(),
		    Info.choiceIndex, Info.choiceCount);

    for (int i = 0; i < Info.choiceCount; i++)
    {
	ParmChoices[i].ChoiceLabel = SM.Session->GetHAPIString(Choices[i].labelSH);
	ParmChoices[i].ChoiceValue = SM.Session->GetHAPIString(Choices[i].valueSH);
    }

    return ParmChoices;
}

HEMAX_NodeId
HEMAX_Parameter::GetInputNodeId()
{
    if (Type == HEMAX_PARAM_NODE)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

	HEMAX_NodeId InputNodeId;
	if (SM.Session->GetParameterNodeValue(Node, Name.c_str(), &InputNodeId))
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

    HEMAX_NodeId InputNodeId = GetInputNodeId();

    if (InputNodeId > -1)
    {
	HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
	HEMAX_NodeInfo InputNodeInfo;
	if (SM.Session->GetNodeInfo(InputNodeId, &InputNodeInfo))
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
    SM.Session->SetParameterIntValues(Node, &Vals.front(),
		    Info.intValuesIndex, Info.size);
}

void
HEMAX_Parameter::UpdateFloatVals(std::vector<float>& Vals)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    SM.Session->SetParameterFloatValues(Node, &Vals.front(),
		    Info.floatValuesIndex, Info.size);
}

void
HEMAX_Parameter::UpdateStringVals(std::vector<std::string>& Vals)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    for (int i = 0; i < Vals.size(); i++)
    {
	SM.Session->SetParameterStringValue(Node, Vals[i].c_str(),
			Info.id, i);
    }
}

void
HEMAX_Parameter::UpdateInputNode(HEMAX_NodeId InputNode)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    SM.Session->SetParameterNodeValue(Node, Name.c_str(), InputNode);
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
    return Type == HEMAX_PARAM_MULTIPARMLIST;    
}

int
HEMAX_Parameter::GetInstancePosition()
{
    return Info.instanceNum - 1;
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
    SM.Session->InsertMultiParameterInstance(Node, Info.id,
		    Position + GetInstanceStartOffset());
}

void
HEMAX_Parameter::RemoveInstance(int Position)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    SM.Session->RemoveMultiParameterInstance(Node, Info.id,
		    Position + GetInstanceStartOffset());
}

void
HEMAX_Parameter::CopyValuesFrom(const HEMAX_Parameter& Other)
{
    switch (Other.Type)
    {
	case HEMAX_PARAM_INTEGER:
	case HEMAX_PARAM_TOGGLE:
	{
	    std::vector<int> Values = Other.GetIntVals();
	    UpdateIntVals(Values);
	} break;
	case HEMAX_PARAM_FLOAT:
	case HEMAX_PARAM_COLOR:
	{
	    std::vector<float> Values = Other.GetFloatVals();
	    UpdateFloatVals(Values);
	} break;
	case HEMAX_PARAM_STRING:
	case HEMAX_PARAM_PATH_FILE:
	case HEMAX_PARAM_PATH_FILE_DIR:
	case HEMAX_PARAM_PATH_FILE_GEO:
	case HEMAX_PARAM_PATH_FILE_IMAGE:
	{
	    std::vector<std::string> Values = Other.GetStringVals();
	    UpdateStringVals(Values);
	} break;
	case HEMAX_PARAM_MULTIPARMLIST:
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

