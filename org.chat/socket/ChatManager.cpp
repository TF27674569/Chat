/*
 * SocketManager.cpp
 *
 *  Created on: 2018��9��3��
 *  Author: TIAN FENG
 */

#include "ChatManager.h"

#include "client/socket_client.h"
#include "server/socket_server.h"

int start_chat_server(int port){
	if(create_socket_server(port)!=0){
		return -1;
	}

	// ��ֹ�������˳�
	while(1);
	return 0;
}



int start_chat_client(int port){
	init_user_select_windows();
	//�����û�socket
	return create_socket_client(port);
}
