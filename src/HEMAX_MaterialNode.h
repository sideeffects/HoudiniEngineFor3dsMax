#pragma once

#include "HEMAX_Types.h"

#include <string>
#include <stdmat.h>

#define HEMAX_MAT_DIFFUSE_COLOR_MAP_0 "ogl_tex1"
#define HEMAX_MAT_DIFFUSE_COLOR_MAP_1 "basecolor_texture"

#define HEMAX_MAT_OGL_ALPHA_0         "ogl_alpha"
#define HEMAX_MAT_OGL_ALPHA_1         "opac"

#define HEMAX_MAT_OGL_AMBIENT         "ogl_amb"

#define HEMAX_MAT_OGL_DIFFUSE_0       "ogl_diff"
#define HEMAX_MAT_OGL_DIFFUSE_1       "basecolor"

#define HEMAX_MAT_OGL_SPECULAR        "ogl_spec"

struct HEMAX_MaterialMapping
{
    StdMat2* Material;
    int SubNumber;
};

class HEMAX_MaterialNode
{
    public:

        HAPI_NodeId NodeId;
        HAPI_MaterialInfo MatInfo;

        std::string Name;

        bool HasTexture;
        HAPI_ParmId TextureParameterId;
        HAPI_ImageInfo ImageInfo;

        int ImageMemoryBufferLength;
        unsigned char* ImageBuffer;
        std::string ImagePath;

        int ImageXResolution;
        int ImageYResolution;

        float Alpha;
        bool HasAlpha;

        float AmbientColor[3];
        bool HasAmbient;

        float DiffuseColor[3];
        bool HasDiffuse;

        float SpecularColor[3];
        bool HasSpecular;

        bool HasTextureBeenRendered;

        HEMAX_MaterialNode(HAPI_NodeId Id);
        ~HEMAX_MaterialNode();

        void UpdateInfo();
        void GetMaterialInformation();
        bool RenderTextureToMemory();

        void Build3dsmaxMaterial();

        bool IsDirty();

        StdMat2* Get3dsmaxMaterial();

    private:

        void Update3dsmaxMaterial();

        StdMat2* MaxMaterial;
        bool Dirty;
};
