#pragma once


// CReceiveMainDlg �Ի���

class CReceiveMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CReceiveMainDlg)

public:
	CReceiveMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReceiveMainDlg();

// �Ի�������
	enum { IDD = IDD_RECV_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnDblclkListEmail();
	CString m_strList;
};
