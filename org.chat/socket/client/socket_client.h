/*
 * client_server.h
 *
 *  Created on: 2018年9月3日
 *  Author: TIAN FENG
 */

#ifndef ORG_CHAT_SOCKET_CLIENT_SOCKET_CLIENT_H_
#define ORG_CHAT_SOCKET_CLIENT_SOCKET_CLIENT_H_

// 初始化用户选择界面
void init_user_select_windows();

//创建用户socket
int create_socket_client(int port);


#endif /* ORG_CHAT_SOCKET_CLIENT_SOCKET_CLIENT_H_ */
