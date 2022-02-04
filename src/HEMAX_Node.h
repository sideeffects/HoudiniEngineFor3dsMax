#pragma once

#include "HEMAX_HAPISession.h"
#include "HEMAX_Parameter.h"
#include "HEMAX_Utilities.h"
#include "HEMAX_Types.h"

#include <string>
#include <unordered_map>

class HEMAX_Node
{
    public:
	HEMAX_Node();
	HEMAX_Node(HAPI_NodeId NodeId, HAPI_NodeType NodeType);

	void Init(const std::string& Asset);
	void Cook();
	void Delete();

	std::vector<HEMAX_Parameter>& GetParameters();
	HEMAX_Parameter* GetParameter(HAPI_ParmId ParmId);
	HEMAX_Parameter* GetParameter(std::string ParmName);
	std::vector<HEMAX_Parameter> GetAllMultiParameterInstances(int MultiId);

	void SetTransform(HEMAX_MaxTransform& Xform);
	void SetParentTransform(HEMAX_MaxTransform& Xform);

	void ConnectInputNode(HAPI_NodeId NodeToConnect, int InputIndex);
	void DisconnectInputNode(int InputIndex);
	HAPI_NodeId QueryNodeInput(int InputIndex);

	bool ShouldCookTwice();

	HAPI_NodeInfo Info;
	HAPI_NodeType Type;	

	std::string AssetName;
	std::string NodeName;

	std::vector<HEMAX_Parameter> Parameters;

	std::vector<std::string> InputLabels;

    private:

        void InitNodeOptions();

    public:

	bool AutoRecookOnParameterUpdate;
	bool RealtimeRecookEnabled;
	bool AutoRecookOnInputChange;
};
