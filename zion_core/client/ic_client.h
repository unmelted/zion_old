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
#include <sys/socket.h> // for socket(), bind(), connect()
#include <arpa/inet.h> // for sockaddr_in, inet_ntoa()
#include <unistd.h> // for close()
#include <stdlib.h> // for exit()
#include <net/if.h>
#include "ic_define.h"

using namespace rapidjson;

class ICClient {
public:
    ICClient(const std::string& configContent);
    ~ICClient();

    int initialize();
    void checkServerAvailability();
    bool addServer(const std::string& name, const std::string& serverIP, int serverPort);
    bool connectToServers();
    bool sendData(const std::string& name, const std::string& data);
    std::string receiveData(const std::string& name);
    void closeConnections();

private:
    struct ServerInfo {
        std::string ip;
        int port;
        int socket;
        bool isAvailable;
        ServerInfo(const std::string& ip, int port) : ip(ip), port(port), socket(-1), isAvailable(false) {}
    };

    std::unordered_map<std::string, ServerInfo> servers_;

    bool connectToServer(ServerInfo& server);
};
