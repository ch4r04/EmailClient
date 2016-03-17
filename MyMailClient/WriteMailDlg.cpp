// WriteMailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyMailClient.h"
#include "MyMailClientDlg.h"
#include "WriteMailDlg.h" 
#include "afxdialogex.h"
#include "smtp.h"


// CWriteMailDlg �Ի���

IMPLEMENT_DYNAMIC(CWriteMailDlg, CDialog)
int process = 0;
CWriteMailDlg::CWriteMailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWriteMailDlg::IDD, pParent)
	, m_editTarget(_T(""))
	, m_editSubject(_T(""))
	, m_editContent(_T(""))
{

}

CWriteMailDlg::~CWriteMailDlg()
{
}

void CWriteMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TARGET_EDIT, m_editTarget);
	DDX_Text(pDX, IDC_SUBJECT_EDIT, m_editSubject);
	DDX_Text(pDX, IDC_CONTENT_EDIT, m_editContent);
	DDX_Control(pDX, IDC_PROGRESS1, m_pro);
}


BEGIN_MESSAGE_MAP(CWriteMailDlg, CDialog)
	ON_BN_CLICKED(IDC_SEND_MAIL_BUTTON, &CWriteMailDlg::OnBnClickedSendMailButton)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()




void CWriteMailDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

//	CDialog::OnOK();
}


BOOL CWriteMailDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CWriteMailDlg::OnBnClickedSendMailButton()
{
	INT_PTR nRes;

	// ��ʾ��Ϣ�Ի���   
	nRes = MessageBox(_T("��ȷ��Ҫ���з��Ͳ�����"), _T("���ʼ�"), MB_OKCANCEL | MB_ICONQUESTION);
	// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
	if (IDCANCEL == nRes)
		return;
	// TODO:  �ڴ���ӿؼ�֪ͨ��������		
	/*��ȡ��¼���û���*/
	UpdateData(true);
	char *szUserName = m_pop3.GetUser();	
	string szUser(szUserName);

	/*��ȡ��¼������*/
	char *szUserPwd = m_pop3.GetPwd();				//��ȡ����
	string szPwd(szUserPwd);

	/*��ȫ���*/
	if (m_editTarget.GetLength() == 0)
	{
		MessageBox(_T("�������ռ��˵�ַ"));
		return;
	}
	int pos = m_editTarget.Find(_T('@'));
	if (-1 == pos || pos < 2)
	{
		MessageBox(_T("�����������淶�ĵ��ʼ���ַ��"));
		return;
	}
	if (m_editSubject.GetLength() == 0)
	{
		MessageBox(_T("����������"));
		return;
	}
	if (m_editContent.GetLength() == 0)
	{
		MessageBox(_T("����������"));
		return;
	}
	/*��ȫ���*/
	/*��ȡ�����˵�ַ*/
	CStringA cszTemp(m_editTarget.GetBuffer());		//��ʱ���� ֻ�ǻ�ȡ��ַ
	string szTarget = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	/*��ȡ�ʼ�����*/
	cszTemp = m_editSubject.GetBuffer(0);
	string szSubject = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	/*��ȡ�ʼ�����*/
	cszTemp = m_editContent.GetBuffer(0);
	string szContent = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	m_smtp.SetUserName(szUser);
	m_smtp.SetPass(szPwd);
	m_smtp.SetPortE(25);
	m_smtp.SetEmailTitle(szSubject);
	m_smtp.SetContent(szContent);
	m_smtp.SetTargetEmail(szTarget);
	AfxBeginThread(function, this);
	AfxBeginThread(function1, this);
	//MessageBox(_T("ִ�����1"));
	/*
	int err;
	if ((err = m_smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			MessageBox(_T("�������粻��ͨ������ʧ��"));
		if (err == 2)
			MessageBox(_T("�û�������"));
		if (err == 3)
			MessageBox(_T("�������"));
	}
	else
	{
		MessageBox("���ͳɹ�");
	}
	*/

	UpdateData(false);
	

	

}


void CWriteMailDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnTimer(nIDEvent);
	m_pro.SetPos(process);
	if (process == -1)
	{
		KillTimer(1);
		MessageBox(_T("ִ�����"));
		return;
	}
}


void CWriteMailDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO:  �ڴ˴������Ϣ����������
	process = 0;
	SetTimer(1, 1000, NULL);
	m_pro.SetRange(1, 100);
}

UINT function(LPVOID pParam)
{
	for (int i = 0; i < 100; i++){
		process++;
		Sleep(25);
	}
	process = -1;
	return 0;
}
//���������function�����������ƽ������仯�ĺ�����
UINT function1(LPVOID pParam)
{
	//=============
	//�ڴ������Ҫ����ʱ��Ĵ���
	int err;
		err = m_smtp.SendEmail_Ex();
	return 0;
}
