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
#include "socket_abstraction.h"

using namespace rapidjson;

class ICServer : public SocketHandlerAbs
{
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

public : 
    ICServer(const std::string& configContent);
    ~ICServer();

    bool beginSocket(int nPort) override;
    bool sendData(const std::string& name, const std::string& strJson) override;

//    typedef std::function<int(char cSeparator, char* pData, int nDataSize)> callback;
//    callback classifier;
//
//    void setHandler(callback f)
//    {
//        classifier = std::move(f);
//    }

	std::list<std::string> getIPList();
	std::string getLocalCompare(std::string strIP);


private:
    void closeSocket(int nSock) override;
    void runSocket() override;
    int receive(int clnt_sock, char* pRecv, int nSize, int flags);

    void* handle_client(std::unique_ptr<ClientSockThreadData> threadData);
    bool addClient(const std::string& clientIp, int clientSocket, int packetSize);
    void removeClient(const std::string& clientName);


private:
    std::unordered_map<std::string, int> serverPorList_;
    std::mutex sockMutex_;
    std::mutex sendMutex_;

    std::unique_ptr<std::thread> mainSocketThread_;

    std::vector<int> clientSocketsList_;
    std::unordered_map<std::string, struct ClientInfo> clientMap_;

    bool isMainSocketThread_;
    int serverSockets_;
    int serverPorts_;
    std::vector<char> sendBuffer_;
//    std::unique_ptr<MessageResponder> msg_rspndr_;
};

