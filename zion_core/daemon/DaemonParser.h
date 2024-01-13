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
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include "Util/logger.h"
#include "define.h"
#include "DMServer.h"
#include "curl/curl.h"

using namespace rapidjson;

class DaemonParser
{
private:
	DMServer* m_dmServer;
	bool m_bThreadStop;

public:
	DaemonParser();
	~DaemonParser();

	void SetDMServer(DMServer* dmServer);

	DMServer* GetDMServer();
	bool IsThreadStop();
	void RunParse(std::string strMessage);
private:
	void ParseThread(void* param, std::string strMessage);
	int GetBasicReturnJson(Document& document, ic::MTdProtocol& mtdProtocol);
	std::string GetDocumentToString(Document& document);
};
