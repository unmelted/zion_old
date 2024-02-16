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

using namespace rapidjson;

ICClient::ICClient(const std::string& configContent)
{
    rapidjson::Document doc;
    doc.Parse(configContent.c_str());

    if (doc.HasMember("servers") && doc["servers"].IsArray())
    {
        for (const auto& server : doc["servers"].GetArray())
        {
            std::string name = server["name"].GetString();
            std::string ip = server["ip"].GetString();
            int port = server["port"].GetInt();
            servers_.emplace(name, ServerInfo(ip, port));
            LOG_DEBUG("Add server: {} {} {}", name, ip, port);
        }
    }
}

ICClient::~ICClient()
{
    requestStop();
    for (auto& thread : threads_)
    {
        thread.join();
    }
    int sock_ = 11; //temporary for compile.
    closeSocket(sock_);
}

int ICClient::checkServerAvailability()
{
    int available_cnt = 0;
    for (auto & serverEntry : servers_)
    {
        auto& server = serverEntry.second;

        int testSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (testSocket < 0)
        {
            std::cerr << "Socket creation failed for test." << std::endl;
            server.isAvailable = false;
            continue;
        }

        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(server.ip.c_str());
        serverAddr.sin_port = htons(server.port);

        if (connect(testSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) >= 0)
        {
            server.isAvailable = true;
            close(testSocket);
            available_cnt++;
        }
        else
        {
            std::cerr << "Connection to " << server.ip << ":" << server.port << " failed." << std::endl;
            server.isAvailable = false;
        }
    }

    return available_cnt;
}

bool ICClient::beginSocket(int nPort)
{
    if(servers_.empty())
    {
        LOG_INFO("There is no server to connect.");
    }
    else
    {
        for (auto& server : servers_)
        {
            LOG_DEBUG("Connect to server: {} {} {} ", server.first, server.second.ip, server.second.port);

            if (!connectToServer(server.second))
            {
                LOG_ERROR("Failed to connect to server: {}", server.first);
            }
        }
    }

    return true;
}

bool ICClient::connectToServer(ServerInfo& server)
{
    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_ERROR("Socket creation error.");
        return false;
    }

    server.socket = sock;
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server.port);

    if(inet_pton(AF_INET, server.ip.c_str(), &serverAddr.sin_addr) <= 0)
    {
        LOG_ERROR("Invalid address/ Address not supported: {}", server.ip);
        return false;
    }

    if (connect(server.socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        LOG_ERROR("Failed to connect to the server: {}", server.ip);
        return false;
    }

    threads_.emplace_back(&ICClient::receive, this, server);
    return true;
}

bool ICClient::sendData(const std::string& name, const std::string& data)
{
    auto it = servers_.find(name);
    if (it == servers_.end())
    {
        LOG_ERROR("Server not found: {}", name);
        return false;
    }

    if (send(it->second.socket, data.c_str(), data.size(), 0) == -1)
    {
        LOG_ERROR("Failed to send data to server: {}", name);
        return false;
    }

    return true;
}

void ICClient::receive(ServerInfo server)
{
    while (!stop_)
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(server.socket, buffer, sizeof(buffer), 0);

        if (bytesRead == -1)
        {
            std::cerr << "Failed to receive data from server: " << server.ip << std::endl;
            break;
        }
        else if (bytesRead == 0)
        {
            LOG_ERROR("Connection closed by server: {}", server.ip);
            break;
        }

        std::string received(buffer, bytesRead);
        LOG_INFO("Data received from server: {}", received);
    }

    if(server.socket != -1)
    {
        closeSocket(server.socket);
        server.socket = -1;
        LOG_INFO("Socket closed: {} : {} ", server.ip, server.port);
    }
}

void ICClient::requestStop()
{
    LOG_INFO("Request stop is called");
    stop_ = true;
}

void ICClient::closeSocket(int nSock)
{
//    close(server.second.socket);
//    server.second.socket = -1;
    shutdown(nSock, SHUT_RDWR);
    close(nSock);

}