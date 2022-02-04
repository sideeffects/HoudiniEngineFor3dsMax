#include "HEMAX_MaterialNode.h"

#include "HEMAX_SessionManager.h"
#include "HEMAX_Logger.h"
#include <IPathConfigMgr.h>
#include <bitmap.h>

HEMAX_MaterialNode::HEMAX_MaterialNode(HAPI_NodeId Id)
    : NodeId(Id)
    , ImageBuffer(nullptr)
    , HasTextureBeenRendered(false)
    , HasTexture(false)
    , HasAlpha(false)
    , HasAmbient(false)
    , HasDiffuse(false)
    , HasSpecular(false)
    , MaxMaterial(nullptr)
    , Dirty(false)
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    HAPI_NodeInfo NodeInfo;
    SM.Session->GetNodeInfo(NodeId, &NodeInfo);
    Name = SM.Session->GetHAPIString(NodeInfo.nameSH);
}

HEMAX_MaterialNode::~HEMAX_MaterialNode()
{
    if (ImageBuffer)
    {
	delete[] ImageBuffer;
    }
}

void
HEMAX_MaterialNode::UpdateInfo()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();

    HAPI_NodeInfo myNodeInfo;
    SM.Session->GetNodeInfo(NodeId, &myNodeInfo);

    if (!myNodeInfo.isValid)
        return;

    SM.Session->GetMaterialInfo(NodeId, &MatInfo);

    if (MatInfo.hasChanged)
    {
        Dirty = true;
    }
}

void
HEMAX_MaterialNode::GetMaterialInformation()
{
    HEMAX_SessionManager& SM = HEMAX_SessionManager::GetSessionManager();
    
    HAPI_NodeInfo myNodeInfo;
    SM.Session->GetNodeInfo(NodeId, &myNodeInfo);

    if (!myNodeInfo.isValid)
        return;

    HAPI_ParmId Tex0ParmId;
    SM.Session->GetParameterIdFromName(NodeId,
                                       HEMAX_MAT_DIFFUSE_COLOR_MAP_0,
                                       &Tex0ParmId);

    HAPI_ParmId Tex1ParmId;
    SM.Session->GetParameterIdFromName(NodeId,
                                       HEMAX_MAT_DIFFUSE_COLOR_MAP_1,
                                       &Tex1ParmId);

    if (Tex0ParmId > -1)
    {
        HAPI_ParmInfo ParmInfo;
        SM.Session->GetParameterInfo(NodeId, Tex0ParmId, &ParmInfo);
        HEMAX_Parameter MatParameter(NodeId, ParmInfo);

        std::string TextureName = MatParameter.GetStringVals()[0];
        HasTexture = (TextureName != "");

        TextureParameterId = MatParameter.Info.id;

        ImagePath = (HasTexture ? Name : "");
    }
    else if (Tex1ParmId > -1)
    {
        HAPI_ParmInfo ParmInfo;
        SM.Session->GetParameterInfo(NodeId, Tex1ParmId, &ParmInfo);
        HEMAX_Parameter MatParameter(NodeId, ParmInfo);

        std::string TextureName = MatParameter.GetStringVals()[0];
        HasTexture = (TextureName != "");

        TextureParameterId = MatParameter.Info.id;

        ImagePath = (HasTexture ? Name : "");
    }

    HAPI_ParmId Alpha0ParmId;
    SM.Session->GetParameterIdFromName(NodeId,
                                       HEMAX_MAT_OGL_ALPHA_0,
                                       &Alpha0ParmId);

    HAPI_ParmId Alpha1ParmId;
    SM.Session->GetParameterIdFromName(NodeId,
                                       HEMAX_MAT_OGL_ALPHA_1,
                                       &Alpha1ParmId);

    if (Alpha0ParmId > -1)
    {
        HAPI_ParmInfo ParmInfo;
        SM.Session->GetParameterInfo(NodeId, Alpha0ParmId, &ParmInfo);
        HEMAX_Parameter MatParameter(NodeId, ParmInfo);
        Alpha = MatParameter.GetFloatVals()[0];
        HasAlpha = true;
    }
    else if (Alpha1ParmId > -1)
    {
        HAPI_ParmInfo ParmInfo;
        SM.Session->GetParameterInfo(NodeId, Alpha1ParmId, &ParmInfo);
        HEMAX_Parameter MatParameter(NodeId, ParmInfo);
        Alpha = MatParameter.GetFloatVals()[0];
        HasAlpha = true;
    }

    HAPI_ParmId AmbientParmId;
    SM.Session->GetParameterIdFromName(NodeId,
                                       HEMAX_MAT_OGL_AMBIENT,
                                       &AmbientParmId);

    if (AmbientParmId > -1)
    {
        HAPI_ParmInfo ParmInfo;
        SM.Session->GetParameterInfo(NodeId, AmbientParmId, &ParmInfo);
        HEMAX_Parameter MatParameter(NodeId, ParmInfo);
        std::vector<float> AmbientVals = MatParameter.GetFloatVals();

        AmbientColor[0] = AmbientVals[0];
        AmbientColor[1] = AmbientVals[1];
        AmbientColor[2] = AmbientVals[2];

        HasAmbient = true;
    }

    HAPI_ParmId Diffuse0ParmId;
    SM.Session->GetParameterIdFromName(NodeId,
                                       HEMAX_MAT_OGL_DIFFUSE_0,
                                       &Diffuse0ParmId);

    HAPI_ParmId Diffuse1ParmId;
    SM.Session->GetParameterIdFromName(NodeId,
                                       HEMAX_MAT_OGL_DIFFUSE_1,
                                       &Diffuse1ParmId);

    if (Diffuse0ParmId > -1)
    {
        HAPI_ParmInfo ParmInfo;
        SM.Session->GetParameterInfo(NodeId, Diffuse0ParmId, &ParmInfo);
        HEMAX_Parameter MatParameter(NodeId, ParmInfo);
        std::vector<float> DiffuseValues = MatParameter.GetFloatVals();

        DiffuseColor[0] = DiffuseValues[0];
        DiffuseColor[1] = DiffuseValues[1];
        DiffuseColor[2] = DiffuseValues[2];

        HasDiffuse = true;
    }
    else if (Diffuse1ParmId > -1)
    {
        HAPI_ParmInfo ParmInfo;
        SM.Session->GetParameterInfo(NodeId, Diffuse1ParmId, &ParmInfo);
        HEMAX_Parameter MatParameter(NodeId, ParmInfo);

        std::vector<float> DiffuseValues = MatParameter.GetFloatVals();

        DiffuseColor[0] = DiffuseValues[0];
        DiffuseColor[1] = DiffuseValues[1];
        DiffuseColor[2] = DiffuseValues[2];

        HasDiffuse = true;
    }

    HAPI_ParmId SpecularParmId;
    SM.Session->GetParameterIdFromName(NodeId,
                                       HEMAX_MAT_OGL_SPECULAR,
                                       &SpecularParmId);

    if (SpecularParmId > -1)
    {
        HAPI_ParmInfo ParmInfo;
        SM.Session->GetParameterInfo(NodeId, SpecularParmId, &ParmInfo);
        HEMAX_Parameter MatParameter(NodeId, ParmInfo);
        std::vector<float> SpecularVals = MatParameter.GetFloatVals();

        SpecularColor[0] = SpecularVals[0];
        SpecularColor[1] = SpecularVals[1];
        SpecularColor[2] = SpecularVals[2];

        HasSpecular = true;
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

	std::string ImagePlanes = std::string(HEMAX_IMAGE_PLANE_DIFFUSE) +
                                  std::string(" ") +
                                  std::string(HEMAX_IMAGE_PLANE_ALPHA);

	if (SM.Session->ExtractImageToMemory(NodeId,
                                             HAPI_RAW_FORMAT_NAME,
                                             ImagePlanes.c_str(),
                                             &ImageMemoryBufferLength))
	{
	    char* Pixels = new char[ImageMemoryBufferLength];

	    if (SM.Session->GetImageMemoryBuffer(NodeId,
                                                 Pixels,
                                                 ImageMemoryBufferLength))
	    {
		ImageBuffer = (unsigned char*)Pixels;
		HasTextureBeenRendered = true;
		return true;
	    }
	    else
	    {
		HEMAX_Logger::Instance().AddEntry(
                        std::string("HEMAX_Material: could not get image "
                                    "memory buffer for HEMAX_Material with "
                                    "Node ID: " + std::to_string(NodeId)),
			HEMAX_LOG_LEVEL_ERROR);
	    }
	}
	else
	{
	    HEMAX_Logger::Instance().AddEntry(
                    std::string("HEMAX_Material: could not extract texture "
                                "image to memory for HEMAX_Material with Node "
                                "ID: " + std::to_string(NodeId)),
		    HEMAX_LOG_LEVEL_ERROR);
	}
    }
    else
    {
	HEMAX_Logger::Instance().AddEntry(
                std::string("HEMAX_Material: could not RenderTextureToImage "
                            "for HEMAX_Material with Node ID: " +
                            std::to_string(NodeId)),
		HEMAX_LOG_LEVEL_ERROR);
    }

    return false;
}

void
HEMAX_MaterialNode::Build3dsmaxMaterial()
{
    if (!MaxMaterial)
    {
        MaxMaterial = NewDefaultStdMat(); 
    }

    Update3dsmaxMaterial();
    Dirty = false;
}

void
HEMAX_MaterialNode::Update3dsmaxMaterial()
{
    if (!MaxMaterial)
        return;

    if (HasTexture)
    {
        if (!RenderTextureToMemory())
        {
            HEMAX_Logger::Instance().AddEntry("Failed to render texture to "
                                              "memory.",
                                              HEMAX_LOG_LEVEL_ERROR);
        }
    }

    BitmapTex* MaterialBitMapTex = nullptr;

    TimeValue CurrentTime = GetCOREInterface()->GetTime();

    if (HasAlpha)
        MaxMaterial->SetOpacity(Alpha, CurrentTime);
    
    if (HasAmbient)
        MaxMaterial->SetAmbient(Color(AmbientColor[0],
                                      AmbientColor[1],
                                      AmbientColor[2]),
                                CurrentTime);

    if (HasDiffuse)
        MaxMaterial->SetDiffuse(Color(DiffuseColor[0],
                                      DiffuseColor[1],
                                      DiffuseColor[2]),
                                CurrentTime);

    if (HasSpecular)
        MaxMaterial->SetSpecular(Color(SpecularColor[0],
                                       SpecularColor[1],
                                       SpecularColor[2]),
                                CurrentTime);

    if (HasTexture && HasTextureBeenRendered)
    {
	MaterialBitMapTex = NewDefaultBitmapTex();
	std::wstring FinalPath = IPathConfigMgr::GetPathConfigMgr()->GetDir(
                                                                APP_IMAGE_DIR);
	FinalPath.append(L"\\");

	std::wstring ImageFileName = std::wstring(ImagePath.begin(),
                                                  ImagePath.end());
	std::wstring Suffix = ImageFileName +
                              std::wstring(L"_") +
                              std::to_wstring(NodeId) +
                              std::wstring(L".bmp");
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
	    HEMAX_Logger::Instance().AddEntry(
                    "HEMAX_GeomObjectPlugin: no image data "
                    "when loading texture",
                    HEMAX_LOG_LEVEL_WARN);
	}

	MaterialBitMapTex->SetMap(BMInfo.GetAsset());
	MaxMaterial->SetSubTexmap(ID_DI, MaterialBitMapTex);
        MaterialBitMapTex->ReloadBitmapAndUpdate();
	MaterialBitMap->DeleteThis();
    }
}

bool
HEMAX_MaterialNode::IsDirty()
{
    return Dirty;
}

StdMat2*
HEMAX_MaterialNode::Get3dsmaxMaterial()
{
    return MaxMaterial;
}
