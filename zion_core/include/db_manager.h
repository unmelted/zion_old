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
#pragma once

#include <sqlite3.h>
#include "ic_define.h"


class DBManager
{
public:
    DBManager();
    ~DBManager();

    int enqueueQuery(std::shared_ptr<ic::MSG_T> msg);

protected:
    bool openDB(std::string strDBPath);
    int createTable(std::string createQuery);
    bool closeDB();
    void queryThread();
    int runQuery(std::shared_ptr<ic::MSG_T> query);

    sqlite3* db_;

    std::unique_ptr<std::thread> queryThread_;
    MessageQueue<std::shared_ptr<ic::MSG_T>> queQuery_;
    std::condition_variable cv_query_;
    std::mutex queryMutex_;

    bool isOpen_;
    bool isQueryThread_;
};

class DBLogManager : public DBManager
{
public :
    DBLogManager(std::string dbName);
    ~DBLogManager();

protected:

    int createTable(std::string createQuery);

};