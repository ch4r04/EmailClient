// MainMenuDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyMailClient.h"
#include "MyMailClientDlg.h"
#include "MainMenuDlg.h"
#include "WriteMailDlg.h"
#include "ReceiveMainDlg.h"
#include "MassEmailsDlg.h"
#include "afxdialogex.h"



// CMainMenuDlg �Ի���

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


// CMainMenuDlg ��Ϣ�������


void CMainMenuDlg::OnBnClickedWriteButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CWriteMailDlg writedlg;
	writedlg.DoModal();
	
}


void CMainMenuDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

//	CDialog::OnOK();
}



BOOL CMainMenuDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
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

		MessageBox(_T("��ȡ�ʼ��б�ʧ��"));
		return;
	}

	m_pop3.SetMailSum(num);		//���ý����ʼ�������
	CStringA strShow;
	strShow.Format(_T("һ�����յ���%d���ʼ�"), num);
	MessageBox(strShow);
	CReceiveMainDlg crecvdlg;
	crecvdlg.DoModal();
}


void CMainMenuDlg::OnBnClickedSendAllButton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMassEmailsDlg cMassDlg;
	cMassDlg.DoModal();
}
