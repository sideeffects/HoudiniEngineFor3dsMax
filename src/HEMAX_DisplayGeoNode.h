#pragma once

#include "HEMAX_Types.h"
#include "HEMAX_Part.h"
#include "HEMAX_Mesh.h"
#include <string>
#include <vector>

class HEMAX_DisplayGeoNode
{
    public:
	HEMAX_DisplayGeoNode();

	void Init(HAPI_NodeId NodeId);
	void Update(bool Cook);

	HEMAX_Part* GetPart(HAPI_PartId Id);

	HAPI_NodeId Node;
	HAPI_GeoInfo Info;
	bool IsVisible;
	std::string Name;
	std::vector<HEMAX_Part> Parts;
};
