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

#include "message_responder.h"
#include "error_manager.h"
#include <set>
#include <future>
#include "ic_util.h"
#include "logger.h"


MessageResponder::MessageResponder()
{

}

MessageResponder::~MessageResponder()
{

}

void MessageResponder::parseAndSendResponse(std::string strMessage)
{
	std::thread th(&MessageResponder::parseThread, this, this, strMessage);

	th.join();
}

bool MessageResponder::isThreadStop()
{
	return isThreadStop_;
}

void MessageResponder::parseThread(void* param, std::string strMessage)
{
	MessageResponder* pMain = (MessageResponder*)param;
	if (pMain->isThreadStop())
		return;

    LOG_INFO("Recv in parseThread: {}", strMessage);

	Document document;
	document.Parse(strMessage.c_str());

	string strError;
	ic::Protocol protocol;
	int nResultCode = getBasicReturnJson(document, protocol);
	Document sendDocument(kObjectType);
	Document::AllocatorType& allocator = sendDocument.GetAllocator();

	sendDocument.AddMember(PROTOCOL_SECTION1, protocol.Type, allocator);
	sendDocument.AddMember(PROTOCOL_SECTION2, protocol.Command, allocator);
	sendDocument.AddMember(PROTOCOL_SECTION3, protocol.SubCommand, allocator);
	sendDocument.AddMember(PROTOCOL_ACTION, protocol.Action, allocator);
	sendDocument.AddMember(PROTOCOL_TOKEN, protocol.Token, allocator);
	sendDocument.AddMember(PROTOCOL_FROM, protocol.To, allocator);
	sendDocument.AddMember(PROTOCOL_TO, protocol.From, allocator);
	sendDocument.AddMember(PROTOCOL_DATA, protocol.Data, allocator);
	sendDocument.AddMember(PROTOCOL_RESULTCODE, nResultCode, allocator);
	sendDocument.AddMember(PROTOCOL_ERRORMSG, "", allocator);

	if (nResultCode != SUCCESS)
	{
		sendDocument[PROTOCOL_ERRORMSG].SetString(getErrorCodeToString(nResultCode), allocator);
		std::string sendString = getDocumentToString(sendDocument);
		if (pMain->icServer_->sendData(protocol.From, sendString))
		{
            LOG_ERROR("sendData failed in parsThread: {}", sendString);
		}
		return;
	}

//	LOG_DEBUG("section {} {} {}", strSection1, strSection2, strSection3);

	std::string strSendString = getDocumentToString(sendDocument);
	pMain->icServer_->sendData(protocol.From, strSendString);
}

void MessageResponder::setICServer(std::shared_ptr<ICServer> icServer)
{
	icServer_ = icServer;
}

std::string MessageResponder::getDocumentToString(Document& document)
{
	StringBuffer strbuf;
	strbuf.Clear();
	PrettyWriter<StringBuffer> writer(strbuf);
	document.Accept(writer);
	std::string ownShipRadarString = strbuf.GetString();

	return ownShipRadarString;
}

int MessageResponder::getBasicReturnJson(Document& document, ic::Protocol& protocol)
{
	if (document.HasMember(PROTOCOL_SECTION1))
		protocol.Type = document[PROTOCOL_SECTION1].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_SEC1;

	if (document.HasMember(PROTOCOL_SECTION2))
		protocol.Command = document[PROTOCOL_SECTION2].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_SEC2;

	if (document.HasMember(PROTOCOL_SECTION3))
		protocol.SubCommand = document[PROTOCOL_SECTION3].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_SEC3;

	if (document.HasMember(PROTOCOL_ACTION))
		protocol.Action = document[PROTOCOL_ACTION].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_ACTION;

	if (document.HasMember(PROTOCOL_TOKEN))
		protocol.Token = document[PROTOCOL_TOKEN].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_TOKEN;

	if (document.HasMember(PROTOCOL_FROM))
		protocol.From = document[PROTOCOL_FROM].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_FROM;

	if (document.HasMember(PROTOCOL_TO))
		protocol.To = document[PROTOCOL_TO].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_TO;

	if (document.HasMember(PROTOCOL_DATA))
		protocol.Data = document[PROTOCOL_DATA].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_ACTION;

	return SUCCESS;
}