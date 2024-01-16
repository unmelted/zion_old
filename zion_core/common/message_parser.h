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

#define RAPIDJSON_HAS_STDSTRING 1
#include "_3rdparty_/rapidjson/include/rapidjson/document.h"
#include "_3rdparty_/rapidjson/include/rapidjson/writer.h"
#include "_3rdparty_/rapidjson/include/rapidjson/stringbuffer.h"
#include "Util/logger.h"
#include "ics_define.h"
#include "server.h"
#include "curl/curl.h"

using namespace rapidjson;

class DaemonParser
{

public:
	DaemonParser();
	~DaemonParser();
	void setDMServer(DMServer* dmServer);
	DMServer* getDMServer();
	bool isThreadStop();
	void runParse(std::string strMessage);

private:
	void parseThread(void* param, std::string strMessage);
	int getBasicReturnJson(Document& document, ic::MTdProtocol& mtdProtocol);
	std::string getDocumentToString(Document& document);

private:
    DMServer* m_dmServer;
    bool m_bThreadStop;

};
