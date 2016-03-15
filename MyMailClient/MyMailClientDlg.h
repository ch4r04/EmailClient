
// MyMailClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "pop3.h"
#include "smtp.h"

extern CSmtp m_smtp;		//����ȫ�ֱ��� smtp
extern CPop3 m_pop3;		//����ȫ�ֱ��� pop
// CMyMailClientDlg �Ի���
class CMyMailClientDlg : public CDialogEx
{
// ����
public:
	CMyMailClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

	UINT ThreadFun(LPVOID pParam);
// �Ի�������
	enum { IDD = IDD_MYMAILCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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


