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

#include "ic_manager.h"
#include <string.h>


ICManager::ICManager()
{
	msg_parser.setICServer(&icServer);
	icServer.beginSocket(CM_CONTROL_DAEMON_PORT, 0);
	icServer.setHandler(std::bind(&ICManager::classfication, this, std::placeholders::_1, placeholders::_2, placeholders::_3));
	msg_manager.setICServer(&icServer);

	Configurator::get().setDirectory();
}

ICManager::~ICManager()
{

}

int	ICManager::classfication(char cSeparator, char* pData, int nDataSize)
{
	switch (cSeparator)
	{
        case (char)ic::PACKET_SEPARATOR::PACKETTYPE_JSON:
		recJson(pData);
		break;

	default:
		//ErrorL << "Invalid Message Separator : " << int(cSeparator) << "\n" << pData;
		break;
	}

	return 0;
}


int ICManager::recJson(std::string strMessage)
{
	Document document;
	bool bSuc = false;
	try {
		bSuc = document.Parse(strMessage.c_str()).HasParseError();
	}
	catch (...) {
		bSuc = true;
	}
	if (bSuc)
	{
		//ErrorL << "Json Parsing Fail";
		//ErrorL << strMessage;
		return 0;
	}

	std::string sec3 = document[MTDPROTOCOL_SECTION3].GetString();
	CMd_INFO("recJson sec3 : {} compare {}", sec3, sec3.compare("Version"));
	if(sec3.compare("Version") == 0) {
		msg_parser.runParse(strMessage);
	}
	else if(sec3.compare("Stabilize") == 0) {
		msg_parser.runParse(strMessage);
		msg_manager.onRcvMessage(strMessage);
	}

	return 1;
}

void ICManager::getBasicReturnJson(Document& document, ic::MTdProtocol& mtdProtocol)
{
	std::string strTp;
	if (document.HasMember("Section1"))
		mtdProtocol.Section1 = document["Section1"].GetString();

	if (document.HasMember("Sectoin2"))
		mtdProtocol.Section2 = document["Sectoin2"].GetString();

	if (document.HasMember("Sectoin3"))
		mtdProtocol.Section3 = document["Sectoin3"].GetString();
	
	if (document.HasMember("SendState"))
		mtdProtocol.SendState = document["SendState"].GetString();

	if (document.HasMember("From"))
		mtdProtocol.From = document["From"].GetString();

	if (document.HasMember("To"))
		mtdProtocol.To = document["To"].GetString();

	if (document.HasMember("action"))
		mtdProtocol.action = document["action"].GetString();
}