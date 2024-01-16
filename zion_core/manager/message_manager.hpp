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


#pragma once 
#include "task_manager.hpp"

class MsgManager {

public:

    MsgManager();
    ~MsgManager();
	void onRcvMessage(std::string pData);
	void onRcvSndMessage(std::string msg);
	void setDMServer(DMServer* dmServer);
	DMServer* setDMServer();

private :
    void* rcvMSGThread(void* arg);
    void* sndMSGThread(void* arg);
    void sendVersionMessage(std::shared_ptr<ic::MSG_T> ptrMsg);

    DMServer* m_dmServer;
    bool b_RMSGThread;
    std::thread* m_pRMSGThread{ nullptr };
    bool b_SMSGThread;
    std::thread* m_pSMSGThread{ nullptr };
    MessageQueue<std::shared_ptr<ic::MSG_T>> m_qRMSG;
    MessageQueue<std::shared_ptr<std::string>> m_qSMSG;
    CMdLogger _logger;

    TaskManager m_taskmanager;
};