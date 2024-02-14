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
#include <list>
#include <sys/socket.h> // for socket(), bind(), connect()
#include <arpa/inet.h> // for sockaddr_in, inet_ntoa()
#include <unistd.h> // for close()
#include <stdlib.h> // for exit()
# include <sys/ioctl.h>
# include <net/if.h>
#include "ic_define.h"

using namespace rapidjson;

class ICServer
{
public : 
    ICServer(int type);
    ~ICServer();

    bool beginSocket(int nPort, int nType);
    bool sendData(const std::string& clientName, std::string strJson);

    typedef std::function<int(char cSeparator, char* pData, int nDataSize)> callback;
    callback classifier;

    void setHandler(callback f)
    {
        classifier = std::move(f);
    }

	std::list<std::string> getIPList();
	std::string getLocalCompare(std::string strIP);


private:
    // this struct is for handling with client_name
    // a member in unordered_map
    struct ClientInfo
    {
        std::string clientIp;
        int clientSocket;
    };

    // this struct is for storing the data for thread
    // it will be created when the client is connected and
    // destroyed when the client is disconnected
    struct ClientSockThreadData
    {
        std::string clientIp;
        ICServer* pthis;
        int socket;
    };

    bool addClient(const std::string& clientIp, int clientSocket, int packetSize);
    void removeClient(const std::string& clientName);
    void closeSocket(int nSock);
    void runSocket();

    void* handle_client(std::unique_ptr<ClientSockThreadData> threadData);
    int receive(int clnt_sock, char* pRecv, int nSize, int flags);


private:

    std::mutex sockMutex_;
    std::mutex sendMutex_;

    std::unique_ptr<std::thread> mainSocketThread_;

    std::vector<int> clientSocketsList_;
    std::unordered_map<std::string, struct ClientInfo> clientMap_;

    bool isMainSocketThread_;
    int serverSockets_;
    int serverPorts_;
    std::vector<char> sendBuffer_;
};

