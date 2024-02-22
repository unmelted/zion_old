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

int ServerTaskManager::eventTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task)
{
    LOG_DEBUG("eventTask is called !! {} ", id);

    if (id == (int)ic::EVENT_ID::EVENT_ID_TCP_LOG_START)
    {
        ic::IC_MSG e_msg;
        e_msg.Command = "TCP_LOG_START";
        e_msg.Token = Configurator::get().generateToken();
        e_msg.Data = info.name;
        msgSender_->parseAndSend(info, e_msg);
//        dbManager_->insertLog(info, e_msg);
        return (int)ErrorCommon::COMMON_ERR_NONE;
    }

    return (int)ErrorCommon::COMMON_ERR_NONE;
}

int ServerTaskManager::errorTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task)
{
    LOG_DEBUG("errorTask is called !! {} ", id);

    if (id == (int)ic::EVENT_ID::EVENT_ID_TCP_LOG_START)
    {
        ic::IC_MSG e_msg;
        e_msg.Command = "TCP_LOG_START";
        e_msg.Token = Configurator::get().generateToken();
        e_msg.Data = info.name;
        msgSender_->parseAndSend(info, e_msg);
//        dbManager_->insertLog(info, e_msg);
        return (int)ErrorCommon::COMMON_ERR_NONE;
    }

    return (int)ErrorCommon::COMMON_ERR_NONE;
}