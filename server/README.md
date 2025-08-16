# Socket (一套用于连接的数字)

- 主机之间通信使用ip地址, 端口号用于区分通信时选择的应�?(进程), IP+端口就叫做Socket
-  通常Socket有两种类�?TCP (可靠) �?UDP (不可�?
- 在socket通信�? 需要用到之前的 **TCP UDP 握手方式**
- 在Linux�? socket是文�? 可以理解为一个代码库, 或者说接口�? 用于给内�?�?进程之间提供高度封装的接�?socket(), bind(), listen(), accept() �?
- 在应用程序的代码中使用这些接口相当于把网络传输的功能外包给内�? 真正执行网络传输的是内核
- 服务端与客户端建立通信�? 分别都有IP地址和端口号, 形成一个四元组, 生成Hash Key, 值为sock, 每次遇到对应的四元组就查询相应的Hash Key

- **1. AF / PF 系列**

  - **AF** = Address Family（地址族）
     表示使用的网络协议族，比如：
    - `AF_INET`：IPv4 地址族
    - `AF_INET6`：IPv6 地址族
    - `AF_UNIX`：本地 Unix 域套接字
  - **PF** = Protocol Family（协议族）
     功能和 AF 基本一样（历史上有区别，现在常等价），比如 `PF_INET` 对应 `AF_INET`

  📌 **记法**：
   `AF`/`PF` = “用哪种通信地址格式”

  ------

  ## **2. sin / sockaddr_in 系列**

  - **sin** = **s**ocket **in**ternet（Internet 套接字）
     出现在 IPv4 专用的 `sockaddr_in` 结构里：

    ```
    c复制编辑struct sockaddr_in {
        sa_family_t    sin_family; // 地址族（AF_INET）
        in_port_t      sin_port;   // 端口（网络字节序）
        struct in_addr sin_addr;   // IP 地址
        char           sin_zero[8];// 填充
    };
    ```

    - `sin_family`：协议族（IPv4 就是 `AF_INET`）
    - `sin_port`：端口号（`htons()` 转成网络字节序）
    - `sin_addr`：IP 地址结构（`struct in_addr`）
    - `sin_zero`：历史原因的填充字段（对齐用）

  - **in**

    - 出现在 `struct in_addr` 中，意思是 **Internet**，用于 IPv4 地址：

      ```
      c复制编辑struct in_addr {
          in_addr_t s_addr; // IPv4 地址（网络字节序）
      };
      ```

  ------

  ## **3. `_in` 后缀**

  - 表示 **Internet（IPv4 专用）** 版本的结构体
    - `sockaddr_in`：IPv4 专用的套接字地址结构
    - 对应 IPv6 的就是 `sockaddr_in6`
  - 这种命名是为了区分 `sockaddr`（通用版本）和具体协议版本的结构体

  ------

  ## **4. 常见函数和宏缩写**

  - `htons`：**h**ost **to** **n**etwork **s**hort（主机字节序 → 网络字节序，16 位）
  - `htonl`：**h**ost **to** **n**etwork **l**ong（主机字节序 → 网络字节序，32 位）
  - `ntohs` / `ntohl`：反方向（网络 → 主机）
  - `inet_pton`：**p**resentation（字符串）**to n**etwork（二进制）
  - `inet_ntop`：反方向（网络 → 字符串）
  - `bind`：绑定（将套接字绑定到 IP 和端口）
  - `listen`：监听（TCP 服务端开始接受连接）
  - `accept`：接受连接请求

  ------

  ## **5. 你可以这样快速记忆**

  - **AF**：Address Family（协议族）
  - **PF**：Protocol Family（协议族）
  - **sin**：socket internet（IPv4 地址结构）
  - **in**：Internet（与 IPv4 相关）
  - **_in**：IPv4 专用版本的结构
  - **hton/ntoh**：Host ↔ Network（字节序转换）

# C++类型转换

1. 是不是加/去 const？ → const_cast
2. 是不是数值类型之间的安全转换？（int ↔ double、size_t ↔ socklen_t、父子类向上转型） → static_cast
3. 是不是不相关类型的指针/引用转换？（sockaddr_in ↔ sockaddr、uintptr_t ↔ 指针） → reinterpret_cast
4. 是不是要向下转型（多态类）并需要运行时检查？ → dynamic_cast
