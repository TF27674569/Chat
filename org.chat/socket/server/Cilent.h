/*
 * Cilent.h
 *
 *  Created on: 2018��9��3��
 *  Author: TIAN FENG
 */


#include <winsock.h>
#include <stdio.h>

struct CLIENT{

	CLIENT* next;
	char name[20];
	SOCKET client_socket;

	// ����������ĸ��û���Ҳ���������ĸ��߳�
	int flag;
};
