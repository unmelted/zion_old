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

#include "ic_define.h"
#include "query_maker.h"

class DBManager
{
public:
    DBManager();
    DBManager(int db_name_idx);
    ~DBManager();

    sqlite3* getDB();
    std::string& getTableNameOfTcpLog();
    int enqueueQuery(const std::string& query);

private:
    bool openDB(std::string db_path);
    int createTable(int db_name_idx);
    bool closeDB();
    void queryThread();
    int runQuery(const shared_ptr<std::string>& query);

private:
    std::unique_ptr<std::thread> queryThread_;
    MessageQueue<std::shared_ptr<std::string>> queQuery_;
    std::condition_variable cv_query_;
    std::mutex queryMutex_;
    std::string table_name_of_tcplog_;

    bool isOpen_ = false;
    bool isQueryThread_ = false;

    sqlite3* db_;
};

