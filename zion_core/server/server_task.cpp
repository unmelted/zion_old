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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/20.
 *
 */

#include "server_task.h"

using namespace rapidjson;

ServerTaskManager::ServerTaskManager()
: TaskManager(ic::TASKPOOL_SIZE)
{

}

ServerTaskManager::~ServerTaskManager()
{

}

void ServerTaskManager::setMsgManager(std::shared_ptr<SeverMsgManager> msg_manager)
{
    msg_manager_ = msg_manager;
}

int ServerTaskManager::eventTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task)
{
    ic::IC_MSG e_msg;
    e_msg.Type = "REQUEST";
    e_msg.Token = Configurator::get().generateToken();
    e_msg.From = "ICS_SERVER";

    if (id == static_cast<int>(ic::EVENT_ID::EVENT_ID_TCP_LOG_START))
    {
        e_msg.Command = "TCP_LOG_START";
        e_msg.Data = info.name;
        msg_sender_->parseAndSend(info, e_msg);
    }
    else if (id == static_cast<int>(ic::EVENT_ID::EVENT_ID_REQUEST_INFO))
    {
        e_msg.Command = "GET_INFO";
        msg_sender_->parseAndSend(info, e_msg);
    }
    LOG_DEBUG("eventTask sent !! Command {} id {} ", e_msg.Command, id);
    storeEventTask(id, e_msg);
    return SUCCESS;
}

int ServerTaskManager::errorTask(int err_id, const ic::ServerInfo& info, const ic::IC_MSG& msg)
{
    ic::IC_MSG e_msg;

    if (err_id == (int)ic::EVENT_ID::EVENT_ID_TCP_LOG_START)
    {
        return SUCCESS;
    }

    LOG_DEBUG("errorTask sent !! Command {} id {} ", e_msg.Command, err_id);
    storeErrorTask(err_id, e_msg);
    return SUCCESS;
}

int ServerTaskManager::storeEventTask(int id, ic::IC_MSG& msg)
{
    Document doc;
    convertMSGToDocument(msg, doc);
    msg_manager_->insertEventTable(doc);
    return SUCCESS;
}

int ServerTaskManager::storeErrorTask(int id, ic::IC_MSG& msg)
{
    Document doc;
    convertMSGToDocument(msg, doc);
    msg_manager_->insertEventTable(doc);
    return SUCCESS;
}