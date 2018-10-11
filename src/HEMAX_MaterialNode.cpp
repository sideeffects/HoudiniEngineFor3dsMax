#include "HEMAX_MaterialNode.h"

#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"
#include <IPathConfigMgr.h>
#include <bitmap.h>

HEMAX_MaterialNode::HEMAX_MaterialNode(HEMAX_NodeId Id)
    : NodeId(Id), ImageBuffer(nullptr), HasTextureBeenRendered(false)
{
}

HEMAX_MaterialNode::~HEMAX_MaterialNode()
{
    if (ImageBuffer)
    {
        delete[] ImageBuffer;
    }
}

bool
HEMAX_MaterialNode::CheckHasChanged()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HEMAX_MaterialInfo MatInfo;
    if (SM.Session->GetMaterialInfo(NodeId, &MatInfo))
    {
        HasChanged = MatInfo.hasChanged;
    }

    return HasChanged;
}

void
HEMAX_MaterialNode::GetMaterialInformation()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HEMAX_MaterialInfo MatInfo;
    if (SM.Session->GetMaterialInfo(NodeId, &MatInfo))
    {
        if (MatInfo.exists)
        {
            Exists = true;
            HasChanged = MatInfo.hasChanged;

            HEMAX_NodeInfo NodeInfo;

            if (SM.Session->GetNodeInfo(NodeId, &NodeInfo))
            {
                HEMAX_Node Node(NodeInfo.id, HEMAX_NODE_SHOP);
                Cook(Node);

                HEMAX_Parameter MatParameter = GetParameter(Node, HEMAX_MAT_DIFFUSE_COLOR_MAP_0);

                if (MatParameter.Type != HEMAX_PARAM_INVALID)
                {
                    std::string TextureName = GetParameterStringValues(MatParameter, true)[0];
                    HasTexture = (TextureName != "");

                    TextureParameterId = MatParameter.Info.id;

                    ImagePath = (HasTexture ? SM.Session->GetHAPIString(NodeInfo.nameSH) : "");
                }
                else
                {
                    MatParameter = GetParameter(Node, HEMAX_MAT_DIFFUSE_COLOR_MAP_1);

                    if (MatParameter.Type != HEMAX_PARAM_INVALID)
                    {
                        std::string TextureName = GetParameterStringValues(MatParameter, true)[0];
                        HasTexture = (TextureName != "");

                        TextureParameterId = MatParameter.Info.id;

                        ImagePath = (HasTexture ? SM.Session->GetHAPIString(NodeInfo.nameSH) : "");
                    }
                }

                MatParameter = GetParameter(Node, HEMAX_MAT_OGL_ALPHA_0);

                if (MatParameter.Type != HEMAX_PARAM_INVALID)
                {
                    Alpha = GetParameterFloatValues(MatParameter)[0];
                }
                else
                {
                    MatParameter = GetParameter(Node, HEMAX_MAT_OGL_ALPHA_1);

                    if (MatParameter.Type != HEMAX_PARAM_INVALID)
                    {
                        Alpha = GetParameterFloatValues(MatParameter)[0];
                    }
                }

                MatParameter = GetParameter(Node, HEMAX_MAT_OGL_AMBIENT);

                if (MatParameter.Type != HEMAX_PARAM_INVALID)
                {
                    std::vector<float> AmbientVals = GetParameterFloatValues(MatParameter);

                    AmbientColor[0] = AmbientVals[0];
                    AmbientColor[1] = AmbientVals[1];
                    AmbientColor[2] = AmbientVals[2];
                }

                MatParameter = GetParameter(Node, HEMAX_MAT_OGL_DIFFUSE_0);

                if (MatParameter.Type != HEMAX_PARAM_INVALID)
                {
                    std::vector<float> DiffuseValues = GetParameterFloatValues(MatParameter);

                    DiffuseColor[0] = DiffuseValues[0];
                    DiffuseColor[1] = DiffuseValues[1];
                    DiffuseColor[2] = DiffuseValues[2];
                }
                else
                {
                    MatParameter = GetParameter(Node, HEMAX_MAT_OGL_DIFFUSE_1);

                    if (MatParameter.Type != HEMAX_PARAM_INVALID)
                    {
                        std::vector<float> DiffuseValues = GetParameterFloatValues(MatParameter);

                        DiffuseColor[0] = DiffuseValues[0];
                        DiffuseColor[1] = DiffuseValues[1];
                        DiffuseColor[2] = DiffuseValues[2];
                    }
                }

                MatParameter = GetParameter(Node, HEMAX_MAT_OGL_SPECULAR);

                if (MatParameter.Type != HEMAX_PARAM_INVALID)
                {
                    std::vector<float> SpecularVals = GetParameterFloatValues(MatParameter);

                    SpecularColor[0] = SpecularVals[0];
                    SpecularColor[1] = SpecularVals[1];
                    SpecularColor[2] = SpecularVals[2];
                }
            }
        }
        else
        {
            Exists = false;
        }
    }
    else
    {
        Exists = false;
    }
}

bool
HEMAX_MaterialNode::RenderTextureToMemory()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    if (SM.Session->RenderTextureToImage(NodeId, TextureParameterId))
    {
        SM.Session->GetImageInfo(NodeId, &ImageInfo);

        ImageInfo.dataFormat = HAPI_IMAGE_DATA_INT16;
        ImageInfo.packing = HAPI_IMAGE_PACKING_RGBA;
        ImageInfo.interleaved = true;
        SM.Session->SetImageInfo(NodeId, &ImageInfo);

        ImageXResolution = ImageInfo.xRes;
        ImageYResolution = ImageInfo.yRes;

        std::string ImagePlanes = std::string(HEMAX_IMAGE_PLANE_DIFFUSE) + std::string(" ") + std::string(HEMAX_IMAGE_PLANE_ALPHA);

        if (SM.Session->ExtractImageToMemory(NodeId, HAPI_RAW_FORMAT_NAME, ImagePlanes.c_str(), &ImageMemoryBufferLength))
        {
            char* Pixels = new char[ImageMemoryBufferLength];

            if (SM.Session->GetImageMemoryBuffer(NodeId, Pixels, ImageMemoryBufferLength))
            {
                ImageBuffer = (unsigned char*)Pixels;
                HasTextureBeenRendered = true;
                return true;
            }
            else
            {
                HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_Material: could not get image memory buffer for HEMAX_Material with Node ID: " + std::to_string(NodeId)),
                    HEMAX_LOG_LEVEL_ERROR);
            }
        }
        else
        {
            HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_Material: could not extract texture image to memory for HEMAX_Material with Node ID: " + std::to_string(NodeId)),
                HEMAX_LOG_LEVEL_ERROR);
        }
    }
    else
    {
        HEMAX_Logger::Instance().AddEntry(std::string("HEMAX_Material: could not RenderTextureToImage for HEMAX_Material with Node ID: " + std::to_string(NodeId)),
            HEMAX_LOG_LEVEL_ERROR);
    }

    return false;
}

StdMat2*
HEMAX_MaterialNode::Build3dsmaxMaterialFromNode()
{
    StdMat2* MaxMat = NewDefaultStdMat();
    BitmapTex* MaterialBitMapTex = nullptr;

    TimeValue CurrentTime = GetCOREInterface()->GetTime();

    MaxMat->SetOpacity(Alpha, CurrentTime);
    MaxMat->SetAmbient(Color(AmbientColor[0], AmbientColor[1], AmbientColor[2]), CurrentTime);
    MaxMat->SetDiffuse(Color(DiffuseColor[0], DiffuseColor[1], DiffuseColor[2]), CurrentTime);
    MaxMat->SetSpecular(Color(SpecularColor[0], SpecularColor[1], SpecularColor[2]), CurrentTime);

    if (HasTexture && HasTextureBeenRendered)
    {
        MaterialBitMapTex = NewDefaultBitmapTex();
        std::wstring FinalPath = IPathConfigMgr::GetPathConfigMgr()->GetDir(APP_IMAGE_DIR);
        FinalPath.append(L"\\");

        std::wstring ImageFileName = std::wstring(ImagePath.begin(), ImagePath.end());
        std::wstring Suffix = ImageFileName + std::wstring(L"_") + std::to_wstring(NodeId) + std::wstring(L".bmp");
        FinalPath.append(Suffix);

        BitmapInfo BMInfo;
        BMInfo.SetType(BMM_TRUE_64);
        BMInfo.SetFlags(MAP_HAS_ALPHA);
        BMInfo.SetWidth(ImageXResolution);
        BMInfo.SetHeight(ImageYResolution);
        BMInfo.SetPath(FinalPath.c_str());
        BMInfo.SetCustomFlag(0);
        BMInfo.SetGamma(HEMAX_HOUDINI_DEFAULT_GAMMA);

        Bitmap* MaterialBitMap = TheManager->Create(&BMInfo);

        unsigned char* ImgChar = ImageBuffer;
        unsigned short* Img = (unsigned short*)ImgChar;

        if (Img)
        {
            BMM_Color_64* TheColor = new BMM_Color_64(0, 0, 0, 0);

            int ImgIndex = 0;
            for (int y = ImageYResolution - 1; y >= 0; --y)
            {
                for (int x = 0; x < ImageXResolution; ++x)
                {
                    TheColor->r = Img[ImgIndex];
                    TheColor->g = Img[ImgIndex + 1];
                    TheColor->b = Img[ImgIndex + 2];
                    TheColor->a = Img[ImgIndex + 3];

                    MaterialBitMap->PutPixels(x, y, 1, TheColor);
                    ImgIndex += 4;
                }
            }

            delete TheColor;

            MaterialBitMap->OpenOutput(&BMInfo);
            MaterialBitMap->Write(&BMInfo, BMM_SINGLEFRAME);
            MaterialBitMap->Close(&BMInfo);
        }
        else
        {
            HEMAX_Logger::Instance().AddEntry("HEMAX_GeomObjectPlugin: no image data when loading texture", HEMAX_LOG_LEVEL_WARN);
        }

        MaterialBitMapTex->SetMap(BMInfo.GetAsset());
        MaxMat->SetSubTexmap(ID_DI, MaterialBitMapTex);
        MaterialBitMap->DeleteThis();
    }

    return MaxMat;
}