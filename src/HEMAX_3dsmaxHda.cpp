#include "HEMAX_3dsmaxHda.h"

#include "HEMAX_Logger.h"

void
HEMAX_3dsmaxHda::InitializeSubnetworks()
{
    if (Hda.MainNode.Info.inputCount > 0)
    {
	SubnetworkNodeInputs.resize(Hda.MainNode.Info.inputCount);

	for (int i = 0; i < SubnetworkNodeInputs.size(); i++)
	{
	    SubnetworkNodeInputs[i] = nullptr;
	}
    }
}

void
HEMAX_3dsmaxHda::Cook3dsmaxHda()
{
    Hda.MainNode.Cook();

    if (Hda.MainNode.ShouldCookTwice())
    {
	Hda.MainNode.Cook();
    }
}

void
HEMAX_3dsmaxHda::SetParameterInput(HEMAX_ParameterId ParamId, HEMAX_InputInstance* Input)
{
    InputNodeMap.insert_or_assign(ParamId, Input);
}

HEMAX_InputInstance*
HEMAX_3dsmaxHda::FindParameterInput(HEMAX_ParameterId ParamId)
{
    auto Search = InputNodeMap.find(ParamId);

    if (Search != InputNodeMap.end())
    {
	return Search->second;
    }

    return nullptr;
}

std::vector<HEMAX_Parameter>
HEMAX_3dsmaxHda::GetAllParametersWithInputs()
{
    std::vector<HEMAX_Parameter> Parameters;

    for (auto It = InputNodeMap.begin(); It != InputNodeMap.end(); It++)
    {
	Parameters.push_back(*(Hda.MainNode.GetParameter(It->first)));
    }

    return Parameters;
}

std::vector<HEMAX_InputInstance*>
HEMAX_3dsmaxHda::GetAllParameter3dsmaxInputs()
{
    std::vector<HEMAX_InputInstance*> Instances;

    for (auto It = InputNodeMap.begin(); It != InputNodeMap.end(); It++)
    {
	if (It->second)
	{
	    Instances.push_back(It->second);
	}
    }

    return Instances;
}

void
HEMAX_3dsmaxHda::UpdateParameterInputNode(HEMAX_ParameterId ParamId)
{
    HEMAX_Input* InputNode = nullptr;

    HEMAX_InputInstance* ParameterInput = FindParameterInput(ParamId);
    HEMAX_Parameter* Parameter = Hda.MainNode.GetParameter(ParamId);

    if (ParameterInput)
    {
	InputNode = ParameterInput->MaxInput->GetInputNode();

	if (ParameterInput->MergeNode)
	{
	    Parameter->UpdateInputNode(ParameterInput->MergeNode->GetMergedInputs().Info.id);
	}
    }
}

void
HEMAX_3dsmaxHda::ClearParameterInputNode(HEMAX_ParameterId ParamId)
{
    auto Search = InputNodeMap.find(ParamId);

    if (Search != InputNodeMap.end())
    {
	Search->second = nullptr;
	InputNodeMap.erase(Search);
    }
}

void
HEMAX_3dsmaxHda::SetSubnetworkInput(int Subnetwork, HEMAX_InputInstance* Input)
{
    SubnetworkNodeInputs[Subnetwork] = Input;
}

HEMAX_InputInstance*
HEMAX_3dsmaxHda::FindSubnetworkInput(int Subnetwork)
{
    if (Subnetwork >= 0 && Subnetwork < SubnetworkNodeInputs.size())
    {
	if (SubnetworkNodeInputs[Subnetwork])
	{
	    return SubnetworkNodeInputs[Subnetwork];
	}
    }

    return nullptr;
}

void
HEMAX_3dsmaxHda::UpdateSubnetworkInput(int Subnetwork)
{
    HEMAX_Input* InputNode = nullptr;

    if (SubnetworkNodeInputs[Subnetwork])
    {
	if (SubnetworkNodeInputs[Subnetwork]->MergeNode)
	{
	    InputNode = SubnetworkNodeInputs[Subnetwork]->MaxInput->GetInputNode();
	    HEMAX_Node HapiInputNode = SubnetworkNodeInputs[Subnetwork]->MergeNode->GetMergedInputs();
	    Hda.MainNode.ConnectInputNode(HapiInputNode.Info.id, Subnetwork);
	}
	else
	{
	    Hda.MainNode.DisconnectInputNode(Subnetwork);
	}
    }
}

void
HEMAX_3dsmaxHda::ClearSubnetworkInput(int Subnetwork)
{
    SubnetworkNodeInputs[Subnetwork] = nullptr;
}

void
HEMAX_3dsmaxHda::UpdateMultiParameterList(HEMAX_Parameter Parameter)
{
    ClearParameterCustomAttributes();
    InitializeParameterCustomAttributes();
    UpdateAllCustomAttributes();
}

std::string
HEMAX_3dsmaxHda::GetCustAttribStringValue(std::string CustAttribName, bool& Success)
{
    Success = false;

    ICustAttribContainer* Container = GetCustAttribContainer();
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
HEMAX_3dsmaxHda::InitializeParameterCustomAttributes()
{
    ICustAttribContainer* CustAttribContainer = GetCustAttribContainer();
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustAttribMap = GetCustAttribMap();

    std::vector<HEMAX_Parameter>& Parameters = Hda.MainNode.GetParameters();

    for (auto It = Parameters.begin(); It != Parameters.end(); It++)
    {
	HEMAX_Parameter* Parameter = &(*It);

	switch (Parameter->Type)
	{
	    case (HEMAX_PARAM_INTEGER):
	    {
		std::vector<int> ParameterValues = Parameter->GetIntVals();

		if (Parameter->Info.size == 1)
		{
		    HEMAX_IntegerParameterAttrib* ParmCustAttrib = new HEMAX_IntegerParameterAttrib;
		    ParmCustAttrib->SetParameterName(Parameter->GetName());
		    ParmCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[0]);

		    CustAttribContainer->AppendCustAttrib(ParmCustAttrib);
		    CustAttribMap->insert({ Parameter->GetName(), ParmCustAttrib });
		}
		else if (Parameter->Info.size > 1)
		{
		    for (int z = 0; z < Parameter->Info.size; z++)
		    {
			HEMAX_IntegerParameterAttrib* ParmCustAttrib = new HEMAX_IntegerParameterAttrib;
			std::string CustAttribName = Parameter->GetName() + "__" + std::to_string(z);
			ParmCustAttrib->SetParameterName(CustAttribName);
			ParmCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[z]);

			CustAttribContainer->AppendCustAttrib(ParmCustAttrib);
			CustAttribMap->insert({ Parameter->GetName(), ParmCustAttrib });
		    }
		}
	    } break;
	    case (HEMAX_PARAM_STRING):
	    case (HEMAX_PARAM_PATH_FILE):
	    case (HEMAX_PARAM_PATH_FILE_DIR):
	    case (HEMAX_PARAM_PATH_FILE_GEO):
	    case (HEMAX_PARAM_PATH_FILE_IMAGE):
	    {
		std::vector<std::string> ParameterValues = Parameter->GetStringVals();

		if (Parameter->Info.size == 1)
		{
		    HEMAX_StringParameterAttrib* ParamCustAttrib = new HEMAX_StringParameterAttrib;
		    ParamCustAttrib->SetParameterName(Parameter->GetName());
		    std::wstring WideVal(ParameterValues[0].begin(), ParameterValues[0].end());
		    ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), WideVal.c_str());

		    CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		    CustAttribMap->insert({ Parameter->GetName(), ParamCustAttrib });
		}
		else if (Parameter->Info.size > 1)
		{
		    for (int z = 0; z < Parameter->Info.size; ++z)
		    {
			HEMAX_StringParameterAttrib* ParamCustAttrib = new HEMAX_StringParameterAttrib;
			std::string CustAttribName = Parameter->GetName() + "__" + std::to_string(z);
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
		std::vector<float> ParameterValues = Parameter->GetFloatVals();

		if (Parameter->Info.size == 1)
		{
		    HEMAX_FloatParameterAttrib* ParamCustAttrib = new HEMAX_FloatParameterAttrib;
		    ParamCustAttrib->SetParameterName(Parameter->GetName());
		    ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[0]);

		    CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		    CustAttribMap->insert({ Parameter->GetName(), ParamCustAttrib });
		}
		else if (Parameter->Info.size > 1)
		{
		    for (int z = 0; z < Parameter->Info.size; ++z)
		    {
			HEMAX_FloatParameterAttrib* ParamCustAttrib = new HEMAX_FloatParameterAttrib;
			std::string CustAttribName = Parameter->GetName() + "__" + std::to_string(z);
			ParamCustAttrib->SetParameterName(CustAttribName);
			ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[z]);

			CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
			CustAttribMap->insert({ CustAttribName, ParamCustAttrib });
		    }
		}
	    } break;
	    case (HEMAX_PARAM_TOGGLE):
	    {
		std::vector<int> ParameterValues = Parameter->GetIntVals();

		if (Parameter->Info.size == 1)
		{
		    HEMAX_ToggleParameterAttrib* ParamCustAttrib = new HEMAX_ToggleParameterAttrib;
		    ParamCustAttrib->SetParameterName(Parameter->GetName());
		    ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[0]);

		    CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		    CustAttribMap->insert({ Parameter->GetName(), ParamCustAttrib });
		}
		else if (Parameter->Info.size > 1)
		{
		    for (int z = 0; z < Parameter->Info.size; ++z)
		    {
			HEMAX_ToggleParameterAttrib* ParamCustAttrib = new HEMAX_ToggleParameterAttrib;

			std::string CustAttribName = Parameter->GetName() + "__" + std::to_string(z);
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
		ParamCustAttrib->SetParameterName(Parameter->GetName());
		CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		CustAttribMap->insert({ Parameter->GetName(), ParamCustAttrib });
	    } break;
	    case (HEMAX_PARAM_MULTIPARMLIST):
	    {
		HEMAX_MultiParameterAttrib* ParamCustAttrib = new HEMAX_MultiParameterAttrib;
		ParamCustAttrib->SetParameterName(Parameter->GetName());
		int InstanceCount = Parameter->Info.instanceCount;
		int InstanceLength = Parameter->Info.instanceLength;
		ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), InstanceCount);
		ParamCustAttrib->PBlock->SetValue(1, GetCOREInterface()->GetTime(), InstanceLength);
		CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		CustAttribMap->insert({ Parameter->GetName(), ParamCustAttrib });
	    } break;
	    default:
	    {
		// Do nothing
	    } break;
	}
    }
}

void
HEMAX_3dsmaxHda::UpdateAllCustomAttributes()
{
    for (auto InputIter = InputNodeMap.begin(); InputIter != InputNodeMap.end(); InputIter++)
    {
	if (InputIter->second)
	{
	    HEMAX_Parameter* TheParameter = Hda.MainNode.GetParameter(InputIter->first);
	    UpdateInputNodeCustomAttribute(*TheParameter, InputIter->second->MaxInput->GetInputNode());
	}
    }

    std::vector<HEMAX_Parameter>& NodeParameters = Hda.MainNode.GetParameters();
    for (auto ParmIt = NodeParameters.begin(); ParmIt != NodeParameters.end(); ParmIt++)
    {
	HEMAX_Parameter& Parameter = *ParmIt;

	switch (Parameter.Type)
	{
	    case (HEMAX_PARAM_INTEGER):
	    {
		std::vector<int> IntValues = Parameter.GetIntVals();
		UpdateIntCustomAttribute(Parameter, IntValues);
	    } break;
	    case (HEMAX_PARAM_STRING):
	    case (HEMAX_PARAM_PATH_FILE):
	    case (HEMAX_PARAM_PATH_FILE_DIR):
	    case (HEMAX_PARAM_PATH_FILE_GEO):
	    case (HEMAX_PARAM_PATH_FILE_IMAGE):
	    {
		std::vector<std::string> StringValues = Parameter.GetStringVals();
		UpdateStringCustomAttribute(Parameter, StringValues);
	    } break;
	    case (HEMAX_PARAM_FLOAT):
	    case (HEMAX_PARAM_COLOR):
	    {
		std::vector<float> FloatValues = Parameter.GetFloatVals();
		UpdateFloatCustomAttribute(Parameter, FloatValues);
	    } break;
	    case (HEMAX_PARAM_TOGGLE):
	    {
		std::vector<int> ToggleValues = Parameter.GetIntVals();
		UpdateToggleCustomAttribute(Parameter, ToggleValues);
	    } break;
	    default:
	    {
		// Do nothing
	    } break;
	}
    }
}

void
HEMAX_3dsmaxHda::ClearParameterCustomAttributes()
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap();
    ICustAttribContainer* CustAttribs = GetCustAttribContainer();

    int LowestIndex = -1;
    int NumSubnetworkCustAttribs = -1;

    if (Type == HEMAX_GEOMETRY_HDA)
    {
	LowestIndex = HEMAX_MAX_HOUDINI_MAX_INDEX;
	NumSubnetworkCustAttribs = Hda.MainNode.Info.inputCount;
    }
    else if (Type == HEMAX_MODIFIER_HDA)
    {
	LowestIndex = HEMAX_HOUDINI_MODIFIER_MAX_INDEX;
	NumSubnetworkCustAttribs = Hda.MainNode.Info.inputCount - 1;
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
HEMAX_3dsmaxHda::UpdateIntCustomAttribute(HEMAX_Parameter& Parameter, std::vector<int>& IntValues)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
	std::string ParameterName = Parameter.GetName();
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
	    std::string ParameterName = Parameter.GetName() + "__" + std::to_string(p);
	    auto Search = CustomAttributeMap->find(ParameterName);

	    if (Search != CustomAttributeMap->end())
	    {
		Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), IntValues[p]);
	    }
	}
    }
}

void
HEMAX_3dsmaxHda::UpdateFloatCustomAttribute(HEMAX_Parameter& Parameter, std::vector<float>& FloatValues)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
	std::string ParameterName = Parameter.GetName();
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
	    std::string ParameterName = Parameter.GetName() + "__" + std::to_string(p);
	    auto Search = CustomAttributeMap->find(ParameterName);

	    if (Search != CustomAttributeMap->end())
	    {
		Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), FloatValues[p]);
	    }
	}
    }
}

void
HEMAX_3dsmaxHda::UpdateStringCustomAttribute(HEMAX_Parameter& Parameter, std::vector<std::string>& StringValues)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
	std::string ParameterName = Parameter.GetName();
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
	    std::string ParameterName = Parameter.GetName() + "__" + std::to_string(p);
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
HEMAX_3dsmaxHda::UpdateToggleCustomAttribute(HEMAX_Parameter& Parameter, std::vector<int>& ToggleValues)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
	std::string ParameterName = Parameter.GetName();
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
	    std::string ParameterName = Parameter.GetName() + "__" + std::to_string(p);
	    auto Search = CustomAttributeMap->find(ParameterName);

	    if (Search != CustomAttributeMap->end())
	    {
		Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ToggleValues[p]);
	    }
	}
    }
}

void
HEMAX_3dsmaxHda::UpdateInputNodeCustomAttribute(HEMAX_Parameter& Parameter, HEMAX_Input* InputNode)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap();
    std::string ParameterName = Parameter.GetName();
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
HEMAX_3dsmaxHda::UpdateSubnetworkCustomAttribute(int Subnetwork, HEMAX_Input* InputNode)
{
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap();
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
HEMAX_3dsmaxHda::ReloadSubnetworkInputsFromCustomAttributes()
{
    ICustAttribContainer* CustAttribContainer = GetCustAttribContainer();
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap();

    std::vector<HEMAX_SubnetworkInputMapping> Mapping;

    for (int z = 0; z < Hda.MainNode.Info.inputCount; z++)
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
HEMAX_3dsmaxHda::ReloadParametersFromCustomAttributes()
{
    std::vector<HEMAX_ParameterInputMapping> InputMap;
    std::unordered_map<std::string, HEMAX_ParameterAttrib*>* CustomAttributeMap = GetCustAttribMap();

    bool AnotherPassRequired = false;
    std::unordered_map<std::string, bool> CompletionMap;

    do
    {
	AnotherPassRequired = false;
	std::vector<HEMAX_Parameter>& NodeParameters = Hda.MainNode.GetParameters();
	for (auto ParmIt = NodeParameters.begin(); ParmIt != NodeParameters.end(); ParmIt++)
	{
	    HEMAX_Parameter Parameter(*ParmIt);

	    if (!(Parameter.Info.type >= HAPI_PARMTYPE_NONVALUE_START && Parameter.Info.type <= HAPI_PARMTYPE_NONVALUE_END))
	    {
		std::string ParameterName = Parameter.GetName();
		auto IsCompleted = CompletionMap.find(ParameterName);
		if (IsCompleted == CompletionMap.end())
		{
		    switch (Parameter.Type)
		    {
			case (HEMAX_PARAM_INTEGER):
			{
			    RemakeIntParameterFromCustAttrib(Parameter, *CustomAttributeMap);
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HEMAX_PARAM_STRING):
			case (HEMAX_PARAM_PATH_FILE):
			case (HEMAX_PARAM_PATH_FILE_DIR):
			case (HEMAX_PARAM_PATH_FILE_GEO):
			case (HEMAX_PARAM_PATH_FILE_IMAGE):
			{
			    RemakeStringParameterFromCustAttrib(Parameter, *CustomAttributeMap);
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HEMAX_PARAM_FLOAT):
			case (HEMAX_PARAM_COLOR):
			{
			    RemakeFloatParameterFromCustAttrib(Parameter, *CustomAttributeMap);
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HEMAX_PARAM_TOGGLE):
			{
			    RemakeToggleParameterFromCustAttrib(Parameter, *CustomAttributeMap);
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HEMAX_PARAM_NODE):
			{
			    HEMAX_ParameterInputMapping InputEntry = RemakeInputParameterFromCustAttrib(Parameter, *CustomAttributeMap);
			    if (InputEntry.Node)
			    {
				InputMap.push_back(InputEntry);
			    }
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HEMAX_PARAM_MULTIPARMLIST):
			{
			    AnotherPassRequired = true;
			    RemakeMultiParameter(Parameter, *CustomAttributeMap);
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			default:
			{
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
		    }
		}

                if (AnotherPassRequired)
                {
                    // Updating multiparameters can shift the parameter IDs
                    // so we need to retrieve the parameters again
                    break;
                }
	    }
	}
        if (AnotherPassRequired)
        {
            Hda.MainNode.Cook();
        }
    } while (AnotherPassRequired);

    return InputMap;
}

void
HEMAX_3dsmaxHda::RemakeIntParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    if (Parameter.Info.size == 1)
    {
	std::string ParameterName = Parameter.GetName();
	auto Search = CustomAttributeMap.find(ParameterName);

	if (Search != CustomAttributeMap.end())
	{
	    int IntValue;
	    Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), IntValue, FOREVER);

	    std::vector<int> IntValues = { IntValue };
	    Parameter.UpdateIntVals(IntValues);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	std::vector<int> IntValues;
	for (int p = 0; p < Parameter.Info.size; p++)
	{
	    std::string ParameterName = Parameter.GetName() + "__" + std::to_string(p);
	    auto Search = CustomAttributeMap.find(ParameterName);

	    if (Search != CustomAttributeMap.end())
	    {
		int IntValue;
		Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), IntValue, FOREVER);
		IntValues.push_back(IntValue);
	    }
	}
	Parameter.UpdateIntVals(IntValues);
    }
}

void
HEMAX_3dsmaxHda::RemakeStringParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    if (Parameter.Info.size == 1)
    {
	std::string ParameterName = Parameter.GetName();
	auto Search = CustomAttributeMap.find(ParameterName);

	if (Search != CustomAttributeMap.end())
	{
	    const MCHAR* Val;
	    Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), Val, FOREVER);

            std::string StringValue = HEMAX_Utilities::WideStringToStringUnsafe(
                Val);
	    std::vector<std::string> StringValues = { StringValue };

	    Parameter.UpdateStringVals(StringValues);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	std::vector<std::string> StringValues;
	for (int p = 0; p < Parameter.Info.size; p++)
	{
	    std::string ParameterName = Parameter.GetName() + "__" + std::to_string(p);
	    auto Search = CustomAttributeMap.find(ParameterName);

	    if (Search != CustomAttributeMap.end())
	    {
		const MCHAR* Val;
		Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), Val, FOREVER);

		std::string StringValue
			= HEMAX_Utilities::WideStringToStringUnsafe(Val);
		StringValues.push_back(StringValue);
	    }
	}
	Parameter.UpdateStringVals(StringValues);
    }
}

void
HEMAX_3dsmaxHda::RemakeFloatParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    if (Parameter.Info.size == 1)
    {
	std::string ParameterName = Parameter.GetName();
	auto Search = CustomAttributeMap.find(ParameterName);

	if (Search != CustomAttributeMap.end())
	{
	    float FloatValue;
	    Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), FloatValue, FOREVER);

	    std::vector<float> FloatValues = { FloatValue };
	    Parameter.UpdateFloatVals(FloatValues);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	std::vector<float> FloatValues;

	for (int p = 0; p < Parameter.Info.size; ++p)
	{
	    std::string ParameterName = Parameter.GetName() + "__" + std::to_string(p);
	    auto Search = CustomAttributeMap.find(ParameterName);

	    if (Search != CustomAttributeMap.end())
	    {
		float FloatValue;
		Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(), FloatValue, FOREVER);
		FloatValues.push_back(FloatValue);
	    }
	}
	Parameter.UpdateFloatVals(FloatValues);
    }
}

void
HEMAX_3dsmaxHda::RemakeToggleParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    std::string ParameterName = Parameter.GetName();
    auto Search = CustomAttributeMap.find(ParameterName);

    if (Parameter.Info.size == 1)
    {
	std::string ParameterName = Parameter.GetName();
	auto Search = CustomAttributeMap.find(ParameterName);

	if (Search != CustomAttributeMap.end())
	{
	    int ToggleValue;
	    Search->second->PBlock->GetValue(0, 0, ToggleValue, FOREVER);

	    std::vector<int> ToggleValues = { ToggleValue };
	    Parameter.UpdateIntVals(ToggleValues);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	std::vector<int> IntValues;

	for (int p = 0; p < Parameter.Info.size; ++p)
	{
	    std::string ParameterName = Parameter.GetName() + "__" + std::to_string(p);
	    auto Search = CustomAttributeMap.find(ParameterName);

	    if (Search != CustomAttributeMap.end())
	    {
		int IntValue;
		Search->second->PBlock->GetValue(0, 0, IntValue, FOREVER);
		IntValues.push_back(IntValue);
	    }
	}
	Parameter.UpdateIntVals(IntValues);
    }
}

HEMAX_ParameterInputMapping
HEMAX_3dsmaxHda::RemakeInputParameterFromCustAttrib(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    HEMAX_ParameterInputMapping Entry;
    Entry.Node = nullptr;
    Entry.ParameterName = "";

    std::string ParameterName = Parameter.GetName();
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
HEMAX_3dsmaxHda::RemakeMultiParameter(HEMAX_Parameter Parameter, std::unordered_map<std::string, HEMAX_ParameterAttrib*>& CustomAttributeMap)
{
    std::string ParameterName = Parameter.GetName();
    auto Search = CustomAttributeMap.find(ParameterName);
    if (Search != CustomAttributeMap.end())
    {
	HEMAX_MultiParameterAttrib* CustAttrib =
            (HEMAX_MultiParameterAttrib*)Search->second;

	int SavedInstanceCount = CustAttrib->PBlock->GetInt(0);
        int CurrentInstanceCount = Parameter.Info.instanceCount;

        if (CurrentInstanceCount > SavedInstanceCount)
        {
            while (CurrentInstanceCount > SavedInstanceCount)
            {
                Parameter.RemoveInstance(CurrentInstanceCount - 1);
                --CurrentInstanceCount;
            }
        }
        else if (CurrentInstanceCount < SavedInstanceCount)
        {
            while (CurrentInstanceCount < SavedInstanceCount)
            {
                Parameter.InsertInstance(CurrentInstanceCount);
                ++CurrentInstanceCount;
            }
        }
    }
}

std::string
HEMAX_3dsmaxHda::GetHardcodedHdaAssetPath()
{
    bool Success;
    if (Type == HEMAX_GEOMETRY_HDA)
    {
	return GetCustAttribStringValue(HEMAX_MAX_HOUDINI_ASSET_PATH_NAME, Success);
    }
    else if (Type == HEMAX_MODIFIER_HDA)
    {
	return GetCustAttribStringValue(HEMAX_HOUDINI_MODIFIER_ASSET_PATH_NAME, Success);
    }

    return "";
}

void
HEMAX_3dsmaxHda::SetHardcodedHdaAssetPath(std::string AssetPath)
{
    HEMAX_StringParameterAttrib* AssetPathAttrib = nullptr;

    if (Type == HEMAX_GEOMETRY_HDA)
    {
	ICustAttribContainer* Container = GetCustAttribContainer();
	AssetPathAttrib = GetStringParameterAttrib(Container, HEMAX_MAX_HOUDINI_ASSET_PATH_NAME);
    }
    else if (Type == HEMAX_MODIFIER_HDA)
    {
	ICustAttribContainer* Container = GetCustAttribContainer();
	AssetPathAttrib = GetStringParameterAttrib(Container, HEMAX_HOUDINI_MODIFIER_ASSET_PATH_NAME);
    }

    if (AssetPathAttrib)
    {
	AssetPathAttrib->SetStringValue(AssetPath);
    }
}

void
HEMAX_3dsmaxHda::CopyAllParameterValues(HEMAX_3dsmaxHda& Source)
{
    if (Hda.HdaAsset.Id != Source.Hda.HdaAsset.Id)
    {
	std::string Msg = "Attempting to copy parameter values from HDA " + Source.Hda.HdaAsset.Names[Source.Hda.HdaAssetIndex] +
	    " to " + Hda.HdaAsset.Names[Hda.HdaAssetIndex] + ", but they are different assets";
	HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_ERROR);

	return;
    }

    std::vector<HEMAX_Parameter>& SourceParms = Source.Hda.MainNode.GetParameters();

    bool Finished = true;
    int ParmIndex = 0;

    do
    {
	Finished = true;
	for (int p = ParmIndex; p < SourceParms.size(); p++)
	{
	    HEMAX_Parameter* DestParm = Hda.MainNode.GetParameter(SourceParms[p].GetName());
	    if (DestParm)
	    {
		DestParm->CopyValuesFrom(SourceParms[p]);
		if (DestParm->IsMultiParameter())
		{
		    Hda.MainNode.Cook();
		    Finished = false;
		    ParmIndex = p + 1;
		    break;
		}
	    }
	}
    } while (!Finished);
}

void
HEMAX_3dsmaxHda::SetCustomAttributeContainer(ICustAttribContainer* Container)
{
    CustomAttributes = Container;
}

ICustAttribContainer*
HEMAX_3dsmaxHda::GetCustAttribContainer()
{
    return CustomAttributes;
}

std::unordered_map<std::string, HEMAX_ParameterAttrib*>*
HEMAX_3dsmaxHda::GetCustAttribMap()
{
    return &CustomAttributeMap;
}
