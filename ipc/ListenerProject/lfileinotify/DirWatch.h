//
// Created by uzi on 12/8/20.
//

#ifndef LFILEINOTIFY_DIRWATCH_H
#define LFILEINOTIFY_DIRWATCH_H
#include"FileWatch.h"

class DirListener:public FileListener
{
public:
    DirListener(const char* path):mPath(path)
    {
        FileListener::AddListen((mPath).c_str());
    }

    void Read(std::vector<notifyInfo>& vec)
    {
        FileListener::Read(vec);
        for(auto it = vec.begin();it!=vec.end();it++)
        {
            it->name = mPath + it->name;
        }
    }
private:
    bool AddListen(const char* filePath)
    {
        return FileListener::AddListen((mPath + filePath).c_str());
    }
    bool DelListen(const char* filePath)
    {
        return FileListener::DelListen((mPath + filePath).c_str());
    }
protected:
    std::string mPath;
};
#endif //LFILEINOTIFY_DIRWATCH_H
