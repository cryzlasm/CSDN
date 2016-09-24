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

    //测试百度
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
    //助理类
    CAssist m_Assist;

    //验证码DLG
    CGetCodeDlg m_CodeDlg;

    //时间戳
    LONG m_nTime;
    
    //连接对象的智能指针
    IWinHttpRequestPtr m_pHttpReq;

    //注册线程
    static DWORD RegThread(LPVOID lParam);
    BOOL m_bIsExitThread;   //线程退出标志码
    CWinThread * m_Thread;  //线程

    //用户名
    CString m_strUser;

    //随机用户名，为十位
    BOOL RandUser();

    //验证码验证流程
    BOOL GetVerifyCode(CString& strRetCode);

    //是否调试代理
    BOOL m_bIsProxyDebug;
    BOOL ProxyHost();


    //设置Btn 的隐藏状态
    BOOL ShowButton(DWORD dwNum);

    //转换字符串到用户名
    BOOL ConvertUserFromMail(CString strSrcUser);

    //用于激活用户
    void OnActiveReg(CString strUser);

    //释放原有Request，实例新的Request
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
        BTN_REG,    //注册
        BTN_SET,    //设置
        BTN_SAVE,   //保存
        BTN_LOGIN   //登录
    };
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSDNDLG_H__004E620C_36C4_4918_AE80_3FA005E939B6__INCLUDED_)
