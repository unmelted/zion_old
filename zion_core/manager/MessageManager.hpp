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
 * Created by Eunkyung Ma(ekma@livsmed.com) on 2024/01/05.
 *
 */


#pragma once 
#include "CMDefine.hpp"
#include "TaskManager.hpp"

class MsgManager {

public:

    MsgManager();
    ~MsgManager();
	void OnRcvMessage(std::string pData);
	void OnRcvSndMessage(std::string msg);
	void SetDMServer(DMServer* dmServer);
	DMServer* GetDMServer();

private :
	DMServer* m_dmServer;
    bool b_RMSGThread;
	std::thread* m_pRMSGThread{ nullptr };
    bool b_SMSGThread;
	std::thread* m_pSMSGThread{ nullptr };
	MessageQueue<std::shared_ptr<CMD::MSG_T>> m_qRMSG;
	MessageQueue<std::shared_ptr<std::string>> m_qSMSG;    
	CMdLogger _logger;	

	TaskPool::TaskManager m_taskmanager;
    void* RcvMSGThread(void* arg);
    void* SndMSGThread(void* arg);
    void SendVersionMessage(std::shared_ptr<CMD::MSG_T> ptrMsg);
};