#pragma once
#define NDEBUG          // 加上这行，则 assert 不可用
#include <assert.h>

void* p;
void assertTest()
{
	assert(p != NULL);    // assert 不可用
}
