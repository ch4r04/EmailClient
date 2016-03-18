#ifndef __SMTP_H__ //�����ظ�����  
#define __SMTP_H__  

#include <iostream>  
#include <list>  
#include <WinSock2.h>  
using namespace std;

const int MAXLEN = 1024;

static const char base64Char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class CSmtp{
	//�������ڷ����ʼ�
private:
	int port;
	string domain;
	string user;
	string pass;
	string targetAddr;
	string targetAddr_Mulit;
	string title;
	string content;

	char buff[MAXLEN + 1];
	int buffLen;
	SOCKET sockClient;  //�ͻ��˵��׽���  
public:
	CSmtp(int port,
		string srvDomain,   //smtp����������  
		string userName,    //�û���  
		string password,    //����  
		string targetEmail, //Ŀ���ʼ���ַ  
		string emailTitle,  //����  
		string content       //���� 
		);
	CSmtp();
	~CSmtp();
	char* base64Encode(char const* origSigned, unsigned origLength);//base64����

	bool CreateConn(); //�������ӷ���
	bool Recv();
	bool Send(string &message);
	void FormatEmailHead(string &email);	//��ʽ���ʼ���Ϣ
	int Login();
	bool SendEmailHead();//�����ʼ���ͷ��
	bool SendEmailHead_Mulit();	//�����ʼ�ͷ�� ����ռ���
	bool SendEmailContent();	//�����ʼ�����
	int SendAttachment_Ex();
	bool SendEnd();
	bool Quit();

	//getter & setter
	void SetSrvDomain(string &domain);
	void SetUserName(string &user);
	void SetPass(string &pass);
	void SetTargetEmail(string &targetAddr);
	void SetEmailTitle(string &title);
	void SetContent(string &content);
	void SetPortE(int port);
	int SendEmail_Ex();
	int SendEmail_Mu();
	void setStrMulit(string mArray[],int count); // targetAddr_Mulit = {"609461975@qq.com","857856615@qq.com"}
};
#endif