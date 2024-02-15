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
    // along the server type, ic_server starts with specific socket
    // and have handler the function for validating the json foramt (dependency injection)
    Configurator::get().setDirectory();
    db_manager_ = std::make_shared<DBManager>((int)ic::DB_TYPE::DB_TYPE_LIVSMED);

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
    socketServer_ = std::make_shared<ICClient>(configContent);
    socketServer_->setHandler(std::bind(&ClientManager::validateMsg, this, std::placeholders::_1, placeholders::_2, placeholders::_3));

    msg_manager_ = std::make_unique<ClientMsgManager>();
    msg_manager_->setSocketServer(socketServer_);
    msg_manager_->setDBManager(db_manager_);

}

ClientManager::~ClientManager()
{

}
