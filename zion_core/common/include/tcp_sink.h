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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/15.
 *
 */

#pragma once

#include "spdlog/sinks/base_sink.h"
#include <fmt/format.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <string>


template<typename Mutex>
class tcp_sink : public spdlog::sinks::base_sink <Mutex>
{

public :
    tcp_sink(const std::string& host, int port) {
        // 소켓 생성
        socket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_ < 0) {
            throw std::runtime_error("Cannot open socket");
        }

        // 서버 주소 설정
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) {
            close(socket_);
            throw std::runtime_error("Invalid address/ Address not supported");
        }

        // 서버에 연결
        if (connect(socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            close(socket_);
            throw std::runtime_error("Connection Failed");
        }
    }

    ~tcp_sink() {
        close(socket_);
    }

protected:

    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        std::string message = fmt::to_string(formatted);

        send(socket_, message.c_str(), message.length(), 0);
    }

    void flush_() override
    {

    }

private:
    int socket_ = -1;

};
