/*
 * client_server.cpp
 *
 *  Created on: 2018年9月3日
 *  Author: TIAN FENG
 */

#include "socket_client.h"

#include <winsock.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define BUFF_SIZE 200

SOCKET socket_client;
char addr[20];
char user_name[20];

//1.初始化WSA库
//2.初始化套接字
//3.链接socket
//4.交互

int initWSA() {
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
	return 0;
}

// 用于读取socket信息的线程
void* recv_runable_thread(void* arg);

void ip_select(int flag) {
	system("cls");
	printf("-------------------------------------------------\n");
	if (flag) {
		printf("请选择 \n");
	} else {
		printf("输入有误请重新输入 \n");
	}
	printf("1.使用本机ip\n");
	printf("2.手动输入ip\n");
	printf("-------------------------------------------------\n");
	int status = 0;
	scanf("%d", &status);
	if (status != 1 && status != 2) {
		ip_select(0);
	}
	switch (status) {
	case 1:
		// 获取本机ip
		struct hostent *phostinfo = gethostbyname("");
		char *p = inet_ntoa(*((struct in_addr *) (*phostinfo->h_addr_list)));
		strcpy(addr, p);
		printf("ip = %s\n", addr);
		break;
	case 2:
		printf("请输入ip\n");
		scanf("%s", addr);
		printf("ip = %s\n", addr);
		break;
	default:
		ip_select(0);
		break;
	}
}

void input_user_name() {
	printf("-------------------------------------------------\n");
	printf("请输入聊天室使用名称\n");
	printf("-------------------------------------------------\n");
	scanf("%s", user_name);
	printf("聊天室使用名称:%s\n", user_name);
}

void init_user_select_windows() {
	initWSA();
	ip_select(1);
	input_user_name();
}

int create_socket_client(int port) {

	// 开始创建socket
	socket_client = socket(AF_INET, SOCK_STREAM, 0);

	//连接socket
	SOCKADDR_IN Addr_Server;
	Addr_Server.sin_family = AF_INET;
	Addr_Server.sin_addr.S_un.S_addr = inet_addr(addr);
	Addr_Server.sin_port = htons(port);

	if (connect(socket_client, (SOCKADDR*) &Addr_Server,
			sizeof(SOCKADDR))==SOCKET_ERROR) {
		printf("connect失败\n");
		printf("程序退出\n");
		WSACleanup();
		closesocket(socket_client);
	}
	// 子线程专门读
	// 主线程专门写
	pthread_t p;
	if (pthread_create(&p, NULL, recv_runable_thread, NULL)) {
		printf("客户端读取线程异常\n");
		printf("程序退出\n");
		return -1;
	}

	// 发送信息
	char buf[BUFF_SIZE];
	strcpy(buf, user_name);
	while (1) {
		if (send(socket_client, buf, BUFF_SIZE, 0) < 0) {
			printf("发送失败\n");
			printf("程序退出\n");
			break;
		}
		memset(buf, 0, BUFF_SIZE);
		scanf("%s", buf);
	}

	return 0;
}

void* recv_runable_thread(void* arg) {
	char buf[BUFF_SIZE] = { 0 };
	while (1) {
		if (recv(socket_client, buf, BUFF_SIZE, 0) < 0) {
			printf("读取失败\n");
			printf("程序退出\n");
			break;
		}
		printf("%s\n", buf);
		memset(buf, 0, BUFF_SIZE);
	}
	return 0;
}

