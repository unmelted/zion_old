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


ICServer::ICServer()
{
	isMainSocketThread_ = false;
	mainSocketThread_ = nullptr;
	serverSockets_ = 0;
    srverPorts_ = 0;

}

ICServer::~ICServer()
{
	isMainSocketThread_ = false;

    for(int socket : clientSocketsList_)
    {
        closeSocket(socket);
    }

	closeSocket(serverSockets_);

	if (mainSocketThread_ != nullptr)
	{
		mainSocketThread_->join();
		mainSocketThread_ = nullptr;
	}

}

void ICServer::closeSocket(int nSock)
{
	//close(nSock);
	shutdown(nSock, SHUT_RDWR);
    
}

bool ICServer::beginSocket(int nPort, int nType)
{
	if (isMainSocketThread_)
		return false;

	srverPorts_ = nPort;
	isMainSocketThread_ = true;
	mainSocketThread_ =  std::make_unique<std::thread>(&ICServer::runSocketThread, this, this);

	return true;
}


void* ICServer::runSocketThread(void* arg)
{
	ICServer* pSocketMgr = (ICServer*)arg;
	((ICServer*)pSocketMgr)->runSocket();

	//InfoL << "beginSocketThread end";
	return 0;
}

void ICServer::runSocket()
{
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;

	serverSockets_ = (int)socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(srverPorts_);

	if (::bind(serverSockets_, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	{
		//ErrorL << "bind() error";
		return;
	}
	if (listen(serverSockets_, 5) == -1)
	{
		//ErrorL << "listen() error";
		return;
	}


	while (isMainSocketThread_)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		int clientSocket = (int)(accept(serverSockets_, (struct sockaddr*)&clnt_adr, (socklen_t*)&clnt_adr_sz));

		if (clientSocket <= 0)
			continue;

		sockMutex_.lock();
        clientSocketsList_.push_back(clientSocket);
        sockMutex_.unlock();

        std::string clientIP = inet_ntoa(clnt_adr.sin_addr);
		std::string strIP = getLocalCompare(clientIP);

        if (strIP.compare(""))
            continue;

        LOG_INFO("Connected Client IP : {} Local IP Address : {}", clientIP, strIP);
        addClient("name_temp", clientIP, clientSocket);

		std::unique_ptr<ClientSockThreadData> threadData = std::make_unique<ClientSockThreadData>();
		threadData->pthis = this;
		threadData->clientIp = clientIP;
		threadData->socket = clientSocket;

        std::thread(&ICServer::handle_client, this, std::move(threadData)).detach();

	}

    sockMutex_.lock();
    for(int socket : clientSocketsList_)
    {
        closeSocket(socket);
    }
    clientSocketsList_.clear();
    sockMutex_.unlock();
}

void* ICServer::handle_client(std::unique_ptr<ClientSockThreadData> threadData)
{
	int clnt_sock = threadData->socket;
	std::string clnt_IP = threadData->clientIp;
	ICServer* pSocketMgr = (ICServer*)threadData->pthis;

	while (pSocketMgr->isMainSocketThread_)
	{
		int str_len = 0;
		int nPacketSize = 0;
		ic::ProtocolHeader header;

		if ((str_len = pSocketMgr->receive(clnt_sock, (char*)&header, sizeof(header), 0)) == 0)
		{
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
			if ((str_len = pSocketMgr->receive(clnt_sock, pData.data(), nPacketSize, 0)) == 0)
			{
				break;
			}
		}

		if (pSocketMgr->classifier != 0)
		{
			int nErrorCode = pSocketMgr->classifier(header.cSeparator, pData.data(), nPacketSize);

			//if (nErrorCode != MTD_PROTOCOL_OK)
			//{
			//	std::string strMessage = GetErrorMessage(nErrorCode);
			//	char* pMessage = new char[strMessage.length() + 200];

			//	sprintf(pMessage, "{ \"ResultCode\":\"%d\",\"ErrorMsg\" : \"%s\" }", nErrorCode, strMessage.c_str());

			//	std::string strErrorReturn = pMessage;
			//	if (pMessage != 0)
			//		delete[] pMessage;

			//	pSocketMgr->sendData(strErrorReturn);
			//}
		}

	}

	pSocketMgr->closeSocket(clnt_sock);

	return NULL;
}

bool ICServer::sendData(const std::string& clientName, std::string strJson)
{
    int nSize = static_cast<int>(strJson.size());
    char cType = static_cast<char>(ic::PACKET_SEPARATOR::PACKETTYPE_JSON);
    int nSendSize = sizeof(int) + 1 + nSize;

    sendMutex_.lock();
    sendBuffer_.reserve(nSendSize);

    sendBuffer_.insert(sendBuffer_.end(), reinterpret_cast<char*>(&nSize), reinterpret_cast<char*>(&nSize) + sizeof(int));
    sendBuffer_.push_back(cType);
    sendBuffer_.insert(sendBuffer_.end(), strJson.begin(), strJson.end());

    sockMutex_.lock();
    int nSend = -1;
    auto clientInfoIterator = clientMap_.find(clientName);
    if (clientInfoIterator != clientMap_.end())
    {
        ClientInfo& clientInfo = clientInfoIterator->second;
        nSend = send(clientInfo.clientSocket, sendBuffer_.data(), sendBuffer_.size(), 0);
    }
    else
    {
        // todo : add error handling when client is not found
    }

	sockMutex_.unlock();
	sendMutex_.unlock();
    sendBuffer_.clear();
    std::vector<char>().swap(sendBuffer_);

	if (nSend != nSendSize)
	{
        LOG_ERROR("Send Fail nSend != nSendSize {} {} ", nSend, nSendSize);
		return false;
	}
    else
    {
        LOG_INFO("Send Success nSend == nSendSize {} {} ", nSend, nSendSize);
        return true;
    }

	return true;

}

void ICServer::addClient(const std::string& clientName, const std::string& clientIp, int clientSocket)
{
    sockMutex_.lock();

    struct ClientInfo clientInfo;
    clientInfo.clientIp = clientIp;
    clientInfo.clientSocket = clientSocket;

    clientMap_[clientName] = clientInfo;

    sockMutex_.unlock();
}

int ICServer::receive(int clnt_sock, char* pRecv, int nSize, int flags)
{
	int nReclen = 0, nTotalRecvSize = 0, nRemainSize = nSize;
	int nRecvSize = 1024;
	if (nRecvSize > nSize)
		nRecvSize = nSize;
	while (1)
	{
		if ((nReclen = recv(clnt_sock, pRecv, nRecvSize, MSG_NOSIGNAL)) == 0)
			return 0;

		if (nReclen <= 0)
			break;

		pRecv += nReclen;
		nTotalRecvSize += nReclen;
        nRemainSize -= nReclen;
		if (nRemainSize <= 0)
			break;

		if (nRecvSize > nRemainSize)
			nRecvSize = nRemainSize;
	}

	return nTotalRecvSize;
}

std::list<std::string> ICServer::getIPList()
{
	std::list<std::string> lstIPAddress;

	int             sock, nRet, nFamily = PF_INET;
	size_t          nNIC;
	const size_t    nMaxNIC = 256;
	struct ifconf   ifc;
	struct ifreq    ifr[nMaxNIC];
	struct sockaddr* pAddr(NULL);
	sock = socket(nFamily, SOCK_STREAM, 0);
	if (sock == -1)
		return lstIPAddress;

	ifc.ifc_len = sizeof(ifr);
	ifc.ifc_ifcu.ifcu_req = ifr;

	nRet = ioctl(sock, SIOCGIFCONF, &ifc);
	if (nRet == -1)
		return lstIPAddress;

	close(sock);
	nNIC = ifc.ifc_len / sizeof(struct ifreq);
	for (size_t i = 0; i < nNIC; i++)
	{
		if (nFamily == ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr.sa_family)
		{
			pAddr = (&ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr);
			sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(pAddr);
			lstIPAddress.push_back(inet_ntoa(sin->sin_addr));
		}
	}

	return lstIPAddress;
}

std::string ICServer::getLocalCompare(std::string strIP)
{
	std::string _strIP = "";

	std::list<std::string> _list = getIPList();
	std::list<std::string>::iterator iter;

	std::string strC = strIP.substr(0, strIP.rfind('.'));

	for (iter = _list.begin(); iter != _list.end(); iter++)
	{
		std::string strData = *iter;

		std::string strDiff = strData.substr(0, strData.rfind('.'));

		if (strDiff == strC)
		{
			_strIP = strData;
			break;
		}
	}

	return _strIP;
}
