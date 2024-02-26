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
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "ic_convertor.h"

using namespace rapidjson;
template<typename Mutex>
class tcp_sink : public spdlog::sinks::base_sink <Mutex>
{

public :
    Document sendDocument;
    Document::AllocatorType* allocator;

    tcp_sink()
    : isConnected(false)
    , socket_(-1)
    {
        sendDocument.SetObject();
        allocator = &sendDocument.GetAllocator();
        sendDocument.AddMember("Type", "LOG", *allocator);
        sendDocument.AddMember("Command", "TCP_LOG", *allocator);
    }

    ~tcp_sink()
    {
        close(socket_);
    }

    void update_tcp_status(int socket, const std::string& name)
    {
        std::cout << "update tcp status : " << name << std::endl;
        if(name != "")
        {
            isConnected = true;
            socket_ = socket;
            sendDocument.AddMember("From", Value(name.c_str(), *allocator), *allocator);
        }
        else
        {
            isConnected = false;
            socket_ = -1;
        }
    }

protected:

    char cType = static_cast<char>(0);
    std::vector<char> sendBuffer_;
    std::mutex bufferMutex_;

    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        if (!isConnected or socket_ < 0)
        {
            return;
        }

        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        std::string message = fmt::to_string(formatted);

        sendDocument.AddMember("Data", Value(message.c_str(), *allocator), *allocator);
        std::string strSendString = convertDocumentForSend(sendDocument);

        int nSize = static_cast<int>(strSendString.size());
        int nSendSize = sizeof(int) + 1 + nSize;

        std::lock_guard<std::mutex> lock(bufferMutex_);

        sendBuffer_.reserve(nSendSize);
        sendBuffer_.insert(sendBuffer_.end(), reinterpret_cast<char*>(&nSize), reinterpret_cast<char*>(&nSize) + sizeof(int));
        sendBuffer_.push_back(cType);
        sendBuffer_.insert(sendBuffer_.end(), strSendString.begin(), strSendString.end());

//        std::cout << "tcp_sink : " << message << std::endl;
        int nSend = -1;
        nSend = send(socket_, sendBuffer_.data(), sendBuffer_.size(), 0);
//            send(socket_, message.c_str(), message.length(), 0);
        sendBuffer_.clear();
        std::vector<char>().swap(sendBuffer_);
        sendDocument.RemoveMember("Data");

    }

    void flush_() override
    {

    }

private:
    int socket_ = -1;
    bool isConnected = false;

};
