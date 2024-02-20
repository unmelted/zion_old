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


#pragma once
#include "ic_define.h"
#include "message_sender.h"
#include "db_manager.h"

using namespace rapidjson;

class SocketMsgManager
{

public:

    SocketMsgManager();
    ~SocketMsgManager();

    void setDBManager(std::shared_ptr<DBManager>& dbManager);
    void onRcvMessage(const ic::ServerInfo info, ic::MSG_T msg_t);
    void onRcvSndMessage(const ic::ServerInfo& info, const ic::MSG_T& msg);
    void insertEventTable(const Document& doc, int msg_type);

protected :
    void rcvMSGThread();
    void sndMSGThread();

private :
    std::unique_ptr<std::thread> rcvMSGThread_;
    std::unique_ptr<std::thread> sndMSGThread_;
    MessageQueue<std::shared_ptr<std::pair<ic::ServerInfo, ic::MSG_T>>> queRcvMSG_;
    MessageQueue<std::shared_ptr<std::pair<ic::ServerInfo, ic::MSG_T>>> queSndMSG_;

    std::shared_ptr<DBManager> dbManager_;
    std::unique_ptr<MessageSender> msgSender_;

    bool isRMSGThread_;
    bool isSMSGThread_;
};