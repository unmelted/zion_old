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

ICClient::ICClient(const ic::ServerInfo& info)
{
    isThreadRunning_ = false;
    mainThread_ = nullptr;
    info_= info;

    LOG_DEBUG("ICClient Constructor Server info: {} {} {}", info.name, info.ip, info.port);
}

ICClient::~ICClient()
{
    isThreadRunning_ = false;
    closeServer();

    if (mainThread_ != nullptr)
    {
        mainThread_->join();
    }
    if (rcvThread_ != nullptr)
    {
        rcvThread_->join();
    }
}

void ICClient::closeServer()
{
    closeSocket(info_.socket);
}

void ICClient::closeSocket(int nSock)
{
    LOG_INFO("CloseServer is called");
    shutdown(nSock, SHUT_RDWR);
    close(nSock);

}

bool ICClient::beginSocket()
{
    LOG_INFO("beginSocket : port {} ", info_.port);
    isThreadRunning_ = true;
    mainThread_ =  std::make_unique<std::thread>(&ICClient::runSocket, this);

    return true;
}

int ICClient::getSocket()
{
    return info_.socket;
}

void ICClient::runSocket()
{
    while (isThreadRunning_)
    {
        int sock = 0;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            LOG_ERROR("Socket creation error.");
            continue;
        }

        info_.socket = sock;
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(info_.port);

        if (inet_pton(AF_INET, info_.ip.c_str(), &serverAddr.sin_addr) <= 0)
        {
            LOG_ERROR("Invalid address/ Address not supported: {}", info_.ip);
            closeSocket(info_.socket);
            continue;
        }

        if (connect(info_.socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        {
            LOG_ERROR("Failed to connect to the server: {}", info_.ip);
            closeSocket(info_.socket);

            std::this_thread::sleep_for(std::chrono::seconds(3));
            continue;
        }
        else
        {
            LOG_INFO("Successfully connected to server. Starting receive thread.");
            rcvThread_ = std::make_unique<std::thread>(&ICClient::receiveThread, this);
            break;
        }
    }

    if (mainThread_ && mainThread_->joinable())
    {
        mainThread_->join();
        mainThread_.reset();
    }
}

void ICClient::receiveThread()
{
    while (isThreadRunning_)
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(info_.socket, buffer, sizeof(buffer), 0);

        if (bytesRead == -1)
        {
            LOG_ERROR("Failed to receive data from server");
            isThreadRunning_ = false;
            break;
        }
        else if (bytesRead == 0)
        {
            LOG_WARN("Connection closed by server {} : {} ", info_.ip, info_.port);
            isThreadRunning_ = false;
            break;
        }

//        std::string received(buffer, bytesRead);
//        LOG_INFO("Data received from server: {}", received);
    }

    if(info_.socket != -1)
    {
        closeSocket(info_.socket);
        info_.socket = -1;
        LOG_INFO("Socket closed: {} : {} ", info_.ip, info_.port);
    }

    if (rcvThread_ && rcvThread_->joinable())
    {
        rcvThread_->join();
        rcvThread_.reset();
    }

    reconnect();
}

void ICClient::reconnect()
{
    isThreadRunning_ = true;
    mainThread_ =  std::make_unique<std::thread>(&ICClient::runSocket, this);
}