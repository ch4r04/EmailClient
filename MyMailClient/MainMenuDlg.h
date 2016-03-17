#pragma once


// CMainMenuDlg 对话框

class CMainMenuDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainMenuDlg)

public:
	CMainMenuDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainMenuDlg();

// 对话框数据
	enum { IDD = IDD_MAIN_MENU_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWriteButton();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedReciveButton();
	afx_msg void OnBnClickedSendAllButton();
};
