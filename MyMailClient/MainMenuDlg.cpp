// MainMenuDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyMailClient.h"
#include "MyMailClientDlg.h"
#include "MainMenuDlg.h"
#include "WriteMailDlg.h"
#include "ReceiveMainDlg.h"
#include "MassEmailsDlg.h"
#include "afxdialogex.h"



// CMainMenuDlg 对话框

IMPLEMENT_DYNAMIC(CMainMenuDlg, CDialog)

CMainMenuDlg::CMainMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainMenuDlg::IDD, pParent)
{

}

CMainMenuDlg::~CMainMenuDlg()
{
}

void CMainMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainMenuDlg, CDialog)
	ON_BN_CLICKED(IDC_WRITE_BUTTON, &CMainMenuDlg::OnBnClickedWriteButton)
	ON_BN_CLICKED(IDC_RECIVE_BUTTON, &CMainMenuDlg::OnBnClickedReciveButton)
	ON_BN_CLICKED(IDC_SEND_ALL_BUTTON, &CMainMenuDlg::OnBnClickedSendAllButton)
END_MESSAGE_MAP()


// CMainMenuDlg 消息处理程序


void CMainMenuDlg::OnBnClickedWriteButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CWriteMailDlg writedlg;
	writedlg.DoModal();
	
}


void CMainMenuDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

//	CDialog::OnOK();
}



BOOL CMainMenuDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;

	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CMainMenuDlg::OnBnClickedReciveButton()
{
	int num;
	if (m_pop3.List(num) == FALSE)
	{

		MessageBox(_T("拉取邮件列表失败"));
		return;
	}

	m_pop3.SetMailSum(num);		//设置接收邮件的数量
	CStringA strShow;
	strShow.Format(_T("一共接收到了%d封邮件"), num);
	MessageBox(strShow);
	CReceiveMainDlg crecvdlg;
	crecvdlg.DoModal();
}


void CMainMenuDlg::OnBnClickedSendAllButton()
{
	// TODO:  在此添加控件通知处理程序代码
	CMassEmailsDlg cMassDlg;
	cMassDlg.DoModal();
}
