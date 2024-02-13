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
    /*
     * std::ifstream file(configFilePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    {
      "servers": [
        {
          "name": "Server1",
          "ip": "192.168.1.100",
          "port": 8080
        },
        {
          "name": "Server2",
          "ip": "192.168.1.101",
          "port": 8081
        }
      ]
    }
     */

    rapidjson::Document doc;
    doc.Parse(configContent.c_str());

    if (doc.HasMember("servers") && doc["servers"].IsArray()) {
        for (const auto& server : doc["servers"].GetArray()) {
            addServer(server["name"].GetString(), server["ip"].GetString(), server["port"].GetInt());
        }
    }

    if(servers_.empty())
    {

    }
    else
    {
        checkServerAvailability();
    }

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

void ICClient::checkServerAvailability()
{
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
        }
        else
        {
            std::cerr << "Connection to " << server.ip << ":" << server.port << " failed." << std::endl;
            server.isAvailable = false;
        }
    }
}

int ICClient::initialize()
{
    return 0;

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