#include <iostream>
#include"FIleManager.h"
#include"PathHelper.h"
#include"FileAttrHelper.h"
int main()
{
    int iRet = 0;
    iRet = PathHelper::SetCurDir(PathHelper::GetHome().c_str());
    if(iRet == -1)
    {
        perror("SetCurDir");
    }
    FileManager::Remove((PathHelper::GetCurDir() + "text").c_str());
    FileManager::Remove((PathHelper::GetCurDir() + "new").c_str());
    iRet = FileManager::CreateDir((PathHelper::GetCurDir() + "text").c_str());
    if(iRet == -1)
    {
        perror("CreateDir");
    }
    std::vector<FileInfo> vecdir;
    FileManager::GetAllFileInfo(vecdir);
    for (auto it = vecdir.begin(); it != vecdir.end(); it++)
    {
        if(FileAttrHelper::IsFolderByType(it->fileType))
        {
            std::cout<<"目录  ";
        }
        else if(FileAttrHelper::IsNormalFileByType(it->fileType))
        {
            std::cout<<"普通文件    ";
        }
        std::cout << "type:" << (int)it->fileType << "   name:" << it->fileName << std::endl;
    }

    std::cout<<"------------------------------------------"<<std::endl;
    iRet = FileManager::Rename((PathHelper::GetCurDir() + "text").c_str(),
                        (PathHelper::GetCurDir() + "new").c_str(),false);
    if(iRet == -1)
    {
        perror("Rename");
    }
    FileManager::GetAllFileInfo(vecdir);
    for (auto it = vecdir.begin(); it != vecdir.end(); it++)
    {
        if(FileAttrHelper::IsFolderByType(it->fileType))
        {
            std::cout<<"目录  ";
        }
        else if(FileAttrHelper::IsNormalFileByType(it->fileType))
        {
            std::cout<<"普通文件    ";
        }
        std::cout << "type:" << (int)it->fileType << "   name:" << it->fileName << std::endl;
    }

    std::cout<<"------------------------------------------"<<std::endl;
    iRet = FileManager::Remove((PathHelper::GetCurDir() + "new").c_str());
    if(iRet == -1)
    {
        perror("CreateDir");
    }
    FileManager::GetAllFileInfo(vecdir);
    for (auto it = vecdir.begin(); it != vecdir.end(); it++)
    {
        if(FileAttrHelper::IsFolderByType(it->fileType))
        {
            std::cout<<"目录  ";
        }
        else if(FileAttrHelper::IsNormalFileByType(it->fileType))
        {
            std::cout<<"普通文件    ";
        }
        std::cout << "type:" << (int)it->fileType << "   name:" << it->fileName << std::endl;
    }

    FolderInfo dir;
    FileManager::GetAllFileInfo(PathHelper::GetCurDir().c_str(),dir);
    vecdir.clear();
    FileManager::GetAllFileInfo(dir,vecdir);
    for (auto it = vecdir.begin(); it != vecdir.end(); it++)
    {
        if(FileAttrHelper::IsFolderByType(it->fileType))
        {
            std::cout<<"目录  ";
        }
        else if(FileAttrHelper::IsNormalFileByType(it->fileType))
        {
            std::cout<<"普通文件    ";
        }
        std::cout << "type:" << (int)it->fileType << "   name:" << it->fileName << std::endl;
    }
    printf("main.cpp isFolder %d\n",FileAttrHelper::IsFolder((PathHelper::GetCurDir() + "桌面").c_str()));
    printf("IsFileExist %d \n",IsFileExist((PathHelper::GetCurDir() + "桌面").c_str()));
    printf("IsFileCanExec %d \n",IsFileCanExec((PathHelper::GetCurDir() + "桌面").c_str()));
    printf("IsFileCanRead %d \n",IsFileCanRead((PathHelper::GetCurDir() + "桌面").c_str()));
    printf("IsFileCanWrite %d \n",IsFileCanWrite((PathHelper::GetCurDir() + "桌面").c_str()));
    return 0;
}
