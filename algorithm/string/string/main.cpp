#include"vector.hpp"
#include<iostream>
#include"List.hpp"
#include"BinarySearchTree.h"
#include"CString.h"
#include"BinaryTree.h"
#include"Tree23.h"
#include"RedBlackTree.h"
int mainCS()
{
    const char* url = "http://typdm.tydevice.com/?jsonstr="
                      "{\"VER\":\"01\""
                      ",\"CTEI\":\"CT1234561234567\""
                      ",\"MAC\":\"24:e2:71:f4:d7:b0\""
                      ",\"IP\":\"192.168.0.1\","
                      "\"UPLINKMAC\":\"01:02:03:04:05:06\","
                      "\"LINK\":\"1\",\"FWVER\":\"1.1\",\"DATE\":\"2018-07-01 12:05:30\"}";
    struct cstring str;
    String_InitStr(&str,url,strlen(url));
    String_ReplaceAllStr(&str," ","%20");
    printf("%s \n",str.string);
    return 0;
}
int mainTree()
{
    //stb::mainBT();
    //stb::mainBST();
    stb::mainRBT();
}
int main()
{
    //mainArr();
    //mainStack();
    //mainSort();
    //mainBT();
    //ListMain();
    //mainBitmap();
    mainTree();
    return 0;
}
