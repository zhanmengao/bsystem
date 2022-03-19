; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMsgTestDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MsgTest.h"

ClassCount=4
Class1=CMsgTestApp
Class2=CMsgTestDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MSGTEST_DIALOG

[CLS:CMsgTestApp]
Type=0
HeaderFile=MsgTest.h
ImplementationFile=MsgTest.cpp
Filter=N

[CLS:CMsgTestDlg]
Type=0
HeaderFile=MsgTestDlg.h
ImplementationFile=MsgTestDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=MsgTestDlg.h
ImplementationFile=MsgTestDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MSGTEST_DIALOG]
Type=1
Class=CMsgTestDlg
ControlCount=4
Control1=IDC_CLOSE,button,1342242816
Control2=IDC_EDIT_WND,button,1342242816
Control3=IDC_GET_WND,button,1342242816
Control4=IDC_EXEC,button,1342242816

