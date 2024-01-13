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

#pragma once

#include "DMServer.h"
#include "define.h"
#include <functional>
#include "DaemonParser.h"
#include "MessageManager.hpp"

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
using namespace rapidjson; 

class DaemonMgr
{
public:
	DaemonMgr();
	~DaemonMgr();

private:
	DMServer m_dmServer;
	DaemonParser m_parser;
	MsgManager m_manger;

	void GetBasicReturnJson(Document& document, ic::MTdProtocol& mtdProtocol);
	int	Classfication(char cSeparator, char* pData, int nDataSize);
	int RecJson(std::string strMessage);
};

