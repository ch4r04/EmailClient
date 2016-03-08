#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include "pop3.h"

CPop3::CPop3()
{
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	WSAStartup(version, &wsaData);
}

CPop3::~CPop3()
{
	WSACleanup();
}

int CPop3::Pop3Recv(char* buf, int len, int flags)
{/*接收数据*/
	int rs;
	int offset = 0;

	do
	{
		if (offset > len - 2)
			return offset;

		rs = recv(m_sock, buf + offset, len - offset, flags);
		if (rs < 0) return -1;

		offset += rs;
		buf[offset] = '\0';
	} while (strstr(buf, "\r\n.\r\n") == (char*)NULL);

	return offset;
}

bool CPop3::Create(
	const char* username,    //用户名  
	const char* userpwd,     //用户密码  
	const char* svraddr,     //服务器地址  
	unsigned short port      //服务端口  
	)
{
	strcpy(m_username, username);
	strcpy(m_userpwd, userpwd);
	strcpy(m_svraddr, svraddr);
	m_port = port;

	return true;
}

bool CPop3::Connect()
{
	//创建套接字  
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	//IP地址  
	char ipaddr[16];

	struct hostent* p;
	if ((p = gethostbyname(m_svraddr)) == NULL) //如果得不到服务器信息,就说明出错  
	{
		return FALSE;
	}


	sprintf(
		ipaddr,
		"%u.%u.%u.%u",
		(unsigned char)p->h_addr_list[0][0],
		(unsigned char)p->h_addr_list[0][1],
		(unsigned char)p->h_addr_list[0][2],
		(unsigned char)p->h_addr_list[0][3]
		);


	//连接pop服务器  
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ipaddr);
	svraddr.sin_port = htons(m_port);
	int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR)
	{
		return FALSE;
	}


	//接收pop3服务器发来的欢迎信息  
	char buf[128];
	int rs = recv(m_sock, buf, sizeof(buf), 0);
	buf[rs] = '\0';

	printf("%s", buf);
	if (rs <= 0 || strncmp(buf, "+OK", 3) != 0)  /*服务器没有返回OK就出错了*/
	{
		return FALSE;
	}

	return TRUE;
}

bool CPop3::Login()
{/*登陆*/

	/*发送用户命令*/

	char sendbuf[128];
	char recvbuf[128];

	sprintf(sendbuf, "USER %s\r\n", m_username);
	printf("%s", sendbuf);
	send(m_sock, sendbuf, strlen(sendbuf), 0); //发送用户名  


	int rs = recv(m_sock, recvbuf, sizeof(recvbuf), 0); //接收服务器发来的信息  
	recvbuf[rs] = '\0';
	if (rs <= 0 || strncmp(recvbuf, "+OK", 3) != 0)  //如果没有"+OK"就说明失败了  
	{
		return FALSE;
	}
	printf("%s", recvbuf);

	/*发送密码信息*/
	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "PASS %s\r\n", m_userpwd);
	send(m_sock, sendbuf, strlen(sendbuf), 0);
	printf("%s", sendbuf);

	rs = recv(m_sock, recvbuf, sizeof(recvbuf), 0);
	recvbuf[rs] = '\0';
	if (rs <= 0 || strncmp(recvbuf, "+OK", 3) != 0)
	{
		return FALSE;
	}
	printf("%s", recvbuf);

	return TRUE;
}

bool CPop3::List(int& sum)
{
	/*发送list命令*/
	char sendbuf[128];
	char recvbuf[256];

	sprintf(sendbuf, "LIST \r\n");
	send(m_sock, sendbuf, strlen(sendbuf), 0);
	printf("%s", sendbuf);

	int rs = recv(m_sock, recvbuf, sizeof(recvbuf), 0);
	if (rs <= 0 || strncmp(recvbuf, "+OK", 3) != 0)
	{
		return FALSE;
	}
	recvbuf[rs] = '\0';
	printf("%s", recvbuf);

	sum = GetMailSum(recvbuf); //得到邮件的数目  

	return TRUE;
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
	sprintf(sendbuf, "RETR %d\r\n", num);
	send(m_sock, sendbuf, strlen(sendbuf), 0);
	do
	{
		rs = Pop3Recv(recvbuf, sizeof(recvbuf), 0); //接收数据  
		if (rs < 0)
		{
			return FALSE;
		}

		recvbuf[rs] = '\0';

		printf("Recv RETR Resp: %s", recvbuf); //输出接收的数据  

		if (flag == 0)
		{
			_itoa(num, filename, 10); //按照序号给文件排名  
			strcat(filename, ".eml");

			flag = 1;
			fp = fopen(filename, "wb");//准备写文件  
		}
		len = strlen(recvbuf);
		fwrite(recvbuf, 1, len, fp);
		fflush(fp); //刷新      
	} while (strstr(recvbuf, "\r\n.\r\n") == (char*)NULL);

	fclose(fp);
	return TRUE;
}

bool CPop3::Quit()
{
	char sendbuf[128];
	char recvbuf[128];

	/*发送QUIT命令*/
	sprintf(sendbuf, "QUIT\r\n");
	send(m_sock, sendbuf, strlen(sendbuf), 0);
	int rs = recv(m_sock, recvbuf, sizeof(recvbuf), 0);
	if (rs <= 0 || strncmp(recvbuf, "+OK", 3) != 0)
	{
		return FALSE;
	}


	closesocket(m_sock);
	return TRUE;
}



int CPop3::GetMailSum(char* buf)
{
	int sum = 0;
	char* p = strstr(buf, "\r\n");
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