/*
 * Application.cpp
 *
 *  Created on: 2018Äê9ÔÂ3ÈÕ
 *  Author: TIAN FENG
 */

#define PORT 8888

#include <stdio.h>
#include <stdlib.h>
#include "socket/ChatManager.h"

int main(int argc, char **argv) {
	start_chat_server(PORT);
//	start_chat_client(PORT);
	system("pause");
	return 0;
}

