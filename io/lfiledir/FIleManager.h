//
// Created by zhanmengao on 10/12/20.
//

#ifndef LFILEMANAGER_FILEMANAGER_H
#define LFILEMANAGER_FILEMANAGER_H
#include"../../../Linux/apue.h"
#include<dirent.h>
#include"../CFileManager.h"
#include"FileAttrHelper.h"
#ifdef __cplusplus
#include<vector>
#include<string>
#endif

#ifdef __cplusplus
struct FileInfo
{
    std::string fileName;
    FILE_TYPE fileType;
    FileInfo()
    {
    }
    FileInfo(const dirent& obj):fileName(obj.d_name),fileType((FILE_TYPE)obj.d_type)
    {
    }
    FileInfo& operator=(const dirent& obj)
    {
        fileName = std::string(obj.d_name);
        fileType = (FILE_TYPE)obj.d_type;
        return *this;
    }
};
struct FileInfoEx
{
    FileInfo info;
    size_t size;
};
struct FolderInfo
{
    std::vector<FileInfo> infolist;                 //file info
    std::vector<FolderInfo> folderList;             //folder info
    FileInfo info;                                  //my info
};
struct FolderInfoEx
{
    std::vector<FileInfoEx> infolist;                 //file info
    std::vector<FolderInfoEx> folderList;             //folder info
    FileInfoEx info;                                  //my info
};

class FileManager
{
public:
    static bool GetAllFileInfo(const char* nPath,FolderInfo& dir);          //Recursion Call Get All Folder File
    static bool GetAllFileInfoEx(const char* nPath,FolderInfoEx& dir);      //Recursion Call Get All Folder FileEx
    static bool GetAllFileInfo(std::vector<FileInfo> &vecdir);                          //get pwd file
    static bool GetAllFileInfo(const char* nPath,std::vector<FileInfo> &vecdir);            //get path file
    static bool GetAllFileInfoEx(std::vector<FileInfoEx> &vecdir);                          //get pwd fileex
    static bool GetAllFileInfoEx(const char* nPath,std::vector<FileInfoEx> &vecdir);       //get file ex

    static bool GetAllFileInfo(const FolderInfo& dir,std::vector<FileInfo> &vecdir);        //add all fileinfo in vecdir

    static bool CreatedMultipleDirectory(const std::string& direct);
    static int CreateDir(const char* nPath,int mode = 0644);
    static int RemoveEmptyDir(const char* nPath);
    static int Link(const char* oldPath,const char* newPath);
    static int Symlink(const char* srcPath,const char* destPath);
    static int Unlink(const char* path);
    static int Remove(const char* nPath);
    static int Rename(const char* oldPath,const char* newPath,int trunc);
    static int Copy(const char* oldfile,const char* newfile,int bLink);
};
#endif
#endif //LFILEMANAGER_FILEMANAGER_H
