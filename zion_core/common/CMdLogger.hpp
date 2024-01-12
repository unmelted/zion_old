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
 * Created by Eunkyung Ma(ekma@livsmed.com) on 2024/01/05.
 *
 */


#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <iostream>

class CMdLogger
{
private:
	static std::shared_ptr<spdlog::logger> _logger;

public:
	CMdLogger();
	~CMdLogger();

	static void Init();
    static int GetLogV();
};

//#define SPd_INFO(...)	 ::Logger::GetConsoleLogger()->info(__VA_ARGS__);SPDLOG_INFO(__VA_ARGS__); ::Logger::GetFileLogger()->info(__VA_ARGS__);
#define CMd_ERROR(...)		SPDLOG_ERROR(__VA_ARGS__)
#define CMd_WARN(...)		SPDLOG_WARN(__VA_ARGS__)
#define CMd_INFO(...)		SPDLOG_INFO(__VA_ARGS__)
#define CMd_DEBUG(...)		SPDLOG_DEBUG(__VA_ARGS__)
#define CMd_CRITICAL(...)	SPDLOG_CRITICAL(__VA_ARGS__)
#define CMd_TRACE(...)		SPDLOG_TRACE(__VA_ARGS__)

