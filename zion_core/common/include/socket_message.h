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
    void onRcvMessage(const ic::ServerInfo info, ic::IC_MSG msg_t);
    void onRcvSndMessage(const ic::ServerInfo& info, const ic::IC_MSG& msg);
    void insertEventTable(const Document& doc);
    int errorMsg(int err_id, const char* file, int line);

protected :
    void rcvMSGThread();
    void sndMSGThread();

private :
    std::unique_ptr<std::thread> rcvMSGThread_;
    std::unique_ptr<std::thread> sndMSGThread_;
    MessageQueue<std::shared_ptr<std::pair<ic::ServerInfo, ic::IC_MSG>>> queRcvMSG_;
    MessageQueue<std::shared_ptr<std::pair<ic::ServerInfo, ic::IC_MSG>>> queSndMSG_;

    std::shared_ptr<DBManager> db_manager_;
    std::unique_ptr<MessageSender> msg_sender_;

    bool isRMSGThread_;
    bool isSMSGThread_;
};