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

#include "sever_manager.h"

ServerManager::ServerManager()
: ICManager<ICServer, SeverMsgManager>()
{
    std::ostringstream ss;
    ss << R"(
    {
        "servers": [
        {
            "name": "Main",
            "port": )" << ic::SERVER_PORT[static_cast<int>(ic::SERVER_TYPE::SERVER_ROBOT_CONTROL)] << R"(
        },
        {
            "name": "LOG_MONITOR",
            "port": )" << ic::SERVER_PORT[static_cast<int>(ic::SERVER_TYPE::SERVER_ROBOT_LOGMONITOR)] << R"(
        }]
    }
    )";
    std::string configContent = ss.str();
    std::cout << "configContent: " << configContent << std::endl;

    Configurator::get().setDirectory();
    db_manager_ = std::make_shared<DBManager>((int)ic::DB_TYPE::DB_TYPE_LIVSMED);

    // along the server type, ic_server starts with specific socket
    // and have handler the function for validating the json format (dependency injection)
    std::shared_ptr<ICServer> socketServer_;
    socketServer_ = std::make_shared<ICServer>(configContent);
	socketServer_->beginSocket(ic::SERVER_PORT[(int)ic::SERVER_TYPE::SERVER_ROBOT_CONTROL]);
	socketServer_->setHandler(std::bind(&ServerManager::validateMsg, this, std::placeholders::_1, placeholders::_2, placeholders::_3));
    socket_list_.push_back(socketServer_);
    msg_rspndr_ = std::make_unique<MessageResponder<ICServer>>();
    msg_manager_ = std::make_unique<SeverMsgManager>();
    msg_rspndr_->setServer(socketServer_);
	msg_manager_->setSocketServer(socketServer_);
    msg_manager_->setDBManager(db_manager_);

}

ServerManager::~ServerManager()
{

}

// this function check the command format
// if received message fits the command format well,
// deliver the message to message_parser or message_manager for further process
int ServerManager::validateMsg(char cSeparator, char* pData, int nDataSize)
{
    std::string strMessage = pData;
	Document document;
    document.Parse(strMessage.c_str()).HasParseError();

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
}
