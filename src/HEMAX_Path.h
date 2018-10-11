#pragma once

#include <string>

namespace HEMAX_Path
{
    const std::string HEMAX_PathPrefix_HFS = "<HFS>";
    extern std::string HEMAX_PathPrefix_HFS_Resolved;

    const std::string HEMAX_PathPrefix_HoudiniToolPath = "<HOUDINI_TOOL_PATH>";

    bool IsPathRelative(const std::string& Path);
    
    std::string GetDirectory(const std::string& Path);

    std::string GetAbsoluteFilePath(const std::string& WorkingDir, const std::string& RelPath);

    std::string ResolvePathPrefixes(const std::string& Path);

    bool DoesFileEixst(std::string Path);
}