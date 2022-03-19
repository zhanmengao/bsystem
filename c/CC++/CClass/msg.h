#ifndef MSG_H
#define MSG_H
#include<stdio.h>
//c 继承
struct MsgBase
{
    int msgID;
};

struct Msg_Send2Client
{
    struct MsgBase base;
    char data[1024];
    unsigned int mType;
};

struct Msg_Close_Client
{
    struct MsgBase base;
    int sock;
};

void mainMsg()
{
    struct Msg_Send2Client sc;
    sc.base.msgID = 1;
    struct Msg_Close_Client cc;
    cc.base.msgID = 2;
    printf("sizeof(MsgBase) = %d \n",sizeof (struct MsgBase));
    printf("sizeof(Msg_Send2Client) = %d sc.msgid = %d\n",sizeof(sc),sc.base.msgID);
    printf("sizeof(Msg_Close_Client) = %d cc.msgid = %d\n",sizeof (cc),cc.base.msgID);
}
#endif
