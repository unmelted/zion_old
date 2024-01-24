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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/05.
 *
 */

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include "spdlog/sinks/base_sink.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <iostream>
#include <sstream>
#include <fmt/format.h>
#include <sqlite3.h>


#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)


class Logger
{
public:
    Logger(std::shared_ptr<sqlite3> db = nullptr);
    ~Logger();

    static void init(const std::shared_ptr<sqlite3> db = nullptr);

private:
    static std::shared_ptr<spdlog::logger> logger_;

};

template<typename Mutex>
class db_sink : public spdlog::sinks::base_sink <Mutex>
{
public :
    void set_db(std::shared_ptr<sqlite3> db, std::string& fileName)
    {
        db_ = db;
        char* errMsg = nullptr;
        std::string createQuery = "CREATE TABLE IF NOT EXISTS log0124_ (date TEXT, level TEXT, file TEXT, msg TEXT)";
        sqlite3_exec(db_.get(), createQuery.c_str(), NULL, NULL, NULL);
        std::cout << "create table query : " << std::endl;
    }

    std::shared_ptr<sqlite3> db_;

protected:

    void sink_it_(const spdlog::details::log_msg& msg) override
    {

        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        std::cout << "test logger " << fmt::to_string(formatted);

    }

    void flush_() override
    {
        std::cout << std::flush;
    }
};
