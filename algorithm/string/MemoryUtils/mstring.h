#pragma once
#include<stdint.h>

uint32_t mstrlen(const char* str);
char* mstrcpy(char* dest,const char* src);

void* mmemcpy(void* dest, const void* src, uint32_t size);


void* mmemmove(void* dest, const void* src, uint32_t size);