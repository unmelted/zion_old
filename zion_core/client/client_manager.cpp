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

//    task_manager_ = std::make_unique<ClientTaskManager>();

    rapidjson::Document doc;
    doc.Parse(configContent.c_str());

    if (doc.HasMember("servers") && doc["servers"].IsArray())
    {
        for (const auto& server : doc["servers"].GetArray())
        {
            std::string ip = server["ip"].GetString();
            std::string name = server["name"].GetString();
            int port = server["port"].GetInt();

            auto info = std::make_shared<ic::ClientInfo>(name, ip, port);
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
        socket->setHandler(std::bind(&ClientManager::processor, this, std::placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

int ClientManager::processor(int mode, const ic::ServerInfo& info, char* pData, int nDataSize)
{
    if(ICManager::processor(mode, info, pData, nDataSize) != SUCCESS)
    {
        return -1;
    }

    std::string strMessage = pData;
    Document document;
    document.Parse(strMessage.c_str()).HasParseError();

    std::string command = document[PROTOCOL_COMMAND].GetString();

    LOG_INFO("validateMsg command : {}", command);

    if (command == "TCP_LOG_START")
    {
        const int socket_order_of_log = 1; //will set by config file or db during construct
        Logger::update_tcp_status(server_info_list_[socket_order_of_log]->socket,
                server_info_list_[socket_order_of_log]->name);
    }

    msg_manager_->insertEventTable(document);
    return SUCCESS;
}
