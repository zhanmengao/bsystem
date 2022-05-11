#ifndef USERMANAGER_H
#define USERMANAGER_H

#include<string>
#include<unistd.h>
#include<pwd.h>
#include<shadow.h>
#include<grp.h>
#include<vector>
struct OSUser
{
    std::string uName;
    std::string passwd;
    gid_t gid;
    uid_t uid;
    std::string gecos;
    std::string dir;
    std::string shell;
    int lstchg;
    int min;
    int max;
    int warn;
    int inact;
    int expire;
};

struct OSGroup
{
    std::string gName;
    std::string passwd;
    gid_t gid;
    std::vector<std::string> gUserList;
};

class UserManager
{
public:
    static std::vector<OSUser> GetUserList();
    static std::vector<OSGroup> GetGroupList();
};

#endif
