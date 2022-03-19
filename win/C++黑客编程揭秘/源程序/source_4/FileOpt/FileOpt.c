#include <ntddk.h>
#define FILENAME L"\\??\\c:\\a.txt"
#define BUFFERLEN 10

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
}

VOID CreateFileTest()
{
    NTSTATUS status = STATUS_SUCCESS;
    
    OBJECT_ATTRIBUTES ObjAttribute;
    IO_STATUS_BLOCK ioStatusBlock;
    
    UNICODE_STRING uniFile;
    
    HANDLE hFile = NULL;
    
    RtlInitUnicodeString(&uniFile, FILENAME);
    
    // ��ʼ��һ����������
    InitializeObjectAttributes(&ObjAttribute,
                &uniFile,
                OBJ_CASE_INSENSITIVE, 
                NULL,
                NULL);
                
    // �����ļ�
    status = ZwCreateFile(&hFile,
                GENERIC_READ | GENERIC_WRITE,
                &ObjAttribute,
                &ioStatusBlock,
                0,
                FILE_ATTRIBUTE_NORMAL,
                FILE_SHARE_READ,
                FILE_OPEN_IF,
                FILE_SYNCHRONOUS_IO_NONALERT,
                NULL,
                0);
    
    if ( NT_SUCCESS(status) )
    {
        KdPrint(("File Create ok ! \r\n"));
    }
    else
    {
        KdPrint(("File Create faild ! \r\n"));
    }
    

    ZwClose(hFile);
}

VOID OpenFileTest()
{
    IO_STATUS_BLOCK ioStatusBlock;
    OBJECT_ATTRIBUTES ObjAttribute;
    HANDLE hFile;
    UNICODE_STRING uniFileName;
    NTSTATUS status;
    
    FILE_STANDARD_INFORMATION fsi;
    FILE_POSITION_INFORMATION fpi;
    PVOID Buffer = NULL;
    
    RtlInitUnicodeString(&uniFileName, FILENAME);
    
    // ��ʼ��һ����������
    InitializeObjectAttributes(&ObjAttribute, 
                        &uniFileName,
                        OBJ_CASE_INSENSITIVE,
                        NULL,
                        NULL);
    
    // ���ļ�
    status = ZwOpenFile(&hFile,
                    GENERIC_READ | GENERIC_WRITE,
                    &ObjAttribute,
                    &ioStatusBlock,
                    FILE_SHARE_READ,
                    FILE_SYNCHRONOUS_IO_NONALERT);
    
    if ( NT_SUCCESS(status) )
    {
        KdPrint(("OpenFile Successfully ! \r\n"));
    }
    else
    {
        KdPrint(("OpenFile UnSuccessfully ! \r\n"));
        return ;
    }
    
    // ��ȡ�ļ�����
    status = ZwQueryInformationFile(hFile,
                        &ioStatusBlock,
                        &fsi,
                        sizeof(FILE_STANDARD_INFORMATION),
                        FileStandardInformation);
    
    // ����´������ļ��Ƿ���Ŀ¼
    KdPrint(("Is Directory %d \r\n", fsi.Directory));
    
    // ����ռ�
    Buffer = ExAllocatePool(PagedPool, BUFFERLEN);
    
    if ( Buffer == NULL )
    {
        ZwClose(hFile);
        return ;
    }
    
    // ���ֵΪ
    RtlFillMemory(Buffer, BUFFERLEN, 0x61);
    
    // д�ļ�
    status = ZwWriteFile(hFile,
                    NULL,
                    NULL,
                    NULL,
                    &ioStatusBlock,
                    Buffer,
                    BUFFERLEN,
                    0,
                    NULL);

    if ( NT_SUCCESS(status) )
    {
        KdPrint(("ZwWriteFile Successfully ! \r\n"));
    }
    
    // �ͷ�����Ŀռ�
    ExFreePool(Buffer);
    
    // ��ȡ�ļ�����
    status = ZwQueryInformationFile(hFile,
                        &ioStatusBlock,
                        &fsi,
                        sizeof(FILE_STANDARD_INFORMATION),
                        FileStandardInformation);
                        
    // ����´������ļ��Ƿ���Ŀ¼
    KdPrint(("FileSize =  %d \r\n", (LONG)fsi.EndOfFile.QuadPart));

    Buffer = ExAllocatePool(PagedPool, (fsi.EndOfFile.QuadPart * 2));
    if ( Buffer == NULL )
    {
        KdPrint(("ExAllocatePool UnSuccessfully ! \r\n"));
        ZwClose(hFile);
        return ;
    }    
    
    
    fpi.CurrentByteOffset.QuadPart = 0;
    // �����ļ�ָ��
    status = ZwSetInformationFile(hFile, 
                            &ioStatusBlock, &fpi,
                            sizeof(FILE_POSITION_INFORMATION),
                            FilePositionInformation);
    
    // ��ȡ�ļ�����
    status = ZwReadFile(hFile, NULL, NULL, NULL,
                        &ioStatusBlock, Buffer,
                        (LONG)fsi.EndOfFile.QuadPart,
                        NULL, NULL);
                        
    if ( NT_SUCCESS(status) )
    {
        KdPrint(("ZwReadFile Successfully ! \r\n"));
    }
    KdPrint(("%s", Buffer));
    
    ExFreePool(Buffer);
    ZwClose(hFile);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, 
                     PUNICODE_STRING pRegistryPath)
{
    pDriverObject->DriverUnload = DriverUnload;
    
    CreateFileTest();
    
    OpenFileTest();
    
    return STATUS_SUCCESS;
}