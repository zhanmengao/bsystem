// EnumModule.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

void EnumModule()
{
    DWORD *PEB         = NULL,         
        *Ldr         = NULL,        
        *Flink       = NULL,         
        *p           = NULL,         
        *BaseAddress = NULL,         
        *FullDllName = NULL;
    
    // ��λPEB
    __asm    
    {        
        // fsλ�ñ�����teb
        // fs:[0x30]λ�ñ�����peb
        mov     eax,fs:[0x30]        
        mov     PEB,eax    
    }    
    
    // �õ�LDR
    Ldr   = *( ( DWORD ** )( ( unsigned char * )PEB + 0x0c ) );
    
    // �ڶ�������
    Flink = *( ( DWORD ** )( ( unsigned char * )Ldr + 0x14 ) ); 
    p     = Flink;   
    
    p = *( ( DWORD ** )p );  
    
    while ( Flink != p )
    {   
        BaseAddress = *( ( DWORD ** )( ( unsigned char * )p + 0x10 ) );        
        FullDllName = *( ( DWORD ** )( ( unsigned char * )p + 0x20 ) );   
        
        if ( BaseAddress == 0 )
        {
            break;
        }
        
        printf("ImageBase = %08x \r\n ModuleFullName = %S \r\n",
                BaseAddress, (unsigned char *)FullDllName);
        
        p = *( ( DWORD ** )p );
    }
}

int main(int argc, char* argv[])
{
    EnumModule();

	return 0;
}
