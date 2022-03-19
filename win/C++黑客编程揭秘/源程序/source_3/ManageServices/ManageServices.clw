; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CManageServicesDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ManageServices.h"

ClassCount=2
Class1=CManageServicesApp
Class2=CManageServicesDlg

ResourceCount=3
Resource2=IDR_MAINFRAME
Resource3=IDD_MANAGESERVICES_DIALOG

[CLS:CManageServicesApp]
Type=0
HeaderFile=ManageServices.h
ImplementationFile=ManageServices.cpp
Filter=N

[CLS:CManageServicesDlg]
Type=0
HeaderFile=ManageServicesDlg.h
ImplementationFile=ManageServicesDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST_SERVICE



[DLG:IDD_MANAGESERVICES_DIALOG]
Type=1
Class=CManageServicesDlg
ControlCount=6
Control1=IDC_LIST_SERVICE,SysListView32,1350631429
Control2=IDC_RADIO_WIN32,button,1342177289
Control3=IDC_RADIO_DRIVER,button,1342177289
Control4=IDC_BTN_START,button,1342242816
Control5=IDC_BTN_STOP,button,1342242816
Control6=IDC_BTN_EXIT,button,1342242816

