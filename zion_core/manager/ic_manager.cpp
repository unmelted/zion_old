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
    // along the server type, ic_server starts with specific socket
    // and have handler the function for validating the json foramt (dependency injectcion)
    icServer_ = std::make_shared<ICServer>((int)ic::SERVER_TYPE::SERVER_ROBOT_CONTROL);
	icServer_->beginSocket(ic::SERVER_PORT[(int)ic::SERVER_TYPE::SERVER_ROBOT_CONTROL], 0);
	icServer_->setHandler(std::bind(&ICManager::validateJson, this, std::placeholders::_1, placeholders::_2, placeholders::_3));

    msg_parser_ = std::make_unique<MessageParser>();
    msg_manager_ = std::make_unique<MsgManager>();
    msg_parser_->setICServer(icServer_);
	msg_manager_->setICServer(icServer_);

	Configurator::get().setDirectory();
}

ICManager::~ICManager()
{

}

// this function check the command format
// if received message fits the command format well, deliver the message to message_parser
int ICManager::validateJson(char cSeparator, char* pData, int nDataSize)
{

    if( cSeparator != (char)ic::PACKET_SEPARATOR::PACKETTYPE_JSON)
    {
        LOG_ERROR("validateJson cSeparator != (char)ic::PACKET_SEPARATOR::PACKETTYPE_JSON");
        return 0;
    }

    std::string strMessage = pData;
	Document document;
	bool isSuccess = false;
	try {
        isSuccess = document.Parse(strMessage.c_str()).HasParseError();
	}
	catch (...) {
        isSuccess = true;
	}

	if (isSuccess)
	{
        LOG_ERROR("Json Parsing Faile {} ", strMessage);
		return 0;
	}

	std::string sec3 = document[PROTOCOL_SECTION3].GetString();

	LOG_INFO("validateJson sec3 : {}", sec3);

	if(sec3.compare("TEST_COMMAND_3") == 0) {
        // if the message has necessary to send response,
        // call the parseAndSendResponse function
		msg_parser_->parseAndSendResponse(strMessage);
	}
	else if(sec3.compare("Stabilize") == 0) {
		msg_parser_->parseAndSendResponse(strMessage);
		msg_manager_->onRcvMessage(strMessage);
	}

	return 1;
}
