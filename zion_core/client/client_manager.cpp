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
: ICManager<ICClient, ClientMsgManager>()
{
    std::ostringstream ss;
    ss << R"(
    {
        "servers": [
        {
            "name": "Server1",
            "ip": "127.0.0.1",
            "port": )" << ic::SERVER_PORT[static_cast<int>(ic::SERVER_TYPE::SERVER_ROBOT_CONTROL)] << R"(
        }]
    }
    )";

    std::string configContent = ss.str();
    std::cout << "configContent: " << configContent << std::endl;

    Configurator::get().setDirectory();
    db_manager_ = std::make_shared<DBManager>((int)ic::DB_TYPE::DB_TYPE_LIVSMED);

    // along the server type, ic_server starts with specific socket
    // and have handler the function for validating the json format (dependency injection)
    std::shared_ptr<ICClient> socketServer_;
    socketServer_ = std::make_shared<ICClient>(configContent);
    int port = 10; // temporary for compile
    socketServer_->beginSocket(port);
    socketServer_->setHandler(std::bind(&ClientManager::validateMsg, this, std::placeholders::_1, placeholders::_2, placeholders::_3));
    socket_list_.push_back(socketServer_);
    msg_manager_ = std::make_unique<ClientMsgManager>();
    msg_manager_->setSocketServer(socketServer_);
    msg_manager_->setDBManager(db_manager_);

}

ClientManager::~ClientManager()
{

}

int ClientManager::initialize()
{

}

// this function check the command format
// if received message fits the command format well,
// deliver the message to message_parser or message_manager for further process
int ClientManager::validateMsg(char cSeparator, char* pData, int nDataSize)
{
    ICManager::validateMsg(cSeparator, pData, nDataSize);

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
    else
    {

    }

    return 1;
}
