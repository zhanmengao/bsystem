//
// Created by zhanmengao on 10/12/20.
//

#ifndef LFILEMANAGER_PATHHELPER_H
#define LFILEMANAGER_PATHHELPER_H
#include"../../../Linux/apue.h"
#include"../../../File/FileManager/CFileManager.h"
static const char* GetHome()
{
    return getenv("HOME");
}

static const char* GetAppFolder()
{
    return "/usr/share/applications/";
}

static const char* GetDesktopFolder(char* path)
{
    const char* home = GetHome();
    sprintf(path,"%s/%s",home,"Desktop");
    if(IsFileExist(path))
    {
        return path;
    }
    else
    {
        sprintf(path,"%s/%s",home,"桌面");
    }
    return path;
}

static const char* GetAutoStartFolder(char *path)
{
    const char* home = GetHome();
    sprintf(path,"%s/%s",home,".config/autostart/");
    return path;
}
#ifdef __cplusplus
#include<string>
class PathHelper
{
public:
    static  std::string  GetCurDir()
    {
        char szPath[256] = {0};
        getcwd(szPath,sizeof(szPath) * sizeof(char));
        return std::string(szPath) + "/";
    }
    //得到上一级目录
    static std::string GetUpFolder(const std::string& strPath)
    {
        std::string ret;
        int nPos = strPath.find_last_of('/');
        ret = std::string(strPath, 0, nPos);
        if (nPos == strPath.size() - 1)
        {
            nPos = std::string(strPath, 0, nPos - 1).find_last_of("/");
            ret = std::string(strPath, 0, nPos);
        }
        //如果已经退到root
        if (ret.size() <= 2)
        {
            ret += "/";
        }
        return ret;
    }
    static int SetCurDir(const char* nPath)
    {
        return chdir(nPath);
    }
    static std::string GetHome()
    {
        return std::string(getenv("HOME")) + "/";
    }
    static void Back()
    {
        std::string curDir = PathHelper::GetCurDir();
        std::string upDir = PathHelper::GetUpFolder(curDir);
        if (curDir != upDir)
        {
            SetCurDir(upDir.c_str());
        }
    }
    static void To(const std::string& fileName)
    {
        //绝对路径
        if (fileName.find('/') != std::string::npos)
        {
            SetCurDir(fileName.c_str());
        }
        //相对路径
        else
        {
            std::string tFilePath = PathHelper::GetCurDir() + fileName;
            SetCurDir(tFilePath.c_str());
        }

    }
};
#endif
#endif //LFILEMANAGER_PATHHELPER_H
