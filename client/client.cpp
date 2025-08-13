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
	//1 创建通信套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		cerr << "socket" << "\n";
		return -1;
	}

	//2 连接:客户端连接服务器IP 端口
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(9999); //9999是服务器的端口
	//将点分IP转换为网络字节序, 存储到saddr.sin_addr.s_addr中, 这里的IP是服务器IP地址
	//也就是说 saddr是服务器的通信地址, 客户端的IP由内核自动分配
	//服务器通过accept方法获取客户端的通信地址
	inet_pton(AF_INET, "172.25.175.140", &saddr.sin_addr.s_addr); 
	int ret = connect(fd, reinterpret_cast<sockaddr*>(&saddr), static_cast<socklen_t>(sizeof(saddr)));
	if (ret == -1) {
		cerr << "connect" << "\n";
		return -1;
	}

	//3 双方通信
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