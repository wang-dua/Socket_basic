#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
using std::cerr;
using std::cout;
using std::string;
int main() {
	//1 ����ͨ���׽���
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		cerr << "socket" << "\n";
		return -1;
	}

	//2 ����:�ͻ������ӷ�����IP �˿�
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(9999); //9999�Ƿ������Ķ˿�
	//�����IPת��Ϊ�����ֽ���, �洢��saddr.sin_addr.s_addr��, �����IP�Ƿ�����IP��ַ
	//Ҳ����˵ saddr�Ƿ�������ͨ�ŵ�ַ, �ͻ��˵�IP���ں��Զ�����
	//������ͨ��accept������ȡ�ͻ��˵�ͨ�ŵ�ַ
	inet_pton(AF_INET, "172.25.175.140", &saddr.sin_addr.s_addr); 
	int ret = connect(fd, reinterpret_cast<sockaddr*>(&saddr), static_cast<socklen_t>(sizeof(saddr)));
	if (ret == -1) {
		cerr << "connect" << "\n";
		return -1;
	}

	//3 ˫��ͨ��
	int n = 0;
	while (true)
	{
		string msg = "Number" + std::to_string(n++);
		send(fd, msg.c_str(), msg.size(), 0);

		char buff[1024];
		int len = recv(fd, buff, sizeof(buff), 0);
		if (len > 0)
		{
			string serverMsg(buff, len);
			cout << serverMsg << "\n";
		}
		else if (len == 0)
		{
			cout << "Server is unconnected." << "\n";
			break;
		}
		else
		{
			cerr << "recv" << "\n";
			return -1;
		}
		sleep(5);
	}
	
	close(fd);
	return 0;
}