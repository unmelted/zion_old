/*
 * LIVSMED CONFIDENTIAL
 *
 * Copyright (c) 2024 LIVSMED, INC.
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains the property
 * of LIVSMED and its suppliers, if any. The intellectual and technical concepts
 * contained herein are proprietary to LIVSMED and its suppliers and may be
 * covered by S.Korea and Foreign Patents, patents in process, and are
 * protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material is
 * strictly forbidden unless prior written permission is obtained from LIVSMED.
 *
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/05.
 *
 */

#pragma once
#include "iostream"
#include <functional>
#include <cstring>
#include <mutex>
#include <thread>
#include <list>
#include <sys/socket.h> // for socket(), bind(), connect()
#include <arpa/inet.h> // for sockaddr_in, inet_ntoa()
#include <unistd.h> // for close()
#include <stdlib.h> // for exit()
# include <sys/ioctl.h>
# include <net/if.h>
#include "ics_define.h"

typedef int SOCKET;
#define NET_INVALID_SOCKET	-1
#define NET_SOCKET_ERROR -1

struct ClientSockThreadData
{
    void* pthis;
    int nType;
    int nSocket;
    std::string strClientIP;
};

class ICServer
{
public : 
    ICServer();
    ~ICServer();

    bool beginSocket(int nPort, int nType);
    //void sendData(char* pData, int nSize);
    bool sendData(std::string strJson);

    bool isConnected();

    typedef std::function<int(char cSeparator, char* pData, int nDataSize)> callback;
    callback m_onClassfication;

    void setHandler(callback f) {
        m_onClassfication = std::move(f);
    }

	std::list<std::string> getIPList();
	std::string getLocalCompare(std::string strIP);
	std::string getLocalAddress() { return m_strIP; }

private:
    void closeSocket(int nSock);
    void runSocket();

    void* runSocketThread(void* arg);
    void* handle_client(void* arg);
    int receive(int clnt_sock, char* pRecv, int nSize, int flags);


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

};

