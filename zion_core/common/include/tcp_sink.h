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
#include "ic_define.h"

template<typename Mutex>
class tcp_sink : public spdlog::sinks::base_sink <Mutex>
{

public :
    tcp_sink()
    : isConnected(false)
    , socket_(-1)
    {

    }

    ~tcp_sink()
    {
        close(socket_);
    }

    void update_tcp_status(int socket)
    {
        std::cout << "update tcp status : " << socket << std::endl;
        if(socket > 0)
        {
            socket_ = socket;
            isConnected = true;
        }
        else
        {
            socket = -1;
            isConnected = false;
        }
    }


protected:

    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        if(tcp_sink::isConnected and socket_ > 0)
        {
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
            std::string message = fmt::to_string(formatted);
            std::cout << "tcp_sink : " << message << std::endl;
            send(socket_, message.c_str(), message.length(), 0);
        }
    }

    void flush_() override
    {

    }

private:
    int socket_;
    bool isConnected;

};
