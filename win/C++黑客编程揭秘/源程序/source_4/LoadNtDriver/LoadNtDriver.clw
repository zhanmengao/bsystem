; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLoadNtDriverDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "LoadNtDriver.h"

ClassCount=4
Class1=CLoadNtDriverApp
Class2=CLoadNtDriverDlg

ResourceCount=3
Resource2=IDR_MAINFRAME
Resource3=IDD_LOADNTDRIVER_DIALOG

[CLS:CLoadNtDriverApp]
Type=0
HeaderFile=LoadNtDriver.h
ImplementationFile=LoadNtDriver.cpp
Filter=N

[CLS:CLoadNtDriverDlg]
Type=0
HeaderFile=LoadNtDriverDlg.h
ImplementationFile=LoadNtDriverDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_LOADNTDRIVER_DIALOG]
Type=1
Class=CLoadNtDriverDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_DRIVER_PATH,edit,1352732676
Control3=IDC_LOAD,button,1342242816
Control4=IDC_UNLOAD,button,1342242816

