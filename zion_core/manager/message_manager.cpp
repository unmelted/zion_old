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

	pRMSGThread_ = std::make_unique<std::thread>(&MsgManager::rcvMSGThread, this);
	pSMSGThread_ = std::make_unique<std::thread>(&MsgManager::sndMSGThread, this);

}

void MsgManager::setICServer(std::shared_ptr<ICServer> icServer)
{
	icServer_ = icServer;
}

MsgManager::~MsgManager()
{

	isRMSGThread_ = false;
	if (pRMSGThread_ != nullptr)
	{
		pRMSGThread_->join();
		pRMSGThread_.reset();
	}

	isSMSGThread_ = false;
	if (pSMSGThread_ != nullptr)
	{
		pSMSGThread_->join();
		pSMSGThread_.reset();
	}
}

void MsgManager::rcvMSGThread()
{

	std::shared_ptr<ic::MSG_T> msg = nullptr;
	while (isRMSGThread_)
	{
		if (queRcvMSG_.IsQueue())
		{
			msg = queRcvMSG_.Dequeue();
			taskmanager_.onRcvTask(msg);
			if (msg != nullptr)
			{
				LOG_INFO("rcvMSGThread : {} ", msg->txt);
                Document recvDoc;
                recvDoc.Parse(msg->txt);

				if (recvDoc.HasMember("Action") == false || recvDoc.HasMember("SubCommand") == false)
				{
					LOG_WARN("Json component missing. can't execute.");
					continue;
				}
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

void MsgManager::onRcvMessage(std::string pData)
{
	std::shared_ptr<ic::MSG_T> ptrMsg = std::shared_ptr<ic::MSG_T>(new ic::MSG_T);
	ptrMsg->type = ic::PACKET_TYPE::TEXT;
	ptrMsg->txt = pData;
	queRcvMSG_.Enqueue(ptrMsg);
}

void MsgManager::onRcvSndMessage(std::string msg)
{
	// LOG_INFO("onRcvSndMessage : {}", msg );
	std::shared_ptr<std::string> pmsg = make_shared<std::string>(msg);
	queSndMSG_.Enqueue(pmsg);
}

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
