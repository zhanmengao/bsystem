#include <stdio.h>
#include"dict.h"
dictType hashDictType = {
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
};
int main()
{
    dict * dic = dictCreate(&hashDictType,NULL);
    if (dic == NULL){
        printf("dic is null \n");
    }
    int ret = dictAdd(dic,"aaa","bbb");
    if(ret == DICT_OK){
        printf("add ok \n");
    }else{
        printf("add fail \n");
    }
    return 0;
}
