/*
 * Cilent.h
 *
 *  Created on: 2018年9月3日
 *  Author: TIAN FENG
 */


#include <winsock.h>
#include <stdio.h>

struct CLIENT{

	CLIENT* next;
	char name[20];
	SOCKET client_socket;

	// 用来标记是哪个用户，也用来处理哪个线程
	int flag;
};
