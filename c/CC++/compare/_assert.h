#pragma once
#define NDEBUG          // �������У��� assert ������
#include <assert.h>

void* p;
void assertTest()
{
	assert(p != NULL);    // assert ������
}
