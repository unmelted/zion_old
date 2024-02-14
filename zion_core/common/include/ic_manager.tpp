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

#include "ic_manager.h"
#include <string.h>

template <typename T, typename U>
ICManager<T, U>::ICManager()
{
    // along the server type, ic_server starts with specific socket
    // and have handler the function for validating the json foramt (dependency injection)
//    socketServer_ = std::make_shared<T>((int)ic::SERVER_TYPE::SERVER_ROBOT_CONTROL);
//    socketServer_->beginSocket(ic::SERVER_PORT[(int)ic::SERVER_TYPE::SERVER_ROBOT_CONTROL], 0);
//    socketServer_->setHandler(std::bind(&ICManager<T, U>::validateMsg, this, std::placeholders::_1, placeholders::_2, placeholders::_3));
//
//    db_manager_ = std::make_shared<DBManager>((int)ic::DB_TYPE::DB_TYPE_LIVSMED);
//    msg_manager_ = std::make_unique<U>();
//    msg_manager_->setSocketServer(socketServer_);
//    msg_manager_->setDBManager(db_manager_);
//
//    Configurator::get().setDirectory();
}

template <typename T, typename U>
ICManager<T, U>::~ICManager()
{

}

// this function check the command format
// if received message fits the command format well,
// deliver the message to message_parser or message_manager for further process
template <typename T, typename U>
int ICManager<T, U>::validateMsg(char cSeparator, char* pData, int nDataSize)
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



    return 1;
}
