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


#include <string>
#include "logger.h"
#include "db_sink.h"
#include "tcp_sink.h"

std::shared_ptr<spdlog::logger> Logger::logger_;

Logger::Logger(std::shared_ptr<sqlite3> db)
{
    std::cout << "Logger Start!" << std::endl;
	init();

//    LOG_TRACE("ICManager Start!");
//    LOG_DEBUG("ICManager Start!");
//    LOG_INFO("ICManager Start!");
//    LOG_WARN("ICManager Start!");
//    LOG_ERROR("ICManager Start!");
//    LOG_CRITICAL("ICManager Start!");

}

Logger::~Logger()
{
	LOG_INFO("Logger End!");
}

// set the 1 to enable the sink, 0 to disable the sink
enum class sink_type
{
    console_sink = 1, //should 1. default sink.
    file_sink = 1,
    db_sink = 1,
    tcp_sink = 0,
};

void Logger::init()
{
	spdlog::flush_every(std::chrono::milliseconds(50));
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::level_enum::trace);

    shared_ptr<spdlog::sinks::rotating_file_sink_mt> file_sink;
    shared_ptr<db_sink<std::mutex>> db_log_sink;
    shared_ptr<tcp_sink<std::mutex>> tcp_log_sink;

//    spdlog::sinks_init_list sink_list = { console_sink };
    std::vector<std::shared_ptr<spdlog::sinks::sink>> sink_list;
    sink_list.push_back(console_sink);

    if(static_cast<int>(sink_type::file_sink) == 1)
    {
        std::string fileName("log/ic_");
        std::string date = Configurator::get().getCurrentDateTime("date");
        fileName += date + ".txt";
        std::cout << "--- log file name : " << fileName << std::endl;

        file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(fileName, 1024 * 1000 * 10, 10);
        file_sink->set_level(spdlog::level::level_enum::trace);
        sink_list.push_back(file_sink);
    }
    if(static_cast<int>(sink_type::db_sink) == 1)
    {
        db_log_sink = std::make_shared<db_sink<std::mutex>>();
        db_log_sink->initialize();
        db_log_sink->set_level(spdlog::level::trace);
        sink_list.push_back(db_log_sink);
    }

    if(static_cast<int>(sink_type::tcp_sink) == 1)
    {
        tcp_log_sink = std::make_shared<tcp_sink<std::mutex>>();
        tcp_log_sink->set_level(spdlog::level::level_enum::info);
        sink_list.push_back(tcp_log_sink);
    }

    logger_ = std::make_shared<spdlog::logger>("ic", sink_list.begin(), sink_list.end());

    for (auto& sink : logger_->sinks())
    {
        std::cout << " set sink list .. " << std::endl;
        sink->set_pattern("[%Y-%m-%d %X.%e] [%P] [%t] [%^%l%$] [%s:%#] %v");
    }

    logger_->set_level(spdlog::level::level_enum::trace);

    spdlog::register_logger(logger_);
	spdlog::set_default_logger(logger_);
    spdlog::set_level(spdlog::level::level_enum::trace);
}
