// MassEmailsDlg.cpp : ʵ���ļ�
//ʵ��Ⱥ������

#include "stdafx.h"
#include "MyMailClient.h"
#include "MyMailClientDlg.h"
#include "MassEmailsDlg.h"
#include "afxdialogex.h"


// CMassEmailsDlg �Ի���

IMPLEMENT_DYNAMIC(CMassEmailsDlg, CDialog)

CMassEmailsDlg::CMassEmailsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMassEmailsDlg::IDD, pParent)
	, m_cszRecvs(_T(""))
	, m_editSubject(_T(""))
	, m_editContent(_T(""))
{

}

CMassEmailsDlg::~CMassEmailsDlg()
{
}

void CMassEmailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RECEVS_EDIT, m_cszRecvs);
	DDX_Text(pDX, IDC_SUBJECT_EDIT, m_editSubject);
	DDX_Text(pDX, IDC_CONTENT_EDIT, m_editContent);
}


BEGIN_MESSAGE_MAP(CMassEmailsDlg, CDialog)
	ON_BN_CLICKED(IDC_SEND_BUTTON, &CMassEmailsDlg::OnBnClickedSendButton)
END_MESSAGE_MAP()


// CMassEmailsDlg ��Ϣ�������


void CMassEmailsDlg::OnBnClickedSendButton()
{
	UpdateData(true);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int n = m_cszRecvs.Replace(";", ";");
	if (m_cszRecvs.Right(1).Compare(_T(";")) != 0)
	{
		//������һ���ַ����Ǽ���;�򷵻�
		MessageBox(_T("ĩβ���Ǽ�;��"));
		return;
	}
	CString mArray[20];
	for (int i = 0; i < n; i++)
	{
		AfxExtractSubString(mArray[i], m_cszRecvs, i, ';');

	}
	/*��ȫ���*/
	if (m_cszRecvs.GetLength() == 0)
	{
		MessageBox(_T("�������ռ���"));

	}
	for (int i = 0; i < n; i++)
	{
		int poc = mArray[i].Find(_T("@"));	//�����ռ�������
		if (-1 == poc || poc < 2)
		{
			MessageBox(_T("�����������淶�ĵ��ʼ���ַ��"));
			return;
		}
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

	/*��ȡ��½���˺�*/
	char *szUserName = m_pop3.GetUser();
	string szUser(szUserName);

	/*��ȡ��¼������*/
	char *szUserPwd = m_pop3.GetPwd();				//��ȡ����
	string szPwd(szUserPwd);

	/*��ȡ����*/
	CStringA cszTemp(m_editSubject.GetBuffer(0));
	string szSubject = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	/*��ȡ����*/
	cszTemp = m_editContent.GetBuffer(0);
	string szContent = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	
	/*���������ȡ�ռ��˵�ַ*/
	m_smtp.SetUserName(szUser);
	m_smtp.SetPass(szPwd);
	m_smtp.SetPortE(25);
	m_smtp.SetEmailTitle(szSubject);
	m_smtp.SetContent(szContent);
	for (int i = 0; i < n; i++)
	{

		cszTemp = mArray[i].GetBuffer(0);
		string szTarget = cszTemp.GetBuffer(0);
		cszTemp.ReleaseBuffer();
		//�ѻ�ȡ�ռ�����Ϣ
		m_smtp.SetTargetEmail(szTarget);			//��ѭ���������ռ��˵ĵ�ַ
		//1:���ȵ�½
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

	}

	UpdateData(false);
}