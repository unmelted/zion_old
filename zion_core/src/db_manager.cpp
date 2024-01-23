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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/22.
 *
 */

#include "db_manager.h"

DBManager::DBManager()
{
    isQueryThread_ = true;
    queryThread_ = std::make_unique<std::thread>(&DBManager::queryThread, this);
}

DBManager::~DBManager()
{
    closeDB();
    isQueryThread_ = false;
    cv_query_.notify_all();
    queryThread_->join();

    LOG_DEBUG("DBManager Destroyed Done.");
}

bool DBManager::openDB(std::string strDBPath)
{
    int nRet = sqlite3_open(strDBPath.c_str(), &db_);
    if (nRet != SQLITE_OK)
    {
        LOG_ERROR("Can't open database: {}", sqlite3_errmsg(db_));
        return false;
    }

    isOpen_ = true;
    return true;
}

int DBManager::createTable(std::string createQuery)
{
    return 0;
}

bool DBManager::closeDB()
{
    return 0;
}

int DBManager::enqueueQuery(std::shared_ptr<ic::MSG_T> msg)
{
    if(!isQueryThread_)
    {
        LOG_ERROR("DBManager is already stopped.");
        return -1;
    }

    std::lock_guard<std::mutex> lock(queryMutex_);
    queQuery_.Enqueue(msg);
    cv_query_.notify_one();

    return 0;
}

void DBManager::queryThread()
{
    while (isQueryThread_)
    {
        std::unique_lock<std::mutex> lock(queryMutex_);
        cv_query_.wait(lock);

        if (queQuery_.IsQueue())
        {
            auto msg = queQuery_.Dequeue();
            if (msg == nullptr)
                continue;

            runQuery(msg);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}

int DBManager::runQuery(std::shared_ptr<ic::MSG_T> query)
{
    int result = sqlite3_exec(db_, query->txt.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        LOG_ERROR("Failed to execute query: {}", sqlite3_errmsg(db_));
        return -1;
    }

    return 0;
}
