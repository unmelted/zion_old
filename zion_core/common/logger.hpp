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
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <iostream>
#include <sstream>
#include <fmt/format.h>

void saveLogToDatabase(const std::string& message);

#define LOG_ERROR(...) do { \
        SPDLOG_ERROR(__VA_ARGS__); \
        std::ostringstream logStream; \
        logStream << __VA_ARGS__;  \
        saveLogToDatabase(logStream.str()); \
    } while(0)

#define LOG_WARN(...)	do { \
        SPDLOG_WARN(__VA_ARGS__);  \
        std::ostringstream logStream; \
        logStream << __VA_ARGS__;  \
        saveLogToDatabase(logStream.str()); \
    } while(0)

#define LOG_INFO(...) 	do { \
        SPDLOG_INFO(__VA_ARGS__); \
        std::string logMessage = fmt::format(__VA_ARGS__);  \
        saveLogToDatabase(logMessage); \
    } while(0)

#define LOG_DEBUG(...)  do { \
        SPDLOG_DEBUG(__VA_ARGS__); \
        std::ostringstream logStream; \
        logStream << __VA_ARGS__;  \
        saveLogToDatabase(logStream.str()); \
    } while(0)

#define LOG_CRITICAL(...)  do { \
        SPDLOG_CRITICAL(__VA_ARGS__); \
        std::ostringstream logStream; \
        logStream << __VA_ARGS__;  \
        saveLogToDatabase(logStream.str()); \
    } while(0)

#define LOG_TRACE(...)  do { \
        SPDLOG_TRACE(__VA_ARGS__); \
        std::ostringstream logStream; \
        logStream << __VA_ARGS__;  \
        saveLogToDatabase(logStream.str()); \
    } while(0)


class Logger
{
public:
    Logger();
    ~Logger();

    static void init();

private:
    static std::shared_ptr<spdlog::logger> _logger;

};

