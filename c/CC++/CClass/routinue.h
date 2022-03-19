#ifndef ROUTINUE_H
#define ROUTINUE_H
#include<stdio.h>
typedef struct
{
    void(*Awake)();
    void(*Start)();
    void(*Tick)();
    void(*Destroy)();
}VirtualTable;

//c 多态
typedef struct
{
    VirtualTable vt;
    void(*ThreadMain)();
}ThreadBase;

typedef struct
{
    ThreadBase base;
}UserRoutinue;

void UserAwake()
{
    printf("UserAwake \n");
}
void UserStart()
{
    printf("UserStart \n");
}
void UserTick()
{
    printf("UserTick \n");
}
void UserDestroy()
{
    printf("UserDestroy \n");
}
void Init_UserRoutinue(UserRoutinue* pRoutinue)
{
    pRoutinue->base.vt.Awake = UserAwake;
    pRoutinue->base.vt.Start = UserStart;
    pRoutinue->base.vt.Tick = UserTick;
    pRoutinue->base.vt.Destroy = UserDestroy;
}


void mainVirmain()
{
    UserRoutinue ur;
    Init_UserRoutinue(&ur);
    ur.base.vt.Awake();
    ur.base.vt.Start();
    ur.base.vt.Tick();
    ur.base.vt.Destroy();
}
#endif
