#include"UserManager.h"

std::vector<OSUser> UserManager::GetUserList()
{
    std::vector<OSUser> ret;
    struct passwd* ptr;
    setpwent();
    while((ptr = getpwent())!=NULL)
    {
        spwd  *ps = getspnam(ptr->pw_name);
        OSUser u;
        if(ps)
        {
            u.max = ps->sp_max;
            u.min = ps->sp_min;
            u.expire = ps->sp_expire;
            u.lstchg = ps->sp_lstchg;
            u.passwd = ps->sp_pwdp;
            u.warn = ps->sp_warn;
            u.inact = ps->sp_inact;
        }
        u.dir = ptr->pw_dir;
        u.gid = ptr->pw_gid;
        u.uid = ptr->pw_uid;
        u.gecos = ptr->pw_gecos;
        u.shell = ptr->pw_shell;
        u.uName = ptr->pw_name;
        ret.push_back(u);
    }
    return ret;
}
std::vector<OSGroup> UserManager::GetGroupList()
{
    std::vector<OSGroup> ret;
    struct group* ptr;
    setpwent();
    while((ptr = getgrent())!=NULL)
    {
        OSGroup g;
        g.gid = ptr->gr_gid;
        g.gName = ptr->gr_name;
        g.passwd = ptr->gr_passwd;
        int idx = 0;
        while(ptr->gr_mem[idx]!=NULL)
        {
            g.gUserList.push_back(ptr->gr_mem[idx]);
            idx++;
        }
        ret.push_back(g);
    }
    return ret;
}
