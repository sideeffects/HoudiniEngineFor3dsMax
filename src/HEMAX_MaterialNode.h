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

struct HEMAX_MaterialNode
{
    HEMAX_NodeId NodeId;
    bool Exists;
    bool HasChanged;

    bool HasTexture;
    HEMAX_ParameterId TextureParameterId;
    HEMAX_ImageInfo ImageInfo;

    int ImageMemoryBufferLength;
    unsigned char* ImageBuffer;
    std::string ImagePath;

    int ImageXResolution;
    int ImageYResolution;

    float Alpha;
    float AmbientColor[3];
    float DiffuseColor[3];
    float SpecularColor[3];

    bool HasTextureBeenRendered;

    HEMAX_MaterialNode(HEMAX_NodeId Id);
    ~HEMAX_MaterialNode();

    bool CheckHasChanged();
    void GetMaterialInformation();
    bool RenderTextureToMemory();

    StdMat2* Build3dsmaxMaterialFromNode();
};