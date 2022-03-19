#include"FIleManager.h"

bool FileManager::GetAllFileInfo(const char* nPath,FolderInfo& dir)
{
    dir.info.fileName = nPath;
    dir.info.fileType = FileAttrHelper::GetType(nPath);
    if(!(dir.info.fileType == FILE_TYPE::FILET_DIR))
    {
        return false;
    }
    DIR *dp = NULL;
    struct dirent *dirp = NULL;
    if((dp = opendir(nPath)) == NULL)
    {
        return false;
    }
    char buf[1024] = {0};
    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0)
        {
            continue;
        }
        //path
        memset(buf,0,sizeof(buf));
        sprintf(buf,"%s/%s",nPath,dirp->d_name);
        if(FileAttrHelper::IsFolderByType(dirp->d_type))
        {
            FolderInfo folder;
            if(GetAllFileInfo(buf,folder))
            {
                dir.folderList.push_back(folder);
            }
        }
        else
        {
            FileInfo info(*dirp);
            info.fileName = buf;
            dir.infolist.push_back(info);
        }
    }
    closedir(dp);
    return true;
}
bool FileManager::GetAllFileInfoEx(const char* nPath,FolderInfoEx& dir)
{
    dir.info.info.fileName = nPath;
    struct stat st = FileAttrHelper::GetAttr(nPath);
    dir.info.info.fileType = FileAttrHelper::fgettype(st.st_mode);
    if(!(dir.info.info.fileType == FILE_TYPE::FILET_DIR))
    {
        dir.info.size = st.st_size;
        return false;
    }
    DIR *dp = NULL;
    struct dirent *dirp = NULL;
    if((dp = opendir(nPath)) == NULL)
    {
        return false;
    }
    char buf[1024] = {0};
    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0)
        {
            continue;
        }
        //path
        memset(buf,0,sizeof(buf));
        sprintf(buf,"%s/%s",nPath,dirp->d_name);
        if(FileAttrHelper::IsFolderByType(dirp->d_type))
        {
            FolderInfoEx folder;
            if(GetAllFileInfoEx(buf,folder))
            {
                dir.folderList.push_back(folder);
            }
        }
        else
        {
            FileInfoEx info;
            info.info = *dirp;
            info.size = FileAttrHelper::GetFilesize(buf);
            info.info.fileName = buf;
            dir.infolist.push_back(info);
        }
    }
    closedir(dp);
    return true;
}
bool FileManager::GetAllFileInfo(const FolderInfo& dir,std::vector<FileInfo> &vecdir)
{
    vecdir.push_back(dir.info);
    for(auto it = dir.infolist.begin();it!=dir.infolist.end();it++)
    {
        vecdir.push_back(*it);
    }
    for(auto it = dir.folderList.begin();it!=dir.folderList.end();it++)
    {
        GetAllFileInfo(*it,vecdir);
    }
}
bool FileManager::GetAllFileInfo(std::vector<FileInfo> &vecdir)
{
    char szPath[256] = {0};
    getcwd(szPath,sizeof(szPath));
    return GetAllFileInfo(szPath,vecdir);
}
bool FileManager::GetAllFileInfoEx(std::vector<FileInfoEx> &vecdir)
{
    char szPath[256] = {0};
    getcwd(szPath,sizeof(szPath));
    return GetAllFileInfoEx(szPath,vecdir);
}
bool FileManager::GetAllFileInfo(const char* nPath,std::vector<FileInfo> &vecdir)
{
    vecdir.clear();
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(nPath)) == NULL)
    {
        return false;
    }
    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0)
        {
            continue;
        }
        vecdir.push_back(*dirp);
    }
    closedir(dp);
    return true;
}
bool FileManager::GetAllFileInfoEx(const char* nPath,std::vector<FileInfoEx> &vecdir)
{
    vecdir.clear();
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(nPath)) == NULL)
    {
        return false;
    }
    char fullPath[1024] = {0};
    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0)
        {
            continue;
        }
        memset(fullPath,0,sizeof(fullPath));
        sprintf(fullPath,"%s/%s",nPath,dirp->d_name);
        FileInfoEx info;
        info.info = (*dirp);
        struct stat st = FileAttrHelper::GetAttr(fullPath);
        info.size = st.st_size;
        vecdir.push_back(info);
    }
    closedir(dp);
    return true;
}
bool FileManager::CreatedMultipleDirectory(const std::string& direct)
{
    std::string Directoryname = direct;
    if (Directoryname[Directoryname.length() - 1] != '/')
    {
        Directoryname.append(1, '/');
    }
    std::vector< std::string> vpath;
    std::string strtemp;
    bool  bSuccess = false;
    for (int i = 0; i < Directoryname.length(); i++)
    {
        if (Directoryname[i] != '/')
        {
            strtemp.append(1, Directoryname[i]);
        }
        else
        {
            vpath.push_back(strtemp);
            strtemp.append(1, '/');
        }
    }
    std::vector< std::string>::iterator vIter = vpath.begin();
    for (; vIter != vpath.end(); vIter++)
    {
        bSuccess = CreateDir(vIter->c_str()) == 0? true : false;
    }
    return bSuccess;
}
int FileManager::CreateDir(const char* nPath,int mode)
{
    return mkdir(nPath,mode);
}
int FileManager::RemoveEmptyDir(const char* nPath)
{
    return rmdir(nPath);
}
int FileManager::Link(const char* oldPath,const char* newPath)
{
    return link(oldPath,newPath);
}
int FileManager::Symlink(const char* srcPath,const char* destPath)
{
    return symlink(srcPath,destPath);
}
int FileManager::Unlink(const char* path)
{
    return unlink(path);
}
int FileManager::Remove(const char* nPath)
{
    return remove(nPath);               //== rmdir or unlink
}
int FileManager::Rename(const char* oldPath,const char* newPath,int trunc)
{
    if(trunc)
    {
        return rename(oldPath,newPath);
    }
    else
    {
        int iret = 0;
        if(FileAttrHelper::IsFolder(oldPath))
        {
            iret = rmdir(oldPath);
            if(iret == -1)
            {
                return iret;
            }
            iret = mkdir(newPath,0755);
            return iret;
        }
        else
        {
            int iret = Link(oldPath,newPath);
            if(iret == -1)
            {
                return iret;
            }
            iret = unlink(oldPath);
            return iret;
        }
    }
    return -1;
}

int FileManager::Copy(const char* oldfile,const char* newfile,int bLink)
{
    if(bLink)
    {
        return link(oldfile,newfile);
    }
        //copy
    else
    {
        int fdold = -1;
        int fdnew = -1;
        int iret = -1;
        do{
            fdold = open(oldfile,O_RDONLY);
            if(fdold == -1)
            {
                perror(oldfile);
                break;
            }
            fdnew = open(newfile,O_WRONLY | O_CREAT);
            if(fdnew == -1)
            {
                perror(newfile);
                break;
            }
            char buffer[4096] = {0};
            iret = read(fdold,buffer,sizeof(buffer));
            do
            {
                iret = read(fdold,buffer,sizeof(buffer));
                if(iret <=0 )
                {
                    perror("read end");
                    break;
                }
                else
                {
                    iret = write(fdnew,buffer,iret);
                    if(iret <=0 )
                    {
                        perror("write error");
                        break;
                    }
                }
            }while(iret > 0);
        }while(0);
        if(fdold!=-1)
        {
            close(fdold);
            fdold = -1;
        }
        if(fdnew!=-1)
        {
            close(fdnew);
            fdnew = -1;
        }
        return iret;
    }
}
