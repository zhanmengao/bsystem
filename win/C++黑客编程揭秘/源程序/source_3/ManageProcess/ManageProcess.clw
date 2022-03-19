; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CManageProcessDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ManageProcess.h"

ClassCount=2
Class1=CManageProcessApp
Class2=CManageProcessDlg

ResourceCount=3
Resource2=IDR_MAINFRAME
Resource3=IDD_MANAGEPROCESS_DIALOG

[CLS:CManageProcessApp]
Type=0
HeaderFile=ManageProcess.h
ImplementationFile=ManageProcess.cpp
Filter=N

[CLS:CManageProcessDlg]
Type=0
HeaderFile=ManageProcessDlg.h
ImplementationFile=ManageProcessDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST_PROCESS



[DLG:IDD_MANAGEPROCESS_DIALOG]
Type=1
Class=CManageProcessDlg
ControlCount=8
Control1=IDC_LIST_PROCESS,SysListView32,1350631425
Control2=IDC_LIST_MODULE,SysListView32,1350631425
Control3=IDC_BTN_CREATE,button,1342242816
Control4=IDC_BTN_TERMINATE,button,1342242816
Control5=IDC_BTN_STOP,button,1342242816
Control6=IDC_BTN_RESUME,button,1342242816
Control7=IDC_BTN_SHOWMODULE,button,1342242816
Control8=IDC_BTN_EXIT,button,1342242816

