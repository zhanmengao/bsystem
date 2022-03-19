#pragma once
#pragma pack(push)  // 保存对齐状态
#pragma pack(4)     // 设定为 4 字节对齐
struct test
{
    char m1;
    double m4;
    int m3;
};
#pragma pack(pop)   // 恢复对齐状态

/*
 * 由于编译器默认情况下会对这个struct作自然边界（有人说“自然对界”我觉得边界更顺口）对齐，
 * 结构的第一个成员x1，其偏移地址为0，占据了第1个字节。
 * 第二个成员x2为short类型，其起始地址必须2字节对，因界此，编译器在x2和x1之间填充了一个空字节。
 * 结构的第三个成员x3和第四个成员x4恰好落在其自然边界地址上，在它们前面不需要额外的填充字节。
 * 在test结构中，成员x3要求4字节对界，是该结构所有成员中要求的最大边界单元，
 * 因而test结构的自然对界条件为4字节，编译器在成员x4后面填充了3个空字节。整个结构所占据空间为12字节。
 */
struct test2
{
    char x1;
    short x2;
    float x3;
    char x4;
};

//min (i,j) = 4，整体向4字节对齐s
//
typedef struct
{
    char a;			//offset = 0 放在0号位置（+1）
    float b;        //offset = min(4,4)的整数倍，所以应放在index = 4的位置 4（+4）
    short c;        //offset = min(2,4)的整数倍，所以应放在index = 8 的位置 8（+2）
    char d;         //offset = min(1,4)的整数倍，应放在index = 10的位置	10（+1） 。
}ss;					//整体对齐，大小为12

//min(i,j) == 4
class node
{
	char c;			//offset = 0，放在0号位置（+1）
	int i;				//offset = min(4,4)的整数倍，所以应放在index = 4的位置	4（+4）
	short s;			//offset = min(2,4)的整数倍，所以应放在index = 8的位置  8 (+2)
};						//整体对齐，大小为12
