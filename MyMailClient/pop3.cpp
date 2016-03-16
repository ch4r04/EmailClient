#include <stdlib.h>
#include <string.h>
#include "StdAfx.h"
#include "pop3.h"

CPop3::CPop3(void)
{
	WSocket::Init();
}

CPop3::~CPop3(void)
{
	WSocket::Clean();
}

int CPop3::Pop3Recv(char* buf, int len, int flags)
{
	int rs;
	int offset = 0;

	do
	{
		if (offset > len - 2)
			return offset;

		rs = m_sock.Recv(buf + offset, len - offset, flags);
		if (rs < 0) return -1;

		offset += rs;
		buf[offset] = '\0';
	} while (strstr(buf, "\r\n.\r\n") == (char*)NULL);

	return offset;
}

bool CPop3::Create(
	const char* username,	//用户名
	const char* userpwd,		//用户密码
	const char* svraddr,		//服务器地址
	unsigned short port		//服务端口
	)
{
	strcpy_s(m_username, username);
	strcpy_s(m_userpwd, userpwd);
	strcpy_s(m_svraddr, svraddr);
	m_port = port;

	return true;
}

bool CPop3::Connect()
{
	//创建套接字
	m_sock.Create(AF_INET, SOCK_STREAM, 0);

	// Parse domain
	char ipaddr[16];
	if (WSocket::DnsParse(m_svraddr, ipaddr) != true)
		return FALSE;

	//连接服务器
	if (m_sock.Connect(ipaddr, m_port) != true)
		return FALSE;

	//接收pop3服务器发来的欢迎信息
	char buf[128];
	int rs = m_sock.Recv(buf, sizeof(buf), 0);
	if (rs <= 0 || strncmp(buf, "+OK", 3) != 0)
	{
		return FALSE;
	}

	return TRUE;
}

int CPop3::Login()
{/*登陆*/

	/*发送用户命令*/
	char sendbuf[128];
	char recvbuf[128];

	sprintf_s(sendbuf, 128, "USER %s\r\n", m_username);
	m_sock.Send(sendbuf, strlen(sendbuf), 0); //发送用户名

	int rs = m_sock.Recv(recvbuf, sizeof(recvbuf), 0);	//接收服务器发来的信息
	if (rs <= 0 || strncmp(recvbuf, "+OK", 3) != 0)  //如果没有"+OK"就说明失败了
	{
		return 1;		//说明没有用户名
	}

	/*发送密码信息*/
	sprintf_s(sendbuf, 128, "PASS %s\r\n", m_userpwd);
	m_sock.Send(sendbuf, strlen(sendbuf), 0);
	rs = m_sock.Recv(recvbuf, sizeof(recvbuf), 0);
	if (rs <= 0 || strncmp(recvbuf, "+OK", 3) != 0)
	{
		return 2;		// 说明密码错误
	}

	return 0;		// 说明登陆验证成功
}

bool CPop3::List(int& sum)
{
	/*发送list命令*/
	char sendbuf[128];
	char recvbuf[256];

	sprintf_s(sendbuf, 128, "LIST \r\n");
	m_sock.Send(sendbuf, strlen(sendbuf), 0);
	int rs = Pop3Recv(recvbuf, sizeof(recvbuf), 0);
	if (rs <= 0 || strncmp(recvbuf, "+OK", 3) != 0)
	{
		return false;
	}
	recvbuf[rs] = '\0';

	sum = GetMailSum(recvbuf); //得到邮件的数目

	return true;
}

bool CPop3::FetchEx(int num)
{
	int rs;
	FILE* fp;
	int flag = 0;
	unsigned int len;
	char filename[256];

	char sendbuf[128];
	char recvbuf[20480];

	/* 发送RETR命令*/
	sprintf_s(sendbuf, 128, "RETR %d\r\n", num);
	m_sock.Send(sendbuf, strlen(sendbuf), 0);
	do
	{
		rs = Pop3Recv(recvbuf, sizeof(recvbuf), 0); //接收数据
		if (rs < 0)
		{
			return false;
		}

		recvbuf[rs] = '\0';

		//printf("Recv RETR Resp: %s", recvbuf);

		//得到mail的主题
		if (flag == 0)
		{
			//GetSubject(subjectname, recvbuf);
			//cout << strlen(subjectname) << endl;

			_itoa(num, filename, 10);
			strcat(filename, ".eml");

			flag = 1;
			fp = fopen(filename, "wb");//准备写文件
		}
		len = strlen(recvbuf);
		fwrite(recvbuf, 1, len, fp);
		fflush(fp); //刷新	
	} while (strstr(recvbuf, "\r\n.\r\n") == (char*)NULL);

	fclose(fp);
	return true;
}

bool CPop3::Quit()
{
	char sendbuf[128];
	char recvbuf[128];

	/*发送QUIT命令*/
	sprintf(sendbuf, "QUIT\r\n");
	m_sock.Send(sendbuf, strlen(sendbuf), 0);
	int rs = m_sock.Recv(recvbuf, sizeof(recvbuf), 0);
	if (rs <= 0 || strncmp(recvbuf, "+OK", 3) != 0)
	{
		return false;
	}


	m_sock.Close();
	return true;
}



int CPop3::GetMailSum(const char* buf)
{
	int sum = 0;
	const char* p = strstr(buf, "\r\n");
	if (p == NULL)
		return sum;

	p = strstr(p + 2, "\r\n");
	if (p == NULL)
		return sum;

	while ((p = strstr(p + 2, "\r\n")) != NULL)
	{
		sum++;
	}

	return sum;
}
char *CPop3::GetUser(){
	
	return m_username;
}
char *CPop3::GetPwd(){

	return m_userpwd;
}
void CPop3::SetUser(char *username){
	if (username != NULL)
	{
		memcpy(m_username, username, sizeof(username));
	}
}
void CPop3::SetPwd(char *userpwd){
	if (userpwd != NULL)
	{
		memcpy(m_userpwd, userpwd, sizeof(userpwd));
		
	}
}
void CPop3::SetMailSum(int num){
	m_mailsum = num;
}
int CPop3::GetAllSum(){
	return m_mailsum;
}