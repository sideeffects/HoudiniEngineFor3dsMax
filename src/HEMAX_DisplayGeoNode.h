#pragma once

#include "HEMAX_Types.h"
#include "HEMAX_Part.h"
#include "HEMAX_Mesh.h"
#include <string>
#include <vector>

struct HEMAX_DisplayGeoNode
{
    HEMAX_GeometryInfo DisplayGeoInfo;
    bool IsVisible;
    std::string NodeName;
    std::vector<HEMAX_Part> Parts;
};

// Returns true if a display node was parsed,
// returns false otherwise.
bool
GetDisplayGeoNodeData(HEMAX_NodeId NodeId, HEMAX_DisplayGeoNode& Node);

bool
LoadMesh(HEMAX_Mesh** Mesh, HEMAX_DisplayGeoNode& Node, int PartNum);

void
UpdateDisplayGeometryNode(HEMAX_NodeId NodeId, HEMAX_DisplayGeoNode& Node);