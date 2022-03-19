; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNoDllInjectDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "NoDllInject.h"

ClassCount=4
Class1=CNoDllInjectApp
Class2=CNoDllInjectDlg

ResourceCount=3
Resource2=IDR_MAINFRAME
Resource3=IDD_NODLLINJECT_DIALOG

[CLS:CNoDllInjectApp]
Type=0
HeaderFile=NoDllInject.h
ImplementationFile=NoDllInject.cpp
Filter=N

[CLS:CNoDllInjectDlg]
Type=0
HeaderFile=NoDllInjectDlg.h
ImplementationFile=NoDllInjectDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_NODLLINJECT_DIALOG]
Type=1
Class=CNoDllInjectDlg
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_PROCESSNAME,edit,1350631552
Control3=IDC_BTN_INJECT,button,1342242816

