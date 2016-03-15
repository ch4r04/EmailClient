
// MyMailClientDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMyMailClientDlg 对话框



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


// CMyMailClientDlg 消息处理程序

BOOL CMyMailClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_comboServer.AddString(_T("网易163")); //添加网易服务器地址

	m_comboServer.AddString(_T("网易126"));

	m_comboServer.AddString(_T("腾讯qq")); //添加腾讯服务器地址

	m_comboServer.SetCurSel(0);	//默认选择网易163服务器
	SetDlgItemText(IDC_SEL_WEB_EDIT, _T("pop.163.com"));
	SetDlgItemText(IDC_SEL_WEB_EDIT2, _T("smtp.163.com"));
	SetDlgItemText(IDC_PORT_EDIT, _T("110"));	//设置好端口 pop服务器端口都是110
	SetDlgItemText(IDC_USER_EDIT, _T("my_test_jsjwl@163.com"));
	SetDlgItemText(IDC_PWD_EDIT, _T("asd123456"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyMailClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyMailClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyMailClientDlg::OnCbnSelchangeServerCombo()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strSev;
	int nSel;
	nSel = m_comboServer.GetCurSel(); // 选择0:pop.163.com 1:pop.126.com 2:pop.qq.com

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
	// TODO:  完成连接服务器操作
	UpdateData(TRUE);

	/*获取服务器的字符串*/
	CString c_address;
	GetDlgItemText(IDC_SEL_WEB_EDIT, c_address); //获取该地址的值
	char *m_address;
	m_address = (LPSTR)(LPCTSTR)c_address;
	//m_address = (LPSTR)(LPCTSTR)c_address;//字符串转换 cstring 到char *

	/*获取用户名的字符串*/
	CString c_user;
	GetDlgItemText(IDC_USER_EDIT, c_user);
	char *m_user = (LPSTR)(LPCTSTR)c_user;
	
	/*获取密码字符串*/
	CString c_pwd;
	GetDlgItemText(IDC_PWD_EDIT, c_pwd);
	char *m_pwd;
	m_pwd = (LPSTR)(LPCTSTR)c_pwd;
	
	m_pop3.Create(m_user, m_pwd, m_address, 110);
	CString str;

	if (FALSE == m_pop3.Connect())
	{
		MessageBox(_T("连接服务器失败"));
		return;
	}
	else
	{
		MessageBox(_T("连接服务器成功"));
		string szSvr = m_editsmtpaddr.GetBuffer();
		m_smtp.SetSrvDomain(szSvr);		//转换字符串到smtp

	
	}
	//尝试使用用户名和密码登陆
	int iloginflag = m_pop3.Login();
	if (iloginflag == 1)
	{
		MessageBox(_T("检查用户名是否存在"));
		return;
	}
	else if (iloginflag == 2)
	{
		MessageBox(_T("密码错误"));
		return;
	}
	else
	{
		MessageBox(_T("登陆成功"));
		//登陆成功后设置用户名和密码
		m_pop3.SetUser(m_user);
		m_pop3.SetPwd(m_pwd);
	}
	// 弹出主菜单模块
	INT_PTR nRes;
	CMainMenuDlg menuDlg;
	nRes = menuDlg.DoModal();



}



void CMyMailClientDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

//	CDialogEx::OnOK();
}


BOOL CMyMailClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
