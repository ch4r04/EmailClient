#pragma once
#include "afxcmn.h"


// CMassEmailsDlg 对话框

class CMassEmailsDlg : public CDialog
{
	DECLARE_DYNAMIC(CMassEmailsDlg)

public:
	CMassEmailsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMassEmailsDlg();

// 对话框数据
	enum { IDD = IDD_MASS_MAIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_cszRecvs;
	CString m_editSubject;
	CString m_editContent;
	afx_msg void OnBnClickedSendButton();
	afx_msg void OnBnClickedCancelButton();
	CProgressCtrl m_pro;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
UINT function2(LPVOID pParam);
UINT function3(LPVOID pParam);