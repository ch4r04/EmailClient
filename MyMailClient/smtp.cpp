#include "smtp.h"  
#include <iostream>  
#include <fstream>  
using namespace std;

#pragma  comment(lib, "ws2_32.lib") /*链接ws2_32.lib动态链接库*/  

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
//////////////////////////////构造函数
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
	int err = recv(sockClient, buff, MAXLEN, 0); //接收数据  
	if (err == SOCKET_ERROR)
	{
		return false;
	}
	buff[err] = '\0';
	cout << buff << endl;
	return true;
}

bool CSmtp::CreateConn(){
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);//建立socket对象
	SOCKADDR_IN addSrv;

	HOSTENT *pHostent;
	pHostent = gethostbyname(domain.c_str());
	addSrv.sin_addr.S_un.S_addr = (*(DWORD*)pHostent->h_addr_list[0]);//网络字节序转换
	addSrv.sin_family = AF_INET;
	addSrv.sin_port = htons(port);//绑定端口号

	int err = connect(sockClient, (SOCKADDR*)&addSrv, sizeof(SOCKADDR));//向服务器发送连接请求
	if (err != 0)
	{
		return false;	//连接失败
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

	if (false == Send(sendBuff) || false == Recv()) //既接收也发送  
	{
		return 1; /*1表示发送失败由于网络错误*/
	}

	sendBuff.empty();
	sendBuff = "AUTH LOGIN\r\n";
	if (false == Send(sendBuff) || false == Recv()) //请求登陆  
	{
		return 1; /*1表示发送失败由于网络错误*/
	}

	sendBuff.empty();
	int pos = user.find('@', 0);
	sendBuff = user.substr(0, pos); //得到用户名  

	char *ecode;
	/*在这里顺带扯一句，关于string类的length函数与C语言中的strlen函数的区别,strlen计算出来的长度，只到'\0'字符为止,而string::length()函数实际上返回的是string类中字符数组的大小,你自己可以测试一下，这也是为什么我下面不使用string::length()的原因*/

	ecode = base64Encode(sendBuff.c_str(), strlen(sendBuff.c_str()));
	sendBuff.empty();
	sendBuff = ecode;
	sendBuff += "\r\n";
	delete[]ecode;

	if (false == Send(sendBuff) || false == Recv()) //发送用户名，并接收服务器的返回  
	{
		return 1; /*错误码1表示发送失败由于网络错误*/
	}

	sendBuff.empty();
	ecode = base64Encode(pass.c_str(), strlen(pass.c_str()));
	sendBuff = ecode;
	sendBuff += "\r\n";
	delete[]ecode;

	if (false == Send(sendBuff) || false == Recv()) //发送用户密码，并接收服务器的返回  
	{
		return 1; /*错误码1表示发送失败由于网络错误*/
	}

	if (NULL != strstr(buff, "550"))
	{
		return 2;/*错误码2表示用户名错误*/
	}

	if (NULL != strstr(buff, "535")) /*535是认证失败的返回*/
	{
		return 3; /*错误码3表示密码错误*/
	}
	return 0;
}

bool CSmtp::SendEmailHead()     //发送邮件头部信息  
{
	string sendBuff;
	sendBuff = "MAIL FROM: <" + user + ">\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; /*表示发送失败由于网络错误*/
	}


	sendBuff.empty();
	sendBuff = "RCPT TO: <" + targetAddr + ">\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; /*表示发送失败由于网络错误*/
	}

	sendBuff.empty();
	sendBuff = "DATA\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; //表示发送失败由于网络错误  
	}

	sendBuff.empty();
	FormatEmailHead(sendBuff);
	if (false == Send(sendBuff))
		//发送完头部之后不必调用接收函数,因为你没有\r\n.\r\n结尾，服务器认为你没有发完数据，所以不会返回什么值  
	{
		return false; /*表示发送失败由于网络错误*/
	}
	return true;
}

bool CSmtp::SendEmailHead_Mulit()     // 多发邮件的实现 遍历数组，构造新字符串
{
	string sendBuff;
	sendBuff = "MAIL FROM: <" + user + ">\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; /*表示发送失败由于网络错误*/
	}


	sendBuff.empty();
	sendBuff = targetAddr_Mulit;
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; /*表示发送失败由于网络错误*/
	}

	sendBuff.empty();
	sendBuff = "DATA\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false; //表示发送失败由于网络错误  
	}

	sendBuff.empty();
	FormatEmailHead(sendBuff);
	if (false == Send(sendBuff))
		//发送完头部之后不必调用接收函数,因为你没有\r\n.\r\n结尾，服务器认为你没有发完数据，所以不会返回什么值  
	{
		return false; /*表示发送失败由于网络错误*/
	}
	return true;
}

void CSmtp::FormatEmailHead(string &email)
{/*格式化要发送的内容*/
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

bool CSmtp::SendEmailContent()  /*发送邮件文本*/
{
	string sendBuff;
	sendBuff = "--qwertyuiop\r\n";
	sendBuff += "Content-Type: text/plain;";
	sendBuff += "charset=\"gb2312\"\r\n\r\n";
	sendBuff += content;
	sendBuff += "\r\n\r\n";
	return Send(sendBuff);
}

bool CSmtp::SendEnd() /*发送结尾信息*/
{
	string sendBuff;
	sendBuff = "--qwertyuiop--";
	sendBuff += "\r\n.\r\n";
	if (false == Send(sendBuff) || false == Recv())
	{
		return false;
	}
	cout << buff << endl;
	//sendBuff.empty();
	//sendBuff = "QUIT\r\n";
	return (Send(sendBuff) && Recv());
}

bool CSmtp::Quit(){	/*发送退出会话指令*/
	string sendBuff;
	sendBuff.empty();
	sendBuff = "QUIT\r\n";
	return (Send(sendBuff) && Recv());
}
//单发模式
int CSmtp::SendEmail_Ex()
{
	if (false == CreateConn())
	{
		return 1;
	}
	//Recv();  
	int err = Login(); //先登录  
	if (err != 0)
	{
		return err; //错误代码必须要返回  
	}
	if (false == SendEmailHead()) //发送EMAIL头部信息  
	{
		return 1; /*错误码1是由于网络的错误*/
	}
	if (false == SendEmailContent())
	{
		return 1; /*错误码1是由于网络的错误*/
	}
	if (false == SendEnd())
	{
		return 1; /*错误码1是由于网络的错误*/
	}
	if (false == Quit())
	{
		return 1;/*退出时网络错误*/
	}
	return 0; /*0表示没有出错*/
}
//群发模式
int CSmtp::SendEmail_Mu(){

	if (false == CreateConn())
	{
		return 1;
	}
	//Recv();  
	int err = Login(); //先登录  
	if (err != 0)
	{
		return err; //错误代码必须要返回  
	}
	if (false == SendEmailHead_Mulit()) //发送EMAIL头部信息  
	{
		return 1; /*错误码1是由于网络的错误*/
	}
	if (false == SendEmailContent())
	{
		return 1; /*错误码1是由于网络的错误*/
	}
	if (false == SendEnd())
	{
		return 1; /*错误码1是由于网络的错误*/
	}
	if (false == Quit())
	{
		return 1;/*退出时网络错误*/
	}
	return 0; /*0表示没有出错*/
}
//------------------Set方法-------------------------------//
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
void CSmtp::SetPortE(int port)
{
	this->port = port;
}
void CSmtp::setStrMulit(string mArray[], int count){
	string strBuff ;
	//遍历数组，在前面加上 RCPT TO:
	for (int i = 0; i < count; i++)
	{
		strBuff += "RCPT TO:";
		strBuff += " <";
		strBuff += mArray[i];	//add first
		strBuff += ">\r\n";
	}
	targetAddr_Mulit = strBuff;	//设置转发语句


}