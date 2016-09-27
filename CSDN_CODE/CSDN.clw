; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCSDNDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CSDN.h"

ClassCount=4
Class1=CCSDNApp
Class2=CCSDNDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_CSDN_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CGetCodeDlg
Resource4=IDD_DLG_GET_CODE

[CLS:CCSDNApp]
Type=0
HeaderFile=CSDN.h
ImplementationFile=CSDN.cpp
Filter=N

[CLS:CCSDNDlg]
Type=0
HeaderFile=CSDNDlg.h
ImplementationFile=CSDNDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_STATIC_INFO

[CLS:CAboutDlg]
Type=0
HeaderFile=CSDNDlg.h
ImplementationFile=CSDNDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CSDN_DIALOG]
Type=1
Class=CCSDNDlg
ControlCount=11
Control1=IDC_BTN_SET,button,1342242816
Control2=IDC_BTN_SAVE,button,1342242816
Control3=IDC_BTN_REG,button,1342242817
Control4=IDC_BTN_LOGIN,button,1342242816
Control5=IDC_STATIC_INFO,static,1342308865
Control6=IDC_BTN_TEST,button,1342242816
Control7=IDC_EDIT_HTML,edit,1352728644
Control8=IDC_EDIT_TEST,edit,1350631552
Control9=IDC_EDIT_REG_USER,edit,1350631552
Control10=IDC_STATIC,static,1342308865
Control11=IDC_BTN_ACTIVE,button,1342242816

[DLG:IDD_DLG_GET_CODE]
Type=1
Class=CGetCodeDlg
ControlCount=4
Control1=IDC_EDIT_GET_CODE,edit,1350631553
Control2=IDOK,button,1342242817
Control3=IDC_STATIC,static,1342308865
Control4=IDC_STATIC_GET_CODE,static,1342177294

[CLS:CGetCodeDlg]
Type=0
HeaderFile=GetCodeDlg.h
ImplementationFile=GetCodeDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CGetCodeDlg

