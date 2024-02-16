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
private:

public:
    ICClient(const std::string& configContent);
    ~ICClient();

    bool beginSocket(int nPort) override;
    bool sendData(const std::string& name, const std::string& strJson) override;


private :
    void runSocket() override;
    void closeSocket(int nSock) override;
    void receive(ic::ServerInfo server) ;

    int checkServerAvailability();
    bool connectToServer(ic::ServerInfo& server);
    void requestStop();


private :
    std::unordered_map<std::string, ic::ServerInfo> servers_;
    std::vector<std::thread> threads_;
    std::atomic<bool> stop_ = false;

};
