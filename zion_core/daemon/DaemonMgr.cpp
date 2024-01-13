﻿/*
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

#include "DaemonMgr.h"
#include <string.h>


DaemonMgr::DaemonMgr()
{
	m_parser.SetDMServer(&m_dmServer);
	m_dmServer.BeginSocket(CM_CONTROL_DAEMON_PORT, 0);
	m_dmServer.setHandler(std::bind(&DaemonMgr::Classfication, this, std::placeholders::_1, placeholders::_2, placeholders::_3));
	m_manger.SetDMServer(&m_dmServer);	

	Configurator::Get().SetDirectory();
}

DaemonMgr::~DaemonMgr()
{

}

int	DaemonMgr::Classfication(char cSeparator, char* pData, int nDataSize)
{
	switch (cSeparator)
	{
	case PACKETTYPE_JSON:
		RecJson(pData);
		break;

	default:
		//ErrorL << "Invalid Message Separator : " << int(cSeparator) << "\n" << pData;
		break;
	}

	return 0;
}


int DaemonMgr::RecJson(std::string strMessage)
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
	CMd_INFO("RecJson sec3 : {} compare {}", sec3, sec3.compare("Version"));
	if(sec3.compare("Version") == 0) {
		m_parser.RunParse(strMessage);
	}
	else if(sec3.compare("Stabilize") == 0) {
		m_parser.RunParse(strMessage);		
		m_manger.OnRcvMessage(strMessage);
	}

	return 1;
}

void DaemonMgr::GetBasicReturnJson(Document& document, ic::MTdProtocol& mtdProtocol)
{
	std::string strTp;
	if (document.HasMember("Section1") == true)
		mtdProtocol.Section1 = document["Section1"].GetString();

	if (document.HasMember("Sectoin2") == true)
		mtdProtocol.Section2 = document["Sectoin2"].GetString();

	if (document.HasMember("Sectoin3") == true)
		mtdProtocol.Section3 = document["Sectoin3"].GetString();
	
	if (document.HasMember("SendState") == true)
		mtdProtocol.SendState = document["SendState"].GetString();

	if (document.HasMember("From") == true)
		mtdProtocol.From = document["From"].GetString();

	if (document.HasMember("To") == true)
		mtdProtocol.To = document["To"].GetString();

	if (document.HasMember("action") == true)
		mtdProtocol.action = document["action"].GetString();
}