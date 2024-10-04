#include "HEMAX_3dsmaxHda.h"

#include "HEMAX_HoudiniApi.h"
#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"

void
HEMAX_3dsmaxHda::Cook3dsmaxHda()
{
    Hda.MainNode.Cook();

    if (Hda.MainNode.ShouldCookTwice())
    {
	Hda.MainNode.Cook();
    }
}

HEMAX_InputInstance*
HEMAX_3dsmaxHda::InitParameterInput(HEMAX_Parameter& Parm)
{
    if (FindParameterInput(Parm))
        return nullptr;

    auto Result = InputNodeMap.emplace(std::piecewise_construct,
            std::forward_as_tuple(Parm.Info.id),
            std::forward_as_tuple(Hda.MainNode, Parm.GetName()));

    if (!Result.second)
        return nullptr;;

    return &(Result.first->second);
}

HEMAX_InputInstance*
HEMAX_3dsmaxHda::FindParameterInput(const HEMAX_Parameter& Parm)
{
    auto Search = InputNodeMap.find(Parm.Info.id);

    if (Search != InputNodeMap.end())
    {
	return &(Search->second);
    }

    return nullptr;
}

void
HEMAX_3dsmaxHda::ClearParameterInput(HEMAX_Parameter* Parameter)
{
    auto Search = InputNodeMap.find(Parameter->Info.id);

    if (Search == InputNodeMap.end())
        return;

    InputNodeMap.erase(Parameter->Info.id);

    INodeTab NoInputNodes;
    UpdateInputNodeCustomAttribute(*Parameter, NoInputNodes);
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
	Instances.push_back(&It->second);
    }

    return Instances;
}

void
HEMAX_3dsmaxHda::UpdateParameterInputNode(HEMAX_Parameter& Parm)
{
    HEMAX_InputInstance* ParameterInput = FindParameterInput(Parm);

    if (!ParameterInput)
        return;

    Parm.UpdateInputNode(ParameterInput->GetMergedInputs().Info.id);
}

void
HEMAX_3dsmaxHda::ResetParameters()
{
    SubnetworkNodeInputs.clear();
    InputNodeMap.clear();

    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    auto&& Parameters = Hda.MainNode.GetParameters();
    for (auto&& Parameter : Parameters)
    {
        if (!(Parameter.Type >= HAPI_PARMTYPE_CONTAINER_START &&
              Parameter.Type <= HAPI_PARMTYPE_CONTAINER_END) &&
            !(Parameter.Type >= HAPI_PARMTYPE_NONVALUE_START &&
              Parameter.Type <= HAPI_PARMTYPE_NONVALUE_END))
        {
            HEMAX_HoudiniApi::RevertParmToDefaults(&SM.Session,
                Hda.MainNode.Info.id, Parameter.GetName().c_str());
        }
    }

    ClearParameterCustomAttributes();
    InitializeParameterCustomAttributes();
}

bool
HEMAX_3dsmaxHda::HasSubnetworkInput(int Subnetwork)
{
    auto Search = SubnetworkNodeInputs.find(Subnetwork);
    return Search != SubnetworkNodeInputs.end();
}

HEMAX_InputInstance*
HEMAX_3dsmaxHda::GetSubnetworkInput(int Subnetwork)
{
    if (Subnetwork >= Hda.MainNode.Info.inputCount)
        return nullptr;

    if (!HasSubnetworkInput(Subnetwork))
        SubnetworkNodeInputs.emplace(std::piecewise_construct,
                std::forward_as_tuple(Subnetwork),
                std::forward_as_tuple(Hda.MainNode, Subnetwork));

    auto Search = SubnetworkNodeInputs.find(Subnetwork);

    if (Search != SubnetworkNodeInputs.end())
        return &Search->second;
    else
        return nullptr;
}

void
HEMAX_3dsmaxHda::UpdateSubnetworkInput(int Subnetwork)
{
    auto Search = SubnetworkNodeInputs.find(Subnetwork);
    if (Search == SubnetworkNodeInputs.end())
    {
        Hda.MainNode.DisconnectInputNode(Subnetwork);
    }
    else
    {
        HEMAX_Node& InputNode = Search->second.GetMergedInputs();
        Hda.MainNode.ConnectInputNode(InputNode.Info.id, Subnetwork);
    }    
}

void
HEMAX_3dsmaxHda::ClearSubnetworkInput(int Subnetwork)
{
    SubnetworkNodeInputs.erase(Subnetwork);
    UpdateSubnetworkInput(Subnetwork);

    INodeTab NoNodes;
    UpdateSubnetworkCustomAttribute(Subnetwork, NoNodes);
}

void
HEMAX_3dsmaxHda::RemoveAllUsages(HEMAX_3dsMaxInput* Input)
{
    for (auto It = SubnetworkNodeInputs.begin();
         It != SubnetworkNodeInputs.end();)
    {
        It->second.RemoveInput(Input);

        if (It->second.GetMergedInputCount() == 0)
            It = SubnetworkNodeInputs.erase(It);
        else
            It++;
    }

    for (auto It = InputNodeMap.begin(); It != InputNodeMap.end();)
    {
        It->second.RemoveInput(Input);

        if (It->second.GetMergedInputCount() == 0)
            It = InputNodeMap.erase(It);
        else
            It++;
    } 
}

void
HEMAX_3dsmaxHda::RefreshInputConnections()
{
    for (auto&& InputIter = SubnetworkNodeInputs.begin();
         InputIter != SubnetworkNodeInputs.end();
         InputIter++)
    {
        InputIter->second.RefreshConnection();
    }

    for (auto&& InputIter = InputNodeMap.begin();
         InputIter != InputNodeMap.end();
         InputIter++)
    {
        InputIter->second.RefreshConnection();
    }
}

void
HEMAX_3dsmaxHda::UpdateMultiParameterList(HEMAX_Parameter Parameter)
{
    ClearParameterCustomAttributes();
    InitializeParameterCustomAttributes();
    UpdateAllCustomAttributes();
}

std::string
HEMAX_3dsmaxHda::GetCustAttribStringValue(const TSTR& CustAttribName,
        bool& Success)
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
    auto&& CustAttribMap = GetCustAttribMap();

    std::vector<HEMAX_Parameter>& Parameters = Hda.MainNode.GetParameters();

    for (auto It = Parameters.begin(); It != Parameters.end(); It++)
    {
	HEMAX_Parameter* Parameter = &(*It);
        TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter->GetName());

	switch (Parameter->Type)
	{
	    case (HAPI_PARMTYPE_INT):
	    {
		std::vector<int> ParameterValues = Parameter->GetIntVals();

		if (Parameter->Info.size == 1)
		{
		    HEMAX_IntegerParameterAttrib* ParmCustAttrib = new HEMAX_IntegerParameterAttrib;
		    ParmCustAttrib->SetParameterName(ParameterName);
		    ParmCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[0]);

		    CustAttribContainer->AppendCustAttrib(ParmCustAttrib);
		    CustAttribMap->insert({ ParameterName.data(), ParmCustAttrib });
		}
		else if (Parameter->Info.size > 1)
		{
		    for (int z = 0; z < Parameter->Info.size; z++)
		    {
			HEMAX_IntegerParameterAttrib* ParmCustAttrib = new HEMAX_IntegerParameterAttrib;
                        TSTR CustAttribName;
                        CustAttribName.printf(_T("%s__%d"), ParameterName.data(), z);
			ParmCustAttrib->SetParameterName(CustAttribName);
			ParmCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[z]);

			CustAttribContainer->AppendCustAttrib(ParmCustAttrib);
			CustAttribMap->insert({ CustAttribName.data(), ParmCustAttrib });
		    }
		}
	    } break;
	    case (HAPI_PARMTYPE_STRING):
	    case (HAPI_PARMTYPE_PATH_FILE):
	    case (HAPI_PARMTYPE_PATH_FILE_DIR):
	    case (HAPI_PARMTYPE_PATH_FILE_GEO):
	    case (HAPI_PARMTYPE_PATH_FILE_IMAGE):
	    {
		std::vector<std::string> ParameterValues = Parameter->GetStringVals();

		if (Parameter->Info.size == 1)
		{
		    HEMAX_StringParameterAttrib* ParamCustAttrib = new HEMAX_StringParameterAttrib;
		    ParamCustAttrib->SetParameterName(ParameterName);
                    // TODO: 001 fix
		    std::wstring WideVal(ParameterValues[0].begin(), ParameterValues[0].end());
		    ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), WideVal.c_str());

		    CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		    CustAttribMap->insert({ ParameterName.data(), ParamCustAttrib });
		}
		else if (Parameter->Info.size > 1)
		{
		    for (int z = 0; z < Parameter->Info.size; ++z)
		    {
			HEMAX_StringParameterAttrib* ParamCustAttrib = new HEMAX_StringParameterAttrib;
                        TSTR CustAttribName;
                        CustAttribName.printf(_T("%s__%d"), ParameterName.data(), z);
			ParamCustAttrib->SetParameterName(CustAttribName);
                        // TODO: 001 fix
			std::wstring WideVal(ParameterValues[z].begin(), ParameterValues[z].end());
			ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), WideVal.c_str());

			CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
			CustAttribMap->insert({ ParameterName.data(), ParamCustAttrib });
		    }
		}
	    } break;
	    case (HAPI_PARMTYPE_FLOAT):
	    case (HAPI_PARMTYPE_COLOR):
	    {
		std::vector<float> ParameterValues = Parameter->GetFloatVals();

		if (Parameter->Info.size == 1)
		{
		    HEMAX_FloatParameterAttrib* ParamCustAttrib = new HEMAX_FloatParameterAttrib;
		    ParamCustAttrib->SetParameterName(ParameterName);
		    ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[0]);

		    CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		    CustAttribMap->insert({ ParameterName.data(), ParamCustAttrib });
		}
		else if (Parameter->Info.size > 1)
		{
		    for (int z = 0; z < Parameter->Info.size; ++z)
		    {
			HEMAX_FloatParameterAttrib* ParamCustAttrib = new HEMAX_FloatParameterAttrib;
                        TSTR CustAttribName;
                        CustAttribName.printf(_T("%s__%d"),
                            ParameterName.data(), z);
			ParamCustAttrib->SetParameterName(CustAttribName);
			ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[z]);

			CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
			CustAttribMap->insert({ CustAttribName.data(), ParamCustAttrib });
		    }
		}
	    } break;
	    case (HAPI_PARMTYPE_TOGGLE):
	    {
		std::vector<int> ParameterValues = Parameter->GetIntVals();

		if (Parameter->Info.size == 1)
		{
		    HEMAX_ToggleParameterAttrib* ParamCustAttrib = new HEMAX_ToggleParameterAttrib;
		    ParamCustAttrib->SetParameterName(ParameterName);
		    ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[0]);

		    CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		    CustAttribMap->insert({ ParameterName.data(), ParamCustAttrib });
		}
		else if (Parameter->Info.size > 1)
		{
		    for (int z = 0; z < Parameter->Info.size; ++z)
		    {
			HEMAX_ToggleParameterAttrib* ParamCustAttrib = new HEMAX_ToggleParameterAttrib;

                        TSTR CustAttribName;
                        CustAttribName.printf(_T("%s__%d"),
                            ParameterName.data(), z);
			ParamCustAttrib->SetParameterName(CustAttribName);
			ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ParameterValues[z]);

			CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
			CustAttribMap->insert({ CustAttribName.data(), ParamCustAttrib });
		    }
		}
	    } break;
	    case (HAPI_PARMTYPE_NODE):
	    {
		HEMAX_NodeListParameterAttrib* ParamCustAttrib =
                    new HEMAX_NodeListParameterAttrib;
		ParamCustAttrib->SetParameterName(ParameterName);
		CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		CustAttribMap->insert({ ParameterName.data(), ParamCustAttrib });
	    } break;
	    case (HAPI_PARMTYPE_MULTIPARMLIST):
	    {
		HEMAX_MultiParameterAttrib* ParamCustAttrib = new HEMAX_MultiParameterAttrib;
		ParamCustAttrib->SetParameterName(ParameterName);
		int InstanceCount = Parameter->Info.instanceCount;
		int InstanceLength = Parameter->Info.instanceLength;
		ParamCustAttrib->PBlock->SetValue(0, GetCOREInterface()->GetTime(), InstanceCount);
		ParamCustAttrib->PBlock->SetValue(1, GetCOREInterface()->GetTime(), InstanceLength);
		CustAttribContainer->AppendCustAttrib(ParamCustAttrib);
		CustAttribMap->insert({ ParameterName.data(), ParamCustAttrib });
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
    for (auto&& InputIter = InputNodeMap.begin();
         InputIter != InputNodeMap.end();
         InputIter++)
    {
        HEMAX_Parameter* Parameter = Hda.MainNode.GetParameter(InputIter->first);
        auto&& MaxInputs = InputIter->second.GetMaxInputs();

        INodeTab InputNodes;

        for (auto&& Input : MaxInputs)
        {
            InputNodes.AppendNode(GetCOREInterface()->GetINodeByHandle(
                Input->Get3dsMaxNodeHandle()));
        }
        UpdateInputNodeCustomAttribute(*Parameter, InputNodes);
    }

    std::vector<HEMAX_Parameter>& NodeParameters = Hda.MainNode.GetParameters();
    for (auto ParmIt = NodeParameters.begin(); ParmIt != NodeParameters.end(); ParmIt++)
    {
	HEMAX_Parameter& Parameter = *ParmIt;

	switch (Parameter.Type)
	{
	    case (HAPI_PARMTYPE_INT):
	    {
		std::vector<int> IntValues = Parameter.GetIntVals();
		UpdateIntCustomAttribute(Parameter, IntValues);
	    } break;
	    case (HAPI_PARMTYPE_STRING):
	    case (HAPI_PARMTYPE_PATH_FILE):
	    case (HAPI_PARMTYPE_PATH_FILE_DIR):
	    case (HAPI_PARMTYPE_PATH_FILE_GEO):
	    case (HAPI_PARMTYPE_PATH_FILE_IMAGE):
	    {
		std::vector<std::string> StringValues = Parameter.GetStringVals();
		UpdateStringCustomAttribute(Parameter, StringValues);
	    } break;
	    case (HAPI_PARMTYPE_FLOAT):
	    case (HAPI_PARMTYPE_COLOR):
	    {
		std::vector<float> FloatValues = Parameter.GetFloatVals();
		UpdateFloatCustomAttribute(Parameter, FloatValues);
	    } break;
	    case (HAPI_PARMTYPE_TOGGLE):
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
    auto&& CustomAttributeMap = GetCustAttribMap();
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
    auto&& CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
        TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());
	auto Search = CustomAttributeMap->find(ParameterName.data());

	if (Search != CustomAttributeMap->end())
	{
	    Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), IntValues[0]);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	for (int p = 0; p < Parameter.Info.size; p++)
	{
            TSTR ParameterName;
            ParameterName.printf(_T("%s__%d"),
                    HEMAX_Utilities::ToTSTR(Parameter.GetName()).data(), p); 
	    auto Search = CustomAttributeMap->find(ParameterName.data());

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
    auto&& CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
        TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());
	auto Search = CustomAttributeMap->find(ParameterName.data());

	if (Search != CustomAttributeMap->end())
	{
	    Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), FloatValues[0]);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	for (int p = 0; p < Parameter.Info.size; p++)
	{
            TSTR ParameterName;
            ParameterName.printf(_T("%s__%d"),
                    HEMAX_Utilities::ToTSTR(Parameter.GetName()).data(), p); 
	    auto Search = CustomAttributeMap->find(ParameterName.data());

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
    auto&& CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
        TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());
	auto Search = CustomAttributeMap->find(ParameterName.data());

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
            TSTR ParameterName;
            ParameterName.printf(_T("%s__%d"),
                    HEMAX_Utilities::ToTSTR(Parameter.GetName()).data(), p); 
	    auto Search = CustomAttributeMap->find(ParameterName.data());

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
    auto&& CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
        TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());
	auto Search = CustomAttributeMap->find(ParameterName.data());

	if (Search != CustomAttributeMap->end())
	{
	    Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ToggleValues[0]);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	for (int p = 0; p < Parameter.Info.size; p++)
	{
            TSTR ParameterName;
            ParameterName.printf(_T("%s__%d"),
                    HEMAX_Utilities::ToTSTR(Parameter.GetName()).data(), p); 

	    auto Search = CustomAttributeMap->find(ParameterName.data());

	    if (Search != CustomAttributeMap->end())
	    {
		Search->second->PBlock->SetValue(0, GetCOREInterface()->GetTime(), ToggleValues[p]);
	    }
	}
    }
}

void
HEMAX_3dsmaxHda::UpdateInputNodeCustomAttribute(
        HEMAX_Parameter& Parameter,
        const INodeTab& InputNodes)
{
    auto&& CustAttribMap = GetCustAttribMap();

    TSTR ParmName = HEMAX_Utilities::ToTSTR(Parameter.GetName());  
    auto Search = CustAttribMap->find(ParmName.data());

    if (Search == CustAttribMap->end())
        return;

    HEMAX_NodeListParameterAttrib* Attrib =
        dynamic_cast<HEMAX_NodeListParameterAttrib*>(Search->second);

    if (!Attrib)
        return;

    Attrib->SetMessagesBlocked(true);
    Attrib->PBlock->ZeroCount(0);

    for (int i = 0; i < InputNodes.Count(); ++i)
    {
        Attrib->PBlock->Append(0, 1, &InputNodes[i]);
    }

    Attrib->SetMessagesBlocked(false);
}

void
HEMAX_3dsmaxHda::UpdateSubnetworkCustomAttribute(
        int Subnetwork,
        const INodeTab& InputNodes)
{
    auto&& CustAttribMap = GetCustAttribMap();
    TSTR SubnetworkName;
    SubnetworkName.printf(_T("subnetwork_%d"), Subnetwork);
    auto Search = CustAttribMap->find(SubnetworkName.data());

    if (Search == CustAttribMap->end())
        return;

    HEMAX_NodeListParameterAttrib* Attrib =
        dynamic_cast<HEMAX_NodeListParameterAttrib*>(Search->second);
    
    if (!Attrib)
        return;

    Attrib->SetMessagesBlocked(true);
    Attrib->PBlock->ZeroCount(0);

    for (int i = 0; i < InputNodes.Count(); ++i)
    {
        Attrib->PBlock->Append(0, 1, &InputNodes[i]);
    }

    Attrib->SetMessagesBlocked(false);
}

std::vector<HEMAX_SubnetworkInputMapping>
HEMAX_3dsmaxHda::ReloadSubnetworkInputsFromCustomAttributes()
{
    std::vector<HEMAX_SubnetworkInputMapping> Mapping;

    auto&& CustAttribMap = GetCustAttribMap();

    for (int z = 0; z < Hda.MainNode.Info.inputCount; z++)
    {
        HEMAX_SubnetworkInputMapping Entry;
        Entry.Subnetwork = z;

        TSTR SubnetworkSearch;
        SubnetworkSearch.printf(_T("subnetwork_%d"), z);
        auto Search = CustAttribMap->find(SubnetworkSearch.data());

        if (Search == CustAttribMap->end())
        {
            Mapping.push_back(Entry);
            continue;
        }

        // If this is true, this means that an old scene file from before multi
        // inputs were supported is being loaded. We must load the input node
        // and then convert the custom attribute from a
        // HEMAX_NodeParameterAttrib to a HEMAX_NodeListParameterAttrib
        if (dynamic_cast<HEMAX_NodeParameterAttrib*>(Search->second))
        {
            INode* Node = Search->second->PBlock->GetINode(0);
            Entry.Nodes.push_back(Node);

            HEMAX_NodeListParameterAttrib* ReplacementAttrib =
                CreateNodeListCustAttrib(z);

            if (Node)
                ReplacementAttrib->PBlock->Append(0, 1, &Node);

            for (int p = 0; p < CustomAttributes->GetNumCustAttribs(); ++p)
            {
                if (CustomAttributes->GetCustAttrib(p) == Search->second)
                {
                    CustomAttributes->SetCustAttrib(p, ReplacementAttrib);
                    break;
                }
            }

            delete Search->second;
            Search->second = ReplacementAttrib; 
        }
        else
        {
            for (int i = 0; i < Search->second->PBlock->Count(0); ++i)
            {
                INode* Node = nullptr;
                Interval ValidityInterval;
                Search->second->PBlock->GetValue(0, 0, Node,
                    ValidityInterval, i);
                Entry.Nodes.push_back(Node);
            }
        }

        Mapping.push_back(Entry);
    }

    return Mapping;
}

std::vector<HEMAX_ParameterInputMapping>
HEMAX_3dsmaxHda::ReloadParametersFromCustomAttributes()
{
    std::vector<HEMAX_ParameterInputMapping> InputMap;
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
			case (HAPI_PARMTYPE_INT):
			{
			    RemakeIntParameterFromCustAttrib(Parameter);
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HAPI_PARMTYPE_STRING):
			case (HAPI_PARMTYPE_PATH_FILE):
			case (HAPI_PARMTYPE_PATH_FILE_DIR):
			case (HAPI_PARMTYPE_PATH_FILE_GEO):
			case (HAPI_PARMTYPE_PATH_FILE_IMAGE):
			{
			    RemakeStringParameterFromCustAttrib(Parameter);
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HAPI_PARMTYPE_FLOAT):
			case (HAPI_PARMTYPE_COLOR):
			{
			    RemakeFloatParameterFromCustAttrib(Parameter);
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HAPI_PARMTYPE_TOGGLE):
			{
			    RemakeToggleParameterFromCustAttrib(Parameter);
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HAPI_PARMTYPE_NODE):
			{
			    HEMAX_ParameterInputMapping InputEntry =
                                RemakeInputParameterFromCustAttrib(Parameter);
			    if (InputEntry.Nodes.size() > 0)
			    {
				InputMap.push_back(InputEntry);
			    }
			    CompletionMap.insert({ ParameterName, true });
			    break;
			}
			case (HAPI_PARMTYPE_MULTIPARMLIST):
			{
			    AnotherPassRequired = true;
			    RemakeMultiParameter(Parameter);
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
HEMAX_3dsmaxHda::RemakeIntParameterFromCustAttrib(HEMAX_Parameter Parameter)
{
    auto&& CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
        TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());
	auto Search = CustomAttributeMap->find(ParameterName.data());

	if (Search != CustomAttributeMap->end())
	{
	    int IntValue;
            Interval Forever = FOREVER;
	    Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(),
                IntValue, Forever);

	    std::vector<int> IntValues = { IntValue };
	    Parameter.UpdateIntVals(IntValues);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	std::vector<int> IntValues;
	for (int p = 0; p < Parameter.Info.size; p++)
	{
            TSTR ParameterName;
            ParameterName.printf(_T("%s__%d"),
                    HEMAX_Utilities::ToTSTR(Parameter.GetName()).data(), p); 
	    auto Search = CustomAttributeMap->find(ParameterName.data());

	    if (Search != CustomAttributeMap->end())
	    {
		int IntValue;
                Interval Forever = FOREVER;
		Search->second->PBlock->GetValue(0,
                    GetCOREInterface()->GetTime(), IntValue, Forever);
		IntValues.push_back(IntValue);
	    }
	}
	Parameter.UpdateIntVals(IntValues);
    }
}

void
HEMAX_3dsmaxHda::RemakeStringParameterFromCustAttrib(HEMAX_Parameter Parameter)
{
    auto&& CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
	TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());
	auto Search = CustomAttributeMap->find(ParameterName.data());

	if (Search != CustomAttributeMap->end())
	{
	    const MCHAR* Val;
            Interval Forever = FOREVER;
	    Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(),
                Val, Forever);

            std::string StringValue = HEMAX_Utilities::GetUtf8String(Val);
	    std::vector<std::string> StringValues = { StringValue };

	    Parameter.UpdateStringVals(StringValues);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	std::vector<std::string> StringValues;
	for (int p = 0; p < Parameter.Info.size; p++)
	{
            TSTR ParameterName;
            ParameterName.printf(_T("%s__%d"),
                    HEMAX_Utilities::ToTSTR(Parameter.GetName()).data(), p); 

	    auto Search = CustomAttributeMap->find(ParameterName.data());

	    if (Search != CustomAttributeMap->end())
	    {
		const MCHAR* Val;
                Interval Forever = FOREVER;
		Search->second->PBlock->GetValue(0,
                    GetCOREInterface()->GetTime(), Val, Forever);

		std::string StringValue = HEMAX_Utilities::GetUtf8String(Val);
		StringValues.push_back(StringValue);
	    }
	}
	Parameter.UpdateStringVals(StringValues);
    }
}

void
HEMAX_3dsmaxHda::RemakeFloatParameterFromCustAttrib(HEMAX_Parameter Parameter)
{
    auto&& CustomAttributeMap = GetCustAttribMap();

    if (Parameter.Info.size == 1)
    {
	TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());
	auto Search = CustomAttributeMap->find(ParameterName.data());

	if (Search != CustomAttributeMap->end())
	{
	    float FloatValue;
            Interval Forever = FOREVER;
	    Search->second->PBlock->GetValue(0, GetCOREInterface()->GetTime(),
                FloatValue, Forever);

	    std::vector<float> FloatValues = { FloatValue };
	    Parameter.UpdateFloatVals(FloatValues);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	std::vector<float> FloatValues;

	for (int p = 0; p < Parameter.Info.size; ++p)
	{
            TSTR ParameterName;
            ParameterName.printf(_T("%s__%d"),
                    HEMAX_Utilities::ToTSTR(Parameter.GetName()).data(), p); 
	    auto Search = CustomAttributeMap->find(ParameterName.data());

	    if (Search != CustomAttributeMap->end())
	    {
		float FloatValue;
                Interval Forever = FOREVER;
		Search->second->PBlock->GetValue(0,
                    GetCOREInterface()->GetTime(), FloatValue, Forever);
		FloatValues.push_back(FloatValue);
	    }
	}
	Parameter.UpdateFloatVals(FloatValues);
    }
}

void
HEMAX_3dsmaxHda::RemakeToggleParameterFromCustAttrib(HEMAX_Parameter Parameter)
{
    auto&& CustomAttributeMap = GetCustAttribMap();

    TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());

    if (Parameter.Info.size == 1)
    {
	auto Search = CustomAttributeMap->find(ParameterName.data());

	if (Search != CustomAttributeMap->end())
	{
	    int ToggleValue;
            Interval Forever = FOREVER;
	    Search->second->PBlock->GetValue(0, 0, ToggleValue, Forever);

	    std::vector<int> ToggleValues = { ToggleValue };
	    Parameter.UpdateIntVals(ToggleValues);
	}
    }
    else if (Parameter.Info.size > 1)
    {
	std::vector<int> IntValues;

	for (int p = 0; p < Parameter.Info.size; ++p)
	{
            TSTR ParameterName;
            ParameterName.printf(_T("%s__%d"),
                    HEMAX_Utilities::ToTSTR(Parameter.GetName()).data(), p); 
	    auto Search = CustomAttributeMap->find(ParameterName.data());

	    if (Search != CustomAttributeMap->end())
	    {
		int IntValue;
                Interval Forever = FOREVER;
		Search->second->PBlock->GetValue(0, 0, IntValue, Forever);
		IntValues.push_back(IntValue);
	    }
	}
	Parameter.UpdateIntVals(IntValues);
    }
}

HEMAX_ParameterInputMapping
HEMAX_3dsmaxHda::RemakeInputParameterFromCustAttrib(HEMAX_Parameter Parameter)
{
    auto&& CustomAttributeMap = GetCustAttribMap();

    HEMAX_ParameterInputMapping Entry;

    TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());
    auto Search = CustomAttributeMap->find(ParameterName.data());

    if (Search == CustomAttributeMap->end())
        return Entry;

    Entry.ParameterName = Parameter.GetName();

    // If this is true, this means that an old scene file from before multi
    // inputs were supported is being loaded. We must load the input node and
    // then convert the custom attribute from a HEMAX_NodeParameterAttrib to a
    // HEMAX_NodeListParameterAttrib
    if (dynamic_cast<HEMAX_NodeParameterAttrib*>(Search->second))
    {
        INode* Node = Search->second->PBlock->GetINode(0);
        Entry.Nodes.push_back(Node);

        HEMAX_NodeListParameterAttrib* ReplacementAttrib =
            new HEMAX_NodeListParameterAttrib;
        ReplacementAttrib->SetParameterName(ParameterName);

        if (Node)
            ReplacementAttrib->PBlock->Append(0, 1, &Node);

        for (int p = 0; p < CustomAttributes->GetNumCustAttribs(); ++p)
        {
            if (CustomAttributes->GetCustAttrib(p) == Search->second)
            {
                CustomAttributes->SetCustAttrib(p, ReplacementAttrib);
                break;
            }
        }

        delete Search->second;
        Search->second = ReplacementAttrib;
    }
    else
    {
        for (int i = 0; i < Search->second->PBlock->Count(0); ++i)
        {
            INode* Node = nullptr;
            Interval ValidityInterval;
            Search->second->PBlock->GetValue(0, 0, Node, ValidityInterval, i);
            Entry.Nodes.push_back(Node);
        } 
    }

    return Entry;
}

void
HEMAX_3dsmaxHda::RemakeMultiParameter(HEMAX_Parameter Parameter)
{
    auto&& CustomAttributeMap = GetCustAttribMap();

    TSTR ParameterName = HEMAX_Utilities::ToTSTR(Parameter.GetName());
    auto Search = CustomAttributeMap->find(ParameterName.data());
    if (Search != CustomAttributeMap->end())
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
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HAPI_AssetLibraryId ThisAssetId = SM.GetStore().FindAsset(Hda.GetAssetPath());
    HAPI_AssetLibraryId OtherAssetId = SM.GetStore().FindAsset(Source.Hda.GetAssetPath()); 

    if (ThisAssetId != OtherAssetId)
    {
        std::string Msg = "Could not copy parameter values because the HDAs "
            "are different.";
        HEMAX_Logger::Instance().AddEntry(Msg, HEMAX_LOG_LEVEL_WARN);
        return;
    }

    std::vector<HEMAX_Parameter>& SourceParms =
        Source.Hda.MainNode.GetParameters();

    bool Finished = true;
    int ParmIndex = 0;

    do
    {
	Finished = true;
	for (int p = ParmIndex; p < SourceParms.size(); p++)
	{
	    HEMAX_Parameter* DestParm =
                Hda.MainNode.GetParameter(SourceParms[p].GetName());
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
