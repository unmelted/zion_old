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
using json = nlohmann::json;

MsgManager::MsgManager()
	: taskmanager_(TASKPOOL_SIZE, this)
{
	isRMSGThread_ = true;
	isSMSGThread_ = true;
	pRMSGThread_ = new std::thread(&MsgManager::rcvMSGThread, this, this);
	pSMSGThread_ = new std::thread(&MsgManager::sndMSGThread, this, this);
//	std::function<void(MsgManager &, const std::string msg)> f1 = &MsgManager::onRcvSndMessage;
//	taskmanager_.setSndQue(f1);
}

ICServer *MsgManager::getICServer()
{
	return icServer_;
}


void MsgManager::setICServer(ICServer *dmServer)
{
	icServer_ = dmServer;
}

MsgManager::~MsgManager()
{

	isRMSGThread_ = false;
	if (pRMSGThread_ != nullptr)
	{
		pRMSGThread_->join();
		delete pRMSGThread_;
		pRMSGThread_ = nullptr;
	}

	isSMSGThread_ = false;
	if (pSMSGThread_ != nullptr)
	{
		pSMSGThread_->join();
		delete pSMSGThread_;
		pSMSGThread_ = nullptr;
	}
}

void *MsgManager::rcvMSGThread(void *arg)
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
				CMd_INFO("rcvMSGThread : {} ", msg->txt);
				json j = json::parse(msg->txt);
				if (j.contains("Action") == false || j.contains("SubCommand") == false)
				{
					CMd_WARN("Json component missing. can't execute.");
					continue;
				}
				string section3 = j["SubCommand"];
				string action = j["Action"];
				// if (action == "Stabilization" || section3 == "Stabilize") {
				// 	taskmanager_.commandTask(ic::POST_STABILIZATION, msg->txt);
				// }
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

	return nullptr;
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
	// CMd_INFO("onRcvSndMessage : {}", msg );
	std::shared_ptr<std::string> pmsg = make_shared<std::string>(msg);
	queSndMSG_.Enqueue(pmsg);
}

void *MsgManager::sndMSGThread(void *arg)
{

	std::shared_ptr<std::string> msg = nullptr;
	while (isSMSGThread_)
	{

		if (queSndMSG_.IsQueue())
		{
			msg = queSndMSG_.Dequeue();
			CMd_INFO(" SndMsg thread msg : {} ", msg->c_str());
            std::string temp_clientname = "name_temp";
			getICServer()->sendData(temp_clientname, msg->c_str());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

	return nullptr;
}
