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
	void setICServer(std::shared_ptr<ICServer> icServer);
//	ICServer* getICServer();

private :
    void* rcvMSGThread(void* arg);
    void* sndMSGThread(void* arg);

    std::shared_ptr<ICServer> icServer_;
    std::thread* pRMSGThread_{ nullptr };
    std::thread* pSMSGThread_{ nullptr };
    MessageQueue<std::shared_ptr<ic::MSG_T>> queRcvMSG_;
    MessageQueue<std::shared_ptr<std::string>> queSndMSG_;
//    Logger _logger;

    TaskManager taskmanager_;

    bool isRMSGThread_;
    bool isSMSGThread_;
};