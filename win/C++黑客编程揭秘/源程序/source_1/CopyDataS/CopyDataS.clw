; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCopyDataSDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CopyDataS.h"

ClassCount=3
Class1=CCopyDataSApp
Class2=CCopyDataSDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_COPYDATAS_DIALOG

[CLS:CCopyDataSApp]
Type=0
HeaderFile=CopyDataS.h
ImplementationFile=CopyDataS.cpp
Filter=N

[CLS:CCopyDataSDlg]
Type=0
HeaderFile=CopyDataSDlg.h
ImplementationFile=CopyDataSDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST_REVCDATA

[CLS:CAboutDlg]
Type=0
HeaderFile=CopyDataSDlg.h
ImplementationFile=CopyDataSDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_COPYDATAS_DIALOG]
Type=1
Class=CCopyDataSDlg
ControlCount=3
Control1=IDC_STATIC,button,1342177287
Control2=IDC_LIST_REVCDATA,listbox,1352728835
Control3=IDC_BTN_DELALL,button,1342242816

