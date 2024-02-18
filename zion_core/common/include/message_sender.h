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

#include <sys/socket.h>
#include "ic_define.h"

using namespace rapidjson;

class MessageSender
{

public:
    MessageSender();
	~MessageSender();

	void parseAndSend(const ic::ServerInfo& info, std::string strMessage);

private:
    bool sendData(const ic::ServerInfo& info, const std::string& strJson);
	void runThread(const ic::ServerInfo& info, std::string strMessage);
	int getBasicReturnJson(Document& document, ic::Protocol& mtdProtocol);
	std::string getDocumentToString(Document& document);

private:
    std::mutex bufferMutex_;
    std::vector<char> sendBuffer_;
};
