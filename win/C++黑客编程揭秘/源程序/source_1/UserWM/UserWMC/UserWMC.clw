; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUserWMCDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "UserWMC.h"

ClassCount=4
Class1=CUserWMCApp
Class2=CUserWMCDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_USERWMC_DIALOG

[CLS:CUserWMCApp]
Type=0
HeaderFile=UserWMC.h
ImplementationFile=UserWMC.cpp
Filter=N

[CLS:CUserWMCDlg]
Type=0
HeaderFile=UserWMCDlg.h
ImplementationFile=UserWMCDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=UserWMCDlg.h
ImplementationFile=UserWMCDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_USERWMC_DIALOG]
Type=1
Class=CUserWMCDlg
ControlCount=4
Control1=IDC_STATIC,button,1342177287
Control2=IDC_EDIT_SENDDATA,edit,1350631552
Control3=IDC_BTN_SEND,button,1342242816
Control4=IDC_EDIT_SENDDATA2,edit,1350631552

