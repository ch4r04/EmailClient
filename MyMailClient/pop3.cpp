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
{/*��������*/
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
	const char* username,    //�û���  
	const char* userpwd,     //�û�����  
	const char* svraddr,     //��������ַ  
	unsigned short port      //����˿�  
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
	//�����׽���  
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	//IP��ַ  
	char ipaddr[16];

	struct hostent* p;
	if ((p = gethostbyname(m_svraddr)) == NULL) //����ò�����������Ϣ,��˵������  
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


	//����pop������  
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ipaddr);
	svraddr.sin_port = htons(m_port);
	int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR)
	{
		return FALSE;
	}


	//����pop3�����������Ļ�ӭ��Ϣ  
	char buf[128];
	int rs = recv(m_sock, buf, sizeof(buf), 0);
	buf[rs] = '\0';

	printf("%s", buf);
	if (rs <= 0 || strncmp(buf, "+OK", 3) != 0)  /*������û�з���OK�ͳ�����*/
	{
		return FALSE;
	}

	return TRUE;
}

bool CPop3::Login()
{/*��½*/

	/*�����û�����*/

	char sendbuf[128];
	char recvbuf[128];

	sprintf(sendbuf, "USER %s\r\n", m_username);
	printf("%s", sendbuf);
	send(m_sock, sendbuf, strlen(sendbuf), 0); //�����û���  


	int rs = recv(m_sock, recvbuf, sizeof(recvbuf), 0); //���շ�������������Ϣ  
	recvbuf[rs] = '\0';
	if (rs <= 0 || strncmp(recvbuf, "+OK", 3) != 0)  //���û��"+OK"��˵��ʧ����  
	{
		return FALSE;
	}
	printf("%s", recvbuf);

	/*����������Ϣ*/
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
	/*����list����*/
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

	sum = GetMailSum(recvbuf); //�õ��ʼ�����Ŀ  

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

	/* ����RETR����*/
	sprintf(sendbuf, "RETR %d\r\n", num);
	send(m_sock, sendbuf, strlen(sendbuf), 0);
	do
	{
		rs = Pop3Recv(recvbuf, sizeof(recvbuf), 0); //��������  
		if (rs < 0)
		{
			return FALSE;
		}

		recvbuf[rs] = '\0';

		printf("Recv RETR Resp: %s", recvbuf); //������յ�����  

		if (flag == 0)
		{
			_itoa(num, filename, 10); //������Ÿ��ļ�����  
			strcat(filename, ".eml");

			flag = 1;
			fp = fopen(filename, "wb");//׼��д�ļ�  
		}
		len = strlen(recvbuf);
		fwrite(recvbuf, 1, len, fp);
		fflush(fp); //ˢ��      
	} while (strstr(recvbuf, "\r\n.\r\n") == (char*)NULL);

	fclose(fp);
	return TRUE;
}

bool CPop3::Quit()
{
	char sendbuf[128];
	char recvbuf[128];

	/*����QUIT����*/
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