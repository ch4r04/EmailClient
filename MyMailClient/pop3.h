#include <WinSock2.h>  
#pragma  comment(lib, "ws2_32.lib") /*����ws2_32.lib��̬���ӿ�*/  

class CPop3 {
	//�������ڽ����ʼ� Ҳ�����û��ĵ�½
public:
	CPop3();
	~CPop3();

	//��ʼ��pop3������  
	bool Create(const char* username, const char* userpwd, const char* svraddr,
		unsigned short port = 110);

	//����pop3������  
	bool Connect();

	//��½�ķ�����  
	bool Login();

	//����list����õ����е��ʼ���Ŀ  
	bool List(int& sum);

	//������Ϊnum���ʼ�  
	bool FetchEx(int num = 1);

	//�˳�����  
	bool Quit();

protected:
	int GetMailSum(char* buf);

	SOCKET m_sock;
	char m_username[32];    /*�û���*/
	char m_userpwd[32];     /*����*/
	char m_svraddr[32];     /*����������*/
	unsigned short m_port;

private:
	int Pop3Recv(char* buf, int len, int flags = 0);

};