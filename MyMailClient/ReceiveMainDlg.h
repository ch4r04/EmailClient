#pragma once


// CReceiveMainDlg 对话框

class CReceiveMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CReceiveMainDlg)

public:
	CReceiveMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReceiveMainDlg();

// 对话框数据
	enum { IDD = IDD_RECV_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnDblclkListEmail();
	CString m_strList;
};
