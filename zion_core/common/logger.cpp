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
#include "logger.hpp"
#include "ic_util.hpp"

std::shared_ptr<spdlog::logger> Logger::_logger;

Logger::Logger()
{
    std::cout << "Logger Start!" << std::endl;
	init();
}

Logger::~Logger()
{
	LOG_INFO("Logger End!");
}

void Logger::init()
{
	spdlog::flush_every(std::chrono::milliseconds(100));
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace);

	std::string fileName("log/ic_");
	fileName += Configurator::get().getCurrentDateTime("date") + ".txt";
//    std::cout << "--- log file name : " << fileName << std::endl;

	auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(fileName, 1024 * 1000 * 10, 10);
	file_sink->set_level(spdlog::level::trace);

	spdlog::sinks_init_list sink_list = { console_sink, file_sink };

	_logger = std::make_shared<spdlog::logger>("ic", sink_list);
	_logger->set_level(spdlog::level::trace);

	spdlog::set_default_logger(_logger);
	spdlog::set_pattern("[%Y-%m-%d %X.%e] [%^%l%$] [%s:%#] - %v");
}

void saveLogToDatabase(const std::string& logMessage)
{
    try
    {

        std::cout << "Saved to database: " << logMessage << std::endl;

    } catch (const std::exception& e)
    {
        // 예외 처리: 데이터베이스 연결 또는 저장 중에 오류 발생 시
        std::cerr << "Error saving log to database: " << e.what() << std::endl;
    }
}
