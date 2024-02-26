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

// this define should be placed before the include of spdlog.h. DO NOT REMOVE!
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
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
#include <sqlite3.h>
#include "tcp_sink.h"

#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(Logger::logger_, __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(Logger::logger_, __VA_ARGS__)
#define LOG_INFO(...)  SPDLOG_LOGGER_INFO(Logger::logger_, __VA_ARGS__)
#define LOG_WARN(...)  SPDLOG_LOGGER_WARN(Logger::logger_, __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(Logger::logger_, __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(Logger::logger_, __VA_ARGS__)

#define LOGE_RETURN_VAL(cond, val) do{ if (!(cond)){LOG_TRACE(#cond "\n"); return (val);} } while(false)
#define LOGE_RETURN(cond) do{ if (!(cond)){LOG_TRACE(#cond "\n"); return;} } while(false)
#define LOGE_THROW_RUNTIME(cond) do{ if (!(cond)){LOG_ERROR(#cond "\n"); throw std::runtime_error(#cond "\n"); } } while(false)


class Logger
{
private :
private:
    enum class sink_enum
    {
        console_sink,
        file_sink,
        db_sink,
        tcp_sink,
        num_sink_type,
    };

public:
    Logger(std::array<bool, 4> sink_type);
    ~Logger();

    static void init();
    static void update_tcp_status(int socket, const std::string& name);

private :
    static void set_sink_type(std::array<bool, 4> sink_type);

public:
    static std::shared_ptr<spdlog::logger> logger_;

private:
    static std::array<bool, static_cast<int>(sink_enum::num_sink_type)> sink_type_list_;
    static std::shared_ptr<tcp_sink<std::mutex>> tcp_log_sink_;

};

