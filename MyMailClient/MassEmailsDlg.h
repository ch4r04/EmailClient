#pragma once


// CMassEmailsDlg �Ի���

class CMassEmailsDlg : public CDialog
{
	DECLARE_DYNAMIC(CMassEmailsDlg)

public:
	CMassEmailsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMassEmailsDlg();

// �Ի�������
	enum { IDD = IDD_MASS_MAIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_cszRecvs;
	CString m_editSubject;
	CString m_editContent;
	afx_msg void OnBnClickedSendButton();
};
