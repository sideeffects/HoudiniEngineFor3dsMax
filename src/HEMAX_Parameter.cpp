#include "HEMAX_Parameter.h"

#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"

HEMAX_Parameter::HEMAX_Parameter()
{
    OwnerId = -1;
    Type = HEMAX_PARAM_INVALID;
}

HEMAX_Parameter::HEMAX_Parameter(HEMAX_NodeId Owner, HEMAX_ParameterInfo ParameterInfo)
{
    OwnerId = Owner;
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

std::vector<int>
GetParameterIntValues(HEMAX_Parameter& Parameter)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    std::vector<int> ParmIntValues(Parameter.Info.size);

    SessionManager.Session->GetParameterIntValues(Parameter.OwnerId, &ParmIntValues.front(), Parameter.Info.intValuesIndex, Parameter.Info.size);

    return ParmIntValues;
}

std::vector<float>
GetParameterFloatValues(HEMAX_Parameter& Parameter)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    std::vector<float> ParmFloatValues(Parameter.Info.size);

    SessionManager.Session->GetParameterFloatValues(Parameter.OwnerId, &ParmFloatValues.front(), Parameter.Info.floatValuesIndex, Parameter.Info.size);

    return ParmFloatValues;
}

std::vector<std::string>
GetParameterStringValues(HEMAX_Parameter& Parameter, bool Evaluate)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HEMAX_StringHandle* SHValues = new HEMAX_StringHandle[Parameter.Info.size];
    std::vector<std::string> ParmStringValues(Parameter.Info.size);

    SessionManager.Session->GetParameterStringValues(Parameter.OwnerId, Evaluate, SHValues, Parameter.Info.stringValuesIndex, Parameter.Info.size);

    for (int i = 0; i < Parameter.Info.size; ++i)
    {
        ParmStringValues[i] = SessionManager.Session->GetHAPIString(SHValues[i]);
    }

    return ParmStringValues;
}

std::vector<HEMAX_ParmChoice>
GetIntParameterChoiceLists(HEMAX_Parameter& Parameter)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HEMAX_ParameterChoiceInfo* Choices = new HEMAX_ParameterChoiceInfo[Parameter.Info.choiceCount];
    std::vector<HEMAX_ParmChoice> ParmChoices(Parameter.Info.choiceCount);

    SessionManager.Session->GetParameterChoiceLists(Parameter.OwnerId, Choices, Parameter.Info.choiceIndex, Parameter.Info.choiceCount);

    for (int i = 0; i < Parameter.Info.choiceCount; ++i)
    {
        ParmChoices[i].ChoiceLabel = SessionManager.Session->GetHAPIString(Choices[i].labelSH);
        ParmChoices[i].ChoiceValue = SessionManager.Session->GetHAPIString(Choices[i].valueSH);
    }

    delete Choices;

    return ParmChoices;
}

std::vector<HEMAX_ParmChoice>
GetStringParameterChoiceLists(HEMAX_Parameter& Parameter)
{
    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    HEMAX_ParameterChoiceInfo* Choices = new HEMAX_ParameterChoiceInfo[Parameter.Info.choiceCount];
    std::vector<HEMAX_ParmChoice> ParmChoices(Parameter.Info.choiceCount);

    SessionManager.Session->GetParameterChoiceLists(Parameter.OwnerId, Choices, Parameter.Info.choiceIndex, Parameter.Info.choiceCount);

    for (int i = 0; i < Parameter.Info.choiceCount; ++i)
    {
        ParmChoices[i].ChoiceLabel = SessionManager.Session->GetHAPIString(Choices[i].labelSH);
        ParmChoices[i].ChoiceValue = SessionManager.Session->GetHAPIString(Choices[i].valueSH);
    }

    delete Choices;

    return ParmChoices;
}

void
UpdateParameterIntValues(HEMAX_Parameter& Parameter, std::vector<int> Values)
{
    HEMAX_SessionManager::GetSessionManager().Session->SetParameterIntValues(Parameter.OwnerId, &Values.front(), Parameter.Info.intValuesIndex, Parameter.Info.size);
}

void
UpdateParameterFloatValues(HEMAX_Parameter& Parameter, std::vector<float> Values)
{
    HEMAX_SessionManager::GetSessionManager().Session->SetParameterFloatValues(Parameter.OwnerId, &Values.front(), Parameter.Info.floatValuesIndex, Parameter.Info.size);
}

void
UpdateParameterStringValues(HEMAX_Parameter& Parameter, std::vector<std::string> Values)
{
    HEMAX_SessionManager& Manager = HEMAX_SessionManager::GetSessionManager();
    for (int i = 0; i < Values.size(); ++i)
    {
        Manager.Session->SetParameterStringValue(Parameter.OwnerId, Values[i].c_str(), Parameter.Info.id, i);
    }
}

void
UpdateParameterInputNode(HEMAX_Parameter& Parameter, HEMAX_NodeId InputNode)
{
    HEMAX_SessionManager& Manager = HEMAX_SessionManager::GetSessionManager();

    Manager.Session->SetParameterNodeValue(Parameter.OwnerId, GetParameterName(Parameter).c_str(), InputNode);
}

std::string
GetParameterName(HEMAX_Parameter& Parameter)
{
    return HEMAX_SessionManager::GetSessionManager().Session->GetHAPIString(Parameter.Info.nameSH);
}

std::string
GetParameterLabel(HEMAX_Parameter& Parameter)
{
    return HEMAX_SessionManager::GetSessionManager().Session->GetHAPIString(Parameter.Info.labelSH);
}

std::string
GetHelpString(HEMAX_Parameter& Parameter)
{
    if (Parameter.Info.helpSH == 0)
    {
        return "";
    }

    std::string HelpString = HEMAX_SessionManager::GetSessionManager().Session->GetHAPIString(Parameter.Info.helpSH);
    return HelpString;
}

HEMAX_NodeId
GetParameterInputNodeId(HEMAX_Parameter& Parameter)
{
    if (Parameter.Type = HEMAX_PARAM_NODE)
    {
        HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

        HEMAX_NodeId NodeId;

        if (SessionManager.Session->GetParameterNodeValue(Parameter.OwnerId, GetParameterName(Parameter).c_str(), &NodeId))
        {
            return NodeId;
        }
    }

    return -1;
}

std::string
GetParameterInputNodeName(HEMAX_Parameter& Parameter)
{
    HEMAX_NodeId InputNodeId = GetParameterInputNodeId(Parameter);

    HEMAX_SessionManager& SessionManager = HEMAX_SessionManager::GetSessionManager();

    if (InputNodeId > -1)
    {
        HEMAX_NodeInfo InputNodeInfo;
        SessionManager.Session->GetNodeInfo(InputNodeId, &InputNodeInfo);

        return SessionManager.Session->GetHAPIString(InputNodeInfo.nameSH);
    }
    else
    {
        return "";
    }
}

bool
IsParameterChoiceList(HEMAX_Parameter& Parameter)
{
    return (Parameter.Info.choiceCount > 0);
}

bool
IsRootLevelParameter(HEMAX_Parameter& Parameter)
{
    return (Parameter.Info.parentId < 0);
}

bool
HasUIConstraints(HEMAX_Parameter& Parameter)
{
    if (Parameter.Info.hasUIMin || Parameter.Info.hasUIMax)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
IsMultiParameter(HEMAX_Parameter& Parameter)
{
    return Parameter.Type == HEMAX_PARAM_MULTIPARMLIST;
}

int
GetMultiParameterInstancePosition(HEMAX_Parameter& Parameter)
{
    return Parameter.Info.instanceNum - 1;
}

int
GetMultiParameterStartOffset(HEMAX_Parameter& Parameter)
{
    return Parameter.Info.instanceStartOffset;
}

void
InsertMultiParameterInstance(HEMAX_Parameter& Parameter, int Position)
{
    HEMAX_SessionManager::GetSessionManager().Session->InsertMultiParameterInstance(Parameter.OwnerId, Parameter.Info.id, Position + GetMultiParameterStartOffset(Parameter));
}

void
RemoveMultiParameterInstance(HEMAX_Parameter& Parameter, int Position)
{
    HEMAX_SessionManager::GetSessionManager().Session->RemoveMultiParameterInstance(Parameter.OwnerId, Parameter.Info.id, Position + GetMultiParameterStartOffset(Parameter));
}

bool
CopyParameter(HEMAX_Parameter& Source, HEMAX_Parameter& Dest)
{
    bool RecookRequired = false;

    if (Source.Type != Dest.Type)
    {
        HEMAX_Logger::Instance().AddEntry("HEMAX_Parameter: attempting to copy parameters but parameters are of different type", HEMAX_LOG_LEVEL_ERROR);
        return RecookRequired;
    }

    switch (Source.Type)
    {
    case HEMAX_PARAM_INTEGER:
    case HEMAX_PARAM_TOGGLE:
    {
        std::vector<int> Values = GetParameterIntValues(Source);
        UpdateParameterIntValues(Dest, Values);
    } break;
    case HEMAX_PARAM_FLOAT:
    {
        std::vector<float> Values = GetParameterFloatValues(Source);
        UpdateParameterFloatValues(Dest, Values);
    } break;
    case HEMAX_PARAM_STRING:
    case HEMAX_PARAM_PATH_FILE:
    case HEMAX_PARAM_PATH_FILE_DIR:
    case HEMAX_PARAM_PATH_FILE_GEO:
    case HEMAX_PARAM_PATH_FILE_IMAGE:
    {
        std::vector<std::string> Values = GetParameterStringValues(Source, true);
        UpdateParameterStringValues(Dest, Values);
    } break;
    case HEMAX_PARAM_MULTIPARMLIST:
    {
        int Source_Count = Source.Info.instanceCount;
        int Dest_Count = Dest.Info.instanceCount;

        if (Source_Count > Dest_Count)
        {
            while (Dest_Count != Source_Count)
            {
                InsertMultiParameterInstance(Dest, Dest_Count++);
            }
        }
        else if (Source_Count < Dest_Count)
        {
            while (Dest_Count != Source_Count)
            {
                RemoveMultiParameterInstance(Dest, (Dest_Count - 1));
                Dest_Count--;
            }
        }

        RecookRequired = true;
    } break;
    default:
    {

    } break;
    }

    return RecookRequired;
}