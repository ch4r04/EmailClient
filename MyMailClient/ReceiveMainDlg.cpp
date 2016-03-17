// ReceiveMainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyMailClient.h"
#include "MyMailClientDlg.h"
#include "ReceiveMainDlg.h"
#include "afxdialogex.h"


// CReceiveMainDlg 对话框

IMPLEMENT_DYNAMIC(CReceiveMainDlg, CDialog)

CReceiveMainDlg::CReceiveMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReceiveMainDlg::IDD, pParent)
	, m_strList(_T(""))
{

}

CReceiveMainDlg::~CReceiveMainDlg()
{
}

void CReceiveMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_LBString(pDX, IDC_LIST_EMAIL, m_strList);
}


BEGIN_MESSAGE_MAP(CReceiveMainDlg, CDialog)
	ON_LBN_DBLCLK(IDC_LIST_EMAIL, &CReceiveMainDlg::OnLbnDblclkListEmail)
END_MESSAGE_MAP()


// CReceiveMainDlg 消息处理程序


BOOL CReceiveMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int num = m_pop3.GetAllSum();
	CStringA str;
	((CListBox *)GetDlgItem(IDC_LIST_EMAIL))->ResetContent();
	for (int i = 1; i <= num; i++)
	{
		m_pop3.FetchEx(i);
		str.Empty();
		str.Format(_T("%d"), i);
		((CListBox *)GetDlgItem(IDC_LIST_EMAIL))->AddString(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CReceiveMainDlg::OnLbnDblclkListEmail()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//MessageBox(m_strList);
	CString fileName ;
//	CString filePath = "/data/";
	fileName = ".\\data\\" + m_strList;
	fileName += ".eml";

	//CStringW stra(fileName.GetBuffer(0));
	LPCSTR stra = fileName.GetBuffer();

	ShellExecute(NULL, _T("open"), stra, NULL, NULL, SW_SHOWNORMAL);

	UpdateData(FALSE);
}
