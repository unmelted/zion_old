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

std::shared_ptr<spdlog::logger> Logger::logger_;
std::array<bool, static_cast<int>(Logger::sink_enum::num_sink_type)> Logger::sink_type_list_;
std::shared_ptr<tcp_sink<std::mutex>> Logger::tcp_log_sink_;

Logger::Logger(std::array<bool, 4> sink_type)
{
    std::cout << "Logger Start!" << std::endl;
    set_sink_type(sink_type);
    init();
}

Logger::~Logger()
{
    spdlog::shutdown();
    LOG_INFO("Logger End!");
}

void Logger::set_sink_type(std::array<bool, 4> sink_type)
{
    for (int i = 0; i < sink_type.size(); i++)
    {
        sink_type_list_[i] = sink_type[i];
        std::cout << " set sink type : " << i << " = " << sink_type_list_[i] << std::endl;
    }
}

void Logger::update_tcp_status(int socket)
{
    std::cout << "update_tcp_status in Logger : " << socket << std::endl;
    if(sink_type_list_[static_cast<int>(sink_enum::tcp_sink)] and socket > 0)
    {
        tcp_log_sink_->update_tcp_status(socket);
    }
}

void Logger::init()
{
    spdlog::flush_every(std::chrono::milliseconds(50));
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    std::cout << "console_sink is on. (default) " << std::endl;

    shared_ptr<spdlog::sinks::rotating_file_sink_mt> file_sink;
    shared_ptr<db_sink<std::mutex>> db_log_sink;
    std::vector<std::shared_ptr<spdlog::sinks::sink>> sink_list;

    sink_list.push_back(console_sink);

    if(sink_type_list_[static_cast<int>(sink_enum::file_sink)])
    {
        std::string fileName("log/ic_");
        std::string date = Configurator::get().getCurrentDateTime("datetime");
        fileName += date + ".txt";
        std::cout << "file sink is on. : " << fileName << std::endl;

        file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(fileName, 1024 * 1000 * 10, 10);
        file_sink->set_level(spdlog::level::level_enum::trace);
        sink_list.push_back(file_sink);
    }

    if(sink_type_list_[static_cast<int>(sink_enum::db_sink)])
    {
        db_log_sink = std::make_shared<db_sink<std::mutex>>();
        if (db_log_sink->initialize() > 0)
        {
            db_log_sink->set_level(spdlog::level::trace);
            sink_list.push_back(db_log_sink);
            std::cout << "db_sink is on. " << std::endl;
        }
    }

    if(sink_type_list_[static_cast<int>(sink_enum::tcp_sink)])
    {
        tcp_log_sink_ = std::make_shared<tcp_sink<std::mutex>>();
        tcp_log_sink_->set_level(spdlog::level::level_enum::info);
        sink_list.push_back(tcp_log_sink_);
        std::cout << "tcp_sink is on. " << std::endl;
    }

    logger_ = std::make_shared<spdlog::logger>("ic", sink_list.begin(), sink_list.end());
    logger_->set_level(spdlog::level::level_enum::trace);

    for (auto& sink : logger_->sinks())
    {
        sink->set_pattern("[%Y-%m-%d %X.%e] [%P] [%t] [%^%l%$] [%s:%#] %v");
        sink->set_level(spdlog::level::level_enum::trace);
    }

    spdlog::register_logger(logger_);
    spdlog::set_default_logger(logger_);
    spdlog::set_level(spdlog::level::level_enum::trace);

    std::cout << "Logger init done!" << std::endl;
}