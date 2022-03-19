; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEasyCrackMeDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "EasyCrackMe.h"

ClassCount=4
Class1=CEasyCrackMeApp
Class2=CEasyCrackMeDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_EASYCRACKME_DIALOG

[CLS:CEasyCrackMeApp]
Type=0
HeaderFile=EasyCrackMe.h
ImplementationFile=EasyCrackMe.cpp
Filter=N

[CLS:CEasyCrackMeDlg]
Type=0
HeaderFile=EasyCrackMeDlg.h
ImplementationFile=EasyCrackMeDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=EasyCrackMeDlg.h
ImplementationFile=EasyCrackMeDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_EASYCRACKME_DIALOG]
Type=1
Class=CEasyCrackMeDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_USER,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_PASSWORD,edit,1350631552
Control5=IDC_BTN_REG,button,1342242816
Control6=IDC_BTN_END,button,1342242816

