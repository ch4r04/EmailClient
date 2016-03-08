#include <WinSock2.h>  
#pragma  comment(lib, "ws2_32.lib") /*链接ws2_32.lib动态链接库*/  

class CPop3 {
	//该类用于接收邮件 也用于用户的登陆
public:
	CPop3();
	~CPop3();

	//初始化pop3的属性  
	bool Create(const char* username, const char* userpwd, const char* svraddr,
		unsigned short port = 110);

	//连接pop3服务器  
	bool Connect();

	//登陆的服务器  
	bool Login();

	//利用list命令得到所有的邮件数目  
	bool List(int& sum);

	//获得序号为num的邮件  
	bool FetchEx(int num = 1);

	//退出命令  
	bool Quit();

protected:
	int GetMailSum(char* buf);

	SOCKET m_sock;
	char m_username[32];    /*用户名*/
	char m_userpwd[32];     /*密码*/
	char m_svraddr[32];     /*服务器域名*/
	unsigned short m_port;

private:
	int Pop3Recv(char* buf, int len, int flags = 0);

};