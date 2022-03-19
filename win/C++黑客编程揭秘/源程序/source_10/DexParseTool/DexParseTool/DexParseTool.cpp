// DexParseTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "DexParse.h"

void dexFileSetupBasicPointers(DexFile* pDexFile, const u1* data) {
    DexHeader *pHeader = (DexHeader*) data;

    pDexFile->baseAddr = data;
    pDexFile->pHeader = pHeader;
    pDexFile->pStringIds = (const DexStringId*) (data + pHeader->stringIdsOff);
    pDexFile->pTypeIds = (const DexTypeId*) (data + pHeader->typeIdsOff);
    pDexFile->pFieldIds = (const DexFieldId*) (data + pHeader->fieldIdsOff);
    pDexFile->pMethodIds = (const DexMethodId*) (data + pHeader->methodIdsOff);
    pDexFile->pProtoIds = (const DexProtoId*) (data + pHeader->protoIdsOff);
    pDexFile->pClassDefs = (const DexClassDef*) (data + pHeader->classDefsOff);
    pDexFile->pLinkData = (const DexLink*) (data + pHeader->linkOff);
}

void dexFileFree(DexFile* pDexFile)
{
    if (pDexFile == NULL)
        return;

    free(pDexFile);
}

DexFile* dexFileParse(const u1* data, size_t length)
{
    DexFile* pDexFile = NULL;
    const DexHeader* pHeader;
    const u1* magic;
    int result = -1;

    pDexFile = (DexFile*) malloc(sizeof(DexFile));
    if (pDexFile == NULL)
        goto bail;      /* alloc failure */
    memset(pDexFile, 0, sizeof(DexFile));

    /*
     * Peel off the optimized header.
     */
    if (memcmp(data, DEX_OPT_MAGIC, 4) == 0) {
        magic = data;
        if (memcmp(magic+4, DEX_OPT_MAGIC_VERS, 4) != 0) {
            goto bail;
        }

        /* ignore the opt header and appended data from here on out */
        data += pDexFile->pOptHeader->dexOffset;
        length -= pDexFile->pOptHeader->dexOffset;
        if (pDexFile->pOptHeader->dexLength > length) {
            goto bail;
        }
        length = pDexFile->pOptHeader->dexLength;
    }

    dexFileSetupBasicPointers(pDexFile, data);
    pHeader = pDexFile->pHeader;


    /*
     * Success!
     */
    result = 0;

bail:
    if (result != 0 && pDexFile != NULL) {
        dexFileFree(pDexFile);
        pDexFile = NULL;
    }
    return pDexFile;
}

void PrintDexMapList(DexFile *pDexFile)
{
    const DexMapList *pDexMapList = dexGetMap(pDexFile);

    printf("DexMapList:\r\n");
    printf("TypeDesc\t\t type unused size offset\r\n");

    for ( u4 i = 0; i < pDexMapList->size; i ++ )
    {
        switch (pDexMapList->list[i].type)
        {
            case 0x0000:printf("kDexTypeHeaderItem");break;
            case 0x0001:printf("kDexTypeStringIdItem");break;
            case 0x0002:printf("kDexTypeTypeIdItem");break;
            case 0x0003:printf("kDexTypeProtoIdItem");break;
            case 0x0004:printf("kDexTypeFieldIdItem");break;
            case 0x0005:printf("kDexTypeMethodIdItem");break;
            case 0x0006:printf("kDexTypeClassDefItem");break;
            case 0x1000:printf("kDexTypeMapList");break;
            case 0x1001:printf("kDexTypeTypeList");break;
            case 0x1002:printf("kDexTypeAnnotationSetRefList");break;
            case 0x1003:printf("kDexTypeAnnotationSetItem");break;
            case 0x2000:printf("kDexTypeClassDataItem");break;
            case 0x2001:printf("kDexTypeCodeItem");break;
            case 0x2002:printf("kDexTypeStringDataItem");break;
            case 0x2003:printf("kDexTypeDebugInfoItem");break;
            case 0x2004:printf("kDexTypeAnnotationItem");break;
            case 0x2005:printf("kDexTypeEncodedArrayItem");break;
            case 0x2006:printf("kDexTypeAnnotationsDirectoryItem");break;
        }

        printf("\t %04X %04X %08X %08X\r\n", 
            pDexMapList->list[i].type,
            pDexMapList->list[i].unused,
            pDexMapList->list[i].size,
            pDexMapList->list[i].offset);
    }
}

void PrintStringIds(DexFile *pDexFile)
{
    printf("DexStringIds:\r\n");

    for ( u4 i = 0; i < pDexFile->pHeader->stringIdsSize; i ++ )
    {
        printf("%d.%s \r\n", i, dexStringById(pDexFile, i));
    }
}

void PrintTypeIds(DexFile *pDexFile)
{
    printf("DexTypeIds:\r\n");

    for ( u4 i = 0; i < pDexFile->pHeader->typeIdsSize; i ++ )
    {
        printf("%d %s \r\n", i, dexStringByTypeIdx(pDexFile, i));
    }
}

void PrintProtoIds(DexFile *pDexFile)
{
    printf("DexProtoIds:\r\n");

    // 对数据的解析
    for ( u4 i = 0; i < pDexFile->pHeader->protoIdsSize; i ++ )
    {
        const DexProtoId *pDexProtoId = dexGetProtoId(pDexFile, i);
        // 输出原始数据
        printf("%08X %08X %08X \r\n", pDexProtoId->shortyIdx, pDexProtoId->returnTypeIdx, pDexProtoId->parametersOff);
        // 输出对应的TypeId
        printf("%s %s\r\n", 
            dexStringById(pDexFile, pDexProtoId->shortyIdx),
            dexStringByTypeIdx(pDexFile, pDexProtoId->returnTypeIdx));

        // 获得参数列表
        const DexTypeList *pDexTypeList = dexGetProtoParameters(pDexFile, pDexProtoId);

        u4 num = pDexTypeList != NULL ? pDexTypeList->size : 0;
        // 输出参数
        for ( u4 j = 0; j < num; j ++ )
        {
            printf("%s ", dexStringByTypeIdx(pDexFile, pDexTypeList->list[j].typeIdx));
        }
        printf("\r\n");
    }

    printf("\r\n");

    // 对解析数据的简单还原
    for ( u4 i = 0; i < pDexFile->pHeader->protoIdsSize; i ++ )
    {
        const DexProtoId *pDexProtoId = dexGetProtoId(pDexFile, i);
        printf("%s", dexStringByTypeIdx(pDexFile, pDexProtoId->returnTypeIdx));
        printf("(");

        // 获得参数列表
        const DexTypeList *pDexTypeList = dexGetProtoParameters(pDexFile, pDexProtoId);

        u4 num = pDexTypeList != NULL ? pDexTypeList->size : 0;
        // 输出参数
        for ( u4 j = 0; j < num; j ++ )
        {
            printf("%s\b, ", dexStringByTypeIdx(pDexFile, pDexTypeList->list[j].typeIdx));
        }
        
        if ( num == 0 )
        {
            printf(");\r\n");
        }
        else 
        {
            printf("\b\b);\r\n");
        }        
    }
}

void PrintFieldIds(DexFile *pDexFile)
{
    printf("DexFieldIds:\r\n");

    for ( u4 i = 0; i < pDexFile->pHeader->fieldIdsSize; i ++ )
    {
        const DexFieldId *pDexFieldId = dexGetFieldId(pDexFile, i);

        printf("%04X %04X %08X \r\n", pDexFieldId->classIdx, pDexFieldId->typeIdx, pDexFieldId->nameIdx);
        printf("%s %s %s\r\n", 
            dexStringByTypeIdx(pDexFile, pDexFieldId->classIdx),
            dexStringByTypeIdx(pDexFile, pDexFieldId->typeIdx),
            dexStringById(pDexFile, pDexFieldId->nameIdx));
    }
}

void PrintMethodIds(DexFile *pDexFile)
{
    printf("DexMethodIds:\r\n");

    // 对数据的解析
    for ( u4 i = 0; i < pDexFile->pHeader->methodIdsSize; i ++ )
    {
        const DexMethodId *pDexMethodId = dexGetMethodId(pDexFile, i);
        printf("%04X %04X %08X \r\n", pDexMethodId->classIdx, pDexMethodId->protoIdx, pDexMethodId->nameIdx);
        printf("%s %s \r\n", 
            dexStringByTypeIdx(pDexFile, pDexMethodId->classIdx),
            dexStringById(pDexFile, pDexMethodId->nameIdx));
    }

    printf("\r\n");

    // 根据protoIds来简单还原方法
    for ( u4 i = 0; i < pDexFile->pHeader->methodIdsSize; i ++ )
    {
        const DexMethodId *pDexMethodId = dexGetMethodId(pDexFile, i);
        const DexProtoId  *pDexProtoId  = dexGetProtoId(pDexFile, pDexMethodId->protoIdx);
        
        printf("%s ", dexStringByTypeIdx(pDexFile, pDexProtoId->returnTypeIdx));
        printf("%s\b.", dexStringByTypeIdx(pDexFile, pDexMethodId->classIdx));
        printf("%s", dexStringById(pDexFile, pDexMethodId->nameIdx));
        printf("(");

        // 获得参数列表
        const DexTypeList *pDexTypeList = dexGetProtoParameters(pDexFile, pDexProtoId);

        u4 num = pDexTypeList != NULL ? pDexTypeList->size : 0;
        // 输出参数
        for ( u4 j = 0; j < num; j ++ )
        {
            printf("%s\b, ", dexStringByTypeIdx(pDexFile, pDexTypeList->list[j].typeIdx));
        }

        if ( num == 0 )
        {
            printf(");");
        }
        else 
        {
            printf("\b\b);");
        } 

        printf("\r\n");
    }
}

void PrintClassDef(DexFile *pDexFile)
{
    for ( u4 i =0; i < pDexFile->pHeader->classDefsSize; i ++ )
    {
        const DexClassDef *pDexClassDef = dexGetClassDef(pDexFile, i);
        // 类所属的源文件
        printf("SourceFile : %s\r\n", dexGetSourceFile(pDexFile, pDexClassDef));
        // 类和父类
        // 因为我们的Dex文件没有接口所以这里就没写
        // 具体解析的时候需要根据实际情况而定
        printf("class %s\b externs %s\b { \r\n", 
            dexGetClassDescriptor(pDexFile, pDexClassDef),
            dexGetSuperClassDescriptor(pDexFile, pDexClassDef));

        const u1 *pu1 = dexGetClassData(pDexFile, pDexClassDef);
        DexClassData *pDexClassData = dexReadAndVerifyClassData(&pu1, NULL);

        // 类中的属性
        for ( u4 z = 0; z < pDexClassData->header.instanceFieldsSize; z ++ )
        {
            const DexFieldId *pDexField = dexGetFieldId(pDexFile, pDexClassData->instanceFields[z].fieldIdx);
            printf("%s %s\r\n", 
                dexStringByTypeIdx(pDexFile, pDexField->typeIdx),
                dexStringById(pDexFile, pDexField->nameIdx));
        }

        // 类中的方法
        for ( u4 z = 0; z < pDexClassData->header.directMethodsSize; z ++ )
        {
            const DexMethodId *pDexMethod = dexGetMethodId(pDexFile, pDexClassData->directMethods[z].methodIdx);
            const DexProtoId  *pDexProtoId  = dexGetProtoId(pDexFile, pDexMethod->protoIdx);
            printf("\t%s ", dexStringByTypeIdx(pDexFile, pDexProtoId->returnTypeIdx));
            printf("%s\b.", dexStringByTypeIdx(pDexFile, pDexMethod->classIdx));
            printf("%s", dexStringById(pDexFile, pDexMethod->nameIdx));

            printf("(");

            // 获得参数列表
            const DexTypeList *pDexTypeList = dexGetProtoParameters(pDexFile, pDexProtoId);

            u4 num = pDexTypeList != NULL ? pDexTypeList->size : 0;
            // 输出参数
            for ( u4 k = 0; k < num; k ++ )
            {
                printf("%s\b v%d, ", dexStringByTypeIdx(pDexFile, pDexTypeList->list[k].typeIdx), k);
            }

            if ( num == 0 )
            {
                printf(")");
            }
            else 
            {
                printf("\b\b)");
            }

            printf("{\r\n");

            // 方法中具体的数据
            const DexCode *pDexCode = dexGetCode(pDexFile, (const DexMethod *)&pDexClassData->directMethods[z]);
            printf("\t\tregister:%d \r\n", pDexCode->registersSize);
            printf("\t\tinsnsSize:%d \r\n", pDexCode->insSize);
            printf("\t\tinsSize:%d \r\n", pDexCode->outsSize);

            // 方法的字节码
            printf("\t\t// ByteCode ...\r\n\r\n");
            printf("\t\t//");

            for ( u2 x = 0; x < pDexCode->insnsSize; x ++ )
            {
                printf("%04X ", pDexCode->insns[x]);
            }

            printf("\r\n");

            printf("\t}\r\n\r\n");
        }

        printf("}\r\n");
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE hFile = CreateFile("D:\\TestDex\\HelloWorld.dex", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ACTION_ADDED, NULL);
    HANDLE hMap  = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    LPVOID hView = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);

    DWORD dwSize = GetFileSize(hFile, NULL);

    DexFile *pDexFile = dexFileParse((const u1 *)hView, (size_t)dwSize);

    printf("===============================\r\n");
    PrintDexMapList(pDexFile);
    printf("===============================\r\n");
    PrintStringIds(pDexFile);
    printf("===============================\r\n");
    PrintTypeIds(pDexFile);
    printf("===============================\r\n");
    PrintProtoIds(pDexFile);
    printf("===============================\r\n");
    PrintFieldIds(pDexFile);
    printf("===============================\r\n");
    PrintMethodIds(pDexFile);
    printf("===============================\r\n");

    PrintClassDef(pDexFile);

    dexFileFree(pDexFile);

    UnmapViewOfFile(hView);
    CloseHandle(hMap);
    CloseHandle(hFile);

	return 0;
}

