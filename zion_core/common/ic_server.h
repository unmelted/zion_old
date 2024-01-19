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
#include <unordered_map>
#include <sys/socket.h> // for socket(), bind(), connect()
#include <arpa/inet.h> // for sockaddr_in, inet_ntoa()
#include <unistd.h> // for close()
#include <stdlib.h> // for exit()
# include <sys/ioctl.h>
# include <net/if.h>
#include "ic_define.h"

typedef int SOCKET;
#define NET_INVALID_SOCKET	-1
#define NET_SOCKET_ERROR -1

struct ClientInfo
{
    std::string clientIp;
    int clientSocket;
};

struct ClientSockThreadData
{
    std::string clientIp;
    void* pthis;
    int socket;
};

class ICServer
{
public : 
    ICServer();
    ~ICServer();

    bool beginSocket(int nPort, int nType);
    bool sendData(const std::string& clientName, std::string strJson);
    void addClient(const std::string& clientName, const std::string& clientIp, int clientSocket);

    typedef std::function<int(char cSeparator, char* pData, int nDataSize)> callback;
    callback classifier;

    void setHandler(callback f)
    {
        classifier = std::move(f);
    }

	std::list<std::string> getIPList();
	std::string getLocalCompare(std::string strIP);


private:
    void closeSocket(int nSock);
    void runSocket();

    void* runSocketThread(void* arg);
    void* handle_client(std::unique_ptr<ClientSockThreadData> threadData);
    int receive(int clnt_sock, char* pRecv, int nSize, int flags);


    std::mutex sockMutex_;
    std::mutex sendMutex_;

    std::unique_ptr<std::thread> mainSocketThread_;

    std::vector<int> clientSocketsList_;
    std::unordered_map<std::string, struct ClientInfo> clientMap_;

    bool isMainSocketThread_;
    int serverSockets_;
    int srverPorts_;
    std::vector<char> sendBuffer_;
};

