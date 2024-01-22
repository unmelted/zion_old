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
#include "message_manager.hpp"

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

// this function is called by task_manager for sending a message.
// this function store the msg in queue for sengin.
void MsgManager::onRcvSndMessage(std::string msg)
{
	std::shared_ptr<std::string> pmsg = make_shared<std::string>(msg);
	queSndMSG_.Enqueue(pmsg);
}

// this function is called by ic_manager
// ic_manager call with data which include information for job (task)
// store the data in RcvMSG
void MsgManager::onRcvMessage(std::string pData)
{
    std::shared_ptr<ic::MSG_T> ptrMsg = std::shared_ptr<ic::MSG_T>(new ic::MSG_T);
    ptrMsg->type = ic::PACKET_TYPE::TEXT;
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


                string section3 = recvDoc["SubCommand"].GetString();
                string action = recvDoc["Action"].GetString();
                // if (action == "Stabilization" || section3 == "Stabilize") {
                // 	taskmanager_.commandTask(ic::POST_STABILIZATION, msg->txt);
                // }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

}

// if receive the data in queSndMSG, this data should be sent through ic_server.
void MsgManager::sndMSGThread()
{
	std::shared_ptr<std::string> msg = nullptr;
	while (isSMSGThread_)
	{
		if (queSndMSG_.IsQueue())
		{
 			msg = queSndMSG_.Dequeue();
			LOG_INFO(" SndMsg thread msg : {} ", msg->c_str());
            std::string temp_clientname = "name_temp";
			icServer_->sendData(temp_clientname, msg->c_str());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

}

