/*
 * socketserver.cpp
 *
 *  Created on: 2018年9月3日
 *  Author: TIAN FENG
 */

#include "socket_server.h"
#include <winsock.h>
#include <stdio.h>
#include <pthread.h>
#include "Cilent.h"

// 最大连接数
#define BACK_LOG 50
#define BUFF_SIZE 200

// 1. 初始化WSA库     WSAStartup
// 2. 创建socket套接字 socket
// 3. 绑定本地端口 bind
// 4. 置为监听状态 linsten
// 5. 阻塞链接客户端socket  accept
// 6. 交互 recv or send
// 7. 关闭socket 释放WSA资源  closesocket WSAClearn


// 保存的指针头
struct CLIENT * pHeader = NULL;

// 服务器绑定的socket
SOCKET socket_server;

void remove(struct CLIENT * client){

	while(pHeader!=NULL){
		struct CLIENT * temp = pHeader;
		if(pHeader == client){
			pHeader = pHeader->next;
		}
		closesocket(temp->client_socket);
		free(temp);
	}

}

// 连接线程
void* accept_runable(void* args);

// 读取线程
void* recv_runable(void* args);

// 转发消息
void dispatch_message(CLIENT* client_from, char* message);

int create_socket_server(int port) {
	// 2.2 版本
	WORD wVersion = MAKEWORD(2, 2);

	WSADATA wsaData;

	// 初始化库
	if (WSAStartup(wVersion, &wsaData) != 0) {
		printf("初始化WSA失败!\n");
		printf("程序退出!\n");
		return -1;
	}

	// 判断版本
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2) {
		printf("WSA版本不对\n");
		printf("程序退出\n");
		WSACleanup();
		return -1;
	}

	// 开始创建socket
	socket_server = socket(AF_INET, SOCK_STREAM, 0);

	// 绑定端口
	SOCKADDR_IN Server_addr;
	Server_addr.sin_family = AF_INET;
	Server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 本地端口
	Server_addr.sin_port = htons(port);
	if (bind(socket_server, (SOCKADDR*) &Server_addr,
			sizeof(SOCKADDR))==SOCKET_ERROR) {
		printf("WSA版本不对\n");
		printf("程序退出\n");
		WSACleanup();
		return -1;
	}
	// 获取本机ip
	struct hostent *phostinfo = gethostbyname("");
	char *p = inet_ntoa(*((struct in_addr *) (*phostinfo->h_addr_list)));
	// 打印地址端口
	printf("服务器->%s:%d\n", p, port);

	// 监听状态
	if (listen(socket_server, BACK_LOG) < 0) {
		printf("监听失败\n");
		printf("程序退出\n");
		WSACleanup();
		return -1;
	}

	// 连接客户端 多客户端需要线程操作
	pthread_t accept_p;
	if (pthread_create(&accept_p, NULL, accept_runable, NULL)) {
		printf("连接客户端线程异常\n");
		printf("程序退出\n");
		return -1;
	}
	return 0;
}

int flag = 0;

// 连接客户端
void* accept_runable(void* args) {
	SOCKADDR_IN Client_addr;
	int len = sizeof(SOCKADDR);

	while (1) {
		SOCKET socket_client = accept(socket_server, (SOCKADDR*) &Client_addr,
				&len);
		if (socket_client == SOCKET_ERROR) {
			printf("连接失败\n");
			continue;
		}

		// 创建内存空间
		CLIENT* _client = (CLIENT*) malloc(sizeof(CLIENT));
		_client->next = pHeader;
		_client->client_socket = socket_client;
		// 需要创建线程直读取
		pthread_t client_t;
		pHeader = _client;
		if (pthread_create(&client_t, NULL, recv_runable, _client)) {
			printf("客户端读取线程异常\n");
		}
	}

	return 0;
}


// 读取线程消息
void* recv_runable(void* args) {
	printf("有客户端连接成功\n");
	CLIENT* _client = (CLIENT*) args;
	strcpy(_client->name,"");
	_client->flag = flag++;
	char buf[BUFF_SIZE] = { 0 };
	char msg[200]={0};
	// 读取消息
	while (recv(_client->client_socket, buf, BUFF_SIZE, 0) >= 0) {
		// 转发消息 默认第一条为用户名
		if(strcmp(_client->name,"")==0){
			strcpy(_client->name,buf);
			strcat(buf,"进入聊天室");
			dispatch_message(_client, buf);
		}else{
			strcat(msg,_client->name);
			strcat(msg,":");
			strcat(msg,buf);
			dispatch_message(_client, msg);
		}
		memset(msg, 0x00, BUFF_SIZE);
		memset(buf, 0x00, BUFF_SIZE);
	}
	printf("读取失败\n");
	return 0;
}



// 转发消息
void dispatch_message(CLIENT* client_form, char* message) {
	CLIENT* temp = pHeader;
	do {
		if (temp->flag != client_form->flag) {
			printf("服务器转发->%s->%s\n",temp->name,message);
			if (send(temp->client_socket, message, BUFF_SIZE, 0) < 0) {
				printf("转发失败\n");
//				remove(temp);
				closesocket(temp->client_socket);
			}
		}
	} while ((temp = temp->next) != NULL);
}
