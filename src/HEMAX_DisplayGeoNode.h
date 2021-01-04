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

	void Init(HEMAX_NodeId NodeId);
	void Update(bool Cook);

	HEMAX_Part* GetPart(HEMAX_PartId Id);

	HEMAX_NodeId Node;
	HEMAX_GeometryInfo Info;
	bool IsVisible;
	std::string Name;
	std::vector<HEMAX_Part> Parts;
};
