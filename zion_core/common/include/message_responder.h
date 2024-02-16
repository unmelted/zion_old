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

#include "ic_define.h"

using namespace rapidjson;

template <typename T>
class MessageResponder
{

public:
	MessageResponder();
	~MessageResponder();
	void setServer(std::shared_ptr<T> server);

	bool isThreadStop();
	void parseAndSendResponse(std::string strMessage);

private:
	void parseThread(void* param, std::string strMessage);
	int getBasicReturnJson(Document& document, ic::Protocol& mtdProtocol);
	std::string getDocumentToString(Document& document);

private:
    std::shared_ptr<T> server_;
    bool isThreadStop_;

};

#include "message_responder.tpp"
