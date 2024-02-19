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
//    LOG_INFO("CloseServer is called");
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
        if(isRcvThreadRunning)
        {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            LOG_INFO("Socket waiting recv.");
            continue;
        }

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
            LOG_WARN("Can't to connect to the server: {}", info_.ip);
            closeSocket(info_.socket);

            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }
        else
        {
            LOG_INFO("Successfully connected to server. Starting receive thread.");
            isRcvThreadRunning = true;
            std::unique_ptr<ServerSockThreadData> threadData = std::make_unique<ServerSockThreadData>(info_, this);
            std::thread(&ICClient::receiveThread, this, std::move(threadData)).detach();
        }
    }
}

void ICClient::receiveThread(std::unique_ptr<ServerSockThreadData> threadData)
{
    LOG_DEBUG("receiveThread is started.");

    ICClient* parentThread = threadData->pthis;

    while (isRcvThreadRunning)
    {
        int str_len = 0;
        int nPacketSize = 0;
        ic::ProtocolHeader header;

        if ((str_len = parentThread->receive(threadData->info.socket, (char*)&header, sizeof(header), 0)) == 0)
        {
            LOG_INFO("Client disconnected or error occurred: Socket {}", threadData->info.socket);
            break;
        }

        if (str_len < sizeof(ic::ProtocolHeader))
            continue;

        nPacketSize = header.nSize;
        if (nPacketSize < 1 || nPacketSize > 5000000 || header.cSeparator >= (int)ic::PACKET_SEPARATOR::PACKETTYPE_SIZE)
        {
            LOG_ERROR("Invalid Header Packet {} Separator {}", nPacketSize, header.cSeparator);
            continue;
        }

        std::vector<char> pData;
        if (nPacketSize >= 0)
        {
            pData.resize(nPacketSize + 1), 0;
            if ((str_len = parentThread->receive(threadData->info.socket, pData.data(), nPacketSize, 0)) == 0)
            {
                LOG_ERROR("received Patcket size Error {} ", str_len);
                continue;
            }
        }

        if( header.cSeparator != (char)ic::PACKET_SEPARATOR::PACKETTYPE_JSON)
        {
            LOG_ERROR("validateMsg cSeparator != (char)ic::PACKET_SEPARATOR::PACKETTYPE_JSON");
            continue;
        }

        int nErrorCode = parentThread->classifier(threadData->info, pData.data(), nPacketSize);
        if (nErrorCode != (int)ErrorCommon::COMMON_ERR_NONE)
        {
            LOG_ERROR("Classifier Error {} ", nErrorCode);
        }
    }

    if(info_.socket != -1)
    {
        closeSocket(info_.socket);
        info_.socket = -1;
        LOG_INFO("Socket closed: {} : {} ", info_.ip, info_.port);
    }

    parentThread->isRcvThreadRunning = false;
}
