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

#include "ic_server.h"

#include <utility>

ICServer::ICServer(ic::ServerInfo info)
{
    isThreadRunning_ = false;
    mainThread_ = nullptr;
    info_= info;

    LOG_DEBUG("ICServer Constructor Server info {} open port : {}", info_.name, info_.port);
}

ICServer::~ICServer()
{
	isThreadRunning_ = false;
    closeServer();

	if (mainThread_ != nullptr)
	{
		mainThread_->join();
		mainThread_ = nullptr;
	}

    LOG_DEBUG("Destroy ICServer Done");
}

void ICServer::closeServer()
{
    for(auto& client : clientMap_)
    {
        closeSocket(client.second.socket);
    }

    closeSocket(info_.port);
}

void ICServer::closeSocket(int nSock)
{
	shutdown(nSock, SHUT_RDWR);
    close(nSock);
}

bool ICServer::beginSocket()
{
    LOG_INFO("beginSocket : port {} ", info_.port);
	isThreadRunning_ = true;
	mainThread_ =  std::make_unique<std::thread>(&ICServer::runSocket, this);

	return true;
}

int ICServer::getSocket()
{
    return info_.socket;
}

void ICServer::runSocket()
{
    struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;

	info_.socket = (int)socket(PF_INET, SOCK_STREAM, 0);
    if (info_.socket == -1)
    {
        LOG_CRITICAL("Socket Creation Error");
        return;
    }

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(info_.port);

    int optval = 1;
    if (setsockopt(info_.socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) == -1)
    {
        LOG_CRITICAL("Socket Opt. SO_REUSEADDR Error");
        return;
    }

	if (::bind(info_.socket, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	{
        LOG_CRITICAL("Socket Bind Error");
		return;
	}
	if (listen(info_.socket, 5) == -1)
	{
        LOG_CRITICAL("Socket Listen Error");
		return;
	}

    LOG_DEBUG("runSocket function will start loop : {} ", info_.port);

	while (isThreadRunning_)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		int client_socket = (int)(accept(info_.socket, (struct sockaddr*)&clnt_adr, (socklen_t*)&clnt_adr_sz));

		if (client_socket <= 0)
			continue;

		infoMutex_.lock();
        clientSocketsList_.push_back(client_socket);
        infoMutex_.unlock();

        std::string client_ip = inet_ntoa(clnt_adr.sin_addr);
		std::string str_ip = getLocalCompare(client_ip);

        if (str_ip.compare(""))
        {
            LOG_ERROR("Invalid client IP : {} local IP : {} only local client is accepted.", client_ip, str_ip);
            continue;
        }

        LOG_INFO("Connected Client IP : {} port {} ", client_ip, info_.port);

        int str_len = 0;
        ic::ProtocolHeader header;

        if ((str_len = receive(client_socket, (char*)&header, sizeof(header), 0)) == 0)
        {
            LOG_WARN("Client disconnected or error occurred: IP {} Socket {}", client_ip, client_socket);
            continue;
        }

        LOG_DEBUG("After add client : IP {} Port {} Socket {}", client_ip, info_.port, client_socket);
        std::string c_name = "Slave_" + std::to_string(client_socket);
        ic::ClientInfo cinfo(c_name, client_ip, info_.port, client_socket);

        if (!addClient(cinfo, header.nSize))
        {
            LOG_ERROR("addClient failed : IP {} Socket {}", client_ip, client_socket);
            continue;
        }

 		std::unique_ptr<ClientSockThreadData> threadData = std::make_unique<ClientSockThreadData>(cinfo, this);
        std::thread(&ICServer::socketThread, this, std::move(threadData)).detach();

        LOG_INFO("runSocket function is end loop. port {} ", info_.port);

	}

    infoMutex_.lock();
    for(int socket : clientSocketsList_)
    {
        closeSocket(socket);
    }
    clientSocketsList_.clear();
    infoMutex_.unlock();
}

void* ICServer::socketThread(std::unique_ptr<ClientSockThreadData> threadData)
{
    LOG_DEBUG("socketThread is started.");

	ICServer* parentThread = threadData->pthis;
    ic::ClientInfo info = threadData->info;

    if(info.port == ic::SERVER_PORT[static_cast<int>(ic::SERVER_TYPE::SERVER_ROBOT_LOGMONITOR)])
    {
        EventManager::setEvent(static_cast<int>(ic::EVENT_ID::EVENT_ID_TCP_LOG_START), (void *)&threadData->info, nullptr);
    }

	while (parentThread->isThreadRunning_)
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

    parentThread->removeClient(threadData->info.socket);
	parentThread->closeSocket(threadData->info.socket);

	return NULL;
}

// addClient() and removeClinet() function handles just clientMap_
// clientSocketsLists_ is handled in runSocket() function
bool ICServer::addClient(const ic::ClientInfo& info, int packetSize)
{
    int str_len = 0;
    std::vector<char> pData;
    pData.resize(packetSize + 1), 0;
    if ((str_len = receive(info.socket, pData.data(), packetSize, 0)) == 0)
    {
        LOG_INFO("Client disconnected or error occurred: Socket {}", info.socket);
        return false;
    }

    if (classifier(info, pData.data(), packetSize) != 1)
    {
        LOG_ERROR("Classifier Error");
        return false;
    }

    std:string message = pData.data();
    Document document;
    document.Parse(message.c_str());

    std::string command = document[PROTOCOL_SECTION2].GetString();
    std::string subCommand = document[PROTOCOL_SECTION3].GetString();

    if (command != "WHOAMI")
    {
        LOG_ERROR("CONNECT Command Error. {} ", document[PROTOCOL_SECTION2].GetString());
        return false;
    }

    std::string clientName = document[PROTOCOL_DATA].GetString();

    infoMutex_.lock();
    clientMap_[clientName] = info;
    infoMutex_.unlock();

    LOG_INFO("Client Connected and addClient success: {} ", clientName);
    return true;
}

void ICServer::removeClient(const int socket)
{
    infoMutex_.lock();
    for (auto it = clientMap_.begin(); it != clientMap_.end();)
    {
        if (it->second.socket == socket)
        {
            it = clientMap_.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
    infoMutex_.unlock();

    LOG_INFO("RemoveClient success: {}", socket);
    for (auto it = clientMap_.begin(); it != clientMap_.end();)
    {
        LOG_INFO("Current clientMap_ name : {} ", it->first);
        LOG_INFO("Current clientMap_ ip : {} ", it->second.ip);
    }
}
