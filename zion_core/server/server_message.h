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

#include "socket_message.h"
#include "task_manager.h"
#include "db_manager.h"
#include "ic_server.h"
#include "socket_abstraction.h"

class SeverMsgManager : public SocketMsgManager
{
public:
    SeverMsgManager();
    ~SeverMsgManager();
    void setDBManagerForMonitor(std::shared_ptr<DBManager>& dbManager);
    void insertLogMonitorTable(const Document& doc);

private:
    std::shared_ptr<DBManager> db_manager_monitor;

};