//
// Created by Kelly Ma on 1/13/24.
//

#ifndef TCP_TEST_COMMON_H
#define TCP_TEST_COMMON_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


std::mutex m_Sockmutx;
std::mutex m_SendMutex;

bool bMainSocketThread;
std::thread* m_mainSocketThread;
std::thread* m_clientReceiveThread;

int m_ServerSockets;
int m_ClientSockets;
int m_ServerPorts;
int m_nSockType;
int m_nSendBufferSize;
char* m_pSendBuffer;

std::string m_strIP;
std::string m_strClientIP;

#endif //TCP_TEST_COMMON_H