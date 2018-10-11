#include "HEMAX_Path.h"

#include <QtCore/qfileinfo.h>
#include <QtCore/qdir.h>

#include "HEMAX_Logger.h"

namespace HEMAX_Path
{
    std::string HEMAX_PathPrefix_HFS_Resolved;

    bool
    IsPathRelative(const std::string& Path)
    {
        QFileInfo FileInfo(Path.c_str());
        return FileInfo.isRelative();
    }

    std::string
    GetDirectory(const std::string& Path)
    {
        QFileInfo FileInfo(Path.c_str());
        return FileInfo.dir().path().toStdString();
    }

    std::string
    GetAbsoluteFilePath(const std::string& WorkingDir, const std::string& RelPath)
    {
        QDir Dir(WorkingDir.c_str());
        return QFileInfo(Dir.absoluteFilePath(RelPath.c_str())).canonicalFilePath().toStdString();
    }

    std::string
    ResolvePathPrefixes(const std::string& Path)
    {
        if (Path[0] != '<')
        {
            return Path;
        }
        else
        {
            size_t Found = Path.find_first_of('>');
            if (Found != std::string::npos)
            {
                std::string Prefix = std::string(Path.begin(), Path.begin() + Found + 1);

                if (Prefix == HEMAX_PathPrefix_HFS)
                {
                    std::string ResolvedPath = HEMAX_PathPrefix_HFS_Resolved + std::string(Path.begin() + Found + 1, Path.end());
                    std::replace(ResolvedPath.begin(), ResolvedPath.end(), '\\', '/');
                    return ResolvedPath;
                }
                else if (Prefix == HEMAX_PathPrefix_HoudiniToolPath)
                {
                    char HoudiniToolPath[2048];
                    if (GetEnvironmentVariableA("HOUDINI_TOOL_PATH", HoudiniToolPath, 2048))
                    {
                        std::string ResolvedPath = std::string(HoudiniToolPath) + std::string(Path.begin() + Found + 1, Path.end());
                        std::replace(ResolvedPath.begin(), ResolvedPath.end(), '\\', '/');
                        return ResolvedPath;
                    }
                    else
                    {
                        HEMAX_Logger::Instance().AddEntry("HEMAX_Path: ResolvePathPrefixes(...) :: Attempted to resolve a path that uses HOUDINI_TOOL_PATH environment variable, but this "
                                                            "variable is not set", HEMAX_LOG_LEVEL_WARN);
                        return Path;
                    }
                }
                else
                {
                    HEMAX_Logger::Instance().AddEntry("HEMAX_Path: ResolvePathPrefixes(...) :: Unsupported path prefix found : " + Prefix + ". Cannot resolve path.", HEMAX_LOG_LEVEL_WARN);
                    return Path;
                }
            }
            else
            {
                return Path;
            }
        }
    }

    bool
    DoesFileEixst(std::string Path)
    {
        QFileInfo FileInfo(Path.c_str());
        return FileInfo.exists();
    }
}