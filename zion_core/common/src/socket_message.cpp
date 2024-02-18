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
 * Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/14.
 *
 */
#include "socket_message.h"

using namespace rapidjson;

SocketMsgManager::SocketMsgManager()
{
    isRMSGThread_ = true;
    isSMSGThread_ = true;

    rcvMSGThread_ = std::make_unique<std::thread>(&SocketMsgManager::rcvMSGThread, this);
    sndMSGThread_ = std::make_unique<std::thread>(&SocketMsgManager::sndMSGThread, this);

    msgSender_ = std::make_unique<MessageSender>();
}

SocketMsgManager::~SocketMsgManager()
{
    isRMSGThread_ = false;
    if (rcvMSGThread_ != nullptr)
    {
        rcvMSGThread_->join();
        rcvMSGThread_.reset();
    }

    isSMSGThread_ = false;
    if (sndMSGThread_ != nullptr)
    {
        sndMSGThread_->join();
        sndMSGThread_.reset();
    }
}

void SocketMsgManager::setDBManager(std::shared_ptr<DBManager>& dbManager)
{
    this->dbManager_ = dbManager;
}

// this function is called by task_manager for sending a message.
// this function store the msg in queue for sending.

void SocketMsgManager::onRcvSndMessage(const ic::ServerInfo& info, std::string msg)
{
    std::pair<ic::ServerInfo, std::string> pair_msg(info, msg);
    std::shared_ptr<std::pair<ic::ServerInfo, std::string>> pMsg = std::make_shared<std::pair<ic::ServerInfo, std::string>>(pair_msg);
    queSndMSG_.Enqueue(pMsg);
}

// this function is called by ic_manager
// ic_manager call with data which include information for job (task)
// store the data in RcvMSG

void SocketMsgManager::onRcvMessage(const ic::ServerInfo info, const ic::MSG_T msg_t)
{
    auto pMsg = std::make_shared<std::pair<ic::ServerInfo, ic::MSG_T>>(info, msg_t);
    queRcvMSG_.Enqueue(pMsg);
}

// onRcvMessage function store the data in RcvMSG que,
// then this thread detect the data and process

void SocketMsgManager::rcvMSGThread()
{
    std::shared_ptr<std::pair<ic::ServerInfo, ic::MSG_T>> msg = nullptr;
    while (isRMSGThread_)
    {
        if (queRcvMSG_.IsQueue())
        {
            msg = queRcvMSG_.Dequeue();
//            taskmanager_.onRcvTask(msg); // call for storing the msg in taskMSG que.
            if (msg != nullptr)
            {
//                LOG_INFO("rcvMSGThread : {} ", msg->txt);
//                Document recvDoc;
//                recvDoc.Parse(msg->txt);
//
//                string section2 = recvDoc["Command"].GetString();
//                string action = recvDoc["Action"].GetString();
//                if (section2 == "START") {
//                    taskmanager_.commandTask((int)ic::COMMAND_CLASS::COMMAND_START, msg->txt);
//                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

}

// if receive the data in queSndMSG, this data should be sent through ic_server.

void SocketMsgManager::sndMSGThread()
{
    while (isSMSGThread_)
    {
        if (queSndMSG_.IsQueue())
        {
            std::shared_ptr<std::pair<ic::ServerInfo, std::string>> dequeuedItem = queSndMSG_.Dequeue();
            int socket = -1;
            ic::ServerInfo info = dequeuedItem->first;
            std::string msg = dequeuedItem->second;

            LOG_INFO(" SndMsg thread target {} msg  {} ", info.ip, msg);
            msgSender_->parseAndSend(info, msg);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

}


void SocketMsgManager::insertEventTable(const Document& doc, int msg_type)
{
    std::string query = QueryMaker::makeEventInsertQuery(doc);
    dbManager_->enqueueQuery(query);
}
