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

#include "ic_define.h"
#include "ic_server.h"

using namespace rapidjson;

class MessageParser
{

public:
	MessageParser();
	~MessageParser();
	void setICServer(std::shared_ptr<ICServer> icServer);

	bool isThreadStop();
	void runParse(std::string strMessage);

private:
	void parseThread(void* param, std::string strMessage);
	int getBasicReturnJson(Document& document, ic::Protocol& mtdProtocol);
	std::string getDocumentToString(Document& document);

private:
    std::shared_ptr<ICServer> icServer_;
    bool isThreadStop_;

};
