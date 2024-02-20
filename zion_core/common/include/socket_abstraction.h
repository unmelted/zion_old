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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/16.
 *
 */

#pragma once
#include <sys/socket.h> // for socket(), bind(), connect()
#include <arpa/inet.h> // for sockaddr_in, inet_ntoa()
#include <unistd.h> // for close()
#include <stdlib.h> // for exit()
#include <sys/ioctl.h>
#include <net/if.h>
#include <list>
#include "ic_define.h"
#include "event_manager.h"

class SocketHandlerAbs
{

public:
    virtual bool beginSocket() = 0;
    virtual int getSocket() = 0;

    typedef std::function<int(const ic::ServerInfo& info, char* pData, int nDataSize)> callback;
    callback classifier;

    void setHandler(callback f)
    {
        classifier = std::move(f);
    }

protected :

    virtual void runSocket() = 0;
    virtual void closeSocket(int nSock) = 0;
    int receive(int clnt_sock, char* pRecv, int nSize, int flags);
    std::list<std::string> getIPList();
    std::string getLocalCompare(std::string ip);

protected:
    bool isThreadRunning_ = false;
    std::unique_ptr<std::thread> mainThread_;
    ic::ServerInfo info_;

};