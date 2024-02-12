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
#include "message_manager.h"

using namespace rapidjson;

MsgManager::MsgManager()
	: taskmanager_(TASKPOOL_SIZE, this)
{
	isRMSGThread_ = true;
	isSMSGThread_ = true;

	rcvMSGThread_ = std::make_unique<std::thread>(&MsgManager::rcvMSGThread, this);
	sndMSGThread_ = std::make_unique<std::thread>(&MsgManager::sndMSGThread, this);

}

MsgManager::~MsgManager()
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

// set the ic_server
void MsgManager::setICServer(std::shared_ptr<ICServer> icServer)
{
    icServer_ = icServer;
}


void MsgManager::setDBManager(std::shared_ptr<DBManager> dbManager)
{
    this->dbManager_ = dbManager;
}

// this function is called by task_manager for sending a message.
// this function store the msg in queue for sengin.
void MsgManager::onRcvSndMessage(std::string target, std::string msg)
{
    std::pair<std::string, std::string> pair_msg(target, msg);
    std::shared_ptr<std::pair<std::string, std::string>> pmsg = std::make_shared<std::pair<std::string, std::string>>(pair_msg);
	queSndMSG_.Enqueue(pmsg);
}

// this function is called by ic_manager
// ic_manager call with data which include information for job (task)
// store the data in RcvMSG
void MsgManager::onRcvMessage(std::string target, std::string pData)
{
    std::shared_ptr<ic::MSG_T> ptrMsg = std::shared_ptr<ic::MSG_T>(new ic::MSG_T);
    ptrMsg->target = target;
    ptrMsg->txt = pData;
    queRcvMSG_.Enqueue(ptrMsg);
}

// onRcvMessage function store the data in RcvMSG que,
// then this thread detect the data and process
void MsgManager::rcvMSGThread()
{

    std::shared_ptr<ic::MSG_T> msg = nullptr;
    while (isRMSGThread_)
    {
        if (queRcvMSG_.IsQueue())
        {
            msg = queRcvMSG_.Dequeue();
            taskmanager_.onRcvTask(msg); // call for storing the msg in taskMSG que.
            if (msg != nullptr)
            {
                LOG_INFO("rcvMSGThread : {} ", msg->txt);
                Document recvDoc;
                recvDoc.Parse(msg->txt);

                string section2 = recvDoc["Command"].GetString();
                string action = recvDoc["Action"].GetString();
                 if (section2 == "START") {
                 	taskmanager_.commandTask((int)ic::COMMAND_CLASS::COMMAND_START, msg->txt);
                 }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

}

// if receive the data in queSndMSG, this data should be sent through ic_server.
void MsgManager::sndMSGThread()
{

	while (isSMSGThread_)
	{
		if (queSndMSG_.IsQueue())
		{
            std::shared_ptr<std::pair<std::string, std::string>> dequeuedItem = queSndMSG_.Dequeue();

            std::string target = dequeuedItem->first;
            std::string msg = dequeuedItem->second;

			LOG_INFO(" SndMsg thread target {} msg  {} ", target, msg);
			icServer_->sendData(target, msg);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

}

void MsgManager::insertEventTable(const Document& doc, int msg_type)
{
    std::string query = QueryMaker::makeEventInsertQuery(doc);

    std::shared_ptr<ic::MSG_T> msg_t = std::make_shared<ic::MSG_T>();
    msg_t->type = msg_type;
    msg_t->txt = query;
    dbManager_->enqueueQuery(msg_t);
}
