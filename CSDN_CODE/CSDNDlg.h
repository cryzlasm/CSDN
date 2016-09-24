// CSDNDlg.h : header file
//

#if !defined(AFX_CSDNDLG_H__004E620C_36C4_4918_AE80_3FA005E939B6__INCLUDED_)
#define AFX_CSDNDLG_H__004E620C_36C4_4918_AE80_3FA005E939B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCSDNDlg dialog

#include "Assist.h"
#include "GetCodeDlg.h"

class CCSDNDlg : public CDialog
{
// Construction
public:
	CCSDNDlg(CWnd* pParent = NULL);	// standard constructor

    //���԰ٶ�
    void TestBaidu();

// Dialog Data
	//{{AFX_DATA(CCSDNDlg)
	enum { IDD = IDD_CSDN_DIALOG };
	CButton	m_BtnSetCtl;
	CButton	m_BtnSaveCtl;
	CButton	m_BtnRegCtl;
	CButton	m_BtnLogCtl;
	CEdit	m_RegUserCtl;
	CEdit	m_TestCtl;
	CEdit	m_HtmlCtl;
	CStatic	m_InfoCtl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSDNDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL


public:
    //������
    CAssist m_Assist;

    //��֤��DLG
    CGetCodeDlg m_CodeDlg;

    //ʱ���
    LONG m_nTime;
    
    //���Ӷ��������ָ��
    IWinHttpRequestPtr m_pHttpReq;

    //ע���߳�
    static DWORD RegThread(LPVOID lParam);
    BOOL m_bIsExitThread;   //�߳��˳���־��
    CWinThread * m_Thread;  //�߳�

    //�û���
    CString m_strUser;

    //����û�����Ϊʮλ
    BOOL RandUser();

    //��֤����֤����
    BOOL GetVerifyCode(CString& strRetCode);

    //�Ƿ���Դ���
    BOOL m_bIsProxyDebug;
    BOOL ProxyHost();


    //����Btn ������״̬
    BOOL ShowButton(DWORD dwNum);

    //ת���ַ������û���
    BOOL ConvertUserFromMail(CString strSrcUser);

    //���ڼ����û�
    void OnActiveReg(CString strUser);

    //�ͷ�ԭ��Request��ʵ���µ�Request
    BOOL GetNewRequest();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCSDNDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBtnReg();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnLogin();
	afx_msg void OnBtnSet();
	afx_msg void OnBtnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    enum _BtnName
    {
        BTN_REG,    //ע��
        BTN_SET,    //����
        BTN_SAVE,   //����
        BTN_LOGIN   //��¼
    };
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSDNDLG_H__004E620C_36C4_4918_AE80_3FA005E939B6__INCLUDED_)
