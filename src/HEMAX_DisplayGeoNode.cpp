#include "HEMAX_DisplayGeoNode.h"

#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"
#include <set>

bool
GetDisplayGeoNodeData(HEMAX_NodeId NodeId, HEMAX_DisplayGeoNode& Node)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    // We don't use the actual display node's name as the name, but rather the node
    // that contains it. Since the display node probably won't be named anything meaningful,
    // or will be something like OUT or END
    HEMAX_NodeInfo NodeInfo;
    SM.Session->GetNodeInfo(NodeId, &NodeInfo);
    Node.NodeName = SM.Session->GetHAPIString(NodeInfo.nameSH);

    if (SM.Session->GetDisplayGeoInfo(NodeId, &Node.DisplayGeoInfo))
    {
        for (int p = 0; p < Node.DisplayGeoInfo.partCount; p++)
        {
            HEMAX_Part NewPart;
            LoadPartInfo(NewPart, Node.DisplayGeoInfo.nodeId, p);
            Node.Parts.push_back(NewPart);
        }
        return true;
    }

    return false;
}

bool
LoadMesh(HEMAX_Mesh** Mesh, HEMAX_DisplayGeoNode& Node, int PartNum)
{
    if (PartNum < Node.Parts.size())
    {
        HEMAX_Part& Part = Node.Parts[PartNum];
        HEMAX_PartInfo& PartInfo = Part.Info;

        if (PartInfo.type == HEMAX_PARTTYPE_INVALID)
        {
            *Mesh = new HEMAX_Mesh(0, 0, 0, 0);
            return true;
        }
        else if (PartInfo.type == HEMAX_PARTTYPE_MESH)
        {
            HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

            HAPI_AttributeInfo PointAttributeInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_POSITION_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT, &PointAttributeInfo);

            *Mesh = new HEMAX_Mesh(PartInfo.faceCount, PartInfo.vertexCount, PointAttributeInfo.count, PointAttributeInfo.tupleSize);

            SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_POSITION_ATTRIBUTE, &PointAttributeInfo, -1, (*Mesh)->GetPointListArray(), 0,
                PointAttributeInfo.count);

            // NORMALS //

            HAPI_AttributeInfo PointNormalAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_NORMAL_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT, &PointNormalAttrInfo);

            HAPI_AttributeInfo VertexNormalAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_NORMAL_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX, &VertexNormalAttrInfo);

            if (PointNormalAttrInfo.exists)
            {
                (*Mesh)->SetNormalsExist(true);
                (*Mesh)->AllocatePointNormalArray();
                SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_NORMAL_ATTRIBUTE, &PointNormalAttrInfo, -1, (*Mesh)->GetPointNormalsListArray(), 0, PointNormalAttrInfo.count);
            }
            else if (VertexNormalAttrInfo.exists)
            {
                (*Mesh)->SetNormalsExist(true);
                (*Mesh)->AllocateVertexNormalArray();
                SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_NORMAL_ATTRIBUTE, &VertexNormalAttrInfo, -1, (*Mesh)->GetVertexNormalsListArray(), 0, VertexNormalAttrInfo.count);
            }
            else
            {
                (*Mesh)->SetNormalsExist(false);
            }

            /////////////

            // COLORS //

            HEMAX_AttributeInfo PointColorAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_COLOR_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT, &PointColorAttrInfo);

            HEMAX_AttributeInfo VertexColorAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_COLOR_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX, &VertexColorAttrInfo);

            if (PointColorAttrInfo.exists)
            {
                (*Mesh)->AllocatePointCdArray();
                SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_COLOR_ATTRIBUTE, &PointColorAttrInfo, -1, (*Mesh)->GetPointCdArray(), 0, PointColorAttrInfo.count);
            }
            else if (VertexColorAttrInfo.exists)
            {
                (*Mesh)->AllocateVertexCdArray();
                SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_COLOR_ATTRIBUTE, &VertexColorAttrInfo, -1, (*Mesh)->GetVertexCdArray(), 0, VertexColorAttrInfo.count);
            }

            ////////////

            // ALPHA //

            HEMAX_AttributeInfo PointAlphaAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_ALPHA_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT, &PointAlphaAttrInfo);

            HEMAX_AttributeInfo VertexAlphaAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_ALPHA_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX, &VertexAlphaAttrInfo);

            if (PointAlphaAttrInfo.exists)
            {
                (*Mesh)->AllocateAlphaArray(HEMAX_ATTRIBUTEOWNER_POINT);
                SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_ALPHA_ATTRIBUTE, &PointAlphaAttrInfo, -1, (*Mesh)->GetAlphaArray(), 0, PointAlphaAttrInfo.count);
            }
            else if (VertexAlphaAttrInfo.exists)
            {
                (*Mesh)->AllocateAlphaArray(HEMAX_ATTRIBUTEOWNER_VERTEX);
                SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_ALPHA_ATTRIBUTE, &VertexAlphaAttrInfo, -1, (*Mesh)->GetAlphaArray(), 0, VertexAlphaAttrInfo.count);
            }

            ///////////

            // ILLUMINATION //

            HEMAX_AttributeInfo PointIllumeAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_ILLUMINATION_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT, &PointIllumeAttrInfo);

            HEMAX_AttributeInfo VertexIllumeAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_ILLUMINATION_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX, &VertexIllumeAttrInfo);

            if (PointIllumeAttrInfo.exists)
            {
                (*Mesh)->AllocateIlluminationArray(HEMAX_ATTRIBUTEOWNER_POINT);
                SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_ILLUMINATION_ATTRIBUTE, &PointIllumeAttrInfo, -1, (*Mesh)->GetIlluminationArray(), 0, PointIllumeAttrInfo.count);
            }
            else if (VertexIllumeAttrInfo.exists)
            {
                (*Mesh)->AllocateIlluminationArray(HEMAX_ATTRIBUTEOWNER_VERTEX);
                SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_ILLUMINATION_ATTRIBUTE, &VertexIllumeAttrInfo, -1, (*Mesh)->GetIlluminationArray(), 0, VertexIllumeAttrInfo.count);
            }

            /////////////////

            // SMOOTHING GROUPS //

            HEMAX_AttributeInfo SGAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_SMOOTHING_GROUP_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_PRIM, &SGAttrInfo);

            if (SGAttrInfo.exists)
            {
                (*Mesh)->AllocateSmoothingGroupsArray();
                SM.Session->GetAttributeIntData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_SMOOTHING_GROUP_ATTRIBUTE, &SGAttrInfo, -1, (*Mesh)->GetSmoothingGroupArray(), 0, (*Mesh)->GetFaceCount());
            }

            //////////////////////

            // MATERIAL IDS //

            HEMAX_AttributeInfo MatIDAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_MATERIAL_ID_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_PRIM, &MatIDAttrInfo);

            if (MatIDAttrInfo.exists)
            {
                (*Mesh)->AllocateMaterialIDArray();
                SM.Session->GetAttributeIntData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_MATERIAL_ID_ATTRIBUTE, &MatIDAttrInfo, -1, (*Mesh)->GetMaterialIDArray(), 0, (*Mesh)->GetFaceCount());
            }

            //////////////////

            // SHOP MATERIAL PATH //

            HEMAX_AttributeInfo MatPathAttrInfo;
            SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_MATERIAL_PATH_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_DETAIL, &MatPathAttrInfo);

            if (MatPathAttrInfo.exists)
            {
                std::vector<HEMAX_StringHandle> StringHandles(1);
                SM.Session->GetAttributeStringData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_MATERIAL_PATH_ATTRIBUTE, &MatPathAttrInfo, StringHandles.data(), 0, 1);
                (*Mesh)->MaterialPath = SM.Session->GetHAPIString(StringHandles[0]);
            }
            else
            {
                (*Mesh)->MaterialPath = "";
            }

            ////////////////////////

            SM.Session->GetFaceCounts(Node.DisplayGeoInfo.nodeId, PartInfo.id, (*Mesh)->GetFaceVertexCountsArray(), 0, PartInfo.faceCount);
            SM.Session->GetVertexList(Node.DisplayGeoInfo.nodeId, PartInfo.id, (*Mesh)->GetVertexListArray(), 0, PartInfo.vertexCount);

            if ((*Mesh)->GetFaceCount() > 0)
            {
                (*Mesh)->AllocateMaterialIdsArray();
                SM.Session->GetMaterialNodeIdsOnFaces(Node.DisplayGeoInfo.nodeId, PartInfo.id, &(*Mesh)->AreMaterialIdsSame, (*Mesh)->GetMaterialIdsArray(), 0, PartInfo.faceCount);
                
                // Main UV Layer

                HAPI_AttributeInfo UVPointAttributeInfo;
                SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_UV_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_POINT, &UVPointAttributeInfo);

                HAPI_AttributeInfo UVVertexAttributeInfo;
                SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_UV_ATTRIBUTE, HEMAX_ATTRIBUTEOWNER_VERTEX, &UVVertexAttributeInfo);

                if (UVPointAttributeInfo.exists)
                {
                    (*Mesh)->AllocatePointUVArray(UVPointAttributeInfo.tupleSize);
                    SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_UV_ATTRIBUTE, &UVPointAttributeInfo, -1, (*Mesh)->GetPointUVArray(), 0, PartInfo.pointCount);
                }

                if (UVVertexAttributeInfo.exists)
                {
                    (*Mesh)->AllocateVertexUVArray(UVVertexAttributeInfo.tupleSize);
                    SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, HEMAX_UV_ATTRIBUTE, &UVVertexAttributeInfo, -1, (*Mesh)->GetVertexUVArray(), 0, PartInfo.vertexCount);
                }

                // Secondary UV Layers

                for (int UVLayer = 2; UVLayer < MAX_MESHMAPS; UVLayer++)
                {
                    std::string UVName = HEMAX_UV_ATTRIBUTE + std::to_string(UVLayer);

                    HEMAX_AttributeInfo UVPointInfo;
                    SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, UVName.c_str(), HEMAX_ATTRIBUTEOWNER_POINT, &UVPointInfo);
                    HEMAX_AttributeInfo UVVertexInfo;
                    SM.Session->GetAttributeInfo(Node.DisplayGeoInfo.nodeId, PartInfo.id, UVName.c_str(), HEMAX_ATTRIBUTEOWNER_VERTEX, &UVVertexInfo);

                    if (UVPointInfo.exists)
                    {
                        (*Mesh)->CreateSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_POINT, UVLayer, PartInfo.pointCount * 3);
                        SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, UVName.c_str(), &UVPointInfo, -1, (*Mesh)->GetSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_POINT, UVLayer).data(), 0, PartInfo.pointCount);
                    }

                    if (UVVertexInfo.exists)
                    {
                        (*Mesh)->CreateSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_VERTEX, UVLayer, PartInfo.vertexCount * 3);
                        SM.Session->GetAttributeFloatData(Node.DisplayGeoInfo.nodeId, PartInfo.id, UVName.c_str(), &UVVertexInfo, -1, (*Mesh)->GetSecondaryUVLayer(HEMAX_ATTRIBUTEOWNER_VERTEX, UVLayer).data(), 0, PartInfo.vertexCount);
                    }
                }
            }
            return true;
        }
        else if (PartInfo.type == HEMAX_PARTTYPE_INVALID)
        {
            HEMAX_Logger::Instance().AddEntry("HEMAX_Loader: found an invalid part type when instantiating geometry plugin.", HEMAX_LOG_LEVEL_WARN);
            return false;
        }
        else
        {
            HEMAX_Logger::Instance().AddEntry("HEMAX_Loader: inside MakeMesh function but was passed an unsupported part type", HEMAX_LOG_LEVEL_ERROR);

            return false;
        }
    }
    return false;
}

void
UpdateDisplayGeometryNode(HEMAX_NodeId NodeId, HEMAX_DisplayGeoNode& Node)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HEMAX_NodeInfo NodeInfo;
    SM.Session->GetNodeInfo(NodeId, &NodeInfo);

    if (SM.Session->GetDisplayGeoInfo(NodeId, &Node.DisplayGeoInfo))
    {
        if (Node.Parts.size() != Node.DisplayGeoInfo.partCount)
        {
            Node.Parts.resize(Node.DisplayGeoInfo.partCount);
        }

        for (int p = 0; p < Node.DisplayGeoInfo.partCount; p++)
        {
            if (p < Node.Parts.size())
            {
                HEMAX_Part FreshPart;
                Node.Parts[p] = FreshPart;
                LoadPartInfo(Node.Parts[p], Node.DisplayGeoInfo.nodeId, p);
            }
        }
    }
}