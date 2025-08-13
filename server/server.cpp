#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
using std::cerr;
using std::endl;
using std::cout;
using std::string;
int main()
{
   
    //1 创建监听的套接字, 该套接字指定了套接字的类型, 是空的, IP和端口要在 bind 阶段
    int fd = socket(AF_INET, SOCK_STREAM, 0); // IPV4, 字节流
    if (fd == -1) {
        cerr << "socket\n";
        return -1;
    }
    
    //2 绑定本地IP, 端口, --插座的位置
    sockaddr_in saddr; // sockaddr_in 是IPV4专用的地址结构, 里面有具体的 IP 端口 等信息
    saddr.sin_family = AF_INET; // IPV4
    saddr.sin_port = htons(9999); //端口号
    // sin_addr是嵌套结构体, s_addr是具体的IP地址, 无符号整型
    //INADDR_ANY是本地任意IP的宏, 值为0, 大端小端相同, 无需转换
    saddr.sin_addr.s_addr = INADDR_ANY; 
    // 在 socket API 中，几乎所有的地址参数类型是 struct sockaddr*，它是一个通用地址结构
    int ret = bind(fd, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)); 
    if (ret == -1) {
        cerr << "ret\n";
        return -1;
    }

    //3 开始监听, 告诉客户端可以连接了(插头可以插入了)
    ret = listen(fd, 128); 
    if (ret == -1) {
        cerr << "listen\n";
        return -1;
    }

    //4 阻塞 等待客户端连接
    sockaddr_in caddr;
    socklen_t addrlen = sizeof(caddr);
    // fd是先前socket bind listen 创建的监听套接字
    // cfd 是通信套接字, accept会填充客户端的IP 端口给 cfd
    int cfd = accept(fd, reinterpret_cast<sockaddr*>(&caddr), &addrlen);
    if (cfd == -1) {
        cerr << "accept\n";
        return -1;
    }
    // 连接成功后使用 inet_ntop(network to presentation) 打印client信息
    //inet_ntop将二进制的信息转换为可读的形式, 将结果放置在缓冲区
    char ip_buffer[32];
    socklen_t ip_len = sizeof(ip_buffer);
    cout << "Client IP: " << inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip_buffer, ip_len) << "\n";
    cout << "Client Port: " << ntohs(caddr.sin_port) << "\n";

    //5 连接成功后可以进行通信 发送 接受数据
    while (1) {
        char buff[1024];
        int len = recv(cfd, buff, socklen_t(sizeof(buff)), 0);
        string clientMsg(buff, len);
        if (len > 0) {
            cout << clientMsg << "\n";
            send(cfd, buff, len, 0);
        }
        else if (len == 0) {
            cout << "Client is unconnected.\n";
            break;
        }
        else {
            cerr << "recv\n";
            break;
        }
    }

    close(fd);
    close(cfd);
}