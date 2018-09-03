/*
 * socketserver.cpp
 *
 *  Created on: 2018��9��3��
 *  Author: TIAN FENG
 */

#include "socket_server.h"
#include <winsock.h>
#include <stdio.h>
#include <pthread.h>
#include "Cilent.h"

// ���������
#define BACK_LOG 50
#define BUFF_SIZE 200

// 1. ��ʼ��WSA��     WSAStartup
// 2. ����socket�׽��� socket
// 3. �󶨱��ض˿� bind
// 4. ��Ϊ����״̬ linsten
// 5. �������ӿͻ���socket  accept
// 6. ���� recv or send
// 7. �ر�socket �ͷ�WSA��Դ  closesocket WSAClearn


// �����ָ��ͷ
struct CLIENT * pHeader = NULL;

// �������󶨵�socket
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

// �����߳�
void* accept_runable(void* args);

// ��ȡ�߳�
void* recv_runable(void* args);

// ת����Ϣ
void dispatch_message(CLIENT* client_from, char* message);

int create_socket_server(int port) {
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

	// ��ʼ����socket
	socket_server = socket(AF_INET, SOCK_STREAM, 0);

	// �󶨶˿�
	SOCKADDR_IN Server_addr;
	Server_addr.sin_family = AF_INET;
	Server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // ���ض˿�
	Server_addr.sin_port = htons(port);
	if (bind(socket_server, (SOCKADDR*) &Server_addr,
			sizeof(SOCKADDR))==SOCKET_ERROR) {
		printf("WSA�汾����\n");
		printf("�����˳�\n");
		WSACleanup();
		return -1;
	}
	// ��ȡ����ip
	struct hostent *phostinfo = gethostbyname("");
	char *p = inet_ntoa(*((struct in_addr *) (*phostinfo->h_addr_list)));
	// ��ӡ��ַ�˿�
	printf("������->%s:%d\n", p, port);

	// ����״̬
	if (listen(socket_server, BACK_LOG) < 0) {
		printf("����ʧ��\n");
		printf("�����˳�\n");
		WSACleanup();
		return -1;
	}

	// ���ӿͻ��� ��ͻ�����Ҫ�̲߳���
	pthread_t accept_p;
	if (pthread_create(&accept_p, NULL, accept_runable, NULL)) {
		printf("���ӿͻ����߳��쳣\n");
		printf("�����˳�\n");
		return -1;
	}
	return 0;
}

int flag = 0;

// ���ӿͻ���
void* accept_runable(void* args) {
	SOCKADDR_IN Client_addr;
	int len = sizeof(SOCKADDR);

	while (1) {
		SOCKET socket_client = accept(socket_server, (SOCKADDR*) &Client_addr,
				&len);
		if (socket_client == SOCKET_ERROR) {
			printf("����ʧ��\n");
			continue;
		}

		// �����ڴ�ռ�
		CLIENT* _client = (CLIENT*) malloc(sizeof(CLIENT));
		_client->next = pHeader;
		_client->client_socket = socket_client;
		// ��Ҫ�����߳�ֱ��ȡ
		pthread_t client_t;
		pHeader = _client;
		if (pthread_create(&client_t, NULL, recv_runable, _client)) {
			printf("�ͻ��˶�ȡ�߳��쳣\n");
		}
	}

	return 0;
}


// ��ȡ�߳���Ϣ
void* recv_runable(void* args) {
	printf("�пͻ������ӳɹ�\n");
	CLIENT* _client = (CLIENT*) args;
	strcpy(_client->name,"");
	_client->flag = flag++;
	char buf[BUFF_SIZE] = { 0 };
	char msg[200]={0};
	// ��ȡ��Ϣ
	while (recv(_client->client_socket, buf, BUFF_SIZE, 0) >= 0) {
		// ת����Ϣ Ĭ�ϵ�һ��Ϊ�û���
		if(strcmp(_client->name,"")==0){
			strcpy(_client->name,buf);
			strcat(buf,"����������");
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
	printf("��ȡʧ��\n");
	return 0;
}



// ת����Ϣ
void dispatch_message(CLIENT* client_form, char* message) {
	CLIENT* temp = pHeader;
	do {
		if (temp->flag != client_form->flag) {
			printf("������ת��->%s->%s\n",temp->name,message);
			if (send(temp->client_socket, message, BUFF_SIZE, 0) < 0) {
				printf("ת��ʧ��\n");
//				remove(temp);
				closesocket(temp->client_socket);
			}
		}
	} while ((temp = temp->next) != NULL);
}
