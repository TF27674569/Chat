/*
 * socketserver.h
 *
 *  Created on: 2018��9��3��
 *  Author: TIAN FENG
 */

// 1. ��ʼ��WSA��     WSAStartup
// 2. ����socket�׽��� socket
// 3. �󶨱��ض˿� bind
// 4. ��Ϊ����״̬ linsten
// 5. �������ӿͻ���socket  accept
// 6. ���� recv or send
// 7. �ر�socket �ͷ�WSA��Դ  closesocket WSAClearn

int create_socket_server(int port);
