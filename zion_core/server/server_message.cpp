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
#include "server_message.h"

using namespace rapidjson;

SeverMsgManager::SeverMsgManager()
: SocketMsgManager()
{

}

SeverMsgManager::~SeverMsgManager()
{

}

void SeverMsgManager::setDBManagerForMonitor(std::shared_ptr<DBManager>& dbManager)
{
    db_manager_monitor = dbManager;
}

void SeverMsgManager::insertLogMonitorTable(const Document& doc)
{
    std::string table = db_manager_monitor->getTableNameOfTcpLog();
    std::string query = QueryMaker::makeLogMonitorInsertQuery(table, doc);
    db_manager_monitor->enqueueQuery(query);
}