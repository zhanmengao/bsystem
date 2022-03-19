; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMultiPortScanDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MultiPortScan.h"

ClassCount=2
Class1=CMultiPortScanApp
Class2=CMultiPortScanDlg

ResourceCount=3
Resource2=IDR_MAINFRAME
Resource3=IDD_MULTIPORTSCAN_DIALOG

[CLS:CMultiPortScanApp]
Type=0
HeaderFile=MultiPortScan.h
ImplementationFile=MultiPortScan.cpp
Filter=N

[CLS:CMultiPortScanDlg]
Type=0
HeaderFile=MultiPortScanDlg.h
ImplementationFile=MultiPortScanDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_IPTREE



[DLG:IDD_MULTIPORTSCAN_DIALOG]
Type=1
Class=CMultiPortScanDlg
ControlCount=13
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_IPSTART,SysIPAddress32,1342242816
Control6=IDC_IPEND,SysIPAddress32,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_PORTSTART,edit,1350631552
Control10=IDC_PORTEND,edit,1350631552
Control11=IDC_IPTREE,SysTreeView32,1350631431
Control12=IDC_BTNSCAN,button,1342242816
Control13=IDC_BTNCANCEL,button,1342242816

