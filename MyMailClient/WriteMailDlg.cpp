// WriteMailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyMailClient.h"
#include "MyMailClientDlg.h"
#include "WriteMailDlg.h" 
#include "afxdialogex.h"
#include "smtp.h"


// CWriteMailDlg 对话框

IMPLEMENT_DYNAMIC(CWriteMailDlg, CDialog)

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
}


BEGIN_MESSAGE_MAP(CWriteMailDlg, CDialog)
	ON_BN_CLICKED(IDC_SEND_MAIL_BUTTON, &CWriteMailDlg::OnBnClickedSendMailButton)
END_MESSAGE_MAP()




void CWriteMailDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

//	CDialog::OnOK();
}


BOOL CWriteMailDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		pMsg->wParam = VK_RETURN;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CWriteMailDlg::OnBnClickedSendMailButton()
{
	// TODO:  在此添加控件通知处理程序代		
	/*获取登录者用户名*/
	UpdateData(true);
	char *szUserName = m_pop3.GetUser();	
	string szUser(szUserName);

	/*获取登录者密码*/
	char *szUserPwd = m_pop3.GetPwd();				//获取密码
	string szPwd(szUserPwd);

	/*安全检查*/
	if (m_editTarget.GetLength() == 0)
	{
		MessageBox(_T("请输入收件人地址"));
		return;
	}
	int pos = m_editTarget.Find(_T('@'));
	if (-1 == pos || pos < 2)
	{
		MessageBox(_T("请输入完整规范的的邮件地址！"));
		return;
	}
	if (m_editSubject.GetLength() == 0)
	{
		MessageBox(_T("请输入主题"));
		return;
	}
	if (m_editContent.GetLength() == 0)
	{
		MessageBox(_T("请输入正文"));
		return;
	}
	/*安全检查*/
	/*获取发件人地址*/
	CStringA cszTemp(m_editTarget.GetBuffer());		//临时变量 只是获取地址
	string szTarget = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	/*获取邮件主题*/
	cszTemp = m_editSubject.GetBuffer(0);
	string szSubject = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	/*获取邮件正文*/
	cszTemp = m_editContent.GetBuffer(0);
	string szContent = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	m_smtp.SetUserName(szUser);
	m_smtp.SetPass(szPwd);
	m_smtp.SetPortE(25);
	m_smtp.SetEmailTitle(szSubject);
	m_smtp.SetContent(szContent);
	m_smtp.SetTargetEmail(szTarget);
	int err;
	if ((err = m_smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			MessageBox(_T("错误，网络不畅通，发送失败"));
		if (err == 2)
			MessageBox(_T("用户名错误"));
		if (err == 3)
			MessageBox(_T("密码错误"));
	}
	else
	{
		MessageBox("发送成功");
	}


	UpdateData(false);
	

	

}
