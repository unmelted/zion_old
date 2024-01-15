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


#include <sstream>
#include <iomanip>
#include <string>
#include "logger.hpp"
#include "ics_util.hpp"
#include "json.hpp"
using json = nlohmann::json;

enum class CMdLoggerLevel : int
{
	NONE = -1,
	CMD_LOG_LEVEL_TRACE = 0,
	CMD_LOG_LEVEL_DEBUG,
	CMD_LOG_LEVEL_INFO,
	CMD_LOG_LEVEL_WARN,
	CMD_LOG_LEVEL_ERROR,
	CMD_LOG_LEVEL_CRITICAL,
	CMD_LOG_LEVEL_OFF
};

//const std::string CONFIG_INI_NAME = "./config.ini";

std::shared_ptr<spdlog::logger> CMdLogger::_logger;

CMdLogger::CMdLogger()
{
	Init();
}

CMdLogger::~CMdLogger()
{
	CMd_INFO("Logger End!");
}

int CMdLogger::GetLogV() {

	json j;
	int log_init = 1;
	std::ifstream json_file(Configurator::Path::INIF);
	if (json_file) {
		json_file >> j;
		if (j.contains("logv") == true) {
			log_init = j["logv"];
			CMd_INFO("Start log file : {}", log_init);
		}
	}

	return log_init;
}

void CMdLogger::Init()
{
	spdlog::flush_every(std::chrono::seconds(1));
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace);
	auto console_link = std::make_shared<spdlog::sinks::dist_sink_mt>();
	console_link->add_sink(console_link);

	std::string fileName("log/CMd_");
	fileName += Configurator::Get().getCurrentDateTime("date") + "_" + std::to_string(GetLogV()) + ".txt";

	auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(fileName, 1024 * 1000 * 10, 10);
	file_sink->set_level(spdlog::level::trace);
	auto file_link = std::make_shared < spdlog::sinks::dist_sink_mt>();
	file_link->add_sink(file_sink);

	spdlog::sinks_init_list sink_list = { console_sink, file_sink };

	_logger = std::make_shared<spdlog::logger>("CMd", sink_list);
	_logger->set_level(spdlog::level::debug);	
	/*
	int get = 1;
	switch (get)
	{
	case 0:
		_logger->set_level(spdlog::level::trace);
		break;
	case 1:
		_logger->set_level(spdlog::level::debug);
		break;
	case 2:
		_logger->set_level(spdlog::level::info);
		break;
	case 3:
		_logger->set_level(spdlog::level::warn);
		break;
	case 4:
		_logger->set_level(spdlog::level::err);
		break;
	case 5:
		_logger->set_level(spdlog::level::critical);
		break;
	}*/

	// Note: Writing PID to the log file is mandatory because MMd(MMC, esp.) can execute multiple processes in one PC.
	//       You can't get proper log files without it because spdlog doesn't support the multiple process.

	spdlog::set_default_logger(_logger);
	//spdlog::set_pattern("[%Y-%m-%d %X.%e] [PID:%P] [thread %t] [%^%l%$] [%s:%#] - %v");
	spdlog::set_pattern("[%Y-%m-%d %X.%e] [%^%l%$] [%s:%#] - %v");	
}