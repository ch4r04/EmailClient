#pragma once
#include "afxwin.h"


// CWriteMailDlg �Ի���

class CWriteMailDlg : public CDialog
{
	DECLARE_DYNAMIC(CWriteMailDlg)

public:
	CWriteMailDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWriteMailDlg();

// �Ի�������
	enum { IDD = IDD_WRITE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSendMailButton();
	// �ռ����ַ���
	CString m_editTarget;
	// �����ʼ�������
	CString m_editSubject;
	// �����ʼ�������
	CString m_editContent;
};
