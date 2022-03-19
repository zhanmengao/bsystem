// HideModule.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

void HideModule(char *szModule)
{
    DWORD *PEB         = NULL,         
        *Ldr         = NULL,        
        *Flink       = NULL,         
        *p           = NULL,         
        *BaseAddress = NULL,         
        *FullDllName = NULL;    
    __asm    
    {        
        mov     eax,fs:[0x30]        
        mov     PEB,eax    
    }    
    
    HMODULE hMod = GetModuleHandle(szModule);

    Ldr   = *( ( DWORD ** )( ( unsigned char * )PEB + 0x0c ) );
    Flink = *( ( DWORD ** )( ( unsigned char * )Ldr + 0x0c ) ); 
    p     = Flink;    

    do    
    {        
        BaseAddress = *( ( DWORD ** )( ( unsigned char * )p + 0x18 ) );        
        FullDllName = *( ( DWORD ** )( ( unsigned char * )p + 0x28 ) );
        if (BaseAddress == (DWORD *)hMod)
        {
            **( ( DWORD ** )(p + 1) ) = (DWORD)*( ( DWORD ** )p );
            *(*( ( DWORD ** )p ) + 1) = (DWORD)*( ( DWORD ** )(p + 1) );
            break;
        }
        p = *( ( DWORD ** )p );    
    }    while ( Flink != p ); 
    
    Flink = *( ( DWORD ** )( ( unsigned char * )Ldr + 0x14 ) ); 
    p     = Flink;    
    do    
    {    
        BaseAddress = *( ( DWORD ** )( ( unsigned char * )p + 0x10 ) );        
        FullDllName = *( ( DWORD ** )( ( unsigned char * )p + 0x20 ) );        
        if (BaseAddress == (DWORD *)hMod)
        {
            **( ( DWORD ** )(p + 1) ) = (DWORD)*( ( DWORD ** )p );
            *(*( ( DWORD ** )p ) + 1) = (DWORD)*( ( DWORD ** )(p + 1) );
            break;
        }
        p = *( ( DWORD ** )p );    
    }    while ( Flink != p ); 
    
    Flink = *( ( DWORD ** )( ( unsigned char * )Ldr + 0x1c ) );
    p     = Flink;    
    do    
    {        
        BaseAddress = *( ( DWORD ** )( ( unsigned char * )p + 0x8 ) );        
        FullDllName = *( ( DWORD ** )( ( unsigned char * )p + 0x18 ) );        
        if (BaseAddress == (DWORD *)hMod)
        {
            **( ( DWORD ** )(p + 1) ) = (DWORD)*( ( DWORD ** )p );
            *(*( ( DWORD ** )p ) + 1) = (DWORD)*( ( DWORD ** )(p + 1) );
            break;
        }
        p = *( ( DWORD ** )p );    
    }    while ( Flink != p ); 
}

int main(int argc, char* argv[])
{
    HideModule("kernel32.dll");

    getchar();

	return 0;
}
