#pragma once


// CMainMenuDlg �Ի���

class CMainMenuDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainMenuDlg)

public:
	CMainMenuDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainMenuDlg();

// �Ի�������
	enum { IDD = IDD_MAIN_MENU_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWriteButton();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedReciveButton();
	afx_msg void OnBnClickedSendAllButton();
};
