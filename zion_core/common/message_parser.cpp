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

#include "message_parser.h"
#include "error_manager.h"
#include "_3rdparty_/rapidjson/include/rapidjson/prettywriter.h"
#include <set>
#include <future>
#include "ic_util.hpp"
#include "logger.hpp"

namespace
{
	template<typename ... Args>
	std::string string_format(const std::string& format, Args ... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}
}

MessageParser::MessageParser()
{

}

MessageParser::~MessageParser()
{

}

void MessageParser::runParse(std::string strMessage)
{
	std::thread th(&MessageParser::parseThread, this, this, strMessage);

	th.join();
}

bool MessageParser::isThreadStop()
{
	return isThreadStop_;
}

void MessageParser::parseThread(void* param, std::string strMessage)
{
	MessageParser* pMain = (MessageParser*)param;
	if (pMain->isThreadStop())
		return;

	//InfoL << "Recv : " << strMessage;
	Document document;
	document.Parse(strMessage.c_str());

	string strError;
	ic::Protocol protocol;
	int nResultCode = getBasicReturnJson(document, protocol);
	Document sendDocument(kObjectType);
	Document::AllocatorType& allocator = sendDocument.GetAllocator();

//	if (document.HasMember(PROTOCOL_SENDSTATE))
//	{
//		std::string sendState = document[PROTOCOL_SENDSTATE].GetString();
//		if (sendState.compare(PROTOCOL_SENDSTATE_RESPONSE) == 0) // Return ó��
//			nResultCode = (int)ErrorCommon::COMMON_ERR_UNKNOWN_SENDSTATE;
//	}

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
        std::string temp_clientname = "name_temp";
		if (pMain->icServer_->sendData(temp_clientname, sendString.c_str()))
		{
			//ErrorL << strSendString;
		}
		return;
	}

	string strSection1 = protocol.Type;
	string strSection2 = protocol.Command;
	string strSection3 = protocol.SubCommand;
	string strAction = protocol.Action;
	LOG_DEBUG("sction {} {} {}", strSection1, strSection2, strSection3);

	if (strSection1.compare("Daemon"))
	{
		if (strSection2.compare("Information"))
		{
			if (strSection3.compare("Version"))
			{
				Value ver(kObjectType);
				Value cmd(kObjectType);
				cmd.AddMember("version", CURRENTVERSION, allocator);
				cmd.AddMember("date", Configurator::get().getCurrentDateTime("now"), allocator);
				ver.AddMember("CMd", cmd, allocator);
				sendDocument.AddMember("Version", ver, allocator);				
				LOG_INFO("version process.");
			}
		}
	}

	std::string strSendString = getDocumentToString(sendDocument);
    std::string temp_clientname = "name_temp";
	pMain->icServer_->sendData(temp_clientname, strSendString.c_str());
}

void MessageParser::setICServer(std::shared_ptr<ICServer> icServer)
{
	icServer_ = icServer;
}

std::string MessageParser::getDocumentToString(Document& document)
{
	StringBuffer strbuf;
	strbuf.Clear();
	PrettyWriter<StringBuffer> writer(strbuf);
	document.Accept(writer);
	std::string ownShipRadarString = strbuf.GetString();

	return ownShipRadarString;
}

int MessageParser::getBasicReturnJson(Document& document, ic::Protocol& protocol)
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