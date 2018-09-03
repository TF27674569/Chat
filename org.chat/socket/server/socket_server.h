/*
 * socketserver.h
 *
 *  Created on: 2018年9月3日
 *  Author: TIAN FENG
 */

// 1. 初始化WSA库     WSAStartup
// 2. 创建socket套接字 socket
// 3. 绑定本地端口 bind
// 4. 置为监听状态 linsten
// 5. 阻塞链接客户端socket  accept
// 6. 交互 recv or send
// 7. 关闭socket 释放WSA资源  closesocket WSAClearn

int create_socket_server(int port);
