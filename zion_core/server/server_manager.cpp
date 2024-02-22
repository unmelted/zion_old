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
    db_manager_monitor = std::make_shared<DBManager>(static_cast<int>(ic::DB_TYPE::DB_TYPE_LOG_MONITOR));

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
    msg_manager_->setDBManagerForMonitor(db_manager_monitor);
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
        socket->setHandler(std::bind(&ServerManager::processor, this, std::placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
    }

    return 0;
}

// this function check the command format
// if received message fits the command format well,
// deliver the message to message_parser or message_manager for further process
int ServerManager::processor(int mode, const ic::ClientInfo& info, char* pData, int nDataSize)
{
    if(ICManager::processor(mode, info, pData, nDataSize) != SUCCESS)
    {
        return -1;
    }

    std::string strMessage = pData;
	Document document;
    document.Parse(strMessage.c_str()).HasParseError();

	std::string command = document[PROTOCOL_COMMAND].GetString();

//	LOG_INFO("validateMsg command : {}", command);

    if (command == "TCP_LOG")
    {
        Document doc_t(kObjectType);
        Document::AllocatorType& allocator = doc_t.GetAllocator();
        doc_t.AddMember("From", document["From"], allocator);
        doc_t.AddMember("Data", document["Data"], allocator);

        msg_manager_->insertLogMonitorTable(doc_t);
        return SUCCESS;
    }
    else if(command =="START")
    {
//        msg_manager_->onRcvMessage(strMessage);
//        msg_manager_->onRcvMessage(document["From"].GetString(), strMessage);
	}
    else
    {

    }
    msg_manager_->insertEventTable(document);
	return SUCCESS;
}
