// ReceiveMainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyMailClient.h"
#include "MyMailClientDlg.h"
#include "ReceiveMainDlg.h"
#include "afxdialogex.h"


// CReceiveMainDlg �Ի���

IMPLEMENT_DYNAMIC(CReceiveMainDlg, CDialog)

CReceiveMainDlg::CReceiveMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReceiveMainDlg::IDD, pParent)
{

}

CReceiveMainDlg::~CReceiveMainDlg()
{
}

void CReceiveMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReceiveMainDlg, CDialog)
END_MESSAGE_MAP()


// CReceiveMainDlg ��Ϣ�������


BOOL CReceiveMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}
