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

#include <set>
#include <future>
#include "ic_define.h"
#include "message_sender.h"
#include "error_define.h"

MessageSender::MessageSender()
{

}

MessageSender::~MessageSender()
{

}

void MessageSender::parseAndSend(const ic::ServerInfo& info, const ic::IC_MSG& msg)
{
	std::thread sender (&MessageSender::runThread, this, info, msg);
    sender.detach();
}

void MessageSender::runThread(const ic::ServerInfo& info, const ic::IC_MSG& msg)
{
    LOG_INFO("Recv in parseThread: {} {} ", msg.Command, msg.Data);

//	Document document;
//	document.Parse(msg.c_str());
//
//	string strError;
//	ic::Protocol protocol;
//	int nResultCode = getBasicReturnJson(document, protocol);
    int nResultCode = SUCCESS;
	Document sendDocument(kObjectType);
    convertMSGToDocument(msg, sendDocument);
//	Document::AllocatorType& allocator = sendDocument.GetAllocator();
//
//	sendDocument.AddMember(PROTOCOL_TYPE, msg.Type, allocator);
//	sendDocument.AddMember(PROTOCOL_COMMAND, msg.Command, allocator);
//	sendDocument.AddMember(PROTOCOL_SUBCOMMAND, msg.SubCommand, allocator);
//	sendDocument.AddMember(PROTOCOL_ACTION, msg.Action, allocator);
//	sendDocument.AddMember(PROTOCOL_TOKEN, msg.Token, allocator);
//	sendDocument.AddMember(PROTOCOL_FROM, msg.To, allocator);
//	sendDocument.AddMember(PROTOCOL_TO, msg.From, allocator);
//	sendDocument.AddMember(PROTOCOL_DATA, msg.Data, allocator);
//	sendDocument.AddMember(PROTOCOL_RESULTCODE, "", allocator);
//	sendDocument.AddMember(PROTOCOL_ERRORMSG, "", allocator);

//	if (nResultCode != SUCCESS) // this parts will be considered when the result of task have be sent.
//	{
//		sendDocument[PROTOCOL_ERRORMSG].SetString(getErrorCodeToString(nResultCode), allocator);
//		std::string sendString = convertDocumentForSend(sendDocument);
//		if (this->sendData(info, sendString))
//		{
//            LOG_ERROR("sendData failed in parsThread: {}", sendString);
//		}
//		return;
//	}

//	LOG_DEBUG("section {} {} {}", strSection1, strSection2, strSection3);

	std::string strSendString = convertDocumentForSend(sendDocument);
	this->sendData(info, strSendString);
}

bool MessageSender::sendData(const ic::ServerInfo& info, const std::string& strJson)
{
    int nSize = static_cast<int>(strJson.size());
    char cType = static_cast<char>(ic::PACKET_SEPARATOR::PACKETTYPE_JSON);
    int nSendSize = sizeof(int) + 1 + nSize;

    std::lock_guard<std::mutex> lock(bufferMutex_);

    sendBuffer_.reserve(nSendSize);
    sendBuffer_.insert(sendBuffer_.end(), reinterpret_cast<char*>(&nSize), reinterpret_cast<char*>(&nSize) + sizeof(int));
    sendBuffer_.push_back(cType);
    sendBuffer_.insert(sendBuffer_.end(), strJson.begin(), strJson.end());

    int nSend = -1;
    LOG_INFO("SendData clientName : {} clientIP : {} : {}", info.name, info.ip, info.port);
    nSend = send(info.socket, sendBuffer_.data(), sendBuffer_.size(), 0);

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