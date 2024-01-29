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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/25.
 *
 */

#pragma once

#include "ic_define.h"
#include "ic_util.h"
#include "spdlog/sinks/base_sink.h"
#include <fmt/format.h>
#include "db_manager.h"

class DBLogManager : public DBManager
{
public :
//    DBLogManager(std::string dbName);
//    ~DBLogManager();

protected:

};

template<typename Mutex>
class db_sink : public spdlog::sinks::base_sink <Mutex>
{
public :

    void set_db(std::shared_ptr<sqlite3> db)
    {
        db_ = std::move(db);
        try
        {
            char* errMsg = nullptr;
            std::string file_name = Configurator::get().getCurrentDateTime("datetime");
            std::string createQuery = "CREATE TABLE IF NOT EXISTS Log_" + file_name +
                                      " (date TEXT, level TEXT, file TEXT, msg TEXT)";
            int result = sqlite3_exec(db_.get(), createQuery.c_str(), NULL, NULL, &errMsg);
            std::cout << "Create table query : " << createQuery << " Result : " << result << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "create table query error : " << e.what() << std::endl;
        }
    }

    std::shared_ptr<sqlite3> db_;

protected:

    void sink_it_(const spdlog::details::log_msg& msg) override
    {

        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        std::cout << "test logger " << fmt::to_string(formatted);
        std::shared_ptr<ic::MSG_T> logMsg = std::make_shared<ic::MSG_T>();
        logMsg->type = (int)ic::MSG_TYPE::MSG_TYPE_LOG;
        logMsg->txt = fmt::to_string(formatted);
        dbLogManager_->enqueueQuery(logMsg);

    }

    void flush_() override
    {
        std::cout << std::flush;
    }

private:
    std::unique_ptr<DBLogManager> dbLogManager_;
};
