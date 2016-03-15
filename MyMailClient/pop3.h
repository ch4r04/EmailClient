#pragma once

#include "wsocket.h"
class CPop3
{
public:
	CPop3(void);
public:
	~CPop3(void);

	//初始化pop3的属性 
	bool Create(
		const char* username,
		const char* userpwd,
		const char* svraddr,
		unsigned short port = 110
		);

	//连接POP3服务器
	bool Connect();

	//登陆
	int Login();

	//列出邮件
	bool List(int& sum);

	//抓取邮件
	bool Fetch(char* buf, int num = 1);
	bool FetchEx(int num = 1);

	//退出
	bool Quit();

	//得到主题
	bool GetSubject(char* subject, const char* buf);

	//写文件
	static bool WriteFile(const char* filename, const char* buffer, unsigned int len);

protected:
	int GetMailSum(const char* buf);

	WSocket m_sock;
	char m_username[64];
	char m_userpwd[64];
	char m_svraddr[32];
	unsigned short m_port;

private:
	int Pop3Recv(char* buf, int len, int flags = 0);
public:
	char *GetUser();
	char *GetPwd();
	void SetUser(char *username);
	void SetPwd(char *userpwd);
};
