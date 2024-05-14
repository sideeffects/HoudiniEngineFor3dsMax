# Houdini Engine for 3ds Max
Houdini Engine for 3ds Max is a 3ds Max plugin that allows deep integration of
Houdini technology into Autodesk 3ds Max through the use of Houdini Engine.

This plugin brings Houdini's powerful and flexible procedural workflow into 3ds
Max through Houdini Digital Assets. Artists can interactively adjust the asset's
parameters inside 3ds Max, and use 3ds Max geometries as an asset's inputs.
Houdini's procedural engine will then "cook" the asset and the results will be
available right inside 3ds Max.

For more information:
* [Documentation](https://www.sidefx.com/docs/3dsmax/)
* [FAQ](https://www.sidefx.com/faq/houdini-engine-faq/)

For support and reporting bugs:

* [Houdini Engine for 3ds Max forum](https://www.sidefx.com/forum/74/)
* [Bug Submission](https://www.sidefx.com/bugs/submit/)

## Supported 3ds Max versions
Currently, the supported 3ds Max versions are:

* 2022
* 2021
* 2020
* 2019
* 2018
* 2017

## Compiling with CMake

The plugin uses CMake to generate a Visual Studio project that can be used to compile the plugin.

To get started:

* Fork this repository to your own Github account using the Fork button at the top.
* Clone the forked repository onto your system.

In order to compile and run the plugin, you will need the following installed on your machine:

* Houdini
* 3ds Max (any supported version)
* 3ds Max SDK (that corresponds with your installed 3ds Max version)
* CMake
* Visual Studio (see the [3ds Max SDK Requirements](https://help.autodesk.com/view/MAXDEV/2022/ENU/?guid=Max_Developer_Help_about_the_3ds_max_sdk_sdk_requirements_html) to determine the correct version)
* Qt (see the [3ds Max SDK Requirements](https://help.autodesk.com/view/MAXDEV/2022/ENU/?guid=Max_Developer_Help_about_the_3ds_max_sdk_sdk_requirements_html) to determine the correct version)

The main CMake variables for configuration are:
* `HOUDINI_ENGINE_ROOT` - Specifies the path to the Houdini version being used to compile the plugin
* `HEMAX_VERSION` - The 3ds Max version that the plugin is being compiled for
* `3DSMAX_WINDOWS_SDK_VERSION` - Specifies the Windows SDK version that is required by the 3ds Max SDK
* `3DSMAX_WINDOWS_PLATFORM_TOOLSET` - Specifies the Windows Platform Toolset that is required by the 3ds Max SDK
* `3DSMAX_QT_PLATFORM_PATH` - Specifies the path to the Qt folder containing the required Qt binaries
* `3DSMAX_SDK_PATH` - Specifies the path to the 3ds Max SDK folder

Here is an example of how to run CMake to generate the plugin's Visual Studio project:

```
cd ~/HoudiniEngineFor3dsMax &&
mkdir build-3dsmax &&
cd build-3dsmax &&
cmake \
    -DHOUDINI_ENGINE_ROOT="C:/Program Files/Side Effects Software/Houdini 19.0.531" \
    -D3DSMAX_QT_PLATFORM_PATH="C:/Qt/5.12.5/msvc2017_64" \
    -D3DSMAX_SDK_PATH="C:/Program Files/Autodesk/3ds Max 2021 SDK" \
    -DHEMAX_VERSION=2021 \
    -D3DSMAX_WINDOWS_ADK_VERSION="10.0.17134.0" \
    -D3DSMAX_WINDOWS_PLATFORM_TOOLSET="v141" \
    -DCMAKE_CONFIGURATION_TYPES=Release ..
```
