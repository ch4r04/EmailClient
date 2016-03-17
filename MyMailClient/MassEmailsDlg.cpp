// MassEmailsDlg.cpp : 实现文件
//实现群发功能

#include "stdafx.h"
#include "MyMailClient.h"
#include "MyMailClientDlg.h"
#include "MassEmailsDlg.h"
#include "afxdialogex.h"


// CMassEmailsDlg 对话框

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


// CMassEmailsDlg 消息处理程序


void CMassEmailsDlg::OnBnClickedSendButton()
{
	UpdateData(true);
	// TODO:  在此添加控件通知处理程序代码
	int n = m_cszRecvs.Replace(";", ";");
	if (m_cszRecvs.Right(1).Compare(_T(";")) != 0)
	{
		//如果最后一个字符忘记加上;则返回
		MessageBox(_T("末尾忘记加;了"));
		return;
	}
	CString mArray[20];
	for (int i = 0; i < n; i++)
	{
		AfxExtractSubString(mArray[i], m_cszRecvs, i, ';');

	}
	/*安全检查*/
	if (m_cszRecvs.GetLength() == 0)
	{
		MessageBox(_T("请输入收件人"));

	}
	for (int i = 0; i < n; i++)
	{
		int poc = mArray[i].Find(_T("@"));	//遍历收件人数组
		if (-1 == poc || poc < 2)
		{
			MessageBox(_T("请输入完整规范的的邮件地址！"));
			return;
		}
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

	/*获取登陆者账号*/
	char *szUserName = m_pop3.GetUser();
	string szUser(szUserName);

	/*获取登录者密码*/
	char *szUserPwd = m_pop3.GetPwd();				//获取密码
	string szPwd(szUserPwd);

	/*获取主题*/
	CStringA cszTemp(m_editSubject.GetBuffer(0));
	string szSubject = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	/*获取正文*/
	cszTemp = m_editContent.GetBuffer(0);
	string szContent = cszTemp.GetBuffer(0);
	cszTemp.ReleaseBuffer();

	
	/*遍历数组获取收件人地址*/
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
		//已获取收件人信息
		m_smtp.SetTargetEmail(szTarget);			//在循环中设置收件人的地址
		//1:首先登陆
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

	}

	UpdateData(false);
}