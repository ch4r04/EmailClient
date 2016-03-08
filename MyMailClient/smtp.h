#ifndef __SMTP_H__ //避免重复包含  
#define __SMTP_H__  

#include <iostream>  
#include <list>  
#include <WinSock2.h>  
using namespace std;

const int MAXLEN = 1024;

static const char base64Char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class CSmtp{
	//该类用于发送邮件
private:
	int port;
	string domain;
	string user;
	string pass;
	string targetAddr;
	string title;
	string content;

	char buff[MAXLEN + 1];
	int buffLen;
	SOCKET sockClient;  //客户端的套接字  
public:
	CSmtp(int port,
		string srvDomain,   //smtp服务器域名  
		string userName,    //用户名  
		string password,    //密码  
		string targetEmail, //目的邮件地址  
		string emailTitle,  //主题  
		string content       //内容 
		);
	CSmtp();
	~CSmtp();
	char* base64Encode(char const* origSigned, unsigned origLength);//base64加密

	bool CreateConn(); //创建连接方法
	bool Recv();
	bool Send(string &message);
	void FormatEmailHead(string &email);	//格式化邮件信息
	int Login();
	bool SendEmailHead();//发送邮件的头部
	bool SendEmailContent();	//发送邮件正文
	int SendAttachment_Ex();
	bool SendEnd();

	//getter & setter
	void SetSrvDomain(string &domain);
	void SetUserName(string &user);
	void SetPass(string &pass);
	void SetTargetEmail(string &targetAddr);
	void SetEmailTitle(string &title);
	void SetContent(string &content);
	void SetPort(int port);
	int SendEmail_Ex();
};
#endif