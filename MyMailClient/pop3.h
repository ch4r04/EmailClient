#pragma once

#include "wsocket.h"
class CPop3
{
public:
	CPop3(void);
public:
	~CPop3(void);

	//��ʼ��pop3������ 
	bool Create(
		const char* username,
		const char* userpwd,
		const char* svraddr,
		unsigned short port = 110
		);

	//����POP3������
	bool Connect();

	//��½
	int Login();

	//�г��ʼ�
	bool List(int& sum);

	//ץȡ�ʼ�
	bool Fetch(char* buf, int num = 1);
	bool FetchEx(int num = 1);

	//�˳�
	bool Quit();

	//�õ�����
	bool GetSubject(char* subject, const char* buf);

	//д�ļ�
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
