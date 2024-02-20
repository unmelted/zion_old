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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/14.
 *
 */

#include "client_manager.h"

ClientManager::ClientManager()
: ICManager<ICClient, ClientTaskManager>()
{
    std::ostringstream ss;
    ss << R"(
    {
        "servers": [
        {
            "name": "Slave1_CTRL",
            "ip": "127.0.0.1",
            "port": )" << ic::SERVER_PORT[static_cast<int>(ic::SERVER_TYPE::SERVER_ROBOT_CONTROL)] << R"(
        },
        {
            "name": "Slave1_LOG",
            "ip": "127.0.0.1",
            "port": )" << ic::SERVER_PORT[static_cast<int>(ic::SERVER_TYPE::SERVER_ROBOT_LOGMONITOR)] << R"(
        }]
    }
    )";

    std::string configContent = ss.str();
    std::cout << "configContent: " << configContent << std::endl;

    task_manager_ = std::make_unique<ClientTaskManager>();

    rapidjson::Document doc;
    doc.Parse(configContent.c_str());

    if (doc.HasMember("servers") && doc["servers"].IsArray())
    {
        for (const auto& server : doc["servers"].GetArray())
        {
            std::string ip = server["ip"].GetString();
            std::string name = server["name"].GetString();
            int port = server["port"].GetInt();

            ic::ClientInfo info(name, ip, port);
            std::shared_ptr<ICClient> socketServer_;
            socketServer_ = std::make_shared<ICClient>(info);

            server_info_list_.push_back(info);
            socket_list_.push_back(socketServer_);
            LOG_DEBUG("Add server: {} {}", name, port);
        }
    }

    msg_manager_ = std::make_unique<ClientMsgManager>();
    msg_manager_->setDBManager(db_manager_);
}

ClientManager::~ClientManager()
{

}

int ClientManager::initialize()
{
    // along the server type, ic_server starts with specific socket
    // and have handler the function for validating the json format (dependency injection)
    for( auto& socket : socket_list_)
    {
        LOG_DEBUG(" loop in initialize : socket {}", socket->getSocket());
        socket->beginSocket();
        socket->setHandler(std::bind(&ClientManager::classifier, this, std::placeholders::_1, placeholders::_2, placeholders::_3));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    EventManager::addEventHandler(static_cast<int>(ic::EVENT_ID::EVENT_ID_WHO),
            [task_manager = task_manager_.get()](int id, void* context1, void* context2) -> int
            {
                auto info = static_cast<ic::ServerInfo*>(context1);
                auto task = static_cast<ic::MSG_T*>(context2);
                return task_manager->eventTask(id, *info, *task);

            });
    return 0;
}

// this function check the command format
// if received message fits the command format well,
// deliver the message to message_parser or message_manager for further process
int ClientManager::classifier(const ic::ServerInfo& info, char* pData, int nDataSize)
{
    ICManager::classifier(info, pData, nDataSize);

    std::string strMessage = pData;
    Document document;
    document.Parse(strMessage.c_str()).HasParseError();

    std::string command = document[PROTOCOL_SECTION2].GetString();

//    msg_manager_->insertEventTable(document, (int)ic::MSG_TYPE::MSG_TYPE_RCV);
    LOG_INFO("validateMsg command : {}", command);

    if (command == "CONNECT")
    {
        // pas this command
    }
    else
    {

    }

    return 1;
}
