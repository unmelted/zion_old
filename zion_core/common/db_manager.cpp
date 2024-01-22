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

}

DBManager::~DBManager()
{

}

bool DBManager::openDB(std::string strDBPath)
{
    int nRet = sqlite3_open(strDBPath.c_str(), &db_);
    if (nRet != SQLITE_OK)
    {
        LOG_ERROR("Can't open database: {}", sqlite3_errmsg(db_));
        return false;
    }

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
    return 0;
}

void DBManager::queryThread()
{

}

int DBManager::runQuery()
{
    return 0;
}