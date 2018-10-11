#include "HEMAX_3dsmaxHda.h"

#include "HEMAX_Logger.h"

void
CreateGeometry3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, HEMAX_Asset& Asset, int AssetIndex)
{
    InstantiateHda(Asset, AssetIndex, MaxHda.Hda);

    MaxHda.Type = HEMAX_GEOMETRY_HDA;
    InitializeSubnetworks(MaxHda);

    CreateNewGeometryHda(MaxHda.GeometryHda, MaxHda.Hda);
    InitializeParameterCustomAttributes(MaxHda);
}

void
CreateGeometry3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, INode* ContainerNode, HEMAX_Asset& Asset, int AssetIndex)
{
    InstantiateHda(Asset, AssetIndex, MaxHda.Hda);

    MaxHda.Type = HEMAX_GEOMETRY_HDA;
    InitializeSubnetworks(MaxHda);

    CreateNewGeometryHda(MaxHda.GeometryHda, MaxHda.Hda, ContainerNode);
    InitializeParameterCustomAttributes(MaxHda);
}

bool
CreateModifier3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, HEMAX_Asset& Asset, int AssetIndex, HEMAX_Modifier* Modifier, INode* MaxNode)
{
    InstantiateHda(Asset, AssetIndex, MaxHda.Hda);

    MaxHda.Type = HEMAX_MODIFIER_HDA;

    if (MaxHda.Hda.MainNode.Info.inputCount > 0)
    {
        MaxHda.SubnetworkNodeInputs.resize(MaxHda.Hda.MainNode.Info.inputCount);
    }

    bool Success = CreateNewModifierHda(MaxHda.ModifierHda, MaxHda.Hda, Modifier, MaxNode);

    if (Success)
    {
        InitializeParameterCustomAttributes(MaxHda);
    }

    return Success;
}

void
RecreateGeometry3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, HEMAX_Asset& Asset, int AssetIndex, INode* ContainerNode)
{
    InstantiateHda(Asset, AssetIndex, MaxHda.Hda);
    MaxHda.Type = HEMAX_GEOMETRY_HDA;
    InitializeSubnetworks(MaxHda);
    
    CreateGeometryHdaFromContainerNode(MaxHda.GeometryHda, MaxHda.Hda, ContainerNode);
}

void
RecreateModifier3dsmaxHda(HEMAX_3dsmaxHda& MaxHda, HEMAX_Asset& Asset, int AssetIndex, HEMAX_Modifier* Modifier, INode* ContainerNode)
{
    InstantiateHda(Asset, AssetIndex, MaxHda.Hda);
    MaxHda.Type = HEMAX_MODIFIER_HDA;
    InitializeSubnetworks(MaxHda);

    RecreateExistingModifierHda(MaxHda.ModifierHda, MaxHda.Hda, Modifier, ContainerNode);
}

void
InitializeSubnetworks(HEMAX_3dsmaxHda& MaxHda)
{
    if (MaxHda.Hda.MainNode.Info.inputCount > 0)
    {
        MaxHda.SubnetworkNodeInputs.resize(MaxHda.Hda.MainNode.Info.inputCount);

        for (int i = 0; i < MaxHda.SubnetworkNodeInputs.size(); i++)
        {
            MaxHda.SubnetworkNodeInputs[i] = nullptr;
        }
    }
}

void
Cook3dsmaxHda(HEMAX_3dsmaxHda& MaxHda)
{
    Cook(MaxHda.Hda.MainNode);
}

void
Update3dsmaxHda(HEMAX_3dsmaxHda& MaxHda)
{
    UpdateHda(MaxHda.Hda);

    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        UpdateGeometryHda(MaxHda.GeometryHda, MaxHda.Hda);
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        UpdateModifierHda(MaxHda.ModifierHda, MaxHda.Hda);
    }
}

void
SetParameterInput(HEMAX_3dsmaxHda& MaxHda, HEMAX_ParameterId ParamId, HEMAX_InputInstance* Input)
{
    MaxHda.InputNodeMap.insert_or_assign(ParamId, Input);
}

HEMAX_InputInstance*
FindParameterInput(HEMAX_3dsmaxHda& MaxHda, HEMAX_ParameterId ParamId)
{
    auto Search = MaxHda.InputNodeMap.find(ParamId);

    if (Search != MaxHda.InputNodeMap.end())
    {
        return Search->second;
    }

    return nullptr;
}

std::vector<HEMAX_Parameter>
GetAllParametersWithInputs(HEMAX_3dsmaxHda& MaxHda)
{
    std::vector<HEMAX_Parameter> Parameters;

    for (auto It = MaxHda.InputNodeMap.begin(); It != MaxHda.InputNodeMap.end(); It++)
    {
        Parameters.push_back(GetParameter(MaxHda.Hda.MainNode, It->first));
    }

    return Parameters;
}

std::vector<HEMAX_InputInstance*>
GetAllParameter3dsmaxInputs(HEMAX_3dsmaxHda& MaxHda)
{
    std::vector<HEMAX_InputInstance*> Instances;

    for (auto It = MaxHda.InputNodeMap.begin(); It != MaxHda.InputNodeMap.end(); It++)
    {
        if (It->second)
        {
            Instances.push_back(It->second);
        }
    }

    return Instances;
}

void
UpdateParameterInputNode(HEMAX_3dsmaxHda& MaxHda, HEMAX_ParameterId ParamId)
{
    HEMAX_Input* InputNode = nullptr;

    HEMAX_InputInstance* ParameterInput = FindParameterInput(MaxHda, ParamId);
    HEMAX_Parameter Parameter = GetParameter(MaxHda.Hda.MainNode, ParamId);

    if (ParameterInput)
    {
        InputNode = ParameterInput->MaxInput->GetInputNode();

        if (ParameterInput->MergeNode)
        {
            UpdateParameterInputNode(Parameter, ParameterInput->MergeNode->GetMergedInputs().Info.id);
        }
    }
}

void
ClearParameterInputNode(HEMAX_3dsmaxHda& MaxHda, HEMAX_ParameterId ParamId)
{
    auto Search = MaxHda.InputNodeMap.find(ParamId);

    if (Search != MaxHda.InputNodeMap.end())
    {
        Search->second = nullptr;
        MaxHda.InputNodeMap.erase(Search);
    }
}

void
SetSubnetworkInput(HEMAX_3dsmaxHda& MaxHda, int Subnetwork, HEMAX_InputInstance* Input)
{
    MaxHda.SubnetworkNodeInputs[Subnetwork] = Input;
}

HEMAX_InputInstance*
FindSubnetworkInput(HEMAX_3dsmaxHda& MaxHda, int Subnetwork)
{
    if (Subnetwork >= 0 && Subnetwork < MaxHda.SubnetworkNodeInputs.size())
    {
        if (MaxHda.SubnetworkNodeInputs[Subnetwork])
        {
            return MaxHda.SubnetworkNodeInputs[Subnetwork];
        }
    }

    return nullptr;
}

void
UpdateSubnetworkInput(HEMAX_3dsmaxHda& MaxHda, int Subnetwork)
{
    HEMAX_Input* InputNode = nullptr;

    if (MaxHda.SubnetworkNodeInputs[Subnetwork])
    {
        if (MaxHda.SubnetworkNodeInputs[Subnetwork]->MergeNode)
        {
            InputNode = MaxHda.SubnetworkNodeInputs[Subnetwork]->MaxInput->GetInputNode();
            HEMAX_Node HapiInputNode = MaxHda.SubnetworkNodeInputs[Subnetwork]->MergeNode->GetMergedInputs();
            ConnectInputNode(MaxHda.Hda.MainNode, HapiInputNode.Info.id, Subnetwork);
        }
        else
        {
            DisconnectInputNode(MaxHda.Hda.MainNode, Subnetwork);
        }
    }
}

void
ClearSubnetworkInput(HEMAX_3dsmaxHda& MaxHda, int Subnetwork)
{
    MaxHda.SubnetworkNodeInputs[Subnetwork] = nullptr;
}

void
UpdateMultiParameterList(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter Parameter)
{
    ClearParameterCustomAttributes(MaxHda);
    InitializeParameterCustomAttributes(MaxHda);
    UpdateAllCustomAttributes(MaxHda);
}

ICustAttribContainer*
GetCustAttribContainer(HEMAX_3dsmaxHda& MaxHda)
{
    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        return MaxHda.GeometryHda.CustomAttributes;
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        return MaxHda.ModifierHda.CustomAttributes;
    }

    return nullptr;
}

std::unordered_map<std::string, HEMAX_ParameterAttrib*>*
GetCustAttribMap(HEMAX_3dsmaxHda& MaxHda)
{
    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        return &MaxHda.GeometryHda.CustomAttributeMap;
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        return &MaxHda.ModifierHda.CustomAttributeMap;
    }

    return nullptr;
}

std::string
GetCustAttribStringValue(HEMAX_3dsmaxHda& MaxHda, std::string CustAttribName, bool& Success)
{
    Success = false;

    ICustAttribContainer* Container = GetCustAttribContainer(MaxHda);
    if (Container)
    {
        CustAttrib* Attrib = GetCustomAttributeByName(Container, CustAttribName);
        if (Attrib)
        {
            HEMAX_ParameterAttrib* ParmAttrib = dynamic_cast<HEMAX_ParameterAttrib*>(Attrib);
            if (ParmAttrib)
            {
                ParamType2 ParamType = ParmAttrib->PBlock->GetParameterType(0);
                if (ParamType == TYPE_STRING)
                {
                    HEMAX_StringParameterAttrib* StringAttrib = dynamic_cast<HEMAX_StringParameterAttrib*>(Attrib);
                    if (StringAttrib)
                    {
                        Success = true;
                        return StringAttrib->GetStringValue();
                    }
                }
            }
        }
    }

    return "";
}

void
InitializeParameterCustomAttributes(HEMAX_3dsmaxHda& MaxHda)
{
    ICustAttribContainer* CustAttribContainer;
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustAttribMap;

    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        CustAttribContainer = MaxHda.GeometryHda.CustomAttributes;
        CustAttribMap = &MaxHda.GeometryHda.CustomAttributeMap;
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        CustAttribContainer = MaxHda.ModifierHda.CustomAttributes;
        CustAttribMap = &MaxHda.ModifierHda.CustomAttributeMap;
    }

    std::vector<HEMAX_Parameter> Parameters = GetAllParameters(MaxHda.Hda.MainNode);

    for (auto It = Parameters.begin(); It != Parameters.end(); It++)
    {
        HEMAX_Parameter* Parameter = &(*It);

        switch (Parameter->Type)
        {
        case (HEMAX_PARAM_INTEGER):
        {
            std::vector<int> ParameterValues = GetParameterIntValues(*Parameter);

            if (Parameter->Info.size == 1)
            {
                HEMAX_IntegerParameterAttrib* ParmCustAttrib = new HEMAX_IntegerParameterAttrib;
                ParmCustAttrib->SetParameterName(GetParameterName(*Parameter));
                ParmCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[0]);

                CustAttribContainer->AppendCustAttrib(ParmCustAttrib);
                CustAttribMap->insert({ GetParameterName(*Parameter), ParmCustAttrib });
            }
            else if (Parameter->Info.size > 1)
            {
                for (int z = 0; z < Parameter->Info.size; z++)
                {
                    HEMAX_IntegerParameterAttrib* ParmCustAttrib = new HEMAX_IntegerParameterAttrib;
                    std::string CustAttribName = GetParameterName(*Parameter) + "__" + std::to_string(z);
                    ParmCustAttrib->SetParameterName(CustAttribName);
                    ParmCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[z]);

                    CustAttribContainer->AppendCustAttrib(ParmCustAttrib);
                    CustAttribMap->insert({ GetParameterName(*Parameter), ParmCustAttrib });
                }
            }
        } break;
        case (HEMAX_PARAM_STRING):
        case (HEMAX_PARAM_PATH_FILE):
        case (HEMAX_PARAM_PATH_FILE_DIR):
        case (HEMAX_PARAM_PATH_FILE_GEO):
        case (HEMAX_PARAM_PATH_FILE_IMAGE):
        {
            std::vector<std::string> ParameterValues = GetParameterStringValues(*Parameter, true);

            if (Parameter->Info.size == 1)
            {
                HEMAX_StringParameterAttrib* ParamCustAttrib = new HEMAX_StringParameterAttrib;
                ParamCustAttrib->SetParameterName(GetParameterName(*Parameter));
                std::wstring WideVal(ParameterValues[0].begin(), ParameterValues[0].end());
                ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), WideVal.c_str());

                CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
                CustAttribMap->insert({ GetParameterName(*Parameter), ParamCustAttrib });
            }
            else if (Parameter->Info.size > 1)
            {
                for (int z = 0; z < Parameter->Info.size; ++z)
                {
                    HEMAX_StringParameterAttrib* ParamCustAttrib = new HEMAX_StringParameterAttrib;
                    std::string CustAttribName = GetParameterName(*Parameter) + "__" + std::to_string(z);
                    ParamCustAttrib->SetParameterName(CustAttribName);
                    std::wstring WideVal(ParameterValues[z].begin(), ParameterValues[z].end());
                    ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), WideVal.c_str());

                    CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
                    CustAttribMap->insert({ CustAttribName, ParamCustAttrib });
                }
            }
        } break;
        case (HEMAX_PARAM_FLOAT):
        case (HEMAX_PARAM_COLOR):
        {
            std::vector<float> ParameterValues = GetParameterFloatValues(*Parameter);

            if (Parameter->Info.size == 1)
            {
                HEMAX_FloatParameterAttrib* ParamCustAttrib = new HEMAX_FloatParameterAttrib;
                ParamCustAttrib->SetParameterName(GetParameterName(*Parameter));
                ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[0]);

                CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
                CustAttribMap->insert({ GetParameterName(*Parameter), ParamCustAttrib });
            }
            else if (Parameter->Info.size > 1)
            {
                for (int z = 0; z < Parameter->Info.size; ++z)
                {
                    HEMAX_FloatParameterAttrib* ParamCustAttrib = new HEMAX_FloatParameterAttrib;
                    std::string CustAttribName = GetParameterName(*Parameter) + "__" + std::to_string(z);
                    ParamCustAttrib->SetParameterName(CustAttribName);
                    ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[z]);

                    CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
                    CustAttribMap->insert({ CustAttribName, ParamCustAttrib });
                }
            }
        } break;
        case (HEMAX_PARAM_TOGGLE):
        {
            std::vector<int> ParameterValues = GetParameterIntValues(*Parameter);

            if (Parameter->Info.size == 1)
            {
                HEMAX_ToggleParameterAttrib* ParamCustAttrib = new HEMAX_ToggleParameterAttrib;
                ParamCustAttrib->SetParameterName(GetParameterName(*Parameter));
                ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[0]);

                CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
                CustAttribMap->insert({ GetParameterName(*Parameter), ParamCustAttrib });
            }
            else if (Parameter->Info.size > 1)
            {
                for (int z = 0; z < Parameter->Info.size; ++z)
                {
                    HEMAX_ToggleParameterAttrib* ParamCustAttrib = new HEMAX_ToggleParameterAttrib;

                    std::string CustAttribName = GetParameterName(*Parameter) + "__" + std::to_string(z);
                    ParamCustAttrib->SetParameterName(CustAttribName);
                    ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[z]);

                    CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
                    CustAttribMap->insert({ CustAttribName, ParamCustAttrib });
                }
            }
        } break;
        case (HEMAX_PARAM_NODE):
        {
            HEMAX_NodeParameterAttrib* ParamCustAttrib = new HEMAX_NodeParameterAttrib;
            ParamCustAttrib->SetParameterName(GetParameterName(*Parameter));
            CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
            CustAttribMap->insert({ GetParameterName(*Parameter), ParamCustAttrib });
        } break;
        case (HEMAX_PARAM_MULTIPARMLIST):
        {
            HEMAX_MultiParameterAttrib* ParamCustAttrib = new HEMAX_MultiParameterAttrib;
            ParamCustAttrib->SetParameterName(GetParameterName(*Parameter));
            int InstanceCount = Parameter->Info.instanceCount;
            int InstanceLength = Parameter->Info.instanceLength;
            ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), InstanceCount);
            ParamCustAttrib->PBlock->SetValue(1, GetCOREInterface()->GetTime(), InstanceLength);
            CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
            CustAttribMap->insert({ GetParameterName(*Parameter), ParamCustAttrib });
        } break;
        default:
        {
            // Do nothing
        } break;
        }
    }
}

void
UpdateAllCustomAttributes(HEMAX_3dsmaxHda& MaxHda)
{
    for (auto InputIter = MaxHda.InputNodeMap.begin(); InputIter != MaxHda.InputNodeMap.end(); InputIter++)
    {
        if (InputIter->second)
        {
            HEMAX_Parameter TheParameter = GetParameter(MaxHda.Hda.MainNode, InputIter->first);
            UpdateInputNodeCustomAttribute(MaxHda, TheParameter, InputIter->second->MaxInput->GetInputNode());
        }
    }

    std::vector<HEMAX_Parameter> NodeParameters = GetAllParameters(MaxHda.Hda.MainNode);
    for (auto ParmIt = NodeParameters.begin(); ParmIt != NodeParameters.end(); ParmIt++)
    {
        HEMAX_Parameter Parameter = *ParmIt;

        switch (Parameter.Type)
        {
        case (HEMAX_PARAM_INTEGER):
        {
            std::vector<int> IntValues = GetParameterIntValues(Parameter);
            UpdateIntCustomAttribute(MaxHda, Parameter, IntValues);
        } break;
        case (HEMAX_PARAM_STRING):
        case (HEMAX_PARAM_PATH_FILE):
        case (HEMAX_PARAM_PATH_FILE_DIR):
        case (HEMAX_PARAM_PATH_FILE_GEO):
        case (HEMAX_PARAM_PATH_FILE_IMAGE):
        {
            std::vector<std::string> StringValues = GetParameterStringValues(Parameter, true);
            UpdateStringCustomAttribute(MaxHda, Parameter, StringValues);
        } break;
        case (HEMAX_PARAM_FLOAT):
        case (HEMAX_PARAM_COLOR):
        {
            std::vector<float> FloatValues = GetParameterFloatValues(Parameter);
            UpdateFloatCustomAttribute(MaxHda, Parameter, FloatValues);
        } break;
        case (HEMAX_PARAM_TOGGLE):
        {
            std::vector<int> ToggleValues = GetParameterIntValues(Parameter);
            UpdateToggleCustomAttribute(MaxHda, Parameter, ToggleValues);
        } break;
        default:
        {
            // Do nothing
        } break;
        }
    }
}

void
ClearParameterCustomAttributes(HEMAX_3dsmaxHda& MaxHda)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = nullptr;
    ICustAttribContainer* CustAttribs = nullptr;
    int LowestIndex = -1;
    int NumSubnetworkCustAttribs = -1;

    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        CustomAttributeMap = &MaxHda.GeometryHda.CustomAttributeMap;
        CustAttribs = MaxHda.GeometryHda.CustomAttributes;
        LowestIndex = MaxHda.GeometryHda.MaxStampIndex;
        NumSubnetworkCustAttribs = MaxHda.Hda.MainNode.Info.inputCount;
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        CustomAttributeMap = &MaxHda.ModifierHda.CustomAttributeMap;
        CustAttribs = MaxHda.ModifierHda.CustomAttributes;
        LowestIndex = MaxHda.ModifierHda.MaxStampIndex;
        NumSubnetworkCustAttribs = MaxHda.Hda.MainNode.Info.inputCount - 1;
    }

    CustomAttributeMap->clear();
    LowestIndex += NumSubnetworkCustAttribs;

    if (CustAttribs)
    {
        int NumCustAttribs = CustAttribs->GetNumCustAttribs();
        for (int z = NumCustAttribs - 1; z > LowestIndex; z--)
        {
            CustAttrib* Attribute = CustAttribs->GetCustAttrib(z);
            if (Attribute)
            {
                delete Attribute;
            }
        }
    }
}

void
UpdateIntCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, std::vector<int>& IntValues)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap(MaxHda);

    if (Parameter.Info.size == 1)
    {
        std::string ParameterName = GetParameterName(Parameter);
        auto Search = CustomAttributeMap->find(ParameterName);

        if (Search != CustomAttributeMap->end())
        {
            Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), IntValues[0]);
        }
    }
    else if (Parameter.Info.size > 1)
    {
        for (int p = 0; p < Parameter.Info.size; p++)
        {
            std::string ParameterName = GetParameterName(Parameter) + "__" + std::to_string(p);
            auto Search = CustomAttributeMap->find(ParameterName);

            if (Search != CustomAttributeMap->end())
            {
                Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), IntValues[p]);
            }
        }
    }
}

void
UpdateFloatCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, std::vector<float>& FloatValues)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap(MaxHda);

    if (Parameter.Info.size == 1)
    {
        std::string ParameterName = GetParameterName(Parameter);
        auto Search = CustomAttributeMap->find(ParameterName);

        if (Search != CustomAttributeMap->end())
        {
            Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), FloatValues[0]);
        }
    }
    else if (Parameter.Info.size > 1)
    {
        for (int p = 0; p < Parameter.Info.size; p++)
        {
            std::string ParameterName = GetParameterName(Parameter) + "__" + std::to_string(p);
            auto Search = CustomAttributeMap->find(ParameterName);

            if (Search != CustomAttributeMap->end())
            {
                Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), FloatValues[p]);
            }
        }
    }
}

void
UpdateStringCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, std::vector<std::string>& StringValues)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap(MaxHda);

    if (Parameter.Info.size == 1)
    {
        std::string ParameterName = GetParameterName(Parameter);
        auto Search = CustomAttributeMap->find(ParameterName);

        if (Search != CustomAttributeMap->end())
        {
            std::wstring WideVal(StringValues[0].begin(), StringValues[0].end());
            Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), WideVal.c_str());
        }
    }
    else if (Parameter.Info.size > 1)
    {
        for (int p = 0; p < Parameter.Info.size; p++)
        {
            std::string ParameterName = GetParameterName(Parameter) + "__" + std::to_string(p);
            auto Search = CustomAttributeMap->find(ParameterName);

            if (Search != CustomAttributeMap->end())
            {
                std::wstring WideVal(StringValues[p].begin(), StringValues[p].end());
                Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), WideVal.c_str());
            }
        }
    }
}

void
UpdateToggleCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, std::vector<int>& ToggleValues)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap(MaxHda);

    if (Parameter.Info.size == 1)
    {
        std::string ParameterName = GetParameterName(Parameter);
        auto Search = CustomAttributeMap->find(ParameterName);

        if (Search != CustomAttributeMap->end())
        {
            Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ToggleValues[0]);
        }
    }
    else if (Parameter.Info.size > 1)
    {
        for (int p = 0; p < Parameter.Info.size; p++)
        {
            std::string ParameterName = GetParameterName(Parameter) + "__" + std::to_string(p);
            auto Search = CustomAttributeMap->find(ParameterName);

            if (Search != CustomAttributeMap->end())
            {
                Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ToggleValues[p]);
            }
        }
    }
}

void
UpdateInputNodeCustomAttribute(HEMAX_3dsmaxHda& MaxHda, HEMAX_Parameter& Parameter, HEMAX_Input* InputNode)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap(MaxHda);
    std::string ParameterName = GetParameterName(Parameter);
    auto Search = CustomAttributeMap->find(ParameterName);

    if (Search != CustomAttributeMap->end())
    {
        if (InputNode)
        {
            INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(InputNode->GetMaxNodeHandle());
            HEMAX_NodeParameterAttrib* NodeParameterAttribute = (HEMAX_NodeParameterAttrib*)Search->second;
            NodeParameterAttribute->SetMessagesBlocked(true);
            NodeParameterAttribute->PBlock->SetValue(0, GetCOREInterface()->GetTime(), MaxInputNode);
            NodeParameterAttribute->SetMessagesBlocked(false);
        }
        else
        {
            INode* NullNode = nullptr;
            HEMAX_NodeParameterAttrib* NodeParameterAttribute = (HEMAX_NodeParameterAttrib*)Search->second;
            NodeParameterAttribute->SetMessagesBlocked(true);
            NodeParameterAttribute->PBlock->SetValue(0, GetCOREInterface()->GetTime(), NullNode);
            NodeParameterAttribute->SetMessagesBlocked(false);
        }
    }
}

void
UpdateSubnetworkCustomAttribute(HEMAX_3dsmaxHda& MaxHda, int Subnetwork, HEMAX_Input* InputNode)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap(MaxHda);
    std::string SubnetworkName = "subnetwork_" + std::to_string(Subnetwork);
    auto Search = CustomAttributeMap->find(SubnetworkName);

    if (Search != CustomAttributeMap->end())
    {
        if (InputNode)
        {
            INode* MaxInputNode = GetCOREInterface()->GetINodeByHandle(InputNode->GetMaxNodeHandle());
            HEMAX_NodeParameterAttrib* NodeParameterAttribute = (HEMAX_NodeParameterAttrib*)Search->second;
            NodeParameterAttribute->SetMessagesBlocked(true);
            NodeParameterAttribute->PBlock->SetValue(0, GetCOREInterface()->GetTime(), MaxInputNode);
            NodeParameterAttribute->SetMessagesBlocked(false);
        }
        else
        {
            INode* NullNode = nullptr;
            HEMAX_NodeParameterAttrib* NodeParameterAttribute = (HEMAX_NodeParameterAttrib*)Search->second;
            NodeParameterAttribute->SetMessagesBlocked(true);
            NodeParameterAttribute->PBlock->SetValue(0, GetCOREInterface()->GetTime(), NullNode);
            NodeParameterAttribute->SetMessagesBlocked(false);
        }
    }
}

std::vector<HEMAX_SubnetworkInputMapping>
ReloadSubnetworkInputsFromCustomAttributes(HEMAX_3dsmaxHda& MaxHda)
{
    ICustAttribContainer* CustAttribContainer = nullptr;
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = nullptr;

    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        CustAttribContainer = MaxHda.GeometryHda.CustomAttributes;
        CustomAttributeMap = &MaxHda.GeometryHda.CustomAttributeMap;
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        CustAttribContainer = MaxHda.ModifierHda.CustomAttributes;
        CustomAttributeMap = &MaxHda.ModifierHda.CustomAttributeMap;
    }

    std::vector<HEMAX_SubnetworkInputMapping> Mapping;

    for (int z = 0; z < MaxHda.Hda.MainNode.Info.inputCount; z++)
    {
        std::string SubnetworkSearch = "subnetwork_" + std::to_string(z);
        auto Search = CustomAttributeMap->find(SubnetworkSearch);
        
        if (Search != CustomAttributeMap->end())
        {
            INode* InputNode = Search->second->PBlock->GetINode(0);

            HEMAX_SubnetworkInputMapping Entry;
            Entry.Subnetwork = z;
            Entry.Node = InputNode;

            Mapping.push_back(Entry);
        }
    }

    return Mapping;
}

std::vector<HEMAX_ParameterInputMapping>
ReloadParametersFromCustomAttributes(HEMAX_3dsmaxHda& MaxHda)
{
    std::vector<HEMAX_ParameterInputMapping> InputMap;
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = nullptr;

    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        CustomAttributeMap = &MaxHda.GeometryHda.CustomAttributeMap;
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        CustomAttributeMap = &MaxHda.ModifierHda.CustomAttributeMap;
    }

    bool AnotherPassRequired = false;
    std::unordered_map<std::string, HEMAX_Parameter> CompletionMap;

    do
    {
        AnotherPassRequired = false;
        std::vector<HEMAX_Parameter> NodeParameters = GetAllParameters(MaxHda.Hda.MainNode);
        for (auto ParmIt = NodeParameters.begin(); ParmIt != NodeParameters.end(); ParmIt++)
        {
            HEMAX_Parameter Parameter = *ParmIt;

            if (!(Parameter.Info.type >= HAPI_PARMTYPE_NONVALUE_START && Parameter.Info.type <= HAPI_PARMTYPE_NONVALUE_END))
            {
                std::string ParameterName = GetParameterName(Parameter);
                auto IsCompleted = CompletionMap.find(ParameterName);
                if (IsCompleted == CompletionMap.end())
                {
                    switch (Parameter.Type)
                    {
                    case (HEMAX_PARAM_INTEGER):
                    {
                        RemakeIntParameterFromCustAttrib(Parameter, *CustomAttributeMap);
                        CompletionMap.insert({ ParameterName, Parameter });
                        break;
                    }
                    case (HEMAX_PARAM_STRING):
                    case (HEMAX_PARAM_PATH_FILE):
                    case (HEMAX_PARAM_PATH_FILE_DIR):
                    case (HEMAX_PARAM_PATH_FILE_GEO):
                    case (HEMAX_PARAM_PATH_FILE_IMAGE):
                    {
                        RemakeStringParameterFromCustAttrib(Parameter, *CustomAttributeMap);
                        CompletionMap.insert({ ParameterName, Parameter });
                        break;
                    }
                    case (HEMAX_PARAM_FLOAT):
                    case (HEMAX_PARAM_COLOR):
                    {
                        RemakeFloatParameterFromCustAttrib(Parameter, *CustomAttributeMap);
                        CompletionMap.insert({ ParameterName, Parameter });
                        break;
                    }
                    case (HEMAX_PARAM_TOGGLE):
                    {
                        RemakeToggleParameterFromCustAttrib(Parameter, *CustomAttributeMap);
                        CompletionMap.insert({ ParameterName, Parameter });
                        break;
                    }
                    case (HEMAX_PARAM_NODE):
                    {
                        HEMAX_ParameterInputMapping InputEntry = RemakeInputParameterFromCustAttrib(Parameter, *CustomAttributeMap);
                        if (InputEntry.Node)
                        {
                            InputMap.push_back(InputEntry);
                        }
                        CompletionMap.insert({ ParameterName, Parameter });
                        break;
                    }
                    case (HEMAX_PARAM_MULTIPARMLIST):
                    {
                        AnotherPassRequired = true;
                        RemakeMultiParameter(Parameter, *CustomAttributeMap);
                        CompletionMap.insert({ ParameterName, Parameter });
                        break;
                    }
                    default:
                    {
                        CompletionMap.insert({ ParameterName, Parameter });
                        break;
                    }
                    }
                }
            }
            if (AnotherPassRequired)
            {
                Cook(MaxHda.Hda.MainNode);
            }
        }
    } while (AnotherPassRequired);

    return InputMap;
}

void
RemakeIntParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    if (Parameter.Info.size == 1)
    {
        std::string ParameterName = GetParameterName(Parameter);
        auto Search = CustomAttributeMap.find(ParameterName);

        if (Search != CustomAttributeMap.end())
        {
            int IntValue;
            Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), IntValue, FOREVER);

            std::vector<int> IntValues = { IntValue };
            UpdateParameterIntValues(Parameter, IntValues);
        }
    }
    else if (Parameter.Info.size > 1)
    {
        std::vector<int> IntValues;
        for (int p = 0; p < Parameter.Info.size; p++)
        {
            std::string ParameterName = GetParameterName(Parameter) + "__" + std::to_string(p);
            auto Search = CustomAttributeMap.find(ParameterName);

            if (Search != CustomAttributeMap.end())
            {
                int IntValue;
                Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), IntValue, FOREVER);
                IntValues.push_back(IntValue);
            }
        }
        UpdateParameterIntValues(Parameter, IntValues);
    }
}

void
RemakeStringParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    if (Parameter.Info.size == 1)
    {
        std::string ParameterName = GetParameterName(Parameter);
        auto Search = CustomAttributeMap.find(ParameterName);

        if (Search != CustomAttributeMap.end())
        {
            const MCHAR* Val;
            Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), Val, FOREVER);

            std::wstring WideStringValue(Val);
            std::string StringValue(WideStringValue.begin(), WideStringValue.end());
            std::vector<std::string> StringValues = { StringValue };

            UpdateParameterStringValues(Parameter, StringValues);
        }
    }
    else if (Parameter.Info.size > 1)
    {
        std::vector<std::string> StringValues;
        for (int p = 0; p < Parameter.Info.size; p++)
        {
            std::string ParameterName = GetParameterName(Parameter) + "__" + std::to_string(p);
            auto Search = CustomAttributeMap.find(ParameterName);

            if (Search != CustomAttributeMap.end())
            {
                const MCHAR* Val;
                Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), Val, FOREVER);

                std::wstring WideStringValue(Val);
                std::string StringValue(WideStringValue.begin(), WideStringValue.end());
                StringValues.push_back(StringValue);
            }
        }
        UpdateParameterStringValues(Parameter, StringValues);
    }
}

void
RemakeFloatParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    if (Parameter.Info.size == 1)
    {
        std::string ParameterName = GetParameterName(Parameter);
        auto Search = CustomAttributeMap.find(ParameterName);

        if (Search != CustomAttributeMap.end())
        {
            float FloatValue;
            Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), FloatValue, FOREVER);

            std::vector<float> FloatValues = { FloatValue };
            UpdateParameterFloatValues(Parameter, FloatValues);
        }
    }
    else if (Parameter.Info.size > 1)
    {
        std::vector<float> FloatValues;

        for (int p = 0; p < Parameter.Info.size; ++p)
        {
            std::string ParameterName = GetParameterName(Parameter) + "__" + std::to_string(p);
            auto Search = CustomAttributeMap.find(ParameterName);

            if (Search != CustomAttributeMap.end())
            {
                float FloatValue;
                Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), FloatValue, FOREVER);
                FloatValues.push_back(FloatValue);
            }
        }
        UpdateParameterFloatValues(Parameter, FloatValues);
    }
}

void
RemakeToggleParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    std::string ParameterName = GetParameterName(Parameter);
    auto Search = CustomAttributeMap.find(ParameterName);

    if (Parameter.Info.size == 1)
    {
        std::string ParameterName = GetParameterName(Parameter);
        auto Search = CustomAttributeMap.find(ParameterName);

        if (Search != CustomAttributeMap.end())
        {
            int ToggleValue;
            Search->second->PBlock->GetValue(0, 0, ToggleValue, FOREVER);

            std::vector<int> ToggleValues = { ToggleValue };
            UpdateParameterIntValues(Parameter, ToggleValues);
        }
    }
    else if (Parameter.Info.size > 1)
    {
        std::vector<int> IntValues;

        for (int p = 0; p < Parameter.Info.size; ++p)
        {
            std::string ParameterName = GetParameterName(Parameter) + "__" + std::to_string(p);
            auto Search = CustomAttributeMap.find(ParameterName);

            if (Search != CustomAttributeMap.end())
            {
                int IntValue;
                Search->second->PBlock->GetValue(0, 0, IntValue, FOREVER);
                IntValues.push_back(IntValue);
            }
        }
        UpdateParameterIntValues(Parameter, IntValues);
    }
}

HEMAX_ParameterInputMapping
RemakeInputParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    HEMAX_ParameterInputMapping Entry;
    Entry.Node = nullptr;
    Entry.ParameterName = "";

    std::string ParameterName = GetParameterName(Parameter);
    auto Search = CustomAttributeMap.find(ParameterName);

    if (Search != CustomAttributeMap.end())
    {
        INode* InputNode = Search->second->PBlock->GetINode(0);

        Entry.Node = InputNode;
        Entry.ParameterName = ParameterName;
    }

    return Entry;
}

void
RemakeMultiParameter(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    std::string ParameterName = GetParameterName(Parameter);
    auto Search = CustomAttributeMap.find(ParameterName);
    if (Search != CustomAttributeMap.end())
    {
        HEMAX_MultiParameterAttrib* CustAttrib = (HEMAX_MultiParameterAttrib*)Search->second;
        int InstanceCount = CustAttrib->PBlock->GetInt(0);
        int InstanceLength = CustAttrib->PBlock->GetInt(1);

        for (int i = 0; i < InstanceCount; i++)
        {
            InsertMultiParameterInstance(Parameter, i);
        }
    }
}

std::string
GetHardcodedHdaAssetPath(HEMAX_3dsmaxHda& MaxHda)
{
    bool Success;
    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        return GetCustAttribStringValue(MaxHda, HEMAX_MAX_HOUDINI_ASSET_PATH_NAME, Success);
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        return GetCustAttribStringValue(MaxHda, HEMAX_HOUDINI_MODIFIER_ASSET_PATH_NAME, Success);
    }
    
    return "";
}

void
SetHardcodedHdaAssetPath(HEMAX_3dsmaxHda& MaxHda, std::string AssetPath)
{
    HEMAX_StringParameterAttrib* AssetPathAttrib = nullptr;

    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        ICustAttribContainer* Container = MaxHda.GeometryHda.CustomAttributes;
        AssetPathAttrib = GetStringParameterAttrib(Container, HEMAX_MAX_HOUDINI_ASSET_PATH_NAME);
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        ICustAttribContainer* Container = MaxHda.ModifierHda.CustomAttributes;
        AssetPathAttrib = GetStringParameterAttrib(Container, HEMAX_HOUDINI_MODIFIER_ASSET_PATH_NAME);
    }

    if (AssetPathAttrib)
    {
        AssetPathAttrib->SetStringValue(AssetPath);
    }
}

void
BakeHda(HEMAX_3dsmaxHda& MaxHda)
{
    if (MaxHda.Type == HEMAX_GEOMETRY_HDA)
    {
        BakeGeometryHda(MaxHda.GeometryHda);
    }
    else if (MaxHda.Type == HEMAX_MODIFIER_HDA)
    {
        HEMAX_Logger::Instance().AddEntry("HEMAX_3dsmaxHda: BakeHda functionality not implemented for modifier HDAs", HEMAX_LOG_LEVEL_INFO);
    }
}

void
CopyAllParameterValues(HEMAX_3dsmaxHda& Dest, HEMAX_3dsmaxHda& Source)
{
    if (Dest.Hda.HdaAsset.Id != Source.Hda.HdaAsset.Id)
    {
        std::string Msg = "Attempting to copy parameter values from HDA " + Source.Hda.HdaAsset.Names[Source.Hda.HdaAssetIndex] +
            " to " + Dest.Hda.HdaAsset.Names[Dest.Hda.HdaAssetIndex] + ", but they are different assets";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR);
        
        return;
    }

    std::vector<HEMAX_Parameter> SourceParms = GetAllParameters(Source.Hda.MainNode);
    std::vector<HEMAX_Parameter> DestParms = GetAllParameters(Dest.Hda.MainNode);

    bool Finished = true;
    int ParmIndex = 0;

    do
    {
        Finished = true;
        for (int p = ParmIndex; p < SourceParms.size(); p++)
        {
            if (CopyParameter(SourceParms[p], DestParms[p]))
            {
                Cook(Dest.Hda.MainNode);
                DestParms = GetAllParameters(Dest.Hda.MainNode);
                Finished = false;
                ParmIndex = p + 1;
                break;
            }
        }
    } while (!Finished);
}