; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSimInputDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SimInput.h"

ClassCount=4
Class1=CSimInputApp
Class2=CSimInputDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SIMINPUT_DIALOG

[CLS:CSimInputApp]
Type=0
HeaderFile=SimInput.h
ImplementationFile=SimInput.cpp
Filter=N

[CLS:CSimInputDlg]
Type=0
HeaderFile=SimInputDlg.h
ImplementationFile=SimInputDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=SimInputDlg.h
ImplementationFile=SimInputDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SIMINPUT_DIALOG]
Type=1
Class=CSimInputDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_CAPTION,edit,1350631552
Control3=IDC_BTN_SIMKEYBD,button,1342242816
Control4=IDC_BTN_SIMMOUSE,button,1342242816

