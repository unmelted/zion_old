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

    LOG_TRACE("ICManager Start!");
    LOG_DEBUG("ICManager Start!");
    LOG_INFO("ICManager Start!");
    LOG_WARN("ICManager Start!");
    LOG_ERROR("ICManager Start!");
    LOG_CRITICAL("ICManager Start!");

}

Logger::~Logger()
{
	LOG_INFO("Logger End!");
}

void Logger::init()
{
	spdlog::flush_every(std::chrono::milliseconds(50));
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace);

	std::string fileName("log/ic_");
    std::string date = Configurator::get().getCurrentDateTime("date");
	fileName += date + ".txt";
//    std::cout << "--- log file name : " << fileName << std::endl;

	auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(fileName, 1024 * 1000 * 10, 10);
	file_sink->set_level(spdlog::level::trace);

//    auto db_log_sink = std::make_shared<db_sink<std::mutex>>();
//    db_log_sink->set_db();
//    db_log_sink->set_level(spdlog::level::trace);

    spdlog::sinks_init_list sink_list = { console_sink, file_sink };
	logger_ = std::make_shared<spdlog::logger>("ic", sink_list);
	logger_->set_level(spdlog::level::level_enum::trace);
    logger_->sinks()[0]->set_pattern("[%Y-%m-%d %X.%e] [%P] [%t] [%^%l%$] [%s:%#] %v");
    logger_->sinks()[1]->set_pattern("[%Y-%m-%d %X.%e] [%P] [%t] [%^%l%$] [%s:%#] %v");

    spdlog::register_logger(logger_);
	spdlog::set_default_logger(logger_);
    spdlog::set_level(spdlog::level::level_enum::trace);
}
