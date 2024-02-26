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

#include "spdlog/sinks/base_sink.h"
#include <regex>

#include "db_manager.h"

std::vector<std::pair<std::string, std::string>> parseLogString(const std::string& log)
{
    std::vector<std::pair<std::string, std::string>> columns;
    std::regex re("\\[(.*?)\\]");
    int index = 0;
    auto words_begin = std::sregex_iterator(log.begin(), log.end(), re);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
    {
        const std::smatch& match = *i;
        std::string match_str = match.str(1);
        columns.push_back({DB_LOG_COLUMN_NAME[index], match_str});
        index ++;
    }

    size_t lastPos = log.find_last_of(']') + 1;
    if (lastPos < log.size())
    {
        std::string lastColumn = log.substr(lastPos, log.size() - lastPos);
        columns.push_back({"msg", lastColumn});
    }

    return columns;
}

template<typename Mutex>
class db_sink : public spdlog::sinks::base_sink <Mutex>
{
public :

    int initialize()
    {
        dbLogManager_ = std::make_unique<DBManager>(static_cast<int>(ic::DB_TYPE::DB_TYPE_LOG));

        try
        {
            char* errMsg = nullptr;
            table_name = "Log_" + Configurator::get().getCurrentDateTime("datetime");
            std::string createQuery = "CREATE TABLE IF NOT EXISTS " + table_name +
                                      " (cur_date DATETIME DEFAULT (strftime('%Y-%m-%d %H:%M:%f', 'now'))," +
                                      " date TEXT, pid TEXT, tid TEXT, level TEXT, file TEXT, msg TEXT)";
            int result = sqlite3_exec(dbLogManager_->getDB(), createQuery.c_str(), NULL, NULL, &errMsg);
            std::cout << "Create table query : " << createQuery << " Result : " << result << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "create table query error : " << e.what() << std::endl;
            return -1;
        }

        return 1;
    }

    std::shared_ptr<sqlite3> db_;

protected:

    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
//        std::cout << "db_sink logger " << fmt::to_string(formatted);
        std::string format_str = fmt::to_string(formatted);
        auto qeury_str = parseLogString(format_str);
        auto query = QueryMaker::makeLogInsertQuery(table_name, qeury_str);
        dbLogManager_->enqueueQuery(query);
    }

    void flush_() override
    {
//        dbLogManager_->commitTransaction();
    }

public:
    std::string table_name;
    std::unique_ptr<DBManager> dbLogManager_;
};
