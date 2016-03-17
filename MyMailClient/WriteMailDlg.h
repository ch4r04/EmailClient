#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CWriteMailDlg 对话框

class CWriteMailDlg : public CDialog
{
	DECLARE_DYNAMIC(CWriteMailDlg)

public:
	CWriteMailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWriteMailDlg();

// 对话框数据
	enum { IDD = IDD_WRITE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSendMailButton();
	// 收件人字符串
	CString m_editTarget;
	// 发送邮件的主题
	CString m_editSubject;
	// 发送邮件的正文
	CString m_editContent;
	CProgressCtrl m_pro;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//public:
	
	//static UINT function(LPVOID pParam);
	//static UINT function1(LPVOID pParam);

};
UINT function(LPVOID pParam);
UINT function1(LPVOID pParam);