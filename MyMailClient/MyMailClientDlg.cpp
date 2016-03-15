
// MyMailClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyMailClient.h"
#include "MyMailClientDlg.h"
#include "MainMenuDlg.h"
#include "afxdialogex.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPop3 m_pop3;
CSmtp m_smtp;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyMailClientDlg �Ի���



CMyMailClientDlg::CMyMailClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyMailClientDlg::IDD, pParent)
	, m_editsmtpaddr(_T(""))
	, m_editpopaddr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyMailClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER_COMBO, m_comboServer);
	DDX_Control(pDX, IDC_PORT_EDIT, m_editport);
	DDX_Text(pDX, IDC_SEL_WEB_EDIT2, m_editsmtpaddr);
	DDX_Text(pDX, IDC_SEL_WEB_EDIT, m_editpopaddr);
}

BEGIN_MESSAGE_MAP(CMyMailClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_SERVER_COMBO, &CMyMailClientDlg::OnCbnSelchangeServerCombo)
	ON_BN_CLICKED(IDC_SEL_SERVER_BUTTON, &CMyMailClientDlg::OnBnClickedSelServerButton)
END_MESSAGE_MAP()


// CMyMailClientDlg ��Ϣ�������

BOOL CMyMailClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_comboServer.AddString(_T("����163")); //������׷�������ַ

	m_comboServer.AddString(_T("����126"));

	m_comboServer.AddString(_T("��Ѷqq")); //�����Ѷ��������ַ

	m_comboServer.SetCurSel(0);	//Ĭ��ѡ������163������
	SetDlgItemText(IDC_SEL_WEB_EDIT, _T("pop.163.com"));
	SetDlgItemText(IDC_SEL_WEB_EDIT2, _T("smtp.163.com"));
	SetDlgItemText(IDC_PORT_EDIT, _T("110"));	//���úö˿� pop�������˿ڶ���110
	SetDlgItemText(IDC_USER_EDIT, _T("my_test_jsjwl@163.com"));
	SetDlgItemText(IDC_PWD_EDIT, _T("asd123456"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyMailClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyMailClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyMailClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyMailClientDlg::OnCbnSelchangeServerCombo()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strSev;
	int nSel;
	nSel = m_comboServer.GetCurSel(); // ѡ��0:pop.163.com 1:pop.126.com 2:pop.qq.com

	switch (nSel)
	{
	case 0:
		SetDlgItemText(IDC_SEL_WEB_EDIT, _T("pop.163.com"));
		strSev = "pop.163.com";
		break;
	case 1:
		SetDlgItemText(IDC_SEL_WEB_EDIT, _T("pop.126.com"));
		strSev = "pop.126.com";
		break;
	case 2:
		SetDlgItemText(IDC_SEL_WEB_EDIT, _T("pop.qq.com"));
		strSev = "pop.qq.com";
		break;
	default:
		break;
	}
	


}


void CMyMailClientDlg::OnBnClickedSelServerButton()
{
	// TODO:  ������ӷ���������
	UpdateData(TRUE);

	/*��ȡ���������ַ���*/
	CString c_address;
	GetDlgItemText(IDC_SEL_WEB_EDIT, c_address); //��ȡ�õ�ַ��ֵ
	char *m_address;
	m_address = (LPSTR)(LPCTSTR)c_address;
	//m_address = (LPSTR)(LPCTSTR)c_address;//�ַ���ת�� cstring ��char *

	/*��ȡ�û������ַ���*/
	CString c_user;
	GetDlgItemText(IDC_USER_EDIT, c_user);
	char *m_user = (LPSTR)(LPCTSTR)c_user;
	
	/*��ȡ�����ַ���*/
	CString c_pwd;
	GetDlgItemText(IDC_PWD_EDIT, c_pwd);
	char *m_pwd;
	m_pwd = (LPSTR)(LPCTSTR)c_pwd;
	
	m_pop3.Create(m_user, m_pwd, m_address, 110);
	CString str;

	if (FALSE == m_pop3.Connect())
	{
		MessageBox(_T("���ӷ�����ʧ��"));
		return;
	}
	else
	{
		MessageBox(_T("���ӷ������ɹ�"));
		string szSvr = m_editsmtpaddr.GetBuffer();
		m_smtp.SetSrvDomain(szSvr);		//ת���ַ�����smtp

	
	}
	//����ʹ���û����������½
	int iloginflag = m_pop3.Login();
	if (iloginflag == 1)
	{
		MessageBox(_T("����û����Ƿ����"));
		return;
	}
	else if (iloginflag == 2)
	{
		MessageBox(_T("�������"));
		return;
	}
	else
	{
		MessageBox(_T("��½�ɹ�"));
		//��½�ɹ��������û���������
		m_pop3.SetUser(m_user);
		m_pop3.SetPwd(m_pwd);
	}
	// �������˵�ģ��
	INT_PTR nRes;
	CMainMenuDlg menuDlg;
	nRes = menuDlg.DoModal();



}



void CMyMailClientDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

//	CDialogEx::OnOK();
}


BOOL CMyMailClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
