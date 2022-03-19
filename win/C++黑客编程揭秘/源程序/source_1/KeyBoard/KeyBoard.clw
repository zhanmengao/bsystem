; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CKeyBoardDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "KeyBoard.h"

ClassCount=3
Class1=CKeyBoardApp
Class2=CKeyBoardDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_KEYBOARD_DIALOG

[CLS:CKeyBoardApp]
Type=0
HeaderFile=KeyBoard.h
ImplementationFile=KeyBoard.cpp
Filter=N

[CLS:CKeyBoardDlg]
Type=0
HeaderFile=KeyBoardDlg.h
ImplementationFile=KeyBoardDlg.cpp
Filter=D
LastObject=IDC_BTN_START
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=KeyBoardDlg.h
ImplementationFile=KeyBoardDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_KEYBOARD_DIALOG]
Type=1
Class=CKeyBoardDlg
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_CAPTION,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_INTERVAL,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_BTN_START,button,1342242816

