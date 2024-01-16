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
	: m_taskmanager(TASKPOOL_SIZE, this)
{
	b_RMSGThread = true;
	b_SMSGThread = true;
	m_pRMSGThread = new std::thread(&MsgManager::rcvMSGThread, this, this);
	m_pSMSGThread = new std::thread(&MsgManager::sndMSGThread, this, this);
	std::function<void(MsgManager &, const std::string msg)> f1 = &MsgManager::onRcvSndMessage;
	m_taskmanager.setSndQue(f1);
}

ICServer *MsgManager::getICServer()
{
	return icServer;
}


void MsgManager::setICServer(ICServer *dmServer)
{
	icServer = dmServer;
}

MsgManager::~MsgManager()
{

	b_RMSGThread = false;
	if (m_pRMSGThread != nullptr)
	{
		m_pRMSGThread->join();
		delete m_pRMSGThread;
		m_pRMSGThread = nullptr;
	}

	b_SMSGThread = false;
	if (m_pSMSGThread != nullptr)
	{
		m_pSMSGThread->join();
		delete m_pSMSGThread;
		m_pSMSGThread = nullptr;
	}
}

void *MsgManager::rcvMSGThread(void *arg)
{

	std::shared_ptr<ic::MSG_T> msg = nullptr;
	while (b_RMSGThread)
	{
		if (m_qRMSG.IsQueue())
		{
			msg = m_qRMSG.Dequeue();
			m_taskmanager.onRcvTask(msg);
			if (msg != nullptr)
			{
				CMd_INFO("rcvMSGThread : {} ", msg->txt);
				json j = json::parse(msg->txt);
				if (j.contains("Action") == false || j.contains("Section3") == false)
				{
					CMd_WARN("Json component missing. can't execute.");
					continue;
				}
				string section3 = j["Section3"];
				string action = j["Action"];
				// if (action == "Stabilization" || section3 == "Stabilize") {
				// 	m_taskmanager.commandTask(ic::POST_STABILIZATION, msg->txt);
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
	m_qRMSG.Enqueue(ptrMsg);
}

void MsgManager::onRcvSndMessage(std::string msg)
{
	// CMd_INFO("onRcvSndMessage : {}", msg );
	std::shared_ptr<std::string> pmsg = make_shared<std::string>(msg);
	m_qSMSG.Enqueue(pmsg);
}

void *MsgManager::sndMSGThread(void *arg)
{

	std::shared_ptr<std::string> msg = nullptr;
	while (b_SMSGThread)
	{

		if (m_qSMSG.IsQueue())
		{
			msg = m_qSMSG.Dequeue();
			CMd_INFO(" SndMsg thread msg : {} ", msg->c_str());
            std::string temp_clientname;
			getICServer()->sendData(temp_clientname, msg->c_str());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}

	return nullptr;
}
