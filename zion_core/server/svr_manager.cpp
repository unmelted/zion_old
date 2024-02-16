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

#include "svr_manager.h"
#include <string.h>


ServerManager::ServerManager()
: ICManager<ICServer, SvrMsgManager>()
, msg_rspndr_(new MessageResponder())
{
    Configurator::get().setDirectory();
    db_manager_ = std::make_shared<DBManager>((int)ic::DB_TYPE::DB_TYPE_LIVSMED);

    // along the server type, ic_server starts with specific socket
    // and have handler the function for validating the json foramt (dependency injection)
    socketServer_ = std::make_shared<ICServer>();
	socketServer_->beginSocket(ic::SERVER_PORT[(int)ic::SERVER_TYPE::SERVER_ROBOT_CONTROL]);
	socketServer_->setHandler(std::bind(&ServerManager::validateMsg, this, std::placeholders::_1, placeholders::_2, placeholders::_3));

    msg_rspndr_ = std::make_unique<MessageResponder>();
    msg_manager_ = std::make_unique<SvrMsgManager>();
    msg_rspndr_->setICServer(socketServer_);
	msg_manager_->setSocketServer(socketServer_);
    msg_manager_->setDBManager(db_manager_);

}

ServerManager::~ServerManager()
{

}

// this function check the command format
// if received message fits the command format well,
// deliver the message to message_parser or message_manager for further process
/*
int ServerManager::validateMsg(char cSeparator, char* pData, int nDataSize)
{

    if( cSeparator != (char)ic::PACKET_SEPARATOR::PACKETTYPE_JSON)
    {
        LOG_ERROR("validateMsg cSeparator != (char)ic::PACKET_SEPARATOR::PACKETTYPE_JSON");
        return 0;
    }

    std::string strMessage = pData;
	Document document;
	bool isSuccess = false;
	try
    {
        isSuccess = document.Parse(strMessage.c_str()).HasParseError();
	}
	catch (...)
    {
        isSuccess = false;
	}

	if (isSuccess)
	{
        LOG_ERROR("Json Parsing Faile {} ", strMessage);
		return 0;
	}

    if (!document.HasMember("Type")
        || !document.HasMember("Command")
        || !document.HasMember("SubCommand")
        || !document.HasMember("Action")
        || !document.HasMember("Token"))
    {
        LOG_ERROR("Json component missing. can't execute.");
        return 0;
    }

	std::string command = document[PROTOCOL_SECTION2].GetString();

    msg_manager_->insertEventTable(document, (int)ic::MSG_TYPE::MSG_TYPE_RCV);
	LOG_INFO("validateMsg command : {}", command);

    if (command == "CONNECT")
    {
        // pas this command
    }
    else if(command =="START")
    {
//        msg_manager_->onRcvMessage(strMessage);
//        msg_rspndr_->parseAndSendResponse(strMessage);
        msg_manager_->onRcvMessage(document["From"].GetString(), strMessage);
	}
	else if(command == "TEST")
    {
        // if the message has necessary to send response,
        // call the parseAndSendResponse()
        // msg_rspndr_->parseAndSendResponse(strMessage);

		msg_rspndr_->parseAndSendResponse(strMessage);
        // call onRcvMessage with message for insert job in queue
		msg_manager_->onRcvMessage(document["From"].GetString(), strMessage);
	}
    else
    {

    }

	return 1;
} */
