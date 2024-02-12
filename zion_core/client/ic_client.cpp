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

#include "ic_client.h"

ICClient::ICClient()
{

}
ICClient::~ICClient()
{
    closeConnections();
}

bool ICClient::addServer(const std::string& name, const std::string& serverIP, int serverPort)
{
    servers_.emplace(name, ServerInfo(serverIP, serverPort));
    return true;
}

bool ICClient::connectToServers()
{
    for (auto& server : servers_)
    {
        if (!connectToServer(server.second))
        {
            std::cerr << "Failed to connect to server: " << server.first << std::endl;
            return false;
        }
    }
    return true;
}

bool ICClient::connectToServer(ServerInfo& server)
{
    server.socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server.socket == -1)
    {
        std::cerr << "Failed to create socket for server: " << server.ip << std::endl;
        return false;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(server.ip.c_str());
    serverAddr.sin_port = htons(server.port);

    if (connect(server.socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Failed to connect to the server: " << server.ip << std::endl;
        return false;
    }

    return true;
}

bool ICClient::sendData(const std::string& name, const std::string& data)
{
    auto it = servers_.find(name);
    if (it == servers_.end())
    {
        std::cerr << "Server not found: " << name << std::endl;
        return false;
    }

    if (send(it->second.socket, data.c_str(), data.size(), 0) == -1)
    {
        std::cerr << "Failed to send data to server: " << name << std::endl;
        return false;
    }

    return true;
}

std::string ICClient::receiveData(const std::string& name)
{
    auto it = servers_.find(name);
    if (it == servers_.end())
    {
        std::cerr << "Server not found: " << name << std::endl;
        return "";
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytesRead = recv(it->second.socket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1)
    {
        std::cerr << "Failed to receive data from server: " << name << std::endl;
        return "";
    }

    return std::string(buffer, bytesRead);
}

void ICClient::closeConnections()
{
    for (auto& server: servers_)
    {
        if (server.second.socket != -1)
        {
            close(server.second.socket);
            server.second.socket = -1;
        }
    }
}