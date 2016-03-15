
// MyMailClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "pop3.h"
#include "smtp.h"

extern CSmtp m_smtp;		//设置全局变量 smtp
extern CPop3 m_pop3;		//设置全局变量 pop
// CMyMailClientDlg 对话框
class CMyMailClientDlg : public CDialogEx
{
// 构造
public:
	CMyMailClientDlg(CWnd* pParent = NULL);	// 标准构造函数

	UINT ThreadFun(LPVOID pParam);
// 对话框数据
	enum { IDD = IDD_MYMAILCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboServer;
	afx_msg void OnCbnSelchangeServerCombo();
	afx_msg void OnBnClickedSelServerButton();
	CEdit m_editport;
public:


	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_editsmtpaddr;
	CString m_editpopaddr;
};


