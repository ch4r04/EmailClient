#pragma once


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
};
