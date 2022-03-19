#pragma once
#include"ByteUtils.h"
#include"../bit/BitUtils.h"
#include<stdint.h>
#ifdef _WIN32
#include<Windows.h>
#pragma comment(lib,"Ws2_32.lib")  
#else
#include<sys/socket.h>
#include<arpa/inet.h>
#endif
typedef struct
{
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
} sByte;				//1���ֽ�
typedef struct
{
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
    unsigned char bit8 : 1;
    unsigned char bit9 : 1;
    unsigned char bit10 : 1;
    unsigned char bit11 : 1;
    unsigned char bit12 : 1;
    unsigned char bit13 : 1;
    unsigned char bit14 : 1;
    unsigned char bit15 : 1;
} tShort;				//2���ֽ�

typedef union
{
    unsigned char byte;
    sByte  bit;
}uByte8;			//һ���ֽڣ��������Сȡ��Ա������
typedef union
{
    unsigned short sint;
    tShort  bit;
}uByte16;			//2���ֽڣ��������Сȡ��Ա������
int main()
{
    uByte8 p = { 0 };
    p.byte = 0x01;                              //b 0000 0001��˷�����λ��ǰ����λΪbit7��
    PrintfBinary<unsigned char>(p.byte);        //b 1000 0000С�˷�����λ��ǰ����λΪbit0��
    if (p.bit.bit0 == 1)
    {
#ifdef _WIN32
        printf("��һ��λΪ1��С�˷�\n");
#else
        printf("bit0 is 1 : Small\n");
#endif
    }
    else
    {
#ifdef _WIN32
        printf("��һ��λΪ0����˷�\n");
#else
        printf("bit0 is 0 : Big\n");
#endif
    }
    printf("Big %d \n", isBigEndian());
    PrintfBinary((int)1);
    PrintfBinary((unsigned)1);
    PrintfBinary(1.0f);
    PrintfBinary((int)INT32_MAX);
    PrintfBinary((int)UINT32_MAX);
    uByte16 us = { 0 };
    us.sint = 1;
    us.sint = htons(us.sint);
    if (us.bit.bit0 == 1)
    {
        //printf("��һ��λΪ1��С�˷�\n");
        printf("htons is Small\n");
    }
    else
    {
        //printf("��һ��λΪ0����˷�\n");
        printf("htons is Big\n");
    }

    int val = 0x01234567;
    char data = val;					//�͵�ַ�����λ��Ϊ��˷������λ��ΪС�˷�
    char* pData = (char*)&val;
    printf("val %u \n", pData);
    if (data == 0x67)
    {
        printf("Small \n");
        pData++;
        if (*pData == 0x45)
        {
            printf("Small \n");
            pData++;
            if (*pData == 0x23)
            {
                printf("Small \n");
                pData++;
                if (*pData == 0x01)
                {
                    printf("Small \n");
                }
            }
        }
    }
    else if (data == 0x01)
    {
        printf("�͵�ַ���λ \n");
    }
    return 0;
}
