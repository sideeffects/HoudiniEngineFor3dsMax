set( CMAKE_SYSTEM_VERSION ${3DSMAX_WINDOWS_SDK_VERSION} )
set( CMAKE_GENERATOR_TOOLSET ${3DSMAX_WINDOWS_PLATFORM_TOOLSET} )

project( HoudiniEngineFor3DSMax )

set( SOURCES
    src/DllEntry.cpp
    src/HEMAX_3dsMaxInput.cpp
    src/HEMAX_3dsmaxHda.cpp
    src/HEMAX_Asset.cpp
    src/HEMAX_CurvePlugin.cpp
    src/HEMAX_CustomAttributeContainer.cpp
    src/HEMAX_DisplayGeoNode.cpp
    src/HEMAX_EditableNode.cpp
    src/HEMAX_Events.cpp
    src/HEMAX_GeometryHda.cpp
    src/HEMAX_GeometryMouseCallback.cpp
    src/HEMAX_GeometryPlugin.cpp
    src/HEMAX_HAPISession.cpp
    src/HEMAX_Hda.cpp
    src/HEMAX_HoudiniApi.cpp
    src/HEMAX_Input.cpp
    src/HEMAX_InputInstance.cpp
    src/HEMAX_InputMerge.cpp
    src/HEMAX_Input_Geometry.cpp
    src/HEMAX_Input_NURBS.cpp
    src/HEMAX_Input_Spline.cpp
    src/HEMAX_Input_Transform.cpp
    src/HEMAX_Logger.cpp
    src/HEMAX_MaterialNode.cpp
    src/HEMAX_MaxScriptInterface.cpp
    src/HEMAX_Mesh.cpp
    src/HEMAX_Modifier.cpp
    src/HEMAX_ModifierAsset.cpp
    src/HEMAX_ModifierHda.cpp
    src/HEMAX_Node.cpp
    src/HEMAX_Object.cpp
    src/HEMAX_Parameter.cpp
    src/HEMAX_Part.cpp
    src/HEMAX_Path.cpp
    src/HEMAX_Platform.cpp
    src/HEMAX_Plugin.cpp
    src/HEMAX_ReferenceMaker.cpp
    src/HEMAX_SessionManager.cpp
    src/HEMAX_Shelf.cpp
    src/HEMAX_ShelfDirectory.cpp
    src/HEMAX_ShelfTool.cpp
    src/HEMAX_Store.cpp
    src/HEMAX_Time.cpp
    src/HEMAX_UserPrefs.cpp
    src/HEMAX_Utilities.cpp
    src/HoudiniEngineFor3DSMax.aps
    src/HoudiniEngineFor3DSMax.cpp
    src/HoudiniEngineFor3DSMax.def
    src/HoudiniEngineFor3DSMax.rc
)

set( HEADERS
    src/HEMAX_3dsMaxInput.h
    src/HEMAX_3dsmaxHda.h
    src/HEMAX_Asset.h
    src/HEMAX_CurvePlugin.h
    src/HEMAX_CustomAttributeContainer.h
    src/HEMAX_DisplayGeoNode.h
    src/HEMAX_EditableNode.h
    src/HEMAX_Events.h
    src/HEMAX_GeometryHda.h
    src/HEMAX_GeometryMouseCallback.h
    src/HEMAX_GeometryPlugin.h
    src/HEMAX_HAPISession.h
    src/HEMAX_Hda.h
    src/HEMAX_HoudiniApi.h
    src/HEMAX_Input.h
    src/HEMAX_InputInstance.h
    src/HEMAX_InputMerge.h
    src/HEMAX_Input_Geometry.h
    src/HEMAX_Input_NURBS.h
    src/HEMAX_Input_Spline.h
    src/HEMAX_Input_Transform.h
    src/HEMAX_Logger.h
    src/HEMAX_MaterialNode.h
    src/HEMAX_MaxScriptInterface.h
    src/HEMAX_Mesh.h
    src/HEMAX_Modifier.h
    src/HEMAX_ModifierAsset.h
    src/HEMAX_ModifierHda.h
    src/HEMAX_Node.h
    src/HEMAX_Object.h
    src/HEMAX_Parameter.h
    src/HEMAX_Part.h
    src/HEMAX_Path.h
    src/HEMAX_Platform.h
    src/HEMAX_Plugin.h
    src/HEMAX_ReferenceMaker.h
    src/HEMAX_SessionManager.h
    src/HEMAX_Shelf.h
    src/HEMAX_ShelfDirectory.h
    src/HEMAX_ShelfTool.h
    src/HEMAX_Store.h
    src/HEMAX_Time.h
    src/HEMAX_Types.h
    src/HEMAX_Undo.h
    src/HEMAX_UserPrefs.h
    src/HEMAX_Utilities.h
    src/HoudiniEngineFor3DSMax.h
)

list( APPEND SOURCES
    src/UI/HEMAX_AssetSelection.cpp
    src/UI/HEMAX_HDASelectionDialog.cpp
    src/UI/HEMAX_HDAWidget.cpp
    src/UI/HEMAX_MaxHoudiniAssetWidget.cpp
    src/UI/HEMAX_OptionsDialog.cpp
    src/UI/HEMAX_ParameterWidget.cpp
    src/UI/HEMAX_SessionWidget.cpp
    src/UI/HEMAX_ShelfGroup.cpp
    src/UI/HEMAX_ShelfTab.cpp
    src/UI/HEMAX_UI.cpp
    src/UI/HEMAX_VersionDialog.cpp
)

list( APPEND HEADERS
    src/UI/HEMAX_AssetSelection.h
    src/UI/HEMAX_HDASelectionDialog.h
    src/UI/HEMAX_HDAWidget.h
    src/UI/HEMAX_MaxHoudiniAssetWidget.h
    src/UI/HEMAX_OptionsDialog.h
    src/UI/HEMAX_ParameterWidget.h
    src/UI/HEMAX_SessionWidget.h
    src/UI/HEMAX_ShelfGroup.h
    src/UI/HEMAX_ShelfTab.h
    src/UI/HEMAX_UI.h
    src/UI/HEMAX_VersionDialog.h
)

set( MAX_LIBRARIES
    bmm.lib
    menus.lib
    core.lib
    geom.lib
    gfx.lib
    mesh.lib
    maxutil.lib
    Maxscrpt.lib
    gup.lib
    paramblk2.lib
    Qt5Core.lib
    Qt5Widgets.lib
    Qt5Gui.lib
    MaxQtBridge.lib
    edmodel.lib
    MNMath.lib
    Poly.lib
)

set( MOC_FILES
    HEMAX_UI
    HEMAX_SessionWidget
    HEMAX_HDAWidget
    HEMAX_ParameterWidget
    HEMAX_AssetSelection
    HEMAX_MaxHoudiniAssetWidget
    HEMAX_HDASelectionDialog
    HEMAX_ShelfTab
    HEMAX_ShelfGroup
    HEMAX_OptionsDialog
)

set( MOC_EXE_PATH "${3DSMAX_QT_PLATFORM_PATH}/bin/moc.exe" )
set( MOC_OUTPUT_PATH "${CMAKE_BINARY_DIR}/moc_includes" )

file( MAKE_DIRECTORY ${MOC_OUTPUT_PATH} )

link_directories( BEFORE "${3DSMAX_QT_PLATFORM_PATH}/lib" )
link_directories( BEFORE "${3DSMAX_SDK_PATH}/maxsdk/lib/x64/Release" )

add_library( MAX_PLUGIN SHARED ${SOURCES} ${HEADERS} )

set_target_properties(
    MAX_PLUGIN
    PROPERTIES
    OUTPUT_NAME HoudiniEngineFor3DSMax
    SUFFIX ".dlu"
    CXX_STANDARD 11
)

set( MAX_COMPILE_OPTIONS
    -W3
    -EHsc
    -w14101
    -w14996
    -wd4535
)

target_compile_options( MAX_PLUGIN PRIVATE ${MAX_COMPILE_OPTIONS} )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
include_directories( "${HOUDINI_ENGINE_ROOT}/toolkit/include/HAPI" )
include_directories( "${3DSMAX_QT_PLATFORM_PATH}/include" )
include_directories( "${3DSMAX_SDK_PATH}/maxsdk/include" )
include_directories( "${MOC_OUTPUT_PATH}" )

target_link_libraries( MAX_PLUGIN ${MAX_LIBRARIES} )

set( HEMAX_VERSION_NAME "HEMAX_VERSION_${HEMAX_VERSION}" )

target_compile_definitions( MAX_PLUGIN PRIVATE
    -DUNICODE -D_UNICODE
    ${HEMAX_VERSION_NAME}=1
    USE_HAPI_VERSION_HEADER=1
)

list( APPEND MOC_DEPENDENCIES "" )

foreach( MOC_FILE ${MOC_FILES} )
    add_custom_command(
        OUTPUT ${MOC_OUTPUT_PATH}/moc_${MOC_FILE}.cpp
        COMMAND "${MOC_EXE_PATH}" -o "${MOC_OUTPUT_PATH}/moc_${MOC_FILE}.cpp" src/UI/${MOC_FILE}.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS src/UI/${MOC_FILE}.cpp src/UI/${MOC_FILE}.h
    )
    add_custom_target( MOC_${MOC_FILE}
        DEPENDS ${MOC_OUTPUT_PATH}/moc_${MOC_FILE}.cpp
    )
    add_dependencies( MAX_PLUGIN MOC_${MOC_FILE} )
endforeach( MOC_FILE )
