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

#include "server_manager.h"

ServerManager::ServerManager()
: ICManager<ICServer, ServerTaskManager>()
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

//    task_manager_ = std::make_unique<ServerTaskManager>();

    rapidjson::Document doc;
    doc.Parse(configContent.c_str());

    if (doc.HasMember("servers") && doc["servers"].IsArray())
    {
        for (const auto& server : doc["servers"].GetArray())
        {
            std::string ip = "127.0.0.1";
            std::string name = server["name"].GetString();
            int port = server["port"].GetInt();

            auto info = std::make_shared<ic::ServerInfo>(name, ip, port);
            std::shared_ptr<ICServer> socketServer_;
            socketServer_ = std::make_shared<ICServer>(info);

            server_info_list_.push_back(info);
            socket_list_.push_back(socketServer_);
            LOG_DEBUG("Add server: {} {}", name, port);
        }
    }

    msg_manager_ = std::make_unique<SeverMsgManager>();
    msg_manager_->setDBManager(db_manager_);
}

ServerManager::~ServerManager()
{

}

int ServerManager::initialize()
{
    for( auto& socket : socket_list_)
    {
        LOG_DEBUG(" loop in initialize : socket {}", socket->getSocket());
        socket->beginSocket();
        socket->setHandler(std::bind(&ServerManager::doManage, this, std::placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
    }

//    EventManager::addEventHandler(
//            [task_manager = task_manager_.get()](int id, void* context1, void* context2) -> int
//            {
//                auto info = static_cast<ic::ServerInfo*>(context1);
//                auto task = static_cast<ic::MSG_T*>(context2);
//                return task_manager->eventTask(id, *info, *task);
//            });
    return 0;
}

// this function check the command format
// if received message fits the command format well,
// deliver the message to message_parser or message_manager for further process
int ServerManager::doManage(int mode, const ic::ClientInfo& info, char* pData, int nDataSize)
{
    ICManager<ICServer, ServerTaskManager>::doManage(mode, info, pData, nDataSize);

    std::string strMessage = pData;
	Document document;
    document.Parse(strMessage.c_str()).HasParseError();

	std::string command = document[PROTOCOL_SECTION2].GetString();

//    msg_manager_->insertEventTable(document, (int)ic::MSG_TYPE::MSG_TYPE_RCV);
	LOG_INFO("validateMsg command : {}", command);

    if (command == "TCP_LOG")
    {
        LOG_DEBUG("TCP_LOG received :  {} ", document["Data"].GetString());
    }
    else if(command =="START")
    {
//        msg_manager_->onRcvMessage(strMessage);
//        msg_manager_->onRcvMessage(document["From"].GetString(), strMessage);
	}
	else if(command == "TEST")
    {
        // if the message has necessary to send response,
        // call the parseAndSendResponse()
        // msg_sender->parseAndSendResponse(strMessage);

        // call onRcvMessage with message for insert job in queue
//		msg_manager_->onRcvMessage(document["From"].GetString(), strMessage);
	}
    else
    {

    }

	return 1;
}
