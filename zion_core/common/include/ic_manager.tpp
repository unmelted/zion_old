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

template <typename T, typename U>
ICManager<T, U>::ICManager()
{
    Configurator::get().setDirectory();
    db_manager_ = std::make_shared<DBManager>(static_cast<int>(ic::DB_TYPE::DB_TYPE_LIVSMED));
    task_manager_ = std::make_unique<U>();

    EventManager::initialize();
    EventManager::addEventHandler(
            [task_manager = task_manager_.get()](int id, void* context1, void* context2) -> int
            {
                auto info = static_cast<ic::ServerInfo*>(context1);
                auto task = static_cast<ic::IC_MSG*>(context2);
                return task_manager->eventTask(id, *info, *task);
            });
    EventManager::addEventHandler(
            [task_manager = task_manager_.get()](int id, void* context1, void* context2) -> int
            {
                auto info = static_cast<ic::ServerInfo*>(context1);
                auto task = static_cast<ic::IC_MSG*>(context2);
                return task_manager->errorTask(id, *info, *task);
            });
}

// this function check the command format
// if received message fits the command format well,
template <typename T, typename U>
int ICManager<T, U>::processor(int mode, const ic::ServerInfo& info, char* pData, int nDataSize)
{
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
        LOG_ERROR("Json Parsing Fail {} ", strMessage);
        return FAIL;
    }

    if (document.HasMember("Type"))
    {
        std::string command = document["Type"].GetString();
        if(command == "LOG")
        {
            return SUCCESS;
        }
    }

    if (!document.HasMember("Type")
        || !document.HasMember("Command")
        || !document.HasMember("SubCommand")
        || !document.HasMember("Action")
        || !document.HasMember("Token"))
    {
        LOG_ERROR("Json component missing. Cannot execute.");
        return FAIL;
    }

    return SUCCESS;
}
