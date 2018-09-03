/*
 * client_server.cpp
 *
 *  Created on: 2018��9��3��
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

//1.��ʼ��WSA��
//2.��ʼ���׽���
//3.����socket
//4.����

int initWSA() {
	// 2.2 �汾
	WORD wVersion = MAKEWORD(2, 2);

	WSADATA wsaData;

	// ��ʼ����
	if (WSAStartup(wVersion, &wsaData) != 0) {
		printf("��ʼ��WSAʧ��!\n");
		printf("�����˳�!\n");
		return -1;
	}

	// �жϰ汾
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2) {
		printf("WSA�汾����\n");
		printf("�����˳�\n");
		WSACleanup();
		return -1;
	}
	return 0;
}

// ���ڶ�ȡsocket��Ϣ���߳�
void* recv_runable_thread(void* arg);

void ip_select(int flag) {
	system("cls");
	printf("-------------------------------------------------\n");
	if (flag) {
		printf("��ѡ�� \n");
	} else {
		printf("������������������ \n");
	}
	printf("1.ʹ�ñ���ip\n");
	printf("2.�ֶ�����ip\n");
	printf("-------------------------------------------------\n");
	int status = 0;
	scanf("%d", &status);
	if (status != 1 && status != 2) {
		ip_select(0);
	}
	switch (status) {
	case 1:
		// ��ȡ����ip
		struct hostent *phostinfo = gethostbyname("");
		char *p = inet_ntoa(*((struct in_addr *) (*phostinfo->h_addr_list)));
		strcpy(addr, p);
		printf("ip = %s\n", addr);
		break;
	case 2:
		printf("������ip\n");
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
	printf("������������ʹ������\n");
	printf("-------------------------------------------------\n");
	scanf("%s", user_name);
	printf("������ʹ������:%s\n", user_name);
}

void init_user_select_windows() {
	initWSA();
	ip_select(1);
	input_user_name();
}

int create_socket_client(int port) {

	// ��ʼ����socket
	socket_client = socket(AF_INET, SOCK_STREAM, 0);

	//����socket
	SOCKADDR_IN Addr_Server;
	Addr_Server.sin_family = AF_INET;
	Addr_Server.sin_addr.S_un.S_addr = inet_addr(addr);
	Addr_Server.sin_port = htons(port);

	if (connect(socket_client, (SOCKADDR*) &Addr_Server,
			sizeof(SOCKADDR))==SOCKET_ERROR) {
		printf("connectʧ��\n");
		printf("�����˳�\n");
		WSACleanup();
		closesocket(socket_client);
	}
	// ���߳�ר�Ŷ�
	// ���߳�ר��д
	pthread_t p;
	if (pthread_create(&p, NULL, recv_runable_thread, NULL)) {
		printf("�ͻ��˶�ȡ�߳��쳣\n");
		printf("�����˳�\n");
		return -1;
	}

	// ������Ϣ
	char buf[BUFF_SIZE];
	strcpy(buf, user_name);
	while (1) {
		if (send(socket_client, buf, BUFF_SIZE, 0) < 0) {
			printf("����ʧ��\n");
			printf("�����˳�\n");
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
			printf("��ȡʧ��\n");
			printf("�����˳�\n");
			break;
		}
		printf("%s\n", buf);
		memset(buf, 0, BUFF_SIZE);
	}
	return 0;
}

