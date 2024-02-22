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

#include "client_task.h"

using namespace rapidjson;

ClientTaskManager::ClientTaskManager()
        : TaskManager(ic::TASKPOOL_SIZE)
{

}

ClientTaskManager::~ClientTaskManager()
{

}

int ClientTaskManager::eventTask(int id, const ic::ServerInfo& info, const ic::IC_MSG& task)
{
    LOG_DEBUG("eventTask is called !! {} ", id);

    if (id == static_cast<int>(ic::EVENT_ID::EVENT_ID_WHO))
    {
        ic::IC_MSG e_msg;
        e_msg.Command = "WHOAMI";
        e_msg.Token = Configurator::get().generateToken();
        e_msg.Data = info.name;
        msgSender_->parseAndSend(info, e_msg);
        return static_cast<int>(ErrorCommon::COMMON_ERR_NONE);
    }

    return static_cast<int>(ErrorCommon::COMMON_ERR_NONE);
}