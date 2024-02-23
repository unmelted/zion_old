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

#include "ic_define.h"
#include <iostream>
#include <string>
#include <sstream>
#include <utility>

using namespace rapidjson;

static std::array<std::string, static_cast<int>(ic::DB_LOG_COLUMN::DB_LOG_COLUMN_SIZE)> DB_LOG_COLUMN_NAME =
{
    "date", "pid", "tid" ,"level", "file", "msg",
};

static std::array<std::string, static_cast<int>(ic::DB_EVENT_HISTORY_COLUMN::DB_EVENT_HISTORY_COLUMN_SIZE)> DB_EVENT_HISTORY_COLUMN_NAME =
{
    "type" ,"command", "subcommand", "action", "token", "from_where", "to_where", "data",
};

static std::array<std::string, static_cast<int>(ic::DB_LOG_MONITOR_COLUMN::DB_LOG_MONITOR_COLUMN_SIZE)> DB_LOG_MONITOR_COLUMN_NAME =
{
    "from_where", "data",
};

static std::array<std::string, static_cast<int>(ic::DB_ERROR_COLUMN::DB_ERROR_COLUMN_SIZE)> DB_ERROR_COLUMN_NAME =
{
    "id", "from_", "where_", "message", "etc",
};


class QueryMaker
{
public:
    static std::pair<string, string> makeInsertProtocolQuery(const Document& doc, const vector<string>& columnNames);
    static std::string makeLogMonitorInsertQuery(std::string& table, const Document& doc);
    static std::string makeEventInsertQuery(std::string& table, const Document& doc);
    static std::string makeLogInsertQuery(const std::string& tableName,
                                          const std::vector<std::pair<std::string, std::string>>& columns);
    static std::string makeErrorMsgQuery(const Document& doc);
};
