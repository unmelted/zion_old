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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/12
 *
 */

#pragma once
#include "socket_abstraction.h"

using namespace rapidjson;

class ICClient : public SocketHandlerAbs
{
private :
    struct ServerSockThreadData
    {
        ic::ServerInfo info;
        ICClient* pthis;
        ServerSockThreadData(const ic::ServerInfo& cinfo, ICClient* server)
                : info(cinfo), pthis(server)
        {
            std::cout << "Client info : " << cinfo.ip << " " << cinfo.port << std::endl;
        }
    };

public:
    ICClient(const ic::ServerInfo& info);
    ~ICClient();

    bool beginSocket() override;
    int getSocket() override;

private :
    void runSocket() override;
    void closeSocket(int nSock) override;
    void closeServer();
    void receiveThread(std::unique_ptr<ServerSockThreadData> threadData);

private :
    std::unique_ptr<std::thread> rcvThread_;
    bool isRcvThreadRunning = false;

    std::function<void()> callback;
};
