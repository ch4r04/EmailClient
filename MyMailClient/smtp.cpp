#include "smtp.h"  
#include <iostream>  
#include <fstream>  
using namespace std;

#pragma  comment(lib, "ws2_32.lib") /*����ws2_32.lib��̬���ӿ�*/  

char* CSmtp::base64Encode(char const* origSigned, unsigned origLength)
{
	unsigned char const* orig = (unsigned char const*)origSigned; // in case any input bytes have the MSB set  
	if (orig == NULL) return NULL;

	unsigned const numOrig24BitValues = origLength / 3;
	bool havePadding = origLength > numOrig24BitValues * 3;
	bool havePadding2 = origLength == numOrig24BitValues * 3 + 2;
	unsigned const numResultBytes = 4 * (numOrig24BitValues + havePadding);
	char* result = new char[numResultBytes + 3]; // allow for trailing '/0'  

	// Map each full group of 3 input bytes into 4 output base-64 characters:  
	unsigned i;
	for (i = 0; i < numOrig24BitValues; ++i)
	{
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
		result[4 * i + 2] = base64Char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
		result[4 * i + 3] = base64Char[orig[3 * i + 2] & 0x3F];
	}

	// Now, take padding into account.  (Note: i == numOrig24BitValues)  
	if (havePadding)
	{
		result[4 * i + 0] = base64Char[(orig[3 * i] >> 2) & 0x3F];
		if (havePadding2)
		{
			result[4 * i + 1] = base64Char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
			result[4 * i + 2] = base64Char[(orig[3 * i + 1] << 2) & 0x3F];
		}
		else
		{
			result[4 * i + 1] = base64Char[((orig[3 * i] & 0x3) << 4) & 0x3F];
			result[4 * i + 2] = '=';
		}
		result[4 * i + 3] = '=';
	}

	result[numResultBytes] = '\0';
	return result;
}
//////////////////////////////���캯��
CSmtp::CSmtp()
{
	this->content = "";
	this->port = 25;
	this->user = "";
	this->pass = "";
	this->targetAddr = "";
	this->title = "";
	this->domain = "";

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	this->sockClient = 0;
}

CSmtp::~CSmtp(void)
{
	closesocket(sockClient);
	WSACleanup();
}
CSmtp::CSmtp(
	int port,
	string srvDomain,
	string userName,
	string password,
	string targetEmail,
	string emailTitle,
	string content
	)
{
	this->content = content;
	this->port = port;
	this->user = userName;
	this->pass = password;
	this->targetAddr = targetEmail;
	this->title = emailTitle;
	this->domain = srvDomain;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	this->sockClient = 0;
}

bool CSmtp::Recv()
{
	memset(buff, 0, sizeof(char)* (MAXLEN + 1));
	int err = recv(sockClient, buff, MAXLEN, 0); //��������  
	if (err == SOCKET_ERROR)
	{
		return false;
	}
	buff[err] = '\0';
	cout << buff << endl;
	return true;
}

bool CSmtp::CreateConn(){
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);//����socket����
	SOCKADDR_IN addSrv;

	HOSTENT *pHostent;
	pHostent = gethostbyname(domain.c_str());
	addSrv.sin_addr.S_un.S_addr = (*(DWORD*)pHostent->h_addr_list[0]);//�����ֽ���ת��
	addSrv.sin_family = AF_INET;
	addSrv.sin_port = htons(port);//�󶨶˿ں�

	int err = connect(sockClient, (SOCKADDR*)&addSrv, sizeof(SOCKADDR));//�������������������
	if (err != 0)
	{
		return false;	//����ʧ��
	}
	else{
		this->sockClient = sockClient;
	}

	if (false == Recv())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CSmtp::Send(string &message)
{
	int err = send(sockClient, message.c_str(), message.length(), 0);
	if (err == SOCKET_ERROR)
	{
		return false;
	}
	string message01;
	cout << message.c_str() << endl;
	return true;
}

int CSmtp::Login()
{
	string sendBuff;
	sendBuff = "EHLO ";
	sendBuff += user;
	sendBuff += "\r\n";

	if (false == Send(sendBuff) || false == Recv()) //�Ƚ���Ҳ����  
	{
		return 1; /*1��ʾ����ʧ�������������*/
	}

	sendBuff.empty();
	sendBuff = "AUTH LOGIN\r\n";
	if (false == Send(sendBuff) || false == Recv()) //�����½  
	{
		return 1; /*1��ʾ����ʧ�������������*/
	}

	sendBuff.empty();
	int pos = user.find('@', 0);
	sendBuff = user.substr(0, pos); //�õ��û���  

	char *ecode;
	/*������˳����һ�䣬����string���length������C�����е�strlen����������,strlen��������ĳ��ȣ�ֻ��'\0'�ַ�Ϊֹ,��string::length()����ʵ���Ϸ��ص���string�����ַ�����Ĵ�С,���Լ����Բ���һ�£���Ҳ��Ϊʲô�����治ʹ��string::length()��ԭ��*/

	ecode = base64Encode(sendBuff.c_str(), strlen(sendBuff.c_str()));
	sendBuff.empty();
	sendBuff = ecode;
	sendBuff += "\r\n";
	delete[]ecode;

	if (false == Send(sendBuff) || false == Recv()) //�����û����������շ������ķ���  
	{
		return 1; /*������1��ʾ����ʧ�������������*/
	}

	sendBuff.empty();
	ecode = base64Encode(pass.c_str(), strlen(pass.c_str()));
	sendBuff = ecode;
	sendBuff += "\r\n";
	delete[]ecode;

	if (false == Send(sendBuff) || false == Recv()) //�����û����룬�����շ������ķ���  
	{
		return 1; /*������1��ʾ����ʧ�������������*/
	}

	if (NULL != strstr(buff, "550"))
	{
		return 2;/*������2��ʾ�û�������*/
	}

	if (NULL != strstr(buff, "535")) /*535����֤ʧ�ܵķ���*/
	{
		return 3; /*������3��ʾ�������*/
	}
	return 0;
}

bool CSmtp::SendEmailHead()     //�����ʼ�ͷ����Ϣ  
{
	string sendBuff;
	sendBuff = "MAIL FROM: <" + user + ">\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; /*��ʾ����ʧ�������������*/
	}


	sendBuff.empty();
	sendBuff = "RCPT TO: <" + targetAddr + ">\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; /*��ʾ����ʧ�������������*/
	}

	sendBuff.empty();
	sendBuff = "DATA\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; //��ʾ����ʧ�������������  
	}

	sendBuff.empty();
	FormatEmailHead(sendBuff);
	if (false == Send(sendBuff))
		//������ͷ��֮�󲻱ص��ý��պ���,��Ϊ��û��\r\n.\r\n��β����������Ϊ��û�з������ݣ����Բ��᷵��ʲôֵ  
	{
		return false; /*��ʾ����ʧ�������������*/
	}
	return true;
}

void CSmtp::FormatEmailHead(string &email)
{/*��ʽ��Ҫ���͵�����*/
	email = "From: ";
	email += user;
	email += "\r\n";

	email += "To: ";
	email += targetAddr;
	email += "\r\n";

	email += "Subject: ";
	email += title;
	email += "\r\n";

	email += "MIME-Version: 1.0";
	email += "\r\n";

	email += "Content-Type: multipart/mixed;boundary=qwertyuiop";
	email += "\r\n";
	email += "\r\n";
}

bool CSmtp::SendEmailContent()  /*�����ʼ��ı�*/
{
	string sendBuff;
	sendBuff = "--qwertyuiop\r\n";
	sendBuff += "Content-Type: text/plain;";
	sendBuff += "charset=\"gb2312\"\r\n\r\n";
	sendBuff += content;
	sendBuff += "\r\n\r\n";
	return Send(sendBuff);
}

bool CSmtp::SendEnd() /*���ͽ�β��Ϣ*/
{
	string sendBuff;
	sendBuff = "--qwertyuiop--";
	sendBuff += "\r\n.\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false;
	}
	cout << buff << endl;
	sendBuff.empty();
	sendBuff = "QUIT\r\n";
	return (Send(sendBuff) && Recv());
}

int CSmtp::SendEmail_Ex()
{
	if (false == CreateConn())
	{
		return 1;
	}
	//Recv();  
	int err = Login(); //�ȵ�¼  
	if (err != 0)
	{
		return err; //����������Ҫ����  
	}
	if (false == SendEmailHead()) //����EMAILͷ����Ϣ  
	{
		return 1; /*������1����������Ĵ���*/
	}
	if (false == SendEmailContent())
	{
		return 1; /*������1����������Ĵ���*/
	}
	if (false == SendEnd())
	{
		return 1; /*������1����������Ĵ���*/
	}
	return 0; /*0��ʾû�г���*/
}
//------------------Set����-------------------------------//
void CSmtp::SetSrvDomain(string &domain)
{
	this->domain = domain;
}

void CSmtp::SetUserName(string &user)
{
	this->user = user;
}

void CSmtp::SetPass(string &pass)
{
	this->pass = pass;
}
void CSmtp::SetTargetEmail(string &targetAddr)
{
	this->targetAddr = targetAddr;
}
void CSmtp::SetEmailTitle(string &title)
{
	this->title = title;
}
void CSmtp::SetContent(string &content)
{
	this->content = content;
}
void CSmtp::SetPort(int port)
{
	this->port = port;
}
int main(){
	CSmtp smtp(
		25,                             /*smtp�˿�*/
		"smtp.163.com",                 /*smtp��������ַ*/
		"my_test_jsjwl@163.com",    /*��������ַ*/
		"asd123456",                  /*��������*/
		"857856615@qq.com",    /*Ŀ�������ַ*/
		"�ð�!",                          /*����*/
		"��ã��յ���ظ���"       /*�ʼ�����*/
		);
	/**
	//��Ӹ���ʱע��,\һ��Ҫд��\\����Ϊת���ַ���Ե��
	string filePath("D:\\�γ���Ʊ���.doc");
	smtp.AddAttachment(filePath);
	*/

	/*�����Ե���CSmtp::DeleteAttachment����ɾ������������һЩ�������Լ���ͷ�ļ���!*/
	//filePath = "C:\\Users\\��ܲ��\\Desktop\\sendEmail.cpp";  
	//smtp.AddAttachment(filePath);  

	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			cout << "����1: �������粻��ͨ������ʧ��!" << endl;
		if (err == 2)
			cout << "����2: �û�������,��˶�!" << endl;
		if (err == 3)
			cout << "����3: �û����������˶�!" << endl;
		if (err == 4)
			cout << "����4: ���鸽��Ŀ¼�Ƿ���ȷ���Լ��ļ��Ƿ����!" << endl;
	}
	system("pause");
	return 0;

}