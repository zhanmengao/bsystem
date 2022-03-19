/*********************************************************
 *  des.h
 *  用户使用des算法头文件
 *	
 *********************************************************/
#ifndef _OPENDESS_H_
#define _OPENDESS_H_

#ifdef __cplusplus
extern "C" {
#endif

//ab\0defg

//用户使用的函数
int DesEnc(
		unsigned char *pInData,
		int            nInDataLen,
		unsigned char *pOutData,
		int           *pOutDataLen);

//用户使用函数des解密
int DesDec(
	   unsigned char *pInData,
	   int            nInDataLen,
	   unsigned char *pOutData,
	   int           *pOutDataLen);

#ifdef __cplusplus
}
#endif

#endif

