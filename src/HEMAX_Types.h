#pragma once

#include <HAPI.h>

#define HEMAX_FRAME_ZERO 0

#define HEMAX_HOUDINI_DEFAULT_GAMMA 2.2f

#define HEMAX_POSITION_ATTRIBUTE	HAPI_ATTRIB_POSITION
#define HEMAX_NORMAL_ATTRIBUTE		HAPI_ATTRIB_NORMAL
#define HEMAX_UV_ATTRIBUTE		HAPI_ATTRIB_UV
#define HEMAX_INSTANCE_ATTRIBUTE	HAPI_ATTRIB_INSTANCE
#define HEMAX_COLOR_ATTRIBUTE		HAPI_ATTRIB_COLOR
#define HEMAX_TRANSLATE_ATTR		"hemax_translate"
#define HEMAX_ROTATE_ATTR		"hemax_rotate"
#define HEMAX_SCALE_ATTR		"hemax_scale"
#define HEMAX_QUATERNION_ATTR		"hemax_quaternion"
#define HEMAX_MAX_RAW_TM_WORLD		"hemax_transform_world"
#define HEMAX_MAX_RAW_TM_LOCAL		"hemax_transform_local"
#define HEMAX_MAX_NODE_NAME             "hemax_node_name"
#define HEMAX_MAX_NODE_NAME_OUTPUT      "hemax_node_name_output"
#define HEMAX_SOFT_SELECTION_ATTRIBUTE	"soft_selection"
#define HEMAX_ALPHA_ATTRIBUTE		"Alpha"
#define HEMAX_ILLUMINATION_ATTRIBUTE	"illumination"
#define HEMAX_SMOOTHING_GROUP_ATTRIBUTE	"hemax_sg"
#define HEMAX_MATERIAL_ID_ATTRIBUTE     "hemax_matid"
#define HEMAX_MATERIAL_PATH_ATTRIBUTE   "hemax_material"
#define HEMAX_METADATA                  "meta_"

#define HEMAX_BAKE_LAYER_NAME_ATTRIBUTE         "hemax_bake_layer_name"
#define HEMAX_BAKE_PARENT_NODE_NAME_ATTRIBUTE   "hemax_bake_parent_node_name"

#define HEMAX_SELECTION_FACE                    "hemax_face_selection"
#define HEMAX_SELECTION_VERTEX                  "hemax_vertex_selection"
#define HEMAX_SELECTION_EDGE                    "hemax_edge_selection"

#define HEMAX_IMAGE_PLANE_ALPHA "A"
#define HEMAX_IMAGE_PLANE_DIFFUSE "C"

#define HEMAX_MODIFIER_STACK_PLUGIN_NAME "Houdini Digital Asset"

#define HEMAX_MAPPING_CHANNEL_COLOR         0
#define HEMAX_MAPPING_CHANNEL_ILLUMINATION -1
#define HEMAX_MAPPING_CHANNEL_ALPHA        -2
#define HEMAX_MAPPING_CHANNEL_UVW           1

typedef enum
{
    HEMAX_IN_PROCESS,
    HEMAX_THRIFT_SOCKET,
    HEMAX_THRIFT_PIPE,
} HEMAX_SessionType;

typedef enum
{
    COOK_FAILED = 0,
    COOK_NOT_FINISHED = 1,
    COOK_FINISHED = 2
} HEMAX_CookResult;

typedef enum
{
    HEMAX_NO_NORMAL,
    HEMAX_POINT_NORMAL,
    HEMAX_VERTEX_NORMAL
} HEMAX_NormalType;

typedef enum
{
    HEMAX_NO_UV,
    HEMAX_POINT_UV,
    HEMAX_VERTEX_UV
} HEMAX_UVType;

enum HEMAX_HdaType
{
    HEMAX_GEOMETRY_HDA,
    HEMAX_MODIFIER_HDA
};

typedef enum
{
    HEMAX_INPUT_PARAMETER,
    HEMAX_INPUT_SUBNETWORK,
    HEMAX_INPUT_EDITABLENODE
} HEMAX_InputType;

typedef enum
{
    HEMAX_ASSET_NO_STATUS,
    HEMAX_ASSET_NOT_FOUND,
    HEMAX_ASSET_ALREADY_LOADED,
    HEMAX_ASSET_INVALID
} HEMAX_AssetLoadStatus;

typedef enum
{
    UNKNOWN_LEVEL_HDA,
    OBJ_LEVEL_HDA,
    SOP_LEVEL_HDA
} HEMAX_HdaLevel;
