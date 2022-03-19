; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CInjectDllDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "InjectDll.h"

ClassCount=4
Class1=CInjectDllApp
Class2=CInjectDllDlg

ResourceCount=3
Resource2=IDR_MAINFRAME
Resource3=IDD_INJECTDLL_DIALOG

[CLS:CInjectDllApp]
Type=0
HeaderFile=InjectDll.h
ImplementationFile=InjectDll.cpp
Filter=N

[CLS:CInjectDllDlg]
Type=0
HeaderFile=InjectDllDlg.h
ImplementationFile=InjectDllDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_INJECTDLL_DIALOG]
Type=1
Class=CInjectDllDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_DLLFILE,edit,1352732804
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_PROCESSNAME,edit,1350631552
Control5=IDC_BTN_INJECT,button,1342242816
Control6=IDC_BTN_UNINJECT,button,1342242816

