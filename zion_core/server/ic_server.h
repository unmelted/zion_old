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
#include "server_message.h"

using namespace rapidjson;

class ICServer : public SocketHandlerAbs
{
private:
    // this struct is for storing the data for thread
    // it will be created when the client is connected and
    // destroyed when the client is disconnected
    struct ClientSockThreadData
    {
        ic::ClientInfo info;
        ICServer* pthis;
        ClientSockThreadData(const ic::ClientInfo& cinfo, ICServer* server)
                : info(cinfo), pthis(server)
        {
            std::cout << "Client info : " << cinfo.ip << " " << cinfo.port << std::endl;
        }
    };

public : 
    ICServer(ic::ServerInfo info);
    ~ICServer();

    bool beginSocket() override;
    int getSocket() override;

private:
    void runSocket() override;
    void closeSocket(int nSock) override;
    void closeServer();

    void* socketThread(std::unique_ptr<ClientSockThreadData> threadData);
    int receive(int clnt_sock, char* pRecv, int nSize, int flags);
    bool addClient(const ic::ClientInfo& info, int packetSize);
    void removeClient(const std::string& clientName);

    std::list<std::string> getIPList();
    std::string getLocalCompare(std::string strIP);

private:
    bool isMainSocketThread_;
    std::mutex infoMutex_;
    std::unique_ptr<std::thread> mainSocketThread_;
    std::unordered_map<std::string, ic::ClientInfo> clientMap_;

    ic::ServerInfo info_;

    std::vector<int> clientSocketsList_;

};

