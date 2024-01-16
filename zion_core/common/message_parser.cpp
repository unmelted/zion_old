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
#include "ics_util.hpp"
#include "logger.hpp"

//#define PRINT_HTTP_REQ
//#define PRINT_HTTP_RES

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
	return m_bThreadStop;
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
	ic::MTdProtocol mtdProtocol;
	int nResultCode = getBasicReturnJson(document, mtdProtocol);
	Document sendDocument(kObjectType);
	Document::AllocatorType& allocator = sendDocument.GetAllocator();

	if (document.HasMember(MTDPROTOCOL_SENDSTATE) == true)
	{
		std::string strSendState = document[MTDPROTOCOL_SENDSTATE].GetString();
		if (strSendState.compare(MTDPROTOCOL_SENDSTATE_RESPONSE) == 0) // Return ó��
			nResultCode = (int)ErrorCommon::COMMON_ERR_UNKNOWN_SENDSTATE;
	}

	sendDocument.AddMember(MTDPROTOCOL_SECTION1, mtdProtocol.Section1, allocator);
	sendDocument.AddMember(MTDPROTOCOL_SECTION2, mtdProtocol.Section2, allocator);
	sendDocument.AddMember(MTDPROTOCOL_SECTION3, mtdProtocol.Section3, allocator);
	sendDocument.AddMember(MTDPROTOCOL_SENDSTATE, "response", allocator);
	sendDocument.AddMember(MTDPROTOCOL_TOKEN, mtdProtocol.Token, allocator);
	sendDocument.AddMember(MTDPROTOCOL_FROM, mtdProtocol.To, allocator);
	sendDocument.AddMember(MTDPROTOCOL_TO, mtdProtocol.From, allocator);
	sendDocument.AddMember(MTDPROTOCOL_ACTION, mtdProtocol.action, allocator);
	sendDocument.AddMember(MTDPROTOCOL_RESULTCODE, nResultCode, allocator);
	sendDocument.AddMember(MTDPROTOCOL_ERRORMSG, "", allocator);

	if (nResultCode != SUCCESS)
	{
		sendDocument[MTDPROTOCOL_ERRORMSG].SetString(getErrorCodeToString(nResultCode), allocator);
		std::string strSendString = getDocumentToString(sendDocument);
        std::string temp_clientname = "name_temp";
		if (pMain->getDMServer()->sendData(temp_clientname, strSendString.c_str()))
		{
			//ErrorL << strSendString;
		}
		return;
	}

	string strSection1 = mtdProtocol.Section1;
	string strSection2 = mtdProtocol.Section2;
	string strSection3 = mtdProtocol.Section3;
	string strAction = mtdProtocol.action;
	CMd_DEBUG("sction {} {} {}", strSection1, strSection2, strSection3);

	if (strSection1.compare("Daemon") == 0)
	{
		if (strSection2.compare("Information") == 0)
		{
			if (strSection3.compare("Version") == 0)
			{
				Value ver(kObjectType);
				Value cmd(kObjectType);
				cmd.AddMember("version", CURRENTVERSION, allocator);
				cmd.AddMember("date", Configurator::get().getCurrentDateTime("now"), allocator);
				ver.AddMember("CMd", cmd, allocator);
				sendDocument.AddMember("Version", ver, allocator);				
				CMd_INFO("version process.");
			}
		}
	}

	std::string strSendString = getDocumentToString(sendDocument);
    std::string temp_clientname = "name_temp";
	pMain->getDMServer()->sendData(temp_clientname, strSendString.c_str());
}


ICServer* MessageParser::getDMServer()
{
	return icServer;
}


void MessageParser::setICServer(ICServer* dmServer)
{
	icServer = dmServer;
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

int MessageParser::getBasicReturnJson(Document& document, ic::MTdProtocol& mtdProtocol)
{
	if (document.HasMember(MTDPROTOCOL_SECTION1) == true)
		mtdProtocol.Section1 = document[MTDPROTOCOL_SECTION1].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_SEC1;

	if (document.HasMember(MTDPROTOCOL_SECTION2) == true)
		mtdProtocol.Section2 = document[MTDPROTOCOL_SECTION2].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_SEC2;

	if (document.HasMember(MTDPROTOCOL_SECTION3) == true)
		mtdProtocol.Section3 = document[MTDPROTOCOL_SECTION3].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_SEC3;

	if (document.HasMember(MTDPROTOCOL_SENDSTATE) == true)
		mtdProtocol.SendState = document[MTDPROTOCOL_SENDSTATE].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_SENDSTATE;

	if (document.HasMember(MTDPROTOCOL_TOKEN) == true)
		mtdProtocol.Token = document[MTDPROTOCOL_TOKEN].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_TOKEN;

	if (document.HasMember(MTDPROTOCOL_FROM) == true)
		mtdProtocol.From = document[MTDPROTOCOL_FROM].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_FROM;

	if (document.HasMember(MTDPROTOCOL_TO) == true)
		mtdProtocol.To = document[MTDPROTOCOL_TO].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_TO;

	if (document.HasMember(MTDPROTOCOL_ACTION) == true)
		mtdProtocol.action = document[MTDPROTOCOL_ACTION].GetString();
	else
		return (int)ErrorCommon::COMMON_ERR_NOT_FOUND_ACTION;

	return SUCCESS;
}